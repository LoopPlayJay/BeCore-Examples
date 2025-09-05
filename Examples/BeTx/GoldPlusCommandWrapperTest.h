#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoldPlusCommandWrapperTest.generated.h"

class UTxSubsystem;
class UGoldPlusCommandWrapper;

/**
 * 골드 커맨드 래퍼 테스트 액터 (UTxCommandWrapper 기반)
 * 새로운 아키텍쳐에 맞춘 테스트 환경
 */
UCLASS()
class BE_API AGoldPlusCommandWrapperTest : public AActor
{
    GENERATED_BODY()

public:
    AGoldPlusCommandWrapperTest();

protected:
    virtual void BeginPlay() override;

    // ===== Blueprint 호출 가능한 테스트 함수들 =====
    UFUNCTION(BlueprintCallable, Category="BeTx|Test")
    void TestBasicGoldCommand();

    UFUNCTION(BlueprintCallable, Category="BeTx|Test")
    void TestFailureSimulation();

    UFUNCTION(BlueprintCallable, Category="BeTx|Test")
    void TestMultipleCommands();

    UFUNCTION(BlueprintCallable, Category="BeTx|Test")
    void TestPoolingSystem();

    UFUNCTION(BlueprintCallable, Category="BeTx|Test")
    void TestBlueprintIntegration();

    UFUNCTION(BlueprintCallable, Category="BeTx|Test")
    void RunAllTests();

    // ===== Blueprint 설정 가능한 프로퍼티 =====
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test Settings")
    int32 TestDelta = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test Settings")
    int32 MultipleCommandCount = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Test Settings")
    bool bAutoRunTests = true;

private:
    void LogTestResult(const FString& TestName, bool bSuccess, const FString& Details = TEXT(""));
    void TestCommandCreation();
    void TestCommandValidation();
    void TestCommandExecution();
    void TestPoolingPerformance();
    void TestBlueprintCompatibility();

    UPROPERTY()
    UTxSubsystem* TxSubsystem = nullptr;

    int32 TestCounter = 0;
    int32 SuccessCount = 0;
    int32 FailureCount = 0;
};
