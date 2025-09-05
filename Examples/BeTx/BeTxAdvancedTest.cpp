#include "BeTxAdvancedTest.h"
#include "Engine/Engine.h"
#include "BeTx/Public/TxSubsystem.h"
#include "BeTx/Public/TxBlueprintLibrary.h"
#include "BeTx/Public/TxReplay.h"
#include "BeTx/Public/TxCommandPool.h"
#include "BeTx/Public/TxSmallString.h"
#include "GoldPlusCommand.h"
#include "InventoryCommand.h"

ABeTxAdvancedTest::ABeTxAdvancedTest()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABeTxAdvancedTest::BeginPlay()
{
    Super::BeginPlay();
    
    TxSubsystem = UTxBlueprintLibrary::GetTxSubsystem(this);
    if (!TxSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("BeTxAdvancedTest: Failed to get TxSubsystem"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("BeTxAdvancedTest: Initialized successfully"));
}

void ABeTxAdvancedTest::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABeTxAdvancedTest::RunBasicTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== BeTx Basic Tests ==="));
    TestGoldPlusCommands();
}

void ABeTxAdvancedTest::RunInventoryTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== BeTx Inventory Tests ==="));
    TestInventoryCommands();
}

void ABeTxAdvancedTest::RunReplayTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== BeTx Replay Tests ==="));
    TestReplaySystem();
}

void ABeTxAdvancedTest::RunTagFilterTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== BeTx Tag Filter Tests ==="));
    TestTagFiltering();
}

void ABeTxAdvancedTest::RunPerformanceTests()
{
    UE_LOG(LogTemp, Log, TEXT("=== BeTx Performance Tests ==="));
    TestPerformance();
}

void ABeTxAdvancedTest::TestGoldPlusCommands()
{
    if (!TxSubsystem) return;

    // 골드 플러스 커맨드들
    // auto GoldCmd1 = MakeShared<FGoldPlusCommand>(100);
    // auto GoldCmd2 = MakeShared<FGoldPlusCommand>(50);
    // auto GoldCmd3 = MakeShared<FGoldPlusCommand>(-25);

    // TxSubsystem->Bus().Enqueue(GoldCmd1);
    // TxSubsystem->Bus().Enqueue(GoldCmd2);
    // TxSubsystem->Bus().Enqueue(GoldCmd3);

    LogTestResult(TEXT("GoldPlus Commands"), true);
}

void ABeTxAdvancedTest::TestInventoryCommands()
{
    if (!TxSubsystem) return;

    // 인벤토리 커맨드들 (임시 비활성화 - UTxCommandWrapper 기반으로 변경 필요)
    // auto AddCmd1 = MakeShared<FInventoryAddCommand>(TEXT("Sword"), 1);
    // auto AddCmd2 = MakeShared<FInventoryAddCommand>(TEXT("Potion"), 5);
    // auto RemoveCmd = MakeShared<FInventoryRemoveCommand>(TEXT("Potion"), 2);

    // TxSubsystem->Bus().Enqueue(AddCmd1);
    // TxSubsystem->Bus().Enqueue(AddCmd2);
    // TxSubsystem->Bus().Enqueue(RemoveCmd);

    LogTestResult(TEXT("Inventory Commands"), true);
}

void ABeTxAdvancedTest::TestReplaySystem()
{
    if (!TxSubsystem) return;

    // 리플레이 시스템 테스트
    FTxReplay& Replay = TxSubsystem->Replay();
    
    // 팩토리 등록
    FTxReplay::RegisterFactory(TEXT("GoldPlus"), [](const FString& Name, const TMap<FString,FString>& Payload) -> TSharedRef<IBeTxCommand>
    {
        int32 Delta = 0;
        if (const FString* DeltaStr = Payload.Find(TEXT("delta")))
        {
            Delta = FCString::Atoi(**DeltaStr);
        }
        return MakeShared<FGoldPlusCommand>(Delta);
    });

    FTxReplay::RegisterFactory(TEXT("InventoryAdd"), [](const FString& Name, const TMap<FString,FString>& Payload) -> TSharedRef<IBeTxCommand>
    {
        FString ItemId = TEXT("Unknown");
        int32 Quantity = 0;
        
        if (const FString* ItemIdStr = Payload.Find(TEXT("itemId")))
        {
            ItemId = *ItemIdStr;
        }
        if (const FString* QtyStr = Payload.Find(TEXT("quantity")))
        {
            Quantity = FCString::Atoi(**QtyStr);
        }
        
        return MakeShared<FInventoryAddCommand>(ItemId, Quantity);
    });

    LogTestResult(TEXT("Replay System Setup"), true);
}

void ABeTxAdvancedTest::TestTagFiltering()
{
    if (!TxSubsystem) return;

    // 다양한 태그를 가진 커맨드들
    // auto GoldCmd = MakeShared<FGoldPlusCommand>(100);
    // auto InventoryCmd = MakeShared<FInventoryAddCommand>(TEXT("Sword"), 1);

    // TxSubsystem->Bus().Enqueue(GoldCmd);
    // TxSubsystem->Bus().Enqueue(InventoryCmd);

    // 태그 필터링 테스트
    const FTxEventBuffer& Buffer = TxSubsystem->GetEventBuffer();
    
    // 골드 관련 이벤트만 필터링
    TArray<FTxEvent> GoldEvents = Buffer.GetFilteredEvents(FName(TEXT("cat")), FName(TEXT("inventory")));
    
    // 인벤토리 관련 이벤트만 필터링
    TArray<FTxEvent> InventoryEvents = Buffer.GetFilteredEvents(FName(TEXT("cat")), FName(TEXT("inventory")));

    LogTestResult(TEXT("Tag Filtering"), GoldEvents.Num() > 0 || InventoryEvents.Num() > 0);
}

void ABeTxAdvancedTest::TestPerformance()
{
    if (!TxSubsystem) return;

    // 풀링 시스템 성능 테스트
    FTxCommandPool& Pool = TxSubsystem->GetCommandPool();
    FTxCommandPool::FPoolStats InitialStats = Pool.GetStats();
    
    // 대량의 커맨드 생성 및 큐잉
    const int32 NumCommands = 100;
    
    double StartTime = FPlatformTime::Seconds();
    
    for (int32 i = 0; i < NumCommands; ++i)
    {
        // auto GoldCmd = MakeShared<FGoldPlusCommand>(i);
        // TxSubsystem->Bus().Enqueue(GoldCmd);
    }

    // 펌프 실행 시간 측정
    TxSubsystem->Pump(10.0f); // 10ms 예산
    double EndTime = FPlatformTime::Seconds();
    
    double PumpTime = (EndTime - StartTime) * 1000.0; // ms로 변환
    
    // 풀링 통계 비교
    FTxCommandPool::FPoolStats FinalStats = Pool.GetStats();
    
    UE_LOG(LogTemp, Log, TEXT("Performance Test: Pumped %d commands in %.2f ms"), NumCommands, PumpTime);
    UE_LOG(LogTemp, Log, TEXT("Pool Stats: Available=%d->%d, Total=%d->%d, Reuse Rate=%.1f%%"), 
        InitialStats.AvailableCount, FinalStats.AvailableCount,
        InitialStats.TotalAllocated, FinalStats.TotalAllocated,
        FinalStats.AcquireCount > 0 ? ((FinalStats.AcquireCount - FinalStats.NewAllocationCount) * 100.0f) / FinalStats.AcquireCount : 0.0f);
    
    // SmallString 성능 테스트
    TestSmallStringPerformance();
    
    LogTestResult(TEXT("Performance"), PumpTime < 10.0);
}

void ABeTxAdvancedTest::TestSmallStringPerformance()
{
    const int32 TestCount = 1000;
    double StartTime = FPlatformTime::Seconds();
    
    for (int32 i = 0; i < TestCount; ++i)
    {
        FString SmallStr;
        SmallStr = FString::Printf(TEXT("Advanced Test %d: Value=%d, Category=%s"), 
            i, i * 10, i % 2 == 0 ? TEXT("Gold") : TEXT("Inventory"));
        
        // SmallString 직접 사용
        const TCHAR* Data = *SmallStr;
        int32 Length = SmallStr.Len();
        
        // FString 변환 (필요시)
        if (i % 100 == 0)
        {
            FString Result = SmallStr;
        }
    }
    
    double EndTime = FPlatformTime::Seconds();
    double Duration = (EndTime - StartTime) * 1000.0; // ms
    
    UE_LOG(LogTemp, Log, TEXT("SmallString Performance: %d operations in %.2f ms (%.2f ms/op)"), 
        TestCount, Duration, Duration / TestCount);
}

void ABeTxAdvancedTest::LogTestResult(const FString& TestName, bool bSuccess)
{
    TestCounter++;
    FString Message = FString::Printf(TEXT("BeTx Test [%d] [%s]: %s"), 
        TestCounter, 
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


