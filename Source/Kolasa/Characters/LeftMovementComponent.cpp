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
	return FMath::Abs(FMath::Sign(newRoll - oldRoll))*deltaTime*smoothClimbFactor;
}

float ULeftMovementComponent::CalcEndIteration(float oldRoll, float newRoll) {
	return FMath::Abs(newRoll - oldRoll);
}