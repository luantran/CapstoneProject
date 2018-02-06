/*=========================================================================

Program:   Visualization Toolkit
Module:    $RCSfile: vtkThinPlateSplineAproximationTransform.h,v $
Language:  C++
Date:      $Date: 2004/05/22 15:30:39 $
Version:   $Revision: 0.99 $
Author:    Qinghu Liao
E-mail:    qing-hu.liao@polymtl.ca

=========================================================================*/  

// .NAME vtkThinPlateSplineAproximationTransform - a nonlinear warp Aproximation
//   transformation 

// .SECTION Description
// vtkThinPlateSplineAproximationTransform describes a nonlinear warp
// Aproximation transform defined by a set of source and target landmarks.
// Any point on the mesh close to a source landmark will be moved to a place
// close to the corresponding target landmark. The points in between are
// interpolated smoothly using Bookstein's Thin Plate Spline algorithm. while
// we should think about the error of landmark, see the paper. 
// "Landmakr-Based Elastic Registration Using Approximating Thin-Plate
//  Splines",K.Rohr, IEEE TRANSACTION ON MEDICAL IMAGING, VOL,20, NO.6
// JUNE2001.  we treat with the ISOTROPIC and ANISOTROPIC error.

// .SECTION Caveats
// 1) The inverse transform is calculated using an iterative method,
// and is several times more expensive than the forward transform.
// 2) Whenever you add, subtract, or set points you must call Modified()
// on the vtkPoints object, or the transformation might not update.
// 3) Collinear point configurations (except those that lie in the XY plane)
// result in an unstable transformation.

// .SECTION see also
// vtkGridTransform vtkGeneralTransform vtkThinPlateSplineTransform
  

#ifndef __vtkThinPlateSplineAproximationTransform_h
#define __vtkThinPlateSplineAproximationTransform_h

#include <vtkThinPlateSplineTransform.h> 
#include <vtkWarpTransform.h>

/** The Following avariables are defined in 
     The vtkThinPlateSplineTransform.h file.
#define VTK_RBF_CUSTOM 0
#define VTK_RBF_R      1   (m=2, d=3 , at least 4 landmarks ) 
#define VTK_RBF_R2LOGR 2   (m=2, d=2 , at least 3 landmarks  )

*/

// ** The new kernel */
#define VTK_RBF_R3  3 /** m=3, d=3, at least 10 landmarks */
#define VTK_RBF_R5  4 /** m=4, d=3 , at least 20 landmarks */


          
class VTK_HYBRID_EXPORT vtkThinPlateSplineAproximationTransform:public
vtkWarpTransform
 
{

 public:
  vtkTypeRevisionMacro (vtkThinPlateSplineAproximationTransform,
                        vtkWarpTransform);  
  void PrintSelf (ostream & os, vtkIndent indent);  
  static vtkThinPlateSplineAproximationTransform *New ();
  
  // Description: 
  // Specify the 'stiffness' of the spline. The default is 1.0.
  vtkGetMacro (Sigma, double);  
  vtkSetMacro (Sigma, double);  

  // Description: 
  // Specify the 'Tradeoff' between the interpolating thin-plate spline
  // transformation and an approximating affine transformation. 
  // The Default is 50
  vtkGetMacro (Tradeoff, double);  
  vtkSetMacro (Tradeoff, double);  

  // Description: 
  // Specify the landmark error for the isotropic case.
  void SetSourceScalarVariance (int num, double V);  
  void SetTargetScalarVariance (int num, double V);
  
  // Description: 
  // Specify the landmark error for the anisotropic case.
  void SetSourceCoVariance (int num, double Vx, double Vy, double Vz);  
  void SetTargetCoVariance (int num, double Vx, double Vy, double Vz);  

  // Description:
  // Specify the radial basis function to use.  The default is
  // R2LogR which is what most people use as the thin plate spline.
  void SetBasis (int basis);  

  vtkGetMacro (Basis, int);  
  void SetBasisToR ()
    {
      this->SetBasis (VTK_RBF_R);
    };  
  void SetBasisToR2LogR ()
    {
      this->SetBasis (VTK_RBF_R2LOGR);
    };  
  const char *GetBasisAsString ();  
  //Description:
  // Get the matrixW for debug.
  void PrintMatrixW ();
  
  //==================================
  // Directly Transform PointData here,
  // because I don't understand the complex strcure of the VTK.
  vtkPoints * TransformPoinData (vtkPoints * sourcepoint);
  

  //BTX
  // Description:
  // Set the radial basis function to a custom function.  You must
  // supply both the function and its derivative with respect to r.
  void SetBasisFunction (double (*U) (double r))
    {    
      if (this->BasisFunction == U)
        {
          return;
        }    
      this->SetBasis (VTK_RBF_CUSTOM);    
      this->BasisFunction = U;    
      this->Modified ();
    };
  
  void SetBasisDerivative (double (*dUdr) (double r, double &dU))
    {    
      this->BasisDerivative = dUdr;    
      this->Modified ();
    };
  
  //ETX    
  // Description:
  // Set the source landmarks for the warp.  If you add or change the
  // vtkPoints object, you must call Modified() on it or the transformation
  // might not update.
  void SetSourceLandmarks (vtkPoints * source);  
  vtkGetObjectMacro (SourceLandmarks, vtkPoints);
  
  // Description:
  // Set the target landmarks for the warp.  If you add or change the
  // vtkPoints object, you must call Modified() on it or the transformation
  // might not update.
  void SetTargetLandmarks (vtkPoints * target);  
  vtkGetObjectMacro (TargetLandmarks, vtkPoints); 
  // Description:
  // Get the MTime.
  unsigned long GetMTime ();  
  // Description:
  // Make another transform of the same type.
  vtkAbstractTransform * MakeTransform ();  
  int NumberOfPoints;  
  double **MatrixW;  
  /*   int TransType; */ 

 protected:
  vtkThinPlateSplineAproximationTransform ();  
  ~vtkThinPlateSplineAproximationTransform ();
  
  // Description:
  // Prepare the transformation for application.
  void InternalUpdate ();
  
  // Description:
  // This method does no type checking, use DeepCopy instead.
  void InternalDeepCopy (vtkAbstractTransform * transform);
  void ForwardTransformPoint (const float in[3], float out[3]);  
  void ForwardTransformPoint (const double in[3], double out[3]); 
  void ForwardTransformDerivative (const float in[3], float out[3],     
                                   float derivative[3][3]);  
  void ForwardTransformDerivative (const double in[3], double out[3],    
                                   double derivative[3][3]);
  
  double Sigma;
  double Tradeoff;
  double **SourceScalarVariance;
  double **TargetScalarVariance;
  double ***SourceCoVariance;  
  double ***TargetCoVariance;  
  vtkPoints * SourceLandmarks;  
  vtkPoints * TargetLandmarks;  

  // the radial basis function to use
  double (*BasisFunction) (double r);  
  double (*BasisDerivative) (double r, double &dUdr);  

  int Basis;

  void Setvertcoord(double thex, double they, double thez);
  double *vertcoord;
 private:
  vtkThinPlateSplineAproximationTransform (const
                 vtkThinPlateSplineAproximationTransform&);
  // Not implemented.
  void operator= (const vtkThinPlateSplineAproximationTransform &);	
  // Not implemented.
};


#endif  /* */
	
  




