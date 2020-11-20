//
// Created by madopew on 17.11.2020.
//

#include "../headers/parser.h"
#include "../headers/token_stream.h"
#include "../headers/tokentypes.h"

tstream *ptokens;

int parse(tstream *s) {
    ptokens = s;
    if(translation_unit() && tend(ptokens)) {
        return 1;
    } else {
        return 0;
    }
}

int translation_unit() {
    if(tend(ptokens)) {
        return 1;
    }

    size_t save = tgeti(ptokens);
    if(external_declaration() && translation_unit()) {
        return 1;
    }

    tseti(ptokens, save);
    if(external_declaration()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
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
    if(declaration_specifiers() &&
            declarator() &&
            compound_statement()) {
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int declaration_specifiers() {
    size_t save = tgeti(ptokens);
    if(type_specifier()) {
        declaration_specifiers();
        return 1;
    }

    tseti(ptokens, save);
    if(tpeek(ptokens) == CONST) {
        tpop(ptokens);
        declaration_specifiers();
        return 1;
    }

    tseti(ptokens, save);
    return 0;
}

int type_specifier() {
    int token = tpeek(ptokens);
    const int AVAILABLE[] = {VOID, CHAR, SHORT, INT, LONG, FLOAT, DOUBLE, SIGNED, UNSIGNED};
    for(int i = 0; i < sizeof(AVAILABLE); i++) {
        if(token == AVAILABLE[i]) {
            tpop(ptokens);
            return 1;
        }
    }
    return 0;
}

int declarator() {
    size_t save = tgeti(ptokens);
    if (pointer() && direct_declarator()) {
        return 1;
    }



    tseti(ptokens, save);
    return 0;
}

int pointer() {
    if(tpeek(ptokens) == MUL_CHAR) {
        tpop(ptokens);
        pointer();
        return 1;
    } else {
        return 0;
    }
}

int direct_declarator() {
    if(tpeek(ptokens) == IDENTIFIER) {
        tpop(ptokens);
        return 1;
    } else if(tpeek(ptokens) == OPEN_BRACKET) {
        tpop(ptokens);
        if(declarator() &&
                tpeek(ptokens) == CLOSE_BRACKET) {
            tpop(ptokens);
            return 1;
        } else {
            return 0;
        }
    } else if(direct_declarator()) {
        if(tpeek(ptokens) == OPEN_SQUARE) {
            tpop(ptokens);
            assignment_expression();
            if(tpeek(ptokens) == CLOSE_SQUARE) {
                tpop(ptokens);
                return 1;
            } else {
                return 0;
            }
        } else if(tpeek(ptokens) == OPEN_BRACKET) {
            tpop(ptokens);
            if(tpeek(ptokens) == CLOSE_BRACKET) {
                tpop(ptokens);
                return 1;
            } else if(parameter_list() &&
                    tpeek(ptokens) == CLOSE_BRACKET) {
                tpop(ptokens);
                return 1;
            } else if(identifier_list() &&
                    tpeek(ptokens) == CLOSE_BRACKET) {
                tpop(ptokens);
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

int assignment_expression() {
    if(unary_expression() &&
            assignment_operator() &&
            logical_or_expression()) {
        return 1;
    } else if(logical_or_expression()) {
        return 1;
    } else {
        return 0;
    }
}

int logical_or_expression() {
    if(logical_and_expression()) {
        if(tpeek(ptokens) == OR_OP) {
            tpop(ptokens);
            if(logical_or_expression()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int logical_and_expression() {
    if(inclusive_or_expression()) {
        if(tpeek(ptokens) == AND_OP) {
            tpop(ptokens);
            if(logical_and_expression()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int inclusive_or_expression() {
    if(exclusive_or_expression()) {
        if(tpeek(ptokens) == OR_CHAR) {
            tpop(ptokens);
            if(inclusive_or_expression()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int exclusive_or_expression() {
    if(and_expression()) {
        if(tpeek(ptokens) == XOR_CHAR) {
            tpop(ptokens);
            if(exclusive_or_expression()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int and_expression() {
    if(equality_expression()) {
        if(tpeek(ptokens) == AND_CHAR) {
            tpop(ptokens);
            if(and_expression()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int equality_expression() {
    if(relational_expression()) {
        if(tpeek(ptokens) == EQ_OP ||
                tpeek(ptokens) == NE_OP) {
            tpop(ptokens);
            if(equality_expression()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int relational_expression() {
    if(shift_expression()) {
        if(tpeek(ptokens) == LESS_CHAR ||
                tpeek(ptokens) == GREATER_CHAR ||
                tpeek(ptokens) == LE_OP ||
                tpeek(ptokens) == GE_OP) {
            tpop(ptokens);
            if(relational_expression()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int shift_expression() {
    if(additive_expression()) {
        if(tpeek(ptokens) == LEFT_OP ||
                tpeek(ptokens) == RIGHT_OP) {
            tpop(ptokens);
            if(shift_expression()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int additive_expression() {
    if(multiplicative_expression()) {
        if(tpeek(ptokens) == PLUS_CHAR ||
                tpeek(ptokens) == MINUS_CHAR) {
            tpop(ptokens);
            if(additive_expression()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int multiplicative_expression() {
    if(cast_expression()) {
        if(tpeek(ptokens) == MUL_CHAR ||
                tpeek(ptokens) == DIV_CHAR ||
                tpeek(ptokens) == MOD_CHAR) {
            tpop(ptokens);
            if(multiplicative_expression()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int cast_expression() {
    if(unary_expression()) {
        return 1;
    } else if(tpeek(ptokens) == OPEN_BRACKET) {
        tpop(ptokens);
        if(type_name() &&
                tpeek(ptokens) == CLOSE_BRACKET) {
            tpop(ptokens);
            if(cast_expression()) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

int unary_expression() {
    if(postfix_expression()) {
        return 1;
    } else if(tpeek(ptokens) == INC_OP ||
            tpeek(ptokens) == DEC_OP) {
        tpop(ptokens);
        if(unary_expression()) {
            return 1;
        } else {
            return 0;
        }
    } else if(tpeek(ptokens) == SIZEOF) {
        tpop(ptokens);
        if(unary_expression()) {
            return 1;
        } else if(tpeek(ptokens) == OPEN_BRACKET) {
            tpop(ptokens);
            if(type_name() &&
                    tpeek(ptokens) == CLOSE_BRACKET) {
                tpop(ptokens);
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
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
    int token = tpeek(ptokens);
    const int AVAILABLE[] = {AND_CHAR, MUL_CHAR, PLUS_CHAR, MINUS_CHAR, TILDA_CHAR, EXCL_CHAR};
    for(int i = 0; i < sizeof(AVAILABLE); i++) {
        if(token == AVAILABLE[i]) {
            tpop(ptokens);
            return 1;
        }
    }
    return 0;
}

int type_name() {
    if(specifier_qualifier_list()) {
        abstract_declarator();
        return 1;
    } else {
        return 0;
    }
}

int specifier_qualifier_list() {
    if(type_specifier()) {
        specifier_qualifier_list();
        return 1;
    } else {
        return 0;
    }
}

int abstract_declarator() {
    if (pointer()) {
        direct_abstract_declarator();
        return 1;
    } else if (direct_abstract_declarator()) {
        return 1;
    } else {
        return 0;
    }
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
    if(declaration_specifiers()) {
        if(tpeek(ptokens) == SEMICOLON_CHAR) {
            tpop(ptokens);
            return 1;
        } else if(init_declarator_list()
                && tpeek(ptokens) == SEMICOLON_CHAR) {
            tpop(ptokens);
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
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