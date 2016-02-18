// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "IBlockable.h"
#include "RayProvider.h"
#include "DirectionMovementComponent.generated.h"

/**
 * 
 */
UCLASS(abstract)
class KOLASA_API UDirectionMovementComponent : public UPawnMovementComponent, public IBlockable
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1000.0"))
	float scanArmLenght = 100.0f;
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	void SetForwardFactor(float value);
	void SetScanRay(RayProvider provider);
	FHitResult GetRayHit();
protected:
	FVector GetRayBegin();
	FRotator GetRayRotation();
	float ForwardFactor;
	FVector Direction;
	FHitResult CollisionHit;
	virtual void RotateOrtogonalToPlane(FHitResult & OutHit) PURE_VIRTUAL(UDirectionMovementComponent::RotateOrtogonalToPlane, ;);
	virtual void Move(FVector value, float DeltaTime);
	virtual FVector GetDisplacement(float DeltaTime);
private:
	RayProvider _provider;
};
