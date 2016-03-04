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
	newRotation = FRotator::ZeroRotator;
	stepCounting = 0.0f;
	clampedCurrent = FRotator::ZeroRotator;
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

	FHitResult result;

	if (!this->bTraceVisibilty) {
		static FName FireTraceIdent = FName(TEXT("Rotation Trace"));
		FCollisionQueryParams TraceParams(FireTraceIdent, true);
		TraceParams.bTraceAsyncScene = true;

		GetWorld()->LineTraceSingleByChannel(result, currentLocation, scanArm, ECollisionChannel::ECC_Visibility, TraceParams);
	}
	else
	{
		TArray<AActor*> ignore;
		UKismetSystemLibrary::LineTraceSingle_NEW(this, currentLocation, scanArm, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ignore, EDrawDebugTrace::ForDuration, result, true);
	}
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


void URotationMovementComponent::SmoothRotateToPlane(FHitResult & InHit, float DeltaTime){
	RotationSwitch* otherSiteRotation = GetOtherInterface();
	CalcNewRotation(InHit, *otherSiteRotation);
	
	SmoothRotate(DeltaTime);

	if (RotationSwitch::IsDurningRotation())
	{
		FVector right = UpdatedComponent->GetRightVector();
		FVector sideDirection = sideComponent->GetDirection();
		if (IsReadyToEnableScanRotation(right, sideDirection)){
			otherSiteRotation->Activate();
			otherSiteRotation->newRotation = UpdatedComponent->GetComponentRotation();
			RotationSwitch::StartSmootRotation();
		}
	}
}

RotationSwitch *& URotationMovementComponent::GetOtherInterface(){
	return _oppositeSiteRotation;
}

void URotationMovementComponent::SetOppositeSiteInterface(RotationSwitch& opposite) {
	_oppositeSiteRotation = &opposite;
}

void URotationMovementComponent::SetSideMovementComponent(ISideDirectionMovement& sideComponent) {
	this->sideComponent = &sideComponent;
}

FRotator URotationMovementComponent::GetOrtogonalToPlane(FHitResult & InHit) {
	normalToPlane = InHit.ImpactNormal;
	rightActor = UpdatedComponent->GetRightVector();
	upActor = UpdatedComponent->GetUpVector();

	crossRightAndUp = UKismetMathLibrary::Cross_VectorVector(rightActor, upActor);
	newRight = UKismetMathLibrary::Cross_VectorVector(normalToPlane, crossRightAndUp);
	newForward = crossRightAndUp;
	newUp = normalToPlane;

	return UKismetMathLibrary::MakeRotationFromAxes(newForward, newRight, newUp);
}

void URotationMovementComponent::CalcNewRotation(FHitResult & hit,  RotationSwitch& otherSite){
	if (hit.IsValidBlockingHit()) {
		otherSite.Deactivate();
		newRotation = GetOrtogonalToPlane(hit);
	}
}

void URotationMovementComponent::FinalizeRotate(bool isReady){
	if (isReady && counter != 0.0f) {
		/*UpdatedComponent->SetRelativeRotation(newRotation);
		counter = 0.0f;
		stepCounting = 0.0f;
		newRotation = FRotator::ZeroRotator;
		clampedCurrent = FRotator::ZeroRotator;
		RotationSwitch::EndSmootRotation();*/
	}
}
