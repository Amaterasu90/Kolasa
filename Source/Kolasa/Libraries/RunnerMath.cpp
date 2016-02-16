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
