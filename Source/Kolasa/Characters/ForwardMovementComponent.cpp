// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Libraries/RunnerMath.h"
#include "ForwardMovementComponent.h"

void UForwardMovementComponent::BeginPlay() {
	Direction = UpdatedComponent->GetForwardVector();
	counter = 0.0f;
	ActivateMove();
}

void UForwardMovementComponent::Move(FVector value, float DeltaTime){

	if (IsActiveMove()) {
		if (!value.IsNearlyZero()) {
			SafeMoveUpdatedComponent(value, UpdatedComponent->GetComponentRotation(), true, CollisionHit);
		}
	}

	FVector currentLocation = GetRayBegin();
	FRotator currentRotation = GetRayRotation();
	FVector forwardVector = UKismetMathLibrary::GetForwardVector(currentRotation);
	FVector scanArm = currentLocation + forwardVector * scanArmLenght;

	TArray<AActor*> ignore;
	UKismetSystemLibrary::LineTraceSingle_NEW(this, currentLocation, scanArm, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ignore, EDrawDebugTrace::ForDuration, RayHit, true);
	
	SmoothRotateToPlane(RayHit,DeltaTime);
}

void UForwardMovementComponent::SmoothRotateToPlane(FHitResult & InHit, float DeltaTime) {
	
	FRotator clampedCurrent = UpdatedComponent->GetComponentRotation();
	if (InHit.IsValidBlockingHit()) {
		DeactivateMove();
		_downMovement->DeactivateMove();
		newRotation = GetOrtogonalToPlane(InHit);
		countingDirection = FMath::Sign(newRotation.Pitch - clampedCurrent.Pitch)*DeltaTime*smoothClimbFactor;
		UpdateDirection(newRotation);
	}
	
	if (FMath::Abs(counter) < FMath::Abs(newRotation.Pitch - clampedCurrent.Pitch)) {
		UpdatedComponent->SetRelativeRotation(FRotator(clampedCurrent.Pitch + counter, newRotation.Yaw, newRotation.Roll));
		counter += countingDirection;
	}
	else
	{
		counter = 0.0f;
		ActivateMove();
		_downMovement->ActivateMove();
	}
}

void UForwardMovementComponent::UpdateDirection(FRotator rotation) {
	Direction = rotation.Vector();
	Direction.Normalize();
}

void UForwardMovementComponent::RotateOrtogonalToPlane(FHitResult & InHit) {
	FRotator newRotation = GetOrtogonalToPlane(InHit);

	Direction = newRotation.Vector();
	Direction.Normalize();
	UpdatedComponent->SetRelativeRotation(newRotation);
}

FVector UForwardMovementComponent::GetDisplacement(float DeltaTime) {
	return Direction * DeltaTime * ForwardFactor;
}


FRotator UForwardMovementComponent::GetOrtogonalToPlane(FHitResult & InHit){
	FVector normalToPlane = RunnerMath::GetCleared(InHit.ImpactNormal,0.01f);
	FVector forwardActor = RunnerMath::GetCleared(UpdatedComponent->GetForwardVector(),0.01f);
	FVector upActor = RunnerMath::GetCleared(UpdatedComponent->GetUpVector(),0.01f);

	FVector crossForwardAndUp = RunnerMath::GetCleared(UKismetMathLibrary::Cross_VectorVector(forwardActor, upActor),0.01f);
	FVector newForward = RunnerMath::GetCleared(UKismetMathLibrary::Cross_VectorVector(normalToPlane, crossForwardAndUp),0.01f);
	FVector newRight = RunnerMath::GetCleared(UKismetMathLibrary::Cross_VectorVector(normalToPlane, newForward),0.01f);

	FVector newUp = RunnerMath::GetCleared(normalToPlane,0.01f);

	return UKismetMathLibrary::MakeRotationFromAxes(newForward, newRight, newUp);
}

void UForwardMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UForwardMovementComponent::SetDown(IBlockable* down){
	_downMovement = down;
}


