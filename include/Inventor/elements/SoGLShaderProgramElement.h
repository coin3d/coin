/* ***-*-c++-*-***************************************************************
 * 
 * ***************************************************************************/

#ifndef  _SO_SHADER_PROGRAM_ELEMENT_
#define  _SO_SHADER_PROGRAM_ELEMENT_

#include <Inventor/elements/SoReplacedElement.h>

#include <Inventor/nodes/SoGLShaderProgram.h>

class SoGLShaderProgramElement : public SoReplacedElement {
typedef SoReplacedElement inherited;
  
  SO_ELEMENT_HEADER(SoGLShaderProgramElement);
  
public:
  // Initializes element.
  virtual void init(SoState *state);
  
  // Sets the current shader in the state.
  static void set(SoState *const state, SoNode *const node, 
		  SoGLShaderProgram *program);
  static SoGLShaderProgram* get(SoState *state);
 
  // handle push()/pop()
  virtual void push(SoState *);
  virtual void pop(SoState *state, const SoElement *prevTopElement);
  
SoINTERNAL public:
  // Initializes the classes.
  static void initClass();  
  SoGLShaderProgram *shaderProgram;

private:
  // Destructor
  virtual ~SoGLShaderProgramElement();
};
#endif /* _SO_SHADER_PROGRAM_ELEMENT_ */
