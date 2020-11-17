cmake_minimum_required(VERSION 3.17)
project(laba3 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

FIND_PACKAGE(FLEX REQUIRED)
FLEX_TARGET(lexer lex/c_lexer.l ${CMAKE_CURRENT_SOURCE_DIR}/lex/c_lexer.c)

add_executable(laba3
        headers/tokentypes.h
        ${FLEX_lexer_OUTPUTS}
        src/main.c
        headers/token_stream.h
        src/token_stream.c
        headers/parser.h src/parser.c)