#ifndef _SMALLOC_H_
#define _SMALLOC_H_

#include <stddef.h>

typedef enum {
    BESTFIT,
    WORSTFIT,
    FIRSTFIT
} smmode;

void *smalloc(size_t s);
void *srealloc(void *p, size_t s);
void sfree(void *p);
void sset_mode(smmode m);
void smcoalesce(void);

#endif /* _SMALLOC_H_ */ 