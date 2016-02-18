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

FRotator RunnerMath::Align(FRotator value) {
	float Pitch = value.Pitch;
	Pitch = Pitch * FMath::Pow(10.0f, 2.0f);
	Pitch = FMath::FloorToFloat(Pitch) * FMath::Pow(10.0f, -2.0f);

	float Yaw = value.Yaw;
	Yaw = Yaw * FMath::Pow(10.0f, 2.0f);
	Yaw = FMath::FloorToFloat(Yaw) * FMath::Pow(10.0f, -2.0f);

	float Roll = value.Roll;
	Roll = Roll * FMath::Pow(10.0f, 2.0f);
	Roll = FMath::FloorToFloat(Roll) * FMath::Pow(10.0f, -2.0f);

	return FRotator(Pitch, Yaw, Roll);
}
