//
// Created by madopew on 17.11.2020.
//

#include <stdlib.h>
#include <stdio.h>
#include "../headers/token_stream.h"

tstream *tinit() {
    tstream *s = malloc(sizeof(tstream));
    if(s == 0) {
        fprintf(stderr, "%s\n", "bad alloc! : tinit tstream");
        exit(1);
    }
    s->index = 0;
    s->size = 0;
    s->capacity = 10;
    s->tokens = malloc(s->capacity * sizeof(int));
    if(s->tokens == 0) {
        fprintf(stderr, "%s\n", "bad alloc! : tinit tstream tokens");
        exit(1);
    }
    return s;
}

void tclose(tstream *s) {
    free(s->tokens);
    free(s);
}

void tadd(tstream *s, int token) {
    if(s->size + 1 > s->capacity) {
        s->capacity *= 2;
        s->tokens = realloc(s->tokens, s->capacity * sizeof(int));
        if(s->tokens == 0) {
            fprintf(stderr, "%s\n", "bad alloc! : add token");
            exit(1);
        }
    }
    s->tokens[s->size] = token;
    s->size += 1;
}

void tpop(tstream *s) {
    if(s->index + 1 > s->size) {
        fprintf(stderr, "%s\n", "index out of range! : pop");
        exit(2);
    }
    (s->index)++;
}

int tpeek(tstream *s) {
    if(s->index >= s->size) {
        fprintf(stderr, "%s\n", "index out of range! : peek");
        exit(2);
    }
    return s->tokens[s->index];
}

void tpopn(tstream *s, size_t amount) {
    if(s->index + amount > s->size) {
        fprintf(stderr, "%s\n", "index out of range! : popn");
        exit(2);
    }
    s->index += amount;
}

int tpeekn(tstream *s, size_t amount) {
    if(s->index + amount >= s->size) {
        fprintf(stderr, "%s\n", "index out of range! : peekn");
        exit(2);
    }
    return s->tokens[s->index + amount];
}