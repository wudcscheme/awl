#include <stdio.h>
#include <stdbool.h>
#include "ptest.h"

#include "common.h"

void test_builtin_arithmetic(void) {
    awlenv* e = setup_test();

    AWL_ASSERT_CHAINED(e, "(+ 5 5)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 10L));

    AWL_ASSERT_CHAINED(e, "(- 5 30)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == -25L));

    AWL_ASSERT_CHAINED(e, "(* 10 8)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 80L));

    AWL_ASSERT_CHAINED(e, "(/ 6 3)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 2L));

    teardown_test(e);
}

void test_builtin_div(void) {
    // Tests proper integer promotion when performing division
    awlenv* e = setup_test();

    AWL_ASSERT_CHAINED(e, "(/ 5 5)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 1L));

    AWL_ASSERT_CHAINED(e, "(/ -5 5)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == -1L));

    AWL_ASSERT_CHAINED(e, "(/ 5 2)",
            AWL_IASSERT(v->type == AWLVAL_FLOAT)
            AWL_IASSERT(v->dbl == 2.5L));

    AWL_ASSERT_CHAINED(e, "(/ 5 5.0)",
            AWL_IASSERT(v->type == AWLVAL_FLOAT)
            AWL_IASSERT(v->dbl == 1.0L));

    AWL_ASSERT_CHAINED(e, "(/ 5.0 5)",
            AWL_IASSERT(v->type == AWLVAL_FLOAT)
            AWL_IASSERT(v->dbl == 1.0L));

    teardown_test(e);
}

void test_builtin_trunc_div(void) {
    // Tests proper integer demotion after performing division
    awlenv* e = setup_test();

    AWL_ASSERT_CHAINED(e, "(// 5 5)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 1L));

    AWL_ASSERT_CHAINED(e, "(// -5 5)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == -1L));

    AWL_ASSERT_CHAINED(e, "(// 5 2)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 2L));

    AWL_ASSERT_CHAINED(e, "(// -5 2)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == -2L));

    AWL_ASSERT_CHAINED(e, "(// 5.0 2)",
            AWL_IASSERT(v->type == AWLVAL_FLOAT)
            AWL_IASSERT(v->dbl == 2.0L));

    teardown_test(e);
}

void test_builtin_mod(void) {
    awlenv* e = setup_test();

    AWL_ASSERT_CHAINED(e, "(% 9 5)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 4L));

    AWL_ASSERT_CHAINED(e, "(% 9 9)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 0L));

    AWL_ASSERT_CHAINED(e, "(% 9 59)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 9L));

    AWL_ASSERT_CHAINED(e, "(% -9 5)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 4L));

    AWL_ASSERT_CHAINED(e, "(% 9 -5)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 4L));

    AWL_ASSERT_CHAINED(e, "(% -9 -5)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 4L));

    AWL_ASSERT_CHAINED(e, "(% 9.0 5)",
            AWL_IASSERT(v->type == AWLVAL_FLOAT)
            AWL_IASSERT(v->dbl == 4.0L));

    AWL_ASSERT_CHAINED(e, "(% 9.0 5.0)",
            AWL_IASSERT(v->type == AWLVAL_FLOAT)
            AWL_IASSERT(v->dbl == 4.0L));

    teardown_test(e);
}

void test_builtin_pow(void) {
    awlenv* e = setup_test();

    AWL_ASSERT_CHAINED(e, "(^ 2 0)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 1L));

    AWL_ASSERT_CHAINED(e, "(^ 2 1)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 2L));

    AWL_ASSERT_CHAINED(e, "(^ 2 2)",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 4L));

    AWL_ASSERT_CHAINED(e, "(^ 2 -1)",
            AWL_IASSERT(v->type == AWLVAL_FLOAT)
            AWL_IASSERT(v->dbl == 0.5L));

    AWL_ASSERT_CHAINED(e, "(^ 2 -2)",
            AWL_IASSERT(v->type == AWLVAL_FLOAT)
            AWL_IASSERT(v->dbl == 0.25L));

    AWL_ASSERT_CHAINED(e, "(^ 4 0.5)",
            AWL_IASSERT(v->type == AWLVAL_FLOAT)
            AWL_IASSERT(v->dbl == 2.0L));

    // Square root of a negative number
    AWL_ASSERT_TYPE(e, "(^ -4 0.5)", AWLVAL_ERR);

    teardown_test(e);
}

void test_builtin_equality(void) {
    awlenv* e = setup_test();

    // Numeric
    AWL_ASSERT_CHAINED(e, "(== 5 5)",
            AWL_IASSERT(v->type == AWLVAL_BOOL)
            AWL_IASSERT(v->bln == true));

    AWL_ASSERT_CHAINED(e, "(== 5 5.0)",
            AWL_IASSERT(v->type == AWLVAL_BOOL)
            AWL_IASSERT(v->bln == true));

    // Strings
    AWL_ASSERT_CHAINED(e, "(== 'a' 'a')",
            AWL_IASSERT(v->type == AWLVAL_BOOL)
            AWL_IASSERT(v->bln == true));

    AWL_ASSERT_CHAINED(e, "(== 'a' 'ab')",
            AWL_IASSERT(v->type == AWLVAL_BOOL)
            AWL_IASSERT(v->bln == false));

    // Booleans
    AWL_ASSERT_CHAINED(e, "(== true false)",
            AWL_IASSERT(v->type == AWLVAL_BOOL)
            AWL_IASSERT(v->bln == false));

    AWL_ASSERT_CHAINED(e, "(== false false)",
            AWL_IASSERT(v->type == AWLVAL_BOOL)
            AWL_IASSERT(v->bln == true));

    // QExprs
    AWL_ASSERT_CHAINED(e, "(== {} {})",
            AWL_IASSERT(v->type == AWLVAL_BOOL)
            AWL_IASSERT(v->bln == true));

    AWL_ASSERT_CHAINED(e, "(== {x} {})",
            AWL_IASSERT(v->type == AWLVAL_BOOL)
            AWL_IASSERT(v->bln == false));

    AWL_ASSERT_CHAINED(e, "(== {x} {x})",
            AWL_IASSERT(v->type == AWLVAL_BOOL)
            AWL_IASSERT(v->bln == true));

    AWL_ASSERT_CHAINED(e, "(== {x {1 2 z}} {x {1 2 z}})",
            AWL_IASSERT(v->type == AWLVAL_BOOL)
            AWL_IASSERT(v->bln == true));

    teardown_test(e);
}

void test_builtin_head(void) {
    awlenv* e = setup_test();

    AWL_ASSERT_TYPE(e, "(head 5)", AWLVAL_ERR);
    AWL_ASSERT_TYPE(e, "(head {})", AWLVAL_ERR);
    AWL_ASSERT_TYPE(e, "(head {1 2 3} {4 5 6})", AWLVAL_ERR);
    AWL_ASSERT_TYPE(e, "(head {x y z})", AWLVAL_ERR);

    AWL_ASSERT_CHAINED(e, "(head {1 2 3})",
            AWL_IASSERT(v->type == AWLVAL_INT)
            AWL_IASSERT(v->lng == 1L));

    AWL_ASSERT_TYPE(e, "(head {{1 2} y z})", AWLVAL_QEXPR);

    teardown_test(e);
}

void test_builtin_tail(void) {
    awlenv* e = setup_test();

    AWL_ASSERT_TYPE(e, "(tail 20)", AWLVAL_ERR);
    AWL_ASSERT_TYPE(e, "(tail {})", AWLVAL_ERR);
    AWL_ASSERT_TYPE(e, "(tail {1 2} {3 4})", AWLVAL_ERR);

    AWL_ASSERT_CHAINED(e, "(tail {1 2 3 4})",
            AWL_IASSERT(v->type == AWLVAL_QEXPR)
            AWL_IASSERT(v->count == 3));

    AWL_ASSERT_CHAINED(e, "(tail {1})",
            AWL_IASSERT(v->type == AWLVAL_QEXPR)
            AWL_IASSERT(v->count == 0));

    teardown_test(e);
}

void test_builtin_first(void) {
    awlenv* e = setup_test();

    AWL_ASSERT_TYPE(e, "(first 20)", AWLVAL_ERR);
    AWL_ASSERT_TYPE(e, "(first {})", AWLVAL_ERR);
    AWL_ASSERT_TYPE(e, "(first {1 2} {3 4})", AWLVAL_ERR);

    AWL_ASSERT_CHAINED(e, "(first {1 2 3 4})",
            AWL_IASSERT(v->type == AWLVAL_QEXPR)
            AWL_IASSERT(v->count == 1)
            AWL_IASSERT(v->cell[0]->type == AWLVAL_INT)
            AWL_IASSERT(v->cell[0]->lng == 1L));

    AWL_ASSERT_CHAINED(e, "(first {1})",
            AWL_IASSERT(v->type == AWLVAL_QEXPR)
            AWL_IASSERT(v->count == 1)
            AWL_IASSERT(v->cell[0]->type == AWLVAL_INT)
            AWL_IASSERT(v->cell[0]->lng == 1L));

    teardown_test(e);
}

void test_builtin_last(void) {
    awlenv* e = setup_test();

    AWL_ASSERT_TYPE(e, "(last 20)", AWLVAL_ERR);
    AWL_ASSERT_TYPE(e, "(last {})", AWLVAL_ERR);
    AWL_ASSERT_TYPE(e, "(last {1 2} {3 4})", AWLVAL_ERR);

    AWL_ASSERT_CHAINED(e, "(last {1 2 3 4})",
            AWL_IASSERT(v->type == AWLVAL_QEXPR)
            AWL_IASSERT(v->count == 1)
            AWL_IASSERT(v->cell[0]->type == AWLVAL_INT)
            AWL_IASSERT(v->cell[0]->lng == 4L));

    AWL_ASSERT_CHAINED(e, "(last {1})",
            AWL_IASSERT(v->type == AWLVAL_QEXPR)
            AWL_IASSERT(v->count == 1)
            AWL_IASSERT(v->cell[0]->type == AWLVAL_INT)
            AWL_IASSERT(v->cell[0]->lng == 1L));

    teardown_test(e);
}

void test_builtin_list(void) {
    awlenv* e = setup_test();

    AWL_ASSERT_TYPE(e, "(list)", AWLVAL_QEXPR);
    AWL_ASSERT_TYPE(e, "(list 20)", AWLVAL_QEXPR);
    AWL_ASSERT_TYPE(e, "(list 20 13 2)", AWLVAL_QEXPR);
    AWL_ASSERT_TYPE(e, "(list {})", AWLVAL_QEXPR);
    AWL_ASSERT_TYPE(e, "(list {1 2} {3 4} 53)", AWLVAL_QEXPR);

    AWL_ASSERT_CHAINED(e, "(list 1 3 24 {21 32} :foo)",
            AWL_IASSERT(v->type == AWLVAL_QEXPR)
            AWL_IASSERT(v->count == 5));

    teardown_test(e);
}

void test_builtin_slice(void) {
    awlenv* e = setup_test();

    eval_string(e, "(global x {1 2 3 4 5 6})");

    AWL_ASSERT_EQ(e, "(slice x 0)", "{1 2 3 4 5 6}");
    AWL_ASSERT_EQ(e, "(slice x 2)", "{3 4 5 6}");
    AWL_ASSERT_EQ(e, "(slice x 2 4)", "{3 4}");
    AWL_ASSERT_EQ(e, "(slice x 2 50)", "{3 4 5 6}");
    AWL_ASSERT_EQ(e, "(slice x 2 2)", "{}");

    AWL_ASSERT_EQ(e, "(slice x 25)", "{}");
    AWL_ASSERT_EQ(e, "(slice x 25 50)", "{}");

    AWL_ASSERT_EQ(e, "(slice x 3 1)", "{4 3}");
    AWL_ASSERT_EQ(e, "(slice x 5 3)", "{6 5}");
    AWL_ASSERT_EQ(e, "(slice x 1 0)", "{2}");

    AWL_ASSERT_EQ(e, "(slice x -1)", "{6}");
    AWL_ASSERT_EQ(e, "(slice x -2)", "{5 6}");
    AWL_ASSERT_EQ(e, "(slice x -25)", "{1 2 3 4 5 6}");
    AWL_ASSERT_EQ(e, "(slice x -5 -2)", "{2 3 4}");
    AWL_ASSERT_EQ(e, "(slice x -2 -5)", "{5 4 3}");

    AWL_ASSERT_EQ(e, "(slice x 2 6 2)", "{3 5}");
    AWL_ASSERT_EQ(e, "(slice x 2 6 3)", "{3 6}");
    AWL_ASSERT_EQ(e, "(slice x 2 5 1)", "{3 4 5}");
    AWL_ASSERT_EQ(e, "(slice x 2 5 -1)", "{6 5 4}");
    AWL_ASSERT_EQ(e, "(slice x 2 5 -2)", "{6 4}");
    AWL_ASSERT_EQ(e, "(slice x 1 5 -3)", "{6 3}");

    AWL_ASSERT_EQ(e, "(slice x 5 1 -2)", "{2 4}");

    teardown_test(e);
}

void suite_builtin(void) {
    pt_add_test(test_builtin_arithmetic, "Test Arithmetic", "Suite Builtin");
    pt_add_test(test_builtin_div, "Test Div", "Suite Builtin");
    pt_add_test(test_builtin_trunc_div, "Test Truncating Div", "Suite Builtin");
    pt_add_test(test_builtin_mod, "Test Modulo", "Suite Builtin");
    pt_add_test(test_builtin_pow, "Test Pow", "Suite Builtin");
    pt_add_test(test_builtin_equality, "Test Equality", "Suite Builtin");
    pt_add_test(test_builtin_head, "Test Head", "Suite Builtin");
    pt_add_test(test_builtin_tail, "Test Tail", "Suite Builtin");
    pt_add_test(test_builtin_first, "Test First", "Suite Builtin");
    pt_add_test(test_builtin_last, "Test Last", "Suite Builtin");
    pt_add_test(test_builtin_list, "Test List", "Suite Builtin");
    pt_add_test(test_builtin_slice, "Test Slice", "Suite Builtin");
}
