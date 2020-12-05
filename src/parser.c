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

int translation_unit() {
    size_t save = tgeti(ptokens);
    if(external_declaration() && translation_unit_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int translation_unit_ext() {
    size_t save = tgeti(ptokens);
    if (tend(ptokens)) {
        printf("\n%s", "No tokens left to parse.");
        return 1;
    }
 
    if (external_declaration() && translation_unit_ext()) {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, 0);
        return 1;
    }
 
    if (EPS) {
        int indexes[] = {};
        print_fit_rule(__func__, indexes, -1);
        return 1;
    }
}

int external_declaration() {
    size_t save = tgeti(ptokens);
    if(function_definition())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(declaration())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int function_definition() {
    size_t save = tgeti(ptokens);
    if(declaration_specifiers() && declarator() && compound_statement())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int declaration_specifiers() {
    size_t save = tgeti(ptokens);
    if(type_specifier() && declaration_specifiers())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(type_specifier())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == CONST && declaration_specifiers())  {
        const char *symbols[] = {"CONST", "'declaration_specifiers'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == CONST)  {
        const char *symbols[] = {"CONST"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int type_specifier() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == VOID)  {
        const char *symbols[] = {"VOID"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == CHAR)  {
        const char *symbols[] = {"CHAR"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SHORT)  {
        const char *symbols[] = {"SHORT"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == INT)  {
        const char *symbols[] = {"INT"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == LONG)  {
        const char *symbols[] = {"LONG"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FLOAT)  {
        const char *symbols[] = {"FLOAT"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DOUBLE)  {
        const char *symbols[] = {"DOUBLE"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SIGNED)  {
        const char *symbols[] = {"SIGNED"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == UNSIGNED)  {
        const char *symbols[] = {"UNSIGNED"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int declarator() {
    size_t save = tgeti(ptokens);
    if(pointer() && direct_declarator())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(direct_declarator())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int pointer() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == MUL_CHAR && pointer())  {
        const char *symbols[] = {"MUL_CHAR", "'pointer'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MUL_CHAR)  {
        const char *symbols[] = {"MUL_CHAR"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_declarator() {
    size_t save = tgeti(ptokens);
    if(direct_declarator_others() && direct_declarator_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_declarator_ext() {
    size_t save = tgeti(ptokens);
    if(direct_declarator_operators() && direct_declarator_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_declarator_operators() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_SQUARE && tpop(ptokens) == CLOSE_SQUARE)  {
        const char *symbols[] = {"OPEN_SQUARE", "CLOSE_SQUARE"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_SQUARE && constant_expression() && tpop(ptokens) == CLOSE_SQUARE)  {
        const char *symbols[] = {"OPEN_SQUARE", "'constant_expression'", "CLOSE_SQUARE"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && parameter_list() && tpop(ptokens) == CLOSE_BRACKET)  {
        const char *symbols[] = {"OPEN_BRACKET", "'parameter_list'", "CLOSE_BRACKET"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && tpop(ptokens) == CLOSE_BRACKET)  {
        const char *symbols[] = {"OPEN_BRACKET", "CLOSE_BRACKET"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && identifier_list() && tpop(ptokens) == CLOSE_BRACKET)  {
        const char *symbols[] = {"OPEN_BRACKET", "'identifier_list'", "CLOSE_BRACKET"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_declarator_others() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_BRACKET && declarator() && tpop(ptokens) == CLOSE_BRACKET)  {
        const char *symbols[] = {"OPEN_BRACKET", "'declarator'", "CLOSE_BRACKET"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == IDENTIFIER)  {
        const char *symbols[] = {"IDENTIFIER"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int assignment_expression() {
    size_t save = tgeti(ptokens);
    if(logical_or_expression() && assignment_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int assignment_expression_ext() {
    size_t save = tgeti(ptokens);
    if(assignment_operator() && unary_expression())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int logical_or_expression() {
    size_t save = tgeti(ptokens);
    if(logical_and_expression() && logical_or_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int logical_or_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OR_OP && logical_or_expression())  {
        const char *symbols[] = {"OR_OP", "'logical_or_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int logical_and_expression() {
    size_t save = tgeti(ptokens);
    if(inclusive_or_expression() && logical_and_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int logical_and_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == AND_OP && logical_and_expression())  {
        const char *symbols[] = {"AND_OP", "'logical_and_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int inclusive_or_expression() {
    size_t save = tgeti(ptokens);
    if(exclusive_or_expression() && inclusive_or_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int inclusive_or_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OR_CHAR && inclusive_or_expression())  {
        const char *symbols[] = {"OR_CHAR", "'inclusive_or_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int exclusive_or_expression() {
    size_t save = tgeti(ptokens);
    if(and_expression() && exclusive_or_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int exclusive_or_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == XOR_CHAR && exclusive_or_expression())  {
        const char *symbols[] = {"XOR_CHAR", "'exclusive_or_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int and_expression() {
    size_t save = tgeti(ptokens);
    if(equality_expression() && and_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int and_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == AND_CHAR && and_expression())  {
        const char *symbols[] = {"AND_CHAR", "'and_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int equality_expression() {
    size_t save = tgeti(ptokens);
    if(relational_expression() && equality_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int equality_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == NE_OP && equality_expression())  {
        const char *symbols[] = {"NE_OP", "'equality_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == EQ_OP && equality_expression())  {
        const char *symbols[] = {"EQ_OP", "'equality_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int relational_expression() {
    size_t save = tgeti(ptokens);
    if(shift_expression() && relational_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int relational_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == LESS_CHAR && relational_expression())  {
        const char *symbols[] = {"LESS_CHAR", "'relational_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == GREATER_CHAR && relational_expression())  {
        const char *symbols[] = {"GREATER_CHAR", "'relational_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == LE_OP && relational_expression())  {
        const char *symbols[] = {"LE_OP", "'relational_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == GE_OP && relational_expression())  {
        const char *symbols[] = {"GE_OP", "'relational_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int shift_expression() {
    size_t save = tgeti(ptokens);
    if(additive_expression() && shift_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int shift_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == LEFT_OP && shift_expression())  {
        const char *symbols[] = {"LEFT_OP", "'shift_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == RIGHT_OP && shift_expression())  {
        const char *symbols[] = {"RIGHT_OP", "'shift_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int additive_expression() {
    size_t save = tgeti(ptokens);
    if(multiplicative_expression() && additive_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int additive_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == PLUS_CHAR && additive_expression())  {
        const char *symbols[] = {"PLUS_CHAR", "'additive_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MINUS_CHAR && additive_expression())  {
        const char *symbols[] = {"MINUS_CHAR", "'additive_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int multiplicative_expression() {
    size_t save = tgeti(ptokens);
    if(cast_expression() && multiplicative_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int multiplicative_expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == MUL_CHAR && multiplicative_expression())  {
        const char *symbols[] = {"MUL_CHAR", "'multiplicative_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DIV_CHAR && multiplicative_expression())  {
        const char *symbols[] = {"DIV_CHAR", "'multiplicative_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MOD_CHAR && multiplicative_expression())  {
        const char *symbols[] = {"MOD_CHAR", "'multiplicative_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int cast_expression() {
    size_t save = tgeti(ptokens);
    if(unary_expression())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && type_name() && tpop(ptokens) == CLOSE_BRACKET && cast_expression())  {
        const char *symbols[] = {"OPEN_BRACKET", "'type_name'", "CLOSE_BRACKET", "'cast_expression'"};
        print_fit_rule(__func__, symbols, 4);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int unary_expression() {
    size_t save = tgeti(ptokens);
    if(postfix_expression())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == INC_OP && unary_expression())  {
        const char *symbols[] = {"INC_OP", "'unary_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DEC_OP && unary_expression())  {
        const char *symbols[] = {"DEC_OP", "'unary_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SIZEOF && unary_expression())  {
        const char *symbols[] = {"SIZEOF", "'unary_expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SIZEOF && tpop(ptokens) == OPEN_BRACKET && type_name() && tpop(ptokens) == CLOSE_BRACKET)  {
        const char *symbols[] = {"SIZEOF", "OPEN_BRACKET", "'type_name'", "CLOSE_BRACKET"};
        print_fit_rule(__func__, symbols, 4);
        return 1;
    }

    tseti(ptokens, save);
    if(unary_operator() && cast_expression())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int postfix_expression() {
    size_t save = tgeti(ptokens);
    if(primary_expression() && postfix_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int postfix_expression_ext() {
    size_t save = tgeti(ptokens);
    if(postfix_expression_operators() && postfix_expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int postfix_expression_operators() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_SQUARE && expression() && tpop(ptokens) == CLOSE_SQUARE)  {
        const char *symbols[] = {"OPEN_SQUARE", "'expression'", "CLOSE_SQUARE"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && tpop(ptokens) == CLOSE_BRACKET)  {
        const char *symbols[] = {"OPEN_BRACKET", "CLOSE_BRACKET"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && argument_expression_list() && tpop(ptokens) == CLOSE_BRACKET)  {
        const char *symbols[] = {"OPEN_BRACKET", "'argument_expression_list'", "CLOSE_BRACKET"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == INC_OP)  {
        const char *symbols[] = {"INC_OP"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DEC_OP)  {
        const char *symbols[] = {"DEC_OP"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int primary_expression() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == IDENTIFIER)  {
        const char *symbols[] = {"IDENTIFIER"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == I_CONSTANT)  {
        const char *symbols[] = {"I_CONSTANT"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == F_CONSTANT)  {
        const char *symbols[] = {"F_CONSTANT"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == STRING_LITERAL)  {
        const char *symbols[] = {"STRING_LITERAL"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET)  {
        const char *symbols[] = {"OPEN_BRACKET", "'expression'", "CLOSE_BRACKET"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int expression() {
    size_t save = tgeti(ptokens);
    if(assignment_expression() && expression_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int expression_ext() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == COMMA_CHAR && expression())  {
        const char *symbols[] = {"COMMA_CHAR", "'expression'"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int argument_expression_list() {
    size_t save = tgeti(ptokens);
    if(assignment_expression() && tpop(ptokens) == COMMA_CHAR && argument_expression_list())  {
        const char *symbols[] = {"'assignment_expression'", "COMMA_CHAR", "'argument_expression_list'"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(assignment_expression())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int unary_operator() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == AND_CHAR)  {
        const char *symbols[] = {"AND_CHAR"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MUL_CHAR)  {
        const char *symbols[] = {"MUL_CHAR"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == PLUS_CHAR)  {
        const char *symbols[] = {"PLUS_CHAR"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MINUS_CHAR)  {
        const char *symbols[] = {"MINUS_CHAR"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == TILDA_CHAR)  {
        const char *symbols[] = {"TILDA_CHAR"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == EXCL_CHAR)  {
        const char *symbols[] = {"EXCL_CHAR"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int type_name() {
    size_t save = tgeti(ptokens);
    if(specifier_qualifier_list() && abstract_declarator())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(specifier_qualifier_list())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int specifier_qualifier_list() {
    size_t save = tgeti(ptokens);
    if(type_specifier() && specifier_qualifier_list())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(type_specifier())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int abstract_declarator() {
    size_t save = tgeti(ptokens);
    if(pointer() && direct_abstract_declarator())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(pointer())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(direct_abstract_declarator())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_abstract_declarator() {
    size_t save = tgeti(ptokens);
    if(direct_abstract_declarator_others() && direct_abstract_declarator_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_abstract_declarator_ext() {
    size_t save = tgeti(ptokens);
    if(direct_abstract_declarator_operators() && direct_abstract_declarator_ext())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(EPS)  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, -1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_abstract_declarator_operators() {
    size_t save = tgeti(ptokens);
    if(direct_abstract_declarator_main())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && parameter_list() && tpop(ptokens) == CLOSE_BRACKET)  {
        const char *symbols[] = {"OPEN_BRACKET", "'parameter_list'", "CLOSE_BRACKET"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_abstract_declarator_others() {
    size_t save = tgeti(ptokens);
    if(direct_abstract_declarator_main())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && abstract_declarator() && tpop(ptokens) == CLOSE_BRACKET)  {
        const char *symbols[] = {"OPEN_BRACKET", "'abstract_declarator'", "CLOSE_BRACKET"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int direct_abstract_declarator_main() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_SQUARE && tpop(ptokens) == CLOSE_SQUARE)  {
        const char *symbols[] = {"OPEN_SQUARE", "CLOSE_SQUARE"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_BRACKET && tpop(ptokens) == CLOSE_BRACKET)  {
        const char *symbols[] = {"OPEN_BRACKET", "CLOSE_BRACKET"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_SQUARE && constant_expression() && tpop(ptokens) == CLOSE_SQUARE)  {
        const char *symbols[] = {"OPEN_SQUARE", "'constant_expression'", "CLOSE_SQUARE"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int parameter_list() {
    size_t save = tgeti(ptokens);
    if(parameter_declaration() && tpop(ptokens) == COMMA_CHAR && parameter_list())  {
        const char *symbols[] = {"'parameter_declaration'", "COMMA_CHAR", "'parameter_list'"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(parameter_declaration())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int parameter_declaration() {
    size_t save = tgeti(ptokens);
    if(declaration_specifiers() && declarator())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(declaration_specifiers() && abstract_declarator())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(declaration_specifiers())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int identifier_list() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == IDENTIFIER && tpop(ptokens) == COMMA_CHAR && identifier_list())  {
        const char *symbols[] = {"IDENTIFIER", "COMMA_CHAR", "'identifier_list'"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == IDENTIFIER)  {
        const char *symbols[] = {"IDENTIFIER"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int assignment_operator() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == EQ_CHAR)  {
        const char *symbols[] = {"EQ_CHAR"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MUL_ASSIGN)  {
        const char *symbols[] = {"MUL_ASSIGN"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DIV_ASSIGN)  {
        const char *symbols[] = {"DIV_ASSIGN"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == MOD_ASSIGN)  {
        const char *symbols[] = {"MOD_ASSIGN"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == ADD_ASSIGN)  {
        const char *symbols[] = {"ADD_ASSIGN"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SUB_ASSIGN)  {
        const char *symbols[] = {"SUB_ASSIGN"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == LEFT_ASSIGN)  {
        const char *symbols[] = {"LEFT_ASSIGN"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == RIGHT_ASSIGN)  {
        const char *symbols[] = {"RIGHT_ASSIGN"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == AND_ASSIGN)  {
        const char *symbols[] = {"AND_ASSIGN"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == XOR_ASSIGN)  {
        const char *symbols[] = {"XOR_ASSIGN"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OR_ASSIGN)  {
        const char *symbols[] = {"OR_ASSIGN"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int compound_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_CURLY && tpop(ptokens) == CLOSE_CURLY)  {
        const char *symbols[] = {"OPEN_CURLY", "CLOSE_CURLY"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == OPEN_CURLY && block_item_list() && tpop(ptokens) == CLOSE_CURLY)  {
        const char *symbols[] = {"OPEN_CURLY", "'block_item_list'", "CLOSE_CURLY"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int block_item_list() {
    size_t save = tgeti(ptokens);
    if(block_item() && block_item_list())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(block_item())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int block_item() {
    size_t save = tgeti(ptokens);
    if(declaration())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(statement())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int declaration() {
    size_t save = tgeti(ptokens);
    if(declaration_specifiers() && init_declarator_list() && tpop(ptokens) == SEMICOLON_CHAR)  {
        const char *symbols[] = {"'declaration_specifiers'", "'init_declarator_list'", "SEMICOLON_CHAR"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(declaration_specifiers() && tpop(ptokens) == SEMICOLON_CHAR)  {
        const char *symbols[] = {"'declaration_specifiers'", "SEMICOLON_CHAR"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int init_declarator_list() {
    size_t save = tgeti(ptokens);
    if(init_declarator() && tpop(ptokens) == COMMA_CHAR && init_declarator_list())  {
        const char *symbols[] = {"'init_declarator'", "COMMA_CHAR", "'init_declarator_list'"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(init_declarator())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int init_declarator() {
    size_t save = tgeti(ptokens);
    if(declarator() && tpop(ptokens) == EQ_CHAR && initializer())  {
        const char *symbols[] = {"'declarator'", "EQ_CHAR", "'initializer'"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(declarator())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int initializer() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == OPEN_CURLY && initializer_list() && tpop(ptokens) == CLOSE_CURLY)  {
        const char *symbols[] = {"OPEN_CURLY", "'initializer_list'", "CLOSE_CURLY"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(assignment_expression())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int initializer_list() {
    size_t save = tgeti(ptokens);
    if(initializer() && tpop(ptokens) == COMMA_CHAR && initializer_list())  {
        const char *symbols[] = {"'initializer'", "COMMA_CHAR", "'initializer_list'"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(initializer())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int statement() {
    size_t save = tgeti(ptokens);
    if(labeled_statement())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(compound_statement())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(expression_statement())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(selection_statement())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(iteration_statement())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    if(jump_statement())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int labeled_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == IDENTIFIER && tpop(ptokens) == COLON_CHAR && statement())  {
        const char *symbols[] = {"IDENTIFIER", "COLON_CHAR", "'statement'"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == CASE && constant_expression() && tpop(ptokens) == COLON_CHAR && statement())  {
        const char *symbols[] = {"CASE", "'constant_expression'", "COLON_CHAR", "'statement'"};
        print_fit_rule(__func__, symbols, 4);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DEFAULT && tpop(ptokens) == COLON_CHAR && statement())  {
        const char *symbols[] = {"DEFAULT", "COLON_CHAR", "'statement'"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int constant_expression() {
    size_t save = tgeti(ptokens);
    if(logical_or_expression())  {
        const char *symbols[] = {};
        print_fit_rule(__func__, symbols, 0);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int expression_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == SEMICOLON_CHAR)  {
        const char *symbols[] = {"SEMICOLON_CHAR"};
        print_fit_rule(__func__, symbols, 1);
        return 1;
    }

    tseti(ptokens, save);
    if(expression() && tpop(ptokens) == SEMICOLON_CHAR)  {
        const char *symbols[] = {"'expression'", "SEMICOLON_CHAR"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int selection_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == IF && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && statement() && tpop(ptokens) == ELSE && statement())  {
        const char *symbols[] = {"IF", "OPEN_BRACKET", "'expression'", "CLOSE_BRACKET", "'statement'", "ELSE", "'statement'"};
        print_fit_rule(__func__, symbols, 7);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == IF && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        const char *symbols[] = {"IF", "OPEN_BRACKET", "'expression'", "CLOSE_BRACKET", "'statement'"};
        print_fit_rule(__func__, symbols, 5);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == SWITCH && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        const char *symbols[] = {"SWITCH", "OPEN_BRACKET", "'expression'", "CLOSE_BRACKET", "'statement'"};
        print_fit_rule(__func__, symbols, 5);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int iteration_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == WHILE && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        const char *symbols[] = {"WHILE", "OPEN_BRACKET", "'expression'", "CLOSE_BRACKET", "'statement'"};
        print_fit_rule(__func__, symbols, 5);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == DO && statement() && tpop(ptokens) == WHILE && tpop(ptokens) == OPEN_BRACKET && expression() && tpop(ptokens) == CLOSE_BRACKET && tpop(ptokens) == SEMICOLON_CHAR)  {
        const char *symbols[] = {"DO", "'statement'", "WHILE", "OPEN_BRACKET", "'expression'", "CLOSE_BRACKET", "SEMICOLON_CHAR"};
        print_fit_rule(__func__, symbols, 7);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FOR && tpop(ptokens) == OPEN_BRACKET && expression_statement() && expression_statement() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        const char *symbols[] = {"FOR", "OPEN_BRACKET", "'expression_statement'", "'expression_statement'", "CLOSE_BRACKET", "'statement'"};
        print_fit_rule(__func__, symbols, 6);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FOR && tpop(ptokens) == OPEN_BRACKET && expression_statement() && expression_statement() && expression() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        const char *symbols[] = {"FOR", "OPEN_BRACKET", "'expression_statement'", "'expression_statement'", "'expression'", "CLOSE_BRACKET", "'statement'"};
        print_fit_rule(__func__, symbols, 7);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FOR && tpop(ptokens) == OPEN_BRACKET && declaration() && expression_statement() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        const char *symbols[] = {"FOR", "OPEN_BRACKET", "'declaration'", "'expression_statement'", "CLOSE_BRACKET", "'statement'"};
        print_fit_rule(__func__, symbols, 6);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == FOR && tpop(ptokens) == OPEN_BRACKET && declaration() && expression_statement() && expression() && tpop(ptokens) == CLOSE_BRACKET && statement())  {
        const char *symbols[] = {"FOR", "OPEN_BRACKET", "'declaration'", "'expression_statement'", "'expression'", "CLOSE_BRACKET", "'statement'"};
        print_fit_rule(__func__, symbols, 7);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

int jump_statement() {
    size_t save = tgeti(ptokens);
    if(tpop(ptokens) == CONTINUE && tpop(ptokens) == SEMICOLON_CHAR)  {
        const char *symbols[] = {"CONTINUE", "SEMICOLON_CHAR"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == BREAK && tpop(ptokens) == SEMICOLON_CHAR)  {
        const char *symbols[] = {"BREAK", "SEMICOLON_CHAR"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == RETURN && tpop(ptokens) == SEMICOLON_CHAR)  {
        const char *symbols[] = {"RETURN", "SEMICOLON_CHAR"};
        print_fit_rule(__func__, symbols, 2);
        return 1;
    }

    tseti(ptokens, save);
    if(tpop(ptokens) == RETURN && expression() && tpop(ptokens) == SEMICOLON_CHAR)  {
        const char *symbols[] = {"RETURN", "'expression'", "SEMICOLON_CHAR"};
        print_fit_rule(__func__, symbols, 3);
        return 1;
    }

    tseti(ptokens, save);
    print_not_fit_rule(save, __func__);
    return 0;
}

