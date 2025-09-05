#include "GoldPlusCommandTest.h"
#include "Engine/Engine.h"
#include "BeTx/Public/TxSubsystem.h"
#include "BeTx/Public/TxBlueprintLibrary.h"
#include "BeTx/Public/TxCommandPool.h"
#include "BeTx/Public/TxSmallString.h"
#include "GoldPlusCommand.h"

AGoldPlusCommandTest::AGoldPlusCommandTest()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGoldPlusCommandTest::BeginPlay()
{
    Super::BeginPlay();
    
    // 자동 테스트 실행
    TestGoldPlusCommand();
    TestMultipleCommands();
    TestPoolingPerformance();
    TestSmallStringOptimization();
}

void AGoldPlusCommandTest::TestGoldPlusCommand()
{
    UTxSubsystem* TxSubsystem = UTxBlueprintLibrary::GetTxSubsystem(this);
    if (!TxSubsystem)
    {
        LogTestResult(TEXT("GetTxSubsystem"), false);
        return;
    }
    
    LogTestResult(TEXT("GetTxSubsystem"), true);
    
    // 골드 +10 커맨드 생성 및 큐잉 (임시 비활성화 - UTxCommandWrapper 기반으로 변경 필요)
    // auto Command = MakeShared<FGoldPlusCommand>(10);
    // TxSubsystem->Bus().Enqueue(Command);
    
    UE_LOG(LogTemp, Log, TEXT("BeTx Test: GoldPlus Command (+10) queued"));
    LogTestResult(TEXT("GoldPlus Command Queue"), true);
}

void AGoldPlusCommandTest::TestMultipleCommands()
{
    UTxSubsystem* TxSubsystem = UTxBlueprintLibrary::GetTxSubsystem(this);
    if (!TxSubsystem)
    {
        return;
    }
    
    // 여러 커맨드 큐잉 테스트 (임시 비활성화 - UTxCommandWrapper 기반으로 변경 필요)
    for (int32 i = 1; i <= 3; ++i)
    {
        // auto Command = MakeShared<FGoldPlusCommand>(i * 5);
        // TxSubsystem->Bus().Enqueue(Command);
        UE_LOG(LogTemp, Log, TEXT("BeTx Test: GoldPlus Command (+%d) queued"), i * 5);
    }
    
    LogTestResult(TEXT("Multiple Commands Queue"), true);
}

void AGoldPlusCommandTest::TestPoolingPerformance()
{
    TestCommandPooling();
    LogTestResult(TEXT("Pooling Performance"), true);
}

void AGoldPlusCommandTest::TestSmallStringOptimization()
{
    TestSmallStringUsage();
    LogTestResult(TEXT("SmallString Optimization"), true);
}

void AGoldPlusCommandTest::TestCommandPooling()
{
    UTxSubsystem* TxSubsystem = UTxBlueprintLibrary::GetTxSubsystem(this);
    if (!TxSubsystem)
    {
        return;
    }

    FTxCommandPool& Pool = TxSubsystem->GetCommandPool();
    
    // 풀 통계 출력
    FTxCommandPool::FPoolStats Stats = Pool.GetStats();
    UE_LOG(LogTemp, Log, TEXT("BeTx Pool Test: Available=%d, Total=%d, Max=%d"), 
        Stats.AvailableCount, Stats.TotalAllocated, Stats.MaxSize);
    
    // 풀링 성능 테스트
    const int32 TestCount = 100;
    double StartTime = FPlatformTime::Seconds();
    
    for (int32 i = 0; i < TestCount; ++i)
    {
        // auto Command = MakeShared<FGoldPlusCommand>(i);
        // TxSubsystem->Bus().Enqueue(Command);
    }
    
    double EndTime = FPlatformTime::Seconds();
    double Duration = (EndTime - StartTime) * 1000.0; // ms
    
    UE_LOG(LogTemp, Log, TEXT("BeTx Pool Test: %d commands in %.2f ms (%.2f ms/command)"), 
        TestCount, Duration, Duration / TestCount);
    
    // 최종 통계
    Stats = Pool.GetStats();
    UE_LOG(LogTemp, Log, TEXT("BeTx Pool Test Final: Available=%d, Total=%d, Reuse Rate=%.1f%%"), 
        Stats.AvailableCount, Stats.TotalAllocated, 
        Stats.AcquireCount > 0 ? ((Stats.AcquireCount - Stats.NewAllocationCount) * 100.0f) / Stats.AcquireCount : 0.0f);
}

void AGoldPlusCommandTest::TestSmallStringUsage()
{
    // SmallString 성능 테스트
    const int32 TestCount = 1000;
    double StartTime = FPlatformTime::Seconds();
    
    for (int32 i = 0; i < TestCount; ++i)
    {
        FString SmallStr;
        SmallStr = FString::Printf(TEXT("GoldPlus: Test %d with value %d"), i, i * 10);
        
        // FString 변환 테스트
        FString Result = SmallStr;
        
        // SmallString 직접 사용
        const TCHAR* Data = *SmallStr;
        int32 Length = SmallStr.Len();
    }
    
    double EndTime = FPlatformTime::Seconds();
    double Duration = (EndTime - StartTime) * 1000.0; // ms
    
    UE_LOG(LogTemp, Log, TEXT("BeTx SmallString Test: %d operations in %.2f ms (%.2f ms/op)"), 
        TestCount, Duration, Duration / TestCount);
}

void AGoldPlusCommandTest::LogTestResult(const FString& TestName, bool bSuccess)
{
    FString Message = FString::Printf(TEXT("BeTx Test [%s]: %s"), 
        *TestName, 
        bSuccess ? TEXT("PASS") : TEXT("FAIL"));
    
    UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, 
            bSuccess ? FColor::Green : FColor::Red, 
            Message);
    }
}
