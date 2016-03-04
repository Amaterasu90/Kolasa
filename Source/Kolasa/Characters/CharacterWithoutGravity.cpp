// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Libraries/RunnerMath.h"
#include "DirectionMovementComponent.h"
#include "CharacterWithoutGravity.h"
#include "GravityMovementComponent.h"


// Sets default values
ACharacterWithoutGravity::ACharacterWithoutGravity() : ACharacterWithoutGravity(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"),
	TEXT("AnimBlueprint'/Game/AnimStarterPack/UE4ASP_HeroTPP_AnimBlueprint.UE4ASP_HeroTPP_AnimBlueprint'"))
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Type::Player0;
}

void ACharacterWithoutGravity::InitializeAnimationClass(TCHAR* animBlueprintPath) {
	UAnimBlueprint *animation = LoadObject<UAnimBlueprint>(NULL, animBlueprintPath);
	if (animation != nullptr) {
		Mesh->SetAnimInstanceClass(animation->GetAnimBlueprintGeneratedClass());
	}
}

void ACharacterWithoutGravity::InitializeAnimationBlueprint(TCHAR* animBlueprintPath) {
	//must static loading
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> animContainer(animBlueprintPath);
	if (animContainer.Succeeded()) {
		Mesh->AnimClass = (UAnimBlueprintGeneratedClass*)animContainer.Object->GetAnimBlueprintGeneratedClass();
	}
}

void ACharacterWithoutGravity::InitializeMovementComponent(){
	ForwardMovementComponent = CreateDefaultSubobject<UForwardMovementComponent>("ForwardComponent");
	ForwardMovementComponent->UpdatedComponent = RootComponent;
	GravityMovementComponent = CreateDefaultSubobject<UGravityMovementComponent>("GravityComponent");
	GravityMovementComponent->UpdatedComponent = RootComponent;
	HorizontalMovementComponent = CreateDefaultSubobject<USideMovementComponent>("HorizontalComponent");
	HorizontalMovementComponent->UpdatedComponent = RootComponent;
	RightMovementComponent = CreateDefaultSubobject<URightMovementComponent>("RightComponent");
	RightMovementComponent->UpdatedComponent = RootComponent;
	/*ConvexRotationComponent = CreateDefaultSubobject<UForwardRotationComponent>("ConvexRotationComponent");
	ConvexRotationComponent->UpdatedComponent = RootComponent;*/
	LeftMovementComponent = CreateDefaultSubobject<ULeftMovementComponent>("LeftComponent");
	LeftMovementComponent->UpdatedComponent = RootComponent;
	SkeletalComponent = CreateDefaultSubobject<USkeletalOrientationComponent>("SkeletalOrientationComponent");
	SkeletalComponent->UpdatedComponent = Mesh;
}

void ACharacterWithoutGravity::InitializeDownTrace(){
	/*BackDownTrace = CreateDefaultSubobject<UArrowComponent>("BackDownTrace");
	BackDownTrace->SetRelativeLocation(-FVector(Capsule->GetScaledCapsuleRadius(),0.0f,0.0f));
	BackDownTrace->RelativeRotation = FRotator(270.0f, 0.0f, 0.0f);
	BackDownTrace->AttachTo(RootComponent);

	FrontDownTrace = CreateDefaultSubobject<UArrowComponent>("FrontDownTrace");
	FrontDownTrace->SetRelativeLocation(FVector(Capsule->GetScaledCapsuleRadius(), 0.0f, 0.0f));
	FrontDownTrace->RelativeRotation = FRotator(270.0f, 0.0f, 0.0f);
	FrontDownTrace->AttachTo(RootComponent);*/
}

void ACharacterWithoutGravity::InitializeRightTrace() {
	RightTrace = CreateDefaultSubobject<UArrowComponent>("RightTrace");
	RightTrace->SetRelativeLocation(ACharacterWithoutGravity::DefaultTraceLocation);
	RightTrace->AddRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	RightTrace->AttachTo(RootComponent);
}

void ACharacterWithoutGravity::InitializeLeftTrace() {
	LeftTrace = CreateDefaultSubobject<UArrowComponent>("LeftTrace");
	LeftTrace->SetRelativeLocation(ACharacterWithoutGravity::DefaultTraceLocation);
	LeftTrace->AddRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	LeftTrace->AttachTo(RootComponent);
}

FRotator ACharacterWithoutGravity::GetYawRotator() {
	FRotator ActualRotation = GetControlRotation();
	return FRotator(0.0f, ActualRotation.Yaw, 0.0f);
}

void ACharacterWithoutGravity::EventMoveRight(float AxisValue){
	
	if (AxisValue != 0.0f) {
		FVector RightVector = GetActorRightVector();
		HorizontalMovementComponent->AddInputVector(RunnerMath::GetCleared(RightVector, 0.01f) * AxisValue);
		
		this->SkeletalComponent->AddInputAxis(AxisValue);
	}
}

void ACharacterWithoutGravity::EventMeshPitchRotation(float AxisValue){
	FRotator currentRotation = Mesh->RelativeRotation;
	if (AxisValue != 0.0f)
		currentRotation = currentRotation + FRotator(0.0f, AxisValue,0.0f);
	Mesh->SetRelativeRotation(currentRotation);
}

ACharacterWithoutGravity::ACharacterWithoutGravity(TCHAR * skeletalMeshPath, TCHAR * animBlueprintPath)
{
	InitializeCapsule();
	InitializeStaticMesh(skeletalMeshPath);
	InitializeSpringArm();
	InitializeCamera();
	InitializeAnimationClass(animBlueprintPath);
	InitializeAnimationBlueprint(animBlueprintPath);
	InitializeMovementComponent();
	InitializeLeftTrace();
	InitializeRightTrace();
	InitializeDownTrace();
}

void ACharacterWithoutGravity::InitializeStaticMesh(TCHAR* skeletalMeshPath) {
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("CharacterMesh");
	Mesh->AttachTo(RootComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshContainer(skeletalMeshPath);
	if (MeshContainer.Succeeded()) {
		Mesh->SetSkeletalMesh(MeshContainer.Object);
	}

	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
	Mesh->AddRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
}

void ACharacterWithoutGravity::InitializeSpringArm(){
	BoomCamera = CreateDefaultSubobject<USpringArmComponent>("BoomCamera");
	BoomCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	USpringArmComponent * springArm = Cast<USpringArmComponent>(BoomCamera);
	springArm->bUsePawnControlRotation = false;
	BoomCamera->AttachTo(RootComponent);
}

void ACharacterWithoutGravity::InitializeCamera()
{
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>("PlayerCamera");
	PlayerCamera->AttachTo(BoomCamera);
}

// Called when the game starts or when spawned
void ACharacterWithoutGravity::BeginPlay(){
	Super::BeginPlay();
	
	ForwardMovementComponent->SetBlockDown(*GravityMovementComponent);
	GravityMovementComponent->SetBlockSide(*HorizontalMovementComponent);

	RightMovementComponent->SetOppositeSiteInterface(*LeftMovementComponent);
	RightMovementComponent->SetSideMovementComponent(*HorizontalMovementComponent);

	RayProvider right(RightTrace);
	RightMovementComponent->SetScanRay(right);

	LeftMovementComponent->SetOppositeSiteInterface(*RightMovementComponent);
	LeftMovementComponent->SetSideMovementComponent(*HorizontalMovementComponent);

	RayProvider left(LeftTrace);
	LeftMovementComponent->SetScanRay(left);

	startRotation = Mesh->RelativeRotation;
	
	this->SkeletalComponent->SetRotationBalance(Mesh->RelativeRotation);
}

// Called every frame
void ACharacterWithoutGravity::Tick( float DeltaTime ){
	Super::Tick( DeltaTime );
}

// Called to bind functionality to input
void ACharacterWithoutGravity::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveRight", this, &ACharacterWithoutGravity::EventMoveRight);
	InputComponent->BindAxis("MeshPitch", this, &ACharacterWithoutGravity::EventMeshPitchRotation);
}

UPawnMovementComponent * ACharacterWithoutGravity::GetMovementComponent() const
{
	return GravityMovementComponent;
}

void ACharacterWithoutGravity::InitializeCapsule()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetCapsuleHalfHeight(100.0f);
	Capsule->SetCapsuleRadius(45.0f);
	FName name = FName("Pawn");
	Capsule->SetCollisionProfileName(name);
	RootComponent = Capsule;
}