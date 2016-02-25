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
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float scanArmLenght = 100.0f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "10.0"))
	float smoothClimbFactor = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bTraceVisibilty = false;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	FHitResult GetRotationRayHit();
	void SetOppositeSiteInterface(RotationSwitch& opposite);
	void SetScanRay(RayProvider provider);
	void SetDown(MoveSwitch& down);
	void SmoothRotateToPlane(FHitResult& InHit, float DeltaTime);
	void SetSideMovementComponent(ISideDirectionMovement& sideComponent);
protected:
	FRotator GetOrtogonalToPlane(FHitResult& InHit);
	RotationSwitch*& GetOtherInterface();
	MoveSwitch*& GetDownInterface();
	FRotator GetRayRotation();
	FVector GetRayBegin();
	FVector GetScanArm(FVector startLocation);
private:
	virtual bool IsReadyToEnableScanRotation(FVector right, FVector sideDirection) PURE_VIRTUAL(URotationMovementComponent::IsReadyToEnableScanRotation, { return false; });
	RotationSwitch* _oppositeSiteRotation;
	MoveSwitch* _downMovement;
	ISideDirectionMovement* sideComponent;
	RayProvider _provider;
	float counter;
	float countingDirection;
	FRotator newRotation;
	FVector lastHitLocation;
	float CalcEndIteration(float oldRoll, float newRoll);
	float CalcIterationStep(float oldRoll, float newRoll, float deltaTime);
};
