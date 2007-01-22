/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s): C. Nouguier & F. Boudon (frederic.boudon@cirad.fr) nouguier 
 *
 *       $Source$
 *       $Id$
 *
 *       Forum for AMAPmod developers    : amldevlp@cirad.fr
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





#include "geom_frustum.h"
#include "util_messages.h"

GEOM_USING_NAMESPACE

/* ----------------------------------------------------------------------- */


const real_t Frustum::DEFAULT_TAPER(.5);


/* ----------------------------------------------------------------------- */


Frustum::Builder::Builder() :
  Cone::Builder(),
  Taper(0) {
}


Frustum::Builder::~Builder() {
  // nothing to do
}


SceneObjectPtr Frustum::Builder::build( ) const {
  if (isValid()) 
    return SceneObjectPtr
      (new Frustum(Radius ? *Radius : DEFAULT_RADIUS,
		   Height ? *Height : DEFAULT_HEIGHT,
		   Taper ? *Taper : DEFAULT_TAPER,
		   Solid ? *Solid : DEFAULT_SOLID,
		   Slices ? *Slices : DEFAULT_SLICES));
  return SceneObjectPtr();
}


void Frustum::Builder::destroy() {
  ConeDestroy();
  if (Taper) delete Taper;
}


bool Frustum::Builder::isValid( ) const {
  if (! ConeValid()) return false;

  // taper field.
  if (Taper && (*Taper < REAL_EPSILON)) {
    genMessage(WARNINGMSG(INVALID_FIELD_VALUE_sss),"Frustum","Taper","Must be not null.");
    return false;
  };

  return true;
}


/* ----------------------------------------------------------------------- */


Frustum::Frustum( const real_t& radius,
		  const real_t& height,
		  const real_t& taper,
		  bool solid,
		  uchar_t slices ) :
  Cone(radius,height,solid,slices),
  __taper(taper) {
  GEOM_ASSERT(isValid());
}

Frustum::~Frustum( ) {
}

bool 
Frustum::apply( Action& action ) {    
  return action.process(this);
}

bool Frustum::isValid( ) const {
  Builder _builder;
  _builder.Radius = const_cast<real_t *>(&__radius);
  _builder.Height = const_cast<real_t *>(&__height);
  _builder.Taper = const_cast<real_t *>(&__taper);
  _builder.Slices = const_cast<uchar_t *>(&__slices);
  return _builder.isValid();
}
  
SceneObjectPtr Frustum::copy() const {
  return SceneObjectPtr(new Frustum(*this));
}

/* ----------------------------------------------------------------------- */

const real_t&
Frustum::getTaper( ) const {
  return __taper;
}

real_t&
Frustum::getTaper( ) {
  return __taper;
}

bool
Frustum::isTaperToDefault( ) const {
  return __taper == DEFAULT_TAPER;
}

/* ----------------------------------------------------------------------- */