// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/DirectionMovementComponent.h"
#include "ForwardMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API UForwardMovementComponent : public UDirectionMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void SetDown(IBlockable* down);
protected:
	virtual void Move(FVector value) override;
	virtual void RotateOrtogonalToPlane(FHitResult & OutHit) override;
	virtual FVector GetDisplacement(float DeltaTime) override;
private:
	IBlockable* downMovement;
};
