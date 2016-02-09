// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class KOLASA_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	USceneComponent* BoomCamera;

	UPROPERTY(EditAnywhere)
	UArrowComponent* TraceForward;

	UPROPERTY(EditAnywhere)
	UArrowComponent* TraceDown;

	UPROPERTY()
	UCameraComponent* PlayerCamera;
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(BlueprintReadWrite, Category = Default)
	bool JumpButtonDown;

	UPROPERTY(BlueprintReadWrite, Category = Default)
	bool CrouchButtonDown;

	UPROPERTY(BlueprintReadWrite, Category = Default)
	bool FireButtonDown;

	UPROPERTY(BlueprintReadWrite, Category = Default)
	bool ProneButtonDown;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

protected:
	AMyCharacter(TCHAR* skeletalMeshPath, TCHAR* animBlueprintPath);
private:
	float MovementInputFactor = 10.0f;
	float ScanForwardArmLenght = 100.0f;
	float ScanDownArmLenght = 100.0f;
	void InitializeAnimationClass(USkeletalMeshComponent* inMesh, TCHAR* animBlueprintPath);
	void InitializeAnimationBlueprint(USkeletalMeshComponent* inMesh, TCHAR* animBlueprintPath);
	void InitializeStaticMesh(USkeletalMeshComponent* inMesh, TCHAR* skeletalMeshPath);
	void InitializeMovementComponent(UCharacterMovementComponent* inMovementComponent);
	void InitializeSpringArmComponent();
	void InitializeCamera();
	void InitializeTraceForward();
	void InitializeTraceDown();
	void EventMoveForward(float AxisValue);
	void EventMoveRight(float AxisValue);
	void EventTurn(float AxisValue);
	void EventLookUp(float AxisValue);
	void EventPromPressed();
	void EventCrouchPressed();
	void EventCrouchReleased();
	void EventSprintPressed();
	void EventSprintReleased();
	void EventFirePressed();
	void EventFireReleased();
	void EventJumpPressed();
	void EventJumpReleased();
	void SetMaxWalkSpeed(float value);
	void RotateOrtogonalToPlane(FHitResult& OutHit);
	void MoveForward();
	bool IsHitObstacle(const UArrowComponent* arm, float armLenght, FHitResult& outResult);
	FRotator GetYawRotator();
	void EventSavePressed();
};
