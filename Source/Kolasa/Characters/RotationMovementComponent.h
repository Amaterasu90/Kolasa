// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Characters/DirectionMovementComponent.h"
#include "Characters/RotationSwitch.h"
#include "Characters/MoveSwitch.h"
#include "ISideDirectionMovement.h"
#include "RotationMovementComponent.generated.h"

/**
 * 
 */
UCLASS(abstract)
class KOLASA_API URotationMovementComponent : public UPawnMovementComponent, public RotationSwitch
{
	GENERATED_BODY()

public:
	FVector normalToPlane, rightActor, upActor, crossRightAndUp, newRight, newForward, newUp;
	float end;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float scanArmLenght = 100.0f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float smoothClimbFactor = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bTraceVisibilty = false;
private:
	RotationSwitch* _oppositeSiteRotation;
	ISideDirectionMovement* sideComponent;
	RayProvider _provider;
	FVector lastHitLocation;
	RotationSwitch* downInterface;
protected:
	float counter;
	float countingDirection;
	float stepCounting;
	FRotator clampedCurrent;
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	FHitResult GetRotationRayHit();
	void SetDownSwitchRotationInterface(RotationSwitch& down);
	void SetOppositeSiteInterface(RotationSwitch& opposite);
	void SetScanRay(RayProvider provider);
	void SmoothRotateToPlane(FHitResult& InHit, float DeltaTime);
	void SetSideMovementComponent(ISideDirectionMovement& sideComponent);
protected:
	FRotator GetOrtogonalToPlane(FHitResult& InHit);
	RotationSwitch*& GetOtherInterface();
	RotationSwitch*& GetDownSwitchRotationInterface();
	FRotator GetRayRotation();
	FVector GetRayBegin();
	FVector GetScanArm(FVector startLocation);
private:
	virtual bool IsReadyToEnableScanRotation(FVector right, FVector sideDirection) PURE_VIRTUAL(URotationMovementComponent::IsReadyToEnableScanRotation, { return false; });
	virtual float CalcEndIteration(float oldRoll, float newRoll) PURE_VIRTUAL(URotationMovementComponent::CalcEndIteration, { return 0.0f; });
	virtual float CalcIterationStep(float oldRoll, float newRoll, float deltaTime) PURE_VIRTUAL(URotationMovementComponent::CalcIterationStep, { return 0.0f; });
	void CalcNewRotation(FHitResult& hit, RotationSwitch& down, RotationSwitch& otherSite);
	virtual void SmoothRotate(float DeltaTime, RotationSwitch& down) PURE_VIRTUAL(URotationMovementComponent::SmoothRotate, {};);
	void FinalizeRotate(bool isReady);
};
