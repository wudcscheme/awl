#include "print.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

#include "mpc.h"
#include "assert.h"

static void awlval_expr_print(const awlval* v, char open, char close) {
    putchar(open);
    for (int i = 0; i < v->count; i++) {
        awlval_print(v->cell[i]);

        if (i != (v->count - 1)) {
            putchar(' ');
        }
    }
    putchar(close);
}

static void awlval_print_str(const awlval* v) {
    char* escaped = malloc(strlen(v->str) + 1);
    strcpy(escaped, v->str);

    escaped = mpcf_escape(escaped);
    printf("\"%s\"", escaped);

    free(escaped);
}

void awlval_println(const awlval* v) {
    awlval_print(v);
    putchar('\n');
}

void awlval_print(const awlval* v) {
    switch (v->type) {
        case AWLVAL_ERR:
            printf("Error: %s", v->err);
            break;

        case AWLVAL_INT:
            printf("%li", v->lng);
            break;

        case AWLVAL_FLOAT:
            printf("%f", v->dbl);
            break;

        case AWLVAL_SYM:
            printf("%s", v->sym);
            break;

        case AWLVAL_QSYM:
            printf(":%s", v->sym);
            break;

        case AWLVAL_STR:
            awlval_print_str(v);
            break;

        case AWLVAL_BOOL:
            if (v->bln) {
                printf("true");
            } else {
                printf("false");
            }
            break;

        case AWLVAL_BUILTIN:
            printf("<builtin %s>", v->builtin_name);
            break;

        case AWLVAL_FUNC:
            printf("(fn ");
            awlval_print(v->formals);
            putchar(' ');
            awlval_print(v->body);
            putchar(')');
            break;

        case AWLVAL_MACRO:
            printf("(macro ");
            awlval_print(v->formals);
            putchar(' ');
            awlval_print(v->body);
            putchar(')');
            break;

        case AWLVAL_SEXPR:
            awlval_expr_print(v, '(', ')');
            break;

        case AWLVAL_QEXPR:
            awlval_expr_print(v, '{', '}');
            break;

        case AWLVAL_EEXPR:
            awlval_expr_print(v, '\\', '\0');
            break;

        case AWLVAL_CEXPR:
            awlval_expr_print(v, '@', '\0');
            break;
    }
}

