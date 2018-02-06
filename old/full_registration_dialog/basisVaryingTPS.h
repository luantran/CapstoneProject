#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkCell.h>
#include "invert_matrix.hpp"
#include <vtkSmartPointer.h>

#pragma once
class basisVaryingTPS
{

	protected:
		double(*P)[4];// = new double[num_landmarks][4]; //row then column
		double(*V)[3];// = new double[num_landmarks+3][3];
		int nNumLandmarks;
		double **W;
		double vertebralCenter[3];
		//vtkPoints* EdgePoints;
		float **angleMRIdistances;
		double vertebralRadius;
		

public:
	basisVaryingTPS(void);
	~basisVaryingTPS(void);
	void SetNumLandmarks(int theNum);
	void SetSourcePoints(vtkPolyData * sourcePoints);
	void SetTargetPoints(vtkPolyData * targetPoints);
	void ComputeWeights();
	vtkImageData* DeformGrid(vtkImageData* inputGrid);
	vtkImageData*DeformGridrBody(vtkImageData* inputGrid);
	vtkPolyData *DeformPoints(vtkPolyData * sourcePoints);
	void setVertMiddle(double x,double y, double z);
	void SetEdgePoints(float** theEdgePoints);
	void setVertebralRadius(double theradius);

private:
	double** matrix_mult(double **invL, double **Y, int sizeL, int sizeY);
	double** inverse(double **a, int n);
	void ludcmp(double** a, int n, int *indx, double *d);
	void lubksb(double **a, int n, int *indx, double b[]);
};

