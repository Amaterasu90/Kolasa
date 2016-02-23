// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Libraries/RunnerMath.h"
#include "LeftMovementComponent.h"

void ULeftMovementComponent::BeginPlay() {
	ActivateMove();
	ActivateRotation();
	counter = 0.0f;
}

void ULeftMovementComponent::ActivateMove() {
	if (!MoveSwitch::IsActive()) {
		MoveSwitch::Activate();
		UE_LOG(ER_Log, Display, TEXT("Activate Move LeftMovementComponent"));
	}
}

void ULeftMovementComponent::DeactivateMove() {
	if (MoveSwitch::IsActive()) {
		MoveSwitch::Deactivate();
		UE_LOG(ER_Log, Display, TEXT("Deactivate Move LeftMovementComponent"));
	}
}

void ULeftMovementComponent::ActivateRotation(){
	if (RotationSwitch::IsActive()) {
		RotationSwitch::Deactivate();
		UE_LOG(ER_Log, Display, TEXT("Activate Rotation LeftMovementComponent"));
	}
}

void ULeftMovementComponent::Move(FVector value, float DeltaTime) {

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

void ULeftMovementComponent::SmoothRotateToPlane(FHitResult& InHit, float DeltaTime) {

	FRotator clampedCurrent = UpdatedComponent->GetComponentRotation();

	if (InHit.IsValidBlockingHit()) {
		DeactivateMove();
		_downMovement->Deactivate();
		_rightMovement->Deactivate();
		newRotation = GetOrtogonalToPlane(InHit);
	}

	float end = CalcEndIteration(clampedCurrent.Roll, newRotation.Roll);
	if (FMath::Abs(counter) < end && newRotation != FRotator::ZeroRotator) {
		UpdatedComponent->SetRelativeRotation(FRotator(newRotation.Pitch, newRotation.Yaw, clampedCurrent.Roll + counter));
		counter += CalcIterationStep(clampedCurrent.Roll, newRotation.Roll, DeltaTime);;
	}
	else if (counter != 0.0f) {
		UpdatedComponent->SetRelativeRotation(newRotation);
		ActivateMove();
		_downMovement->Activate();
		counter = 0.0f;
		newRotation = FRotator::ZeroRotator;
		RotationSwitch::bIsEndSmoothRotation = true;
	}

	
	if (RotationSwitch::bIsEndSmoothRotation)
	{
		FHitResult hitRight = _rightHit->GetRayHit();
		if (!hitRight.IsValidBlockingHit())
		{
			_rightMovement->Activate();
			RotationSwitch::bIsEndSmoothRotation = false;
		}
	}
}

float ULeftMovementComponent::CalcIterationStep(float oldRoll, float newRoll, float deltaTime) {
	if (newRoll > 0.0f && oldRoll < 0.0f) {
		return FMath::Sign(-FMath::Abs(oldRoll + newRoll))*deltaTime*smoothClimbFactor;
	}
	else {
		return FMath::Sign(newRoll - oldRoll)*deltaTime*smoothClimbFactor;
	}
	return 0.0f;
}

float ULeftMovementComponent::CalcEndIteration(float oldRoll, float newRoll) {
	if (newRoll > 0.0f && oldRoll < 0.0f) {
		return FMath::Abs(-FMath::Abs(oldRoll + newRoll));
	}
	else {
		return FMath::Abs(newRoll - oldRoll);
	}
	return 0.0f;
}

void ULeftMovementComponent::UpdateDirection(FRotator rotation) {
	
}

void ULeftMovementComponent::RotateOrtogonalToPlane(FHitResult & InHit) {
	FRotator newRotation = GetOrtogonalToPlane(InHit);
	UpdatedComponent->SetRelativeRotation(newRotation);
}

FVector ULeftMovementComponent::GetDisplacement(float DeltaTime) {
	return FVector::ZeroVector;
}


FRotator ULeftMovementComponent::GetOrtogonalToPlane(FHitResult& InHit) {
	FVector normalToPlane = RunnerMath::GetCleared(InHit.ImpactNormal, 0.01f);
	FVector rightActor = RunnerMath::GetCleared(UpdatedComponent->GetRightVector(), 0.01f);
	FVector upActor = RunnerMath::GetCleared(UpdatedComponent->GetUpVector(), 0.01f);

	FVector crossRightAndUp = RunnerMath::GetCleared(UKismetMathLibrary::Cross_VectorVector(rightActor, upActor), 0.01f);
	FVector newRight = RunnerMath::GetCleared(UKismetMathLibrary::Cross_VectorVector(normalToPlane, crossRightAndUp), 0.01f);
	FVector newForward = crossRightAndUp;
	FVector newUp = RunnerMath::GetCleared(normalToPlane, 0.01f);

	return UKismetMathLibrary::MakeRotationFromAxes(newForward, newRight, newUp);
}

void ULeftMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULeftMovementComponent::SetDown(MoveSwitch* down) {
	_downMovement = down;
}

void ULeftMovementComponent::SetRight(RotationSwitch * right){
	_rightMovement = right;
}

void ULeftMovementComponent::SetHitRight(IHitable * hit){
	this->_rightHit = hit;
}


