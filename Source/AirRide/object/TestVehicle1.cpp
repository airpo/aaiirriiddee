// Fill out your copyright notice in the Description page of Project Settings.

#include "AirRide.h"
#include "TestVehicle1.h"


// Sets default values
ATestVehicle1::ATestVehicle1()
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Engine/BasicShapes/Cube"));

    Mesh->SetStaticMesh(SphereVisualAsset.Object);
    int32 hnum = 4;
    SetupHoverUnit(hnum);
    float angl_offset = PI / 4.0f;
    float rad = 50.0f;
    for (int32 i = 0; i < hnum; i++) {
      //  FVector fv = { 0,0,0 };
        float sv, cv;
        FMath::SinCos(&sv, &cv, 2.0f*PI*float(i) / float(hnum)+angl_offset);
      //  fv[i] = 100;
        HoverUnits[i]->SetRelativeLocation({ rad*sv,rad*cv,-40.0f });
    }

    HoistStabilizer->SetRelativeLocation(Mesh->GetUpVector()*(50.0f));

}

void ATestVehicle1::Tick(float DeltaTime)
{

    Super::Tick(DeltaTime);
    
    const FVector fwdVec = RootComponent->GetForwardVector();
    const float currVel = Mesh->GetComponentVelocity().Size();
    const float force = BaseStats.boost*(BaseStats.topspeed - currVel);
    //if(this->IsPlayerControlled())
    Mesh->AddForceAtLocation(force*fwdVec,Mesh->GetCenterOfMass());

    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("topspeed:%f boost:%f currentspeed:%f"), BaseStats.topspeed,BaseStats.boost,currVel));
    
}

