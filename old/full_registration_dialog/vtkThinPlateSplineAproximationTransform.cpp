/*=========================================================================

Program:   Visualization Toolkit
Module:    $RCSfile: vtkThinPlateSplineAproximationTransform.h,v $
Language:  C++
Date:      $Date: 2004/05/22 15:30:39 $
Version:   $Revision: 0.99 $
Author:    Qinghu Liao
E-mail:    qing-hu.liao@polymtl.ca

=========================================================================*/  
#include "stdafx.h"
#include "vtkThinPlateSplineAproximationTransform.h"
  
#include <vtkMath.h>
#include <vtkObjectFactory.h>
#include <vtkPoints.h>
vtkCxxRevisionMacro (vtkThinPlateSplineAproximationTransform,
		     "$Revision: 0.99 $");
vtkStandardNewMacro (vtkThinPlateSplineAproximationTransform);

//------------------------------------------------------------------------
// some dull matrix things
inline double **
vtkNewMatrix (int rows, int cols) 
{
  double *matrix = new double[rows * cols];
  double **m = new double *[rows];
  for (int i = 0; i < rows; i++)    
    {
      m[i] = &matrix[i * cols];
    } 
  return m;
}

//------------------------------------------------------------------------
inline void
vtkDeleteMatrix (double **m) 
{
  delete[] * m;
  delete[]m;
} 

//------------------------------------------------------------------------
inline double ***
vtkNewMatrix (int num, int rows, int cols) 
{
  double ***m = new double **[num];
  for (int i = 0; i < num; i++)    
    {
      m[i] = vtkNewMatrix (rows, cols);
    } 
  return m;
}

//------------------------------------------------------------------------
inline void
vtkDeleteMatrix (double ***m) 
{
  delete[] ** m;
  delete[]m;
}


//------------------------------------------------------------------------
inline void
vtkZeroMatrix (double **m, int rows, int cols) 
{
  for (int i = 0; i < rows; i++)    
    {
      for (int j = 0; j < cols; j++)	
        m[i][j] = 0.0;
    } 
} 

//------------------------------------------------------------------------
inline void
vtkMatrixScalarMultiplyandAdd (double **a, double **b, double scalar,
                               int arows, int acols) 
{
  // a and b must have size arows*bcols (we assume this)
  for (int i = 0; i < arows; i++)    
    {
      for (int j = 0; j < acols; j++)	
          a[i][j] += b[i][j] * scalar; 
    } 
} 

//------------------------------------------------------------------------
inline void
vtkMatrixMultiply (double **a, double **b, double **c, int arows, int acols,
                   int brows, int bcols) 
{
  if (acols != brows)    
    {
      return;        // acols must equal br otherwise we can't proceed
    }
  
  // c must have size arows*bcols (we assume this)
  for (int i = 0; i < arows; i++)    
    {
      for (int j = 0; j < bcols; j++)	
        {
          c[i][j] = 0.0;
          for (int k = 0; k < acols; k++)	    
            c[i][j] += a[i][k] * b[k][j];
        } 
    } 
}

//------------------------------------------------------------------------
inline void
vtkMatrixTranspose (double **a, double **b, int rows, int cols) 
{
  for (int i = 0; i < rows; i++)    
    {
      for (int j = 0; j < cols; j++)	
        {
          double tmp = a[i][j];
          b[i][j] = a[j][i];
          b[j][i] = tmp;
        } 
    } 
} 

//------------------------------------------------------------------------
vtkThinPlateSplineAproximationTransform::
vtkThinPlateSplineAproximationTransform () 
{
  this->SourceLandmarks = NULL;
  this->TargetLandmarks = NULL;
  this->Sigma = 1.0;
  this->Tradeoff = 0.0;
  
  //   this->TransType = 2;
  this->SourceScalarVariance = NULL;
  this->TargetScalarVariance = NULL;
  this->SourceCoVariance = NULL;
  this->TargetCoVariance = NULL;
  
  // If the InverseFlag is set, then we use an iterative
  // method to invert the transformation.  
  // The InverseTolerance sets the precision to which we want to 
  // calculate the inverse.
  this->InverseTolerance = 0.001;
  this->InverseIterations = 500;
  this->Basis = -1;
  this->SetBasisToR();
  this->NumberOfPoints = 0;
  this->MatrixW = NULL;
} 

//------------------------------------------------------------------------
vtkThinPlateSplineAproximationTransform::
~vtkThinPlateSplineAproximationTransform () 
{
  if (this->SourceLandmarks)    
    {
      this->SourceLandmarks->Delete ();
    }
  if (this->TargetLandmarks)    
    {
      this->TargetLandmarks->Delete ();
    }
  if (this->SourceScalarVariance)   
    {
      vtkDeleteMatrix (this->SourceScalarVariance);
      this->SourceScalarVariance = NULL;
    }
  if (this->TargetScalarVariance)    
    {
      vtkDeleteMatrix (this->TargetScalarVariance);
      this->TargetScalarVariance = NULL;
    }
  if (this->SourceCoVariance)    
    {
      vtkDeleteMatrix (this->SourceCoVariance);
      this->SourceCoVariance = NULL;
    }
  if (this->TargetCoVariance)    
    {
      vtkDeleteMatrix (this->TargetCoVariance);
      this->TargetCoVariance = NULL;
    }
  if (this->MatrixW)    
    {
      vtkDeleteMatrix (this->MatrixW);
      this->MatrixW = NULL;
    }
}


//------------------------------------------------------------------------
void vtkThinPlateSplineAproximationTransform::
SetSourceScalarVariance (int num, double V) 
{
  int num_of_mark;
  num_of_mark = this->SourceLandmarks->GetNumberOfPoints ();
  if (num > num_of_mark || num == 0)    
    {
      printf ("error, num should be index from 1,...n");
      exit (-1);
    }
  if (this->SourceScalarVariance == NULL)    
    {
      this->SourceScalarVariance = vtkNewMatrix (num_of_mark, 1);
    }
  this->SourceScalarVariance[num - 1][0] = V;
}


//------------------------------------------------------------------------
void vtkThinPlateSplineAproximationTransform::
SetTargetScalarVariance (int num, double V) 
{
  int num_of_mark;
  num_of_mark = this->SourceLandmarks->GetNumberOfPoints ();
  if (num > num_of_mark || num == 0)    
    {
      printf ("error, num should be index from 1,...n");
      exit (-1);
    }
  if (this->TargetScalarVariance == NULL)    
    {
      this->TargetScalarVariance = vtkNewMatrix (num_of_mark, 1);
    }
  this->TargetScalarVariance[num - 1][0] = V;
}


//------------------------------------------------------------------------
void
vtkThinPlateSplineAproximationTransform::
SetSourceCoVariance (int num, double Vx, double Vy, double Vz) 
{
  const int D = 3;      // dimensions
  int num_of_mark;
  num_of_mark = this->SourceLandmarks->GetNumberOfPoints ();
  if (num >= num_of_mark || num < 0)    
    {
      printf ("error, num should be index from 0,...n-1");
      exit (-1);
    }
  if (this->SourceCoVariance == NULL)    
    {
      this->SourceCoVariance = vtkNewMatrix (num_of_mark, D, D);
    }
  this->SourceCoVariance[num][0][0] = Vx;
  this->SourceCoVariance[num][0][1] = sqrt (Vx * Vy);
  this->SourceCoVariance[num][0][2] = sqrt (Vx * Vz);
  this->SourceCoVariance[num][1][0] = sqrt (Vy * Vx);
  this->SourceCoVariance[num][1][1] = Vy;
  this->SourceCoVariance[num][1][2] = sqrt (Vy * Vz);
  this->SourceCoVariance[num][2][0] = sqrt (Vz * Vx);
  this->SourceCoVariance[num][2][1] = sqrt (Vz * Vy);
  this->SourceCoVariance[num][2][2] = Vz;
}


//------------------------------------------------------------------------
void vtkThinPlateSplineAproximationTransform::
SetTargetCoVariance (int num, double Vx, double Vy, double Vz) 
{
  const int D = 3;       // dimensions
  int num_of_mark;
  num_of_mark = this->SourceLandmarks->GetNumberOfPoints ();
  if (num >= num_of_mark || num < 0)    
    {
      printf ("error, num should be index from 0,...n-1");
      exit (-1);
    }
  if (this->TargetCoVariance == NULL)    
    {
      this->TargetCoVariance = vtkNewMatrix (num_of_mark, D, D);
    }
  this->TargetCoVariance[num][0][0] = Vx;
  this->TargetCoVariance[num][0][1] = sqrt (Vx * Vy);
  this->TargetCoVariance[num][0][2] = sqrt (Vx * Vz);
  this->TargetCoVariance[num][1][0] = sqrt (Vy * Vx);
  this->TargetCoVariance[num][1][1] = Vy;
  this->TargetCoVariance[num][1][2] = sqrt (Vy * Vz);
  this->TargetCoVariance[num][2][0] = sqrt (Vz * Vx);
  this->TargetCoVariance[num][2][1] = sqrt (Vz * Vy);
  this->TargetCoVariance[num][2][2] = Vz;
}


//------------------------------------------------------------------------
void vtkThinPlateSplineAproximationTransform::
SetSourceLandmarks (vtkPoints *  source) 
{
  if (this->SourceLandmarks == source)    
    {
      return;
    }
  if (this->SourceLandmarks)    
    {
      this->SourceLandmarks->Delete ();
    }
  source->Register (this);
  this->SourceLandmarks = source;
  this->Modified ();
}


//------------------------------------------------------------------------
void vtkThinPlateSplineAproximationTransform::
SetTargetLandmarks (vtkPoints *  target) 
{
  if (this->TargetLandmarks == target)    
    {
      return;
    }
  if (this->TargetLandmarks)    
    {
      this->TargetLandmarks->Delete ();
    }
  target->Register (this);
  this->TargetLandmarks = target;
  this->Modified ();
}


//------------------------------------------------------------------------
unsigned long vtkThinPlateSplineAproximationTransform::GetMTime () 
{
  unsigned long result = this->vtkWarpTransform::GetMTime ();
  unsigned long mtime;
  if (this->SourceLandmarks)    
    {
      mtime = this->SourceLandmarks->GetMTime ();
      if (mtime > result)	
        {
          result = mtime;
        }
    }
  if (this->TargetLandmarks)    
    {
      mtime = this->TargetLandmarks->GetMTime ();
      if (mtime > result)	
        {
          result = mtime;
        }
    }
  return result;
}


//------------------------------------------------------------------------
void vtkThinPlateSplineAproximationTransform::InternalUpdate () 
{
  vtkDebugMacro (<<"==Internal Update====\n");
  if (this->SourceLandmarks == NULL || this->TargetLandmarks == NULL)    
    {
      if (this->MatrixW)	
        {
           vtkDeleteMatrix (this->MatrixW);
        }
      this->MatrixW = NULL;
      this->NumberOfPoints = 0;
      return;
    }
  if (this->SourceLandmarks->GetNumberOfPoints () !=
      this->TargetLandmarks->GetNumberOfPoints ())    
    {
      vtkErrorMacro (<< "Update: Source and Target Landmarks contain" << 
                        "a different number of points");
      return;
    }
  const vtkIdType N = this->SourceLandmarks->GetNumberOfPoints ();
  const int D = 3;   // dimensions
  
  // the out weights matrix should be W'=(Wij*Id) 
  // where Id=[1,1,1]' is identity vector 
  double **W;  // the output weights matrix
  
  if (N >= 3)    
    {     
      // Notation and inspiration from:
      // Fred L. Bookstein (1997) "Shape and the Information in Medical Images: 
      // A Decade of the Morphometric Synthesis" Computer Vision and Image 
      // Understanding 66(2):97-118
      // and online work published by Tim Cootes
      // (http://www.wiau.man.ac.uk/~bim)
      W = vtkNewMatrix (3 * (N + D + 1), D);
      double **A = &W[3 * (N + 1)];	// the linear rotation + scale matrix 
      double *C = W[3 * N];	// the linear translation 
      
      // the input matrices
      double **L = vtkNewMatrix (3 * (N + D + 1), 3 * (N + D + 1));
      double **X = vtkNewMatrix (3 * (N + D + 1), D);
      
      // build L
      // will leave the bottom-right corner with zeros
      vtkZeroMatrix (L, 3 * (N + D + 1), 3 * (N + D + 1));
      
      int q, c;
      double p[3], p2[3];
      double dx, dy, dz;
      double r;
      double (*phi) (double) = this->BasisFunction;
      for (q = 0; q < N; q++)	
        {
          this->SourceLandmarks->GetPoint (q, p);	  
          // fill in the top-right and bottom-left corners of L (Q)
          L[3 * N + 0][3 * q + 0] = L[3 * q + 0][3 * N + 0] = 1.0;
          L[3 * N + 1][3 * q + 0] = L[3 * q + 0][3 * N + 1] = 1.0;
          L[3 * N + 2][3 * q + 0] = L[3 * q + 0][3 * N + 2] = 1.0;
          L[3 * N + 0][3 * q + 1] = L[3 * q + 1][3 * N + 0] = 1.0;
          L[3 * N + 1][3 * q + 1] = L[3 * q + 1][3 * N + 1] = 1.0;
          L[3 * N + 2][3 * q + 1] = L[3 * q + 1][3 * N + 2] = 1.0;
          L[3 * N + 0][3 * q + 2] = L[3 * q + 2][3 * N + 0] = 1.0;
          L[3 * N + 1][3 * q + 2] = L[3 * q + 2][3 * N + 1] = 1.0;
          L[3 * N + 2][3 * q + 2] = L[3 * q + 2][3 * N + 2] = 1.0;
          L[3 * (N + 1) + 0][3 * q + 0] = L[3 * q + 0][3 * (N + 1) + 0] = p[0];
          L[3 * (N + 1) + 1][3 * q + 0] = L[3 * q + 0][3 * (N + 1) + 1] = p[0];
          L[3 * (N + 1) + 2][3 * q + 0] = L[3 * q + 0][3 * (N + 1) + 2] = p[0];
          L[3 * (N + 1) + 0][3 * q + 1] = L[3 * q + 1][3 * (N + 1) + 0] = p[0];
          L[3 * (N + 1) + 1][3 * q + 1] = L[3 * q + 1][3 * (N + 1) + 1] = p[0];
          L[3 * (N + 1) + 2][3 * q + 1] = L[3 * q + 1][3 * (N + 1) + 2] = p[0];
          L[3 * (N + 1) + 0][3 * q + 2] = L[3 * q + 2][3 * (N + 1) + 0] = p[0];
          L[3 * (N + 1) + 1][3 * q + 2] = L[3 * q + 2][3 * (N + 1) + 1] = p[0];
          L[3 * (N + 1) + 2][3 * q + 2] = L[3 * q + 2][3 * (N + 1) + 2] = p[0];
          L[3 * (N + 2) + 0][3 * q + 0] = L[3 * q + 0][3 * (N + 2) + 0] = p[1];
          L[3 * (N + 2) + 1][3 * q + 0] = L[3 * q + 0][3 * (N + 2) + 1] = p[1];
          L[3 * (N + 2) + 2][3 * q + 0] = L[3 * q + 0][3 * (N + 2) + 2] = p[1];
          L[3 * (N + 2) + 0][3 * q + 1] = L[3 * q + 1][3 * (N + 2) + 0] = p[1];
          L[3 * (N + 2) + 1][3 * q + 1] = L[3 * q + 1][3 * (N + 2) + 1] = p[1];
          L[3 * (N + 2) + 2][3 * q + 1] = L[3 * q + 1][3 * (N + 2) + 2] = p[1];
          L[3 * (N + 2) + 0][3 * q + 2] = L[3 * q + 2][3 * (N + 2) + 0] = p[1];
          L[3 * (N + 2) + 1][3 * q + 2] = L[3 * q + 2][3 * (N + 2) + 1] = p[1];
          L[3 * (N + 2) + 2][3 * q + 2] = L[3 * q + 2][3 * (N + 2) + 2] = p[1];
          L[3 * (N + 3) + 0][3 * q + 0] = L[3 * q + 0][3 * (N + 3) + 0] = p[2];
          L[3 * (N + 3) + 1][3 * q + 0] = L[3 * q + 0][3 * (N + 3) + 1] = p[2];
          L[3 * (N + 3) + 2][3 * q + 0] = L[3 * q + 0][3 * (N + 3) + 2] = p[2];
          L[3 * (N + 3) + 0][3 * q + 1] = L[3 * q + 1][3 * (N + 3) + 0] = p[2];
          L[3 * (N + 3) + 1][3 * q + 1] = L[3 * q + 1][3 * (N + 3) + 1] = p[2];
          L[3 * (N + 3) + 2][3 * q + 1] = L[3 * q + 1][3 * (N + 3) + 2] = p[2];
          L[3 * (N + 3) + 0][3 * q + 2] = L[3 * q + 2][3 * (N + 3) + 0] = p[2];
          L[3 * (N + 3) + 1][3 * q + 2] = L[3 * q + 2][3 * (N + 3) + 1] = p[2];
          L[3 * (N + 3) + 2][3 * q + 2] = L[3 * q + 2][3 * (N + 3) + 2] = p[2];
          
          // fill in the top-left corner of L (K), using symmetry
          //(K+N*tradeoff*W(-1)) . fill the first part.
          for (c = 0; c < q; c++)	    
            {
              this->SourceLandmarks->GetPoint (c, p2);
              dx = p[0] - p2[0];
              dy = p[1] - p2[1];
              dz = p[2] - p2[2];
              r = sqrt (dx * dx + dy * dy + dz * dz);
              double tmp = phi (r / this->Sigma);
              L[3 * q + 0][3 * c + 0] = L[3 * c + 0][3 * q + 0] = tmp;
              L[3 * q + 1][3 * c + 0] = L[3 * c + 0][3 * q + 1] = tmp;
              L[3 * q + 2][3 * c + 0] = L[3 * c + 0][3 * q + 2] = tmp;
              L[3 * q + 0][3 * c + 1] = L[3 * c + 1][3 * q + 0] = tmp;
              L[3 * q + 1][3 * c + 1] = L[3 * c + 1][3 * q + 1] = tmp;
              L[3 * q + 2][3 * c + 1] = L[3 * c + 1][3 * q + 2] = tmp;
              L[3 * q + 0][3 * c + 2] = L[3 * c + 2][3 * q + 0] = tmp;
              L[3 * q + 1][3 * c + 2] = L[3 * c + 2][3 * q + 1] = tmp;
              L[3 * q + 2][3 * c + 2] = L[3 * c + 2][3 * q + 2] = tmp;
            } 
          //fill the seconde part. of covariance.further assume that the images 
          //have approximately the same oritentation and scale . 
          //we can simple add the two covariance matrices.
          L[3 * q + 0][3 * q + 0] =
            L[3 * q + 0][3 * q + 0] +
            N * Tradeoff * (this->SourceCoVariance[q][0][0] +
                            this->TargetCoVariance[q][0][0]);
          
          L[3 * q + 0][3 * q + 1] =
            L[3 * q + 0][3 * q + 1] +
            N * Tradeoff * (this->SourceCoVariance[q][0][1] +
                            this->TargetCoVariance[q][0][1]);
          L[3 * q + 0][3 * q + 2] =
            L[3 * q + 0][3 * q + 2] +
            N * Tradeoff * (this->SourceCoVariance[q][0][2] +
                            this->TargetCoVariance[q][0][2]);
          L[3 * q + 1][3 * q + 0] =
            L[3 * q + 1][3 * q + 0] +
            N * Tradeoff * (this->SourceCoVariance[q][1][0] +
                            this->TargetCoVariance[q][1][0]);
          L[3 * q + 1][3 * q + 1] =
            L[3 * q + 1][3 * q + 1] +
            N * Tradeoff * (this->SourceCoVariance[q][1][1] +
                            this->TargetCoVariance[q][1][1]);
          L[3 * q + 1][3 * q + 2] =
            L[3 * q + 1][3 * q + 2] +
            N * Tradeoff * (this->SourceCoVariance[q][1][2] +
                            this->TargetCoVariance[q][1][2]);
          L[3 * q + 2][3 * q + 0] =
            L[3 * q + 2][3 * q + 0] +
            N * Tradeoff * (this->SourceCoVariance[q][2][0] +
                            this->TargetCoVariance[q][2][0]);
          L[3 * q + 2][3 * q + 1] =
            L[3 * q + 2][3 * q + 1] +
            N * Tradeoff * (this->SourceCoVariance[q][2][1] +
                            this->TargetCoVariance[q][2][1]);
          L[3 * q + 2][3 * q + 2] =
            L[3 * q + 2][3 * q + 2] +
            N * Tradeoff * (this->SourceCoVariance[q][2][2] +
                            this->TargetCoVariance[q][2][2]);
      } 

      // build X
      vtkZeroMatrix (X, 3 * (N + D + 1), D);
      for (q = 0; q < N; q++)	
        {
          this->TargetLandmarks->GetPoint (q, p);
          X[3 * q + 0][0] = p[0];
          X[3 * q + 1][0] = p[0];
          X[3 * q + 2][0] = p[0];
          X[3 * q + 0][1] = p[1];
          X[3 * q + 1][1] = p[1];
          X[3 * q + 2][1] = p[1];
          X[3 * q + 0][2] = p[2];
          X[3 * q + 1][2] = p[2];
          X[3 * q + 2][2] = p[2];
        }
      

      // solve for W, where W = Inverse(L)*X; 
      // this is done via eigenvector decomposition so
      // that we can avoid singular values
      // W = V*Inverse(w)*U*X  
      double *values = new double[3 * (N + D + 1)];
      double **V = vtkNewMatrix (3 * (N + D + 1), 3 * (N + D + 1));
      double **w = vtkNewMatrix (3 * (N + D + 1), 3 * (N + D + 1));
      double **U = L;		// reuse the space
      vtkMath::JacobiN (L, 3 * (N + D + 1), values, V);
      vtkMatrixTranspose (V, U, 3 * (N + D + 1), 3 * (N + D + 1));
      vtkIdType i, j;
      double maxValue = 0.0;	// maximum eigenvalue
      for (i = 0; i < 3 * (N + D + 1); i++)	
        {
          double tmp = fabs (values[i]);
          if (tmp > maxValue)	    
            {
              maxValue = tmp;
            }
        }
      for (i = 0; i < 3 * (N + D + 1); i++)	
        {
          for (j = 0; j < 3 * (N + D + 1); j++)	    
            {
              w[i][j] = 0.0;
            }
          
          // here's the trick: don't invert the singular values
          if (fabs (values[i] / maxValue) > 1e-16)	    
            {
              w[i][i] = 1.0 / values[i];
            }
        }
      delete[]values;
      vtkMatrixMultiply (U, X, W, 3 * (N + D + 1), 3 * (N + D + 1),
        		 3 * (N + D + 1), D);
      vtkMatrixMultiply (w, W, X, 3 * (N + D + 1), 3 * (N + D + 1),
        		 3 * (N + D + 1), D);
      vtkMatrixMultiply (V, X, W, 3 * (N + D + 1), 3 * (N + D + 1),
        		 3 * (N + D + 1), D);
      vtkDeleteMatrix (V);
      vtkDeleteMatrix (w);
      vtkDeleteMatrix (U);
      vtkDeleteMatrix (X);
      
      // now the linear portion of the warp must be checked
      // (this is a very poor check for now)
      if (fabs (vtkMath::Determinant3x3 ((double (*)[3]) *A)) < 1e-16)	
        {
          for (i = 0; i < 3; i++)	    
            {
              if (sqrt(A[0][i] * A[0][i] + A[1][i] * A[1][i] +
                  A[2][i] * A[2][i])  <1e-16)		
                {
                  A[0][i] = A[1][i] = A[2][i] = A[i][0] 
                               = A[i][1] = A[i][2] = 0;
                  A[i][i] = 1.0;
                }
            }
        }
    }
  
  // special cases, I added these to ensure that this class doesn't 
  // misbehave if the user supplied fewer than 3 landmarks
  else   // (N < 3)
    {
      W = vtkNewMatrix (N + D + 1, D);
      double **A = &W[N + 1]; // the linear rotation + scale matrix 
      double *C = W[N];       // the linear translation 
      vtkIdType i, j;
      
      // set nonlinear portion of transformation to zero
      for (i = 0; i < N; i++)	
        {
          for (j = 0; j < D; j++)	    
            {
              W[i][j] = 0;
            }
        }
      if (N == 2)	
        {  // two landmarks, construct a similarity transformation
          double s0[3], t0[3], s1[3], t1[3];
          this->SourceLandmarks->GetPoint (0, s0);
          this->TargetLandmarks->GetPoint (0, t0);
          this->SourceLandmarks->GetPoint (1, s1);
          this->TargetLandmarks->GetPoint (1, t1);
          double ds[3], dt[3], as[3], at[3];
          double rs = 0, rt = 0;
          for (i = 0; i < 3; i++)	    
            {
              as[i] = (s0[i] + s1[i]) / 2;	// average of endpoints
              ds[i] = s1[i] - s0[i];	// vector between points
              rs += ds[i] * ds[i];
              at[i] = (t0[i] + t1[i]) / 2;
              dt[i] = t1[i] - t0[i];
              rt += dt[i] * dt[i];
            }

          // normalize the two vectors
          rs = sqrt (rs);
          ds[0] /= rs;
          ds[1] /= rs;
          ds[2] /= rs;
          rt = sqrt (rt);
          dt[0] /= rt;
          dt[1] /= rt;
          dt[2] /= rt;
          double w, x, y, z;
          
          // take dot & cross product
          w = ds[0] * dt[0] + ds[1] * dt[1] + ds[2] * dt[2];
          x = ds[1] * dt[2] - ds[2] * dt[1];
          y = ds[2] * dt[0] - ds[0] * dt[2];
          z = ds[0] * dt[1] - ds[1] * dt[0];
          double r = sqrt (x * x + y * y + z * z);
          double theta = atan2 (r, w);
          
          // construct quaternion
          w = cos (theta / 2);
          if (r != 0)	    
            {
              r = sin (theta / 2) / r;
              x = x * r;
              y = y * r;
              z = z * r;
            }  
          else	// rotation by 180 degrees
            {      
              // rotate around a vector perpendicular to ds
              vtkMath::Perpendiculars (ds, dt, 0, 0);
              r = sin (theta / 2);
              x = dt[0] * r;
              y = dt[1] * r;
              z = dt[2] * r;
            }
 
          // now r is scale factor for matrix
          r = rt / rs;
          
          // build a rotation + scale matrix
          A[0][0] = (w * w + x * x - y * y - z * z) * r;
          A[0][1] = (x * y + w * z) * 2 * r;
          A[0][2] = (x * z - w * y) * 2 * r;
          A[1][0] = (x * y - w * z) * 2 * r;
          A[1][1] = (w * w - x * x + y * y - z * z) * r;
          A[1][2] = (y * z + w * x) * 2 * r;
          A[2][0] = (x * z + w * y) * 2 * r;
          A[2][1] = (y * z - w * x) * 2 * r;
          A[2][2] = (w * w - x * x - y * y + z * z) * r;
  
          // include the translation
          C[0] = at[0] - as[0] * A[0][0] - as[1] * A[1][0] - as[2] * A[2][0];
          C[1] = at[1] - as[0] * A[0][1] - as[1] * A[1][1] - as[2] * A[2][1];
          C[2] = at[2] - as[0] * A[0][2] - as[1] * A[1][2] - as[2] * A[2][2];
        }
      
      else if (N == 1)		// one landmark, translation only
        {
          double p[3], p2[3];
          this->SourceLandmarks->GetPoint (0, p);
          this->TargetLandmarks->GetPoint (0, p2);
          for (i = 0; i < D; i++)	    
            {
              for (j = 0; j < D; j++)		
                {
                  A[i][j] = 0;
                }
              A[i][i] = 1;
              C[i] = p2[i] - p[i];
            }
        }      
      else   // if no landmarks, set to identity
        {
          for (i = 0; i < D; i++)    
            {
              for (j = 0; j < D; j++)
                {
                  A[i][j] = 0;
                }
               A[i][i] = 1;
               C[i] = 0;
            }
        }
    }
  if (this->MatrixW)    
    {
      vtkDeleteMatrix (this->MatrixW);
    }
  this->MatrixW = W;
  this->NumberOfPoints = N;
  
  //PrintMatrixW();
}


//------------------------------------------------------------------------
// The matrix W was created by Update.  Not much has to be done to
// apply the transform:  do an affine transformation, then do
// perturbations based on the landmarks.
template < class T > inline void
vtkThinPlateSplineForwardTransformPoint
(vtkThinPlateSplineAproximationTransform * self, double **W, int N,
 double (*phi) (double), const T point[3], T output[3]) 
{
  if (N == 0)    
    {
      output[0] = point[0];
      output[1] = point[1];
      output[2] = point[2];
      return;
    }
  double **C;
  double **A;
  double dx, dy, dz;
  double p[3];
  double U, r;
  double invSigma = 1.0 / self->GetSigma ();
  double **interpoint;
  double x = 0, y = 0, z = 0;
  vtkIdType i;
  vtkPoints * sourceLandmarks = self->GetSourceLandmarks ();
  if (N <= 3)    
  //this case is the roate and scale translate. not realted to the thin-plate
  //spline, becuase the number of the landmark is too few.
    {      
      // do the nonlinear stuff
      C = &W[N];
      A = &W[N + 1];
      for (i = 0; i < N; i++)	
        {
          sourceLandmarks->GetPoint (i, p);
          dx = point[0] - p[0];
          dy = point[1] - p[1];
          dz = point[2] - p[2];
          r = sqrt (dx * dx + dy * dy + dz * dz);
          U = phi (r * invSigma);
          x += U * W[i][0];
          y += U * W[i][1];
          z += U * W[i][2];
        }
      
      // finish off with the affine transformation
      x +=
        C[0][0] + point[0] * A[0][0] + point[1] * A[1][0] +
        point[2] * A[2][0];
      y +=
        C[0][1] + point[0] * A[0][1] + point[1] * A[1][1] +
        point[2] * A[2][1];
      z +=
        C[0][2] + point[0] * A[0][2] + point[1] * A[1][2] +
        point[2] * A[2][2];
      output[0] = x;
      output[1] = y;
      output[2] = z;
    }  
  else    
    // this case is the thin-plate spline approximation translate.
    {
      interpoint = vtkNewMatrix (3, 3);
      vtkZeroMatrix (interpoint, 3, 3);
      C = &W[3 * N];
      A = &W[3 * (N + 1)];
      double **w;
      
      // Do the nonlinear stuff
      for (i = 0; i < N; i++)	
        {
          sourceLandmarks->GetPoint (i, p);
          dx = point[0] - p[0];
          dy = point[1] - p[1];
          dz = point[2] - p[2];
          r = sqrt (dx * dx + dy * dy + dz * dz);
          U = phi (r * invSigma);
          w = &W[3 * i];
          vtkMatrixScalarMultiplyandAdd (interpoint, w, 3 * U, 3, 3);
        }
      
      // finish off with the affine transformation
      for (i = 0; i < 3; i++)	
        {
          w = &A[3 * i];
          vtkMatrixScalarMultiplyandAdd (interpoint, w, 3 * point[i], 3, 3);
        }
      vtkMatrixScalarMultiplyandAdd (interpoint, C, 3.0, 3, 3);
      
      // get the mean vector for the output point.
      x = (interpoint[0][0] + interpoint[1][0] + interpoint[2][0]) / 3;
      y = (interpoint[0][1] + interpoint[1][1] + interpoint[2][1]) / 3;
      z = (interpoint[0][2] + interpoint[1][2] + interpoint[2][2]) / 3;
      output[0] = x;
      output[1] = y;
      output[2] = z;
      vtkDeleteMatrix (interpoint);
    }
}

void
vtkThinPlateSplineAproximationTransform::
ForwardTransformPoint (const double point[3], double output[3]) 
{
  vtkThinPlateSplineForwardTransformPoint (this, this->MatrixW,
                                           this->NumberOfPoints,
                                           this->BasisFunction, point,
                                           output);
}

void
vtkThinPlateSplineAproximationTransform::
ForwardTransformPoint (const float point[3], float output[3]) 
{
  vtkThinPlateSplineForwardTransformPoint (this, this->MatrixW,
                                           this->NumberOfPoints,
                                           this->BasisFunction, point,
                                           output);
} 

//------------------------------------------------------------------------
vtkPoints * vtkThinPlateSplineAproximationTransform::
TransformPoinData (vtkPoints *  sourcepoint) 
{
  vtkPoints * result = vtkPoints::New ();
  double point[3], output[3];
  this->InternalUpdate ();
  vtkIdType num, i;
  num = sourcepoint->GetNumberOfPoints ();
  for (i = 0; i < num; i++)    
    {
      sourcepoint->GetPoint (i, point);
      ForwardTransformPoint (point, output);
      result->InsertPoint (i, output);
    }
  return result;
}


//------------------------------------------------------------------
void vtkThinPlateSplineAproximationTransform::PrintMatrixW () 
{
  int num;
  if (this->NumberOfPoints <= 3)
    num = this->NumberOfPoints + 3 + 1;
  
  else
    num = 3 * (this->NumberOfPoints + 3 + 1);
  ofstream out;
  out.open ("MatrixW1");
  for (int i = 0; i < num; i++)    
    {
      out << this->MatrixW[i][0] << "\t";
      out << this->MatrixW[i][1] << "\t";
      out << this->MatrixW[i][2] << "\n";
    } 
  out.close ();
} 

//----------------------------------------------------------------------------
// calculate the thin plate spline as well as the jacobian
template < class T > inline void
vtkThinPlateSplineForwardTransformDerivative
(
vtkThinPlateSplineAproximationTransform * self, double **W, int N,
 double (*phi) (double, double &), const T point[3], T output[3],
 T derivative[3][3]) 
{
  if (N == 0)    
    {
      for (int i = 0; i < 3; i++)	
        {
          output[i] = point[i];
          derivative[i][0] = derivative[i][1] = derivative[i][2] = 0.0;
          derivative[i][i] = 1.0;
        } 
      return;
    }
  double *C = W[N];
  double **A = &W[N + 1];
  double dx, dy, dz;
  double p[3];
  double r, U, f, Ux, Uy, Uz;
  double x = 0, y = 0, z = 0;
  double invSigma = 1.0 / self->GetSigma ();
  derivative[0][0] = derivative[0][1] = derivative[0][2] = 0;
  derivative[1][0] = derivative[1][1] = derivative[1][2] = 0;
  derivative[2][0] = derivative[2][1] = derivative[2][2] = 0;
  vtkPoints * sourceLandmarks = self->GetSourceLandmarks ();
  
  // do the nonlinear stuff
  for (vtkIdType i = 0; i < N; i++)    
    {
      sourceLandmarks->GetPoint (i, p);
      dx = point[0] - p[0];
      dy = point[1] - p[1];
      dz = point[2] - p[2];
      r = sqrt (dx * dx + dy * dy + dz * dz);
      
      // get both U and its derivative and do the sigma-mangling
      U = 0;
      f = 0;
      if (r != 0)	
        {
          U = phi (r * invSigma, f);
          f *= invSigma / r;
        }
      Ux = f * dx;
      Uy = f * dy;
      Uz = f * dz;
      x += U * W[i][0];
      y += U * W[i][1];
      z += U * W[i][2];
      derivative[0][0] += Ux * W[i][0];
      derivative[0][1] += Uy * W[i][0];
      derivative[0][2] += Uz * W[i][0];
      derivative[1][0] += Ux * W[i][1];
      derivative[1][1] += Uy * W[i][1];
      derivative[1][2] += Uz * W[i][1];
      derivative[2][0] += Ux * W[i][2];
      derivative[2][1] += Uy * W[i][2];
      derivative[2][2] += Uz * W[i][2];
    }
  
  // finish with the affine transformation
  x += C[0] + point[0] * A[0][0] + point[1] * A[1][0] + point[2] * A[2][0];
  y += C[1] + point[0] * A[0][1] + point[1] * A[1][1] + point[2] * A[2][1];
  z += C[2] + point[0] * A[0][2] + point[1] * A[1][2] + point[2] * A[2][2];
  output[0] = x;
  output[1] = y;
  output[2] = z;
  derivative[0][0] += A[0][0];
  derivative[0][1] += A[1][0];
  derivative[0][2] += A[2][0];
  derivative[1][0] += A[0][1];
  derivative[1][1] += A[1][1];
  derivative[1][2] += A[2][1];
  derivative[2][0] += A[0][2];
  derivative[2][1] += A[1][2];
  derivative[2][2] += A[2][2];
}

void
vtkThinPlateSplineAproximationTransform::
ForwardTransformDerivative (const double point[3], double output[3],
			    double derivative[3][3]) 
{
  vtkThinPlateSplineForwardTransformDerivative (this, this->MatrixW,
                                                this->NumberOfPoints,
                                                this->BasisDerivative,
                                                point, output, derivative);
} 

void
vtkThinPlateSplineAproximationTransform::
ForwardTransformDerivative (const float point[3], float output[3],
                            float derivative[3][3]) 
{
  vtkThinPlateSplineForwardTransformDerivative (this, this->MatrixW,
                                                this->NumberOfPoints,
                                                this->BasisDerivative,
                                                point, output, derivative);
} 

//------------------------------------------------------------------------
void vtkThinPlateSplineAproximationTransform::
PrintSelf (ostream & os, vtkIndent indent) 
{
  this->Superclass::PrintSelf (os, indent);
  os << indent << "Sigma: " << this->Sigma << "\n";
  os << indent << "Basis: " << this->GetBasisAsString () << "\n";
  os << indent << "Source Landmarks: " << this->SourceLandmarks << "\n";
  if (this->SourceLandmarks)    
    {
      this->SourceLandmarks->PrintSelf (os, indent.GetNextIndent ());
    }
  os << indent << "Target Landmarks: " << this->TargetLandmarks << "\n";
  if (this->TargetLandmarks)    
    {
      this->TargetLandmarks->PrintSelf (os, indent.GetNextIndent ());
    }
}


//----------------------------------------------------------------------------
vtkAbstractTransform * vtkThinPlateSplineAproximationTransform::
MakeTransform() 
{
  return vtkThinPlateSplineAproximationTransform::New ();
}


//----------------------------------------------------------------------------
void vtkThinPlateSplineAproximationTransform::
InternalDeepCopy (vtkAbstractTransform * transform) 
{
  vtkThinPlateSplineAproximationTransform * t =
    (vtkThinPlateSplineAproximationTransform *) transform;
  this->SetInverseTolerance (t->InverseTolerance);
  this->SetInverseIterations (t->InverseIterations);
  this->SetSigma (t->Sigma);
  this->SetBasis (t->GetBasis ());
  this->SetSourceLandmarks (t->SourceLandmarks);
  this->SetTargetLandmarks (t->TargetLandmarks);
  if (this->InverseFlag != t->InverseFlag)    
    {
      this->InverseFlag = t->InverseFlag;
      this->Modified ();
    }
}

////////////////rola adddd
void vtkThinPlateSplineAproximationTransform::Setvertcoord(double thex, double they, double thez)
{
	vertcoord = new double[3];
	vertcoord[0] = thex;
	vertcoord[1] = they;
	vertcoord[2] = thez;
}

/*double vtkRBFRigidbody(double r)
{
	return 
}*/

//////////////end rola adddd
//------------------------------------------------------------------------
//a very basic radial basis function: VTK_RBF_R=1
double vtkRBFrApro (double r) 
{
  return r;
}
//calculate both phi(r) its derivative wrt r
double vtkRBFDRrApro (double r, double &dUdr) 
{
  dUdr = 1;
  return r;
}

//------------------------------------------------------------------------
//the standard 2D thin plate spline basis function: VTK_RBF_R2LOGR=2
double vtkRBFr2logrApro (double r) 
{
  if (r)        
      return r * r * log (r);  
  else        
      return 0;
}
//calculate both phi(r) its derivative wrt r
double vtkRBFDRr2logrApro (double r, double &dUdr) 
{
  if (r)    
    {
      double tmp = log (r);
      dUdr = r * (1 + 2 * tmp);
      return r * r * tmp;
    }  
  else    
    {
      dUdr = 0;
      return 0;
    }
}

//------------------------------------------------------------------------
// my custom functions. VTK_RBF_R3 = 3
double vtkRBFr3Apro (double r)
{
  if (r)
      return r * r * r;
  else
      return 0;
}
//calculate both phi(r) its derivative wrt r
double vtkRBFDRr3Apro (double r, double &dUdr)
{
  if (r)
    {
      double tmp = r*r;
      dUdr = 3*tmp;
      return r*tmp;
    }
  else
    {
      dUdr = 0;
      return 0;
    }
}
//------------------------------------------------------------------------
// my custom functions. VTK_RBF_R5 = 3  /** m=4, d=3 */
double vtkRBFr5Apro (double r)
{
  if (r)
      return r * r * r * r * r;
  else
      return 0;
}
//calculate both phi(r) its derivative wrt r
double vtkRBFDRr5Apro (double r, double &dUdr)
{
  if (r)
    {
      double tmp = r*r*r*r;
      dUdr = 5*tmp;
      return r*tmp;
    }
  else
    {
      dUdr = 0;
      return 0;
    }
}


void vtkThinPlateSplineAproximationTransform::SetBasis (int basis) 
{
  if (basis == this->Basis)    
    {
      return;
    }
  switch (basis)    
    {
    case VTK_RBF_CUSTOM:
      break;
    case VTK_RBF_R:
      this->BasisFunction = &vtkRBFrApro;
      this->BasisDerivative = &vtkRBFDRrApro;
      break;
    case VTK_RBF_R2LOGR:
      this->BasisFunction = &vtkRBFr2logrApro;
      this->BasisDerivative = &vtkRBFDRr2logrApro;
      break;
    case VTK_RBF_R3:
      this->BasisFunction = &vtkRBFr3Apro;
      this->BasisDerivative = &vtkRBFDRr3Apro;
      break;
    case VTK_RBF_R5:
      this->BasisFunction = &vtkRBFr5Apro;
      this->BasisDerivative = &vtkRBFDRr5Apro;
      break;
    default:
      vtkErrorMacro (<<"SetBasisFunction: Unrecognized basis function");
      break;
    }
  this->Basis = basis;
  this->Modified ();
}


//------------------------------------------------------------------------
const char *
vtkThinPlateSplineAproximationTransform::GetBasisAsString () 
{
  switch (this->Basis)    
    {
    case VTK_RBF_CUSTOM:
      return "Custom";
    case VTK_RBF_R:
      return "R";
    case VTK_RBF_R2LOGR:
      return "R2LogR";
    }
  return "Unknown";
}
