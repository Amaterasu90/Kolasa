#include "Kolasa.h"
#include "AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFloatTest, "UnitTests.Equal.Float", (EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))

bool FFloatTest::RunTest(const FString& Parameters)
{
	TestEqual<float>("Float equeal", 1.0f, 1.1f);
	//TestTrue("FHotReloadExampleTest Succes",true);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FIntTest, "UnitTests.Equal.Int", (EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))

bool FIntTest::RunTest(const FString& Parameters)
{
	TestEqual<int>("Float equeal", 1,2);
	//TestTrue("FHotReloadExampleTest Succes",true);
	return true;
}