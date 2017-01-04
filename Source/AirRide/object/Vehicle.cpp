// Fill out your copyright notice in the Description page of Project Settings.

#include "AirRide.h"
#include "Vehicle.h"


// Sets default values
AVehicle::AVehicle()
{
    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = RootScene;
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Engine/BasicShapes/Sphere"));
    Mesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetStaticMesh(SphereVisualAsset.Object);
    Mesh->SetSimulatePhysics(true);
    Mesh->SetupAttachment(RootComponent);
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //test
    BaseStats.boost = 100.0f;
    BaseStats.topspeed = 1000.0f;
    BaseStats.weight = 1.0f;

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

    //test
    const FVector fwdVec = RootComponent->GetForwardVector();
    const float currVel = Mesh->GetComponentVelocity().Size();
    const float force = BaseStats.boost*(BaseStats.topspeed - currVel);
    //if(this->IsPlayerControlled())
    Mesh->AddForce(force*fwdVec);

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("topspeed:%f boost:%f currentspeed:%f"), BaseStats.topspeed,BaseStats.boost,currVel));
    

}

// Called to bind functionality to input
void AVehicle::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

