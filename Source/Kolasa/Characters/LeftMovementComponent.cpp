// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Libraries/RunnerMath.h"
#include "LeftMovementComponent.h"

bool ULeftMovementComponent::IsReadyToEnableScanRotation(FVector right, FVector sideDirection){
	return (right - sideDirection.GetClampedToMaxSize(1.0f)).Size() < right.Size();
}