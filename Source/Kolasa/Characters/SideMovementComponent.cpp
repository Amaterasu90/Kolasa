// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "SideMovementComponent.h"

void USideMovementComponent::BeginPlay() {
	MoveSwitch::Activate();
}

FVector USideMovementComponent::GetDirection() {
	return DesiredMovementThisFrame;
}

void USideMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f)* MoveFactor * DeltaTime;
	if (!DesiredMovementThisFrame.IsNearlyZero()) {
		FHitResult hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, hit);
	}
}