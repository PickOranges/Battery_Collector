//#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


DEFINE_LOG_CATEGORY_STATIC(TestLog, Log, All);
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMultiThreadTest, "Battery_Collector_Test.PublicTest.MultiThreadTest",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

	bool FMultiThreadTest::RunTest(const FString& Parameters)
{
	UE_LOG(TestLog, Log, TEXT("Hello"));
	return true;
}
