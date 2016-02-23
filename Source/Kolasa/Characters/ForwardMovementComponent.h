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
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float smoothClimbFactor = 1.0f;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void SetDown(MoveSwitch* down);
	void SmoothRotateToPlane(FHitResult& InHit, float DeltaTime);
	void UpdateDirection(FRotator rotation);
	void DeactivateMove();
	void ActivateMove();
protected:
	virtual void Move(FVector value, float DeltaTime) override;
	virtual void RotateOrtogonalToPlane(FHitResult & InHit) override;
	virtual FVector GetDisplacement(float DeltaTime) override;
private:
	FRotator GetOrtogonalToPlane(FHitResult& InHit);
	MoveSwitch* _downMovement;
	FRotator newRotation;
	float counter;
	float countingDirection;
};
