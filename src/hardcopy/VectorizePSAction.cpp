/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoVectorizePSAction HardCopy/SoVectorizePSAction.h
  \brief The SoVectorizePSAction class is used for rendering to Postscript.

*/

#include <HardCopy/SoVectorizePSAction.h>
#include "VectorizeActionP.h"
#include <stdio.h>

/* helper function for ascii85 handling. Will use from Coin tidbits when moved */
static int
encode_ascii85(const unsigned char * in, unsigned char * out)
{
  uint32_t data =
    ((uint32_t)(in[0])<<24) |
    ((uint32_t)(in[1])<<16) |
    ((uint32_t)(in[2])<< 8) |
    ((uint32_t)(in[3]));

  if (data == 0) {
    out[0] = 'z';
    return 1;
  }
  out[4] = (unsigned char) (data%85 + '!');
  data /= 85;
  out[3] = (unsigned char) (data%85 + '!');
  data /= 85;
  out[2] = (unsigned char) (data%85 + '!');
  data /= 85;
  out[1] = (unsigned char) (data%85 + '!');
  data /= 85;
  out[0] = (unsigned char) (data%85 + '!');
  return 5;
}

static void
output_ascii85(FILE * fp,
               const unsigned char val,
               unsigned char * tuple,
               unsigned char * linebuf,
               int * tuplecnt, int * linecnt,
               const int rowlen,
               const SbBool flush)
{
  int i;
  if (flush) {
    // fill up tuple
    for (i = *tuplecnt; i < 4; i++) tuple[i] = 0;
  }
  else {
    tuple[(*tuplecnt)++] = val;
  }
  if (flush || *tuplecnt == 4) {
    if (*tuplecnt) {
      int add = encode_ascii85(tuple, linebuf + *linecnt);
      if (flush) {
        if (add == 1) {
          for (i = 0; i < 5; i++) linebuf[*linecnt + i] = '!';
        }
        *linecnt += *tuplecnt + 1;
      }
      else *linecnt += add;
      *tuplecnt = 0;
    }
    if (*linecnt >= rowlen) {
      unsigned char store = linebuf[rowlen];
      linebuf[rowlen] = 0;
      fprintf(fp, "%s\n", linebuf);
      linebuf[rowlen] = store;
      for (i = rowlen; i < *linecnt; i++) {
        linebuf[i-rowlen] = linebuf[i];
      }
      *linecnt -= rowlen;
    }
    if (flush && *linecnt) {
      linebuf[*linecnt] = 0;
      fprintf(fp, "%s\n", linebuf);
    }
  }
}

static void
flush_ascii85(FILE * fp,
              unsigned char * tuple,
              unsigned char * linebuf,
              int * tuplecnt, int * linecnt,
              const int rowlen)
{
  output_ascii85(fp, 0, tuple, linebuf, tuplecnt, linecnt, rowlen, TRUE);
}


// *************************************************************************

class SoVectorizePSActionP {
public:
  SoVectorizePSActionP(SoVectorizePSAction * p) {
    this->publ = p;
    this->linepattern = 0xffff;
    this->linewidth = 1.0f;

    this->fontname = "";
    this->fontsize = -1.0f;
  }

  double gouraudeps;
  SbString default2dfont;
  uint16_t linepattern;
  float linewidth;

  SbString fontname;
  float fontsize;
private:
  SoVectorizePSAction * publ;
};


#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->publ)

// *************************************************************************

SO_ACTION_SOURCE(SoVectorizePSAction);

// *************************************************************************

// doc in parent
void
SoVectorizePSAction::initClass(void)
{
   SO_ACTION_INIT_CLASS(SoVectorizePSAction, SoVectorizeAction);
}

/*!
  Default constructor.
*/
SoVectorizePSAction::SoVectorizePSAction(void)
{
  PRIVATE(this) = new SoVectorizePSActionP(this);
  PRIVATE(this)->gouraudeps = 0.0f;
  PRIVATE(this)->default2dfont = "Courier";
  SO_ACTION_CONSTRUCTOR(SoVectorizePSAction);

  this->setOutput(new SoPSVectorOutput);
}

/*!
  Default destructor.
*/
SoVectorizePSAction::~SoVectorizePSAction()
{
  delete PRIVATE(this);
}

/*!
  Sets the Gouraud shading threshold. A threshold of 0.0 will disable
  Gouraud shading. Default is 0.0, since we've seen strange artifacts
  for some models when Gouraud shading is enabled..
*/
void
SoVectorizePSAction::setGouraudThreshold(const double eps)
{
  PRIVATE(this)->gouraudeps = eps;
}

// *************************************************************************

/*!
  Sets the default font name. This font will be used for rendering
  Text2-nodes which have no Font-nodes preceding them. The default
  value is "Courier".
*/
void
SoVectorizePSAction::setDefault2DFont(const SbString & fontname)
{
  PRIVATE(this)->default2dfont = fontname;
}

/*!
  Returns the default font name.

  \sa setDefault2DFont()
*/
const SbString &
SoVectorizePSAction::getDefault2DFont(void) const
{
  return PRIVATE(this)->default2dfont;
}

/*!
  Returns the SoPSVectorOutput used by this instance.
*/
SoPSVectorOutput *
SoVectorizePSAction::getOutput(void) const
{
  return (SoPSVectorOutput*)SoVectorizeAction::getOutput();
}

/*!
  Returns the SoPSVectorOutput used by this instance. Provided
  for API compatibility with TGS HardCopy support.
*/
SoPSVectorOutput *
SoVectorizePSAction::getPSOutput(void) const
{
  return (SoPSVectorOutput*)SoVectorizeAction::getOutput();
}

static char * gouraudtriangle[] = {
  "% the gouraudtriangle PostScript fragement below is free",
  "% written by Frederic Delhoume (delhoume@ilog.fr)",
  "/bd{bind def}bind def /triangle { aload pop   setrgbcolor  aload pop 5 3",
  "roll 4 2 roll 3 2 roll exch moveto lineto lineto closepath fill } bd",
  "/computediff1 { 2 copy sub abs threshold ge {pop pop pop true} { exch 2",
  "index sub abs threshold ge { pop pop true} { sub abs threshold ge } ifelse",
  "} ifelse } bd /computediff3 { 3 copy 0 get 3 1 roll 0 get 3 1 roll 0 get",
  "computediff1 {true} { 3 copy 1 get 3 1 roll 1 get 3 1 roll 1 get",
  "computediff1 {true} { 3 copy 2 get 3 1 roll  2 get 3 1 roll 2 get",
  "computediff1 } ifelse } ifelse } bd /middlecolor { aload pop 4 -1 roll",
  "aload pop 4 -1 roll add 2 div 5 1 roll 3 -1 roll add 2 div 3 1 roll add 2",
  "div 3 1 roll exch 3 array astore } bd /gouraudtriangle { computediff3 { 4",
  "-1 roll aload 7 1 roll 6 -1 roll pop 3 -1 roll pop add 2 div 3 1 roll add",
  "2 div exch 3 -1 roll aload 7 1 roll exch pop 4 -1 roll pop add 2 div 3 1",
  "roll add 2 div exch 3 -1 roll aload 7 1 roll pop 3 -1 roll pop add 2 div 3",
  "1 roll add 2 div exch 7 3 roll 10 -3 roll dup 3 index middlecolor 4 1 roll",
  "2 copy middlecolor 4 1 roll 3 copy pop middlecolor 4 1 roll 13 -1 roll",
  "aload pop 17 index 6 index 15 index 19 index 6 index 17 index 6 array",
  "astore 10 index 10 index 14 index gouraudtriangle 17 index 5 index 17",
  "index 19 index 5 index 19 index 6 array astore 10 index 9 index 13 index",
  "gouraudtriangle 13 index 16 index 5 index 15 index 18 index 5 index 6",
  "array astore 12 index 12 index 9 index gouraudtriangle 17 index 16 index",
  "15 index 19 index 18 index 17 index 6 array astore 10 index 12 index 14",
  "index gouraudtriangle 18 {pop} repeat } { aload pop 5 3 roll aload pop 7 3",
  "roll aload pop 9 3 roll 4 index 6 index 4 index add add 3 div 10 1 roll 7",
  "index 5 index 3 index add add 3 div 10 1 roll 6 index 4 index 2 index add",
  "add 3 div 10 1 roll 9 {pop} repeat 3 array astore triangle } ifelse } bd",
  NULL
};

static char * flatshadetriangle[] = {
  "% flatshade a triangle",
  "/flatshadetriangle",
  "{ newpath moveto",
  "lineto",
  "lineto",
  "closepath",
  "setrgbcolor",
  "fill } def",
  NULL
};

static char * rightshow[] = {
  "% print a right justified string",
  "/rightshow",
  "{ dup stringwidth pop",
  "neg",
  "0 rmoveto",
  "show } def",
  NULL
};

static char * centershow[] = {
  "% print a center justified string",
  "/centershow",
  "{ dup stringwidth pop",
  "0.5 mul",
  "neg",
  "0 rmoveto",
  "show } def",
  NULL
};

static void print_array(FILE * fp, char ** array)
{
  for (int i = 0; array[i]; i++) {
    fputs(array[i], fp);
    fputs("\n", fp);
  }
  fputs("\n", fp);
}

// doc in parent
void
SoVectorizePSAction::printHeader(void) const
{
  FILE * file = this->getOutput()->getFilePointer();

  float viewport[4];

  viewport[0] = this->convertToPS(this->getPageStartpos())[0];
  viewport[1] = this->convertToPS(this->getPageStartpos())[1];
  viewport[2] = this->convertToPS(this->getPageSize())[0];
  viewport[3] = this->convertToPS(this->getPageSize())[1];

  fputs("%!PS-Adobe-2.0 EPSF-2.0\n", file);
  fprintf(file, "%%%%Creator: Coin 2.0\n");
  fprintf(file, "%%%%BoundingBox: %g %g %g %g\n",
          viewport[0], viewport[1], viewport[2], viewport[3]);
  fputs("%%EndComments\n", file);
  fputs("\n", file);
  fputs("gsave\n", file);
  fputs("\n", file);

  fprintf(file, "/threshold %g def %% used by gouraudtriangle\n", PRIVATE(this)->gouraudeps);

  print_array(file, gouraudtriangle);
  print_array(file, flatshadetriangle);
  print_array(file, rightshow);
  print_array(file, centershow);

  if (this->getOrientation() == LANDSCAPE) {
    SbVec2f psize = this->convertToPS(this->getPageSize());
    SbVec2f porg = this->convertToPS(this->getPageStartpos());
    psize *= 0.5f;
    fputs("% rotate to LANDSCAPE orientation\n", file);
    fprintf(file, "%g %g translate\n", porg[0] + psize[0], porg[1] + psize[1]);
    fprintf(file, "90 rotate\n");
    fprintf(file, "%g %g translate\n\n", -(psize[1]+porg[1]), -(psize[0]+porg[1]));
  }
}

// doc in parent
void
SoVectorizePSAction::printFooter(void) const
{
  FILE * file = this->getOutput()->getFilePointer();

  fputs("\ngrestore\n", file);
  fputs("showpage\n", file);
}

// doc in parent
void
SoVectorizePSAction::printViewport(void) const
{
  FILE * file = this->getOutput()->getFilePointer();

  float viewport[4];
  viewport[0] = this->convertToPS(this->getRotatedViewportStartpos())[0];
  viewport[1] = this->convertToPS(this->getRotatedViewportStartpos())[1];
  viewport[2] = this->convertToPS(this->getRotatedViewportSize())[0] + viewport[0];
  viewport[3] = this->convertToPS(this->getRotatedViewportSize())[1] + viewport[1];

  fputs("% set up clipping for viewport\n", file);
  fprintf(file, "newpath\n");
  fprintf(file, "%g %g moveto\n", viewport[0], viewport[1]);
  fprintf(file, "%g %g lineto\n", viewport[0], viewport[3]);
  fprintf(file, "%g %g lineto\n", viewport[2], viewport[3]);
  fprintf(file, "%g %g lineto\n", viewport[2], viewport[1]);
  fprintf(file, "closepath clip\n\n");
}

// doc in parent
void
SoVectorizePSAction::printBackground(void) const
{
  FILE * file = this->getOutput()->getFilePointer();

  float viewport[4];
  SbColor bgcol;

  viewport[0] = this->convertToPS(this->getRotatedViewportStartpos())[0];
  viewport[1] = this->convertToPS(this->getRotatedViewportStartpos())[1];
  viewport[2] = this->convertToPS(this->getRotatedViewportSize())[0];
  viewport[3] = this->convertToPS(this->getRotatedViewportSize())[1];

  (void) this->getBackgroundColor(bgcol);

  fputs("% clear background\n", file);
  fprintf(file, "newpath\n");
  fprintf(file, "%g %g moveto\n", viewport[0], viewport[1]);
  fprintf(file, "0 %g rlineto\n", viewport[3]);
  fprintf(file, "%g 0 rlineto\n", viewport[2]);
  fprintf(file, "0 %g neg rlineto\n", viewport[3]);
  fprintf(file, "closepath\n");
  fprintf(file, "%g %g %g setrgbcolor\n",
          bgcol[0], bgcol[1], bgcol[2]);
  fprintf(file, "fill\n");
}

// doc in parent
void
SoVectorizePSAction::printItem(const SoVectorizeItem * item) const
{
  switch (item->type) {
  case SoVectorizeItem::TRIANGLE:
    this->printTriangle((SoVectorizeTriangle*)item);
    break;
  case SoVectorizeItem::LINE:
    this->printLine((SoVectorizeLine*)item);
    break;
  case SoVectorizeItem::POINT:
    this->printPoint((SoVectorizePoint*)item);
    break;
  case SoVectorizeItem::TEXT:
    this->printText((SoVectorizeText*)item);
    break;
  case SoVectorizeItem::IMAGE:
    this->printImage((SoVectorizeImage*)item);
    break;
  default:
    assert(0 && "unsupported item");
    break;
  }
}

static int count_bits(uint16_t mask, int & pos, SbBool onoff)
{
  int cnt = -1;
  pos++;
  SbBool res;
  do {
    cnt++;
    pos--;
    if (pos < 0) break;
    uint16_t bit = 1 << pos;
    res = (bit & mask) ? TRUE : FALSE;
  } while (res == onoff);
  return cnt;
}

//
// Set up line stipple.
//
void
SoVectorizePSAction::printSetdash(uint16_t pattern) const
{
  FILE * file = this->getOutput()->getFilePointer();
  fputs("[", file);

  int pos = 15;
  SbBool onoff = TRUE;
  while (pos >= 0) {
    int cnt = count_bits(pattern, pos, onoff);
    fprintf(file," %d", cnt);
    onoff = !onoff;
  }
  if (!onoff) { // need pairs of values
    fputs(" 0] 0 setdash\n", file);
  }
  else {
    fputs("] 0 setdash\n", file);
  }
}

//
// make sure we have the correct font
//
void
SoVectorizePSAction::updateFont(const SbString & fontname, const float fontsize) const
{
  FILE * file = this->getOutput()->getFilePointer();

  if (fontname != PRIVATE(this)->fontname ||
      fontsize != PRIVATE(this)->fontsize) {
    fprintf(file, "/%s findfont\n", fontname.getString());
    fprintf(file, "%g scalefont\n", fontsize);
    fprintf(file, "setfont\n");

    PRIVATE(this)->fontname = fontname;
    PRIVATE(this)->fontsize = fontsize;
  }
}

//
// Make sure line width and line stipple is correct.
//
void
SoVectorizePSAction::updateLineAttribs(const SoVectorizeLine * line) const
{
  FILE * file = this->getOutput()->getFilePointer();

  float lw = line->width;
  uint16_t lp = line->pattern;

  if (lw != PRIVATE(this)->linewidth) {
    PRIVATE(this)->linewidth = lw;
    fprintf(file, "%g setlinewidth\n", this->convertToPS(lw * this->getNominalWidth()));
  }

  if (lp != PRIVATE(this)->linepattern) {
    PRIVATE(this)->linepattern = lp;
    if (lp == 0xffff) {
      fprintf(file, "[] 0 setdash\n");
    }
    else {
      this->printSetdash(lp);
    }
  }
}

//
// will output a line in postscript format
//
void
SoVectorizePSAction::printLine(const SoVectorizeLine * item) const
{
  FILE * file = this->getOutput()->getFilePointer();

  SbVec2f mul = this->convertToPS(this->getRotatedViewportSize());
  SbVec2f add = this->convertToPS(this->getRotatedViewportStartpos());

  int i;
  const SbBSPTree & bsp = this->getBSPTree();

  SbVec3f v[2];
  SbColor c[2];
  float t[2];

  for (i = 0; i < 2; i++) {
    v[i] = bsp.getPoint(item->vidx[i]);
    v[i][0] = (v[i][0] * mul[0]) + add[0];
    v[i][1] = (v[i][1] * mul[1]) + add[1];
    c[i].setPackedValue(item->col[i], t[i]);
  }

  this->updateLineAttribs(item);
  fprintf(file, "%g %g %g setrgbcolor\n", c[0][0], c[0][1], c[0][2]);
  fprintf(file, "newpath\n");
  fprintf(file, "%g %g moveto\n", v[0][0], v[0][1]);
  fprintf(file, "%g %g lineto\n", v[1][0], v[1][1]);
  fprintf(file,"stroke\n\n");
}


void
SoVectorizePSAction::printCircle(const SbVec3f & v, const SbColor & c, const float radius) const
{
  FILE * file = this->getOutput()->getFilePointer();

  fprintf(file, "newpath %g %g %g 0 360 arc closepath\n", v[0], v[1], radius);
  fprintf(file, "%g %g %g setrgbcolor\n", c[0], c[1], c[2]);
  fprintf(file, "fill\n\n");
}

//
// will output a point in postscript format
//
void
SoVectorizePSAction::printPoint(const SoVectorizePoint * item) const
{
  FILE * file = this->getOutput()->getFilePointer();

  SbVec2f mul = this->convertToPS(this->getRotatedViewportSize());
  SbVec2f add = this->convertToPS(this->getRotatedViewportStartpos());

  const SbBSPTree & bsp = this->getBSPTree();

  SbVec3f v;
  SbColor c;
  float t;

  v = bsp.getPoint(item->vidx);
  v[0] = (v[0] * mul[0]) + add[0];
  v[1] = (v[1] * mul[1]) + add[1];
  c.setPackedValue(item->col, t);

  this->printCircle(v, c, this->convertToPS(item->size * this->getNominalWidth()));
}

void
SoVectorizePSAction::printTriangle(const SbVec3f * v, const SbColor * c) const
{
  FILE * file = this->getOutput()->getFilePointer();

  if ((PRIVATE(this)->gouraudeps == 0.0f) ||
      ((c[0] == c[1]) && (c[1] == c[2]))) {
    // flatshaded
    fprintf(file, "%g %g %g %g %g %g %g %g %g flatshadetriangle\n",
            c[0][0], c[0][1], c[0][2],
            v[2][0], v[2][1],
            v[1][0], v[1][1],
            v[0][0], v[0][1]);
  }
  else {
    // gouraud
    fprintf(file, "[%g %g %g %g %g %g]",
            v[0][0], v[1][0], v[2][0],
            v[0][1], v[1][1], v[2][1]);
    fprintf(file, " [%g %g %g] [%g %g %g] [%g %g %g] gouraudtriangle\n",
            c[0][0], c[0][1], c[0][2],
            c[1][0], c[1][1], c[1][2],
            c[2][0], c[2][1], c[2][2]);
  }
}

//
// will output a triangle in postscript format
//
void
SoVectorizePSAction::printTriangle(const SoVectorizeTriangle * item) const
{
  FILE * file = this->getOutput()->getFilePointer();

  SbVec2f mul = this->convertToPS(this->getRotatedViewportSize());
  SbVec2f add = this->convertToPS(this->getRotatedViewportStartpos());

  int i;
  const SbBSPTree & bsp = this->getBSPTree();

  SbVec3f v[3];
  SbColor c[3];
  float t[3];

  for (i = 0; i < 3; i++) {
    v[i] = bsp.getPoint(item->vidx[i]);
    v[i][0] = (v[i][0] * mul[0]) + add[0];
    v[i][1] = (v[i][1] * mul[1]) + add[1];

    c[i].setPackedValue(item->col[i], t[i]);
  }
  this->printTriangle((SbVec3f*)v, (SbColor*)c);
}

//
// will output an image in postscript format
//
void
SoVectorizePSAction::printImage(const SoVectorizeImage * item) const
{
  FILE * fp = this->getOutput()->getFilePointer();
  SbVec2f mul = this->convertToPS(this->getRotatedViewportSize());
  SbVec2f add = this->convertToPS(this->getRotatedViewportStartpos());

  fprintf(fp, "gsave\n");
  fprintf(fp, "%% workaround for bug in some PS interpreters\n");
  fprintf(fp, "%% which doesn't skip the ASCII85 EOD marker.\n");
  fprintf(fp, "/~ {currentfile read pop pop} def\n\n");

  SbVec2s size = item->image.size;
  int nc = item->image.nc;
  const unsigned char * src = item->image.data;

  //  fprintf(fp, "0 0 moveto\n");
  fprintf(fp, "%g %g translate\n",
          item->pos[0]*mul[0] + add[0],
          item->pos[1]*mul[1] + add[1]);

  fprintf(fp, "/pix %d string def\n", size[0] * (nc >= 3 ? 3 : 1));

  fprintf(fp, "%g %g scale\n",
          item->size[0] * mul[0],
          item->size[1] * mul[1]);

  fprintf(fp, "%d %d 8 [%d 0 0 %d 0 0] currentfile\n",
          (int)item->image.size[0], (int)item->image.size[1],
          (int)item->image.size[0], (int)item->image.size[1]);

  fprintf(fp, "/ASCII85Decode filter\n");
  if (item->image.nc >= 3) fprintf(fp, "false 3\ncolorimage\n");
  else fprintf(fp,"image\n");

  const int rowlen = 72;
  int num = size[0] * size[1];
  unsigned char tuple[4];
  unsigned char linebuf[rowlen+5];
  int tuplecnt = 0;
  int linecnt = 0;
  int cnt = 0;

  while (cnt < num) {
    switch (nc) {
    default: // avoid warning
    case 1:
      output_ascii85(fp, src[cnt], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
      break;
    case 2:
      output_ascii85(fp, src[cnt*2], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
      break;
    case 3:
      output_ascii85(fp, src[cnt*3], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
      output_ascii85(fp, src[cnt*3+1], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
      output_ascii85(fp, src[cnt*3+2], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
      break;
    case 4:
      output_ascii85(fp, src[cnt*4], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
      output_ascii85(fp, src[cnt*4+1], tuple, linebuf, &tuplecnt, &linecnt,rowlen, FALSE);
      output_ascii85(fp, src[cnt*4+2], tuple, linebuf, &tuplecnt, &linecnt, rowlen, FALSE);
      break;
    }
    cnt++;
  }

  // flush data in ascii85 encoder
  flush_ascii85(fp, tuple, linebuf, &tuplecnt, &linecnt, rowlen);

  fprintf(fp, "~>\n\n"); // ASCII85 EOD marker
  fprintf(fp, "grestore\n");
}

//
// will output text in postscript format
//
void
SoVectorizePSAction::printText(const SoVectorizeText * item) const
{
  FILE * file = this->getOutput()->getFilePointer();

  SbVec2f mul = this->convertToPS(this->getRotatedViewportSize());
  SbVec2f add = this->convertToPS(this->getRotatedViewportStartpos());

  SbString fontname = item->fontname.getString();
  if (fontname == "defaultFont") {
    fontname = PRIVATE(this)->default2dfont;
  }

  SbColor c;
  float dummy;
  c.setPackedValue(item->col, dummy);

  this->updateFont(fontname, item->fontsize * mul[1]);

  fprintf(file, "%g %g %g setrgbcolor\n",
          c[0], c[1], c[2]);
  fprintf(file, "%g %g moveto\n",
          item->pos[0]*mul[0] + add[0],
          item->pos[1]*mul[1] + add[1]);

  SbString op;
  switch (item->justification) {
  default:
  case SoVectorizeText::LEFT:
    op = "show";
    break;
  case SoVectorizeText::CENTER:
    op = "centershow";
    break;
  case SoVectorizeText::RIGHT:
    op = "rightshow";
    break;
  }

  fprintf(file, "(%s) %s\n\n", item->string.getString(), op.getString());
}

// a standard PS unit is 1/72 inch
SbVec2f
SoVectorizePSAction::convertToPS(const SbVec2f & mm) const
{
  return from_mm(mm, INCH) * 72.0f;
}

// a standard PS unit is 1/72 inch
float
SoVectorizePSAction::convertToPS(const float mm) const
{
  return from_mm(mm, INCH) * 72.0f;
}

// *************************************************************************
