#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "BeTxSettings.generated.h"

/**
 * BeTx 시스템 설정
 * 프로젝트 설정에서 구성 가능한 파라미터들
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="BeTx Settings"))
class BE_API UBeTxSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UBeTxSettings();

    // UDeveloperSettings 인터페이스
    virtual FName GetCategoryName() const override { return TEXT("BeTx"); }

    /** 오버레이 표시 여부 */
    UPROPERTY(config, EditAnywhere, Category="Overlay", meta=(DisplayName="Show Overlay"))
    bool bShowOverlay = false;

    /** 오버레이에 표시할 최대 라인 수 */
    UPROPERTY(config, EditAnywhere, Category="Overlay", meta=(DisplayName="Max Overlay Lines", ClampMin="1", ClampMax="50"))
    int32 MaxOverlayLines = 10;

    /** Pump 예산 시간 (ms) */
    UPROPERTY(config, EditAnywhere, Category="Performance", meta=(DisplayName="Pump Budget (ms)", ClampMin="0.1", ClampMax="100.0"))
    float PumpBudgetMs = 1.0f;

    /** 기본 필터 키 */
    UPROPERTY(config, EditAnywhere, Category="Filter", meta=(DisplayName="Default Filter Key"))
    FString DefaultFilterKey = TEXT("");

    /** 기본 필터 값 */
    UPROPERTY(config, EditAnywhere, Category="Filter", meta=(DisplayName="Default Filter Value"))
    FString DefaultFilterValue = TEXT("");

    /** 실패한 명령만 표시 */
    UPROPERTY(config, EditAnywhere, Category="Filter", meta=(DisplayName="Show Only Failed"))
    bool bShowOnlyFailed = false;

    /** 필터 프리셋들 */
    UPROPERTY(config, EditAnywhere, Category="Filter Presets", meta=(DisplayName="Filter Presets"))
    TMap<FString, FString> FilterPresets;

    // ===== 풀링 시스템 설정 =====
    /** 명령 풀 초기 크기 */
    UPROPERTY(config, EditAnywhere, Category="Pooling", meta=(DisplayName="Initial Pool Size", ClampMin="10", ClampMax="1000"))
    int32 InitialPoolSize = 100;

    /** 명령 풀 최대 크기 */
    UPROPERTY(config, EditAnywhere, Category="Pooling", meta=(DisplayName="Max Pool Size", ClampMin="100", ClampMax="10000"))
    int32 MaxPoolSize = 1000;

    /** 풀링 시스템 활성화 */
    UPROPERTY(config, EditAnywhere, Category="Pooling", meta=(DisplayName="Enable Command Pooling"))
    bool bEnableCommandPooling = true;

    /** 자동 풀 정리 간격 (초) */
    UPROPERTY(config, EditAnywhere, Category="Pooling", meta=(DisplayName="Auto Trim Interval (seconds)", ClampMin="0", ClampMax="3600"))
    float AutoTrimInterval = 300.0f; // 5분

    /** 설정 인스턴스 가져오기 */
    static const UBeTxSettings* Get();
};
