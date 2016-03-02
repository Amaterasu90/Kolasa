// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/RotationMovementComponent.h"
#include "RightMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API URightMovementComponent : public URotationMovementComponent
{
	GENERATED_BODY()
	
public:
	virtual bool IsReadyToEnableScanRotation(FVector right, FVector sideDirection) override;
	virtual float CalcIterationStep(float oldRoll, float newRoll, float deltaTime) override;
	virtual float CalcEndIteration(float oldRoll, float newRoll) override;
};
