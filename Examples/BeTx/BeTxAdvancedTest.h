#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeTxAdvancedTest.generated.h"

class UTxSubsystem;

/**
 * BeTx 고급 테스트 액터
 * 다양한 커맨드 타입과 리플레이 기능 테스트
 */
UCLASS()
class BE_API ABeTxAdvancedTest : public AActor
{
    GENERATED_BODY()

public:
    ABeTxAdvancedTest();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    /**
     * 기본 테스트 실행
     */
    UFUNCTION(BlueprintCallable, Category="BeTx|Test")
    void RunBasicTests();

    /**
     * 인벤토리 테스트 실행
     */
    UFUNCTION(BlueprintCallable, Category="BeTx|Test")
    void RunInventoryTests();

    /**
     * 리플레이 테스트 실행
     */
    UFUNCTION(BlueprintCallable, Category="BeTx|Test")
    void RunReplayTests();

    /**
     * 태그 필터링 테스트 실행
     */
    UFUNCTION(BlueprintCallable, Category="BeTx|Test")
    void RunTagFilterTests();

    /**
     * 성능 테스트 실행
     */
    UFUNCTION(BlueprintCallable, Category="BeTx|Test")
    void RunPerformanceTests();

private:
    void LogTestResult(const FString& TestName, bool bSuccess);
    void TestGoldPlusCommands();
    void TestInventoryCommands();
    void TestReplaySystem();
    void TestTagFiltering();
    void TestPerformance();
    void TestSmallStringPerformance();

    UPROPERTY()
    UTxSubsystem* TxSubsystem = nullptr;

    int32 TestCounter = 0;
};


