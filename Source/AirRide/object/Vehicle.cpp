// Fill out your copyright notice in the Description page of Project Settings.

#include "AirRide.h"
#include "Vehicle.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AVehicle::AVehicle()
{
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = RootScene;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    HoverUnitRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HoverUnitRoot"));
    HoistStabilizer = CreateDefaultSubobject<USceneComponent>(TEXT("HoistStabilizer"));
    /*
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Engine/BasicShapes/Sphere"));
    
    Mesh->SetStaticMesh(SphereVisualAsset.Object);
    */
    Mesh->SetSimulatePhysics(true);
    Mesh->SetupAttachment(RootComponent);

    HoverUnitRoot->SetupAttachment(Mesh);
    HoistStabilizer->SetupAttachment(Mesh);

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //test
    BaseStats.boost = 15.0f;
    BaseStats.topspeed = 1000.0f;
    BaseStats.weight = 1.0f;




}

void AVehicle::SetupHoverUnit(int32 num) {
    for (int32 i = 0; i < num; i++) {
        FString name = "HoverUnit" + FString::FromInt(i);
        USceneComponent* hp = CreateDefaultSubobject<USceneComponent>(*name);
        hp->SetupAttachment(HoverUnitRoot);
        this->FinishAndRegisterComponent(hp);
        HoverUnits.Add(hp);
    }
}
void AVehicle::TickStabilizer() {
    FVector HoistStabilizeOrigin = Mesh->GetCenterOfMass() + FVector{ 0.0f, 0.0f, 50.0f };
    FVector stbSpd = Mesh->GetPhysicsLinearVelocityAtPoint(HoistStabilizer->GetComponentLocation())-Mesh->GetComponentVelocity();
    Mesh->AddForceAtLocation(100.0f*(HoistStabilizeOrigin-HoistStabilizer->GetComponentLocation())-25.0f*stbSpd, HoistStabilizer->GetComponentLocation());
}
bool AVehicle::TickHover() {
    const int32 count = HoverUnits.Num();
    TArray<FHitResult> GroundHits;
    TArray<bool> Hits;
    bool IsHitAll=true;
    GroundHits.SetNum(count); Hits.SetNum(count);
    int32 hitcount = 0;
    
    for (int32 i = 0; i < count; i++) {
        USceneComponent* hp = HoverUnits[i];
        const FVector HitStart = hp->GetComponentLocation();
        const FVector HitEnd = HitStart - 60.0f*hp->GetUpVector();
        bool hit = GetWorld()->LineTraceSingleByChannel(GroundHits[i], HitStart, HitEnd, ECC_WorldStatic);
        DrawDebugLine(
            GetWorld(),
            HitStart,
            HitEnd,
            FColor(255, 0, 0),
            false, -1, 0,
            12.333
        );
        DrawDebugSphere(
            GetWorld(),
            HitStart,
            24,
            32,
            FColor(255, 0, 0)
        );
        IsHitAll = IsHitAll&&hit;
        if (hit) {
            float flForce = 1000 * (30.0f - GroundHits[i].Distance);
            flForce = flForce < 0 ? flForce*0.1f : flForce;
            FVector hpSpd = Mesh->GetPhysicsLinearVelocityAtPoint(hp->GetComponentLocation());
            //float vgvvg = FVector::DotProduct(GroundHits[i].Normal, hpSpd);
           // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("normspeed:%f vel:%f"), vgvvg, hpSpd.Size()));
            FVector velFromGr = FVector::DotProduct(GroundHits[i].Normal, hpSpd)*GroundHits[i].Normal;
            Mesh->AddForceAtLocation(flForce*GroundHits[i].Normal - 100.0f*velFromGr, hp->GetComponentLocation());
        }
        else {
            Mesh->AddForceAtLocation(1000.0f*hp->GetUpVector(), hp->GetComponentLocation());
        }

        
    }
    return IsHitAll;

        //Mesh->AddForce(-1000.0f*FVector::DotProduct(Mesh->GetUpVector(), Mesh->GetComponentVelocity().GetSafeNormal())*Mesh->GetComponentVelocity());

}
// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVehicle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


    if(!TickHover())
    TickStabilizer();
    //Mesh->AddForce(- 1.0f*Mesh->GetComponentVelocity());
    //test
    /*
    const FVector fwdVec = RootComponent->GetForwardVector();
    const float currVel = Mesh->GetComponentVelocity().Size();
    const float force = BaseStats.boost*(BaseStats.topspeed - currVel);
    //if(this->IsPlayerControlled())
    Mesh->AddForce(force*fwdVec);

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("topspeed:%f boost:%f currentspeed:%f"), BaseStats.topspeed,BaseStats.boost,currVel));
    */
    /*
    FHitResult GroundHit;
    const FVector HitStart = BottomPos->GetComponentLocation();
    const FVector HitEnd = HitStart - 40.0f*Mesh->GetUpVector();
    //const FCollisionObjectQueryParams coqp(ECC_TO_BITFIELD(ECC_WorldStatic));
    bool TouchGround = GetWorld()->LineTraceSingleByObjectType(GroundHit, HitStart, HitEnd, ECC_TO_BITFIELD(ECC_WorldStatic));
    if (TouchGround) {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("!touch! torq:%f"), FVector::CrossProduct(Mesh->GetUpVector(), GroundHit.Normal).Size()));
        const float flForce = 100*(30.0f- GroundHit.Distance);
        Mesh->AddForce(flForce*GroundHit.Normal-50.0f*Mesh->GetComponentVelocity()*Mesh->GetUpVector());
        Mesh->AddTorque(100*FVector::CrossProduct(Mesh->GetUpVector(), GroundHit.Normal).GetSafeNormal()*(1.0f-FVector::DotProduct(Mesh->GetUpVector(),GroundHit.Normal)));
    }
    */
}

// Called to bind functionality to input
void AVehicle::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

