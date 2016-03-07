// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "SkeletalOrientationComponent.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API USkeletalOrientationComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
private:
	float LastControlInputAxis;
	float ControlInputAxis;
	FRotator rotationBalance;
	
public:
	UPROPERTY(EditAnywhere)
	float leftDeviation = 45.0f;
	UPROPERTY(EditAnywhere)
	float rightDeviation = 45.0f;
	UPROPERTY(EditAnywhere)
	float SkeletalRotationFactor = 1.0f;
	float counter;
public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void AddInputAxis(float accelerationAxis);
	virtual float ConsumeAxis();
	virtual void BeginPlay() override;
	void SetRotationBalance(FRotator rotationBalance);
};
