#ifndef COIN_EVALUATOR_H
#define COIN_EVALUATOR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  /* the expression node structur */
  typedef struct _so_eval_node {
    int id;              /* node id */
    float value;         /* used only by value nodes */
    char regname[2];     /* register/field name in SoCalculator */
    short regidx;        /* index into vector register */
    struct _so_eval_node *child1, *child2, *child3;
  } so_eval_node;
  
  /* callbacks are needed to read/write values from/to fields */
  typedef struct {
    void (*readfieldcb)(const char *reg, float *data, int fieldidx, int component, void *userdata);
    void (*writefieldcb)(const char *reg, float *data, int fieldidx, int component, void *userdata);
    void *userdata;
  } so_eval_cbdata;

  /* create a tree structure from expression string */
  so_eval_node *so_eval_parse(const char *buffer); /* defined in epsilon.y */

  /* free memory used by tree structure */
  void so_eval_delete(so_eval_node *node);
  
  /* evaluates the tree structure */
  void so_eval_evaluate(so_eval_node *node, int fieldidx, const so_eval_cbdata *cbdata);

  /* methods to create misc nodes */
  so_eval_node *so_eval_create_unary(int id, so_eval_node *topnode);
  so_eval_node *so_eval_create_binary(int id, so_eval_node *lhs, so_eval_node *rhs);
  so_eval_node *so_eval_create_ternary(int id, so_eval_node *cond, 
                                       so_eval_node *branch1, so_eval_node *branch2);
  so_eval_node *so_eval_create_reg(const char *regname);
  so_eval_node *so_eval_create_reg_comp(const char *regname, int index);
  so_eval_node *so_eval_create_flt_val(float val);
    

/* node ids */
enum {
  ID_ADD,
  ID_ADD_VEC,
  ID_SUB,
  ID_SUB_VEC,
  ID_MUL,
  ID_DIV,
  ID_NEG,
  ID_NEG_VEC,
  ID_AND,
  ID_OR,
  ID_LEQ,
  ID_GEQ,
  ID_EQ,
  ID_NEQ,
  ID_COS,
  ID_SIN,
  ID_TAN,
  ID_ACOS,
  ID_ASIN,
  ID_ATAN,
  ID_ATAN2,
  ID_COSH,
  ID_SINH,
  ID_TANH,
  ID_SQRT,
  ID_EXP,
  ID_LOG,
  ID_LOG10,
  ID_CEIL,
  ID_FLOOR,
  ID_FABS,
  ID_FMOD,
  ID_RAND,
  ID_CROSS,
  ID_DOT,
  ID_LEN,
  ID_NORMALIZE,
  ID_TEST_FLT,
  ID_TEST_VEC,
  ID_VEC3F,
  ID_FLT_REG,
  ID_VEC_REG,
  ID_VEC_REG_COMP,
  ID_FLT_COND,
  ID_VEC_COND,
  ID_VALUE,
  ID_ASSIGN_FLT,
  ID_ASSIGN_VEC,
  ID_SEPARATOR,
  ID_NOT,
  ID_LT,
  ID_GT,
  ID_POW
};

/* lexical tokens */
/* do not uncomment. Bison will define these
enum {
  LEX_FLTFUNC,
  LEX_COMPARE,
  LEX_AND,
  LEX_OR,
  LEX_POW,
  LEX_FMOD,
  LEX_VEC3F,
  LEX_VALUE,
  LEX_TMP_FLT_REG,
  LEX_OUT_FLT_REG,
  LEX_IN_FLT_REG, 
  LEX_TMP_VEC_REG,
  LEX_OUT_VEC_REG,
  LEX_IN_VEC_REG, 
  LEX_LEN,
  LEX_ERROR
  };
*/
  
  
#ifdef __cplusplus
}; /* extern "C" */
#endif /* __cplusplus */

#endif /* COIN_EVALUATOR_H */


