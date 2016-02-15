// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "ForwardMovementComponent.h"

void UForwardMovementComponent::BeginPlay() {
	Direction = UpdatedComponent->GetForwardVector();
	counter = 0.0f;
	ActivateMove();
}

void UForwardMovementComponent::Move(FVector value){

	if (IsActiveMove()) {
		if (!value.IsNearlyZero()) {
			SafeMoveUpdatedComponent(value, UpdatedComponent->GetComponentRotation(), true, CollisionHit);
		}
	}

	FVector currentLocation = GetRayBegin();
	FRotator currentRotation = GetRayRotation();
	FVector forwardVector = UKismetMathLibrary::GetForwardVector(currentRotation);
	FVector scanArm = currentLocation + forwardVector * 200.0f;

	TArray<AActor*> ignore;
	UKismetSystemLibrary::LineTraceSingle_NEW(this, currentLocation, scanArm, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ignore, EDrawDebugTrace::ForDuration, RayHit, true);
	
	//UpdatedComponent->AddRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
	/*if(CollisionHit.IsValidBlockingHit())
		RotateOrtogonalToPlane(CollisionHit);
	*/SmoothRotateToPlane(RayHit);
}

void UForwardMovementComponent::SmoothRotateToPlane(FHitResult & InHit) {
	
	FRotator clampedCurrent = UpdatedComponent->GetComponentRotation();
	if (InHit.IsValidBlockingHit()) {
		DeactivateMove();
		_downMovement->DeactivateMove();
		newRotation = GetOrtogonalToPlane(InHit);
		Direction = newRotation.Vector();
		Direction.Normalize();
	}

	else
	{
		counter = 0.0f;
		_downMovement->ActivateMove();
		ActivateMove();
	}


	
	if (clampedCurrent.Pitch + newRotation.Pitch > 0.0f || clampedCurrent.Pitch > newRotation.Pitch) {
			UpdatedComponent->AddRelativeRotation(FRotator((newRotation.Pitch - clampedCurrent.Pitch)*smoothClimbFactor, 0.0f, 0.0f));
	} else if (clampedCurrent.Pitch + newRotation.Pitch < 0.0f) {
		if (FMath::Abs(clampedCurrent.Pitch) < FMath::Abs(newRotation.Pitch)) {
			UpdatedComponent->SetRelativeRotation(FRotator((newRotation.Pitch + clampedCurrent.Pitch)*smoothClimbFactor, newRotation.Yaw, newRotation.Roll));
		}
		else {
			UpdatedComponent->AddRelativeRotation(FRotator((newRotation.Pitch - clampedCurrent.Pitch)*smoothClimbFactor, 0.0f, 0.0f));
		}
	}

	
}

void UForwardMovementComponent::RotateOrtogonalToPlane(FHitResult & InHit) {
	FRotator newRotation = GetOrtogonalToPlane(InHit);

	Direction = newRotation.Vector();
	Direction.Normalize();
	UpdatedComponent->SetRelativeRotation(newRotation);
}

FVector UForwardMovementComponent::GetDisplacement(float DeltaTime) {
	return Direction * DeltaTime * ForwardFactor;
}

FRotator UForwardMovementComponent::GetOrtogonalToPlane(FHitResult & InHit){
	FVector normalToPlane = InHit.ImpactNormal;
	FVector forwardActor = UpdatedComponent->GetForwardVector();
	FVector upActor = UpdatedComponent->GetUpVector();

	FVector crossForwardAndUp = UKismetMathLibrary::Cross_VectorVector(forwardActor, upActor);
	FVector newForward = UKismetMathLibrary::Cross_VectorVector(normalToPlane, crossForwardAndUp);
	FVector newRight = UKismetMathLibrary::Cross_VectorVector(normalToPlane, newForward);
	FVector newUp = normalToPlane;

	return UKismetMathLibrary::MakeRotationFromAxes(newForward, newRight, newUp);
}

void UForwardMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UForwardMovementComponent::SetDown(IBlockable* down){
	_downMovement = down;
}


