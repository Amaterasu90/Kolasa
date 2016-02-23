#include "Kolasa.h"
#include "Characters/CharacterWithoutGravity.h"
#include "Engine.h"
#include "AutomationCommon.h" //To get the wait latent command.
#include "AutomationTest.h"

namespace CharacterTest {
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
}

ACharacterWithoutGravity* GetControlledCharacter(int playerIndex) {
	UWorld* world = CharacterTest::GetWorld();
	if (world) {
		APlayerController *playerController = UGameplayStatics::GetPlayerController(world, playerIndex);
		APawn* pawn = playerController->GetPawn();
		return Cast<ACharacterWithoutGravity>(pawn);
	}
	return NULL;
}

//IMPLEMENT_SIMPLE_AUTOMATION_TEST(FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraRelativeLocationTest, "UnitTests.CharacterWithoutGravity.InitializeSpringArmComponent_DefaultConstructor_BoomCameraRelativeLocationTest",
//	(EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))
//	bool FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraRelativeLocationTest::RunTest(const FString& Parameters) {
//	FVector expected = FVector(0.0f, 0.0f, 40.0f);
//
//	AMyCharacter* character = GetControlledCharacter(0);
//
//	if (character) {
//		FVector actual = character->BoomCamera->RelativeLocation;
//		TestEqual<FVector>("SpringArm location equal", expected, actual);
//	}
//	return true;
//}