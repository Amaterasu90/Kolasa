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
		if (newRotation == FRotator(90.0f, 0.0f, 180.0f))
			newRotation = FRotator(90.0f, 180.0f, 0.0f);
		if (newRotation == FRotator(-90.0f, 0.0f, 0.0f))
			newRotation = FRotator(-90.0f, 180.0f, 180.0f);
		Direction = newRotation.Vector();
		Direction.Normalize();
		FRotator rot = UKismetMathLibrary::MakeRotationFromAxes(FVector(0.0f, 0.0f, -1.1f), FVector(0.0f, -1.1f, 0.0f), FVector(1.0f, 0.0f, 0.0f));
		rot.Add(1.0f, 0.0f, 0.0f);
	}
	
	countingDirection = FMath::Sign(newRotation.Pitch - clampedCurrent.Pitch)*smoothClimbFactor;
	if (countingDirection > 0.0f && counter < newRotation.Pitch - clampedCurrent.Pitch) {
		UpdatedComponent->SetRelativeRotation(FRotator(clampedCurrent.Pitch + counter, newRotation.Yaw, newRotation.Roll));
		counter += countingDirection;
	}
	else if (countingDirection < 0.0f && counter > newRotation.Pitch - clampedCurrent.Pitch) {
		UpdatedComponent->SetRelativeRotation(FRotator(clampedCurrent.Pitch + counter, newRotation.Yaw, newRotation.Roll));
		counter += countingDirection;
	}
	else
	{
		counter = 0.0f;
		_downMovement->ActivateMove();
		ActivateMove();
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
	normalToPlane.X = FMath::IsNearlyZero(normalToPlane.X,0.01f) ? 0.0f : normalToPlane.X;
	normalToPlane.Y = FMath::IsNearlyZero(normalToPlane.Y, 0.01f) ? 0.0f : normalToPlane.Y;
	normalToPlane.Z = FMath::IsNearlyZero(normalToPlane.Z, 0.01f) ? 0.0f : normalToPlane.Z;
	FVector forwardActor = UpdatedComponent->GetForwardVector();
	forwardActor.X = FMath::IsNearlyZero(forwardActor.X, 0.01f) ? 0.0f : forwardActor.X;
	forwardActor.Y = FMath::IsNearlyZero(forwardActor.Y, 0.01f) ? 0.0f : forwardActor.Y;
	forwardActor.Z = FMath::IsNearlyZero(forwardActor.Z, 0.01f) ? 0.0f : forwardActor.Z;
	FVector upActor = UpdatedComponent->GetUpVector();
	upActor.X = FMath::IsNearlyZero(upActor.X, 0.01f) ? 0.0f : upActor.X;
	upActor.Y = FMath::IsNearlyZero(upActor.Y, 0.01f) ? 0.0f : upActor.Y;
	upActor.Z = FMath::IsNearlyZero(upActor.Z, 0.01f) ? 0.0f : upActor.Z;
	FVector rightActor = UpdatedComponent->GetRightVector();

	FVector crossForwardAndUp = UKismetMathLibrary::Cross_VectorVector(forwardActor, upActor);
	crossForwardAndUp.X = FMath::IsNearlyZero(crossForwardAndUp.X, 0.01f) ? 0.0f : crossForwardAndUp.X;
	crossForwardAndUp.Y = FMath::IsNearlyZero(crossForwardAndUp.Y, 0.01f) ? 0.0f : crossForwardAndUp.Y;
	crossForwardAndUp.Z = FMath::IsNearlyZero(crossForwardAndUp.Z, 0.01f) ? 0.0f : crossForwardAndUp.Z;
	FVector newForward = UKismetMathLibrary::Cross_VectorVector(normalToPlane, crossForwardAndUp);
	newForward.X = FMath::IsNearlyZero(newForward.X, 0.01f) ? 0.0f : newForward.X;
	newForward.Y = FMath::IsNearlyZero(newForward.Y, 0.01f) ? 0.0f : newForward.Y;
	newForward.Z = FMath::IsNearlyZero(newForward.Z, 0.01f) ? 0.0f : newForward.Z;
	FVector newRight = UKismetMathLibrary::Cross_VectorVector(normalToPlane, newForward);
	newRight.X = FMath::IsNearlyZero(newRight.X, 0.01f) ? 0.0f : newRight.X;
	newRight.Y = FMath::IsNearlyZero(newRight.Y, 0.01f) ? 0.0f : newRight.Y;
	newRight.Z = FMath::IsNearlyZero(newRight.Z, 0.01f) ? 0.0f : newRight.Z;
	FVector newUp = normalToPlane;
	newUp.X = FMath::IsNearlyZero(newUp.X, 0.01f) ? 0.0f : newUp.X;
	newUp.Y = FMath::IsNearlyZero(newUp.Y, 0.01f) ? 0.0f : newUp.Y;
	newUp.Z = FMath::IsNearlyZero(newUp.Z, 0.01f) ? 0.0f : newUp.Z;

	return UKismetMathLibrary::MakeRotationFromAxes(newForward, newRight, newUp);
}

void UForwardMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UForwardMovementComponent::SetDown(IBlockable* down){
	_downMovement = down;
}


