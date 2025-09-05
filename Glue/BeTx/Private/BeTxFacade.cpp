#include "../Public/BeTxFacade.h"
#include "../Public/BeTxSettings.h"
#include "BeTx/Public/TxBlueprintLibrary.h"
#include "BeTx/Public/TxSubsystem.h"
#include "BeTx/Public/TxCommandWrapper.h"
#include "../../Examples/BeTx/GoldPlusCommandWrapper.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

// ===== 커맨드 큐잉 =====
void UBeTxFacade::EnqueueCommand(UObject* WorldContextObject, UObject* CommandObject)
{
    if (UTxSubsystem* TxSubsystem = GetTxSubsystem(WorldContextObject))
    {
        if (UTxCommandWrapper* CommandWrapper = Cast<UTxCommandWrapper>(CommandObject))
        {
            TxSubsystem->EnqueueCommand(CommandWrapper);
            UE_LOG(LogTemp, Log, TEXT("BeTxFacade: Command '%s' enqueued successfully"), *CommandWrapper->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("BeTxFacade: CommandObject is not a UTxCommandWrapper"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("BeTxFacade: TxSubsystem not found"));
    }
}

void UBeTxFacade::RunGoldExample(UObject* WorldContextObject, int32 Delta)
{
    // 예제 코드는 별도로 구현되어야 함
    UE_LOG(LogTemp, Warning, TEXT("BeTxFacade: RunGoldExample is deprecated. Use Examples/BeTx/GoldPlusCommandTest instead."));
}

void UBeTxFacade::RunGoldPlusExample(UObject* WorldContextObject, int32 Delta, bool bSimulateFailure)
{
    if (UTxSubsystem* TxSubsystem = GetTxSubsystem(WorldContextObject))
    {
        // 새로운 UTxCommandWrapper 기반 골드 명령 생성
        UGoldPlusCommandWrapper* Command = NewObject<UGoldPlusCommandWrapper>(WorldContextObject);
        Command->SetDelta(Delta);
        Command->SetSimulateFailure(bSimulateFailure);
        
        // 명령 실행
        TxSubsystem->EnqueueCommand(Command);
        
        UE_LOG(LogTemp, Log, TEXT("BeTxFacade: GoldPlus example executed - Delta: %d, SimulateFailure: %s"), 
            Delta, bSimulateFailure ? TEXT("true") : TEXT("false"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("BeTxFacade: TxSubsystem not found for GoldPlus example"));
    }
}

// ===== 치트 명령어들 =====
void UBeTxFacade::Betx_Toggle(UObject* WorldContextObject)
{
    if (UTxSubsystem* Tx = GetTxSubsystem(WorldContextObject))
    {
        const UBeTxSettings* Settings = UBeTxSettings::Get();
        bool bNewState = !Settings->bShowOverlay;
        
        Tx->SetOverlayVisible(bNewState);
        UE_LOG(LogTemp, Log, TEXT("BeTxFacade: Toggle overlay = %s"), bNewState ? TEXT("ON") : TEXT("OFF"));
    }
}

void UBeTxFacade::Betx_FilterKeyVal(UObject* WorldContextObject, const FString& Key, const FString& Value)
{
    if (UTxSubsystem* Tx = GetTxSubsystem(WorldContextObject))
    {
        FName FilterKey = Key.IsEmpty() ? NAME_None : FName(*Key);
        FName FilterValue = Value.IsEmpty() ? NAME_None : FName(*Value);
        Tx->SetFilter(FilterKey, FilterValue.ToString());
        UE_LOG(LogTemp, Log, TEXT("BeTxFacade: Set filter Key='%s', Value='%s'"), *Key, *Value);
    }
}

void UBeTxFacade::Betx_OnlyFail(UObject* WorldContextObject, int32 bShowOnlyFail)
{
    if (UTxSubsystem* Tx = GetTxSubsystem(WorldContextObject))
    {
        Tx->SetShowOnlyFailed(bShowOnlyFail != 0);
        UE_LOG(LogTemp, Log, TEXT("BeTxFacade: Set show only failed = %s"), (bShowOnlyFail != 0) ? TEXT("true") : TEXT("false"));
    }
}

void UBeTxFacade::Betx_ClearFilter(UObject* WorldContextObject)
{
    Betx_FilterKeyVal(WorldContextObject, TEXT(""), TEXT(""));
    UE_LOG(LogTemp, Log, TEXT("BeTxFacade: Clear filter"));
}

void UBeTxFacade::Betx_ApplyPreset(UObject* WorldContextObject, const FString& PresetName)
{
    ApplyFilterPreset(WorldContextObject, PresetName);
}

void UBeTxFacade::Betx_ListPresets(UObject* WorldContextObject)
{
    const UBeTxSettings* Settings = UBeTxSettings::Get();
    UE_LOG(LogTemp, Log, TEXT("BeTxFacade: Available presets:"));
    
    for (const auto& Preset : Settings->FilterPresets)
    {
        UE_LOG(LogTemp, Log, TEXT("  %s: %s"), *Preset.Key, *Preset.Value);
    }
}

// ===== 오버레이 제어 =====
void UBeTxFacade::SetOverlayVisible(UObject* WorldContextObject, bool bVisible)
{
    if (UTxSubsystem* Tx = GetTxSubsystem(WorldContextObject))
    {
        Tx->SetOverlayVisible(bVisible);
        UE_LOG(LogTemp, Log, TEXT("BeTxFacade: Set overlay visible = %s"), bVisible ? TEXT("true") : TEXT("false"));
    }
}

void UBeTxFacade::SetFilter(UObject* WorldContextObject, const FString& Key, const FString& Value)
{
    Betx_FilterKeyVal(WorldContextObject, Key, Value);
}

void UBeTxFacade::SetShowOnlyFailed(UObject* WorldContextObject, bool bOnlyFailed)
{
    Betx_OnlyFail(WorldContextObject, bOnlyFailed ? 1 : 0);
}

void UBeTxFacade::ApplyFilterPreset(UObject* WorldContextObject, const FString& PresetName)
{
    const UBeTxSettings* Settings = UBeTxSettings::Get();
    
    if (const FString* PresetValue = Settings->FilterPresets.Find(PresetName))
    {
        // "Key=Value" 형태의 문자열을 파싱
        FString Key, Value;
        if (PresetValue->Split(TEXT("="), &Key, &Value))
        {
            SetFilter(WorldContextObject, Key, Value);
            UE_LOG(LogTemp, Log, TEXT("BeTxFacade: Applied preset '%s': %s=%s"), *PresetName, *Key, *Value);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("BeTxFacade: Invalid preset format: %s"), **PresetValue);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("BeTxFacade: Preset '%s' not found"), *PresetName);
    }
}

void UBeTxFacade::PrintCurrentSettings(UObject* WorldContextObject)
{
    const UBeTxSettings* Settings = UBeTxSettings::Get();
    
    UE_LOG(LogTemp, Log, TEXT("BeTxFacade: Current Settings:"));
    UE_LOG(LogTemp, Log, TEXT("  Show Overlay: %s"), Settings->bShowOverlay ? TEXT("true") : TEXT("false"));
    UE_LOG(LogTemp, Log, TEXT("  Max Overlay Lines: %d"), Settings->MaxOverlayLines);
    UE_LOG(LogTemp, Log, TEXT("  Pump Budget: %.2f ms"), Settings->PumpBudgetMs);
    UE_LOG(LogTemp, Log, TEXT("  Default Filter: %s=%s"), *Settings->DefaultFilterKey, *Settings->DefaultFilterValue);
    UE_LOG(LogTemp, Log, TEXT("  Show Only Failed: %s"), Settings->bShowOnlyFailed ? TEXT("true") : TEXT("false"));
}

// ===== 내부 헬퍼 함수들 =====
UTxSubsystem* UBeTxFacade::GetTxSubsystem(UObject* WorldContextObject)
{
    return UTxBlueprintLibrary::GetTxSubsystem(WorldContextObject);
}

void UBeTxFacade::ApplySettingsToSubsystem(UTxSubsystem* TxSubsystem)
{
    if (!TxSubsystem) return;
    
    const UBeTxSettings* Settings = UBeTxSettings::Get();
    
    // 설정을 서브시스템에 적용
    TxSubsystem->SetOverlayVisible(Settings->bShowOverlay);
    TxSubsystem->SetFilter(FName(*Settings->DefaultFilterKey), Settings->DefaultFilterValue);
    TxSubsystem->SetShowOnlyFailed(Settings->bShowOnlyFailed);
}

// ===== 풀링 시스템 관리 =====
void UBeTxFacade::ShowPoolStats(UObject* WorldContextObject)
{
    if (UTxSubsystem* TxSubsystem = GetTxSubsystem(WorldContextObject))
    {
        FTxCommandPool& Pool = TxSubsystem->GetCommandPool();
        FTxCommandPool::FPoolStats Stats = Pool.GetStats();
        
        UE_LOG(LogTemp, Log, TEXT("=== BeTx Command Pool Statistics ==="));
        UE_LOG(LogTemp, Log, TEXT("Available Commands: %d"), Stats.AvailableCount);
        UE_LOG(LogTemp, Log, TEXT("Total Allocated: %d"), Stats.TotalAllocated);
        UE_LOG(LogTemp, Log, TEXT("Max Pool Size: %d"), Stats.MaxSize);
        UE_LOG(LogTemp, Log, TEXT("Acquire Count: %d"), Stats.AcquireCount);
        UE_LOG(LogTemp, Log, TEXT("Release Count: %d"), Stats.ReleaseCount);
        UE_LOG(LogTemp, Log, TEXT("New Allocations: %d"), Stats.NewAllocationCount);
        
        float ReuseRate = Stats.AcquireCount > 0 ? 
            ((Stats.AcquireCount - Stats.NewAllocationCount) * 100.0f) / Stats.AcquireCount : 0.0f;
        UE_LOG(LogTemp, Log, TEXT("Reuse Rate: %.1f%%"), ReuseRate);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("BeTxFacade: TxSubsystem not found"));
    }
}

void UBeTxFacade::TrimCommandPool(UObject* WorldContextObject)
{
    if (UTxSubsystem* TxSubsystem = GetTxSubsystem(WorldContextObject))
    {
        FTxCommandPool& Pool = TxSubsystem->GetCommandPool();
        Pool.TrimPool();
        UE_LOG(LogTemp, Log, TEXT("BeTxFacade: Command pool trimmed successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("BeTxFacade: TxSubsystem not found"));
    }
}

UTxCommandWrapper* UBeTxFacade::CreatePooledCommand(UObject* WorldContextObject)
{
    if (UTxSubsystem* TxSubsystem = GetTxSubsystem(WorldContextObject))
    {
        FTxCommandPool& Pool = TxSubsystem->GetCommandPool();
        return Pool.AcquireCommand();
    }
    return nullptr;
}

void UBeTxFacade::ReturnPooledCommand(UObject* WorldContextObject, UTxCommandWrapper* Command)
{
    if (UTxSubsystem* TxSubsystem = GetTxSubsystem(WorldContextObject))
    {
        if (Command)
        {
            FTxCommandPool& Pool = TxSubsystem->GetCommandPool();
            Pool.ReleaseCommand(Command);
        }
    }
}
