#pragma once
#include "BeTx/Public/TxCommand.h" // Corrected include path

/**
 * 인벤토리 아이템 추가 커맨드
 */
class FInventoryAddCommand : public IBeTxCommand
{
public:
    explicit FInventoryAddCommand(const FString& InItemId, int32 InQuantity) 
        : ItemId(InItemId), Quantity(InQuantity) {}

    virtual FTxResult Apply(FTxContext& Ctx) override 
    { 
        // 실제로는 인벤토리 시스템에 아이템 추가 로직
        UE_LOG(LogTemp, Log, TEXT("InventoryAdd: Adding %d of %s"), Quantity, *ItemId);
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Applied); 
    }

    virtual FTxResult Commit(FTxContext& Ctx) override 
    { 
        UE_LOG(LogTemp, Log, TEXT("InventoryAdd: Committed %d of %s"), Quantity, *ItemId);
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Committed); 
    }

    virtual FTxResult Rollback(FTxContext& Ctx) override 
    { 
        UE_LOG(LogTemp, Log, TEXT("InventoryAdd: Rolled back %d of %s"), Quantity, *ItemId);
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::RolledBack); 
    }

    virtual FString Name() const override { return TEXT("InventoryAdd"); }
    
    virtual TMap<FString,FString> Payload() const override 
    { 
        return {{TEXT("itemId"), ItemId}, {TEXT("quantity"), LexToString(Quantity)}}; 
    }

    virtual FTxTags Tags() const override
    {
        FTxTags T;
        T.Add(TEXT("cat"), TEXT("inventory")); // category
        T.Add(TEXT("op"),  TEXT("add"));       // operation
        T.Add(TEXT("item"), FName(*ItemId));   // item id
        T.Add(TEXT("qty"),  FName(*LexToString(Quantity))); // quantity
        return T;
    }

private:
    FString ItemId;
    int32 Quantity = 0;
};

/**
 * 인벤토리 아이템 제거 커맨드
 */
class FInventoryRemoveCommand : public IBeTxCommand
{
public:
    explicit FInventoryRemoveCommand(const FString& InItemId, int32 InQuantity) 
        : ItemId(InItemId), Quantity(InQuantity) {}

    virtual FTxResult Apply(FTxContext& Ctx) override 
    { 
        UE_LOG(LogTemp, Log, TEXT("InventoryRemove: Removing %d of %s"), Quantity, *ItemId);
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Applied); 
    }

    virtual FTxResult Commit(FTxContext& Ctx) override 
    { 
        UE_LOG(LogTemp, Log, TEXT("InventoryRemove: Committed %d of %s"), Quantity, *ItemId);
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Committed); 
    }

    virtual FTxResult Rollback(FTxContext& Ctx) override 
    { 
        UE_LOG(LogTemp, Log, TEXT("InventoryRemove: Rolled back %d of %s"), Quantity, *ItemId);
        return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::RolledBack); 
    }

    virtual FString Name() const override { return TEXT("InventoryRemove"); }
    
    virtual TMap<FString,FString> Payload() const override 
    { 
        return {{TEXT("itemId"), ItemId}, {TEXT("quantity"), LexToString(Quantity)}}; 
    }

    virtual FTxTags Tags() const override
    {
        FTxTags T;
        T.Add(TEXT("cat"), TEXT("inventory")); // category
        T.Add(TEXT("op"),  TEXT("remove"));    // operation
        T.Add(TEXT("item"), FName(*ItemId));   // item id
        T.Add(TEXT("qty"),  FName(*LexToString(Quantity))); // quantity
        return T;
    }

private:
    FString ItemId;
    int32 Quantity = 0;
};


