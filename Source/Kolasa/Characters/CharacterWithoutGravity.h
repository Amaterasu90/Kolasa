// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "GravityMovementComponent.h"
#include "ForwardMovementComponent.h"
#include "RightMovementComponent.h"
#include "CharacterWithoutGravity.generated.h"

UCLASS()
class KOLASA_API ACharacterWithoutGravity : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USceneComponent* BoomCamera;

	UPROPERTY(EditAnywhere)
	UCameraComponent* PlayerCamera;

	UPROPERTY(EditAnywhere)
	UGravityMovementComponent* GravityMovementComponent;

	UPROPERTY(EditAnywhere)
	UForwardMovementComponent* ForwardMovementComponent;

	UPROPERTY(EditDefaultsOnly)
	URightMovementComponent* RightMovementComponent;

	UArrowComponent* DownTrace;
	UArrowComponent* ForwardTrace;
	UArrowComponent* RightTrace;
	UArrowComponent* LeftTrace;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1.0", ClampMax = "10000.0"))
	float forwardFactor = 500.0f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1.0", ClampMax = "10000.0"))
	float gravityFactor = 500.0f;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "1.0", ClampMax = "10000.0"))
	float rightFactor = 500.0f;

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
	void InitializeForwardTrace();
	void InitializeDownTrace();
	void InitializeRightTrace();
	void InitializeLeftTrace();

	void EventMoveRight(float AxisValue);
	
	FRotator GetYawRotator();

protected:
	const FVector DefaultTraceLocation = FVector(0.0f, 0.0f, -75.0f);
};
