// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "ForwardMovementComponent.h"
#include "ForwardRotationComponent.generated.h"

/**
 * 
 */
UCLASS()
class KOLASA_API UForwardRotationComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
		UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1000.0"))
		float scanArmLenght = 100.0f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "10.0"))
		float smoothClimbFactor = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Debug")
		bool bTraceVisibilty = false;
private:
	UForwardMovementComponent* forwardComponent;
	RayProvider _provider;
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	FHitResult GetRotationRayHit();
	FRotator GetRayRotation();
	FVector GetRayBegin();
	FVector GetScanArm(FVector startLocation);
	void SetScanRay(RayProvider provider);
	void SetForwardComponent(UForwardMovementComponent& forward);
	FRotator currentRotation;
	FVector forwardVector;
};
