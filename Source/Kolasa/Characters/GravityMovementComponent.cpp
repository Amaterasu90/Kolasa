// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "ForwardMovementComponent.h"
#include "GravityMovementComponent.h"

void UGravityMovementComponent::SetForward(UForwardMovementComponent* forward){
	forwardMovement = forward;
}

void UGravityMovementComponent::Move(FVector value){
	if (!value.IsNearlyZero()) {
		SafeMoveUpdatedComponent(value, UpdatedComponent->GetComponentRotation(), true, Hit);
		if (Hit.IsValidBlockingHit()) {
			forwardMovement->UnlockMove();
			RotateOrtogonalToPlane(Hit);
		}
	}
}

void UGravityMovementComponent::RotateOrtogonalToPlane(FHitResult & OutHit) {
	FVector normalToPlane = OutHit.ImpactNormal;
	FVector forwardActor = UpdatedComponent->GetForwardVector();
	FVector rightActor = UpdatedComponent->GetRightVector();
	FVector downActor = -UpdatedComponent->GetUpVector();

	FVector crossDownAndRight = UKismetMathLibrary::Cross_VectorVector(downActor, rightActor);
	FVector newRight = UKismetMathLibrary::Cross_VectorVector(normalToPlane, crossDownAndRight);

	FRotator newRotation = UKismetMathLibrary::MakeRotationFromAxes(forwardActor, newRight, normalToPlane);

	Direction = newRotation.Vector();
	Direction.Normalize();

	UpdatedComponent->SetRelativeRotation(newRotation);
}

FVector UGravityMovementComponent::GetDisplacement(float DeltaTime){
	FVector updateMove = -UpdatedComponent->GetUpVector();
	return updateMove.GetClampedToMaxSize(1.0f) * DeltaTime * ForwardFactor;
}

void UGravityMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}