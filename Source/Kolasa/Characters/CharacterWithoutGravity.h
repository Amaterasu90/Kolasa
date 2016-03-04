// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "GravityMovementComponent.h"
#include "ForwardMovementComponent.h"
#include "RightMovementComponent.h"
#include "LeftMovementComponent.h"
#include "SideMovementComponent.h"
#include "ForwardRotationComponent.h"
#include "SkeletalOrientationComponent.h"
#include "CharacterWithoutGravity.generated.h"

UCLASS()
class KOLASA_API ACharacterWithoutGravity : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "200.0"))
	float DownScanRayDistance = 100.0f;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* BoomCamera;

	UPROPERTY(EditAnywhere)
	UCameraComponent* PlayerCamera;

	UPROPERTY(EditDefaultsOnly)
	UGravityMovementComponent* GravityMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	UForwardMovementComponent* ForwardMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	URightMovementComponent* RightMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	USideMovementComponent* HorizontalMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	ULeftMovementComponent* LeftMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	USkeletalOrientationComponent* SkeletalComponent;

	/*UPROPERTY(EditDefaultsOnly)
	UForwardRotationComponent* ConvexRotationComponent;*/

	//UArrowComponent* FrontDownTrace;
	//UArrowComponent* BackDownTrace;
	UArrowComponent* RightTrace;
	UArrowComponent* LeftTrace;

	// Sets default values for this pawn's properties
	ACharacterWithoutGravity();

	ACharacterWithoutGravity(TCHAR* skeletalMeshPath, TCHAR* animBlueprintPath);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

private:
	void InitializeCapsule();
	void InitializeStaticMesh(TCHAR* skeletalMeshPath);
	void InitializeSpringArm();
	void InitializeCamera();
	void InitializeAnimationClass(TCHAR* animBlueprintPath);
	void InitializeAnimationBlueprint(TCHAR* animBlueprintPath);
	void InitializeMovementComponent();
	void InitializeDownTrace();
	
	void InitializeLeftTrace();
	void InitializeRightTrace();

	void EventMoveRight(float AxisValue);
	void EventMeshPitchRotation(float AxisValue);
	
	FRotator GetYawRotator();

	FRotator startRotation;

protected:
	const FVector DefaultTraceLocation = FVector(0.0f, 0.0f, -75.0f);
};
