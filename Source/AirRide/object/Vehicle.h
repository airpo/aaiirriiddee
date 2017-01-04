// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Vehicle.generated.h"
USTRUCT(BlueprintType)
struct FVehicleBaseStats
{
    GENERATED_BODY()

    UPROPERTY()
        float topspeed;

    UPROPERTY()
        float boost;

    UPROPERTY()
        float weight;
};

UCLASS()
class AIRRIDE_API AVehicle : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVehicle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    FVehicleBaseStats BaseStats;

    UPROPERTY(VisibleAnywhere)
        USceneComponent* RootScene;

    UPROPERTY(VisibleAnywhere)
        UStaticMeshComponent* Mesh;
	
	
};
