#include "Kolasa.h"
#include "Characters/MoveSwitch.h"
#include "Engine.h"
#include "AutomationCommon.h" //To get the wait latent command.
#include "AutomationTest.h"

MoveSwitch defaultMoveSwitch;
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIsAcviteMove_ActivateMove_true, "UnitTests.IBlockable.IsAcviteMove_ActivateMove_true", (EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))

bool FIsAcviteMove_ActivateMove_true::RunTest(const FString& Parameters)
{

	MoveSwitch blockable = defaultMoveSwitch;
	blockable.Activate();

	bool condition = blockable.IsActive();
	TestTrue("IsActiveMove true", condition);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIsAcviteMove_DeactivateMove_false, "UnitTests.IBlockable.IsAcviteMove_DeactivateMove_false", (EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))
bool FIsAcviteMove_DeactivateMove_false::RunTest(const FString& Parameters)
{
	MoveSwitch blockable = defaultMoveSwitch;
	blockable.Deactivate();

	bool condition = blockable.IsActive();
	TestFalse("IsActiveMove false", condition);
	return true;
}