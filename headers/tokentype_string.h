//
// Created by Lenovo on 03.12.2020.
//

#ifndef LABA3_TOKENTYPE_STRING_H
#define LABA3_TOKENTYPE_STRING_H
#include "tokentypes.h"

char *TOKENTYPE_STRING[] =
        {
                "BREAK",
                "CASE",
                "CHAR",
                "CONST",
                "CONTINUE",
                "DEFAULT",
                "DO",
                "DOUBLE",
                "ELSE",
                "FLOAT",
                "FOR",
                "IF",
                "INT",
                "LONG",
                "RETURN",
                "SHORT",
                "SIGNED",
                "SIZEOF",
                "SWITCH",
                "UNSIGNED",
                "VOID",
                "WHILE",
                "IDENTIFIER",
                "I_CONSTANT",
                "F_CONSTANT",
                "STRING_LITERAL",
                "ADD_ASSIGN",
                "SUB_ASSIGN",
                "MUL_ASSIGN",
                "DIV_ASSIGN",
                "MOD_ASSIGN",
                "AND_ASSIGN",
                "XOR_ASSIGN",
                "OR_ASSIGN",
                "LEFT_ASSIGN",
                "RIGHT_ASSIGN",
                "RIGHT_OP",
                "LEFT_OP",
                "INC_OP",
                "DEC_OP",
                "AND_OP",
                "OR_OP",
                "LE_OP",
                "GE_OP",
                "EQ_OP",
                "NE_OP",
                "SEMICOLON_CHAR",
                "OPEN_CURLY",
                "CLOSE_CURLY",
                "COMMA_CHAR",
                "COLON_CHAR",
                "EQ_CHAR",
                "OPEN_BRACKET",
                "CLOSE_BRACKET",
                "OPEN_SQUARE",
                "CLOSE_SQUARE",
                "DOT_CHAR",
                "AND_CHAR",
                "EXCL_CHAR",
                "TILDA_CHAR",
                "MINUS_CHAR",
                "PLUS_CHAR",
                "MUL_CHAR",
                "DIV_CHAR",
                "MOD_CHAR",
                "LESS_CHAR",
                "GREATER_CHAR",
                "XOR_CHAR",
                "OR_CHAR"
        };

char *get_string_token(enum TOKENTYPE t) {
    return TOKENTYPE_STRING[t - START_TOKEN];
}
#endif //LABA3_TOKENTYPE_STRING_H
