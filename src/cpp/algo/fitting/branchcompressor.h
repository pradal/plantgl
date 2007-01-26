/* -*-c++-*-
 *  ----------------------------------------------------------------------------
 *
 *       AMAPmod: Exploring and Modeling Plant Architecture 
 *
 *       Copyright 1995-2000 UMR Cirad/Inra Modelisation des Plantes
 *
 *       File author(s) : F. Boudon (frederic.boudon@cirad.fr) 
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
				
/*! \file branchcompressor.h
    \brief An algorithm to compress geometry of lineic component.
*/

/* ----------------------------------------------------------------------- */

#ifndef __branch_compressor_h__
#define __branch_compressor_h__

/* ----------------------------------------------------------------------- */

#include "GEOM/geom_nurbscurve.h"
#include "GEOM/appe_appearance.h"
#include "GEOM/geom_pointarray.h" 
#include "GEOM/scne_scene.h"
#include <vector>

GEOM_BEGIN_NAMESPACE

struct BranchInput {
  Point3ArrayPtr points;
  Point2ArrayPtr radius;
  AppearancePtr appearance;
  int id;
  int father;
};

class GEOM_API BranchCompressor {
public :
    static Curve2DPtr DEFAULT_CROSS_SECTION;
	BranchCompressor();
	~BranchCompressor(){};

	void setVerbose(bool b) { __verbose = b; }
	void setData(const std::vector<BranchInput>&, int sorttype = 0);
	void clear();

  ScenePtr
	compress(real_t taux = 50,
			 real_t * obtainedrate = NULL,
			 real_t * computationtime = NULL, 
			 real_t * fittime = NULL, 
			 real_t ErreurBound = 1.0, 
			 int degre = 3, 
			 int option = 0);

	std::vector<BranchInput> sortByIncSize(const std::vector<BranchInput>&);
    std::vector<BranchInput> sortByDecSize(const std::vector<BranchInput>&);
	std::vector<BranchInput> sortByIncOrder(const std::vector<BranchInput>&);
    std::vector<BranchInput> sortByDecOrder(const std::vector<BranchInput>&);
	ScenePtr inputScene() const;

public :
  std::vector<BranchInput> __inputs;
  bool __verbose;

private :
  int __roots;

  void interConnection(ScenePtr& scene);

  TOOLS(Vector3) connectionTo(const TOOLS(Vector3)& p, 
					   const ScenePtr& scene,
					   int fid);
  TOOLS(Vector4) connectionTo(const TOOLS(Vector4)& p, 
					   const ScenePtr& scene,
					   int fid);

  void addScene(ScenePtr scene, int c_branch,LineicModelPtr axis) const;
  void addScene(ScenePtr scene, int c_branch,GeometryPtr geom) const;
  void addScene(ScenePtr scene, int c_branch,
				const TOOLS(Vector3)& p1,const TOOLS(Vector3)& p2) const;

  void removeDouble(Point2ArrayPtr&,RealArrayPtr&) const;
  bool isDefault(RealArrayPtr&) const;
};

/* ----------------------------------------------------------------------- */

Point3ArrayPtr  
discretizeWithCurvature(NurbsCurvePtr C,int NumberOfPoint);

Point3ArrayPtr  
compressPolyline(Point3ArrayPtr C,int NumberOfPoint);

NurbsCurvePtr
fitt(const Point3ArrayPtr&  MyVector, 
	 int deg, int CPbegin, int nbPtCtrlMax, 
	 real_t ErreurBound, int &  Nbloop, real_t & SommeEk);

/* ----------------------------------------------------------------------- */

GEOM_END_NAMESPACE

#endif
