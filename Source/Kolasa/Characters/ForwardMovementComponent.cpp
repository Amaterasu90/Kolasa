// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Libraries/RunnerMath.h"
#include "ForwardMovementComponent.h"

void UForwardMovementComponent::BeginPlay() {
	SetDirection(UpdatedComponent->GetForwardVector());
	ActivateMove();
	//DeactivateMove();
}

void UForwardMovementComponent::Move(FVector value){
	FHitResult collisionHit;
	if (MoveSwitch::IsActive()) {
		if (!value.IsNearlyZero()) {
			SafeMoveUpdatedComponent(value, UpdatedComponent->GetComponentRotation(), true, collisionHit);
		}
	}
}

void UForwardMovementComponent::DeactivateMove(){
	if (MoveSwitch::IsActive()) {
		MoveSwitch::Deactivate();
	}
}

void UForwardMovementComponent::ActivateMove(){
	if (!MoveSwitch::IsActive()) {
		MoveSwitch::Activate();
	}
}

void UForwardMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector desiredStepMove = GetDisplacement(DeltaTime);
	Move(desiredStepMove);
}

void UForwardMovementComponent::SetBlockDown(MoveSwitch& down){
	downMovement = &down;
}


