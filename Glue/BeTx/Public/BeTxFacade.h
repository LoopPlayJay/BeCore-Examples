#pragma once

#include "CoreMinimal.h"
#include "BeTxFacade.generated.h"

class IBeTxCommand;

/**
 * BeTx 시스템의 Glue 레이어
 * BP/치트에서 사용하는 진입점 제공
 * 태그 필터/오버레이/치트 기능을 통합 관리
 */
UCLASS()
class BE_API UBeTxFacade : public UObject
{
    GENERATED_BODY()

public:
    // ===== 커맨드 큐잉 =====
    /**
     * 커맨드 큐잉
     */
    UFUNCTION(BlueprintCallable, Category="BeTx", meta=(WorldContext="WorldContextObject"))
    static void EnqueueCommand(UObject* WorldContextObject, UObject* CommandObject);

    /**
     * 골드 증가 예제 실행 (Deprecated)
     * @note 예제는 Examples/BeTx/GoldPlusCommandTest를 사용하세요
     */
    UFUNCTION(BlueprintCallable, Category="BeTx", meta=(WorldContext="WorldContextObject", DeprecatedFunction, DeprecationMessage="Use Examples/BeTx/GoldPlusCommandTest instead"))
    static void RunGoldExample(UObject* WorldContextObject, int32 Delta);

    /**
     * 새로운 UTxCommandWrapper 기반 골드 명령 생성 및 실행
     */
    UFUNCTION(BlueprintCallable, Category="BeTx|Examples", meta=(WorldContext="WorldContextObject"))
    static void RunGoldPlusExample(UObject* WorldContextObject, int32 Delta, bool bSimulateFailure = false);

    // ===== 치트 명령어들 (Exec 함수) =====
    /** 오버레이 토글 */
    UFUNCTION(Exec)
    static void Betx_Toggle(UObject* WorldContextObject);

    /** 키/값 필터 설정 */
    UFUNCTION(Exec)
    static void Betx_FilterKeyVal(UObject* WorldContextObject, const FString& Key, const FString& Value);

    /** 실패한 명령만 표시 */
    UFUNCTION(Exec)
    static void Betx_OnlyFail(UObject* WorldContextObject, int32 bShowOnlyFail);

    /** 필터 초기화 */
    UFUNCTION(Exec)
    static void Betx_ClearFilter(UObject* WorldContextObject);

    /** 프리셋 적용 */
    UFUNCTION(Exec)
    static void Betx_ApplyPreset(UObject* WorldContextObject, const FString& PresetName);

    /** 프리셋 목록 출력 */
    UFUNCTION(Exec)
    static void Betx_ListPresets(UObject* WorldContextObject);

    // ===== 오버레이 제어 =====
    /** 오버레이 표시 설정 */
    UFUNCTION(BlueprintCallable, Category="BeTx", meta=(WorldContext="WorldContextObject"))
    static void SetOverlayVisible(UObject* WorldContextObject, bool bVisible);

    /** 필터 설정 */
    UFUNCTION(BlueprintCallable, Category="BeTx", meta=(WorldContext="WorldContextObject"))
    static void SetFilter(UObject* WorldContextObject, const FString& Key, const FString& Value);

    /** 실패한 명령만 표시 설정 */
    UFUNCTION(BlueprintCallable, Category="BeTx", meta=(WorldContext="WorldContextObject"))
    static void SetShowOnlyFailed(UObject* WorldContextObject, bool bOnlyFailed);

    /** 프리셋 적용 */
    UFUNCTION(BlueprintCallable, Category="BeTx", meta=(WorldContext="WorldContextObject"))
    static void ApplyFilterPreset(UObject* WorldContextObject, const FString& PresetName);

    /** 현재 설정 상태 출력 */
    UFUNCTION(BlueprintCallable, Category="BeTx", meta=(WorldContext="WorldContextObject"))
    static void PrintCurrentSettings(UObject* WorldContextObject);

    // ===== 풀링 시스템 관리 =====
    /** 명령 풀 통계 출력 */
    UFUNCTION(BlueprintCallable, Category="BeTx|Pooling", meta=(WorldContext="WorldContextObject"))
    static void ShowPoolStats(UObject* WorldContextObject);

    /** 명령 풀 정리 */
    UFUNCTION(BlueprintCallable, Category="BeTx|Pooling", meta=(WorldContext="WorldContextObject"))
    static void TrimCommandPool(UObject* WorldContextObject);

    /** 풀링된 명령 래퍼 생성 */
    UFUNCTION(BlueprintCallable, Category="BeTx|Pooling", meta=(WorldContext="WorldContextObject"))
    static class UTxCommandWrapper* CreatePooledCommand(UObject* WorldContextObject);

    /** 명령 래퍼 반환 */
    UFUNCTION(BlueprintCallable, Category="BeTx|Pooling", meta=(WorldContext="WorldContextObject"))
    static void ReturnPooledCommand(UObject* WorldContextObject, UTxCommandWrapper* Command);

private:
    // 내부 헬퍼 함수들
    static class UTxSubsystem* GetTxSubsystem(UObject* WorldContextObject);
    static void ApplySettingsToSubsystem(UTxSubsystem* TxSubsystem);
};
