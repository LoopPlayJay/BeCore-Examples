#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoldPlusCommandTest.generated.h"

class UTxSubsystem;

/**
 * 골드 커맨드 테스트 액터
 */
UCLASS()
class BE_API AGoldPlusCommandTest : public AActor
{
    GENERATED_BODY()

public:
    AGoldPlusCommandTest();

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category="BeTx Test")
    void TestGoldPlusCommand();

    UFUNCTION(BlueprintCallable, Category="BeTx Test")
    void TestMultipleCommands();

    UFUNCTION(BlueprintCallable, Category="BeTx Test")
    void TestPoolingPerformance();

    UFUNCTION(BlueprintCallable, Category="BeTx Test")
    void TestSmallStringOptimization();

private:
    void LogTestResult(const FString& TestName, bool bSuccess);
    void TestCommandPooling();
    void TestSmallStringUsage();
};
