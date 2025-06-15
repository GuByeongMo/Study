#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "smalloc.h"

#define HEADER_SIZE 24  // 헤더 크기 (bytes)

typedef struct header {
    size_t size;        // 데이터 영역의 크기
    int used;           // 사용 여부 (0: unused, 1: used)
    struct header *next;// 다음 헤더의 포인터
} header_t;

static header_t *head = NULL;  // 첫 번째 헤더를 가리키는 포인터
static smmode current_mode = BESTFIT;  // 기본 할당 모드

// mmap()을 사용하여 새로운 페이지 할당
static void *allocate_pages(size_t size) {
    size_t page_size = getpagesize();
    size_t total_size = ((size + HEADER_SIZE + page_size - 1) / page_size) * page_size;
    
    void *ptr = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        return NULL;
    }

    header_t *header = (header_t *)ptr;
    header->size = total_size - HEADER_SIZE;
    header->used = 0;
    header->next = head;
    head = header;

    return ptr;
}

// 최적의 메모리 블록 찾기
static header_t *find_best_fit(size_t size) {
    header_t *best = NULL;
    header_t *current = head;
    size_t min_diff = SIZE_MAX;

    while (current != NULL) {
        if (!current->used && current->size >= size) {
            size_t diff = current->size - size;
            if (current_mode == BESTFIT && diff < min_diff) {
                min_diff = diff;
                best = current;
            } else if (current_mode == WORSTFIT && diff > min_diff) {
                min_diff = diff;
                best = current;
            } else if (current_mode == FIRSTFIT) {
                return current;
            }
        }
        current = current->next;
    }
    return best;
}

void *smalloc(size_t s) {
    if (s == 0) return NULL;

    header_t *header = find_best_fit(s);
    if (!header) {
        header = allocate_pages(s);
        if (!header) return NULL;
    }

    // 분할이 필요한 경우
    if (header->size >= s + HEADER_SIZE + 24) {
        header_t *new_header = (header_t *)((char *)header + HEADER_SIZE + s);
        new_header->size = header->size - s - HEADER_SIZE;
        new_header->used = 0;
        new_header->next = header->next;
        header->next = new_header;
        header->size = s;
    }

    header->used = 1;
    return (char *)header + HEADER_SIZE;
}

void sfree(void *p) {
    if (!p) return;

    header_t *header = (header_t *)((char *)p - HEADER_SIZE);
    if (header < head || header >= (header_t *)((char *)head + head->size + HEADER_SIZE)) {
        abort();
    }

    header->used = 0;
    smcoalesce();
}

void *srealloc(void *p, size_t s) {
    if (!p) return smalloc(s);
    if (s == 0) {
        sfree(p);
        return NULL;
    }

    header_t *header = (header_t *)((char *)p - HEADER_SIZE);
    if (header->size >= s) return p;

    void *new_ptr = smalloc(s);
    if (!new_ptr) return NULL;

    size_t copy_size = header->size < s ? header->size : s;
    memcpy(new_ptr, p, copy_size);
    sfree(p);

    return new_ptr;
}

void sset_mode(smmode m) {
    current_mode = m;
}

void smcoalesce(void) {
    header_t *current = head;
    while (current && current->next) {
        if (!current->used && !current->next->used) {
            current->size += HEADER_SIZE + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
} 