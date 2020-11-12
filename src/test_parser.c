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
        | expression_list

  expression_list
        | expression SEMICOLON
 */
#include <stdio.h>
#include "../headers/tokentypes.h"

extern int yylex();
extern FILE *yyin;

int selection_statement();
int expression();
int statement();
int expression_list();

int current_index = 0;
int arr[1000] = {0};

int peek() {
    return arr[current_index];
}

int pop() {
    return arr[current_index++];
}

int main() {
    yyin = fopen("../res/test.c", "r");
    while((arr[current_index++] = yylex())) {
        printf(" %d ", arr[current_index - 1]);
    }
    current_index = 0;
    int result = selection_statement();
    printf("Result: %d\n", result);
    return 0;
}

int selection_statement() {
    int result = 0;
    int first = pop();
    int second = pop();
    if(first == IF && second == OPEN_BRACKET) {
        result = expression();
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
    int first = pop();
    int second = pop();
    int third = pop();
    if(first == I_CONSTANT && third == I_CONSTANT) {
        if(second == LESS_CHAR || second == GREATER_CHAR || second == EQ_OP)
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
            result = pop() == CLOSE_CURLY;
        }
    } else {
        result = expression_list();
    }
    return result;
}

int expression_list() {
    int result = expression();
    if(result) {
        result = pop() == CLOSE_CURLY;
    }
    return result;
}


