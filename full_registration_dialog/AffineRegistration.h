#ifndef __AffineRegistration_h
#define __AffineRegistration_h

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

class AffineRegistration
{
public:
  AffineRegistration ();

 static bool GenerateMRISlices(vtkImageData* movingimage, int slicenumber,const char *filename);

 static bool RegisterAffine(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *rigidtx,vtkImageData * TPvolume,vtkPolyData *mri_newlandmarkspart[17], ublas::matrix<double>& thescale);
static bool RegisterAffine2(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *rigidtx,vtkImageData * TPvolume,vtkPolyData *mri_newlandmarkspart[17], ublas::matrix<double>& thescale);
static bool GenerateTPslice(vtkPolyData *pdata, vtkPolyData *TPpoly,vtkImageData* TPvolume, int slicenumber, vtkMatrix4x4 *Topotx, const char *name);
 // static bool RegisterAffine(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *rigidtx,vtkImageData * TPvolume,vtkPolyData *mri_newlandmarkspart[17], ublas::matrix<double>& thescale);
static bool FindOverlapDerivative(vtkImageData* movingimage,vtkImageData* TPvolume,ublas::matrix<double>& overlapderivative, int theslicenumber);
static vtkImageData* GeneratevolfrpmTP(vtkImageData* inputvol, vtkPolyData * polydatasze);
static vtkImageData* ReadGeneratedTPVolume(vtkImageData* inputvol);
static vtkImageData* CalculateImageDerivative(vtkImageData* movingimage, int slicenumber, int direction);
static bool finddTdPhi(ublas::matrix<double>& control_points,int numcp,int xsize,int ysize,ublas::matrix<double>& dTdPhiMatrix_x,ublas::matrix<double>& dTdPhiMatrix_y);
static bool FinddCdPhi(ublas::matrix<double>& theoverlapderivative,vtkImageData* dBdTimagedatax,vtkImageData* dBdTimagedatay,ublas::matrix<double>& dTdPhiMatrixx,ublas::matrix<double>& dTdPhiMatrixy,ublas::matrix<double>& dCdPhi, int slicenumber);
static bool FinddCdPhi2(ublas::matrix<double>& theoverlapderivative,vtkImageData* dBdTimagedatax,vtkImageData* dBdTimagedatay,ublas::matrix<double>& dTdPhiMatrixx,ublas::matrix<double>& dTdPhiMatrixy,ublas::matrix<double>& dCdPhi, int slicenumber);
//static bool GenerateRegistrationPoints(vtkPolyData *TPpoly, vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *XrayVert, vtkImageData * TPvolume,vtkPolyData *topoSlicePolyData,vtkPolyData *mriSlicePolyData, const char *filename, bool fullon);
static bool GenerateRegistrationPoints(vtkPolyData *TPpoly, vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *XrayVert, vtkImageData * TPvolume,vtkPoints *topoSlicePoints,vtkPoints *mriSlicePoints,const char *filename, bool fullon);
//static bool GetnewMRIEdgePoints(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *MRIVert, vtkMatrix4x4 *articulatedtx, float **anglepoints,vtkPolyData *mriEdgePolydata, const char *filename);
static vtkImageData* loadPPM(const char *name);
static vtkImageData* loadPPMvol(const char *name, int slicenum);
static vtkImageData* loadPPMvol2(vtkImageData *tempimage, const char *name, int slicenum);
static void savePPM( vtkImageData* theim, const char *name, int slicenum);
static void savePPMunNorm( vtkImageData* theim, const char *name, int slicenum) ;
static bool GetMRIEdgePoints(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *XrayVert, float **anglepoints,vtkPoints *mriEdgePoints, const char *filename);
//static bool GetnewMRIEdgePoints(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *MRIVert, vtkMatrix4x4 *articulatedtx, float **anglepoints,vtkPolyData *mriEdgePolydata, const char *filename);
static bool GeneratenewRegistrationPoints(vtkPolyData *TPpoly, vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *MRIVert, vtkMatrix4x4 *XrayVert, vtkMatrix4x4 *articulatedtx,vtkPolyData *topoSlicePolyData,vtkPolyData *mriSlicePolyData,const char *filename, const char *filenameTP);
static bool GenerateonlyMRIContourPoints(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *MRIVert, vtkMatrix4x4 *articulatedtx, vtkPolyData *mriSlicePolyData,const char *filename);

static bool GetnewMRIEdgePoints(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *MRIVert, vtkMatrix4x4 *articulatedtx, float **anglepoints,vtkPolyData *mriEdgePolydata, const char *filename, const char *filenameTP);
static float getkappa(vtkFloatArray *scalarsMRI , vtkFloatArray *scalarsTP, int sizex, int sizey, int *numelem  );
static bool CutMRIarms( vtkImageData* movingimage, int slicenumber,const char *filename);
};

#endif