// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Libraries/RunnerMath.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "RotationMovementComponent.h"

void URotationMovementComponent::BeginPlay() {
	RotationSwitch::Activate();
	counter = 0.0f;
	lastHitLocation = GetRayBegin();
}

void URotationMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		return;
	}

	if (RotationSwitch::IsActive()) {
		FHitResult hit = GetRotationRayHit();
		SmoothRotateToPlane(hit, DeltaTime);
	}
}

FHitResult URotationMovementComponent::GetRotationRayHit() {
	FVector currentLocation = GetRayBegin();
	FVector scanArm = GetScanArm(currentLocation);

	TArray<AActor*> ignore;
	FHitResult result;
	UKismetSystemLibrary::LineTraceSingle_NEW(this, currentLocation, scanArm, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ignore, EDrawDebugTrace::ForDuration, result, true);

	return result;
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

FVector URotationMovementComponent::GetScanArm(FVector startLocation) {
	FRotator currentRotation = GetRayRotation();
	FVector forwardVector = RunnerMath::GetCleared(UKismetMathLibrary::GetForwardVector(currentRotation), 0.01f);
	return startLocation + forwardVector * scanArmLenght;
}

void URotationMovementComponent::SetDown(MoveSwitch& down) {
	_downMovement = &down;
}

float URotationMovementComponent::CalcIterationStep(float oldRoll, float newRoll, float deltaTime) {
	if (newRoll > 0.0f && oldRoll < 0.0f) {
		return FMath::Sign(-FMath::Abs(oldRoll + newRoll))*deltaTime*smoothClimbFactor;
	}
	else {
		return FMath::Sign(newRoll - oldRoll)*deltaTime*smoothClimbFactor;
	}
	return 0.0f;
}

float URotationMovementComponent::CalcEndIteration(float oldRoll, float newRoll) {
	if (newRoll > 0.0f && oldRoll < 0.0f) {
		return FMath::Abs(-FMath::Abs(oldRoll + newRoll));
	}
	else {
		return FMath::Abs(newRoll - oldRoll);
	}
	return 0.0f;
}

void URotationMovementComponent::SmoothRotateToPlane(FHitResult & InHit, float DeltaTime){
	FRotator clampedCurrent = UpdatedComponent->GetComponentRotation();
	MoveSwitch* downMovement = GetDownInterface();
	RotationSwitch* otherSiteRotation = GetOtherInterface();
	if (InHit.IsValidBlockingHit()) {
		downMovement->Deactivate();
		otherSiteRotation->Deactivate();
		newRotation = GetOrtogonalToPlane(InHit);
	}

	float end = CalcEndIteration(clampedCurrent.Roll, newRotation.Roll);
	if (FMath::Abs(counter) < end && newRotation != FRotator::ZeroRotator) {
		UpdatedComponent->SetRelativeRotation(FRotator(newRotation.Pitch, newRotation.Yaw, clampedCurrent.Roll + counter));
		counter += CalcIterationStep(clampedCurrent.Roll, newRotation.Roll, DeltaTime);
	}
	else if (counter != 0.0f) {
		UpdatedComponent->SetRelativeRotation(newRotation);
		downMovement->Activate();
		counter = 0.0f;
		newRotation = FRotator::ZeroRotator;
		RotationSwitch::EndSmootRotation();
	}

	if (RotationSwitch::IsDurningRotation())
	{
		FVector right = UpdatedComponent->GetRightVector();
		FVector sideDirection = sideComponent->GetDirection();
		if (IsReadyToEnableScanRotation(right, sideDirection))
		{
			otherSiteRotation->Activate();
			RotationSwitch::StartSmootRotation();
		}
	}
}

RotationSwitch *& URotationMovementComponent::GetOtherInterface(){
	return _oppositeSiteRotation;
}

MoveSwitch *& URotationMovementComponent::GetDownInterface(){
	return _downMovement;
}


void URotationMovementComponent::SetOppositeSiteInterface(RotationSwitch& opposite) {
	_oppositeSiteRotation = &opposite;
}



void URotationMovementComponent::SetSideMovementComponent(ISideDirectionMovement& sideComponent) {
	this->sideComponent = &sideComponent;
}

FRotator URotationMovementComponent::GetOrtogonalToPlane(FHitResult & InHit) {
	FVector normalToPlane = RunnerMath::GetCleared(InHit.ImpactNormal, 0.01f);
	FVector rightActor = RunnerMath::GetCleared(UpdatedComponent->GetRightVector(), 0.01f);
	FVector upActor = RunnerMath::GetCleared(UpdatedComponent->GetUpVector(), 0.01f);

	FVector crossRightAndUp = RunnerMath::GetCleared(UKismetMathLibrary::Cross_VectorVector(rightActor, upActor), 0.01f);
	FVector newRight = RunnerMath::GetCleared(UKismetMathLibrary::Cross_VectorVector(normalToPlane, crossRightAndUp), 0.01f);
	FVector newForward = crossRightAndUp;
	FVector newUp = RunnerMath::GetCleared(normalToPlane, 0.01f);

	return UKismetMathLibrary::MakeRotationFromAxes(newForward, newRight, newUp);
}