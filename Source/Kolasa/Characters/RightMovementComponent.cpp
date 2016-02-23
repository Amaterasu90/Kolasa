// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Libraries/RunnerMath.h"
#include "RightMovementComponent.h"

void URightMovementComponent::BeginPlay() {
	ActivateMove();
	ActivateRotation();
	counter = 0.0f;
	lastHitLocation = GetRayBegin();
}

void URightMovementComponent::ActivateMove() {
	if (!MoveSwitch::IsActive()) {
		MoveSwitch::Activate();
		UE_LOG(ER_Log, Display, TEXT("Activate Move RightMovementComponent"));
	}
}

void URightMovementComponent::DeactivateMove() {
	if (MoveSwitch::IsActive()) {
		MoveSwitch::Deactivate();
		UE_LOG(ER_Log, Display, TEXT("Deactivate Move RightMovementComponent"));
	}
}

void URightMovementComponent::ActivateRotation(){
	if (!RotationSwitch::IsActive()) {
		RotationSwitch::Activate();
		UE_LOG(ER_Log, Display, TEXT("Activate Rotation RightMovementComponent"))
	}
}

void URightMovementComponent::DeactivateRotation(){
	if (RotationSwitch::IsActive()) {
		RotationSwitch::Deactivate();
		UE_LOG(ER_Log, Display, TEXT("Deactivate Rotation RightMovementComponent"));
	}
}

void URightMovementComponent::Move(FVector value, float DeltaTime) {

	if (MoveSwitch::IsActive()) {
		FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f)* ForwardFactor * DeltaTime + value;
		if (!DesiredMovementThisFrame.IsNearlyZero()) {
			FHitResult hit;
			SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, hit);
		}
	}
	if (RotationSwitch::IsActive()) {
		FHitResult hit = GetRayHit();
		SmoothRotateToPlane(hit, DeltaTime);
	}
}

void URightMovementComponent::SmoothRotateToPlane(FHitResult & InHit, float DeltaTime) {
	FRotator clampedCurrent = UpdatedComponent->GetComponentRotation();

	if (InHit.IsValidBlockingHit()) {
		DeactivateMove();
		_downMovement->Deactivate();
		_leftRotation->Deactivate();
		newRotation = GetOrtogonalToPlane(InHit);
	}

	float end = CalcEndIteration(clampedCurrent.Roll, newRotation.Roll);
	if (FMath::Abs(counter) < end && newRotation != FRotator::ZeroRotator) {
		UpdatedComponent->SetRelativeRotation(FRotator(newRotation.Pitch, newRotation.Yaw, clampedCurrent.Roll + counter));
		counter += CalcIterationStep(clampedCurrent.Roll, newRotation.Roll, DeltaTime);
	}
	else if (counter != 0.0f) {
		UpdatedComponent->SetRelativeRotation(newRotation);
		ActivateMove();
		_downMovement->Activate();
		counter = 0.0f;
		newRotation = FRotator::ZeroRotator;
		_leftRotation->bIsEndSmoothRotation = true;
	}

	if (_leftRotation->bIsEndSmoothRotation)
	{
		FHitResult hitLeft = _leftHit->GetRayHit();
		if (!hitLeft.IsValidBlockingHit())
		{
			_leftRotation->Activate();
			_leftRotation->bIsEndSmoothRotation = false;
		}
	}
}

float URightMovementComponent::CalcIterationStep(float oldRoll, float newRoll, float deltaTime) {
	if (newRoll > 0.0f && oldRoll < 0.0f) {
		return FMath::Sign(-FMath::Abs(oldRoll + newRoll))*deltaTime*smoothClimbFactor;
	}
	else {
		return FMath::Sign(newRoll - oldRoll)*deltaTime*smoothClimbFactor;
	}
	return 0.0f;
}

float URightMovementComponent::CalcEndIteration(float oldRoll, float newRoll){
	if (newRoll > 0.0f && oldRoll < 0.0f) {
		return FMath::Abs(-FMath::Abs(oldRoll + newRoll));
	}
	else {
		return FMath::Abs(newRoll - oldRoll);
	}
	return 0.0f;
}

void URightMovementComponent::UpdateDirection(FRotator rotation) {
	
}

void URightMovementComponent::RotateOrtogonalToPlane(FHitResult & InHit) {
	FRotator newRotation = GetOrtogonalToPlane(InHit);		
	UpdatedComponent->SetRelativeRotation(newRotation);
}

FVector URightMovementComponent::GetDisplacement(float DeltaTime) {
	return FVector::ZeroVector;
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

void URightMovementComponent::SetDown(MoveSwitch* down) {
	_downMovement = down;
}

void URightMovementComponent::SetLeft(RotationSwitch * left){
	_leftRotation = left;
}

void URightMovementComponent::SetLeftHit(IHitable * hit){
	_leftHit = hit;
}
