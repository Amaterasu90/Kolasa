// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "MyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

void AMyCharacter::InitializeAnimationClass(USkeletalMeshComponent* inMesh, TCHAR* animBlueprintPath) {
	UAnimBlueprint *animation = LoadObject<UAnimBlueprint>(NULL, animBlueprintPath);
	if (animation != nullptr) {
		inMesh->SetAnimInstanceClass(animation->GetAnimBlueprintGeneratedClass());
	}
}

void AMyCharacter::InitializeStaticMesh(USkeletalMeshComponent* inMesh, TCHAR* skeletalMeshPath) {
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshContainer(skeletalMeshPath);
	if (MeshContainer.Succeeded()) {
		inMesh->SetSkeletalMesh(MeshContainer.Object);
	}

	inMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
	inMesh->AddRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
}

void AMyCharacter::InitializeAnimationBlueprint(USkeletalMeshComponent* inMesh, TCHAR* animBlueprintPath) {
	//must static loading
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> animContainer(animBlueprintPath);
	if (animContainer.Succeeded()) {
		inMesh->AnimBlueprintGeneratedClass = (UAnimBlueprintGeneratedClass*)animContainer.Object->GetAnimBlueprintGeneratedClass();
	}
}

void AMyCharacter::InitializeMovementComponent(UCharacterMovementComponent* inMovementComponent) {
	inMovementComponent->GravityScale = 1.5f;
	inMovementComponent->bOrientRotationToMovement = true;

	inMovementComponent->JumpZVelocity = 300.0f;

	inMovementComponent->MaxSwimSpeed = 170.0f;

	inMovementComponent->MaxFlySpeed = 270.0f;

	inMovementComponent->MaxCustomMovementSpeed = 270.0f;

	inMovementComponent->MaxWalkSpeed = 270.0f;
	inMovementComponent->MaxWalkSpeedCrouched = 125.0f;
	inMovementComponent->bIgnoreBaseRotation = true;
}

void AMyCharacter::InitializeSpringArmComponent() {
	BoomCamera = CreateDefaultSubobject<USpringArmComponent>("BoomCamera");
	BoomCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	USpringArmComponent * springArm = Cast<USpringArmComponent>(BoomCamera);
	springArm->bUsePawnControlRotation = true;
	BoomCamera->AttachTo(RootComponent);
}

void AMyCharacter::InitializeCamera() {
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->AttachTo(BoomCamera);
}

AMyCharacter::AMyCharacter(TCHAR* skeletalMeshPath, TCHAR* animBlueprintPath) : Super() {

	InitializeSpringArmComponent();
	InitializeCamera();

	USkeletalMeshComponent* mesh = GetMesh();
	InitializeStaticMesh(mesh, skeletalMeshPath);
	InitializeAnimationClass(mesh, animBlueprintPath);
	InitializeAnimationBlueprint(mesh, animBlueprintPath);

	UCharacterMovementComponent* movementComponent = GetCharacterMovement();
	InitializeMovementComponent(movementComponent);

	bUseControllerRotationYaw = false;
	
}

// Sets default values
AMyCharacter::AMyCharacter() : AMyCharacter(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"),
	TEXT("AnimBlueprint'/Game/AnimStarterPack/UE4ASP_HeroTPP_AnimBlueprint.UE4ASP_HeroTPP_AnimBlueprint'"))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* capsule = GetCapsuleComponent();
	capsule->SetCapsuleHalfHeight(100.0f);
	capsule->SetCapsuleRadius(45.0f);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

FRotator AMyCharacter::GetYawRotator() {
	FRotator ActualRotation = GetControlRotation();
	return FRotator(0.0f, ActualRotation.Yaw, 0.0f);
}

void AMyCharacter::EventMoveForward(float AxisValue) {
	FRotator UpdateRotator = GetYawRotator();
	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(UpdateRotator);
	AddMovementInput(ForwardVector, AxisValue, false);
}

void AMyCharacter::EventMoveRight(float AxisValue) {
	FRotator UpdateRotator = GetYawRotator();
	FVector RightVector = UKismetMathLibrary::GetRightVector(UpdateRotator);
	AddMovementInput(RightVector, AxisValue, false);
}

void AMyCharacter::EventTurn(float AxisValue) {
	AddControllerYawInput(AxisValue);
}

void AMyCharacter::EventLookUp(float AxisValue) {
	AddControllerPitchInput(AxisValue);
}

void AMyCharacter::EventPromPressed() {
	this->ProneButtonDown = !this->ProneButtonDown;
}

void AMyCharacter::SetMaxWalkSpeed(float value) {
	UCharacterMovementComponent* movementComponent = GetCharacterMovement();
	movementComponent->MaxWalkSpeed = value;
}

void AMyCharacter::EventCrouchPressed() {
	this->CrouchButtonDown = true;
	SetMaxWalkSpeed(125.0f);
}

void AMyCharacter::EventCrouchReleased() {
	this->CrouchButtonDown = false;
	SetMaxWalkSpeed(150.0f);
}

void AMyCharacter::EventSprintPressed() {
	if (!this->CrouchButtonDown)
		SetMaxWalkSpeed(270.0f);
}

void AMyCharacter::EventSprintReleased() {
	if (!this->CrouchButtonDown)
		SetMaxWalkSpeed(150.0f);
}

void AMyCharacter::EventFirePressed() {
	this->FireButtonDown = true;
}

void AMyCharacter::EventFireReleased() {
	this->FireButtonDown = false;
}

void AMyCharacter::EventJumpPressed() {
	Jump();
	UCharacterMovementComponent* characterMovement = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	JumpButtonDown = CanJump();
	FVector velocity = characterMovement->Velocity;
	float lenght = UKismetMathLibrary::VSize(velocity);
	if (lenght > 0.0f) {
		characterMovement->JumpZVelocity = 400.0f;
	} 
	else {
		characterMovement->JumpZVelocity = 400.0f;
	}
}

void AMyCharacter::EventJumpReleased() {
	StopJumping();
	UCharacterMovementComponent* characterMovement = Cast<UCharacterMovementComponent>(GetCharacterMovement());
	JumpButtonDown = false;
	FVector velocity = characterMovement->Velocity;
	float lenght = UKismetMathLibrary::VSize(velocity);
	if (lenght > 0.0f) {
		characterMovement->JumpZVelocity = 400.0f;
	}
	else {
		characterMovement->JumpZVelocity = 400.0f;
	}
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveForward", this, &AMyCharacter::EventMoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMyCharacter::EventMoveRight);
	InputComponent->BindAxis("Turn", this, &AMyCharacter::EventTurn);
	InputComponent->BindAxis("LookUp", this, &AMyCharacter::EventLookUp);

	InputComponent->BindAction("Prom", EInputEvent::IE_Pressed, this, &AMyCharacter::EventPromPressed);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &AMyCharacter::EventCrouchPressed);
	InputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &AMyCharacter::EventCrouchReleased);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AMyCharacter::EventSprintPressed);
	InputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AMyCharacter::EventSprintReleased);
	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &AMyCharacter::EventFirePressed);
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &AMyCharacter::EventFireReleased);
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &AMyCharacter::EventJumpPressed);
	InputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &AMyCharacter::EventJumpReleased);
}

