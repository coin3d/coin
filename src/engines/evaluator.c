#include "evaluator.h"
#include <math.h>
#include <assert.h>
#include <stddef.h> /* NULL */


/* 
 * create node, initialize all values to default 
 */
static so_eval_node *
create_node(int id)
{
  so_eval_node *node = (so_eval_node*) malloc(sizeof(so_eval_node));
  node->id = id;
  node->child1 = NULL;
  node->child2 = NULL;
  node->child3 = NULL;
  node->regidx = -1;
  node->regname[0] = 'x';
  node->regname[1] = 0;
  node->value = 0.0f;
  return node;
}

/*
 * convenience method that creates an unary node
 */
so_eval_node *
so_eval_create_unary(int id, so_eval_node *topnode)
{
  so_eval_node *node = create_node(id);
  node->child1 = topnode;
  return node;
}

/*
 * convenience method that creates a binary node
 */
so_eval_node *
so_eval_create_binary(int id, so_eval_node *lhs, so_eval_node *rhs)
{
  so_eval_node *node = create_node(id);
  node->child1 = lhs;
  node->child2 = rhs;
  return node;
}

/*
 * convenience method that creates a ternary node
 */
so_eval_node *
so_eval_create_ternary(int id, so_eval_node *cond, so_eval_node *branch1, 
                       so_eval_node *branch2)
{
  so_eval_node *node = create_node(id);
  node->child1 = cond;
  node->child2 = branch1;
  node->child3 = branch2;
  return node;
}

/*
 * creates a node that "references" a field (register) in the SoCalculator node.
 */
so_eval_node *
so_eval_create_reg(const char *regname)
{
  so_eval_node *node;
  int id, idx;
  
  if (regname[0] == 't' || regname[0] == 'o') {
    idx = 1;
  }
  else idx = 0;

  if (regname[idx] >= 'a' && regname[idx] <= 'h') {
    node = create_node(ID_FLT_REG);
  }
  else if (regname[idx] >= 'A' && regname[idx] <= 'H') {
    node = create_node(ID_VEC_REG);
  }
  node->regname[0] = regname[0];
  node->regname[1] = regname[1];
  return node;
}

/*
 * creates a node that references a component in a SoCalculator vector field.
 */
so_eval_node *
so_eval_create_reg_comp(const char *regname, int index)
{
  so_eval_node *node = create_node(ID_VEC_REG_COMP);
  node->regname[0] = regname[0];
  node->regname[1] = regname[1];
  node->regidx = index;
  return node;
}

/*
 * creates a node that holds a float value.
 */
so_eval_node *
so_eval_create_flt_val(float val)
{
  so_eval_node *node = create_node(ID_VALUE);
  node->value = val;
  return node;
}

/*
 * used for returning values from the traverse method.
 */
typedef union {
  int trueorfalse;
  float value;
  float vec[3];
} so_eval_param;

/*
 * clamp-function
 */
static float 
clamp(float val, float minval, float maxval)
{
  if (val <= minval) return minval;
  else if (val >= maxval) return maxval;
  return val;
}

/*
 * returns the dot product of the two vectors.
 */
static float 
dot_product(float *v0, float *v1)
{
  return v0[0]*v1[0] + v0[1]*v1[1] + v0[2]*v1[2];
}

/*
 * treverses (evaluates) the tree structure.
 */
void 
so_eval_traverse(so_eval_node *node, so_eval_param *result, int fieldidx, const so_eval_cbdata *cbdata)
{
  so_eval_param param1, param2, param3;
  
  if (node->id != ID_FLT_COND && node->id != ID_VEC_COND &&
      node->id != ID_ASSIGN_FLT && node->id != ID_ASSIGN_VEC) {
    if (node->child1) so_eval_traverse(node->child1, &param1, fieldidx, cbdata);
    if (node->child2) so_eval_traverse(node->child2, &param2, fieldidx, cbdata);
    if (node->child3) so_eval_traverse(node->child3, &param3, fieldidx, cbdata);
  }

  switch (node->id) {
  case ID_ADD:
    result->value = param1.value + param2.value;
    break;
  case ID_ADD_VEC:
    result->vec[0] = param1.vec[0] + param2.vec[0];
    result->vec[1] = param1.vec[1] + param2.vec[1];
    result->vec[2] = param1.vec[2] + param2.vec[2];
    break;
  case ID_SUB:
    result->value = param1.value - param2.value;
    break;
  case ID_SUB_VEC:
    result->vec[0] = param1.vec[0] + param2.vec[0];
    result->vec[1] = param1.vec[1] + param2.vec[1];
    result->vec[2] = param1.vec[2] + param2.vec[2];
    break;
  case ID_MUL:
    result->value = param1.value * param2.value;
    break;
  case ID_DIV:
    if (param2.value == 0.0f) {
      result->value = param1.value / FLT_EPSILON; /* FIXME: is this ok? */
    }
    else {
      result->value = param1.value / param2.value;
    }
    break;
  case ID_FMOD:
    result->value = fmod(param1.value, param2.value);
    break;
  case ID_NEG:
    result->value = - param1.value;
    break;
  case ID_NEG_VEC:
    result->vec[0] = -param1.vec[0];
    result->vec[1] = -param1.vec[1];
    result->vec[2] = -param1.vec[2];
    break;
  case ID_AND:
    result->trueorfalse = param1.trueorfalse && param2.trueorfalse;
    break;
  case ID_OR:
    result->trueorfalse = param1.trueorfalse && param2.trueorfalse;
    break;
  case ID_LEQ:
    result->trueorfalse = param1.value <= param2.value;
    break;
  case ID_GEQ:
    result->trueorfalse = param1.value >= param2.value;
    break;
  case ID_EQ:
    result->trueorfalse = param1.value == param2.value;
    break;
  case ID_NEQ:
    result->trueorfalse = param1.value != param2.value;
    break;
  case ID_COS:
    result->value = (float)cos(param1.value);
    break;
  case ID_SIN:
    result->value = (float)sin(param1.value);
    break;
  case ID_TAN:
    result->value = (float)tan(param1.value);
    break;
  case ID_ACOS:
    result->value = (float)acos(clamp(param1.value, -1.0f, 1.0f));
    break;
  case ID_ASIN:
    result->value = (float)asin(clamp(param1.value, -1.0f, 1.0f));
    break;
  case ID_ATAN:
    result->value = (float)atan(param1.value);
    break;
  case ID_ATAN2:
    if (param2.value == 0.0) {
      result->value = (float)atan2(param1.value, FLT_EPSILON); /* FIXME: is this ok? */
    }
    else {
      result->value = (float)atan2(param1.value, param2.value);
    }
    break;
  case ID_COSH:
    result->value = (float) cosh(param1.value);
    break;
  case ID_SINH:
    result->value = (float) sinh(param1.value);
    break;
  case ID_TANH:
    result->value = (float) tanh(param1.value);
    break;
  case ID_SQRT:
    result->value = param1.value >= 0.0f ? (float) sqrt(param1.value) : 0.0f;
    break;
  case ID_EXP:
    result->value = (float) exp(param1.value);
    break;
  case ID_LOG:
    result->value = param1.value <= 0.0f ? -128.0f : (float) log(param1.value);
    break;
  case ID_LOG10:
    result->value = param1.value <= 0.0f ? -38.0f : (float)log10(param1.value);
    break;
  case ID_CEIL:
    result->value = ceil(param1.value);
    break;
  case ID_FLOOR:
    result->value = floor(param1.value);
    break;
  case ID_FABS:
    result->value = fabs(param1.value);
    break;
  case ID_RAND:
    assert(0 && "FIXME: not implemented");
    break;
  case ID_CROSS:
    result->vec[0] = param1.vec[1]*param2.vec[2] - param1.vec[2]*param2.vec[1];
    result->vec[1] = param1.vec[2]*param2.vec[0] - param1.vec[0]*param2.vec[2];
    result->vec[2] = param1.vec[0]*param2.vec[1] - param1.vec[1]*param2.vec[0];
    break;
  case ID_DOT:
    result->value = dot_product(param1.vec, param2.vec); 
    break;
  case ID_LENGTH:
    result->value = (float)sqrt(dot_product(param1.vec, param2.vec));
    break;
  case ID_NORMALIZE:
    {
      /* FIXME: or should normalize work on the parameter? */
      float len = sqrt(dot_product(param1.vec, param1.vec));
      if (len == 0.0f) len = FLT_EPSILON;
      result->vec[0] = param1.vec[0] / len;
      result->vec[1] = param1.vec[1] / len;
      result->vec[2] = param1.vec[2] / len;
    }
    break;
  case ID_TEST_FLT:
    result->trueorfalse = param1.value != 0.0f;
    break;
  case ID_TEST_VEC:
    result->trueorfalse = 
      param1.vec[0] != 0.0f || 
      param1.vec[1] != 0.0f ||
      param1.vec[2] != 0.0f;
    break;
  case ID_VEC3F:
    result->vec[0] = param1.value;
    result->vec[1] = param2.value;
    result->vec[2] = param3.value;
    break;
  case ID_FLT_REG:
    cbdata->readfieldcb(node->child1->regname, &result->value, fieldidx, -1, cbdata->userdata); 
    break;
  case ID_VEC_REG:
    cbdata->readfieldcb(node->child1->regname, result->vec, fieldidx, -1, cbdata->userdata);
    break;
  case ID_VEC_REG_COMP:
    cbdata->readfieldcb(node->child1->regname, &result->value, fieldidx, node->child1->regidx, cbdata->userdata);
    break;
  case ID_FLT_COND:
    so_eval_traverse(node->child1, &param1, fieldidx, cbdata);
    so_eval_traverse(param1.trueorfalse ? node->child2 : node->child3, &param2, fieldidx, cbdata);
    result->value = param2.value;
    break;
  case ID_VEC_COND:
    so_eval_traverse(node->child1, &param1, fieldidx, cbdata);
    so_eval_traverse(param1.trueorfalse ? node->child2 : node->child3, &param2, fieldidx, cbdata);
    result->vec[0] = param2.vec[0];
    result->vec[1] = param2.vec[1];
    result->vec[2] = param2.vec[2];
    break;
  case ID_VALUE:
    result->value = node->value;
    break;
  case ID_ASSIGN_FLT:
    /* this is safe, since regidx always will be -1 for other than vector components */
    so_eval_traverse(node->child1, &param1, fieldidx, cbdata);
    cbdata->writefieldcb(node->child1->regname, &param1.value, fieldidx, node->child1->regidx, cbdata->userdata);
    break;
  case ID_ASSIGN_VEC:
    so_eval_traverse(node->child1, &param1, fieldidx, cbdata);
    cbdata->writefieldcb(node->child1->regname, param1.vec, fieldidx, -1, cbdata->userdata);
    break;
  case ID_SEPARATOR:
    /* do nothing, both children have been traversed */
    break;
  }
}

/*
 * evaluates the tree structure
 */
void 
so_eval_evaluate(so_eval_node *node, int fieldidx, const so_eval_cbdata *cbdata)
{
  so_eval_param dummy;
  so_eval_traverse(node, &dummy, fieldidx, cbdata);
}


void
so_eval_delete(so_eval_node *node)
{
  if (node != NULL) {
    if (node->child1) so_eval_delete_rec(node->child1);
    if (node->child2) so_eval_delete_rec(node->child2);
    if (node->child3) so_eval_delete_rec(node->child3);
    free(node);
  }
}
