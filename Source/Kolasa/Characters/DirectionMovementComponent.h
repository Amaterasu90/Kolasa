// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "DirectionMovementComponent.generated.h"

/**
 * 
 */
UCLASS(abstract)
class KOLASA_API UDirectionMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void SetForwardFactor(float value);
protected:
	float ForwardFactor;
	FVector Direction;
	FHitResult Hit;
	virtual void RotateOrtogonalToPlane(FHitResult & OutHit) PURE_VIRTUAL(UDirectionMovementComponent::RotateOrtogonalToPlane, ;);
	virtual void Move(FVector value);
	virtual FVector GetDisplacement(float DeltaTime);
};
