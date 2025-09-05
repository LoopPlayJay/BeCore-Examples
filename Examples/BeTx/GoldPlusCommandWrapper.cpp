#include "GoldPlusCommandWrapper.h"
#include "BeTx/Public/TxTypes.h"
#include "BeTx/Public/TxResult.h"
#include "BeTx/Public/TxContext.h"

UGoldPlusCommandWrapper::UGoldPlusCommandWrapper()
{
    // 기본값 설정
    Delta = 100;
    bSimulateFailure = false;
    Description = TEXT("Gold Plus Command");
}

FString UGoldPlusCommandWrapper::GetCommandName_Implementation() const
{
    return TEXT("GoldPlus");
}

TMap<FString, FString> UGoldPlusCommandWrapper::GetPayload_Implementation() const
{
    TMap<FString, FString> Payload;
    Payload.Add(TEXT("delta"), LexToString(Delta));
    Payload.Add(TEXT("simulate_failure"), bSimulateFailure ? TEXT("true") : TEXT("false"));
    return Payload;
}

FTxTags UGoldPlusCommandWrapper::GetTags_Implementation() const
{
    FTxTags Tags;
    Tags.Add(TEXT("cat"), TEXT("inventory")); // category
    Tags.Add(TEXT("op"), TEXT("gold"));       // operation
    Tags.Add(TEXT("delta"), FName(*LexToString(Delta))); // 값도 태그로
    return Tags;
}

FTxResult UGoldPlusCommandWrapper::ValidateCommand_Implementation(FTxContext& Context)
{
    // 유효성 검사
    if (Delta <= 0)
    {
        return FTxResult::Fail(ETxError::Validate, FString::Printf(TEXT("Invalid delta: %d"), Delta));
    }
    
    if (Delta > 10000)
    {
        return FTxResult::Fail(ETxError::Validate, FString::Printf(TEXT("Delta too large: %d"), Delta));
    }
    
    UE_LOG(LogTemp, Log, TEXT("GoldPlus: Validation passed for delta %d"), Delta);
    return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Validated);
}

FTxResult UGoldPlusCommandWrapper::ApplyCommand_Implementation(FTxContext& Context)
{
    // SmallString을 사용한 로깅 최적화
    FString LogMsg = FString::Printf(TEXT("GoldPlus: Adding %d gold"), Delta);
    UE_LOG(LogTemp, Log, TEXT("%s"), *LogMsg);
    
    // TODO: GameState.Gold += Delta;
    // 실제 게임 상태 업데이트는 여기서 구현
    
    return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Applied);
}

FTxResult UGoldPlusCommandWrapper::CommitCommand_Implementation(FTxContext& Context)
{
    if (bSimulateFailure)
    {
        FString ErrorMsg = FString::Printf(TEXT("GoldPlus: Server denied %d gold (simulated failure)"), Delta);
        UE_LOG(LogTemp, Warning, TEXT("%s"), *ErrorMsg);
        return FTxResult::Fail(ETxError::Network, ErrorMsg);
    }
    
    UE_LOG(LogTemp, Log, TEXT("GoldPlus: Committed %d gold successfully"), Delta);
    return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::Committed);
}

FTxResult UGoldPlusCommandWrapper::RollbackCommand_Implementation(FTxContext& Context)
{
    FString LogMsg = FString::Printf(TEXT("GoldPlus: Rolling back %d gold"), Delta);
    UE_LOG(LogTemp, Log, TEXT("%s"), *LogMsg);
    
    // TODO: GameState.Gold -= Delta;
    // 실제 게임 상태 롤백은 여기서 구현
    
    return FTxResult::Ok(FGuid::NewGuid(), ETxPhase::RolledBack);
}
