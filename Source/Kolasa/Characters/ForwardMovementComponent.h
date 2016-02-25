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
private:
	MoveSwitch* downMovement;
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void SetBlockDown(MoveSwitch& down);
	void DeactivateMove();
	void ActivateMove();
protected:
	virtual void Move(FVector value);
};
