// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Libraries/RunnerMath.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "RotationMovementComponent.h"

void URotationMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		return;
	}

	FVector DesiredMovementThisFrame = GetDisplacement(DeltaTime);
	Move(DesiredMovementThisFrame, DeltaTime);
}

FHitResult URotationMovementComponent::GetRayHit() {
	FVector currentLocation = GetRayBegin();
	FVector scanArm = GetScanArm(currentLocation);

	TArray<AActor*> ignore;
	FHitResult result;
	UKismetSystemLibrary::LineTraceSingle_NEW(this, currentLocation, scanArm, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ignore, EDrawDebugTrace::ForDuration, result, true);

	return result;
}

void URotationMovementComponent::SetForwardFactor(float value) {
	this->ForwardFactor = value;
}

void URotationMovementComponent::SetScanRay(RayProvider provider) {
	_provider = provider;
}

FRotator URotationMovementComponent::GetRayRotation() {
	return _provider.GetRotation();
}

FVector URotationMovementComponent::GetRayBegin() {
	return _provider.GetLocation();
}

void URotationMovementComponent::Move(FVector value, float DeltaTime) {
	return;
}

FVector URotationMovementComponent::GetDisplacement(float DeltaTime) { return FVector::ZeroVector; }

FVector URotationMovementComponent::GetScanArm(FVector startLocation) {
	FRotator currentRotation = GetRayRotation();
	FVector forwardVector = RunnerMath::GetCleared(UKismetMathLibrary::GetForwardVector(currentRotation), 0.01f);
	return startLocation + forwardVector * scanArmLenght;
}

void URotationMovementComponent::BeginPlay() {
	Direction = FVector::ZeroVector;
}