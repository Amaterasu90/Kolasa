// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "ForwardMovementComponent.h"
#include "GravityMovementComponent.h"

void UGravityMovementComponent::BeginPlay() {
	MoveSwitch::Activate();
	FVector gravityDirection = (-UpdatedComponent->GetUpVector()).GetClampedToMaxSize(1.0f);
	this->SetDirection(gravityDirection);
}

void UGravityMovementComponent::ManageBlockMove(FHitResult& result) {
	if (result.IsValidBlockingHit()) {
		//forwardMovement->Activate();
		sideMovement->Activate();
	}
	else {
		//forwardMovement->Deactivate();
		sideMovement->Deactivate();
	}
}

void UGravityMovementComponent::Move(FVector value){
	FHitResult collisionHit;
	if (MoveSwitch::IsActive())
		if (!value.IsNearlyZero()) {
			SafeMoveUpdatedComponent(value, UpdatedComponent->GetComponentRotation(), true, collisionHit);
		}

	UpdateDirection();

	ManageBlockMove(collisionHit);
}

void UGravityMovementComponent::UpdateDirection() {
	FVector gravityDirection = (-UpdatedComponent->GetUpVector()).GetClampedToMaxSize(1.0f);
	SetDirection(gravityDirection);
}

void UGravityMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector desiredStepMove = this->GetDisplacement(DeltaTime);
	Move(desiredStepMove);
}

void UGravityMovementComponent::SetBlockSide(MoveSwitch& side){
	sideMovement = &side;
}

void UGravityMovementComponent::SetBlockLeft(RotationSwitch& left){
	leftRotation = &left;
}

void UGravityMovementComponent::SetBlockRight(RotationSwitch& right){
	rightRotation = &right;
}
