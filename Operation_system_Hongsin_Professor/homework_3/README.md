# Memory Allocator (smalloc)

이 프로젝트는 리눅스 환경에서 동작하는 간단한 메모리 할당자를 구현한 것입니다.

## 기능

- `smalloc`: 메모리 할당
- `sfree`: 메모리 해제
- `srealloc`: 메모리 재할당
- `sset_mode`: 메모리 할당 모드 설정 (BESTFIT, WORSTFIT, FIRSTFIT)
- `smcoalesce`: 인접한 미사용 메모리 블록 병합

## 빌드 방법

```bash
make
```

## 실행 방법

```bash
./test_smalloc
```

## 구현 세부사항

- 메모리 할당은 `mmap()`을 사용하여 페이지 단위로 수행됩니다.
- 각 메모리 블록은 24바이트 헤더를 가지며, 다음 정보를 포함합니다:
  - size: 데이터 영역의 크기
  - used: 사용 여부 (0: unused, 1: used)
  - next: 다음 헤더의 포인터
- 메모리 할당 전략은 BESTFIT, WORSTFIT, FIRSTFIT 중 선택 가능합니다.

## 정리

```bash
make clean
``` 