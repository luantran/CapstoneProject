#ifndef __BSplineRegistration_h
#define __BSplineRegistration_h

#include <ostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vtkPolyData.h>
#include <vtkFloatArray.h>
#include <vtkThinPlateSplineTransform.h>
#include "vtkSZEImporter.h"
#include "vtkSZEWriter.h"
#include "vtkThinPlateSplineAproximationTransform.h"

#include <assert.h> //matt code
#include <iomanip>
#include <cmath>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
 #include <boost/numeric/ublas/lu.hpp>
 #include <boost/numeric/ublas/vector_proxy.hpp>
 #include <boost/numeric/ublas/triangular.hpp>
 #include <boost/numeric/ublas/lu.hpp>
 #include <boost/numeric/ublas/io.hpp>
#include "invert_matrix.hpp"

class BSplineRegistration
{
public:
  BSplineRegistration ();

  static vtkImageData* RegisterBspline(vtkImageData* movingimage,vtkImageData * TPvolume,vtkPolyData *mri_newlandmarkspart[17]);
static bool FindOverlapDerivative(vtkImageData* movingimage,vtkImageData* TPvolume,ublas::matrix<double>& overlapderivative, int theslicenumber);
  //static vtkImageData* RegisterBspline(vtkImageData* movingimage,int moving_index, int target_index);
//vtkImageData* RegisterBspline(vtkImageData* movingimage,vtkImageData* targetimage);
static vtkImageData* ApplyBsplineDeformation(ublas::matrix<double>& control_points,vtkImageData* movingimage,int numcp,int moving_index);
//static double* evaluate_Bspline_basis( int ordre, int n, int nb_vectNodal, ublas::matrix<double>& vecteur_nodal, double t);
static int trouver_index_intervalle( double t, int ordre, int n, int nb_vectNodal, ublas::matrix<double> vecteur_nodal );
static bool evaluate_Bspline_basis(int theinterpi,int thenumcols ,int thenumcp, int cpdistance, ublas::matrix<double>& Nipvals);
static vtkImageData* GeneratevolfrpmTP(vtkImageData* inputvol, vtkPolyData * polydatasze);
static void GeneratevolfrpmTP(vtkImageData* inputvol, vtkImageData* theTPvol,  int curr_slice, vtkPolyData * polydatasze);
static vtkImageData* ReadGeneratedTPVolume(vtkImageData* inputvol);
static vtkImageData* CalculateImageDerivative(vtkImageData* movingimage, int slicenumber, int direction);
//static bool finddTdPhi(ublas::matrix<double>& control_points,int numcp,int xsize,int ysize,ublas::matrix<double>& dTdPhiMatrix);
static bool finddTdPhi(ublas::matrix<double>& control_points,int numcp,int xsize,int ysize,ublas::matrix<double>& dTdPhiMatrix_x,ublas::matrix<double>& dTdPhiMatrix_y);
//static bool FinddCdPhi(int numcp, ublas::matrix<double>& theoverlapderivative,vtkImageData* dBdTimagedatax,vtkImageData* dBdTimagedatay,ublas::matrix<double>& dTdPhiMatrixx,ublas::matrix<double>& dTdPhiMatrixy,ublas::matrix<double>& dCdPhi);
static bool FinddCdPhi(int numcp, ublas::matrix<double>& theoverlapderivative,vtkImageData* dBdTimagedatax,vtkImageData* dBdTimagedatay,ublas::matrix<double>& dTdPhiMatrixx,ublas::matrix<double>& dTdPhiMatrixy,ublas::matrix<double>& dCdPhix,ublas::matrix<double>& dCdPhiy);


};

#endif