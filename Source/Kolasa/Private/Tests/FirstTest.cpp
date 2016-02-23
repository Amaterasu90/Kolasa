#include "Kolasa.h"
#include "Characters/MyCharacter.h"
#include "Engine.h"
#include "AutomationCommon.h" //To get the wait latent command.
#include "AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraNameBoomCameraTest, "Character.UnitTests.FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraNameBoomCameraTest",
	(EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))
UWorld* GetWorld()
{
	if (GEngine)
	{
		if (GEngine->GetWorldContexts().Num() == 1)
		{
			return GEngine->GetWorldContexts()[0].World();
		}
	}
	return nullptr;
}

AMyCharacter* GetControlledCharacter(int playerIndex) {
	UWorld* world = GetWorld();
	if (world) {
		APlayerController *playerController = UGameplayStatics::GetPlayerController(world, playerIndex);
		APawn* pawn = playerController->GetPawn();
		return Cast<AMyCharacter>(pawn);
	}
	return NULL;
}

bool FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraNameBoomCameraTest::RunTest(const FString& Parameters) {

	FString expected = FString(TEXT("BoomCamera"));

	AMyCharacter* character = GetControlledCharacter(0);

	if (character) {
		FString actual = character->BoomCamera->GetName();
		TestEqual<FString>("SpringArm name equal", expected, actual);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraRelativeLocationTest, "Character.UnitTests.FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraRelativeLocationTest",
	(EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))
bool FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraRelativeLocationTest::RunTest(const FString& Parameters) {
	FVector expected = FVector(0.0f, 0.0f, 40.0f);

	AMyCharacter* character = GetControlledCharacter(0);

	if (character) {
		FVector actual = character->BoomCamera->RelativeLocation;
		TestEqual<FVector>("SpringArm location equal", expected, actual);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraAttachToRootComponentTest,
	"Character.UnitTests.FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraAttachToRootComponentTest",
	(EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter)) 
bool FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraAttachToRootComponentTest::RunTest(const FString& Parameters) {
	AMyCharacter *character = GetControlledCharacter(0);

	if (character) {
		USceneComponent *component = character->GetRootComponent();
		TestTrue("SpringArm attached to RootComponent", component->AttachChildren.Contains(character->BoomCamera));
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FACharacter_InitializeCameraComponent_DefaultConstructor_PlayerCameraNamePlayerCameraTest,
	"Character.UnitTests.FACharacter_InitializeCameraComponent_DefaultConstructor_PlayerCameraNamePlayerCameraTest",
	(EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))
bool FACharacter_InitializeCameraComponent_DefaultConstructor_PlayerCameraNamePlayerCameraTest::RunTest(const FString& Parameters) {

	FString expected = FString(TEXT("PlayerCamera"));

	AMyCharacter* character = GetControlledCharacter(0);

	if (character) {
		FString actual = character->PlayerCamera->GetName();
		TestEqual<FString>("Camera name equal", expected, actual);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FACharacter_InitializeCameraComponent_DefaultConstructor_PlayerCameraAttachToBoomCameraTest,
	"Character.UnitTests.FACharacter_InitializeCameraComponent_DefaultConstructor_PlayerCameraAttachToBoomCameraTest",
	(EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))
bool FACharacter_InitializeCameraComponent_DefaultConstructor_PlayerCameraAttachToBoomCameraTest::RunTest(const FString& Parameters) {
	AMyCharacter *character = GetControlledCharacter(0);

	if (character) {
		UCameraComponent* camera = character->PlayerCamera;
		USceneComponent* springArm = character->BoomCamera;
		bool condition = springArm->AttachChildren.Contains(camera);
		TestTrue("BoomCamera contains PlayerCamera", condition);
	}
	return true;
}
