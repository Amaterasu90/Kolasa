// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Libraries/RunnerMath.h"
#include "RightMovementComponent.h"

bool URightMovementComponent::IsReadyToEnableScanRotation(FVector right, FVector sideDirection){
	return (right + sideDirection.GetClampedToMaxSize(1.0f)).Size() < right.Size();;
}

float URightMovementComponent::CalcIterationStep(float oldRoll, float newRoll, float deltaTime) {
	return FMath::Abs(FMath::Sign(newRoll - oldRoll)*deltaTime*smoothClimbFactor);
}

float URightMovementComponent::CalcEndIteration(float oldRoll, float newRoll) {
	return newRoll - oldRoll;
}

void URightMovementComponent::SmoothRotate(float DeltaTime, RotationSwitch& down)
{
	FRotator current = UpdatedComponent->GetComponentRotation();
	if (!(RunnerMath::GetCleared(newRotation) - current).IsNearlyZero(0.01f)) {
		if (counter == 0.0f) {
			stepCounting = CalcIterationStep(clampedCurrent.Roll, newRotation.Roll, DeltaTime);
			clampedCurrent = UpdatedComponent->GetComponentRotation();
		}
		else {
			UpdatedComponent->SetRelativeRotation(FRotator(newRotation.Pitch, newRotation.Yaw, clampedCurrent.Roll - counter));
		}
		counter += stepCounting;
	}
	else if ((newRotation - current).IsNearlyZero(0.01f) && !FMath::IsNearlyZero(counter)) {
		UpdatedComponent->SetRelativeRotation(RunnerMath::GetCleared(newRotation));
		counter = 0.0f;
		stepCounting = 0.0f;
		down.Activate();
		RotationSwitch::EndSmootRotation();
	}
}
