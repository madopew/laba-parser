//
// Created by madopew on 17.11.2020.
//

#ifndef LABA3_TOKEN_STREAM_H
#define LABA3_TOKEN_STREAM_H

#include <stdlib.h>

typedef struct {
    int *tokens;
    size_t size;
    size_t capacity;
    size_t index;
} tstream;

tstream *tinit(void);
void tclose(tstream*);
void tadd(tstream*, int);
int tend(tstream*);

void tpop(tstream*);
int tpeek(tstream*);
void tpopn(tstream*, size_t);
int tpeekn(tstream*, size_t);

#endif //LABA3_TOKEN_STREAM_H
