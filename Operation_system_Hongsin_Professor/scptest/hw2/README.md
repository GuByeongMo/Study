# Homework 2. Game-agent Match
## **Team number :** 213

- 구병모 : 2022041057
- 전준석 : 2021041051

### 개요

6x7의 게임 보드판에서 두 에이전트의 Connect Four 게임의 심판을 해주는 프로그램입니다.
두 개의 에이전트를 자식 프로세스로 실행하고 파이프를 통해 게임을 진행합니다.

### 기능

- 두 개의 에이전트를 자식프로세스에 교차로 실행하고, 각 턴마다 입출력 처리
- 게임의 승자, 패자 및 무승부 판별 기능
- 에이전트의 응답이 3초가 지날경우 패배확인
- 게임판 유지 및 출력

### 요구 사항

- **Linux or POSIX environment**
- **Clang 10.0.0**
- **GCC 9.4.0**
- **GNU Make 4.2.1**
  
### 파일 구조

```bash
.
├── gamatch.c            # Connect Four 게임 심판 메인 파일
├── rand_agent.c
├── greedy_agent.c      
├── timeout_agent.c
├── Makefile             # 빌드 스크립트
└── README.md            # 설명서

```
### 설치

터미널에서 아래 명령어로 컴파일하세요:

```bash
make
```

빌드 후 아래와 같은 실행 파일들이 생성됩니다:

- `gamatch` — 메인 실행파일

### 사용 방법

기본 실행 명령어는 아래와 같습니다:

```bash
./gamatch -X <agent-binary> -Y <agent-binary>
```

- `X` : 첫 번째 플레이어 에이전트 (X)
- `Y` : 두 번째 플레이어 에이전트 (O)
- 인자를 잘못 적을 경우 실행 거부 ( "Usage: ./gamatch -X \<agent1\> -Y \<agent2\>" 라고 사용법 출력)

### 라이선스

N/A

### 제작자

- 구병모 : 2022041057, 전준석 : 2021041051