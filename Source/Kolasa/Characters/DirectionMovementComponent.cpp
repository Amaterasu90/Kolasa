// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "DirectionMovementComponent.h"

void UDirectionMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) {
		return;
	}

	FVector DesiredMovementThisFrame = GetDisplacement(DeltaTime);
	Move(DesiredMovementThisFrame,DeltaTime);
}

void UDirectionMovementComponent::SetForwardFactor(float value){
	this->ForwardFactor = value;
}

void UDirectionMovementComponent::SetScanRay(RayProvider provider){
	_provider = provider;
}

FVector UDirectionMovementComponent::GetRayBegin(){
	return _provider.GetLocation();
}

FRotator UDirectionMovementComponent::GetRayRotation(){
	return _provider.GetRotation();
}

void UDirectionMovementComponent::Move(FVector value,float DeltaTime){
	return;
}

FVector UDirectionMovementComponent::GetDisplacement(float DeltaTime) { return FVector::ZeroVector; }

void UDirectionMovementComponent::BeginPlay() {
	Direction = UpdatedComponent->GetForwardVector();
}