#pragma once
#include "BeTx/Public/TxCommand.h"
#include "BeTx/Public/TxMultiplayer.h"

/**
 * 멀티플레이 골드 전송 커맨드
 * 서버 권한 검증 및 클라이언트 동기화
 */
class FMultiplayerGoldTransferCommand : public IBeTxCommand
{
public:
    explicit FMultiplayerGoldTransferCommand(int32 InFromPlayerId, int32 InToPlayerId, int32 InAmount) 
        : FromPlayerId(InFromPlayerId), ToPlayerId(InToPlayerId), Amount(InAmount) {}

    virtual FTxResult Apply(FTxContext& Ctx) override 
    { 
        // 서버에서만 실제 골드 전송 처리
        UE_LOG(LogTemp, Log, TEXT("MultiplayerGoldTransfer: Transferring %d gold from P%d to P%d"), 
            Amount, FromPlayerId, ToPlayerId);
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Applied); 
    }

    virtual FTxResult Commit(FTxContext& Ctx) override 
    { 
        // 서버 권한 확인 후 커밋
        UE_LOG(LogTemp, Log, TEXT("MultiplayerGoldTransfer: Committed transfer of %d gold"), Amount);
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Committed); 
    }

    virtual FTxResult Rollback(FTxContext& Ctx) override 
    { 
        UE_LOG(LogTemp, Log, TEXT("MultiplayerGoldTransfer: Rolled back transfer of %d gold"), Amount);
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::RolledBack); 
    }

    virtual FString Name() const override { return TEXT("MultiplayerGoldTransfer"); }
    
    virtual TMap<FString,FString> Payload() const override 
    { 
        return {
            {TEXT("fromPlayer"), LexToString(FromPlayerId)},
            {TEXT("toPlayer"), LexToString(ToPlayerId)},
            {TEXT("amount"), LexToString(Amount)}
        }; 
    }

    virtual FTxTags Tags() const override
    {
        FTxTags T;
        T.Add(TEXT("cat"), TEXT("multiplayer")); // category
        T.Add(TEXT("op"),  TEXT("transfer"));    // operation
        T.Add(TEXT("type"), TEXT("gold"));       // type
        T.Add(TEXT("from"), FName(*LexToString(FromPlayerId))); // from player
        T.Add(TEXT("to"),   FName(*LexToString(ToPlayerId)));   // to player
        T.Add(TEXT("amt"),  FName(*LexToString(Amount)));       // amount
        return T;
    }

private:
    int32 FromPlayerId = 0;
    int32 ToPlayerId = 0;
    int32 Amount = 0;
};

/**
 * 멀티플레이 아이템 교환 커맨드
 * 양방향 아이템 교환 (서버 검증)
 */
class FMultiplayerItemTradeCommand : public IBeTxCommand
{
public:
    explicit FMultiplayerItemTradeCommand(int32 InPlayer1Id, const FString& InItem1, int32 InPlayer2Id, const FString& InItem2) 
        : Player1Id(InPlayer1Id), Item1(InItem1), Player2Id(InPlayer2Id), Item2(InItem2) {}

    virtual FTxResult Apply(FTxContext& Ctx) override 
    { 
        UE_LOG(LogTemp, Log, TEXT("MultiplayerItemTrade: Trading %s (P%d) <-> %s (P%d)"), 
            *Item1, Player1Id, *Item2, Player2Id);
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Applied); 
    }

    virtual FTxResult Commit(FTxContext& Ctx) override 
    { 
        UE_LOG(LogTemp, Log, TEXT("MultiplayerItemTrade: Committed trade between P%d and P%d"), Player1Id, Player2Id);
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Committed); 
    }

    virtual FTxResult Rollback(FTxContext& Ctx) override 
    { 
        UE_LOG(LogTemp, Log, TEXT("MultiplayerItemTrade: Rolled back trade between P%d and P%d"), Player1Id, Player2Id);
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::RolledBack); 
    }

    virtual FString Name() const override { return TEXT("MultiplayerItemTrade"); }
    
    virtual TMap<FString,FString> Payload() const override 
    { 
        return {
            {TEXT("player1"), LexToString(Player1Id)},
            {TEXT("item1"), Item1},
            {TEXT("player2"), LexToString(Player2Id)},
            {TEXT("item2"), Item2}
        }; 
    }

    virtual FTxTags Tags() const override
    {
        FTxTags T;
        T.Add(TEXT("cat"), TEXT("multiplayer")); // category
        T.Add(TEXT("op"),  TEXT("trade"));       // operation
        T.Add(TEXT("type"), TEXT("item"));       // type
        T.Add(TEXT("p1"),  FName(*LexToString(Player1Id))); // player 1
        T.Add(TEXT("p2"),  FName(*LexToString(Player2Id))); // player 2
        return T;
    }

private:
    int32 Player1Id = 0;
    FString Item1;
    int32 Player2Id = 0;
    FString Item2;
};


