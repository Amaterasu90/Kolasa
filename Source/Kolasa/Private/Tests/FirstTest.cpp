#include "Kolasa.h"
#include "Characters/MyCharacter.h"
#include "Engine.h"
#include "AutomationCommon.h" //To get the wait latent command.
#include "AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFloatTest, "UnitTests.Equal.Float", (EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))

bool FFloatTest::RunTest(const FString& Parameters)
{
	TestEqual<float>("Float equeal", 1.0f, 1.1f);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIntTest, "UnitTests.Equal.Int", (EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))

bool FIntTest::RunTest(const FString& Parameters)
{
	TestEqual<int>("Float equeal", 1,2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraNameBoomCameraTest, "Characer.UnitTests",
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

bool FACharacter_InitializeSpringArmComponent_DefaultConstructor_BoomCameraNameBoomCameraTest::RunTest(const FString& Parameters) {
	UWorld* world = GetWorld();
	if (world)
	{
		FString expected = FString(TEXT("BoomCamera1"));
		FString actual = FString();

		AMyCharacter* character = world->SpawnActor<AMyCharacter>(AMyCharacter::StaticClass());

		actual = character->BoomCamera->GetName();
		TestEqual<FString>("SpringArm name equal", expected, actual);
	}
	return true;
}