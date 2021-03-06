/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       PlantGL: The Plant Graphic Library
 *
 *       Copyright 1995-2007 UMR CIRAD/INRIA/INRA DAP 
 *
 *       File author(s): F. Boudon et al.
 *
 *  ----------------------------------------------------------------------------
 *
 *                      GNU General Public Licence
 *
 *       This program is free software; you can redistribute it and/or
 *       modify it under the terms of the GNU General Public License as
 *       published by the Free Software Foundation; either version 2 of
 *       the License, or (at your option) any later version.
 *
 *       This program is distributed in the hope that it will be useful,
 *       but WITHOUT ANY WARRANTY; without even the implied warranty of
 *       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
 *       GNU General Public License for more details.
 *
 *       You should have received a copy of the GNU General Public
 *       License along with this program; see the file COPYING. If not,
 *       write to the Free Software Foundation, Inc., 59
 *       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  ----------------------------------------------------------------------------
 */



#include "curve.h"
#include <plantgl/math/util_vector.h>
#include "../function/function.h"

PGL_USING_NAMESPACE
TOOLS_USING_NAMESPACE


/* ----------------------------------------------------------------------- */

/*
Curve::Builder::Builder() :
  ParametricModel::Builder() {
}


Curve::Builder::~Builder() {
  // nothing to do
}
*/

/* ----------------------------------------------------------------------- */
/*

Curve::Curve( ) :
  ParametricModel() {
}

*/
/* ----------------------------------------------------------------------- */

const uchar_t Curve2D::DEFAULT_WIDTH = 1;

/* ----------------------------------------------------------------------- */


Curve2D::Builder::Builder() :
    PlanarModel::Builder(),
    Width(0)  {
}


Curve2D::Builder::~Builder() {
  // nothing to do
}

void Curve2D::Builder::cdestroy( ) {
  if (Width) delete Width;
}

/* ----------------------------------------------------------------------- */


Curve2D::Curve2D(uchar_t width) :
    PlanarModel (), __width(width) {
}

Curve2D::~Curve2D( ) {
}

bool Curve2D::isACurve( ) const {
  return true;
}

bool Curve2D::isASurface( ) const {
  return false;
} 

bool Curve2D::isAVolume( ) const {
  return false;
} 

real_t 
Curve2D::getLength(real_t begin, real_t end) const
{
  real_t fk = getFirstKnot();
  real_t lk = getLastKnot();

  if (begin < getFirstKnot()) begin = fk;
  if (end > getLastKnot()) end = lk;

  real_t deltau = (lk - fk)/getStride();
  // We use the same u sequence to compute the length
  // For this, we compute the closer smaller u value from begin and end
  real_t beginI = int((begin-fk)/deltau) * deltau + fk;
  real_t endI = int((end-fk)/deltau) * deltau + fk;

  Vector2 p1 = getPointAt(beginI);
  Vector2 p2;

  real_t length = 0; 

  // Eventually we do some adjustement according to the real begin and end values 
  // here and just after the loop
  if (begin-beginI > GEOM_EPSILON){
    p2 = getPointAt(begin);
    length -= norm(p2 - p1);
  }

  for(real_t u = beginI + fk + deltau ; u <= endI ; u += deltau){
    p2 = getPointAt(u);
    length += norm(p2 - p1);
    p1 = p2;
  }

  if (end-endI > GEOM_EPSILON){
    p2 = getPointAt(end);
    length += norm(p2 - p1);
  }

  return length;
}


/* ----------------------------------------------------------------------- */


QuantisedFunctionPtr Curve2D::getArcLengthToUMapping() const
{
  real_t totlength = getLength();

  real_t fk = getFirstKnot();
  real_t lk = getLastKnot();
  uint_t stride = getStride();

  real_t deltau = (lk - fk)/stride;

  Vector2 p1 = getPointAt(fk);
  Vector2 p2;

  real_t length = 0; 
  real_t n = 0;

  Point2ArrayPtr points(new Point2Array(stride+1));
  points->setAt(0,Vector2(0,fk));
  real_t u = fk + deltau;
  uint_t j = 1;
  for(uint_t i = 1 ; i <= stride; ++i, u += deltau){
    p2 = getPointAt(u);
    n = norm(p2 - p1);
    if (n > 0){
        length += n;
        p1 = p2;
        points->setAt(j,Vector2(length/totlength,u));
        ++j;
    }
  }
  points->setAt(j-1,Vector2(1.0,lk));
  if (j != stride+1){
      points = Point2ArrayPtr(new Point2Array(points->begin(),points->begin()+j));
  }
  return QuantisedFunctionPtr(new QuantisedFunction(points,5*stride));
}

QuantisedFunctionPtr Curve2D::getUToArcLengthMapping() const
{
  real_t totlength = getLength();

  real_t fk = getFirstKnot();
  real_t lk = getLastKnot();
  uint_t stride = getStride();

  real_t deltau = (lk - fk)/stride;

  Vector2 p1 = getPointAt(fk);
  Vector2 p2;

  real_t length = 0; 

  Point2ArrayPtr points(new Point2Array(stride+1));
  points->setAt(0,Vector2(fk,0));
  real_t u = fk + deltau;
  for(uint_t i = 1 ; i <= stride; ++i, u += deltau){
    p2 = getPointAt(u);
    length += norm(p2 - p1);
    p1 = p2;
    points->setAt(i,Vector2(u,length/totlength));
  }
  points->setAt(stride,Vector2(lk,1.0));
  return QuantisedFunctionPtr(new QuantisedFunction(points,5*stride));
}

/* ----------------------------------------------------------------------- */

Vector2 
Curve2D::findClosest(const Vector2& p, real_t* ui) const{
  real_t u0 = getFirstKnot();
  real_t u1 = getLastKnot();
  real_t deltau = (u1 - u0)/getStride();
  Vector2 p1 = getPointAt(u0);
  Vector2 res = p1;
  real_t dist = normSquared(p-res);
  Vector2 p2, pt;
  real_t lu;
  for(real_t u = u0 + deltau ; u <= u1 ; u += deltau){
    p2 = getPointAt(u);
	pt = p;
	real_t d = closestPointToSegment(pt,p1,p2,&lu);
	if(d < dist){
	  dist = d;
	  res = pt;
      if (ui != NULL) *ui = u + deltau * (lu -1);
	}
    p1 = p2;
  }
  return res;
}

real_t 
PGL(closestPointToSegment)(Vector2& p, 
					       const Vector2& segA,
					       const Vector2& segB,
                           real_t* u)
{
  Vector2 diff = p - segA;
  Vector2 M = segB - segA;
  real_t t = dot(M,diff);
  if(t > 0){
	real_t dotMM = dot(M,M);
	if(t < dotMM){
	  t = t / dotMM;
	  diff -= M*t;
	  p = segA + M*t;
	}
	else {
	  t = 1;
	  diff -= M;
	  p = segB;
	}
  }
  else {
	t = 0;
	p = segA;
  }
  if (u != NULL) *u = t;
  return dot(diff,diff);
}