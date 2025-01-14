// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/DirectionMovementComponent.h"
#include "Characters/RotationSwitch.h"
#include "ForwardMovementComponent.h"
#include "GravityMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API UGravityMovementComponent : public UDirectionMovementComponent
{
	GENERATED_BODY()
private:
	MoveSwitch* sideMovement;
	RotationSwitch* leftRotation;
	RotationSwitch* rightRotation;
	MoveSwitch* forwardMovement;
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void SetBlockSide(MoveSwitch& side);
	void SetBlockLeft(RotationSwitch& left);
	void SetBlockRight(RotationSwitch& right);
	void SetBlockForward(MoveSwitch& forward);
protected:
	void Move(FVector value);
private:
	void ManageBlockMove(FHitResult& result);
	void UpdateDirection();
	//forwardSwitch
};
