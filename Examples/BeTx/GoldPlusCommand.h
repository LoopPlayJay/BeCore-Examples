#pragma once

#include "BeTx/Public/TxCommand.h"
#include "BeTx/Public/TxSmallString.h"

/**
 * 골드 증가 데모 커맨드
 * Commit 실패 시뮬레이션으로 Rollback 테스트
 * 풀링 시스템과 SmallString 최적화 적용
 */
class FGoldPlusCommand : public IBeTxCommand
{
public:
    explicit FGoldPlusCommand(int32 InDelta) : Delta(InDelta) {}

    virtual FTxResult Apply(FTxContext& Ctx) override
    {
        // SmallString을 사용한 로깅 최적화
        FString LogMsg;
        LogMsg = FString::Printf(TEXT("GoldPlus: Adding %d gold"), Delta);
        UE_LOG(LogTemp, Log, TEXT("%s"), *LogMsg);
        
        // TODO: GameState.Gold += Delta;
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Applied);
    }

    virtual FTxResult Commit(FTxContext& Ctx) override
    {
        const bool bServerOk = false; // 실패 시뮬레이션
        if (!bServerOk) 
        {
            FString ErrorMsg;
            ErrorMsg = FString::Printf(TEXT("GoldPlus: Server denied %d gold"), Delta);
            return FTxResult::Fail(ETxError::Network, ErrorMsg);
        }
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Committed);
    }

    virtual FTxResult Rollback(FTxContext& Ctx) override
    {
        FString LogMsg;
        LogMsg = FString::Printf(TEXT("GoldPlus: Rolling back %d gold"), Delta);
        UE_LOG(LogTemp, Log, TEXT("%s"), *LogMsg);
        
        // TODO: GameState.Gold -= Delta;
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::RolledBack);
    }

    virtual FTxResult Compensate(FTxContext& Ctx) override
    {
        // 비즈니스 로직에 맞는 보상: 골드 대신 아이템 지급
        FString CompMsg;
        CompMsg = FString::Printf(TEXT("GoldPlus: Compensating %d gold with items"), Delta);
        UE_LOG(LogTemp, Log, TEXT("%s"), *CompMsg);
        
        // TODO: Give compensation items
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::RolledBack);
    }

    virtual FString Name() const override { return TEXT("GoldPlus"); }

    virtual TMap<FString,FString> Payload() const override
    {
        return {{TEXT("delta"), LexToString(Delta)}};
    }

    virtual FTxTags Tags() const override
    {
        FTxTags T;
        T.Add(TEXT("cat"), TEXT("inventory")); // category
        T.Add(TEXT("op"),  TEXT("gold"));      // operation
        T.Add(TEXT("delta"), FName(*LexToString(Delta))); // 값도 태그로
        return T;
    }

private:
    int32 Delta = 0;
};
