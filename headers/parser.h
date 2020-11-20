//
// Created by madopew on 17.11.2020.
//

#ifndef LABA3_PARSER_H
#define LABA3_PARSER_H

#include "token_stream.h"

int parse(tstream*);

int translation_unit();
int translation_unit_ext();
int external_declaration();
int function_definition();
int declaration_specifiers();
int type_specifier();
int declarator();
int pointer();
int direct_declarator();
int direct_declarator_ext();
int direct_declarator_operators();
int direct_declarator_others();
int assignment_expression();
int logical_or_expression();
int logical_and_expression();
int inclusive_or_expression();
int exclusive_or_expression();
int and_expression();
int equality_expression();
int relational_expression();
int shift_expression();
int additive_expression();
int multiplicative_expression();
int cast_expression();
int unary_expression();
int postfix_expression();
int postfix_expression_ext();
int postfix_expression_operators();
int primary_expression();
int expression();
int argument_expression_list();
int unary_operator();
int type_name();
int specifier_qualifier_list();
int abstract_declarator();
int direct_abstract_declarator();
int parameter_list();
int parameter_declaration();
int identifier_list();
int assignment_operator();
int compound_statement();
int block_item_list();
int block_item();
int declaration();
int init_declarator_list();
int init_declarator();
int initializer();
int initializer_list();
int statement();
int labeled_statement();
int constant_expression();
int expression_statement();
int selection_statement();
int iteration_statement();
int jump_statement();



#endif //LABA3_PARSER_H
