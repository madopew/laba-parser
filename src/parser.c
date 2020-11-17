//
// Created by madopew on 17.11.2020.
//

#include "../headers/parser.h"
#include "../headers/token_stream.h"
#include "../headers/tokentypes.h"

tstream *ptokens;

int parse(tstream *s) {
    ptokens = s;
    return translation_unit();
}

int translation_unit() {
}

int external_declaration() {
}

int function_definition() {
}

int declaration_specifiers() {
}

int type_specifier() {
}

int declarator() {
}

int pointer() {
}

int direct_declarator() {
}

int assignment_expression() {
}

int logical_or_expression() {
}

int logical_and_expression() {
}

int inclusive_or_expression() {
}

int exclusive_or_expression() {
}

int and_expression() {
}

int equality_expression() {
}

int relational_expression() {
}

int shift_expression() {
}

int additive_expression() {
}

int multiplicative_expression() {
}

int cast_expression() {
}

int unary_expression() {
}

int postfix_expression() {
}

int primary_expression() {
}

int expression() {
}

int argument_expression_list() {
}

int unary_operator() {
}

int type_name() {
}

int specifier_qualifier_list() {
}

int abstract_declarator() {
}

int direct_abstract_declarator() {
}

int parameter_list() {
}

int parameter_declaration() {
}

int identifier_list() {
}

int assignment_operator() {
}

int compound_statement() {
}

int block_item_list() {
}

int block_item() {
}

int declaration() {
}

int init_declarator_list() {
}

int init_declarator() {
}

int initializer() {
}

int initializer_list() {
}

int designation() {
}

int designator_list() {
}

int designator() {
}

int statement() {
}

int labeled_statement() {
}

int constant_expression() {
}

int expression_statement() {
}

int selection_statement() {
}

int iteration_statement() {
}

int jump_statement() {
}