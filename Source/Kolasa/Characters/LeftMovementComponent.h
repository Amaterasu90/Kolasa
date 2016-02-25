// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/RotationMovementComponent.h"
#include "LeftMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API ULeftMovementComponent : public URotationMovementComponent
{
	GENERATED_BODY()
public:
	virtual bool IsReadyToEnableScanRotation(FVector right, FVector sideDirection) override;
};
