#include "eval.h"

lval* lval_eval(lenv* e, lval* v) {
    if (v->type == LVAL_SYM) {
        lval* x = lenv_get(e, v);
        lval_del(v);
        return x;
    }
    if (v->type == LVAL_SEXPR) {
        return lval_eval_sexpr(e, v);
    }
    return v;
}

lval* lval_eval_sexpr(lenv* e, lval* v) {
    for (int i = 0; i < v->count; i++) {
        v->cell[i] = lval_eval(e, v->cell[i]);
    }

    for (int i = 0; i < v->count; i++) {
        if (v->cell[i]->type == LVAL_ERR) {
            return lval_take(v, i);
        }
    }

    if (v->count == 0) { return v; }

    if (v->count == 1) { return lval_take(v, 0); }

    lval* f = lval_pop(v, 0);
    if (f->type != LVAL_FUN) {
        lval* err = lval_err("cannot evaluate %s; incorrect type for arg 0; got %s, expected %s",
                ltype_name(LVAL_SEXPR), ltype_name(f->type), ltype_name(LVAL_FUN));
        lval_del(v);
        lval_del(f);
        return err;
    }

    lval* result = lval_call(e, f, v);
    lval_del(f);
    return result;
}

lval* lval_call(lenv* e, lval* f, lval* a) {
    if (f->builtin) {
        return f->builtin(e, a);
    }

    int given = a->count;
    int total = f->formals->count;

    while (a->count) {
        if (f->formals->count == 0) {
            lval_del(a);
            return lval_err("function passed too many arguments; got %i, expected %i", given, total);
        }
        lval* sym = lval_pop(f->formals, 0);

        /* special case for variadic functions */
        if (strcmp(sym->sym, "&") == 0) {
            if (f->formals->count != 1) {
                lval_del(a);
                return lval_err("function format invalid; symbol '&' not followed by single symbol");
            }

            lval* nsym = lval_pop(f->formals, 0);
            lenv_put(f->env, nsym, builtin_list(e, a), false);
            lval_del(sym);
            lval_del(nsym);
            break;
        }

        lval* val = lval_pop(a, 0);

        lenv_put(f->env, sym, val, false);
        lval_del(sym);
        lval_del(val);
    }

    if (f->formals->count > 0 &&
            strcmp(f->formals->cell[0]->sym, "&") == 0) {
        if (f->formals->count != 2) {
            return lval_err("function format invalid; symbol '&' not followed by single symbol");
        }
        lval_del(lval_pop(f->formals, 0));
        lval* sym = lval_pop(f->formals, 0);
        lval* val = lval_qexpr();

        lenv_put(f->env, sym, val, false);
        lval_del(sym);
        lval_del(val);
    }

    lval_del(a);

    if (f->formals->count == 0) {
        f->env->parent = e;
        return builtin_eval(f->env, lval_add(lval_sexpr(), lval_copy(f->body)));
    } else {
        return lval_copy(f);
    }
}

