#include "Kolasa.h"
#include "Characters/ASwitch.h"
#include "Engine.h"
#include "AutomationCommon.h" //To get the wait latent command.
#include "AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FA_IsActive_Activate_true, "UnitTests.SwitchInterface.ASwitch.IsActive_Activate_true", (EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))
bool FA_IsActive_Activate_true::RunTest(const FString& Parameters)
{
	ASwitch blockable;

	blockable.Activate();

	bool condition = blockable.IsActive();
	TestTrue("IsActiveMove true", condition);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FA_IsActive_Deactivate_false, "UnitTests.SwitchInterface.ASwitch.IsActive_Deactivate_false", (EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))
bool FA_IsActive_Deactivate_false::RunTest(const FString& Parameters)
{
	ASwitch blockable;

	blockable.Deactivate();

	bool condition = blockable.IsActive();
	TestFalse("IsActiveMove false", condition);
	return true;
}