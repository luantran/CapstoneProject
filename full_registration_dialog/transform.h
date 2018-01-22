/***************************************************************************
                          transform.h  -  description
                             -------------------
    begin                : Tue Dec 16 2003
    copyright            : (C) 2003 by Liao Qinghu
    email                : qing-hu.liao@polymtl.ca
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef __Transform_h
#define __Transform_h

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
#include <vtkMatrix4x4.h>
//#include <boost/numeric/ublas/matrix.hpp>

//#include "linalg3d.h" ///// comes from project http://elonen.iki.fi/code/tpsdemo/index.html
//#include "ludecomposition.h"

//#include <vector>
//#include <cmath>
using namespace std;
struct voldata
{
  int num_components;
  int maxdist;
  float *thedata;
};

struct params
{
  float error_x, error_y, error_z;
  vtkPolyData *src;
  vtkPolyData *dst;
  vtkPolyData *mark1;
  vtkPolyData *mark2;
  int kernel;
};

struct topo_data
{
    vtkPolyData *polydata_szelandmarks;
	vtkPoints *szelandmark_points;
	string *the_landmark;
	vtkPolyData *polydatasze;
	int numelem;
};

struct xray_data
{
    vtkPolyData *polydata_wrllandmarks;
	vtkPoints *wrllandmark_points;
	string *the_landmark;
	vtkPolyData *polydatawrl;
	int numelem;
};

struct landmark_data
{
    vtkPolyData *polydata_szelandmarks;
	vtkPolyData *polydata_wrllandmarks;
	vtkPoints *szelandmark_points;
	vtkPoints *wrllandmark_points;
	string *the_landmark;
	int numelem;
};

struct sze_model_data
{
    vtkPolyData *polydata_szelandmarks;
	vtkPolyData *polydata_wrllandmarks;
	vtkPolyData *polydata_sze;
	vtkPolyData *polydata_wrl;
	string *the_landmark;
	int numelem;
};

struct landmark_data_mri
{
    vtkPolyData *polydata_mrilandmarks;
	vtkPolyData *polydata_wrllandmarks;
	vtkPoints *mrilandmark_points;
	vtkPoints *wrllandmark_points;
	string *the_landmark;
	int numelem;
	vtkDataArray *scalarsold ;
	vtkDataArray *scalarsnew; 
};

struct landmark_data_peds
{
    vtkPolyData *polydata_peds;
	vtkPoints *peds_points;
	string *the_landmark;
	int numelem;
	vtkDataArray *scalarsold ;
	vtkDataArray *scalarsnew; 
};

struct rebuild_data
{
int transx;
int transy;
int transz;
};

struct rigid_vertebrae
{
vtkPolyData **polydata;
int num_vertebrae;
};

class Transform
{
public:
  Transform ();
  static int convert (string input_filename, string out_filename);
  static vtkPolyData *read (string input_filename);
  static void save (vtkPolyData * polydata,string output_filename);
  static float compare (string input_filename1, string input_filename2,
			 string output_filename);
//static vtkMatrix4x4 ** readArticulatedTranfos(char o3txfile[]);
 static void savewrlsep (vtkActor **thevertebrae, string output_filename, int numverts);
  static topo_data RegisterTopoTPS (topo_data topo_data,xray_data unregisteredxray);
 static landmark_data_mri read_spinal_marks_xray(string input_filenamewrl);
static topo_data read_landmarks_topo (string input_filenamesze, topo_data thedata);
static xray_data read_landmarks_xray (string input_filenamewrl, xray_data thedata);
static landmark_data_mri read_spinal_marks_xray_peds(string input_filenamewrl);
static landmark_data_mri read_spinal_marks_mri(string input_filenamemri);
static landmark_data_mri read_spinal_marks_mri_tomo(string mri_marker_file);
static landmark_data_mri read_spinal_marks_mri_tomo_all(string mri_marker_file);
static landmark_data_mri read_spinal_marks_xray8(string input_filenamewrl);
//static vtkPolyData** read_wrlsep (string input_filename);
static rigid_vertebrae read_wrlsep (string input_filename);
static void mInverse(double **diag, double **invdiag, int num_vars);
static void CoFactor2(double **a,int n,double **b);
static double Determinant(double **a, int n);
static void Transpose(double **a, int n);
//static void matrix_mult(double **invL, double **Y, double **W, int sizeL, int sizeY);
static double** matrix_mult(double **invL, double **Y, int sizeL, int sizeY);
static void saveMRImage (vtkImageData *theimage);

static void ludcmp(double** a, int n, int *indx, double *d);
static void lubksb(double **a, int n, int *indx, double b[]);
static double det(double **a, int n) ;
static double** inverse(double **a, int n);
  
private:
  static float compare (vtkPolyData * polydata1, vtkPolyData * polydata2,
                       bool bidirection=false, bool clip=false ,
											 float *range=NULL);
  static float compareMark (vtkPoints * mark1, vtkPoints * mark2);
  static float statis (vtkFloatArray * array, float ratio);


};

#endif
