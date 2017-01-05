// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "object/Vehicle.h"
#include "TestVehicle1.generated.h"

/**
 * 
 */
UCLASS()
class AIRRIDE_API ATestVehicle1 : public AVehicle
{
    GENERATED_BODY()
public:
        ATestVehicle1();
        virtual void Tick(float DeltaSeconds) override;
	
	
	
	
};
