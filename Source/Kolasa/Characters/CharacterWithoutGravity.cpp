// Fill out your copyright notice in the Description page of Project Settings.

#include "Kolasa.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "CharacterWithoutGravity.h"


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
		Mesh->AnimBlueprintGeneratedClass = (UAnimBlueprintGeneratedClass*)animContainer.Object->GetAnimBlueprintGeneratedClass();
	}
}

void ACharacterWithoutGravity::InitializeMovementComponent(){
	MovementComponent = CreateDefaultSubobject<UNGPawnMovementComponent>("PawnMovementComponent");
	MovementComponent->UpdatedComponent = RootComponent;
}

void ACharacterWithoutGravity::InitializeForwardTrace(){
	ForwardTrace = CreateDefaultSubobject<UArrowComponent>("ForwardTrace");
	ForwardTrace->SetRelativeLocation(FVector(0.0f, 0.0f, -75.0f));
	ForwardTrace->AttachTo(RootComponent);
}

void ACharacterWithoutGravity::InitializeDownTrace(){
	DownTrace = CreateDefaultSubobject<UArrowComponent>("DownTrace");
	DownTrace->SetRelativeLocation(FVector(0.0f, 0.0f, -75.0f));
	DownTrace->AddRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	DownTrace->AttachTo(RootComponent);
}

FRotator ACharacterWithoutGravity::GetYawRotator() {
	FRotator ActualRotation = GetControlRotation();
	return FRotator(0.0f, ActualRotation.Yaw, 0.0f);
}

void ACharacterWithoutGravity::EventMoveRight(float AxisValue){
	FRotator UpdateRotator = GetYawRotator();
	FVector RightVector = GetActorRightVector();
	if (AxisValue != 0.0f)
		AddMovementInput(RightVector, AxisValue * 100.0f, false);
}

ACharacterWithoutGravity::ACharacterWithoutGravity(TCHAR * skeletalMeshPath, TCHAR * animBlueprintPath)
{
	InitializeCapsule();
	InitializeMovementComponent();
	InitializeStaticMesh(skeletalMeshPath);
	InitializeSpringArm();
	InitializeCamera();
	InitializeAnimationClass(animBlueprintPath);
	InitializeAnimationBlueprint(animBlueprintPath);
	InitializeDownTrace();
	InitializeForwardTrace();
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

void ACharacterWithoutGravity::InitializeSpringArm()
{
	BoomCamera = CreateDefaultSubobject<USpringArmComponent>("BoomCamera");
	BoomCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	USpringArmComponent * springArm = Cast<USpringArmComponent>(BoomCamera);
	springArm->bUsePawnControlRotation = true;
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
}

// Called every frame
void ACharacterWithoutGravity::Tick( float DeltaTime ){
	Super::Tick( DeltaTime );

	MoveForward(DeltaTime);
	FHitResult hitResult;
	if (IsHitObstacle(ForwardTrace, 100.0f, hitResult)){
		RotateOrtogonalToPlane(hitResult);
	}
}

// Called to bind functionality to input
void ACharacterWithoutGravity::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("MoveRight", this, &ACharacterWithoutGravity::EventMoveRight);
}

UPawnMovementComponent * ACharacterWithoutGravity::GetMovementComponent() const
{
	return MovementComponent;
}

void ACharacterWithoutGravity::InitializeCapsule()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetCapsuleHalfHeight(100.0f);
	Capsule->SetCapsuleRadius(45.0f);
	RootComponent = Capsule;
}

void ACharacterWithoutGravity::RotateOrtogonalToPlane(FHitResult & OutHit)
{
	FVector normalToPlane = OutHit.ImpactNormal;
	FVector forwardActor = GetActorForwardVector();
	FVector upActor = GetActorUpVector();

	FVector crossForwardAndUp = UKismetMathLibrary::Cross_VectorVector(forwardActor, upActor);
	FVector newForward = UKismetMathLibrary::Cross_VectorVector(normalToPlane, crossForwardAndUp);
	FVector newRight = UKismetMathLibrary::Cross_VectorVector(normalToPlane, newForward);
	FVector newUp = upActor;

	FRotator newRotation = UKismetMathLibrary::MakeRotationFromAxes(newForward, newRight, newUp);

	SetActorRotation(newRotation);
}

void ACharacterWithoutGravity::MoveForward(float DeltaTime)
{
	FHitResult hitResult;
	if (!IsHitObstacle(DownTrace, 25.0f, hitResult)) {
		AddActorWorldOffset(DownTrace->GetForwardVector()*10.0f);
	}
	else if (!IsHitObstacle(ForwardTrace, 100.0f, hitResult)) {
		AddActorWorldOffset(ForwardTrace->GetForwardVector()*10.0f);
	}
	else {
		AddActorWorldOffset(FVector(270.0f, 0.0f, 0.0f));
	}
}

bool ACharacterWithoutGravity::IsHitObstacle(const UArrowComponent* arm, float armLenght, FHitResult& outResult)
{

	FVector currentLocation = arm->K2_GetComponentLocation();
	FRotator currentRotation = arm->K2_GetComponentRotation();
	FVector forwardVector = UKismetMathLibrary::GetForwardVector(currentRotation);
	FVector scanArm = currentLocation + forwardVector * armLenght;

	TArray<AActor*> ignore;
	return UKismetSystemLibrary::LineTraceSingle_NEW(this, currentLocation, scanArm, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ignore, EDrawDebugTrace::ForDuration, outResult, true);
}