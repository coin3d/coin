/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef __SONODE_H__
#define __SONODE_H__

#include <Inventor/fields/SoFieldContainer.h>

#if defined(COIN_EXCLUDE_SONODE)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SONODE

// Remove all these dependencies in SoNode when compiling a minimal
// version of the Coin library. This is for debugging purposes only,
// see the Coin/build/base.pro file. 19990611 mortene.

#if defined(__SOLIB_MAKEASBASE__)
// Trigger setting of action dependencies.
#include <Inventor/actions/SoAction.h>

// Remove all nodes, except the abstract SoNode base class and the
// ones commented out below.
#define COIN_EXCLUDE_SOANNOTATION 1
#define COIN_EXCLUDE_SOANNOTEXT3 1
#define COIN_EXCLUDE_SOANNOTEXT3PROPERTY 1
#define COIN_EXCLUDE_SOANTISQUISH 1
#define COIN_EXCLUDE_SOARRAY 1
#define COIN_EXCLUDE_SOASCIITEXT 1
#define COIN_EXCLUDE_SOBASECOLOR 1
#define COIN_EXCLUDE_SOBLINKER 1
#define COIN_EXCLUDE_SOCALLBACK 1
#define COIN_EXCLUDE_SOCAMERA 1
#define COIN_EXCLUDE_SOCLIPPLANE 1
#define COIN_EXCLUDE_SOCOLORINDEX 1
#define COIN_EXCLUDE_SOCOMPLEXITY 1
#define COIN_EXCLUDE_SOCONE 1
#define COIN_EXCLUDE_SOCOORDINATE3 1
#define COIN_EXCLUDE_SOCOORDINATE4 1
#define COIN_EXCLUDE_SOCUBE 1
#define COIN_EXCLUDE_SOCYLINDER 1
#define COIN_EXCLUDE_SODIRECTIONALLIGHT 1
#define COIN_EXCLUDE_SODRAWSTYLE 1
#define COIN_EXCLUDE_SOENVIRONMENT 1
#define COIN_EXCLUDE_SOEVENTCALLBACK 1
#define COIN_EXCLUDE_SOEXTSELECTION 1
#define COIN_EXCLUDE_SOFACESET 1
#define COIN_EXCLUDE_SOFILE 1
#define COIN_EXCLUDE_SOFONT 1
#define COIN_EXCLUDE_SOFONTSTYLE 1
//  #define COIN_EXCLUDE_SOGROUP 1
#define COIN_EXCLUDE_SOIMAGE 1
#define COIN_EXCLUDE_SOINDEXEDFACESET 1
#define COIN_EXCLUDE_SOINDEXEDLINESET 1
#define COIN_EXCLUDE_SOINDEXEDNURBSCURVE 1
#define COIN_EXCLUDE_SOINDEXEDNURBSSURFACE 1
#define COIN_EXCLUDE_SOINDEXEDSHAPE 1
#define COIN_EXCLUDE_SOINDEXEDTRIANGLESTRIPSET 1
//  #define COIN_EXCLUDE_SOINFO 1
#define COIN_EXCLUDE_SOLABEL 1
#define COIN_EXCLUDE_SOLEVELOFDETAIL 1
#define COIN_EXCLUDE_SOLEVELOFSIMPLIFICATION 1
#define COIN_EXCLUDE_SOLIGHT 1
#define COIN_EXCLUDE_SOLIGHTMODEL 1
#define COIN_EXCLUDE_SOLINEARPROFILE 1
#define COIN_EXCLUDE_SOLINESET 1
#define COIN_EXCLUDE_SOLOCATEHIGHLIGHT 1
#define COIN_EXCLUDE_SOLOD 1
#define COIN_EXCLUDE_SOMARKERSET 1
#define COIN_EXCLUDE_SOMATERIAL 1
#define COIN_EXCLUDE_SOMATERIALBINDING 1
#define COIN_EXCLUDE_SOMATRIXTRANSFORM 1
#define COIN_EXCLUDE_SOMULTIPLECOPY 1
#define COIN_EXCLUDE_SONONINDEXEDSHAPE 1
#define COIN_EXCLUDE_SONORMAL 1
#define COIN_EXCLUDE_SONORMALBINDING 1
#define COIN_EXCLUDE_SONURBSCURVE 1
#define COIN_EXCLUDE_SONURBSPROFILE 1
#define COIN_EXCLUDE_SONURBSSURFACE 1
#define COIN_EXCLUDE_SOORTHOGRAPHICCAMERA 1
#define COIN_EXCLUDE_SOPACKEDCOLOR 1
#define COIN_EXCLUDE_SOPATHSWITCH 1
#define COIN_EXCLUDE_SOPENDULUM 1
#define COIN_EXCLUDE_SOPERSPECTIVECAMERA 1
#define COIN_EXCLUDE_SOPICKSTYLE 1
#define COIN_EXCLUDE_SOPOINTLIGHT 1
#define COIN_EXCLUDE_SOPOINTSET 1
#define COIN_EXCLUDE_SOPOLYGONOFFSET 1
#define COIN_EXCLUDE_SOPROFILE 1
#define COIN_EXCLUDE_SOPROFILECOORDINATE2 1
#define COIN_EXCLUDE_SOPROFILECOORDINATE3 1
#define COIN_EXCLUDE_SOQUADMESH 1
#define COIN_EXCLUDE_SORESETTRANSFORM 1
#define COIN_EXCLUDE_SOROTATION 1
#define COIN_EXCLUDE_SOROTATIONXYZ 1
#define COIN_EXCLUDE_SOROTOR 1
#define COIN_EXCLUDE_SOSCALE 1
#define COIN_EXCLUDE_SOSELECTION 1
#define COIN_EXCLUDE_SOSEPARATOR 1
#define COIN_EXCLUDE_SOSHAPE 1
#define COIN_EXCLUDE_SOSHAPEHINTS 1
#define COIN_EXCLUDE_SOSHUTTLE 1
#define COIN_EXCLUDE_SOSPHERE 1
#define COIN_EXCLUDE_SOSPOTLIGHT 1
#define COIN_EXCLUDE_SOSURROUNDSCALE 1
#define COIN_EXCLUDE_SOSWITCH 1
#define COIN_EXCLUDE_SOTEXT2 1
#define COIN_EXCLUDE_SOTEXT3 1
#define COIN_EXCLUDE_SOTEXTURE2 1
#define COIN_EXCLUDE_SOTEXTURE2TRANSFORM 1
#define COIN_EXCLUDE_SOTEXTURECOORDINATE2 1
#define COIN_EXCLUDE_SOTEXTURECOORDINATEBINDING 1
#define COIN_EXCLUDE_SOTEXTURECOORDINATEDEFAULT 1
#define COIN_EXCLUDE_SOTEXTURECOORDINATEENVIRONMENT 1
#define COIN_EXCLUDE_SOTEXTURECOORDINATEFUNCTION 1
#define COIN_EXCLUDE_SOTEXTURECOORDINATEPLANE 1
#define COIN_EXCLUDE_SOTRANSFORM 1
#define COIN_EXCLUDE_SOTRANSFORMATION 1
#define COIN_EXCLUDE_SOTRANSFORMSEPARATOR 1
#define COIN_EXCLUDE_SOTRANSLATION 1
#define COIN_EXCLUDE_SOTRIANGLESTRIPSET 1
#define COIN_EXCLUDE_SOUNITS 1
#define COIN_EXCLUDE_SOVERTEXPROPERTY 1
#define COIN_EXCLUDE_SOVERTEXSHAPE 1
#define COIN_EXCLUDE_SOWWWANCHOR 1
#define COIN_EXCLUDE_SOWWWINLINE 1
#endif // !__SOLIB_MAKEASBASE__

class SoAction;
class SoCallbackAction;
class SoChildList;
class SoGLRenderAction;
class SoGetBoundingBoxAction;
class SoGetMatrixAction;
class SoGetPrimitiveCountAction;
class SoHandleEventAction;
class SoNodeList;
class SoNotList;
class SoOutput;
class SoPickAction;
class SoRayPickAction;
class SoSearchAction;
class SoWriteAction;


class SoNode : public SoFieldContainer {
  typedef SoFieldContainer inherited;

public:
  enum Stage { FIRST_INSTANCE,
	       PROTO_INSTANCE,
	       OTHER_INSTANCE
  };

  void setOverride(const SbBool state);
  SbBool isOverride(void) const;

  SoNode * copy(SbBool copyConnections = FALSE) const;
  virtual SbBool affectsState(void) const;

#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void doAction(SoAction * action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
  virtual void GLRenderBelowPath(SoGLRenderAction * action);
  virtual void GLRenderInPath(SoGLRenderAction * action);
  virtual void GLRenderOffPath(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
  virtual void getMatrix(SoGetMatrixAction * action);
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  virtual void handleEvent(SoHandleEventAction * action);
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  virtual void pick(SoPickAction * action);
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  virtual void rayPick(SoRayPickAction * action);
#endif // !COIN_EXCLUDE_SORAYPICKACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
  virtual void search(SoSearchAction * action);
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SOWRITEACTION)
  virtual void write(SoWriteAction * action);
#endif // !COIN_EXCLUDE_SOWRITEACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

  virtual void grabEventsSetup(void);
  virtual void grabEventsCleanup(void);

  virtual void startNotify(void);
  virtual void notify(SoNotList * list);

  uint32_t getNodeId(void) const;
  virtual SoChildList * getChildren(void) const;
  
  virtual void writeInstance(SoOutput * out);
  virtual SoNode * addToCopyDict(void) const;
  virtual void copyContents(const SoFieldContainer * fromFC,
			    SbBool copyConnections);
  virtual SoFieldContainer * copyThroughConnection(void) const;


  static SoType getClassTypeId(void);
  static SoNode * getByName(const SbName & name);
  static int getByName(const SbName & name, SoNodeList & list);
  
  static void initClass(void);
  static void initClasses(void);

  static uint32_t getNextNodeId(void);
  static int getActionMethodIndex(const SoType type);

#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  static void getBoundingBoxS(SoAction * const action,SoNode * const node);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  static void GLRenderS(SoAction * const action, SoNode * const node);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  static void callbackS(SoAction * const action, SoNode * const node);
#endif // COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
  static void getMatrixS(SoAction * const action, SoNode * const node);
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION
#if !defined(COIN_EXCLUDE_SOHANDLEEVENTACTION)
  static void handleEventS(SoAction * const action, SoNode * const node);
#endif // !COIN_EXCLUDE_SOHANDLEEVENTACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  static void pickS(SoAction * const action, SoNode * const node);
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SORAYPICKACTION)
  static void rayPickS(SoAction * const action, SoNode * const node);
#endif // !COIN_EXCLUDE_SORAYPICKACTION
#if !defined(COIN_EXCLUDE_SOSEARCHACTION)
  static void searchS(SoAction * const action, SoNode * const node);
#endif // !COIN_EXCLUDE_SOSEARCHACTION
#if !defined(COIN_EXCLUDE_SOWRITEACTION)
  static void writeS(SoAction * const action, SoNode * const node);
#endif // !COIN_EXCLUDE_SOWRITEACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  static void getPrimitiveCountS(SoAction * const action, SoNode * const node);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION

protected:
  SoNode(void);
  virtual ~SoNode();

  static const SoFieldData ** getFieldDataPtr(void);

  uint32_t uniqueId;
  static uint32_t nextUniqueId;
  static int nextActionMethodIndex;

private:
  Stage stage;
  static SoType classTypeId;

  struct {
    unsigned int override : 1;
  } stateflags;
};

#endif // !__SONODE_H__
