// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Libraries/RunnerMath.h"
#include "RightMovementComponent.h"

bool URightMovementComponent::IsReadyToEnableScanRotation(FVector right, FVector sideDirection){
	return (right + sideDirection.GetClampedToMaxSize(1.0f)).Size() < right.Size();
}

float URightMovementComponent::CalcIterationStep(float oldRoll, float newRoll, float deltaTime) {
	if (FMath::IsNearlyZero(oldRoll))
		oldRoll = 360.0f;
	return FMath::Sign(newRoll - oldRoll)*deltaTime*smoothClimbFactor;
}

float URightMovementComponent::CalcEndIteration(float oldRoll, float newRoll) {
	if (FMath::IsNearlyZero(oldRoll))
		oldRoll = 360.0f;
	return newRoll - oldRoll;
}
