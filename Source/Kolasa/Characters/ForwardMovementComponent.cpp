// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "ForwardMovementComponent.h"

void UForwardMovementComponent::Move(FVector value){
	if (!value.IsNearlyZero()) {
		SafeMoveUpdatedComponent(value, UpdatedComponent->GetComponentRotation(), true, Hit);
		if (Hit.IsValidBlockingHit()) {
			RotateOrtogonalToPlane(Hit);
		}
	}
}

void UForwardMovementComponent::RotateOrtogonalToPlane(FHitResult & OutHit) {
	FVector normalToPlane = OutHit.ImpactNormal;
	FVector forwardActor = UpdatedComponent->GetForwardVector();
	FVector upActor = UpdatedComponent->GetUpVector();

	FVector crossForwardAndUp = UKismetMathLibrary::Cross_VectorVector(forwardActor, upActor);
	FVector newForward = UKismetMathLibrary::Cross_VectorVector(normalToPlane, crossForwardAndUp);
	FVector newRight = UKismetMathLibrary::Cross_VectorVector(normalToPlane, newForward);
	FVector newUp = upActor;

	FRotator newRotation = UKismetMathLibrary::MakeRotationFromAxes(newForward, newRight, newUp);

	Direction = newRotation.Vector();
	Direction.Normalize();

	UpdatedComponent->SetRelativeRotation(newRotation);
}

FVector UForwardMovementComponent::GetDisplacement(float DeltaTime) {
	return Direction * DeltaTime * ForwardFactor;
}

void UForwardMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}


