// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/DirectionMovementComponent.h"
#include "Characters/RotationSwitch.h"
#include "Characters/MoveSwitch.h"
#include "Characters/IHitable.h"
#include "RotationMovementComponent.generated.h"

/**
 * 
 */
UCLASS(abstract)
class KOLASA_API URotationMovementComponent : public UPawnMovementComponent, public RotationSwitch, public MoveSwitch, public IHitable
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float scanArmLenght = 100.0f;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual FHitResult GetRayHit();
	void SetForwardFactor(float value);
	void SetScanRay(RayProvider provider);
protected:
	FRotator GetRayRotation();
	float ForwardFactor;
	FVector Direction;
	FVector GetRayBegin();
	virtual void Move(FVector value, float DeltaTime);
	virtual void RotateOrtogonalToPlane(FHitResult & OutHit) PURE_VIRTUAL(UDirectionMovementComponent::RotateOrtogonalToPlane, ;);
	virtual FVector GetDisplacement(float DeltaTime);
	FVector GetScanArm(FVector startLocation);
private:
	RayProvider _provider;
};
