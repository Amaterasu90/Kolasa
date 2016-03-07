// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "SkeletalOrientationComponent.h"

void USkeletalOrientationComponent::BeginPlay() {
	counter = 0.0f;
}

void USkeletalOrientationComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		return;
	}
	float desiredAxis = ConsumeAxis();
	if (!FMath::IsNearlyZero(desiredAxis, 0.01f)) {
		FRotator desiredRotation = UpdatedComponent->RelativeRotation.Clamp();
		desiredRotation.Yaw = UKismetMathLibrary::ClampAngle(desiredRotation.Yaw + desiredAxis*SkeletalRotationFactor, rotationBalance.Yaw - leftDeviation, rotationBalance.Yaw + rightDeviation);
		UpdatedComponent->SetRelativeRotation(desiredRotation);
		counter = 0.0f;
	}
	else
	{
		if (counter == 0.0f) {
			counter = UpdatedComponent->RelativeRotation.Clamp().Yaw - rotationBalance.Clamp().Yaw;
		}
		if (!(rotationBalance.Clamp() - UpdatedComponent->RelativeRotation.Clamp()).IsNearlyZero()) {
			counter -= FMath::Sign(counter)*SkeletalRotationFactor;
			UpdatedComponent->SetRelativeRotation(FRotator(rotationBalance.Pitch, rotationBalance.Yaw + counter, rotationBalance.Roll));
		}
		else {
			counter = 0.0f;
			UpdatedComponent->SetRelativeRotation(rotationBalance);
		}
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
