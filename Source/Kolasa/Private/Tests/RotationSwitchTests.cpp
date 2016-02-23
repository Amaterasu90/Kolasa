#include "Kolasa.h"
#include "Characters/RotationSwitch.h"
#include "Engine.h"
#include "AutomationCommon.h" //To get the wait latent command.
#include "AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRS_IsActive_Activate_true, "UnitTests.SwitchInterface.RotationSwitch.IsActive_Activate_true", (EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))
bool FRS_IsActive_Activate_true::RunTest(const FString& Parameters)
{
	RotationSwitch blockable;

	blockable.Activate();

	bool condition = blockable.IsActive();
	TestTrue("IsActiveRotation true", condition);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRS_IsActive_Deactivate_false, "UnitTests.SwitchInterface.RotationSwitch.IsActive_Deactivate_false", (EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))
bool FRS_IsActive_Deactivate_false::RunTest(const FString& Parameters)
{
	RotationSwitch blockable;

	blockable.Deactivate();

	bool condition = blockable.IsActive();
	TestFalse("IsActiveRotation false", condition);
	return true;
}