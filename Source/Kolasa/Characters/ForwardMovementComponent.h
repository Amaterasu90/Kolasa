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
	UPROPERTY(EditAnywhere)
	float smoothClimbFactor = 0.1f;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void SetDown(IBlockable* down);
	void SmoothRotateToPlane(FHitResult& InHit);
protected:
	virtual void Move(FVector value) override;
	virtual void RotateOrtogonalToPlane(FHitResult & InHit) override;
	virtual FVector GetDisplacement(float DeltaTime) override;
private:
	FRotator GetOrtogonalToPlane(FHitResult& InHit);
	IBlockable* _downMovement;
	FRotator newRotation;
	float counter;
	float countingDirection;
};
