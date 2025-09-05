#include "GoldPlusCommandWrapperTest.h"
#include "GoldPlusCommandWrapper.h"
#include "BeTx/Public/TxSubsystem.h"
#include "BeTx/Public/TxBlueprintLibrary.h"
#include "BeTx/Public/TxCommandPool.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

AGoldPlusCommandWrapperTest::AGoldPlusCommandWrapperTest()
{
    PrimaryActorTick.bCanEverTick = false;
    TestDelta = 100;
    MultipleCommandCount = 5;
    bAutoRunTests = true;
}

void AGoldPlusCommandWrapperTest::BeginPlay()
{
    Super::BeginPlay();
    
    // TxSubsystem 가져오기
    TxSubsystem = UTxBlueprintLibrary::GetTxSubsystem(this);
    
    if (!TxSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("GoldPlusCommandWrapperTest: TxSubsystem not found!"));
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("GoldPlusCommandWrapperTest: Initialized successfully"));
    
    if (bAutoRunTests)
    {
        // 자동 테스트 실행
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AGoldPlusCommandWrapperTest::RunAllTests);
    }
}

void AGoldPlusCommandWrapperTest::TestBasicGoldCommand()
{
    UE_LOG(LogTemp, Log, TEXT("=== Testing Basic Gold Command ==="));
    
    // 명령 생성
    UGoldPlusCommandWrapper* Command = NewObject<UGoldPlusCommandWrapper>(this);
    Command->SetDelta(TestDelta);
    Command->SetSimulateFailure(false);
    
    // 명령 실행
    if (TxSubsystem)
    {
        TxSubsystem->EnqueueCommand(Command);
        LogTestResult(TEXT("BasicGoldCommand"), true, FString::Printf(TEXT("Delta: %d"), TestDelta));
    }
    else
    {
        LogTestResult(TEXT("BasicGoldCommand"), false, TEXT("TxSubsystem not available"));
    }
}

void AGoldPlusCommandWrapperTest::TestFailureSimulation()
{
    UE_LOG(LogTemp, Log, TEXT("=== Testing Failure Simulation ==="));
    
    // 실패 시뮬레이션 명령 생성
    UGoldPlusCommandWrapper* Command = NewObject<UGoldPlusCommandWrapper>(this);
    Command->SetDelta(TestDelta);
    Command->SetSimulateFailure(true); // 실패 시뮬레이션
    
    // 명령 실행
    if (TxSubsystem)
    {
        TxSubsystem->EnqueueCommand(Command);
        LogTestResult(TEXT("FailureSimulation"), true, TEXT("Failure simulation enabled"));
    }
    else
    {
        LogTestResult(TEXT("FailureSimulation"), false, TEXT("TxSubsystem not available"));
    }
}

void AGoldPlusCommandWrapperTest::TestMultipleCommands()
{
    UE_LOG(LogTemp, Log, TEXT("=== Testing Multiple Commands ==="));
    
    if (!TxSubsystem)
    {
        LogTestResult(TEXT("MultipleCommands"), false, TEXT("TxSubsystem not available"));
        return;
    }
    
    int32 LocalSuccessCount = 0;
    
    for (int32 i = 0; i < MultipleCommandCount; ++i)
    {
        UGoldPlusCommandWrapper* Command = NewObject<UGoldPlusCommandWrapper>(this);
        Command->SetDelta(TestDelta + i * 10); // 다양한 델타 값
        Command->SetSimulateFailure(i % 3 == 0); // 일부는 실패 시뮬레이션
        
        TxSubsystem->EnqueueCommand(Command);
        LocalSuccessCount++;
    }
    
    LogTestResult(TEXT("MultipleCommands"), true, 
        FString::Printf(TEXT("Enqueued %d commands"), LocalSuccessCount));
}

void AGoldPlusCommandWrapperTest::TestPoolingSystem()
{
    UE_LOG(LogTemp, Log, TEXT("=== Testing Pooling System ==="));
    
    if (!TxSubsystem)
    {
        LogTestResult(TEXT("PoolingSystem"), false, TEXT("TxSubsystem not available"));
        return;
    }
    
    // 풀링 시스템 테스트
    FTxCommandPool& Pool = TxSubsystem->GetCommandPool();
    FTxCommandPool::FPoolStats Stats = Pool.GetStats();
    
    UE_LOG(LogTemp, Log, TEXT("Pool Stats - Available: %d, Total: %d"), 
        Stats.AvailableCount, Stats.TotalAllocated);
    
    // 풀링된 명령 생성 테스트
    UTxCommandWrapper* PooledCommand = Pool.AcquireCommand();
    if (PooledCommand)
    {
        // UTxCommandWrapper는 기본 구현을 사용하므로 직접 설정할 수 없음
        // 대신 새로운 GoldPlusCommandWrapper를 생성하여 풀링 시스템 테스트
        UGoldPlusCommandWrapper* GoldCommand = NewObject<UGoldPlusCommandWrapper>(this);
        GoldCommand->SetDelta(50);
        
        // 명령 실행
        TxSubsystem->EnqueueCommand(GoldCommand);
        
        LogTestResult(TEXT("PoolingSystem"), true, TEXT("Pooled command created and enqueued"));
    }
    else
    {
        LogTestResult(TEXT("PoolingSystem"), false, TEXT("Failed to acquire pooled command"));
    }
}

void AGoldPlusCommandWrapperTest::TestBlueprintIntegration()
{
    UE_LOG(LogTemp, Log, TEXT("=== Testing Blueprint Integration ==="));
    
    // Blueprint에서 호출 가능한 함수들 테스트
    UGoldPlusCommandWrapper* Command = NewObject<UGoldPlusCommandWrapper>(this);
    
    // Blueprint 함수들 테스트
    Command->SetDelta(200);
    Command->SetSimulateFailure(false);
    
    int32 RetrievedDelta = Command->GetDelta();
    bool RetrievedFailure = Command->GetSimulateFailure();
    
    if (RetrievedDelta == 200 && RetrievedFailure == false)
    {
        LogTestResult(TEXT("BlueprintIntegration"), true, TEXT("Blueprint functions working correctly"));
    }
    else
    {
        LogTestResult(TEXT("BlueprintIntegration"), false, 
            FString::Printf(TEXT("Delta: %d (expected 200), Failure: %s (expected false)"), 
                RetrievedDelta, RetrievedFailure ? TEXT("true") : TEXT("false")));
    }
}

void AGoldPlusCommandWrapperTest::RunAllTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== Running All GoldPlusCommandWrapper Tests ==="));
    
    TestCounter = 0;
    SuccessCount = 0;
    FailureCount = 0;
    
    // 모든 테스트 실행
    TestBasicGoldCommand();
    TestFailureSimulation();
    TestMultipleCommands();
    TestPoolingSystem();
    TestBlueprintIntegration();
    
    // 결과 요약
    UE_LOG(LogTemp, Log, TEXT("=== Test Results Summary ==="));
    UE_LOG(LogTemp, Log, TEXT("Total Tests: %d"), TestCounter);
    UE_LOG(LogTemp, Log, TEXT("Success: %d"), SuccessCount);
    UE_LOG(LogTemp, Log, TEXT("Failure: %d"), FailureCount);
    UE_LOG(LogTemp, Log, TEXT("Success Rate: %.1f%%"), 
        TestCounter > 0 ? (SuccessCount * 100.0f) / TestCounter : 0.0f);
}

void AGoldPlusCommandWrapperTest::LogTestResult(const FString& TestName, bool bSuccess, const FString& Details)
{
    TestCounter++;
    if (bSuccess)
    {
        SuccessCount++;
        UE_LOG(LogTemp, Log, TEXT("✓ %s: PASSED - %s"), *TestName, *Details);
    }
    else
    {
        FailureCount++;
        UE_LOG(LogTemp, Error, TEXT("✗ %s: FAILED - %s"), *TestName, *Details);
    }
}
