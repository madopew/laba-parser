#include <stdio.h>
#include <string.h>
#include "../headers/parser.h"
#include "../headers/token_stream.h"
#include "../headers/tokentypes.h"
#include "../headers/tokentype_string.h"
#define EPS 1


tstream *ptokens;

#define DEBUG
#ifdef DEBUG
unsigned long long _debug_counter_ = 0;
#define debug() printf("Operations amount: %llu.\n", _debug_counter_)
#define return _debug_counter_++; return
#else
#define debug()
#endif


int parse(tstream *s) {
    ptokens = s;
    if (translation_unit() && tend(ptokens)) {
        debug();
        return 1;
    } else {
        return 0;
    }
}


void print_fit_rule(const char *functionName, const int *indexArray, int tokensAmount) {
    printf("Rule: %s passed successfully. ", functionName);
    if (tokensAmount == -1) {
        printf("EPS was founded.\n");
    } else if (tokensAmount == 0) {
        printf("Saving last token...\n");
    } else {
        printf("Tokens: [");
        if (indexArray[0] != -1) {
            printf("%s", get_string_token(ptokens->tokens[indexArray[0]]));
        } else {
            printf("%s", "...other rule...");
        }
        for (int i = 1; i < tokensAmount; i++) {
            if (indexArray[i] == -1) {
                printf("%s", ", ...other rule... ");
            } else {
                printf(", %s", get_string_token(ptokens->tokens[indexArray[i]]));
            }
        }
        printf("] are saved!\n");
    }
}

void print_not_fit_rule(int index, const char *functionName) {
    char output[300] = {0};
    char *prevStr;
    if (index == 0) {
        prevStr = "~empty~";
    }
    else {
        prevStr = get_string_token(ptokens->tokens[index - 1]);
    }
    char *indexStr = get_string_token(ptokens->tokens[index]);
    char *nextStr;
    if (ptokens->size == index + 1) {
        nextStr = "~empty~";
    }
    else {
        nextStr = get_string_token(ptokens->tokens[index + 1]);
    }
    sprintf(output, "Rule: '%s' failed. Returning to [..., %s, %s, %s, ...]\n",  functionName, prevStr, indexStr, nextStr);
    printf("%s", output);
    printf("%*s%.*s\n", strlen(output) - 9 - strlen(indexStr) - strlen(nextStr), " ", strlen(indexStr), "^^^^^^^^^^^^^^^^^^^^^^^^^");
}

int translation_unit() {
    size_t save = tgeti(ptokens);
    if(external_declaration() && translation_unit_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int translation_unit_ext() {
    size_t save = tgeti(ptokens);
    if(tend(ptokens)) {
        return 1;
    }

    if(external_declaration() && translation_unit_ext()) {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    if(EPS) {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }
}

int external_declaration() {
    size_t save = tgeti(ptokens);
    if(function_definition())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(declaration())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int function_definition() {
    size_t save = tgeti(ptokens);
    if(declaration_specifiers() && declarator() && compound_statement())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int declaration_specifiers() {
    size_t save = tgeti(ptokens);
    if(type_specifier() && declaration_specifiers())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(type_specifier())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == CONST && declaration_specifiers())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == CONST)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int type_specifier() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == VOID)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == CHAR)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SHORT)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == INT)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == LONG)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FLOAT)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DOUBLE)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SIGNED)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == UNSIGNED)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int declarator() {
    size_t save = tgeti(ptokens);
    if(pointer() && direct_declarator())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(direct_declarator())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int pointer() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == MUL_CHAR && pointer())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MUL_CHAR)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_declarator() {
    size_t save = tgeti(ptokens);
    if(direct_declarator_others() && direct_declarator_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_declarator_ext() {
    size_t save = tgeti(ptokens);
    if(direct_declarator_operators() && direct_declarator_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_declarator_operators() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_SQUARE && tpop(ptokens) == CLOSE_SQUARE)  {
        int indexes[] = {save,save + 1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_SQUARE && constant_expression() && tpop(ptokens) == CLOSE_SQUARE)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && parameter_list() && tpop(ptokens) == CLOSE_BRACKET)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && tpop(ptokens) == CLOSE_BRACKET)  {
        int indexes[] = {save,save + 1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && identifier_list() && tpop(ptokens) == CLOSE_BRACKET)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_declarator_others() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_BRACKET && declarator() && tpop(ptokens) == CLOSE_BRACKET)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == IDENTIFIER)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int assignment_expression() {
    size_t save = tgeti(ptokens);
    if(logical_or_expression() && assignment_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int assignment_expression_ext() {
    size_t save = tgeti(ptokens);
    if(assignment_operator() && unary_expression())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int logical_or_expression() {
    size_t save = tgeti(ptokens);
    if(logical_and_expression() && logical_or_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int logical_or_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OR_OP && logical_or_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int logical_and_expression() {
    size_t save = tgeti(ptokens);
    if(inclusive_or_expression() && logical_and_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int logical_and_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == AND_OP && logical_and_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int inclusive_or_expression() {
    size_t save = tgeti(ptokens);
    if(exclusive_or_expression() && inclusive_or_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int inclusive_or_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OR_CHAR && inclusive_or_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int exclusive_or_expression() {
    size_t save = tgeti(ptokens);
    if(and_expression() && exclusive_or_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int exclusive_or_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == XOR_CHAR && exclusive_or_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int and_expression() {
    size_t save = tgeti(ptokens);
    if(equality_expression() && and_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int and_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == AND_CHAR && and_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int equality_expression() {
    size_t save = tgeti(ptokens);
    if(relational_expression() && equality_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int equality_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == NE_OP && equality_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == EQ_OP && equality_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int relational_expression() {
    size_t save = tgeti(ptokens);
    if(shift_expression() && relational_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int relational_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == LESS_CHAR && relational_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == GREATER_CHAR && relational_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == LE_OP && relational_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == GE_OP && relational_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int shift_expression() {
    size_t save = tgeti(ptokens);
    if(additive_expression() && shift_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int shift_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == LEFT_OP && shift_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == RIGHT_OP && shift_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int additive_expression() {
    size_t save = tgeti(ptokens);
    if(multiplicative_expression() && additive_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int additive_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == PLUS_CHAR && additive_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MINUS_CHAR && additive_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int multiplicative_expression() {
    size_t save = tgeti(ptokens);
    if(cast_expression() && multiplicative_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int multiplicative_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == MUL_CHAR && multiplicative_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DIV_CHAR && multiplicative_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MOD_CHAR && multiplicative_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int cast_expression() {
    size_t save = tgeti(ptokens);
    if(unary_expression())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && type_name() && tpop(ptokens) == CLOSE_BRACKET && cast_expression())  {
        int indexes[] = {save,-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 4);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int unary_expression() {
    size_t save = tgeti(ptokens);
    if(postfix_expression())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == INC_OP && unary_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DEC_OP && unary_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SIZEOF && unary_expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SIZEOF && tpop(ptokens) == OPEN_BRACKET && type_name() && tpop(ptokens) == CLOSE_BRACKET)  {
        int indexes[] = {save,save + 1,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 4);
        return 1;
    }

    tseti(ptokens, save);
    if(unary_operator() && cast_expression())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int postfix_expression() {
    size_t save = tgeti(ptokens);
    if(primary_expression() && postfix_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int postfix_expression_ext() {
    size_t save = tgeti(ptokens);
    if(postfix_expression_operators() && postfix_expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int postfix_expression_operators() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_SQUARE && expression() && tpop(ptokens) == CLOSE_SQUARE)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && tpop(ptokens) == CLOSE_BRACKET)  {
        int indexes[] = {save,save + 1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && argument_expression_list() && tpop(ptokens) == CLOSE_BRACKET)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == INC_OP)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DEC_OP)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int primary_expression() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == IDENTIFIER)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == I_CONSTANT)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == F_CONSTANT)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == STRING_LITERAL)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int expression() {
    size_t save = tgeti(ptokens);
    if(assignment_expression() && expression_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == COMMA_CHAR && expression())  {
        int indexes[] = {save,-1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int argument_expression_list() {
    size_t save = tgeti(ptokens);
    if(assignment_expression() && tpop(ptokens) == COMMA_CHAR && argument_expression_list())  {
        int indexes[] = {-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(assignment_expression())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int unary_operator() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == AND_CHAR)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MUL_CHAR)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == PLUS_CHAR)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MINUS_CHAR)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == TILDA_CHAR)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == EXCL_CHAR)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int type_name() {
    size_t save = tgeti(ptokens);
    if(specifier_qualifier_list() && abstract_declarator())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(specifier_qualifier_list())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int specifier_qualifier_list() {
    size_t save = tgeti(ptokens);
    if(type_specifier() && specifier_qualifier_list())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(type_specifier())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int abstract_declarator() {
    size_t save = tgeti(ptokens);
    if(pointer() && direct_abstract_declarator())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(pointer())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(direct_abstract_declarator())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_abstract_declarator() {
    size_t save = tgeti(ptokens);
    if(direct_abstract_declarator_others() && direct_abstract_declarator_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_abstract_declarator_ext() {
    size_t save = tgeti(ptokens);
    if(direct_abstract_declarator_operators() && direct_abstract_declarator_ext())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_abstract_declarator_operators() {
    size_t save = tgeti(ptokens);
    if(direct_abstract_declarator_main())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && parameter_list() && tpop(ptokens) == CLOSE_BRACKET)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_abstract_declarator_others() {
    size_t save = tgeti(ptokens);
    if(direct_abstract_declarator_main())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && abstract_declarator() && tpop(ptokens) == CLOSE_BRACKET)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_abstract_declarator_main() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_SQUARE && tpop(ptokens) == CLOSE_SQUARE)  {
        int indexes[] = {save,save + 1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && tpop(ptokens) == CLOSE_BRACKET)  {
        int indexes[] = {save,save + 1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_SQUARE && constant_expression() && tpop(ptokens) == CLOSE_SQUARE)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int parameter_list() {
    size_t save = tgeti(ptokens);
    if(parameter_declaration() && tpop(ptokens) == COMMA_CHAR && parameter_list())  {
        int indexes[] = {-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(parameter_declaration())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int parameter_declaration() {
    size_t save = tgeti(ptokens);
    if(declaration_specifiers() && declarator())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(declaration_specifiers() && abstract_declarator())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(declaration_specifiers())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int identifier_list() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == IDENTIFIER && tpop(ptokens) == COMMA_CHAR && identifier_list())  {
        int indexes[] = {save,save + 1,-1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == IDENTIFIER)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int assignment_operator() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == EQ_CHAR)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MUL_ASSIGN)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DIV_ASSIGN)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MOD_ASSIGN)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == ADD_ASSIGN)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SUB_ASSIGN)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == LEFT_ASSIGN)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == RIGHT_ASSIGN)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == AND_ASSIGN)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == XOR_ASSIGN)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OR_ASSIGN)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int compound_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_CURLY && tpop(ptokens) == CLOSE_CURLY)  {
        int indexes[] = {save,save + 1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_CURLY && block_item_list() && tpop(ptokens) == CLOSE_CURLY)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int block_item_list() {
    size_t save = tgeti(ptokens);
    if(block_item() && block_item_list())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(block_item())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int block_item() {
    size_t save = tgeti(ptokens);
    if(declaration())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(statement())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int declaration() {
    size_t save = tgeti(ptokens);
    if(declaration_specifiers() && init_declarator_list() && tpop(ptokens) == SEMICOLON_CHAR)  {
        int indexes[] = {-1,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(declaration_specifiers() && tpop(ptokens) == SEMICOLON_CHAR)  {
        int indexes[] = {-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int init_declarator_list() {
    size_t save = tgeti(ptokens);
    if(init_declarator() && tpop(ptokens) == COMMA_CHAR && init_declarator_list())  {
        int indexes[] = {-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(init_declarator())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int init_declarator() {
    size_t save = tgeti(ptokens);
    if(declarator() && tpop(ptokens) == EQ_CHAR && initializer())  {
        int indexes[] = {-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(declarator())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int initializer() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_CURLY && initializer_list() && tpop(ptokens) == CLOSE_CURLY)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(assignment_expression())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int initializer_list() {
    size_t save = tgeti(ptokens);
    if(initializer() && tpop(ptokens) == COMMA_CHAR && initializer_list())  {
        int indexes[] = {-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(initializer())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int statement() {
    size_t save = tgeti(ptokens);
    if(labeled_statement())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(compound_statement())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(expression_statement())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(selection_statement())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(iteration_statement())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(jump_statement())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int labeled_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == IDENTIFIER && tpop(ptokens) == COLON_CHAR && statement())  {
        int indexes[] = {save,save + 1,-1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == CASE && constant_expression() && tpop(ptokens) == COLON_CHAR && statement())  {
        int indexes[] = {save,-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 4);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DEFAULT && tpop(ptokens) == COLON_CHAR && statement())  {
        int indexes[] = {save,save + 1,-1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int constant_expression() {
    size_t save = tgeti(ptokens);
    if(logical_or_expression())  {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int expression_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == SEMICOLON_CHAR)  {
        int indexes[] = {save};
        print_fit_rule(__func__, indexes, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(expression() && tpop(ptokens) == SEMICOLON_CHAR)  {
        int indexes[] = {-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int selection_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == IF && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && statement() && tpop(ptokens) == ELSE && statement())  {
        int indexes[] = {save,save + 1,-1,ptokens->index - 2,-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 7);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == IF && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        int indexes[] = {save,save + 1,-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 5);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SWITCH && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        int indexes[] = {save,save + 1,-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 5);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int iteration_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == WHILE && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        int indexes[] = {save,save + 1,-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 5);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DO && statement() && tpop(ptokens) == WHILE && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && tpop(ptokens) == SEMICOLON_CHAR)  {
        int indexes[] = {save,-1,ptokens->index - 3,ptokens->index - 2,-1,ptokens->index - 0,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 7);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FOR && tpop(ptokens) == OPEN_BRACKET && expression_statement() && expression_statement() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        int indexes[] = {save,save + 1,-1,-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 6);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FOR && tpop(ptokens) == OPEN_BRACKET && expression_statement() && expression_statement() && expression() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        int indexes[] = {save,save + 1,-1,-1,-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 7);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FOR && tpop(ptokens) == OPEN_BRACKET && declaration() && expression_statement() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        int indexes[] = {save,save + 1,-1,-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 6);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FOR && tpop(ptokens) == OPEN_BRACKET && declaration() && expression_statement() && expression() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        int indexes[] = {save,save + 1,-1,-1,-1,ptokens->index - 0,-1};
        print_fit_rule(__func__, indexes, 7);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int jump_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == CONTINUE && tpop(ptokens) == SEMICOLON_CHAR)  {
        int indexes[] = {save,save + 1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == BREAK && tpop(ptokens) == SEMICOLON_CHAR)  {
        int indexes[] = {save,save + 1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == RETURN && tpop(ptokens) == SEMICOLON_CHAR)  {
        int indexes[] = {save,save + 1};
        print_fit_rule(__func__, indexes, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == RETURN && expression() && tpop(ptokens) == SEMICOLON_CHAR)  {
        int indexes[] = {save,-1,ptokens->index - 1};
        print_fit_rule(__func__, indexes, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

