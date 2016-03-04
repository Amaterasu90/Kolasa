// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Libraries/RunnerMath.h"
#include "LeftMovementComponent.h"

bool ULeftMovementComponent::IsReadyToEnableScanRotation(FVector right, FVector sideDirection){
	return (right - sideDirection.GetClampedToMaxSize(1.0f)).Size() < right.Size();
}

float ULeftMovementComponent::CalcIterationStep(float oldRoll, float newRoll, float deltaTime) {
	return FMath::Abs(FMath::Sign(oldRoll- newRoll)*deltaTime*smoothClimbFactor);
}

float ULeftMovementComponent::CalcEndIteration(float oldRoll, float newRoll) {
	return FMath::Abs(oldRoll - newRoll);
}

void ULeftMovementComponent::SmoothRotate(float DeltaTime){
	FRotator current = UpdatedComponent->GetComponentRotation();
	if (!(RunnerMath::GetCleared(newRotation) - current).IsNearlyZero(0.01f)) {
		if (counter == 0.0f) {
			stepCounting = CalcIterationStep(clampedCurrent.Roll, newRotation.Roll, DeltaTime);
			clampedCurrent = UpdatedComponent->GetComponentRotation();
		}
		else {
			UpdatedComponent->SetRelativeRotation(FRotator(newRotation.Pitch, newRotation.Yaw, clampedCurrent.Roll + counter));
		}
		counter += stepCounting;
	}
	else if ((newRotation - current).IsNearlyZero(0.01f)) {
		UpdatedComponent->SetRelativeRotation(RunnerMath::GetCleared(newRotation));
		counter = 0.0f;
		stepCounting = 0.0f;
		RotationSwitch::EndSmootRotation();
	}
}