//
// Created by Madi and Sasha on 12.11.2020.
//

#include <stdio.h>
#include <string.h>
#include "../headers/token_stream.h"
#include "../headers/parser.h"

#define FILE_PATH_SIZE 64
#define bool int
#define true 1
#define false 0

extern FILE *yyin;
extern int yylex();

char *findExtension(char *filePath) {
    return strrchr(filePath, '.');
}

bool isCorrectExtension(char *filepath) {
    char *fileExtension = findExtension(filepath);
    if(!fileExtension) {
        puts("Your source file is not C or C++ program");
        return false;
    }
    bool isCFile = !strcmp(fileExtension, ".c");
    bool isCppFile = !strcmp(fileExtension, ".cpp");
    if (!isCFile && !isCppFile) {
        puts("Your source file is not C or C++ program");
        return false;
    }
    if (isCppFile) {
        puts("Your source file is C++ program. It may be not parsed correctly.");
    }
    return true;
}

void inputFilePath(char *path, size_t len) {
    len--;
    puts("Input path to source file:");
    fflush(stdin);
    while(fgets(path, len, stdin) == NULL) {
        fflush(stdin);
        puts("File path is too long! Try again.");
    }
    path[strlen(path) - 1] = 0;
}

void inputCorrectFilePath(char *file, size_t len) {
    do {
        memset(file, 0, len);
        inputFilePath(file, len);
    } while(!isCorrectExtension(file));
}

void initTokenStream(char *file, tstream *tokens) {
    yyin = fopen(file, "r");
    int c;
    while((c = yylex())) {
        tadd(tokens, c);
    }
}

void outputResult(tstream *tokens) {
    int result = parse(tokens);
    printf("Syntactic analysis completed %s\n", result ? "successfully." : "with an error.");
}

int main() {
//    char file[FILE_PATH_SIZE];
//    inputCorrectFilePath(file, FILE_PATH_SIZE);

    tstream *tokens = tinit();
    initTokenStream("../res/test.c", tokens);
    outputResult(tokens);
    tclose(tokens);

    return 0;
}

