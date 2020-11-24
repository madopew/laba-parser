//
// Created by madopew on 17.11.2020.
//

#include <stdio.h>
#include "../headers/parser.h"
#include "../headers/token_stream.h"
#include "../headers/tokentypes.h"

#define EPS 1


tstream *ptokens;

//#define DEBUG
#ifdef DEBUG
unsigned long long n = 0;
#define return n++; return
#endif


int parse(tstream *s) {
    ptokens = s;
    if (translation_unit() && tend(ptokens)) {
        //printf("amount: %llu\n", n);
        return 1;
    } else {
        return 0;
    }
}



int translation_unit() {
    size_t save = tgeti(ptokens);
    if (external_declaration() && translation_unit_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int translation_unit_ext() {
    if(tend(ptokens)) {
        return 1;
    }

    if(external_declaration() && translation_unit_ext()) {
        return 1;
    }

    if(EPS)
        return 1;
}

int external_declaration() {
    size_t save = tgeti(ptokens);
    if(function_definition()) {
        return 1;
    }

    tseti(ptokens, save);
    if(declaration()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int function_definition() {
    size_t save = tgeti(ptokens);
    if(declaration_specifiers() && declarator() && compound_statement()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int declaration_specifiers() {
    size_t save = tgeti(ptokens);
    if(type_specifier() && declaration_specifiers()) {
        return 1;
    }

    tseti(ptokens, save);
    if(type_specifier()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == CONST && declaration_specifiers()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == CONST) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int type_specifier() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == VOID) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SHORT) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == INT) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == LONG) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FLOAT) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DOUBLE) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SIGNED) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == UNSIGNED) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int declarator() {
    size_t save = tgeti(ptokens);
    if(pointer() && direct_declarator()) {
        return 1;
    }

    tseti(ptokens, save);
    if(direct_declarator()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int pointer() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == MUL_CHAR && pointer()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MUL_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int direct_declarator() {
    size_t save = tgeti(ptokens);
    if(direct_declarator_others() && direct_declarator_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int direct_declarator_ext() {
    size_t save = tgeti(ptokens);
    if(direct_declarator_operators() && direct_declarator_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int direct_declarator_operators() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_SQUARE && tpop(ptokens) == CLOSE_SQUARE) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_SQUARE && constant_expression() && tpop(ptokens) == CLOSE_SQUARE) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && parameter_list() && tpop(ptokens) == CLOSE_BRACKET) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && tpop(ptokens) == CLOSE_BRACKET) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && identifier_list() && tpop(ptokens) == CLOSE_BRACKET) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int direct_declarator_others() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_BRACKET && declarator() && tpop(ptokens) == CLOSE_BRACKET) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == IDENTIFIER) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int assignment_expression() {
    size_t save = tgeti(ptokens);
    if(logical_or_expression() && assignment_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int assignment_expression_ext() {
    size_t save = tgeti(ptokens);
    if(assignment_operator() && unary_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int logical_or_expression() {
    size_t save = tgeti(ptokens);
    if(logical_and_expression() && logical_or_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int logical_or_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OR_OP && logical_or_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int logical_and_expression() {
    size_t save = tgeti(ptokens);
    if(inclusive_or_expression() && logical_and_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int logical_and_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == AND_OP && logical_and_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int inclusive_or_expression() {
    size_t save = tgeti(ptokens);
    if(exclusive_or_expression() && inclusive_or_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int inclusive_or_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OR_CHAR && inclusive_or_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int exclusive_or_expression() {
    size_t save = tgeti(ptokens);
    if(and_expression() && exclusive_or_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int exclusive_or_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == XOR_CHAR && exclusive_or_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int and_expression() {
    size_t save = tgeti(ptokens);
    if(equality_expression() && and_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int and_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == AND_CHAR && and_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int equality_expression() {
    size_t save = tgeti(ptokens);
    if(relational_expression() && equality_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int equality_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == NE_OP && equality_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == EQ_OP && equality_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int relational_expression() {
    size_t save = tgeti(ptokens);
    if(shift_expression() && relational_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int relational_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == LESS_CHAR && relational_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == GREATER_CHAR && relational_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == LE_OP && relational_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == GE_OP && relational_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int shift_expression() {
    size_t save = tgeti(ptokens);
    if(additive_expression() && shift_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int shift_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == LEFT_OP && shift_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == RIGHT_OP && shift_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int additive_expression() {
    size_t save = tgeti(ptokens);
    if(multiplicative_expression() && additive_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int additive_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == PLUS_CHAR && additive_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MINUS_CHAR && additive_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int multiplicative_expression() {
    size_t save = tgeti(ptokens);
    if(cast_expression() && multiplicative_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int multiplicative_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == MUL_CHAR && multiplicative_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DIV_CHAR && multiplicative_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MOD_CHAR && multiplicative_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int cast_expression() {
    size_t save = tgeti(ptokens);
    if(unary_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET &&
        type_name() &&
        tpop(ptokens) == CLOSE_BRACKET &&
        cast_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int unary_expression() {
    size_t save = tgeti(ptokens);
    if(postfix_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == INC_OP && unary_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DEC_OP && unary_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SIZEOF && unary_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SIZEOF && tpop(ptokens) == OPEN_BRACKET && type_name() && tpop(ptokens) == CLOSE_BRACKET) {
        return 1;
    }

    tseti(ptokens, save);
    if(unary_operator() && cast_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int postfix_expression() {
    size_t save = tgeti(ptokens);
    if(primary_expression() && postfix_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int postfix_expression_ext() {
    size_t save = tgeti(ptokens);
    if(postfix_expression_operators() && postfix_expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int postfix_expression_operators() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_SQUARE && expression() && tpop(ptokens) == CLOSE_SQUARE) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && tpop(ptokens) == CLOSE_BRACKET) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && argument_expression_list() && tpop(ptokens) == CLOSE_BRACKET) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == INC_OP) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DEC_OP) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int primary_expression() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == IDENTIFIER) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == I_CONSTANT) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == F_CONSTANT) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == STRING_LITERAL) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int expression() {
    size_t save = tgeti(ptokens);
    if(assignment_expression() && expression_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == COMMA_CHAR && expression()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int argument_expression_list() {
    size_t save = tgeti(ptokens);
    if(assignment_expression() && tpop(ptokens) == COMMA_CHAR && argument_expression_list()) {
        return 1;
    }

    tseti(ptokens, save);
    if(assignment_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int unary_operator() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == AND_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MUL_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == PLUS_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MINUS_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == TILDA_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == EXCL_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int type_name() {
    size_t save = tgeti(ptokens);
    if(specifier_qualifier_list() &&
        abstract_declarator()) {
        return 1;
    }

    tseti(ptokens, save);
    if(specifier_qualifier_list()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int specifier_qualifier_list() {
    size_t save = tgeti(ptokens);
    if(type_specifier() && specifier_qualifier_list()) {
        return 1;
    }

    tseti(ptokens, save);
    if(type_specifier()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int abstract_declarator() {
    size_t save = tgeti(ptokens);
    if(pointer() && direct_abstract_declarator()) {
        return 1;
    }

    tseti(ptokens, save);
    if(pointer()) {
        return 1;
    }

    tseti(ptokens, save);
    if(direct_abstract_declarator()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int direct_abstract_declarator() {
    size_t save = tgeti(ptokens);
    if(direct_abstract_declarator_others() && direct_abstract_declarator_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int direct_abstract_declarator_ext() {
    size_t save = tgeti(ptokens);
    if(direct_abstract_declarator_operators() && direct_abstract_declarator_ext()) {
        return 1;
    }

    tseti(ptokens, save);
    if(EPS) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int direct_abstract_declarator_operators() {
    size_t save = tgeti(ptokens);
    if(direct_abstract_declarator_main()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && parameter_list() && tpop(ptokens) == CLOSE_BRACKET) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int direct_abstract_declarator_others() {
    size_t save = tgeti(ptokens);
    if(direct_abstract_declarator_main()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && abstract_declarator() && tpop(ptokens) == CLOSE_BRACKET) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int direct_abstract_declarator_main() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_SQUARE && tpop(ptokens) == CLOSE_SQUARE) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && tpop(ptokens) == CLOSE_BRACKET) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_SQUARE && constant_expression() && tpop(ptokens) == CLOSE_SQUARE) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int parameter_list() {
    size_t save = tgeti(ptokens);
    if(parameter_declaration() && tpop(ptokens) == COMMA_CHAR && parameter_list()) {
        return 1;
    }

    tseti(ptokens, save);
    if(parameter_declaration()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int parameter_declaration() {
    size_t save = tgeti(ptokens);
    if(declaration_specifiers() && declarator()) {
        return 1;
    }

    tseti(ptokens, save);
    if(declaration_specifiers() && abstract_declarator()) {
        return 1;
    }

    tseti(ptokens, save);
    if(declaration_specifiers()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int identifier_list() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == IDENTIFIER && tpop(ptokens) == COMMA_CHAR && identifier_list()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == IDENTIFIER) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int assignment_operator() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == EQ_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MUL_ASSIGN) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DIV_ASSIGN) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MOD_ASSIGN) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == ADD_ASSIGN) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SUB_ASSIGN) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == LEFT_ASSIGN) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == RIGHT_ASSIGN) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == AND_ASSIGN) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == XOR_ASSIGN) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OR_ASSIGN) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int compound_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_CURLY && tpop(ptokens) == CLOSE_CURLY) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_CURLY && block_item_list() && tpop(ptokens) == CLOSE_CURLY) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int block_item_list() {
    size_t save = tgeti(ptokens);
    if(block_item() && block_item_list()) {
        return 1;
    }

    tseti(ptokens, save);
    if(block_item()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int block_item() {
    size_t save = tgeti(ptokens);
    if(declaration()) {
        return 1;
    }

    tseti(ptokens, save);
    if(statement()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int declaration() {
    size_t save = tgeti(ptokens);
    if(declaration_specifiers() &&
        init_declarator_list() &&
        tpop(ptokens) == SEMICOLON_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(declaration_specifiers() && tpop(ptokens) == SEMICOLON_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int init_declarator_list() {
    size_t save = tgeti(ptokens);
    if(init_declarator() &&
        tpop(ptokens) == COMMA_CHAR &&
        init_declarator_list()) {
        return 1;
    }

    tseti(ptokens, save);
    if(init_declarator()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int init_declarator() {
    size_t save = tgeti(ptokens);
    if(declarator() &&
        tpop(ptokens) == EQ_CHAR &&
        initializer()) {
        return 1;
    }

    tseti(ptokens, save);
    if(declarator()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int initializer() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_CURLY && initializer_list() && tpop(ptokens) == CLOSE_CURLY) {
        return 1;
    }

    tseti(ptokens, save);
    if(assignment_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int initializer_list() {
    size_t save = tgeti(ptokens);
    if(initializer() && tpop(ptokens) == COMMA_CHAR && initializer_list()) {
        return 1;
    }

    tseti(ptokens, save);
    if(initializer()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int statement() {
    size_t save = tgeti(ptokens);
    if(labeled_statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(compound_statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(expression_statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(selection_statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(iteration_statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(jump_statement()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int labeled_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == IDENTIFIER && tpop(ptokens) == COLON_CHAR && statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == CASE && constant_expression() && tpop(ptokens) == COLON_CHAR && statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DEFAULT && tpop(ptokens) == COLON_CHAR && statement()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int constant_expression() {
    size_t save = tgeti(ptokens);
    if(logical_or_expression()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int expression_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == SEMICOLON_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(expression() && tpop(ptokens) == SEMICOLON_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int selection_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == IF && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && statement() && tpop(ptokens) == ELSE && statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == IF && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SWITCH && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && statement()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int iteration_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == WHILE && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DO && statement() && tpop(ptokens) == WHILE && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && tpop(ptokens) == SEMICOLON_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FOR && tpop(ptokens) == OPEN_BRACKET && expression_statement() && expression_statement() && tpop(ptokens) == CLOSE_BRACKET && statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FOR && tpop(ptokens) == OPEN_BRACKET && expression_statement() && expression_statement() && expression() && tpop(ptokens) == CLOSE_BRACKET && statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FOR && tpop(ptokens) == OPEN_BRACKET && declaration() && expression_statement() && tpop(ptokens) == CLOSE_BRACKET && statement()) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FOR && tpop(ptokens) == OPEN_BRACKET && declaration() && expression_statement() && expression() && tpop(ptokens) == CLOSE_BRACKET && statement()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int jump_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == CONTINUE && tpop(ptokens) == SEMICOLON_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == BREAK && tpop(ptokens) == SEMICOLON_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == RETURN && tpop(ptokens) == SEMICOLON_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == RETURN && expression() && tpop(ptokens) == SEMICOLON_CHAR) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}
