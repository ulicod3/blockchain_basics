#include "mpc.h"

#define LASSERT(args, cond, err)                                               \
  if (!(cond)) {                                                               \
    lval_del(args);                                                            \
    return lval_err(err);                                                      \
  }

struct lval;
struct lenv;
typedef struct lval lval;
typedef struct lenv lenv;

typedef lval *(*lbuiltin)(lenv *, lval *);

// Lisp value
struct lval {
  int type;
  long num;
  char *err;
  char *sym;
  lbuiltin fun;
  int count;
  lval **cell;
};

enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR };

// Possible Error Types
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

lval *lval_read(mpc_ast_t *t);
void lval_expr_print(lval *v, char open, char close);
void lval_print(lval *v);
void lval_println(lval *v);
void lval_del(lval *v);
lval *lval_err(char *m);
lval *lval_pop(lval *v, int i);
lval *lval_take(lval *v, int i);
lval *lval_add(lval *v, lval *x);
lval *lval_sexpr(void);
lval *lval_qexpr(void);
