// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/DirectionMovementComponent.h"
#include "RightMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API URightMovementComponent : public UDirectionMovementComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float smoothClimbFactor = 1.0f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float scanArmLenght = 100.0f;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void SetDown(IBlockable* down);
	void SmoothRotateToPlane(FHitResult& InHit, float DeltaTime);
	void UpdateDirection(FRotator rotation);
	virtual void ActivateMove() override;
	virtual void DeactivateMove() override;
protected:
	virtual void Move(FVector value, float DeltaTime) override;
	virtual void RotateOrtogonalToPlane(FHitResult & InHit) override;
	virtual FVector GetDisplacement(float DeltaTime) override;
	FHitResult GetRayHit();
	FRotator GetOrtogonalToPlane(FHitResult& InHit);
private:
	IBlockable* _downMovement;
	float counter;
	float countingDirection;
	FRotator newRotation;
	float CalcEndIteration(float oldRoll, float newRoll);
	float CalcIterationStep(float oldRoll, float newRoll, float deltaTime);
};
