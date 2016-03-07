// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "RotationSwitch.h"
#include "ForwardRotationComponent.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API UForwardRotationComponent : public UPawnMovementComponent, public RotationSwitch
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float smoothClimbFactor = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bTraceVisibilty = false;
private:
	float backScanArmLenght = 200.0f;
	RotationSwitch* left;
	RotationSwitch* right;
	bool flag = true;
	RayProvider backProvider;
	RayProvider frontProvider;
	FRotator backNewRotation, frontNewRotation;
	float counter;
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	FHitResult GetFrontRotationRayHit();
	FRotator GetFrontRayRotation();
	FVector GetFrontRayBegin();
	FVector GetFrontScanArm(FVector startLocation);
	void SetFrontScanRay(RayProvider provider);
	FHitResult GetBackRotationRayHit();
	FRotator GetBackRayRotation();
	FVector GetBackRayBegin();
	FVector GetBackScanArm(FVector startLocation);
	void SetBackScanRay(RayProvider provider);
	void SetLeftComponent(RotationSwitch& componentRef);
	void SetRightComponent(RotationSwitch& componentRef);
};
