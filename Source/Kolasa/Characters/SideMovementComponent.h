// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "MoveSwitch.h"
#include "ISideDirectionMovement.h"
#include "SideMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API USideMovementComponent : public UPawnMovementComponent, public MoveSwitch, public ISideDirectionMovement
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcces = "true"))
	float MoveFactor = 500.0f;
	FVector DesiredMovementThisFrame;
public:
	virtual FVector GetDirection() override;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
