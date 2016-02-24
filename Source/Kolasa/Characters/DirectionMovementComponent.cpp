// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "DirectionMovementComponent.h"

void UDirectionMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		return;
	}
}

void UDirectionMovementComponent::SetMoveFactor(float newMoveFactor){
	MoveFactor = newMoveFactor;
}

void UDirectionMovementComponent::SetDirection(FVector newDirection){
	this->Direction = newDirection;
}

FVector UDirectionMovementComponent::GetDisplacement(float DeltaTime) { return Direction*DeltaTime*MoveFactor; }

