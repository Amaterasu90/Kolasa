// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "RunnerMath.h"

FVector RunnerMath::GetCleared(FVector vector, float tolerance) {
	FVector result;
	result = FVector(FMath::IsNearlyZero(vector.X, 0.01f) ? 0.0f : vector.X,
		FMath::IsNearlyZero(vector.Y, 0.01f) ? 0.0f : vector.Y,
		FMath::IsNearlyZero(vector.Z, 0.01f) ? 0.0f : vector.Z);
	return result;
}

FVector RunnerMath::GetUnitVector(FVector value) {
	FVector result;
	if (value.X != 0.0f)
		result.X = value.X / FMath::Abs(value.X);
	else
		result.X = 0.0f;
	
	if (value.Y != 0.0f)
		result.Y = value.Y / FMath::Abs(value.Y);
	else
		result.Y = 0.0f;
	
	if (value.Z != 0.0f)
		result.Z = value.Z / FMath::Abs(value.Z);
	else
		result.Z = 0.0f;
	return result;
}
