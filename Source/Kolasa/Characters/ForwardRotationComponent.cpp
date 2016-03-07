// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Libraries/RunnerMath.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "ForwardRotationComponent.h"

void UForwardRotationComponent::BeginPlay() {
	RotationSwitch::Activate();
}

void UForwardRotationComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	FHitResult backHit = GetBackRotationRayHit();
	FHitResult frontHit = GetFrontRotationRayHit();
	
	if (RotationSwitch::IsActive() ) {
		if (backHit.IsValidBlockingHit() && frontHit.IsValidBlockingHit()) {
			FVector backNormal = backHit.ImpactNormal;
			FVector backForward = UpdatedComponent->GetForwardVector();
			FVector backUp = UpdatedComponent->GetUpVector();
			FVector backNewRight = UKismetMathLibrary::Cross_VectorVector(backForward, backUp);
			FVector backNewForward = UKismetMathLibrary::Cross_VectorVector(backNormal, backNewRight);
			FVector backNewUp = backNormal;
			backNewRotation = UKismetMathLibrary::MakeRotationFromAxes(backNewForward, -backNewRight, backNewUp);

			FVector frontNormal = frontHit.ImpactNormal;
			FVector frontForward = UpdatedComponent->GetForwardVector();
			FVector frontUp = UpdatedComponent->GetUpVector();
			FVector frontNewRight = UKismetMathLibrary::Cross_VectorVector(frontForward, frontUp);
			FVector frontNewForward = UKismetMathLibrary::Cross_VectorVector(frontNormal, frontNewRight);
			FVector frontNewUp = frontNormal;
			frontNewRotation = UKismetMathLibrary::MakeRotationFromAxes(frontNewForward, -frontNewRight, frontNewUp);

			FRotator result = FMath::Lerp(backNewRotation, frontNewRotation, 0.5f);
			right->newRotation = result;
			left->newRotation = result;
			UpdatedComponent->SetRelativeRotation(result);
		}
	}
}

FHitResult UForwardRotationComponent::GetFrontRotationRayHit()
{
	FVector currentLocation = GetFrontRayBegin();
	FVector scanArm = GetFrontScanArm(currentLocation);

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

FRotator UForwardRotationComponent::GetFrontRayRotation()
{
	return frontProvider.GetRotation();
}

FVector UForwardRotationComponent::GetFrontRayBegin()
{
	return frontProvider.GetLocation();
}

FVector UForwardRotationComponent::GetFrontScanArm(FVector startLocation)
{
	FRotator currentRotation = GetFrontRayRotation();
	FVector forwardVector = UKismetMathLibrary::GetForwardVector(currentRotation);
	return startLocation + forwardVector * backScanArmLenght;
}

void UForwardRotationComponent::SetFrontScanRay(RayProvider provider){
	frontProvider = provider;
}

FHitResult UForwardRotationComponent::GetBackRotationRayHit() {
	FVector currentLocation = GetBackRayBegin();
	FVector scanArm = GetBackScanArm(currentLocation);

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

FRotator UForwardRotationComponent::GetBackRayRotation() {
	return backProvider.GetRotation();
}

FVector UForwardRotationComponent::GetBackRayBegin() {
	return backProvider.GetLocation();
}

void UForwardRotationComponent::SetBackScanRay(RayProvider provider) {
	backProvider = provider;
}

void UForwardRotationComponent::SetLeftComponent(RotationSwitch& componentRef){
	left = &componentRef;
}

void UForwardRotationComponent::SetRightComponent(RotationSwitch & componentRef){
	right = &componentRef;
}

FVector UForwardRotationComponent::GetBackScanArm(FVector startLocation) {
	FRotator currentRotation = GetBackRayRotation();
	FVector forwardVector = UKismetMathLibrary::GetForwardVector(currentRotation);
	return startLocation + forwardVector * backScanArmLenght;
}
