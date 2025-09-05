# BeTx 예제 모음 v1.2.0

BeTx 플러그인의 다양한 사용 예제들을 제공합니다. UTxCommandWrapper 기반 새로운 아키텍쳐와 풀링 시스템, SmallString 최적화가 적용된 최신 버전입니다.

## 📁 예제 구조

```
BeTxExamples/
├── GoldPlusCommand/          # 기본 골드 명령 예제 (TSharedRef 기반)
├── GoldPlusCommandWrapper/   # 새로운 UTxCommandWrapper 기반 골드 명령 예제
├── MultiplayerCommand/       # 멀티플레이어 명령 예제
├── TaggedCommand/           # 태그 기반 명령 예제
├── examples.json            # 예제 메타데이터
└── README.md               # 이 파일
```

## 🎯 예제 목록

### 기본 예제 (Basic)

#### GoldPlusCommand
- **설명**: 골드 증가 명령 예제 (풀링 시스템 적용)
- **난이도**: 초급
- **파일**: `GoldPlusCommand.h`, `GoldPlusCommand.cpp`, `GoldPlusCommandTest.h`, `GoldPlusCommandTest.cpp`
- **사용법**: 기본적인 명령 패턴 구현 방법을 보여줍니다.
- **새로운 기능**: 
  - SmallString을 사용한 로깅 최적화
  - 보상(Compensation) 메커니즘 구현
  - 풀링 시스템 성능 테스트
  - SmallString 성능 벤치마크

#### GoldPlusCommandWrapper (NEW!)
- **설명**: UTxCommandWrapper 기반 골드 명령 예제 (새로운 아키텍쳐)
- **난이도**: 초급
- **파일**: `GoldPlusCommandWrapper.h`, `GoldPlusCommandWrapper.cpp`, `GoldPlusCommandWrapperTest.h`, `GoldPlusCommandWrapperTest.cpp`
- **사용법**: Blueprint 친화적인 새로운 아키텍쳐 기반 명령 구현
- **새로운 기능**:
  - UTxCommandWrapper 기반 Blueprint 통합
  - 완전한 파사드 연동
  - 실패 시뮬레이션 기능
  - Blueprint 설정 가능한 프로퍼티
  - 자동화된 테스트 환경

### 멀티플레이어 (Multiplayer)

#### MultiplayerCommand
- **설명**: 멀티플레이어 명령 예제
- **난이도**: 중급
- **파일**: `MultiplayerCommand.h`, `MultiplayerCommand.cpp`
- **사용법**: 서버/클라이언트 권한 기반 멀티플레이어 명령 구현

### 고급 예제 (Advanced)

#### TaggedCommand
- **설명**: 태그 기반 명령 예제
- **난이도**: 고급
- **파일**: `TaggedCommand.h`, `TaggedCommand.cpp`
- **사용법**: FName 기반 태그 시스템을 활용한 명령 분류 및 필터링

## 🚀 사용 방법

### 1. 예제 복사
```cpp
// 예제 파일을 프로젝트에 복사
#include "GoldPlusCommand.h"
```

### 2. 명령 생성 (새로운 아키텍쳐)
```cpp
// UTxCommandWrapper 기반 골드 명령 생성
UGoldPlusCommandWrapper* Command = NewObject<UGoldPlusCommandWrapper>(this);
Command->SetDelta(100);
Command->SetSimulateFailure(false);
```

### 3. 명령 실행
```cpp
// TxSubsystem을 통해 명령 실행
if (UTxSubsystem* TxSubsystem = GetWorld()->GetSubsystem<UTxSubsystem>())
{
    TxSubsystem->EnqueueCommand(Command);
}

// 또는 파사드를 통해 실행
UBeTxFacade::RunGoldPlusExample(this, 100, false);
```

## 🔧 의존성

- **BeTx**: 핵심 플러그인 (v1.2.0+)
- **Networking**: 멀티플레이어 예제용
- **새로운 기능**:
  - Command Pooling System
  - SmallString Optimization
  - Fixed-size Ring Buffers
  - Performance Monitoring
  - UTxCommandWrapper Architecture
  - Blueprint-friendly Commands
  - Enhanced Facade Integration

## 📋 예제 메타데이터

각 예제의 상세 정보는 `examples.json` 파일에서 확인할 수 있습니다.

```json
{
  "name": "GoldPlusCommand",
  "description": "골드 증가 명령 예제",
  "category": "basic",
  "difficulty": "beginner",
  "tags": ["gold", "command", "basic"]
}
```

## 🎮 테스트 방법

### 콘솔 명령어
```
Betx_Test GoldPlusCommand
Betx_Test GoldPlusCommandWrapper  # 새로운 아키텍쳐 테스트
Betx_Test MultiplayerCommand
Betx_Test TaggedCommand
Betx_PoolStats          # 풀링 시스템 통계
Betx_PoolTrim           # 풀 정리
Betx_Toggle             # 오버레이 토글
Betx_FilterKeyVal       # 필터 설정
```

### 블루프린트
- 각 예제는 블루프린트에서도 사용 가능합니다.
- `BeTxFacade`를 통해 블루프린트 함수로 호출할 수 있습니다.

## 📝 라이선스

이 예제들은 MIT 라이선스 하에 제공됩니다.

## 🤝 기여하기

새로운 예제를 추가하거나 기존 예제를 개선하고 싶으시다면:

1. 예제 파일을 생성
2. `examples.json`에 메타데이터 추가
3. 이 README 파일 업데이트
4. Pull Request 제출

## 📞 지원

문제가 있거나 질문이 있으시면:
- GitHub Issues: [BeTx Issues](https://github.com/yourusername/BeTx-Plugin/issues)
- 문서: [BeTx Documentation](https://github.com/yourusername/BeTx-Plugin/docs)


