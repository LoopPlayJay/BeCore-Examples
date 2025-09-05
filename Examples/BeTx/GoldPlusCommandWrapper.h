#pragma once

#include "CoreMinimal.h"
#include "BeTx/Public/TxCommandWrapper.h"
#include "GoldPlusCommandWrapper.generated.h"

/**
 * 골드 증가 명령 래퍼 (UTxCommandWrapper 기반)
 * 새로운 아키텍쳐에 맞춘 Blueprint 친화적 구현
 */
UCLASS(BlueprintType, Blueprintable)
class BE_API UGoldPlusCommandWrapper : public UTxCommandWrapper
{
    GENERATED_BODY()

public:
    UGoldPlusCommandWrapper();

    // ===== UTxCommandWrapper 오버라이드 =====
    virtual FString GetCommandName_Implementation() const override;
    virtual TMap<FString, FString> GetPayload_Implementation() const override;
    virtual FTxTags GetTags_Implementation() const override;
    virtual FTxResult ValidateCommand_Implementation(FTxContext& Context) override;
    virtual FTxResult ApplyCommand_Implementation(FTxContext& Context) override;
    virtual FTxResult CommitCommand_Implementation(FTxContext& Context) override;
    virtual FTxResult RollbackCommand_Implementation(FTxContext& Context) override;

    // ===== Blueprint 설정 가능한 프로퍼티 =====
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GoldPlus")
    int32 Delta = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GoldPlus")
    bool bSimulateFailure = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GoldPlus")
    FString Description = TEXT("Gold Plus Command");

    // ===== Blueprint 함수 =====
    UFUNCTION(BlueprintCallable, Category="GoldPlus")
    void SetDelta(int32 NewDelta) { Delta = NewDelta; }

    UFUNCTION(BlueprintCallable, Category="GoldPlus")
    void SetSimulateFailure(bool bSimulate) { bSimulateFailure = bSimulate; }

    UFUNCTION(BlueprintPure, Category="GoldPlus")
    int32 GetDelta() const { return Delta; }

    UFUNCTION(BlueprintPure, Category="GoldPlus")
    bool GetSimulateFailure() const { return bSimulateFailure; }

protected:
    // 생성자에서 초기화
};
