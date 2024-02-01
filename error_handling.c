// Lisp Value: Make errors a possible result of evaluating an expression.
#include "error_handling.h"
#include "mpc.h"
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { LVAL_NUM, LVAL_ERR };
// Possible Error Types
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

lval lval_num(long x) {
  lval v;
  v.type = LVAL_NUM;
  v.num = x;
  return v;
}

lval lval_err(int x) {
  lval v;
  v.type = LVAL_ERR;
  v.err = x;
  return v;
}

// Print an lval
void lval_print(lval v) {
  switch (v.type) {
  case LVAL_NUM:
    printf("%li", v.num);
    break;
  case LVAL_ERR:
    if (v.err == LERR_DIV_ZERO) {
      printf("Error: Division by zero!");
    }
    if (v.err == LERR_BAD_OP) {
      printf("Error: Invalid Operator!");
    }
    if (v.err == LERR_BAD_NUM) {
      printf("Error: Invalid Number!");
    }
    break;
  }
}

void lval_println(lval v) {
  lval_print(v);
  putchar('\n');
}

// Evaluate
lval eval_op(lval x, char *op, lval y) {
  if (x.type == LVAL_ERR) {
    return x;
  }
  if (y.type == LVAL_ERR) {
    return y;
  }

  if (strcmp(op, "+") == 0) {
    return lval_num(x.num + y.num);
  }
  if (strcmp(op, "-") == 0) {
    return lval_num(x.num - y.num);
  }
  if (strcmp(op, "*") == 0) {
    return lval_num(x.num * y.num);
  }
  if (strcmp(op, "/") == 0) {
    return y.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x.num / y.num);
  }
  if (strcmp(op, "^") == 0) {
    return lval_num(pow(x.num, y.num));
  }
  if (strcmp(op, "%") == 0) {
    return lval_num(x.num % y.num);
  }

  return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t *t) {
  if (strstr(t->tag, "number")) {
    errno = 0;
    long x = strtol(t->contents, NULL, 10);
    return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
  }

  char *op = t->children[1]->contents;
  lval x = eval(t->children[2]);

  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }
  return x;
}
