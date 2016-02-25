// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "SkeletalOrientationComponent.h"

void USkeletalOrientationComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		return;
	}
	float desiredAxis = ConsumeAxis();
	if (!FMath::IsNearlyZero(desiredAxis,0.01f)) {
		FRotator desiredRotation = UpdatedComponent->RelativeRotation.Clamp();
		desiredRotation.Yaw = UKismetMathLibrary::ClampAngle(desiredRotation.Yaw + desiredAxis*SkeletalRotationFactor, rotationBalance.Yaw - leftDeviation, rotationBalance.Yaw + rightDeviation);

		UpdatedComponent->SetRelativeRotation(desiredRotation);
	}
	else {
		float rotationDistance = rotationBalance.Yaw - UpdatedComponent->RelativeRotation.Clamp().Yaw;
		float step = SkeletalRotationFactor*UKismetMathLibrary::SignOfFloat(rotationDistance);
		FHitResult hit;
		if (FMath::Abs(rotationDistance) > FMath::Abs(step)) {
			UpdatedComponent->RelativeRotation.Yaw = UpdatedComponent->RelativeRotation.Clamp().Yaw + step;
		}
		else
			UpdatedComponent->RelativeRotation.Yaw = rotationBalance.Yaw;
	}
}

void USkeletalOrientationComponent::AddInputAxis(float accelerationAxis){
	if (!IsMoveInputIgnored())
	{
		ControlInputAxis = accelerationAxis;
	}
}

float USkeletalOrientationComponent::ConsumeAxis(){
	LastControlInputAxis = ControlInputAxis;
	ControlInputAxis = 0.0f;
	return LastControlInputAxis;
}

void USkeletalOrientationComponent::SetRotationBalance(FRotator rotationBalance){
	this->rotationBalance = rotationBalance;
}
