#include <Inventor/elements/SoGLShaderProgramElement.h>
#include <assert.h>

SO_ELEMENT_SOURCE(SoGLShaderProgramElement);

void SoGLShaderProgramElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLShaderProgramElement, inherited);
}

SoGLShaderProgramElement::~SoGLShaderProgramElement()
{
  this->shaderProgram = NULL;
}

void SoGLShaderProgramElement::init(SoState *state)
{
  inherited::init(state);
  this->shaderProgram = NULL;
}

void SoGLShaderProgramElement::set(SoState* const state, SoNode *const node,
				 SoGLShaderProgram* program)
{
  SoGLShaderProgramElement* element = 
    (SoGLShaderProgramElement*)inherited::getElement(state,classStackIndex,node);
  element->shaderProgram = program;
}
SoGLShaderProgram* SoGLShaderProgramElement::get(SoState *state)
{
  const SoElement *element = getConstElement(state, classStackIndex);  
  assert(element);
  return ((const SoGLShaderProgramElement *)element)->shaderProgram;
}

void SoGLShaderProgramElement::push(SoState *state)
{
  inherited::push(state);

  SoGLShaderProgramElement *last=(SoGLShaderProgramElement *)getNextInStack();
  assert(last);
  if (last->shaderProgram) last->shaderProgram->disable();
}
  
void SoGLShaderProgramElement::pop(SoState *state, const SoElement *prev)
{
  SoGLShaderProgramElement *elem = (SoGLShaderProgramElement *)prev;
  if (elem->shaderProgram) elem->shaderProgram->disable();
  
  if (this->shaderProgram) this->shaderProgram->enable();

  inherited::pop(state, prev);
}
