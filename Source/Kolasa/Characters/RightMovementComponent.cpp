// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Libraries/RunnerMath.h"
#include "RightMovementComponent.h"

void URightMovementComponent::BeginPlay() {
	Direction = FVector::ZeroVector;
	counter = 0.0f;
	ActivateMove();
}

void URightMovementComponent::Move(FVector value, float DeltaTime) {

	if (IsActiveMove()) {
		FVector DesiredMovementThisFrame = ConsumeInputVector().ClampMaxSize(1.0f)* ForwardFactor * DeltaTime + value;
		if (!DesiredMovementThisFrame.IsNearlyZero()) {
			SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, CollisionHit);
		}
	}

	FVector currentLocation = GetRayBegin();
	FRotator currentRotation = GetRayRotation();
	FVector forwardVector = UKismetMathLibrary::GetForwardVector(currentRotation);
	FVector scanArm = currentLocation + forwardVector * scanArmLenght;

	TArray<AActor*> ignore;
	UKismetSystemLibrary::LineTraceSingle_NEW(this, currentLocation, scanArm, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ignore, EDrawDebugTrace::ForDuration, RayHit, true);

	//if (RayHit.IsValidBlockingHit())
		//RotateOrtogonalToPlane(RayHit);
	SmoothRotateToPlane(RayHit, DeltaTime);
}

void URightMovementComponent::SmoothRotateToPlane(FHitResult & InHit, float DeltaTime) {

	FRotator clampedCurrent = UpdatedComponent->GetComponentRotation();
	if (InHit.IsValidBlockingHit()) {
		DeactivateMove();
		_downMovement->DeactivateMove();
		newRotation = GetOrtogonalToPlane(InHit);
	}

	if (newRotation != FRotator::ZeroRotator) {
		float end;
		if (newRotation.Roll > 0.0f && clampedCurrent.Roll < 0.0f) {
			countingDirection = FMath::Sign(-FMath::Abs(clampedCurrent.Roll+newRotation.Roll))*DeltaTime*smoothClimbFactor;
			end = FMath::Abs(-FMath::Abs(clampedCurrent.Roll + newRotation.Roll));
		}
		else {
			countingDirection = FMath::Sign(newRotation.Roll - clampedCurrent.Roll)*DeltaTime*smoothClimbFactor;
			end = FMath::Abs(newRotation.Roll - clampedCurrent.Roll);
		}

		if (FMath::Abs(counter) < end) {
			UpdatedComponent->SetRelativeRotation(FRotator(newRotation.Pitch, newRotation.Yaw, clampedCurrent.Roll + counter));
			counter += countingDirection;
		}
		else
		{
			counter = 0.0f;
			ActivateMove();
			_downMovement->ActivateMove();
		}
	}
}

void URightMovementComponent::UpdateDirection(FRotator rotation) {
	Direction = rotation.Vector();
	Direction.Normalize();
}

void URightMovementComponent::RotateOrtogonalToPlane(FHitResult & InHit) {
	FRotator newRotation = GetOrtogonalToPlane(InHit);		
	//UpdateDirection(newRotation);
	UpdatedComponent->SetRelativeRotation(newRotation);
}

FVector URightMovementComponent::GetDisplacement(float DeltaTime) {
	return Direction * DeltaTime * ForwardFactor;
}


FRotator URightMovementComponent::GetOrtogonalToPlane(FHitResult & InHit) {
	FVector normalToPlane = RunnerMath::GetCleared(InHit.ImpactNormal, 0.01f);
	FVector rightActor = RunnerMath::GetCleared(UpdatedComponent->GetRightVector(), 0.01f);
	FVector upActor = RunnerMath::GetCleared(UpdatedComponent->GetUpVector(), 0.01f);
	
	FVector crossRightAndUp = RunnerMath::GetCleared(UKismetMathLibrary::Cross_VectorVector(rightActor, upActor), 0.01f);
	FVector newRight = RunnerMath::GetCleared(UKismetMathLibrary::Cross_VectorVector(normalToPlane, crossRightAndUp), 0.01f);
	FVector newForward = crossRightAndUp;
	FVector newUp = RunnerMath::GetCleared(normalToPlane, 0.01f);

	return UKismetMathLibrary::MakeRotationFromAxes(newForward, newRight, newUp);
}

void URightMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URightMovementComponent::SetDown(IBlockable* down) {
	_downMovement = down;
}