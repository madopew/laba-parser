//
// Created by Madi on 12.11.2020.
//

/*
  selection_statement
        : IF OPEN_BRACKET expression CLOSE_BRACKET statement ELSE statement
        | IF OPEN_BRACKET expression CLOSE_BRACKET statement

  expression
        : I_CONSTANT LESS_CHAR I_CONSTANT
        | I_CONSTANT GREATER_CHAR I_CONSTANT
        | I_CONSTANT EQ_OP I_CONSTANT

  statement
        : OPEN_CURLY expression_list CLOSE_CURLY
        | expression SEMICOLON

  expression_list
        : expression SEMICOLON expression_list
        | expression SEMICOLON
 */
#include <stdio.h>
#include "../headers/tokentypes.h"
#include "../headers/token_stream.h"

extern int yylex();
extern FILE *yyin;

int selection_statement();
int expression();
int statement();
int expression_list();

int size = 0;
int current_index = 0;
int arr[1000] = {0};

int peek() {
    return arr[current_index];
}

int peekDif(int dif) {
    if (current_index + dif >= size) {
        return -1;
    } //no sense

    return arr[current_index + dif];
}

int pop() {
    return arr[current_index++];
}

void popTimes(int times) {
    for (int i = 0; i < times; i++) {
        pop();
    }
}

int main() {
    yyin = fopen("../res/test.c", "r");
    tstream *s = tinit();
    int c;
    while((c = yylex())) {
        tadd(s, c);
    }
    printf("%lu %lu", s->size, s->capacity);
    /*while((arr[current_index++] = yylex())) {
        printf(" %d ", arr[current_index - 1]);
    }*/
   /* size = current_index - 1;
    current_index = 0;
    int result = selection_statement();
    printf("Result: %d\n", result);*/
    tclose(s);
    return 0;
}

int selection_statement() {
    int result = 0;
    int first = pop();
    int second = pop();
    if(first == IF && second == OPEN_BRACKET) {
        result = expression();
        popTimes(3);
        if(result) {
            result = pop() == CLOSE_BRACKET;
            if(result) {
                result = statement();
                if(result) {
                    first = peek();
                    if(first == ELSE) {
                        pop();
                        result = statement();
                    }
                }
            }
        }
    }
    return result;
}

int expression() {
    int result = 0;
    int first = peek();
    int second = peekDif(1);
    int third = peekDif(2);
    if(first == I_CONSTANT && third == I_CONSTANT) {
        if(second == EQ_CHAR || second == LESS_CHAR || second == GREATER_CHAR)
            result = 1;
    }
    return result;
}

int statement() {
    int result;
    int first = peek();
    if(first == OPEN_CURLY) {
        pop();
        result = expression_list();
        if(result) {
            int nextToken = peek();
            if (nextToken == CLOSE_CURLY) {
                pop();
            } else {
                result = 0;
            }
        }
    } else {
        result = expression();
        if (result) {
            popTimes(3);
            int nextToken = peek();
            if (nextToken == SEMICOLON_CHAR) {
                pop();
            } else {
                result = 0;
            }
        }
    }
    return result;
}

int expression_list() {
    int result = expression();
    if(result) {
        int nextToken = peekDif(3);
        if (nextToken == SEMICOLON_CHAR) {
            popTimes(4);
            while (expression_list());
        } else {
            result = 0;
        }
    }
    return result;
}


