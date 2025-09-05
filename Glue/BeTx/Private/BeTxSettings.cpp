#include "../Public/BeTxSettings.h"

UBeTxSettings::UBeTxSettings()
{
    // 기본 필터 프리셋들 설정
    FilterPresets.Add(TEXT("Gold"), TEXT("Type=Gold"));
    FilterPresets.Add(TEXT("Demo"), TEXT("Category=Demo"));
    FilterPresets.Add(TEXT("Failed"), TEXT("Success=false"));
    FilterPresets.Add(TEXT("Success"), TEXT("Success=true"));
    
    // 풀링 시스템 기본 설정
    InitialPoolSize = 100;
    MaxPoolSize = 1000;
    bEnableCommandPooling = true;
    AutoTrimInterval = 300.0f; // 5분
}

const UBeTxSettings* UBeTxSettings::Get()
{
    return GetDefault<UBeTxSettings>();
}
