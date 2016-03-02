// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Libraries/RunnerMath.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "ForwardRotationComponent.h"

void UForwardRotationComponent::BeginPlay() {
	MoveSwitch::Activate();
}

void UForwardRotationComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	FHitResult hit = GetRotationRayHit();
	if (hit.IsValidBlockingHit() && MoveSwitch::IsActive()) {
		FVector normal = hit.ImpactNormal;
		FVector forward = UpdatedComponent->GetForwardVector();
		FVector up = UpdatedComponent->GetUpVector();
		FVector newRight = UKismetMathLibrary::Cross_VectorVector(forward,up);
		FVector newForward = UKismetMathLibrary::Cross_VectorVector(normal, newRight);
		FVector newUp = normal;
		FRotator newRotation = UKismetMathLibrary::MakeRotationFromAxes(newForward, -newRight, newUp);
		UpdatedComponent->SetRelativeRotation(newRotation);
		forwardComponent->SetDirection(newForward);
	}
}

FHitResult UForwardRotationComponent::GetRotationRayHit() {
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

FRotator UForwardRotationComponent::GetRayRotation() {
	return _provider.GetRotation();
}

FVector UForwardRotationComponent::GetRayBegin() {
	return _provider.GetLocation();
}

void UForwardRotationComponent::SetScanRay(RayProvider provider) {
	_provider = provider;
}

void UForwardRotationComponent::SetForwardComponent(UForwardMovementComponent& forward){
	forwardComponent = &forward;
}

FVector UForwardRotationComponent::GetScanArm(FVector startLocation) {
	FRotator currentRotation = GetRayRotation();
	FVector forwardVector = UKismetMathLibrary::GetForwardVector(currentRotation);
	return startLocation + forwardVector * scanArmLenght;
}
