//
// Created by Madi on 05.12.2020.
//

#ifndef PARSER_CLASS_LOG_WRITER_H
#define PARSER_CLASS_LOG_WRITER_H

#include "./tokentype_string.h"

#ifndef NDEBUG

#define return _debug_counter_++; return
unsigned long long _debug_counter_ = 0;
void debug() {
    printf("\nOperations amount: %llu.\n", _debug_counter_);
}

int isStartFittedRulesChain = 0;
int isStartFailedRulesChain = 0;
int isFittedRulesChain = 0;
void print_fit_rule(const char *functionName, const char *symbols[], int tokensAmount) {
    isStartFailedRulesChain = 0;
    if (tokensAmount == 0) {
        if (isFittedRulesChain) {
            if (isStartFittedRulesChain) {
                printf(" -> '%s'", functionName);
            } else {
                printf("\nClimbing success rule stack: '%s'", functionName);
                isStartFittedRulesChain = 1;
            }
        } else {
            printf("\nRule: '%s' passed successfully. Saving last token...", functionName);
            isFittedRulesChain = 1;
        }
    } else {
        isStartFittedRulesChain = 0;
        printf("\nRule: '%s' passed successfully. ", functionName);
        if (tokensAmount == -1) {
            printf("EPS was found.");
        } else {
            printf("Symbols: [%s", symbols[0]);
            for (int i = 1; i < tokensAmount; i++) {
                printf(", %s", symbols[i]);
            }
            printf("] are saved!");
        }
    }
}

extern tstream *ptokens;
char *pastPrevStr = "";
char *pastIndexStr = "";
char *pastNextStr = "";
void print_not_fit_rule(int index, const char *functionName) {
    isStartFittedRulesChain = 0;
    char output[300] = {0};
    char *prevStr;
    if (index == 0) {
        prevStr = "~empty~";
    } else {
        prevStr = get_string_token(ptokens->tokens[index - 1]);
    }
    char *indexStr = get_string_token(ptokens->tokens[index]);
    char *nextStr;
    if (ptokens->size == index + 1) {
        nextStr = "~empty~";
    } else {
        nextStr = get_string_token(ptokens->tokens[index + 1]);
    }
    if (strcmp(pastPrevStr, prevStr) || strcmp(pastIndexStr, indexStr) || strcmp(pastNextStr, nextStr)) {
        isStartFailedRulesChain = 0;
        sprintf(output, "\nRule: '%s' failed. Returning to [..., %s, %s, %s, ...]\n", functionName,
                prevStr, indexStr, nextStr);
        printf("%s", output);
        printf("%*s%.*s", strlen(output) - 10 - strlen(indexStr) - strlen(nextStr), " ", strlen(indexStr),
               "^^^^^^^^^^^^^^^^^^^^^^^^^");
    } else {
        if (isStartFailedRulesChain) {
            printf(" -> '%s'", functionName);
        } else {
            printf("\nRolling back failed rules stack: ");
            printf("'%s'", functionName);
            isStartFailedRulesChain = 1;
        }
    }
    pastPrevStr = prevStr;
    pastIndexStr = indexStr;
    pastNextStr = nextStr;
}

#else
#define debug() ((void)0)
#define print_fit_rule(a, b, c) ((void)0)
#define print_not_fit_rule(a, b) ((void)0)
#endif

#endif //PARSER_CLASS_LOG_WRITER_H
