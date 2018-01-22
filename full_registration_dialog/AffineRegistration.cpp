#include "stdafx.h"
#include "AffineRegistration.h"
#include <string>
 #include <math.h>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
//#include "D:/segmentation/vtkSuperPixelSegmentation.h"
#include <string>

 #define TINY 1.0e-20; 
#define PI 3.14159265

#define BUF_SIZE 256

#ifndef _CRT_SECURE_NO_DEPRECATE
#  define _CRT_SECURE_NO_DEPRECATE
 #endif 

//===vtk===========
//#include <vtkActor.h>
#include <vtkActorCollection.h>
#include <vtkAppendPolyData.h>
#include <vtkCellLocator.h>
#include <vtkCellData.h>
#include <vtkCell.h>
#include <vtkCleanPolyData.h>
#include <vtkContourFilter.h>
#include <vtkDataArray.h>
#include <vtkDataSetReader.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkDelaunay3D.h>
#include <vtkDelaunay2D.h>
#include <vtkDecimatePro.h>
#include <vtkFloatArray.h>
#include <vtkFeatureEdges.h>
#include <vtkGeometryFilter.h>
#include <vtkGeneralTransform.h>
#include <vtkGenericCell.h>
#include <vtkLandmarkTransform.h>
#include <vtkIdList.h>
#include <vtkSystemIncludes.h>
#include <vtkIntArray.h>
#include <vtkImageData.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkMergePoints.h>
#include <vtkMeshQuality.h>
#include <vtkOBBTree.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPointData.h>
#include <vtkPointLocator.h>
#include <vtkPoints.h>
#include <vtkRenderer.h>
#include <vtkReverseSense.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkSTLReader.h>
//#include <vtkSTLWriter.h>
#include <vtkLoopSubdivisionFilter.h>
#include <vtkSurfaceReconstructionFilter.h>
//#include <vkThinPlateSplineTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangleFilter.h>
#include <vtkTetra.h>
#include <vtkVRMLImporter.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkVRMLExporter.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkproperty.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkGlyph3D.h>
#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkDataArray.h>
#include <vtkFloatArray.h>
#include <vtkLight.h>
#include <vtkProperty.h>
#include <vtkTextMapper.h>
#include <vtkTextActor.h>
#include <vtkFollower.h>
#include <vtkVectorText.h>
#include <vtkTextProperty.h>
#include <vtkAxes.h>
#include <vtkExtractGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkTriangleFilter.h>
//#include <vtkDecimate.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkCellPicker.h>
#include <vtkImagePlaneWidget.h>
#include <vtkStructuredPoints.h>
#include <vtkUnstructuredGrid.h>
//reading mri volume - testing
#include <vtkVolume16Reader.h>
//#include <vtkOutlineFilter.h>
#include <vtkProperty.h>
#include <vtkPolyDataNormals.h>
#include <vtkContourFilter.h>
#include <vtkCamera.h>
#include <vtkShrinkFilter.h>
#include <vtkImageReslice.h>
#include <vtkPlaneSource.h>
#include <vtkExtractVOI.h>
#include <vtkImageViewer.h>
#include <vtkQuadricClustering.h>
#include <vtkImagePlaneWidget.h>
#include <vtkCellPicker.h> 
//#include <vtk/vtkGdcmReader.h>
//#include <vtk/vtkDICOMImageReader.h>
//#include <vtkPlanes.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkColorTransferFunction.h>
#include <vtkImplicitModeller.h>
#include <vtkImageCast.h>
#include <vtkImageEuclideanDistance.h>
#include <vtkImageGaussianSmooth.h>
#include "vtkGridTransform.h"
#include "vtkShortArray.h"
#include "vtkPNMReader.h" 
#include <cmath>




//======================
#include <algorithm>
#include <iterator>
#include <list>
#include <functional>
#include <stdio.h>
#include <fstream>
#include <iostream>
 #include <sstream>
#include <stdlib.h>

//to segment the MRI
/*#include "D:/segment/segmentmri/segmentmri/image.h"
#include "D:/segment/segmentmri/segmentmri/segment-image.h"

#include "D:/segment/segmentmri/segmentmri/misc.h"
#include "D:/segment/segmentmri/segmentmri/pnmfile.h"*/



using namespace boost::numeric::ublas;
typedef permutation_matrix<std::size_t> pmatrix;

bool AffineRegistration::CutMRIarms( vtkImageData* movingimage, int slicenumber,const char *filename)
{
		int imageextent[6];
		movingimage->GetExtent(imageextent);
		int newmri_dimz=imageextent[5];
		int newmri_dimy=imageextent[3];
		int newmri_dimx=imageextent[1];


			vtkShortArray *scalarsMRI= vtkShortArray::New();
	scalarsMRI->SetNumberOfComponents(3);
	scalarsMRI = (vtkShortArray *)(movingimage->GetPointData()->GetScalars());

	//open segmentation file
	vtkImageData* MRIbinary = vtkImageData::New();
	MRIbinary = loadPPM(filename);
	vtkShortArray *scalarsbinary= vtkShortArray::New();
	scalarsbinary->SetNumberOfComponents(3);
	scalarsbinary = (vtkShortArray *)(MRIbinary->GetPointData()->GetScalars());
	
	//go through slice and remove pixels
	for (int i=0;i<=imageextent[3];i++)
	{
		for (int j=0;j<=imageextent[1];j++)
		{
				int offsetbinary = (int)((newmri_dimx+1)*i+j);

			  double *themid = new double[3];
			  themid =  scalarsbinary->GetTuple3(offsetbinary); 

			 if((short)themid[0]<10)
			 {
				 //set scalars to 0
				 int offsetMRI = (int)(((newmri_dimy+1)*(newmri_dimx+1)*(slicenumber))+(newmri_dimx+1)*(int)i+(int)j);
				 scalarsMRI->SetTuple1(offsetMRI,0);
				//float MRIval = scalarsMRI->GetTuple1(offsetMRI);
				
			 }
		
		}
	}

	//set image to new scalar data
	movingimage->GetPointData()->SetScalars(scalarsMRI);

	return true;

}

bool AffineRegistration::GenerateMRISlices(vtkImageData* movingimage, int slicenumber,const char *filename)
{

				//just for segmentation, save resampled file and exit
			savePPM(movingimage, filename, slicenumber);
			return true;
}

bool AffineRegistration::GenerateonlyMRIContourPoints(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *MRIVert, vtkMatrix4x4 *articulatedtx, vtkPolyData *mriSlicePolyData,const char *filename)
{

	//NOTE: here the 
	int angleinterval=10; //10 worked till 8. 15 worked for the upper slices, needed 20 for som eproblematic ones with inverse issues
				double *bounds = new double[6];

	vtkPoints *mriSlicePoints = vtkPoints::New();
	vtkPoints *topoSlicePoints = vtkPoints::New();
	vtkShortArray *scalarsMRI = vtkShortArray::New();
	
	
		double *worldMRIcenter = new double[3];
		worldMRIcenter[0]=(MRIVert->GetElement(0,3));
		worldMRIcenter[1]=(MRIVert->GetElement(1,3));
		worldMRIcenter[2]=(MRIVert->GetElement(2,3));

		int imageextent[6];
		movingimage->GetExtent(imageextent);
		int newmri_dimz=imageextent[5];
		int newmri_dimy=imageextent[3];
		int newmri_dimx=imageextent[1];


		//get the values of volumes into scalars
		for (int i=0;i<newmri_dimz*newmri_dimy*newmri_dimx;i++)
				scalarsMRI->InsertNextTuple1(0);
		scalarsMRI = (vtkShortArray *)(movingimage->GetPointData()->GetScalars());



		//get the image coordinates of the vertebral center as a starting point for both MRI and TP
			double *paraCoords = new double[3]; 
			double *paraCoords4 = new double[3]; 
			double *xyzCoords  = new double[3]; 
			double *vtkcellweights = new double[4]; 
			double *vtkcellweights4 = new double[4];

			float *MRIcenter  = new float[3]; 
			float *TPcenter  = new float[3]; 

 
			double mri1origin[3];
			movingimage->GetOrigin(mri1origin);
			int theimagecoordMRI[3];

			
			float previousdist=1000000000;

			/* segmentation parameters		
			superPixelSegmentation->SetSigma(.5);
			superPixelSegmentation->SetK(500);
			superPixelSegmentation->SetMinSize(50);
			*/

						//find vertebral center coordinates in untransformed MRI slice 
			movingimage->ComputeStructuredCoordinates(worldMRIcenter,theimagecoordMRI,paraCoords);

			//otherwise load segmented MRI
			vtkImageData* MRIbinary = vtkImageData::New();
			MRIbinary = loadPPM(filename);
			vtkShortArray *scalarsbinary= vtkShortArray::New();
			scalarsbinary->SetNumberOfComponents(3);
			scalarsbinary = (vtkShortArray *)(MRIbinary->GetPointData()->GetScalars());
	 	    int offsetbinary = (int)((newmri_dimx+1)*(int)theimagecoordMRI[1]+(int)theimagecoordMRI[0]);


			//the image as is has no transformation. thus transform, but the above no longer applies

			  double *themid = new double[3];
			  double *mricurrentpoint = new double[3];
			 themid =  scalarsbinary->GetTuple3(offsetbinary); 
			  short midr = (short)themid[0];
			 short midg = (short)themid[1];
			 short midb = (short)themid[2];
		
			 			//get the world coordinates of the MRI center by transforming world of structures (thexMRI, theyMRI, slicenumber)

			int offsetMRItemp = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theimagecoordMRI[1]+(int)theimagecoordMRI[0]);
			movingimage->GetCell(offsetMRItemp)->GetBounds(bounds);
			vtkPoints *MRIworldcernterpoint = vtkPoints::New();
			MRIworldcernterpoint->InsertNextPoint(bounds[0], bounds[2], bounds[4]);
			vtkPolyData *MRIworldcernterdata = vtkPolyData::New();
			MRIworldcernterdata->SetPoints(MRIworldcernterpoint);
			vtkTransform *articulatedImageTransform=vtkTransform::New();
		articulatedImageTransform->SetMatrix(articulatedtx);
		articulatedImageTransform->Update();
		vtkTransformPolyDataFilter *middletransfilter = vtkTransformPolyDataFilter::New ();
		middletransfilter->SetInput (MRIworldcernterdata); //original
		middletransfilter->SetTransform (articulatedImageTransform);
		middletransfilter->Update ();
		MRIworldcernterdata=middletransfilter->GetOutput();
		MRIworldcernterdata->DeepCopy(middletransfilter->GetOutput());

		for (double theangle2=0;theangle2<360;theangle2+=angleinterval)
		{
			double theangle = (theangle2*PI/180);

			//get the MRI index
			/////need index of center in MRI volume coordinates...
			double thexMRI=(double)theimagecoordMRI[0];//+40*cos(theangle);
			double theyMRI=(double)theimagecoordMRI[1];//+40*sin(theangle);
			double thexMRInext = thexMRI+cos(theangle); double theyMRInext= theyMRI+(sin(theangle));
			double thexMRInext2 = thexMRI+3*cos(theangle); double theyMRInext2= theyMRI+3*(sin(theangle));

			int offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			int offsetMRInext = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)(theyMRInext)+(int)(thexMRInext));
			int offsetMRInext2 = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)(theyMRInext2)+(int)(thexMRInext2));
			float MRIval = scalarsMRI->GetTuple1(offsetMRI);
			float MRIvalnext = scalarsMRI->GetTuple1(offsetMRInext);
			float MRIvalnext2 = scalarsMRI->GetTuple1(offsetMRInext2);

			//get the value of the MRI and topo



			int ii=0,jj=0,kk=0;
		

			thexMRI=(double)theimagecoordMRI[0];//+40*cos(theangle)
			theyMRI=(double)theimagecoordMRI[1];//+40*sin(theangle)
			thexMRInext = thexMRI+cos(theangle);  theyMRInext= theyMRI+(sin(theangle));
			thexMRInext2 = thexMRI+3*cos(theangle);  theyMRInext2= theyMRI+3*(sin(theangle));

			 offsetbinary = (int)((newmri_dimx+1)*(int)theyMRI+(int)thexMRI);
		 	 mricurrentpoint =  scalarsbinary->GetTuple3(offsetbinary);
			short r = mricurrentpoint[0];
			 short g = mricurrentpoint[1];
			 short b = mricurrentpoint[2];

			while((thexMRI>4) && (theyMRI>4) && (theyMRI<(newmri_dimy-4)) && (thexMRI<(newmri_dimx-4)) && (r == midr) && (g == midg) && (b == midb))
			{
				thexMRI+=cos(theangle);
				theyMRI+=sin(theangle);

				offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			movingimage->GetCell(offsetMRI)->GetBounds(bounds);
			 offsetbinary = (int)((newmri_dimx+1)*(int)theyMRI+(int)thexMRI);//was workin uptil today, mebbe still is... 18/02
			// offsetbinary = (int)((newmri_dimx)*(int)theyMRI+(int)thexMRI);
		 	 mricurrentpoint =  scalarsbinary->GetTuple3(offsetbinary); //size 786429
			 r = mricurrentpoint[0];
			 g = mricurrentpoint[1];
			 b = mricurrentpoint[2];

			}


			offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);

			movingimage->GetCell(offsetMRI)->GetBounds(bounds);
			mriSlicePoints->InsertNextPoint(bounds[0],bounds[2],bounds[4]); 
		}

		mriSlicePolyData->SetPoints(mriSlicePoints); 

		return TRUE;
		}

bool AffineRegistration::GeneratenewRegistrationPoints(vtkPolyData *TPpoly, vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *MRIVert, vtkMatrix4x4 *XrayVert, vtkMatrix4x4 *articulatedtx,vtkPolyData *topoSlicePolyData,vtkPolyData *mriSlicePolyData,const char *filename, const char *filenameTP)
{

	//NOTE: here the 
	int angleinterval=10; //10 worked till 8. 15 worked for the upper slices, needed 20 for som eproblematic ones with inverse issues
				double *bounds = new double[6];
	//transform polydata
/*	vtkGeneralTransform *tnoreg44 = vtkGeneralTransform::New ();
	tnoreg44->RotateZ(-90.0); //invert the body back
	tnoreg44->RotateX(90.0);
	vtkTransformPolyDataFilter *f44 = vtkTransformPolyDataFilter::New ();
	f44->SetInput (TPpoly);
	f44->SetTransform (tnoreg44);
	f44->Update ();
	vtkPolyData *polydatasze=f44->GetOutput();*/

	//transform into MRI space
	vtkPolyData *tildtedTopo = vtkPolyData::New();
	
	vtkTransform *toporevtx=vtkTransform::New();
	toporevtx->SetMatrix(articulatedtx); //inverse of mriregvol, mriregvol I guess is good for polydata but not images...
	toporevtx->Inverse(); //test without just for the hell of it.
	toporevtx->Update();


		//appkly tx applied in howtopo, then apply art, then apply inverse show
		vtkTransform *noregtopotx = vtkTransform::New();
		//noregtopotx->PostMultiply();
		noregtopotx->RotateZ(-90); 
		noregtopotx->RotateX(90); 
		noregtopotx->Update();

		vtkTransform *noregtopotxinv = vtkTransform::New();
		//noregtopotxinv->PostMultiply();
		noregtopotxinv->RotateZ(-90); 
		noregtopotxinv->RotateX(90); 
		noregtopotxinv->Inverse();
		noregtopotxinv->Update();

		vtkTransformPolyDataFilter *fpre = vtkTransformPolyDataFilter::New ();
		fpre->SetInput (TPpoly);
		fpre->SetTransform (noregtopotx);
		fpre->Update ();
		tildtedTopo=fpre->GetOutput();

		vtkTransformPolyDataFilter *ftilted = vtkTransformPolyDataFilter::New ();
		ftilted->SetInput (tildtedTopo);
		ftilted->SetTransform (toporevtx);
		ftilted->Update ();
		tildtedTopo=ftilted->GetOutput();

		vtkTransformPolyDataFilter *fpost = vtkTransformPolyDataFilter::New ();
		fpost->SetInput(tildtedTopo);
		fpost->SetTransform (noregtopotxinv);
		fpost->Update ();
		tildtedTopo=fpost->GetOutput();

			vtkPoints *TPtempopints = vtkPoints::New();
	vtkGeneralTransform *tnoreg44 = vtkGeneralTransform::New ();
	tnoreg44->RotateZ(-90.0); //invert the body back
	tnoreg44->RotateX(90.0);
	vtkTransformPolyDataFilter *f444 = vtkTransformPolyDataFilter::New ();
	f444->SetInput (tildtedTopo);
	f444->SetTransform (tnoreg44);
	f444->Update ();
	vtkPolyData *polydatasze=f444->GetOutput();

	vtkCellLocator *cellLocatorreg = vtkCellLocator::New();
	cellLocatorreg->SetDataSet(polydatasze);//(polydatasze);
	cellLocatorreg->BuildLocator();
	vtkPoints *mriSlicePoints = vtkPoints::New();
	vtkPoints *topoSlicePoints = vtkPoints::New();
	vtkShortArray *scalarsMRI = vtkShortArray::New();
	
	
		double *worldMRIcenter = new double[3];
		worldMRIcenter[0]=(MRIVert->GetElement(0,3));
		worldMRIcenter[1]=(MRIVert->GetElement(1,3));
		worldMRIcenter[2]=(MRIVert->GetElement(2,3));

		int imageextent[6];
		movingimage->GetExtent(imageextent);
		int newmri_dimz=imageextent[5];
		int newmri_dimy=imageextent[3];
		int newmri_dimx=imageextent[1];


		//get the values of volumes into scalars
		for (int i=0;i<newmri_dimz*newmri_dimy*newmri_dimx;i++)
				scalarsMRI->InsertNextTuple1(0);
		scalarsMRI = (vtkShortArray *)(movingimage->GetPointData()->GetScalars());



		//get the image coordinates of the vertebral center as a starting point for both MRI and TP
			double *paraCoords = new double[3]; 
			double *paraCoords4 = new double[3]; 
			double *xyzCoords  = new double[3]; 
			double *vtkcellweights = new double[4]; 
			double *vtkcellweights4 = new double[4];

			float *MRIcenter  = new float[3]; 
			float *TPcenter  = new float[3]; 

 
			double mri1origin[3];
			movingimage->GetOrigin(mri1origin);
			int theimagecoordMRI[3];

			
			float previousdist=1000000000;

			/* segmentation parameters		
			superPixelSegmentation->SetSigma(.5);
			superPixelSegmentation->SetK(500);
			superPixelSegmentation->SetMinSize(50);
			*/

						//find vertebral center coordinates in untransformed MRI slice 
			movingimage->ComputeStructuredCoordinates(worldMRIcenter,theimagecoordMRI,paraCoords);

			//otherwise load segmented MRI
			vtkImageData* MRIbinary = vtkImageData::New();
			MRIbinary = loadPPM(filename);
			vtkShortArray *scalarsbinary= vtkShortArray::New();
			scalarsbinary->SetNumberOfComponents(3);
			scalarsbinary = (vtkShortArray *)(MRIbinary->GetPointData()->GetScalars());
	 	    int offsetbinary = (int)((newmri_dimx+1)*(int)theimagecoordMRI[1]+(int)theimagecoordMRI[0]);


			//the image as is has no transformation. thus transform, but the above no longer applies

			  double *themid = new double[3];
			  double *mricurrentpoint = new double[3];
			 themid =  scalarsbinary->GetTuple3(offsetbinary); 
			  short midr = (short)themid[0];
			 short midg = (short)themid[1];
			 short midb = (short)themid[2];
		
			 			//get the world coordinates of the MRI center by transforming world of structures (thexMRI, theyMRI, slicenumber)

			int offsetMRItemp = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theimagecoordMRI[1]+(int)theimagecoordMRI[0]);
			movingimage->GetCell(offsetMRItemp)->GetBounds(bounds);
			vtkPoints *MRIworldcernterpoint = vtkPoints::New();
			MRIworldcernterpoint->InsertNextPoint(bounds[0], bounds[2], bounds[4]);
			vtkPolyData *MRIworldcernterdata = vtkPolyData::New();
			MRIworldcernterdata->SetPoints(MRIworldcernterpoint);
			vtkTransform *articulatedImageTransform=vtkTransform::New();
		articulatedImageTransform->SetMatrix(articulatedtx);
		articulatedImageTransform->Update();
		vtkTransformPolyDataFilter *middletransfilter = vtkTransformPolyDataFilter::New ();
		middletransfilter->SetInput (MRIworldcernterdata); //original
		middletransfilter->SetTransform (articulatedImageTransform);
		middletransfilter->Update ();
		MRIworldcernterdata=middletransfilter->GetOutput();
		MRIworldcernterdata->DeepCopy(middletransfilter->GetOutput());

		for (double theangle2=0;theangle2<360;theangle2+=angleinterval)
		{
			double theangle = (theangle2*PI/180);

			//get the MRI index
			/////need index of center in MRI volume coordinates...
			double thexMRI=(double)theimagecoordMRI[0];//+40*cos(theangle);
			double theyMRI=(double)theimagecoordMRI[1];//+40*sin(theangle);
			double thexMRInext = thexMRI+cos(theangle); double theyMRInext= theyMRI+(sin(theangle));
			double thexMRInext2 = thexMRI+3*cos(theangle); double theyMRInext2= theyMRI+3*(sin(theangle));

			int offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			int offsetMRInext = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)(theyMRInext)+(int)(thexMRInext));
			int offsetMRInext2 = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)(theyMRInext2)+(int)(thexMRInext2));
			float MRIval = scalarsMRI->GetTuple1(offsetMRI);
			float MRIvalnext = scalarsMRI->GetTuple1(offsetMRInext);
			float MRIvalnext2 = scalarsMRI->GetTuple1(offsetMRInext2);

			//get the value of the MRI and topo



			int ii=0,jj=0,kk=0;
		

			thexMRI=(double)theimagecoordMRI[0];//+40*cos(theangle)
			theyMRI=(double)theimagecoordMRI[1];//+40*sin(theangle)
			thexMRInext = thexMRI+cos(theangle);  theyMRInext= theyMRI+(sin(theangle));
			thexMRInext2 = thexMRI+3*cos(theangle);  theyMRInext2= theyMRI+3*(sin(theangle));

			 offsetbinary = (int)((newmri_dimx+1)*(int)theyMRI+(int)thexMRI);
		 	 mricurrentpoint =  scalarsbinary->GetTuple3(offsetbinary);
			short r = mricurrentpoint[0];
			 short g = mricurrentpoint[1];
			 short b = mricurrentpoint[2];

			while((thexMRI>4) && (theyMRI>4) && (theyMRI<(newmri_dimy-4)) && (thexMRI<(newmri_dimx-4)) && (r == midr) && (g == midg) && (b == midb))
			{
				thexMRI+=cos(theangle);
				theyMRI+=sin(theangle);

				offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			movingimage->GetCell(offsetMRI)->GetBounds(bounds);
			 offsetbinary = (int)((newmri_dimx+1)*(int)theyMRI+(int)thexMRI);//was workin uptil today, mebbe still is... 18/02
			// offsetbinary = (int)((newmri_dimx)*(int)theyMRI+(int)thexMRI);
		 	 mricurrentpoint =  scalarsbinary->GetTuple3(offsetbinary); //size 786429
			 r = mricurrentpoint[0];
			 g = mricurrentpoint[1];
			 b = mricurrentpoint[2];

			}


			offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);

			movingimage->GetCell(offsetMRI)->GetBounds(bounds);
			mriSlicePoints->InsertNextPoint(bounds[0],bounds[2],bounds[4]); 
		}

		mriSlicePolyData->SetPoints(mriSlicePoints); ////dont reg mri, dereg topo, well dereg MRI later...

		
		//now for the surface topography, this center not best idea. must be on slice
		double *worldXraycenter = new double[3];
		worldXraycenter=MRIworldcernterdata->GetPoints()->GetPoint(0);//(XrayVert->GetElement(0,3));
		//worldXraycenter[1]=//(XrayVert->GetElement(1,3));
		//worldXraycenter[2]=//(XrayVert->GetElement(2,3));

		double *articulatedAngle = new double[3];
		vtkTransform *articulatedImageTransform2=vtkTransform::New();
		articulatedImageTransform2->SetMatrix(articulatedtx);
		articulatedImageTransform2->Update();
		articulatedAngle = articulatedImageTransform2->GetOrientation();

		vtkFloatArray *mripointscalars = vtkFloatArray::New();
		mripointscalars = (vtkFloatArray *)mriSlicePolyData->GetPoints()->GetData();

		
		//get the edge of the moving image (0,0,0),(newmri_dimx,0,0),(0,newmri_dimy,0), (newmri_dimx, newmri_dimy,0)
	
		vtkImageData* TPbinary = vtkImageData::New();
		TPbinary = loadPPM(filenameTP);
		vtkShortArray *scalarsbinaryTP= vtkShortArray::New();
		scalarsbinaryTP->SetNumberOfComponents(3);
		scalarsbinaryTP = (vtkShortArray *)(TPbinary->GetPointData()->GetScalars());
		
		//first find the points in the same way the we found the MRI points
			double thexTP=(double)theimagecoordMRI[0];
			double theyTP=(double)theimagecoordMRI[1];
			 double *themidTP = new double[3];
			 offsetbinary = (int)((newmri_dimx+1)*(int)theyTP+(int)thexTP);
			 themidTP =  scalarsbinaryTP->GetTuple3(offsetbinary); 
			  short midrTP = (short)themidTP[0];
			 short midgTP = (short)themidTP[1];
			 short midbTP = (short)themidTP[2];
		
		for (double theangle2=0;theangle2<360;theangle2+=angleinterval)
		{
			double *theclosestpoint = new double[3];
			vtkIdType id6; int subID;
			double theangle = (theangle2*PI/180);
			double thexTP=(double)theimagecoordMRI[0];
			double theyTP=(double)theimagecoordMRI[1];
			int offsetTP = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyTP+(int)thexTP);
			offsetbinary = (int)((newmri_dimx+1)*(int)theyTP+(int)thexTP);
		 	 mricurrentpoint =  scalarsbinaryTP->GetTuple3(offsetbinary);
			short r = mricurrentpoint[0];
			 short g = mricurrentpoint[1];
			 short b = mricurrentpoint[2];
			 double currentDist= 999;

			while( (thexTP>4) && (theyTP>4) && (theyTP<(newmri_dimy-4)) && (thexTP<(newmri_dimx-4)) && (r == midrTP) && (g == midgTP) && (b == midbTP))
			{
				thexTP+=cos(theangle);
				theyTP+=sin(theangle);

				offsetTP = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyTP+(int)thexTP);
				offsetbinary = (int)((newmri_dimx+1)*(int)theyTP+(int)thexTP);
			
		 	 mricurrentpoint =  scalarsbinaryTP->GetTuple3(offsetbinary); //size 786429
			 r = mricurrentpoint[0];
			 g = mricurrentpoint[1];
			 b = mricurrentpoint[2];
			 movingimage->GetCell(offsetTP)->GetBounds(bounds);
			xyzCoords[0] = bounds[0];
			xyzCoords[1] = bounds[2];
			xyzCoords[2] = bounds[4];
			 cellLocatorreg->FindClosestPoint(xyzCoords ,theclosestpoint, id6, subID, currentDist);
			}


			offsetTP = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyTP+(int)thexTP);

			movingimage->GetCell(offsetTP)->GetBounds(bounds);
			topoSlicePoints->InsertNextPoint(bounds[0],bounds[2],bounds[4]); 
		}
		topoSlicePolyData->SetPoints(topoSlicePoints);

		//now transform the MRI points with the articulated model transformation
/*		vtkPolyData *tempdata = vtkPolyData::New();
		tempdata->SetPoints(mriSlicePoints);
		vtkTransform *articulatedImageTransformtemp=vtkTransform::New();
		articulatedImageTransformtemp->SetMatrix(articulatedtx);
		articulatedImageTransformtemp->Update();
		vtkTransformPolyDataFilter *fvertsurfacetemp = vtkTransformPolyDataFilter::New ();
		fvertsurfacetemp->SetInput (tempdata); //original
		fvertsurfacetemp->SetTransform (articulatedImageTransformtemp);
		fvertsurfacetemp->Update ();
		tempdata=fvertsurfacetemp->GetOutput();
		tempdata->DeepCopy(fvertsurfacetemp->GetOutput());
		mriSlicePolyData->SetPoints(tempdata->GetPoints());

		double prevpoint[3];
		for (double theangle2=0;theangle2<360;theangle2+=angleinterval)
		{

			
			double *theclosestpoint = new double[3];
			double *mripoint = new double[3];
			//first get the MRI point at that angle and find the dx,dy,dz to increment
			mripoint = mripointscalars->GetTuple3((int)(theangle2/(double)angleinterval));
			double dx = (mripoint[0]-worldXraycenter[0])/(3.0* abs(mripoint[0]-worldXraycenter[0]));
			double dy = (mripoint[1]-worldXraycenter[1])/(3.0*abs(mripoint[0]-worldXraycenter[0]));
			double dz = (mripoint[2]-worldXraycenter[2])/(3.0*abs(mripoint[0]-worldXraycenter[0]));

			//find the closest point on surface
			vtkIdType id6; int subID;
			double theangle = (theangle2*PI/180);
	
			xyzCoords[0] = worldXraycenter[0]+1.0*dx;
			xyzCoords[1] = worldXraycenter[1]+1.0*dy;
			xyzCoords[2] = worldXraycenter[2]+1.0*dz;//+sqrt(pow(10*cos(theangle),2)+pow(10*sin(theangle),2));
			vtkGenericCell *  	cell = vtkGenericCell::New();

			double previousDist = 1000, nextdist = 998,  previousDist2 = 1001, nextdist2 = 997,currentDist= 999, d;
			
			while((currentDist > 20)  && (abs(xyzCoords[1]) < abs( worldXraycenter[1]*9))) // *9 && (xyzCoords[0]>=leftextreme) && (xyzCoords[0] < rightextreme)  && (xyzCoords[1] >bottomextreme)  && (xyzCoords[1] <topextreme))// && ( ((currentDist>=nextdist) || (currentDist>=previousDist)) || ((currentDist>=nextdist2) ||(currentDist>=previousdist2))))// was 10, 15 for 7//&& ((TPval>=nextdist2)||(TPval>=previousdist2)))
			{
				previousdist2 = previousDist;
				previousDist = currentDist;
				currentDist = nextdist;
	
				xyzCoords[0]+= dx;xyzCoords[1]+=dy;xyzCoords[2]+=dz;//sqrt(pow(cos(theangle),2)+pow(sin(theangle),2));
				//cellLocatorreg->FindClosestPoint(xyzCoords ,theclosestpoint, id6, subID, nextdist2); //not good
				cellLocatorreg->FindClosestPoint(xyzCoords ,theclosestpoint, id6, subID, currentDist);

				//topoSlicePoints->InsertNextPoint(xyzCoords[0],xyzCoords[1],xyzCoords[2]);
			//	if (cellLocatorreg->FindClosestPointWithinRadius(xyzCoords,40,theclosestpoint, cell,id6, subID, currentDist) == 0)
				//	currentDist = 999;

			//	cellLocatorreg->FindClosestPoint(xyzCoords,theclosestpoint, cell,id6, subID, currentDist);
			//	if(theangle2>0)
			//	{
			//		if  (abs(theclosestpoint[0] - prevpoint[0])  > 100) 
			//			theclosestpoint[0] = prevpoint[0];
			//		if (abs(theclosestpoint[1] - prevpoint[1])  > 100)
			//			theclosestpoint[1] = prevpoint[1];
			//	}
		
			}	

			
			
			//find more close points and take the one with the closest z value instead
	
		//	if(currentDist = 999)
		//		topoSlicePoints->InsertNextPoint(xyzCoords[0],xyzCoords[1],xyzCoords[2]);
		//	else
		//		topoSlicePoints->InsertNextPoint(theclosestpoint[0],theclosestpoint[1],xyzCoords[2]);

				prevpoint[0]= theclosestpoint[0];
				prevpoint[1]= theclosestpoint[1];
				prevpoint[2]= theclosestpoint[2];
				topoSlicePoints->InsertNextPoint(xyzCoords[0],xyzCoords[1],xyzCoords[2]);//xyzCoords[2]);
		//	topoSlicePoints->InsertNextPoint(bounds[0],bounds[2],bounds[4]); //yellow, good...
		}

		//topoSlicePolyData->SetPoints(topoSlicePoints);
		
		//dereg topo
		vtkPolyData *tempdata3 = vtkPolyData::New();
		tempdata3->SetPoints(topoSlicePoints);
		vtkTransform *articulatedImageTransform4=vtkTransform::New();
		articulatedImageTransform4->SetMatrix(articulatedtx);
		articulatedImageTransform4->Inverse();
		articulatedImageTransform4->Update();
		vtkTransformPolyDataFilter *fvertsurface2 = vtkTransformPolyDataFilter::New ();
		fvertsurface2->SetInput (tempdata3); //original
		fvertsurface2->SetTransform (articulatedImageTransform4);
		fvertsurface2->Update ();
		tempdata3=fvertsurface2->GetOutput();
		tempdata3->DeepCopy(fvertsurface2->GetOutput());

		topoSlicePolyData->SetPoints(tempdata3->GetPoints());

		//now dereg mri
				vtkPolyData *tempdata2 = vtkPolyData::New();
				tempdata2->SetPoints(mriSlicePolyData->GetPoints());
		vtkTransform *articulatedImageTransform3=vtkTransform::New();
		articulatedImageTransform3->SetMatrix(articulatedtx);
		articulatedImageTransform3->Inverse();
		articulatedImageTransform3->Update();
		vtkTransformPolyDataFilter *fvertsurface3 = vtkTransformPolyDataFilter::New ();
		fvertsurface3->SetInput (tempdata2); //original
		fvertsurface3->SetTransform (articulatedImageTransform3);
		fvertsurface3->Update ();
		tempdata2=fvertsurface3->GetOutput();
		tempdata2->DeepCopy(fvertsurface3->GetOutput());

		mriSlicePolyData->SetPoints(tempdata2->GetPoints());*/
		return true;

}

bool AffineRegistration::GenerateRegistrationPoints(vtkPolyData *TPpoly, vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *XrayVert, vtkImageData * TPvolume,vtkPoints *topoSlicePoints,vtkPoints *mriSlicePoints,const char *filename, bool fullon)
{
	int angleinterval=20;
		vtkGeneralTransform *tnoreg44 = vtkGeneralTransform::New ();
	tnoreg44->RotateZ(-90.0); //invert the body back
	tnoreg44->RotateX(90.0);
	vtkTransformPolyDataFilter *f44 = vtkTransformPolyDataFilter::New ();
	f44->SetInput (TPpoly);
	f44->SetTransform (tnoreg44);
	f44->Update ();
	vtkPolyData *polydatasze=f44->GetOutput();

	vtkCellLocator *cellLocatorreg = vtkCellLocator::New();
	cellLocatorreg->SetDataSet(polydatasze);
	cellLocatorreg->BuildLocator();


		vtkShortArray *scalarsMRI = vtkShortArray::New();
		vtkFloatArray *scalarsTP = vtkFloatArray::New();
	
		double *worldcenter = new double[3];
		worldcenter[0]=(XrayVert->GetElement(0,3));
		worldcenter[1]=(XrayVert->GetElement(1,3));
		worldcenter[2]=(XrayVert->GetElement(2,3));

		int imageextent[6];
		movingimage->GetExtent(imageextent);
		int newmri_dimz=imageextent[5];
		int newmri_dimy=imageextent[3];
		int newmri_dimx=imageextent[1];

		double mri1origin[3];
		movingimage->GetOrigin(mri1origin);
		double current_voxel_location[3];

		//get the values of volumes into scalars
		for (int i=0;i<newmri_dimz*newmri_dimy*newmri_dimx;i++)
		{
				scalarsMRI->InsertNextTuple1(0);
				scalarsTP->InsertNextTuple1(0);
		}
		scalarsMRI = (vtkShortArray *)(movingimage->GetPointData()->GetScalars());
		scalarsTP = (vtkFloatArray *)(TPvolume->GetPointData()->GetScalars());

		//get the image coordinates of the vertebral center as a starting point for both MRI and TP
			double *paraCoords = new double[3]; 
			double *paraCoords4 = new double[3]; 
			double *xyzCoords  = new double[3]; 
			double *vtkcellweights = new double[4]; 
			double *vtkcellweights4 = new double[4];

			float *MRIcenter  = new float[3]; 
			float *TPcenter  = new float[3]; 
			double *bounds = new double[6];
 
			//subID = movingimage->GetCell( offset11 )->GetParametricCenter(paraCoords);
			int theimagecoordMRI[3],theimagecoordTP[3];
			movingimage->ComputeStructuredCoordinates(worldcenter,theimagecoordMRI,paraCoords);
			TPvolume->ComputeStructuredCoordinates(worldcenter,theimagecoordTP,paraCoords);
			float tt1 = worldcenter[0],tt2 = worldcenter[1],tt3 = worldcenter[2];
			float previousdist=1000000000;
			float thedist, previousdist2,previousdist3, nextdist, nextdist2, nextdist3;

			if (fullon == false)
			{
				//just for segmentation, save resampled file and exit
				savePPM(movingimage, filename, slicenumber);
				exit(0);
			}

				  vtkImageData* MRIbinary = vtkImageData::New();
					  MRIbinary = loadPPM(filename);
				 vtkShortArray *scalarsbinary= vtkShortArray::New();
			 scalarsbinary->SetNumberOfComponents(3);
			 scalarsbinary = (vtkShortArray *)(MRIbinary->GetPointData()->GetScalars());
			// int offsetbinary = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theimagecoordMRI[1]+(int)theimagecoordMRI[0]);
			  int offsetbinary = (int)((newmri_dimx+1)*(int)theimagecoordMRI[1]+(int)theimagecoordMRI[0]);

			  double *themid = new double[3];
			  double *mricurrentpoint = new double[3];
			 themid =  scalarsbinary->GetTuple3(offsetbinary); 
			  short midr = (short)themid[0];
			 short midg = (short)themid[1];
			 short midb = (short)themid[2];
		
  //MRIbinary->DeepCopy(superPixelSegmentation->GetOutput());


		for (double theangle2=0;theangle2<360;theangle2+=angleinterval)
		{
			double theangle = (theangle2*PI/180);

			//get the MRI index
			/////need index of center in MRI volume coordinates...
			double thexMRI=(double)theimagecoordMRI[0];//+40*cos(theangle);
			double theyMRI=(double)theimagecoordMRI[1];//+40*sin(theangle);
			double thexMRInext = thexMRI+cos(theangle); double theyMRInext= theyMRI+(sin(theangle));
			double thexMRInext2 = thexMRI+3*cos(theangle); double theyMRInext2= theyMRI+3*(sin(theangle));

			int offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			int offsetMRInext = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)(theyMRInext)+(int)(thexMRInext));
			int offsetMRInext2 = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)(theyMRInext2)+(int)(thexMRInext2));
			float MRIval = scalarsMRI->GetTuple1(offsetMRI);
			float MRIvalnext = scalarsMRI->GetTuple1(offsetMRInext);
			float MRIvalnext2 = scalarsMRI->GetTuple1(offsetMRInext2);

			//get the value of the MRI and topo
			int ii=0,jj=0,kk=0;
		


			////try now with the segmented MRI

			thexMRI=(double)theimagecoordMRI[0];//+40*cos(theangle)
			theyMRI=(double)theimagecoordMRI[1];//+40*sin(theangle)
			thexMRInext = thexMRI+cos(theangle);  theyMRInext= theyMRI+(sin(theangle));
			thexMRInext2 = thexMRI+3*cos(theangle);  theyMRInext2= theyMRI+3*(sin(theangle));

		//	offsetbinary = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theimagecoordMRI[1]+(int)theimagecoordMRI[0]);
		
			 offsetbinary = (int)((newmri_dimx+1)*(int)theyMRI+(int)thexMRI);
		 	 mricurrentpoint =  scalarsbinary->GetTuple3(offsetbinary);
			short r = mricurrentpoint[0];
			 short g = mricurrentpoint[1];
			 short b = mricurrentpoint[2];

			while((thexMRI>4) && (theyMRI>4) && (theyMRI<(newmri_dimy-4)) && (thexMRI<(newmri_dimx-4)) && (r == midr) && (g == midg) && (b == midb))
			{
				thexMRI+=cos(theangle);
				theyMRI+=sin(theangle);

				offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			movingimage->GetCell(offsetMRI)->GetBounds(bounds);
			 offsetbinary = (int)((newmri_dimx+1)*(int)theyMRI+(int)thexMRI);//was workin uptil today, mebbe still is... 18/02
			// offsetbinary = (int)((newmri_dimx)*(int)theyMRI+(int)thexMRI);
		 	 mricurrentpoint =  scalarsbinary->GetTuple3(offsetbinary); //size 786429
			 r = mricurrentpoint[0];
			 g = mricurrentpoint[1];
			 b = mricurrentpoint[2];

			}


/*			int subID, cellNum; 
			float *paraCoords2 = new float[3]; 
			float *xyzCoords2  = new float[3]; 
			float *paraCoords3 = new float[3]; 
			float *xyzCoords3  = new float[3]; 
			float *vtkcellweights2 = new float[4]; 
			float *vtkcellweights3 = new float[4];*/ 
		
			offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			//theyMRI=newmri_dimy-1;thexMRI=newmri_dimx-1;
			//offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			//subID = movingimage->GetCell( offsetMRI )->GetParametricCenter(paraCoords2);
			//int &subIDadd4 = subID; 
			//movingimage->GetCell(offsetMRI)->EvaluateLocation(subIDadd4, paraCoords2, xyzCoords2, vtkcellweights2); 

			
			//TPvolume->GetCell(offsetMRI)->GetBounds(bounds);
			//topoSlicePoints->InsertNextPoint(xyzCoords3[0],xyzCoords3[1],xyzCoords3[2]);
			//mriSlicePoints->InsertNextPoint(bounds[1],bounds[3],bounds[5]); //purple

		//	offsetMRI = (int)((newmri_dimy*newmri_dimx*(0))+newmri_dimx*(int)281+(int)212);
			//subID = movingimage->GetCell( offsetMRI )->GetParametricCenter(paraCoords2);
			//int &subIDadd14 = subID; 
			//movingimage->GetCell(offsetMRI)->EvaluateLocation(subIDadd14, paraCoords2, xyzCoords2, vtkcellweights2); 
			//mriSlicePoints->InsertNextPoint(xyzCoords2[0],xyzCoords2[1],xyzCoords2[2]); //is in the middle
			movingimage->GetCell(offsetMRI)->GetBounds(bounds);
			mriSlicePoints->InsertNextPoint(bounds[0],bounds[2],bounds[4]); 
		}
		//mriSlicePoints->InsertNextPoint(0,0,0);
		
		//insert the center point
		//mriSlicePoints->InsertNextPoint(worldcenter[0],worldcenter[2],worldcenter[4]); 

		for (double theangle2=0;theangle2<360;theangle2+=angleinterval)
		{
			double theangle = (theangle2*PI/180);
			//now for the surface topography
			double	thexTP=(double)theimagecoordTP[0]+40*cos(theangle);
			double theyTP=(double)theimagecoordTP[1]+40*sin(theangle);	
			//double	thexTP=(double)theimagecoordTP[0]+55*cos(theangle);
			//double theyTP=(double)theimagecoordTP[1]+55*sin(theangle);	
			double thexTPnext = thexTP+cos(theangle) , theyTPnext= theyTP+(sin(theangle));
			double thexTPprev = thexTP-cos(theangle) , theyTPprev= theyTP-(sin(theangle));
			double thexTPnext2 = thexTP+2*cos(theangle) , theyTPnext2= theyTP+(2*sin(theangle));
			double thexTPprev2 = thexTP-2*cos(theangle) , theyTPprev2= theyTP-(2*sin(theangle));
			int offsetTP = (int)((slicenumber)*(newmri_dimy+1)*(newmri_dimx+1)+((int)theyTP)*(newmri_dimx+1)+(int)thexTP);	
			int offsetTPnext = (int)((slicenumber)*(newmri_dimy+1)*(newmri_dimx+1)+(int)(theyTPnext)*(newmri_dimx+1)+(int)thexTPnext);
			int offsetTPprev= (int)((slicenumber)*(newmri_dimy+1)*(newmri_dimx+1)+(int)(theyTPprev)*(newmri_dimx+1)+(int)thexTPprev);
			int offsetTPnext2 = (int)((slicenumber)*(newmri_dimy+1)*(newmri_dimx+1)+(int)(theyTPnext2)*(newmri_dimx+1)+(int)thexTPnext2);
			int offsetTPprev2= (int)((slicenumber)*(newmri_dimy+1)*(newmri_dimx+1)+(int)(theyTPprev2)*(newmri_dimx+1)+(int)thexTPprev2);

			float TPval = scalarsTP->GetTuple1(offsetTP);
			nextdist=scalarsTP->GetTuple1(offsetTPnext);
			previousdist=scalarsTP->GetTuple1(offsetTPprev);
		
			nextdist2=scalarsTP->GetTuple1(offsetTPnext2);
			previousdist2=scalarsTP->GetTuple1(offsetTPprev2);//should be +(int)cos(angle)

				 //(TPval>=40)  ||
			while((theyTP>4) && (thexTP>4) &&(theyTP<(newmri_dimy-4)) && (thexTP<(newmri_dimx-4)) &&((TPval>=15)  ||  (TPval>=nextdist)  ||(TPval>=previousdist)))// was 10, 15 for 7//&& ((TPval>=nextdist2)||(TPval>=previousdist2)))
			{

				thexTP+=cos(theangle);
				theyTP+=sin(theangle);

				thexTPnext = thexTP+cos(theangle) , theyTPnext= theyTP+(sin(theangle));
				thexTPprev = thexTP-cos(theangle) , theyTPprev= theyTP-(sin(theangle));

				thexTPnext2 = thexTP+2*cos(theangle) , theyTPnext2= theyTP+(2*sin(theangle));
				thexTPprev2 = thexTP-2*cos(theangle) , theyTPprev2= theyTP-(2*sin(theangle));

				offsetTPnext = (int)((slicenumber)*(newmri_dimy+1)*(newmri_dimx+1)+(int)(theyTPnext)*(newmri_dimx+1)+(int)thexTPnext);
				offsetTP = (int)((slicenumber)*(newmri_dimy+1)*(newmri_dimx+1)+((int)theyTP)*(newmri_dimx+1)+(int)thexTP);	
				offsetTPprev= (int)((slicenumber)*(newmri_dimy+1)*(newmri_dimx+1)+(int)(theyTPprev)*(newmri_dimx+1)+(int)thexTPprev);
				offsetTPnext2 = (int)((slicenumber)*(newmri_dimy+1)*(newmri_dimx+1)+(int)(theyTPnext2)*(newmri_dimx+1)+(int)thexTPnext2);
				offsetTPprev2= (int)((slicenumber)*(newmri_dimy+1)*(newmri_dimx+1)+(int)(theyTPprev2)*(newmri_dimx+1)+(int)thexTPprev2);

				nextdist=scalarsTP->GetTuple1(offsetTPnext);
				previousdist=scalarsTP->GetTuple1(offsetTPprev);
				TPval = scalarsTP->GetTuple1(offsetTP);
				nextdist2=scalarsTP->GetTuple1(offsetTPnext2);
				previousdist2=scalarsTP->GetTuple1(offsetTPprev2);//should be +(int)cos(angle)

			
			//temp
				offsetTP = (int)((slicenumber)*(newmri_dimy)*(newmri_dimx)+((int)theyTP)*(newmri_dimx)+(int)thexTP); //removed the +1 for points
				TPvolume->GetCell(offsetTP)->GetBounds(bounds);
			//	topoSlicePoints->InsertNextPoint(bounds[0],bounds[2],bounds[4]); //yellow
			}	

			offsetTP = (int)((slicenumber)*(newmri_dimy)*(newmri_dimx)+((int)theyTP)*(newmri_dimx)+(int)thexTP); //removed the +1 for points
			//offsetTP = (int)((newmri_dimy*newmri_dimx*(3))+newmri_dimx*(int)theyTP+(int)(300)); // works without -1
			//newmri_dimx-1 starts new
			//subID = movingimage->GetCell( offsetTP )->GetParametricCenter(paraCoords3);
			//int &subIDadd5 = subID; 
			//movingimage->GetCell(offsetTP)->EvaluateLocation(subIDadd5, paraCoords3, xyzCoords3, vtkcellweights3); 

			TPvolume->GetCell(offsetTP)->GetBounds(bounds);
			//topoSlicePoints->InsertNextPoint(xyzCoords3[0],xyzCoords3[1],xyzCoords3[2]);
			float yaya1 = bounds[0], yaya2 = bounds[2], yaya3 = bounds[4];

			//slight variation, find the closest point on the topo to this new point now, and keep x and y coords
			int subID, cellNum; 
			double *paraCoords = new double[3]; 
			double *xyzCoords  = new double[3]; 
			double *vtkcellweights = new double[4]; 
			double theclosestpoint[3];
			int coords[3];
			double d;
			//coords[0] =(int)thexTP; coords[1] = (int)theyTP; coords[2] = slicenumber;
			vtkIdType id6;//= TPvolume->ComputeCellId(coords);
			xyzCoords[0]=bounds[0];xyzCoords[1]=bounds[2];xyzCoords[2]=bounds[4];
			int offsetTP2 = (int)((slicenumber)*(newmri_dimy+1)*(newmri_dimx+1)+((int)theyTP)*(newmri_dimx+1)+(int)thexTP);
			subID = TPvolume->GetCell(offsetTP2)->GetParametricCenter(paraCoords);
			//int &subIDadd3 = subID; 
			cellLocatorreg->FindClosestPoint(xyzCoords ,theclosestpoint, id6, subID, d);


			//find more close points and take the one with the closest z value instead
	
			topoSlicePoints->InsertNextPoint(theclosestpoint[0],theclosestpoint[1],xyzCoords[2]);//xyzCoords[2]);
		//	topoSlicePoints->InsertNextPoint(bounds[0],bounds[2],bounds[4]); //yellow, good...
		}
		
	//	topoSlicePoints->InsertNextPoint(0,0,0);
		//topoSlicePoints->InsertNextPoint(worldcenter[0],worldcenter[2],worldcenter[4]); 
		/*
		//find the thin plate spline parameters
		int Ptopo[12] = {201,125,5,208,335,5,271,244,5,135,260,5};
		int Pmri[12] = {173,128,5, 205,366,5,311,252,5,127,256,5};

		for (int ff=0;ff<4;ff++)
		{

			int offset_affine = (int)((newmri_dimy*newmri_dimx*Ptopo[ff*3+2])+newmri_dimx*Ptopo[ff*3+1]+Ptopo[ff*3]);
			//subID = imageData_ax1rigid->GetCell( offset_affine )->GetParametricCenter(paraCoords); //imageData_ax1base is after rigid registration 
			subID = tempimage->GetCell( offset_affine )->GetParametricCenter(paraCoords);
			int &subIDadd4 = subID; 
			tempimage->GetCell(offset_affine)->EvaluateLocation(subIDadd4, paraCoords, xyzCoords, vtkcellweights); 
			topoSlicePoints->InsertNextPoint(xyzCoords[0],xyzCoords[1],xyzCoords[2]);

			float yaya1=xyzCoords[0];
			float yaya2=xyzCoords[1];
			float yaya3=xyzCoords[2];

			int offset_affine2 = (int)((newmri_dimy*newmri_dimx*Pmri[ff*3+2])+newmri_dimx*Pmri[ff*3+1]+Pmri[ff*3]);
			subID = tempimage->GetCell( offset_affine2 )->GetParametricCenter(paraCoords2); //imageData_ax1base is after rigid registration 
			int &subIDadd5 = subID; 
			tempimage->GetCell(offset_affine2)->EvaluateLocation(subIDadd5, paraCoords2, xyzCoords2, vtkcellweights); 
			mriSlicePoints->InsertNextPoint(xyzCoords2[0],xyzCoords2[1],xyzCoords2[2]);

			float yaya4=xyzCoords2[0];
			float yaya5=xyzCoords2[1];
			float yaya6=xyzCoords2[2];
		}*/
		return true;
}

bool AffineRegistration::GenerateTPslice(vtkPolyData *pdata, vtkPolyData *TPpoly, vtkImageData* TPvolume, int slicenumber, vtkMatrix4x4 *Topotx, const char *name)
{

	//preliminary transformations which are inverse of the MRI transformation. it winds up aligning well
	
		vtkPolyData *tildtedTopo = vtkPolyData::New();
	
		vtkTransform *toporevtx=vtkTransform::New();
		toporevtx->SetMatrix(Topotx); //Topotx = mrivert_regvol[closest_vert]
		toporevtx->Inverse();
		toporevtx->Update();


		//apply tx applied in howtopo, then apply art, then apply inverse show
		vtkTransform *noregtopotx = vtkTransform::New();
		noregtopotx->RotateZ(-90); 
		noregtopotx->RotateX(90); 
		noregtopotx->Update();

		vtkTransform *noregtopotxinv = vtkTransform::New();
		noregtopotxinv->RotateZ(-90); 
		noregtopotxinv->RotateX(90); 
		noregtopotxinv->Inverse();
		noregtopotxinv->Update();

		//rotate
		vtkTransformPolyDataFilter *fpre = vtkTransformPolyDataFilter::New ();
		fpre->SetInput (TPpoly);
		fpre->SetTransform (noregtopotx);
		fpre->Update ();
		tildtedTopo=fpre->GetOutput();

		//transform
		vtkTransformPolyDataFilter *f44 = vtkTransformPolyDataFilter::New ();
		f44->SetInput (tildtedTopo);
		f44->SetTransform (toporevtx);
		f44->Update ();
		tildtedTopo=f44->GetOutput();
		vtkPolyData *polydatasze = f44->GetOutput();

		//rotate back
		/*vtkTransformPolyDataFilter *fpost = vtkTransformPolyDataFilter::New ();
		fpost->SetInput(tildtedTopo);
		fpost->SetTransform (noregtopotxinv);
		fpost->Update ();
		tildtedTopo=fpost->GetOutput();*/

			//this was here before, rotate..
/*	vtkGeneralTransform *tnoreg44 = vtkGeneralTransform::New ();
	tnoreg44->RotateZ(-90.0); //invert the body back
	tnoreg44->RotateX(90.0);
	vtkTransformPolyDataFilter *f444 = vtkTransformPolyDataFilter::New ();
	f444->SetInput (tildtedTopo);
	f444->SetTransform (tnoreg44);
	f444->Update ();
	vtkPolyData *polydatasze=f444->GetOutput();*/

		//transform the contour points
		vtkTransformPolyDataFilter *f44n = vtkTransformPolyDataFilter::New ();
		f44n->SetInput (pdata);
		f44n->SetTransform (toporevtx);
		f44n->Update ();
		pdata=f44n->GetOutput();

	vtkCellLocator *cellLocatorreg = vtkCellLocator::New();
	cellLocatorreg->SetDataSet(polydatasze);
	cellLocatorreg->BuildLocator();


vtkFloatArray *scalarsTPout = vtkFloatArray::New();
vtkFloatArray *scalarsTP = vtkFloatArray::New();


		int imageextent[6];
		TPvolume->GetExtent(imageextent);
		int newmri_dimz=imageextent[5]+1;
		newmri_dimz=1;  /////////////////////////////////////////////remove
		int newmri_dimy=imageextent[3]+1;
		int newmri_dimx=imageextent[1]+1;

		double mri1origin[3];
		TPvolume->GetOrigin(mri1origin);
		double current_voxel_location[3];

			
		//get the values of volumes into scalars
		for (int i=0;i<(newmri_dimz)*(newmri_dimy)*(newmri_dimx);i++)
		{
				scalarsTPout->InsertNextTuple1(255.0);
				scalarsTP->InsertNextTuple1(255.0);
		}

		scalarsTP=  (vtkFloatArray *)TPvolume->GetPointData()->GetScalars();
		int subId = 0;
	vtkIdType cellid;
	for(int i1=slicenumber;i1<(slicenumber+1);i1=i1+1) //(intervalz-4)varying this makes x-y work as a rectangle fine,, so i1*intervaly*intervalx is good
	{//for(int j1=-20;j1<=(newmri_dimy-2);j1=j1+4) 
		for(int j1=0;j1<=(newmri_dimy-2);j1=j1+4) 
		{//for(int k1=-20;k1<=(newmri_dimx-2);k1=k1+4)
			for(int k1=0;k1<=(newmri_dimx-2);k1=k1+4)//intervalx, varying this makes z-y work as a rectangle fine, +k1 is good
			{	
				//find the distance to the closest point in sze
				// Compute the distance d between the point m and the closest point c on the surface.
				double theclosestpoint[3], mindist;
				double m[3],d;
				double bounds[6];
				//find location of volume element
				int offset22= ((int)slicenumber*newmri_dimx*newmri_dimy+ (int)(j1)*(newmri_dimx)+(int)(k1));
					TPvolume->GetCell(offset22)->GetBounds(bounds);
							m[0]=bounds[0];
							m[1]=bounds[2];
							m[2]=bounds[4];
				cellLocatorreg->FindClosestPoint(m ,theclosestpoint, cellid, subId, d);

				if(d<1)
					d=50;
				for(int jj = 0;jj<=4;jj++)
				{
					for(int kk = 0;kk<=4;kk++)
					{
					//	int offset1 = (int)((i1)*intervaly*intervalx+(j1+jj)*intervalx+k1+kk);
						int offset2= ((int)slicenumber*newmri_dimx*newmri_dimy+ (int)(j1+jj)*(newmri_dimx)+(int)(k1+kk));
						int offsetTP = (int)((slicenumber)*(newmri_dimy+1)*(newmri_dimx+1)+((int)(j1+jj))*(newmri_dimx+1)+(int)(k1+kk));	

						//int offset2= ((int)slicenumber*(newmri_dimx-1)*(newmri_dimy-1)+ (int)(j1+jj)*(newmri_dimx-1)+(int)(k1+kk));	
							TPvolume->GetCell(offset22)->GetBounds(bounds);
							m[0]=bounds[0];
							m[1]=bounds[2];
							m[2]=bounds[4];

							d=sqrt(pow((theclosestpoint[0]-m[0]),2)+pow((theclosestpoint[1]-m[1]),2)+pow((theclosestpoint[2]-m[2]),2));
							scalarsTPout->SetTuple1(offsetTP,(255-d));//oppose valued for segmenting purposes
					}
				}
			}
		}
	}
	
	//set the topo points
	int numtopopoints = pdata->GetNumberOfPoints();
	vtkFloatArray *values22 = vtkFloatArray::New();
	if (numtopopoints > 0)
			values22->DeepCopy(pdata->GetPoints()->GetData());
	for(int pointstoset=0; pointstoset<numtopopoints; pointstoset++)
	{
		//get the location of the point in 3D world coords
		double *pointme = new double[3];
		values22->GetTuple(pointstoset,pointme);

		//pointme = pdata->GetPointData()->GetTuple(pointstoset);
		//find the location in the volume coords	
		double *paraCoords = new double[3]; 
		int *theimagecoordMRI = new int[3];
		TPvolume->ComputeStructuredCoordinates(pointme,theimagecoordMRI,paraCoords);
		//find the offset
		int offsetTP = (int)((slicenumber)*(newmri_dimy)*(newmri_dimx)+((int)(theimagecoordMRI[1]))*(newmri_dimx)+(int)theimagecoordMRI[0]);	

		//set the value to 0
		scalarsTPout->SetTuple1(offsetTP,100);

	}


	////////I changed dimensions from theim->SetExtent(0,newmri_dimx-1, 0, newmri_dimy-1, 0, newmri_dimz-1 ); because it was crasging
		vtkImageData* theim = vtkImageData::New();
		theim->SetExtent(0,newmri_dimx-1, 0, newmri_dimy-1, 0, newmri_dimz-1 );
		theim->SetDimensions(newmri_dimx,newmri_dimy,newmri_dimz);
		theim->SetNumberOfScalarComponents(1);
		theim->SetScalarTypeToFloat();
		theim->AllocateScalars();
		theim->GetPointData()->CopyAllOn();
		theim->GetPointData()->CopyScalarsOn();
		theim->GetPointData()->SetScalars(scalarsTPout);

	theim->Update();

	//save it
	savePPM(theim, name, slicenumber); 
	return true;
}

float AffineRegistration::getkappa(vtkFloatArray *scalarsMRI , vtkFloatArray *scalarsTP, int sizex, int sizey, int *numelem )
{
	//numelem = new int[3];
		//for each element in size imagedata, calculate TP, FP, FN
	int TruePositive = 0, FalsePositive = 0, FalseNegative=0;
	double *TPval = new double[3];
	double *MRIval = new double[3];
	for (int i=0; i<sizey; i++)
	{
			for (int j=0; j<sizex; j++)
			{
				//get the 2 values
				int offset= (i*(sizex)+j);
				TPval = (scalarsTP->GetTuple3(offset));
				MRIval =(scalarsMRI->GetTuple3(offset));
				int TPsegval = 0, MRIsegval =0;
				if( ((int)TPval[0] > 250) )//&&  ((int)TPval[1] == 0)  && ((int)TPval[2] == 0))
					TPsegval=1;
				if( ((int)MRIval[0] > 250) )//&&  ((int)MRIval[1] == 0)  && ((int)MRIval[2] == 0))
					MRIsegval=1;


				if( ((int)TPsegval == 1) && ((int)MRIsegval == 1) )
				{
					TruePositive+=1;
				}
				if( ((int)TPsegval == 0) && ((int)MRIsegval == 1) )
				{
					FalsePositive+=1;
				}
					if( ((int)TPsegval== 1) && ((int)MRIsegval == 0) )
				{
					FalseNegative+=1;
				}

				/*
				if( ((int)TPval[0] > 100) && ((int)MRIval[0] > 100) )
				{
					TruePositive+=1;
				}
				if( ((int)TPval[0] <100) && ((int)MRIval[0] > 100) )
				{
					FalsePositive+=1;
				}
					if( ((int)TPval[0] > 100) && ((int)MRIval[0] <100) )
				{
					FalseNegative+=1;
				}
				*/
			}
	}

	*(numelem) = TruePositive;
	*(numelem+1)= FalsePositive;
	*(numelem+2)= FalseNegative;

	float thekappa = (float)(2*TruePositive)/((float)(2*TruePositive+ FalsePositive + FalseNegative));

	return thekappa;
}


bool AffineRegistration::GetnewMRIEdgePoints(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *MRIVert, vtkMatrix4x4 *articulatedtx, float **anglepoints,vtkPolyData *mriEdgePolydata, const char *filename, const char *filenameTP)
{

	vtkShortArray *scalarsMRI = vtkShortArray::New();

	double *worldMRIcenter = new double[3];
		worldMRIcenter[0]=(MRIVert->GetElement(0,3));
		worldMRIcenter[1]=(MRIVert->GetElement(1,3));
		worldMRIcenter[2]=(MRIVert->GetElement(2,3));

		int imageextent[6];
		movingimage->GetExtent(imageextent);
		int newmri_dimz=imageextent[5];
		int newmri_dimy=imageextent[3];
		int newmri_dimx=imageextent[1];


		double current_voxel_location[3];

		//get the values of volumes into scalars
		for (int i=0;i<newmri_dimz*newmri_dimy*newmri_dimx;i++)
		{
				scalarsMRI->InsertNextTuple1(0);
		}
		scalarsMRI = (vtkShortArray *)(movingimage->GetPointData()->GetScalars());
	

		//get the image coordinates of the vertebral center as a starting point for both MRI and TP
			double *paraCoords = new double[3]; 
			double *paraCoords4 = new double[3]; 
			double *xyzCoords  = new double[3]; 


			float *MRIcenter  = new float[3]; 
			float *TPcenter  = new float[3]; 
			double *bounds = new double[6];
 
			double mri1origin[3];
			movingimage->GetOrigin(mri1origin);
			int theimagecoordMRI[3];

			
			/* segmentation parameters		
			superPixelSegmentation->SetSigma(.5);
			superPixelSegmentation->SetK(500);
			superPixelSegmentation->SetMinSize(50);
			*/

						//find vertebral center coordinates in untransformed MRI slice 
			movingimage->ComputeStructuredCoordinates(worldMRIcenter,theimagecoordMRI,paraCoords);

			//otherwise load segmented MRI
			vtkImageData* MRIbinary = vtkImageData::New();
			MRIbinary = loadPPM(filename);
			vtkShortArray *scalarsbinary= vtkShortArray::New();
			scalarsbinary->SetNumberOfComponents(3);
			scalarsbinary = (vtkShortArray *)(MRIbinary->GetPointData()->GetScalars());

			vtkImageData* TPbinary = vtkImageData::New();
			TPbinary = loadPPM(filenameTP);
			vtkShortArray *scalarsbinaryTP= vtkShortArray::New();
			scalarsbinaryTP->SetNumberOfComponents(3);
			scalarsbinaryTP = (vtkShortArray *)(TPbinary->GetPointData()->GetScalars());

	 	    int offsetbinary = (int)((newmri_dimx+1)*(int)theimagecoordMRI[1]+(int)theimagecoordMRI[0]);


			//the image as is has no transformation. thus transform, but the above no longer applies

			  double *themid = new double[3];
			  double *mricurrentpoint = new double[3];
			 themid =  scalarsbinary->GetTuple3(offsetbinary); 
			  short midr = (short)themid[0];
			 short midg = (short)themid[1];
			 short midb = (short)themid[2];
		
			 vtkPoints *mriSlicePoints = vtkPoints::New();

		for (double theangle2=0;theangle2<360;theangle2+=1)
		{
			double theangle = (theangle2*PI/180);

			//get the MRI index
			/////need index of center in MRI volume coordinates...
			double thexMRI=(double)theimagecoordMRI[0];//+40*cos(theangle);
			double theyMRI=(double)theimagecoordMRI[1];//+40*sin(theangle);
			double thexMRInext = thexMRI+cos(theangle); double theyMRInext= theyMRI+(sin(theangle));
			double thexMRInext2 = thexMRI+3*cos(theangle); double theyMRInext2= theyMRI+3*(sin(theangle));

			int offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			int offsetMRInext = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)(theyMRInext)+(int)(thexMRInext));
			int offsetMRInext2 = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)(theyMRInext2)+(int)(thexMRInext2));
			float MRIval = scalarsMRI->GetTuple1(offsetMRI);
			float MRIvalnext = scalarsMRI->GetTuple1(offsetMRInext);
			float MRIvalnext2 = scalarsMRI->GetTuple1(offsetMRInext2);

			//get the value of the MRI and topo
			int ii=0,jj=0,kk=0;
		

			thexMRI=(double)theimagecoordMRI[0];//+40*cos(theangle)
			theyMRI=(double)theimagecoordMRI[1];//+40*sin(theangle)
			thexMRInext = thexMRI+cos(theangle);  theyMRInext= theyMRI+(sin(theangle));
			thexMRInext2 = thexMRI+3*cos(theangle);  theyMRInext2= theyMRI+3*(sin(theangle));

			 offsetbinary = (int)((newmri_dimx+1)*(int)theyMRI+(int)thexMRI);
		 	 mricurrentpoint =  scalarsbinary->GetTuple3(offsetbinary);
			double *mricurrentpointTP=new double[3];
			mricurrentpointTP= scalarsbinaryTP->GetTuple3(offsetbinary);
			short r = mricurrentpoint[0];
			 short g = mricurrentpoint[1];
			 short b = mricurrentpoint[2];

			while((mricurrentpointTP[0]==midr) && (thexMRI>4) && (theyMRI>4) && (theyMRI<(newmri_dimy-4)) && (thexMRI<(newmri_dimx-4)) && (r == midr) && (g == midg) && (b == midb))
			{
				thexMRI+=cos(theangle);
				theyMRI+=sin(theangle);

				offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			movingimage->GetCell(offsetMRI)->GetBounds(bounds);
			 offsetbinary = (int)((newmri_dimx+1)*(int)theyMRI+(int)thexMRI);
			// offsetbinary = (int)((newmri_dimx)*(int)theyMRI+(int)thexMRI);
		 	 mricurrentpoint =  scalarsbinary->GetTuple3(offsetbinary); //size 786429
			 mricurrentpointTP= scalarsbinaryTP->GetTuple3(offsetbinary);
			 r = mricurrentpoint[0];
			 g = mricurrentpoint[1];
			 b = mricurrentpoint[2];

			}


			offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);

			movingimage->GetCell(offsetMRI)->GetBounds(bounds);
			mriSlicePoints->InsertNextPoint(bounds[0],bounds[2],bounds[4]); 
		}

		//now transform the MRI points with the articulated model transformation
		vtkPolyData *tempdata = vtkPolyData::New();
		tempdata->SetPoints(mriSlicePoints);
		/*vtkTransform *articulatedImageTransform=vtkTransform::New();
		articulatedImageTransform->SetMatrix(articulatedtx);
		articulatedImageTransform->Update();
		vtkTransformPolyDataFilter *fvertsurface = vtkTransformPolyDataFilter::New ();
		fvertsurface->SetInput (tempdata); //original
		fvertsurface->SetTransform (articulatedImageTransform);
		fvertsurface->Update ();
		tempdata=fvertsurface->GetOutput();
		tempdata->DeepCopy(fvertsurface->GetOutput());
		*/
		mriEdgePolydata->SetPoints(tempdata->GetPoints());
		
		//update the array information since this is what is used for weighting calculations
		vtkFloatArray *registeredPointsArray = vtkFloatArray::New();

		double *temptuple = new double[3];
		registeredPointsArray= (vtkFloatArray *)tempdata->GetPoints()->GetData();
			for (double theangle2=0;theangle2<360;theangle2+=1)
			{
					
				int theind = (int)theangle2;
				//temptuple = tempdata->GetPoints()->GetData()->GetTuple(theind);
				temptuple = registeredPointsArray->GetTuple(theind);
					anglepoints[0][theind]= temptuple[0];
					anglepoints[1][theind] = temptuple[1];
					anglepoints[2][theind] = temptuple[2];
			}

			return true;
}

bool AffineRegistration::GetMRIEdgePoints(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *XrayVert, float **anglepoints,vtkPoints *mriEdgePoints, const char *filename)
{

	/*anglepoints = new float*[3];
	for(int i = 0;i <3;i++)
		anglepoints[i] = new float[360];*/

		vtkShortArray *scalarsMRI = vtkShortArray::New();
		vtkFloatArray *scalarsTP = vtkFloatArray::New();
	
		double *worldcenter = new double[3];
		worldcenter[0]=(float)(XrayVert->GetElement(0,3));
		worldcenter[1]=(float)(XrayVert->GetElement(1,3));
		worldcenter[2]=(float)(XrayVert->GetElement(2,3));

		int imageextent[6];
		movingimage->GetExtent(imageextent);
		int newmri_dimz=imageextent[5];
		int newmri_dimy=imageextent[3];
		int newmri_dimx=imageextent[1];

		double mri1origin[3];
		movingimage->GetOrigin(mri1origin);
		float current_voxel_location[3];

		//get the values of volumes into scalars
		for (int i=0;i<newmri_dimz*newmri_dimy*newmri_dimx;i++)
		{
				scalarsMRI->InsertNextTuple1(0);
		}
		scalarsMRI = (vtkShortArray *)(movingimage->GetPointData()->GetScalars());

		//get the image coordinates of the vertebral center as a starting point for both MRI and TP
			double *paraCoords = new double[3]; 
			double *paraCoords4 = new double[3]; 
			double *xyzCoords  = new double[3]; 
			float *vtkcellweights = new float[4]; 
			float *vtkcellweights4 = new float[4];

			float *MRIcenter  = new float[3]; 

			double *bounds = new double[6];
 
			//subID = movingimage->GetCell( offset11 )->GetParametricCenter(paraCoords);
			int theimagecoordMRI[3];
			movingimage->ComputeStructuredCoordinates(worldcenter,theimagecoordMRI,paraCoords);

			float tt1 = worldcenter[0],tt2 = worldcenter[1],tt3 = worldcenter[2];
			float previousdist=1000000000;
			float thedist, previousdist2,previousdist3, nextdist, nextdist2, nextdist3;


			  vtkImageData* MRIbinary = vtkImageData::New();

	MRIbinary = loadPPM(filename);
	
			 vtkShortArray *scalarsbinary= vtkShortArray::New();
			 scalarsbinary->SetNumberOfComponents(3);
			 scalarsbinary = (vtkShortArray *)(MRIbinary->GetPointData()->GetScalars());
		    int offsetbinary = (int)((newmri_dimx+1)*(int)theimagecoordMRI[1]+(int)theimagecoordMRI[0]);

			  double *themid = new double[3];
			  double *mricurrentpoint = new double[3];
			 themid =  scalarsbinary->GetTuple3(offsetbinary); //good: 109,193,93
			  short midr = (short)themid[0];//->GetComponent(offsetbinary,0);
			 short midg = (short)themid[1];//scalarsbinary->GetComponent(offsetbinary,1);
			 short midb = (short)themid[2];//scalarsbinary->GetComponent(offsetbinary,2);
		

		for (double theangle2=0;theangle2<360;theangle2+=1)
		{
			double theangle = (theangle2*PI/180);

			//get the MRI index
			/////need index of center in MRI volume coordinates...
			double thexMRI=(double)theimagecoordMRI[0]+5*cos(theangle);
			double theyMRI=(double)theimagecoordMRI[1]+5*sin(theangle);
			double thexMRInext = thexMRI+cos(theangle); double theyMRInext= theyMRI+(sin(theangle));
			double thexMRInext2 = thexMRI+3*cos(theangle); double theyMRInext2= theyMRI+3*(sin(theangle));

			int offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			int offsetMRInext = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)(theyMRInext)+(int)(thexMRInext));
			int offsetMRInext2 = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)(theyMRInext2)+(int)(thexMRInext2));
			float MRIval = scalarsMRI->GetTuple1(offsetMRI);
			float MRIvalnext = scalarsMRI->GetTuple1(offsetMRInext);
			float MRIvalnext2 = scalarsMRI->GetTuple1(offsetMRInext2);

			//get the value of the MRI and topo
			int ii=0,jj=0,kk=0;
		
		//		if((thedist<3.0) && (thedist<previousdist) &&  (thedist<previousdist2)&&  (thedist<previousdist3)&&(thedist<nextdist)&&  (thedist<nextdist2)&&  (thedist<nextdist3))//we have reached a min

		/*	while((MRIvalnext>0) && (MRIval<350))//((((MRIval- MRIvalnext)/MRIval)<0.8) || (((MRIval- MRIvalnext2)/MRIval)<0.8) || ((MRIval>0) && (MRIvalnext<350)))
			{
				thexMRI+=cos(theangle);
				theyMRI+=sin(theangle);
				
			 thexMRInext = thexMRI+cos(theangle) , theyMRInext= theyMRI+(sin(theangle));
			 thexMRInext2 = thexMRI+3*cos(theangle) , theyMRInext2= theyMRI+3*(sin(theangle));

			 offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			 offsetMRInext = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)(theyMRInext)+(int)(thexMRInext));
			 offsetMRInext2 = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)(theyMRInext2)+(int)(thexMRInext2));
			 MRIval = scalarsMRI->GetTuple1(offsetMRI);
			 MRIvalnext = scalarsMRI->GetTuple1(offsetMRInext);
			 MRIvalnext2 = scalarsMRI->GetTuple1(offsetMRInext2);
			 int yyayayay=0;

				}*/

			////try now with the segmented MRI

			thexMRI=(double)theimagecoordMRI[0]+40*cos(theangle);
			theyMRI=(double)theimagecoordMRI[1]+40*sin(theangle);
			thexMRInext = thexMRI+cos(theangle);  theyMRInext= theyMRI+(sin(theangle));
			thexMRInext2 = thexMRI+3*cos(theangle);  theyMRInext2= theyMRI+3*(sin(theangle));
			//should be 222, 165, 62
			
			 offsetbinary = (int)((newmri_dimx+1)*(int)theyMRI+(int)thexMRI);
		 	 mricurrentpoint =  scalarsbinary->GetTuple3(offsetbinary);
			short r = mricurrentpoint[0];
			 short g = mricurrentpoint[1];
			 short b = mricurrentpoint[2];

			while((thexMRI>4) && (theyMRI>4) && (theyMRI<(newmri_dimy-4)) && (thexMRI<(newmri_dimx-4)) && (r == midr) && (g == midg) && (b == midb))
			{
				thexMRI+=cos(theangle);
				theyMRI+=sin(theangle);

				offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			movingimage->GetCell(offsetMRI)->GetBounds(bounds);
		//	mriSlicePoints->InsertNextPoint(bounds[0],bounds[2],bounds[4]); 
		//	mriEdgePoints->InsertNextPoint(bounds[0],bounds[1],bounds[2]);
			 offsetbinary = (int)((newmri_dimx+1)*(int)theyMRI+(int)thexMRI);
		 	 mricurrentpoint =  scalarsbinary->GetTuple3(offsetbinary); //size 786429
			 r = mricurrentpoint[0];
			 g = mricurrentpoint[1];
			 b = mricurrentpoint[2];

			}



			offsetMRI = (int)((newmri_dimy*newmri_dimx*(slicenumber))+newmri_dimx*(int)theyMRI+(int)thexMRI);
			movingimage->GetCell(offsetMRI)->GetBounds(bounds);
			//save in array
			int theind = (int)theangle2;
			anglepoints[0][theind]= bounds[0];
			anglepoints[1][theind] = bounds[2];
			anglepoints[2][theind] = bounds[4];
			mriEdgePoints->InsertNextPoint(bounds[0],bounds[2],bounds[4]);

		}

		
		return true;
}
void AffineRegistration::savePPM( vtkImageData* theim, const char *name, int slicenum) {

	vtkDataArray *volumedatArray = vtkFloatArray::New();
	volumedatArray = (vtkFloatArray *)(theim->GetPointData()->GetScalars());

	int thedims[3];
	theim->GetDimensions(thedims);
  int width = thedims[0];
  int height = thedims[1];
  std::ofstream file(name, std::ios::out | std::ios::binary);

  file << "P6\n" << width << " " << height << "\n" << UCHAR_MAX << "\n";
  //file.read((char *)(&thechardata[0]) , width * height * 3*sizeof(unsigned char));
  unsigned char *thevaluetemp = new unsigned char[3];
  float maxnorm = volumedatArray->GetMaxNorm();
  for(int i = 0; i<height ; i++)
  {
	    for(int j = 0; j<width ; j++)
		 {
			 int offset1 = (int)((width*height*(slicenum))+width*(int)i+(int)j);
			 float floatval = (((float)volumedatArray->GetTuple1(offset1)*255.0)/maxnorm);//was 256
			 //int offset1 = i*width+j;
			  thevaluetemp[0] = (unsigned char)(floatval);
			  thevaluetemp[1] = (unsigned char)(floatval);
			  thevaluetemp[2] = (unsigned char)(floatval);
			  file.write((char *)thevaluetemp, 3*sizeof(unsigned char));
		//	  file.write((char *)i,  sizeof(int));
		//	  file.write((char *)i,  sizeof(int));
		}
  }

  
  file.close();

  return;
}

void AffineRegistration::savePPMunNorm( vtkImageData* theim, const char *name, int slicenum) {

	vtkDataArray *volumedatArray = vtkFloatArray::New();
	volumedatArray = (vtkFloatArray *)(theim->GetPointData()->GetScalars());

	int thedims[3];
	theim->GetDimensions(thedims);
  int width = thedims[0];
  int height = thedims[1];
  std::ofstream file(name, std::ios::out | std::ios::binary);

  file << "P6\n" << width << " " << height << "\n" << UCHAR_MAX << "\n";
  //file.read((char *)(&thechardata[0]) , width * height * 3*sizeof(unsigned char));
  unsigned char *thevaluetemp = new unsigned char[3];
  float maxnorm = volumedatArray->GetMaxNorm();
  for(int i = 0; i<height ; i++)
  {
	    for(int j = 0; j<width ; j++)
		 {
			 int offset1 = (int)((width*height*(slicenum))+width*(int)i+(int)j);
			 float floatval = ((float)volumedatArray->GetTuple1(offset1));//was 256
			 //int offset1 = i*width+j;
			  thevaluetemp[0] = (unsigned char)(floatval);
			  thevaluetemp[1] = (unsigned char)(floatval);
			  thevaluetemp[2] = (unsigned char)(floatval);
			  file.write((char *)thevaluetemp, 3*sizeof(unsigned char));
		//	  file.write((char *)i,  sizeof(int));
		//	  file.write((char *)i,  sizeof(int));
		}
  }

  
  file.close();

  return;
}
/* read PNM field, skipping comments */ 
static void pnm_read(std::ifstream &file, char *buf) {
  char doc[BUF_SIZE];
  char c;
  char temp;

  file >> c;
  while (c == '#') {
    file.getline(doc, BUF_SIZE);
    file >> c;
  }
  file.putback(c);
  
  file.width(BUF_SIZE);
  file >> buf;
  file.ignore();
  //file >> temp;
}

vtkImageData* AffineRegistration::loadPPMvol(const char *name, int slicenum)
{
  char buf[BUF_SIZE], doc[BUF_SIZE];
  
  /* read header */
  std::ifstream file(name, std::ios::in | std::ios::binary);
     if(!file) { 
    exit(1);
   } 

  pnm_read(file, buf);
  if (strncmp(buf, "P6", 2))
    exit(1);

  pnm_read(file, buf);
  int width = atoi(buf);
  pnm_read(file, buf);
  int height = atoi(buf);

  pnm_read(file, buf);
 // if (atoi(buf) > UCHAR_MAX)
//    throw pnm_error();

  /* read data and store in vtkimagedata*/

 // image<rgb> *im = new image<rgb>(width, height);
   unsigned char *thechardata = new unsigned char[width * height*3];
   	   vtkImageData* theim = vtkImageData::New();
   theim->SetExtent(0,width-1, 0, height-1, 0, slicenum );
   theim->SetDimensions(width,height,slicenum+1);
	vtkDataArray *volumedatArray = vtkFloatArray::New();
	volumedatArray->SetNumberOfComponents(3);
		for( int k11=0;k11<(slicenum+1);k11++)
	{
	for( int i11=0;i11<height;i11++)
	{
		for( int j11=0;j11<width;j11++)
		{
				volumedatArray->InsertNextTuple3(0,0,0);
		}
	}
		}
//  file.read((char *)imPtr(im, 0, 0), width * height * sizeof(rgb));
    file.read((char *)(&thechardata[0]) , width * height * 3*sizeof(unsigned char));

	//now store in imagedata

	for( int k11=slicenum;k11<slicenum+1;k11++)
	{
		for( int i11=0;i11<height;i11++)
		{
		for( int j11=0;j11<width;j11++)
		{
			int offset1 = (int)(3*(i11*width+j11));
			int offset2 = (int)(k11*width*height+i11*width+j11);//i11*width+j11;//j11*height+i11;//(int)(3*(i11*width+j11));
			float temp1 = (float)thechardata[offset1];
			float temp2 = (float)thechardata[offset1+1];
			float temp3 = (float)thechardata[offset1+2];
			volumedatArray->SetTuple3(offset2,(float)thechardata[offset1],(float)thechardata[offset1+1],(float)thechardata[offset1+2]);
		}}}

		theim->SetNumberOfScalarComponents(3);
	theim->SetScalarTypeToFloat();
	theim->AllocateScalars();
	theim->GetPointData()->CopyAllOn();
	theim->GetPointData()->CopyScalarsOn();
	theim->GetPointData()->SetScalars(volumedatArray);
	theim->Update();

	file.close();
  return theim;
}

vtkImageData* AffineRegistration::loadPPMvol2(vtkImageData *tempimage, const char *name, int slicenum)
{
  char buf[BUF_SIZE], doc[BUF_SIZE];
  
  /* read header */
  std::ifstream file(name, std::ios::in | std::ios::binary);
     if(!file) { 
    exit(1);
   } 

  pnm_read(file, buf);
  if (strncmp(buf, "P6", 2))
    exit(1);

  pnm_read(file, buf);
  int width = atoi(buf);
  pnm_read(file, buf);
  int height = atoi(buf);

  pnm_read(file, buf);
 // if (atoi(buf) > UCHAR_MAX)
//    throw pnm_error();

  /* read data and store in vtkimagedata*/

   unsigned char *thechardata = new unsigned char[width * height*3];
   	   

	vtkDataArray *volumedatArray2 = vtkFloatArray::New();
	volumedatArray2->SetNumberOfComponents(1);
		int tempextent[6];

		tempimage->GetExtent(tempextent);

	for( int k11=0;k11<(tempextent[5]+1);k11++)
	{
	for( int i11=0;i11<(tempextent[3]+1);i11++)
	{
		for( int j11=0;j11<(tempextent[1]+1);j11++)
		{
				volumedatArray2->InsertNextTuple1(100);
		}
		}
		}

	volumedatArray2 = tempimage->GetPointData()->GetScalars();
//  file.read((char *)imPtr(im, 0, 0), width * height * sizeof(rgb));
    file.read((char *)(&thechardata[0]) , width * height * 3*sizeof(unsigned char));

	//now store in imagedata

	int heightvol=tempextent[3]+1;
	int widthvol=tempextent[1]+1;
	for( int k11=slicenum;k11<slicenum+1;k11++)
	{
		for( int i11=0;i11<height;i11++)
		{
		for( int j11=0;j11<width;j11++)
		{
			int offset1 = (int)(3*(i11*width+j11));
			int offset2 = (int)(k11*widthvol*heightvol+i11*widthvol+j11);//i11*width+j11;//j11*height+i11;//(int)(3*(i11*width+j11));
			float temp1 = (float)thechardata[offset1];
			float temp2 = (float)thechardata[offset1+1];
			float temp3 = (float)thechardata[offset1+2];
			volumedatArray2->SetTuple1(offset2,(float)thechardata[offset1]);

		}}}

	/*	tempimage->SetNumberOfScalarComponents(1);
	tempimage->SetScalarTypeToFloat();*/
	tempimage->AllocateScalars();
	tempimage->GetPointData()->CopyAllOn();
	tempimage->GetPointData()->CopyScalarsOn();
	tempimage->GetPointData()->SetScalars(volumedatArray2);
	tempimage->Update();

	
	
	file.close();
  //return theim;
	return tempimage;
}


vtkImageData* AffineRegistration::loadPPM(const char *name)
{
  char buf[BUF_SIZE], doc[BUF_SIZE];
  
  /* read header */
  std::ifstream file(name, std::ios::in | std::ios::binary);
     if(!file) { 
    exit(1);
   } 

  pnm_read(file, buf);
  if (strncmp(buf, "P6", 2))
    exit(1);

  pnm_read(file, buf);
  int width = atoi(buf);
  pnm_read(file, buf);
  int height = atoi(buf);

  pnm_read(file, buf);
 // if (atoi(buf) > UCHAR_MAX)
//    throw pnm_error();

  /* read data and store in vtkimagedata*/

 // image<rgb> *im = new image<rgb>(width, height);
   unsigned char *thechardata = new unsigned char[width * height*3];
   	   vtkImageData* theim = vtkImageData::New();
   theim->SetExtent(0,width-1, 0, height-1, 0, 0 );
   theim->SetDimensions(width,height,1);
	vtkDataArray *volumedatArray = vtkFloatArray::New();
	volumedatArray->SetNumberOfComponents(3);
	for( int i11=0;i11<height;i11++)
	{
		for( int j11=0;j11<width;j11++)
		{
				volumedatArray->InsertNextTuple3(0,0,0);
		}
	}
//  file.read((char *)imPtr(im, 0, 0), width * height * sizeof(rgb));
    file.read((char *)(&thechardata[0]) , width * height * 3*sizeof(unsigned char));

	//now store in imagedata


		for( int i11=0;i11<height;i11++)
		{
		for( int j11=0;j11<width;j11++)
		{
			int offset1 = (int)(3*(i11*width+j11));
			int offset2 = i11*width+j11;//j11*height+i11;//(int)(3*(i11*width+j11));
			float temp1 = (float)thechardata[offset1];
			float temp2 = (float)thechardata[offset1+1];
			float temp3 = (float)thechardata[offset1+2];
			volumedatArray->SetTuple3(offset2,(float)thechardata[offset1],(float)thechardata[offset1+1],(float)thechardata[offset1+2]);
		}}

		theim->SetNumberOfScalarComponents(3);
	theim->SetScalarTypeToFloat();
	theim->AllocateScalars();
	theim->GetPointData()->CopyAllOn();
	theim->GetPointData()->CopyScalarsOn();
	theim->GetPointData()->SetScalars(volumedatArray);
	theim->Update();

	file.close();
  return theim;
}

bool AffineRegistration::RegisterAffine2(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *rigidtx,vtkImageData * TPvolume,vtkPolyData *mri_newlandmarkspart[17], ublas::matrix<double>& thescale)
{

	int imageextent[6], imageextentTP[6];
	movingimage->GetExtent(imageextent);

	TPvolume->GetExtent(imageextentTP);
	//apply the transformation	
	vtkTransform *txscale=vtkTransform::New();
	txscale->SetMatrix(rigidtx);
	txscale->Update();

	vtkImageData *movingimageregistered= vtkImageData::New();
	vtkImageReslice *resliceax1temp = vtkImageReslice::New();
	resliceax1temp->SetInput(movingimage);
	resliceax1temp->InterpolateOn();
	resliceax1temp->SetInterpolationModeToCubic();
	resliceax1temp->SetResliceTransform(txscale);
	resliceax1temp->SetOutputSpacing(movingimage->GetSpacing());
	resliceax1temp->AutoCropOutputOn();
	resliceax1temp->GetInput()->Update();
	resliceax1temp->Update();
	movingimageregistered->DeepCopy( resliceax1temp->GetOutput());
	movingimageregistered->Update();

	movingimageregistered->GetExtent(imageextent);
	vtkImageReslice *resliceax1temp2 = vtkImageReslice::New();
	vtkImageData *movingimageregistered_slice = vtkImageData::New();
	resliceax1temp2->SetInput(movingimageregistered);
	resliceax1temp2->InterpolateOn();
	resliceax1temp2->SetInterpolationModeToCubic();
	resliceax1temp2->SetOutputExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],slicenumber,slicenumber);
	resliceax1temp2->SetOutputOrigin(movingimageregistered->GetOrigin());
	resliceax1temp2->SetOutputSpacing(movingimage->GetSpacing());
	resliceax1temp2->AutoCropOutputOn();
	resliceax1temp2->GetInput()->Update();
	resliceax1temp2->Update();
	movingimageregistered_slice=resliceax1temp2->GetOutput();
	movingimageregistered_slice->Update();	


	//calculate dTdScale, which is just the x,y value


	//find the optimal transform coefficients
	int theiteration=0;
	
		//testing gaussian to be removed!!!!!
	vtkImageData* cacatemp = vtkImageData::New();
	cacatemp->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],0,1);

	double prevnorm=-1, thenorm=0;
	 while ((theiteration<10))// && (prevnorm <thenorm))
	 {
             //only reason this is here is for setting the right imageextent
		 	matrix<double> dTdPhiMatrixx(imageextent[1]*imageextent[3],4);
		matrix<double> dTdPhiMatrixy(imageextent[1]*imageextent[3],4);

	for(int i=0;i<imageextent[1];i++)
	{
		for(int j=0;j<imageextent[3];j++)
		{
			//x component of derivative
			dTdPhiMatrixx(imageextent[3]*i+j,0) = i; //wrt Scale x
			dTdPhiMatrixx(imageextent[3]*i+j,1) = 0; //wrt skew x
			dTdPhiMatrixx(imageextent[3]*i+j,2) = j; //wrt skew y, should be z
			dTdPhiMatrixx(imageextent[3]*i+j,3) = 0; //wrt scale y

			//y component of derivative
			dTdPhiMatrixy(imageextent[3]*i+j,0) = 0; //wrt scale x
			dTdPhiMatrixy(imageextent[3]*i+j,1) = i;  //wrt skew x, should be = z
			dTdPhiMatrixy(imageextent[3]*i+j,2) = 0; //wrt skew y
			dTdPhiMatrixy(imageextent[3]*i+j,3) = j;  //wrt Scale y
		}
	}

		 //calculate the similarity metric
		 matrix<double> theoverlapderivative(imageextent[1],imageextent[3]); //imageextent recalculated at bottom

		FindOverlapDerivative(movingimageregistered,TPvolume,theoverlapderivative,slicenumber);

		//calculate dBdT, convolution of moving image
		vtkImageData* dBdTimagedatax = vtkImageData::New();
		//dBdTimagedatax->SetOrigin(movingimage->GetOrigin()); //without this nothingness appears
		dBdTimagedatax->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[5],imageextent[6]);
		//dBdTimagedatax->SetSpacing(movingimage->GetSpacing());//(mri_spacing[0],mri_spacing[1],mri_spacing[2]); 
		dBdTimagedatax->SetScalarTypeToFloat();
		dBdTimagedatax = CalculateImageDerivative(movingimageregistered,slicenumber,1);

		vtkImageData* dBdTimagedatay = vtkImageData::New();
		//dBdTimagedatay->SetOrigin(movingimage->GetOrigin()); //without this nothingness appears
		dBdTimagedatay->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[5],imageextent[6]);
		//dBdTimagedatay->SetSpacing(movingimage->GetSpacing());//(mri_spacing[0],mri_spacing[1],mri_spacing[2]); 
		dBdTimagedatay->SetScalarTypeToFloat();
		dBdTimagedatay = CalculateImageDerivative(movingimageregistered,slicenumber,2);

		 //calculate dCdPhi = 1/norm(i) sum_i (dC/dB * dB/dT * dT/dphi)
		matrix<double> dCdPhi(4, 1);
		FinddCdPhi2(theoverlapderivative,dBdTimagedatax,dBdTimagedatay,dTdPhiMatrixx,dTdPhiMatrixy,dCdPhi,slicenumber);

		//calculate the norm of dCdPhi
		prevnorm = thenorm;
		thenorm = norm_1(dCdPhi);

			/*dCdPhi(0,0)=sumx;
			dCdPhi(1,0)=sumskewx;
			dCdPhi(2,0)=sumskewy;
			dCdPhi(3,0)=sumy;*/

		 //coeffy(j1,j2)= squeeze(coeffy(j1,j2))-squeeze((themu.*dCdPhi(2,j1,j2))./norm(dCdPhifornorm)) ;
		thescale(0,0) = thescale(0,0)+0.05*dCdPhi(0,0)/thenorm;
		thescale(1,0) = thescale(1,0)+0.05*dCdPhi(1,0)/thenorm;
		thescale(2,0) = thescale(2,0)+0.05*dCdPhi(2,0)/thenorm;
		thescale(3,0) = thescale(3,0)+0.05*dCdPhi(3,0)/thenorm;

		//skewx->SetElement(2, 1,tan(_szy*(pi/180.0)));   skewx->SetElement(1, 2,tan(_syz*(pi/180.0))); 
		// skewy->SetElement(2, 0,tan(_szx*(pi/180.0)));   skewy->SetElement(0, 2,tan(_sxz*(pi/180.0)));
		// skewz->SetElement(1, 0,tan(_sxy*(pi/180.0)));    skewz->SetElement(0, 1,tan(_syx*(pi/180.0))); 

		//apply transformation

	//	thescale(0,0) = 1;
	//	thescale(1,0) = 0.3;
	//	thescale(2,0) = 0;
	//	thescale(3,0) = 1;


		rigidtx->SetElement(2,1,thescale(1,0)); //skewx
		rigidtx->SetElement(1,2,thescale(1,0));

		rigidtx->SetElement(2,0,thescale(2,0)); //skewy
		rigidtx->SetElement(0,2,thescale(2,0));

		rigidtx->SetElement(0,0,thescale(0,0)); //scale
		rigidtx->SetElement(1,1,thescale(3,0));

		txscale->SetMatrix(rigidtx);
		txscale->Update();

	resliceax1temp->SetInput(movingimage);
	resliceax1temp->InterpolateOn();
	resliceax1temp->SetInterpolationModeToCubic();
	resliceax1temp->SetResliceTransform(txscale);
	resliceax1temp->SetOutputSpacing(movingimage->GetSpacing());
	resliceax1temp->AutoCropOutputOn();
	resliceax1temp->GetInput()->Update();
	resliceax1temp->Update();
	movingimageregistered->DeepCopy( resliceax1temp->GetOutput());
	movingimageregistered->Update();

	movingimageregistered->GetExtent(imageextent);
	resliceax1temp2->SetInput(movingimageregistered);
	resliceax1temp2->InterpolateOn();
	resliceax1temp2->SetInterpolationModeToCubic();
	resliceax1temp2->SetOutputExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],slicenumber,slicenumber);
	resliceax1temp2->SetOutputOrigin(movingimageregistered->GetOrigin());
	resliceax1temp2->SetOutputSpacing(movingimage->GetSpacing());
	resliceax1temp2->AutoCropOutputOn();
	resliceax1temp2->GetInput()->Update();
	resliceax1temp2->Update();
	movingimageregistered_slice=resliceax1temp2->GetOutput();
	movingimageregistered_slice->Update();	
				
		theiteration++;
	 }

	 return true;
}

bool AffineRegistration::RegisterAffine(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *rigidtx,vtkImageData * TPvolume,vtkPolyData *mri_newlandmarkspart[17], ublas::matrix<double>& thescale)
{

	int imageextent[6], imageextentTP[6];
	movingimage->GetExtent(imageextent);

	TPvolume->GetExtent(imageextentTP);
	//apply the transformation	
	vtkTransform *txscale=vtkTransform::New();
	txscale->SetMatrix(rigidtx);
	txscale->Update();

	vtkImageData *movingimageregistered= vtkImageData::New();
	vtkImageReslice *resliceax1temp = vtkImageReslice::New();
	resliceax1temp->SetInput(movingimage);
	resliceax1temp->InterpolateOn();
	resliceax1temp->SetInterpolationModeToCubic();
	resliceax1temp->SetResliceTransform(txscale);
	resliceax1temp->SetOutputSpacing(movingimage->GetSpacing());
	resliceax1temp->AutoCropOutputOn();
	resliceax1temp->GetInput()->Update();
	resliceax1temp->Update();
	movingimageregistered->DeepCopy( resliceax1temp->GetOutput());
	movingimageregistered->Update();

	movingimageregistered->GetExtent(imageextent);
	vtkImageReslice *resliceax1temp2 = vtkImageReslice::New();
	vtkImageData *movingimageregistered_slice = vtkImageData::New();
	resliceax1temp2->SetInput(movingimageregistered);
	resliceax1temp2->InterpolateOn();
	resliceax1temp2->SetInterpolationModeToCubic();
	resliceax1temp2->SetOutputExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],slicenumber,slicenumber);
	resliceax1temp2->SetOutputOrigin(movingimageregistered->GetOrigin());
	resliceax1temp2->SetOutputSpacing(movingimage->GetSpacing());
	resliceax1temp2->AutoCropOutputOn();
	resliceax1temp2->GetInput()->Update();
	resliceax1temp2->Update();
	movingimageregistered_slice=resliceax1temp2->GetOutput();
	movingimageregistered_slice->Update();	


	//calculate dTdScale, which is just the x,y value


	//find the optimal transform coefficients
	int theiteration=0;
	
		//testing gaussian to be removed!!!!!
	vtkImageData* cacatemp = vtkImageData::New();
	cacatemp->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],0,1);

	 while (theiteration<5)//% && (prevnorm/norm(dCdPhifornorm) <1)
	 {
             //only reason this is here is for setting the right imageextent
		 	matrix<double> dTdPhiMatrixx(imageextent[1]*imageextent[3],2);
	matrix<double> dTdPhiMatrixy(imageextent[1]*imageextent[3],2);

	for(int i=0;i<imageextent[1];i++)
	{
		for(int j=0;j<imageextent[3];j++)
		{
			dTdPhiMatrixx(imageextent[3]*i+j,0) = i;
			dTdPhiMatrixx(imageextent[3]*i+j,1) = 0;
			dTdPhiMatrixy(imageextent[3]*i+j,0) = 0;
			dTdPhiMatrixy(imageextent[3]*i+j,1) = j;
		}
	}

		 //calculate the similarity metric
		 matrix<double> theoverlapderivative(imageextent[1],imageextent[3]); //imageextent recalculated at bottom

		FindOverlapDerivative(movingimageregistered,TPvolume,theoverlapderivative,slicenumber);

		//calculate dBdT, convolution of moving image
		vtkImageData* dBdTimagedatax = vtkImageData::New();
		//dBdTimagedatax->SetOrigin(movingimage->GetOrigin()); //without this nothingness appears
		dBdTimagedatax->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[5],imageextent[6]);
		//dBdTimagedatax->SetSpacing(movingimage->GetSpacing());//(mri_spacing[0],mri_spacing[1],mri_spacing[2]); 
		dBdTimagedatax->SetScalarTypeToFloat();
		dBdTimagedatax = CalculateImageDerivative(movingimageregistered,slicenumber,1);

		vtkImageData* dBdTimagedatay = vtkImageData::New();
		//dBdTimagedatay->SetOrigin(movingimage->GetOrigin()); //without this nothingness appears
		dBdTimagedatay->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[5],imageextent[6]);
		//dBdTimagedatay->SetSpacing(movingimage->GetSpacing());//(mri_spacing[0],mri_spacing[1],mri_spacing[2]); 
		dBdTimagedatay->SetScalarTypeToFloat();
		dBdTimagedatay = CalculateImageDerivative(movingimageregistered,slicenumber,2);

		 //calculate dCdPhi = 1/norm(i) sum_i (dC/dB * dB/dT * dT/dphi)
		matrix<double> dCdPhi(2, 1);
		FinddCdPhi(theoverlapderivative,dBdTimagedatax,dBdTimagedatay,dTdPhiMatrixx,dTdPhiMatrixy,dCdPhi,slicenumber);

		//calculate the norm of dCdPhi
		double thenorm = norm_1(dCdPhi);


		 //coeffy(j1,j2)= squeeze(coeffy(j1,j2))-squeeze((themu.*dCdPhi(2,j1,j2))./norm(dCdPhifornorm)) ;
		thescale(0,0) = thescale(0,0)-0.05*dCdPhi(0,0)/thenorm;
		thescale(1,1) = thescale(1,1)-0.05*dCdPhi(1,0)/thenorm;


		//apply transformation
		rigidtx->SetElement(0,0,thescale(0,0));
		rigidtx->SetElement(1,1,thescale(1,1));
		txscale->SetMatrix(rigidtx);
		txscale->Update();

	resliceax1temp->SetInput(movingimage);
	resliceax1temp->InterpolateOn();
	resliceax1temp->SetInterpolationModeToCubic();
	resliceax1temp->SetResliceTransform(txscale);
	resliceax1temp->SetOutputSpacing(movingimage->GetSpacing());
	resliceax1temp->AutoCropOutputOn();
	resliceax1temp->GetInput()->Update();
	resliceax1temp->Update();
	movingimageregistered->DeepCopy( resliceax1temp->GetOutput());
	movingimageregistered->Update();

	movingimageregistered->GetExtent(imageextent);
	resliceax1temp2->SetInput(movingimageregistered);
	resliceax1temp2->InterpolateOn();
	resliceax1temp2->SetInterpolationModeToCubic();
	resliceax1temp2->SetOutputExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],slicenumber,slicenumber);
	resliceax1temp2->SetOutputOrigin(movingimageregistered->GetOrigin());
	resliceax1temp2->SetOutputSpacing(movingimage->GetSpacing());
	resliceax1temp2->AutoCropOutputOn();
	resliceax1temp2->GetInput()->Update();
	resliceax1temp2->Update();
	movingimageregistered_slice=resliceax1temp2->GetOutput();
	movingimageregistered_slice->Update();	
				
		theiteration++;
	 }

	 return true;
}

//this function uncludes skew parametes as opposed to the original one
bool AffineRegistration::FinddCdPhi2(ublas::matrix<double>& theoverlapderivative,vtkImageData* dBdTimagedatax,vtkImageData* dBdTimagedatay,ublas::matrix<double>& dTdPhiMatrixx,ublas::matrix<double>& dTdPhiMatrixy,ublas::matrix<double>& dCdPhi, int slicenumber)
{

	//calculate dCdPhi = 1/norm(i) sum_i (dC/dB * dB/dT * dT/dphi)
	int imageextent[6];
	dBdTimagedatax->GetExtent(imageextent);
vtkFloatArray *scalarsdBdTx = vtkFloatArray::New();
scalarsdBdTx = (vtkFloatArray *)(dBdTimagedatax->GetPointData()->GetScalars());
vtkFloatArray *scalarsdBdTy = vtkFloatArray::New();
scalarsdBdTy = (vtkFloatArray *)(dBdTimagedatay->GetPointData()->GetScalars());
			double sumx=0,sumy=0, sumskewx=0,sumskewy=0;
/*
	dTdPhiMatrixx(imageextent[3]*i+j,0) = i; //wrt Scale x
			dTdPhiMatrixx(imageextent[3]*i+j,1) = 0; //wrt skew x
			dTdPhiMatrixx(imageextent[3]*i+j,2) = j; //wrt skew y, should be z
			dTdPhiMatrixx(imageextent[3]*i+j,3) = 0; //wrt scale y

			//y component of derivative
			dTdPhiMatrixy(imageextent[3]*i+j,0) = 0; //wrt scale x
			dTdPhiMatrixy(imageextent[3]*i+j,1) = i;  //wrt skew x, should be = z
			dTdPhiMatrixy(imageextent[3]*i+j,2) = 0; //wrt skew y
			dTdPhiMatrixy(imageextent[3]*i+j,3) = j;  //wrt Scale y
			*/


			//int offset2 = (int)((v)*3+u);
			for(int j1=0;j1<(imageextent[3]);j1=j1+1) 
			{
				for(int k1=0;k1<(imageextent[1]);k1=k1+1)
				{
					
					int offset4 = (int)((j1)*imageextent[1]+k1);
					int offset1 = (int)((slicenumber)*imageextent[1]*imageextent[3] + (j1)*imageextent[1]+k1);
				
					double curvalMovingx =  (double)(scalarsdBdTx->GetTuple1(offset1));
					double curvalMovingy =  (double)(scalarsdBdTy->GetTuple1(offset1));
					sumx+=theoverlapderivative(k1,j1)*curvalMovingx*dTdPhiMatrixx(offset4,0);
					sumy+=theoverlapderivative(k1,j1)*curvalMovingy*dTdPhiMatrixy(offset4,3);
					sumskewx += theoverlapderivative(k1,j1)*curvalMovingy*dTdPhiMatrixy(offset4,1);
					sumskewy += theoverlapderivative(k1,j1)*curvalMovingx*dTdPhiMatrixy(offset4,2);

				}
			}

			dCdPhi(0,0)=sumx;
			dCdPhi(1,0)=sumskewx;
			dCdPhi(2,0)=sumskewy;
			dCdPhi(3,0)=sumy;
			double thenorm = norm_1(dCdPhi);
			dCdPhi(0,0)=dCdPhi(0,0)/thenorm;
			dCdPhi(1,0)=dCdPhi(1,0)/thenorm;
			dCdPhi(2,0)=dCdPhi(2,0)/thenorm;
			dCdPhi(3,0)=dCdPhi(3,0)/thenorm;

	return true;
}



bool AffineRegistration::FinddCdPhi(ublas::matrix<double>& theoverlapderivative,vtkImageData* dBdTimagedatax,vtkImageData* dBdTimagedatay,ublas::matrix<double>& dTdPhiMatrixx,ublas::matrix<double>& dTdPhiMatrixy,ublas::matrix<double>& dCdPhi, int slicenumber)
{

	//calculate dCdPhi = 1/norm(i) sum_i (dC/dB * dB/dT * dT/dphi)
	int imageextent[6];
	dBdTimagedatax->GetExtent(imageextent);
vtkFloatArray *scalarsdBdTx = vtkFloatArray::New();
scalarsdBdTx = (vtkFloatArray *)(dBdTimagedatax->GetPointData()->GetScalars());
vtkFloatArray *scalarsdBdTy = vtkFloatArray::New();
scalarsdBdTy = (vtkFloatArray *)(dBdTimagedatay->GetPointData()->GetScalars());
			double sumx=0,sumy=0;

	for(int v = 0; v < 2; ++v)
	{
		for(int u = 0; u < 2; ++u)
		{

			int offset2 = (int)((v)*2+u);
			for(int j1=0;j1<(imageextent[3]);j1=j1+1) 
			{
				for(int k1=0;k1<(imageextent[1]);k1=k1+1)
				{
					
					int offset4 = (int)((j1)*imageextent[1]+k1);
					int offset1 = (int)((slicenumber)*imageextent[1]*imageextent[3] + (j1)*imageextent[1]+k1);
				
					double curvalMovingx =  (double)(scalarsdBdTx->GetTuple1(offset1));
					double curvalMovingy =  (double)(scalarsdBdTy->GetTuple1(offset1));
					sumx+=theoverlapderivative(k1,j1)*curvalMovingx*dTdPhiMatrixx(offset4,u);
					sumy+=theoverlapderivative(k1,j1)*curvalMovingy*dTdPhiMatrixy(offset4,v);	

					if((j1<100) && (j1>95) && (k1==100))
					{
						double yaya1 = theoverlapderivative(k1,j1);
						double yaya2 = dTdPhiMatrixx(offset4,u);
						double yaya3 = dTdPhiMatrixy(offset4,v);
						int y=1;
					}
				}
			}
		}
	}
			dCdPhi(0,0)=sumx;
			dCdPhi(1,0)=sumy;
			double thenorm = norm_1(dCdPhi);
			dCdPhi(0,0)=dCdPhi(0,0)/thenorm;
			dCdPhi(1,0)=dCdPhi(1,0)/thenorm;

	return true;
}


bool AffineRegistration::finddTdPhi(ublas::matrix<double>& control_points,int numcp,int xsize,int ysize,ublas::matrix<double>& dTdPhiMatrix_x,ublas::matrix<double>& dTdPhiMatrix_y)
{
	// ublas::matrix<double>& dTdPhiMatrix(imageextent[1]*imageextent[3],numcp*numcp);
	double hx = (double)xsize/(double)numcp;
	double hy = (double)ysize/(double)numcp;
	matrix<double> Nip(numcp, numcp);
	//double mumu = (double)theinterpi/(double)cpdistance;
	matrix<double> theNipvals(1, numcp);
	//for (int jj =0;jj<thenumcp;jj++)
	//{

			for(int j1=0;j1<(ysize);j1=j1+1) 
			{
				for(int k1=0;k1<(xsize);k1=k1+1)
				{
					int offset1 = (int)((j1)*xsize+k1);
					for(int v = 0; v < numcp; ++v)
					{
				
					for(int u = 0; u < numcp; ++u)
					{
						int offset2 = (int)((v)*numcp+u);
						dTdPhiMatrix_x(offset1,offset2)=theNipvals(0,u);
					}}

					for(int u = 0; u < numcp; ++u)
					{
				
					for(int v = 0; v < numcp; ++v)
					{
						int offset2 = (int)((v)*numcp+u);
						dTdPhiMatrix_y(offset1,offset2)=theNipvals(0,v);
					}}

					//dTdPhiMatrix_y(offset1,offset2)=evaluate_Bspline_basis(k1,ysize ,numcp,hy,theNip);
			
					
					 //dTdPhi(1,j1,j2,imx1,imx2)=  findBsplineBasis(ioverh(imx1,imx2)-j1); %0
                    //dTdPhi(2,j1,j2,imx1,imx2)=  findBsplineBasis(ioverh(imx1,imx2)-j2);
				}
			}

	return true;
}

vtkImageData* AffineRegistration::CalculateImageDerivative(vtkImageData* movingimage, int slicenumber, int direction)
{
	int imageextent[6];
	movingimage->GetExtent(imageextent);
	

		vtkImageGaussianSmooth *GaussianSmooth = vtkImageGaussianSmooth::New();
        GaussianSmooth->SetInput(movingimage);
		GaussianSmooth->SetDimensionality(3); 
		GaussianSmooth->SetStandardDeviation(3,0);

		if(direction==1)
			GaussianSmooth->SetStandardDeviation(4,0);
		else
			GaussianSmooth->SetStandardDeviation(0,4);
        //GaussianSmooth->SetRadiusFactor(3);
        GaussianSmooth->Update();

	//reslice
	vtkImageReslice *resliceax1temp2 = vtkImageReslice::New();
	resliceax1temp2->SetInput(GaussianSmooth->GetOutput());
	resliceax1temp2->InterpolateOn();
	resliceax1temp2->SetInterpolationModeToCubic();
	resliceax1temp2->SetOutputExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],slicenumber,slicenumber);
	//resliceax1temp2->SetOutputOrigin(movingimage->GetOrigin());
	resliceax1temp2->SetOutputSpacing(movingimage->GetSpacing());
	resliceax1temp2->AutoCropOutputOn();
	resliceax1temp2->GetInput()->Update();
	resliceax1temp2->Update();

	return GaussianSmooth->GetOutput();//resliceax1temp2->GetOutput();
	/*	vtkImageData* imagederivative = vtkImageData::New();
		imagederivative->SetOrigin(movingimage->GetOrigin()); //without this nothingness appears
		imagederivative->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],0,1);
		imagederivative->SetSpacing(movingimage->GetSpacing());//(mri_spacing[0],mri_spacing[1],mri_spacing[2]); 
		imagederivative->SetScalarTypeToFloat();

		imagederivative = resliceax1temp2->GetOutput();*/
        //YourOutputObject->SetInput( GaussianSmooth->GetOutput() );
        //GaussianSmooth->Delete();

		//imagederivative->setin
	//GaussianSmooth->GetOutput();
		//return resliceax1temp2->GetOutput();;//GaussianSmooth->GetOutput();
		/*
		vtkFloatArray *scalarsImage = vtkFloatArray::New();
		scalarsImage = (vtkFloatArray *)(movingimage->GetPointData()->GetScalars());

	for(int i1=0;i1<(1);i1=i1+1) 
	{
		for(int j1=0;j1<(intervaly);j1=j1+1) 
	{
	for(int k1=0;k1<(intervalx);k1=k1+1)
	{
		int offset1 = (int)((i1)*intervaly*intervalx+(j1)*intervalx+k1);
		float curvalImage =  scalarsImage->GetTuple1(offset1);

	}}}
*/
	/*  radius = (int) (this->StandardDeviations[axis] * this->RadiusFactors[axis]); 
size = 2*radius + 1; 
kernel = new double[size]; */

}

bool AffineRegistration::FindOverlapDerivative(vtkImageData* movingimage,vtkImageData* TPvolume, ublas::matrix<double>& overlapderivative, int theSliceNumber)
{
	double overlap = 0.0;
	int imageextent[6], imageextentTP[6];
	movingimage->GetExtent(imageextent);
	TPvolume->GetExtent(imageextentTP);
	float voloriginx, voloriginy,voloriginz;

//movingimage->GetOrigin(voloriginx, voloriginy,voloriginz);
	int intervalx=(int)(imageextent[1]);//-voloriginx);
	int intervaly=(int)(imageextent[3]);//-voloriginy);
	int intervalz=(int)(imageextent[5]);//-voloriginz);
	vtkFloatArray *scalarsMoving = vtkFloatArray::New();
	scalarsMoving = (vtkFloatArray *)(movingimage->GetPointData()->GetScalars());
	vtkFloatArray *scalarsTP = vtkFloatArray::New();
	scalarsTP = (vtkFloatArray *)(TPvolume->GetPointData()->GetScalars());

 //dCdB = 2.*(theimagedest-thewarpedImage);
	for(int i1=theSliceNumber;i1<(theSliceNumber+1);i1=i1+1) 
	{
		for(int j1=0;j1<(intervaly);j1=j1+1) 
	{
	for(int k1=0;k1<(intervalx);k1=k1+1)
	{
		int offset1 = (int)((i1)*intervaly*intervalx+(j1)*intervalx+k1);
		//get the values for the 2 volumes if the offset is within both extents
		if((j1<imageextentTP[3]) && (k1<imageextentTP[1]))
		{
		float curvalMoving =  scalarsMoving->GetTuple1(offset1);
		float curvalTP=  scalarsTP->GetTuple1(offset1);
		
		//if both values > 5 then overlap+1, but keep in mind the arms!
		if(((curvalMoving>5) &&(curvalTP<5)) ||((curvalMoving<5) &&(curvalTP>5)))
		{///***also check if k1,j1 <max of overlapderivative, if so then match if curvalmoving<5*/
			overlap+=1;
			overlapderivative(k1,j1)=2;
		}
				else
			overlapderivative(k1,j1)=0.001;
		}
		else
			overlapderivative(k1,j1)=0.001;
		}

	}}

	//delete the float arrays
	//scalarsTP->Delete();
	//scalarsMoving->Delete();
	return true;//2*overlap;
}

vtkImageData* AffineRegistration::ReadGeneratedTPVolume(vtkImageData* inputvol)
{
	int num_components2;
	int imageextent[6];
	inputvol->GetExtent(imageextent);
	int intervalx=(int)(imageextent[1]);
	int intervaly=(int)(imageextent[3]);
	int intervalz=(int)(imageextent[5]);

	std::ifstream myFile2 ("tpvolume.bin", ios::in | ios::binary);
	myFile2.seekg(0);
	myFile2.read ((char*) &num_components2, sizeof(int)); 
	float *thedata=new float[intervalx*intervaly*intervalz];
	//thedata=new float[num_components2];
	myFile2.read ((char*) thedata, (num_components2*sizeof(float)));
	myFile2.close();


vtkDataArray *volumedatArrayBinary = vtkFloatArray::New();
	for( int i11=0;i11<intervalz;i11++)
	{
		for( int j11=0;j11<intervaly;j11++)
		{
			for( int k11=0;k11<intervalx;k11++)
			{
				volumedatArrayBinary->InsertNextTuple1(0);
			}
		}
	}

	for(int i1=0;i1<intervalz;i1=i1+=1) //(intervalz-4)varying this makes x-y work as a rectangle fine,, so i1*intervaly*intervalx is good
	{
		for(int j1=0;j1<(intervaly);j1+=1) 
		{
			for(int k1=0;k1<(intervalx);k1+=1)//intervalx, varying this makes z-y work as a rectangle fine, +k1 is good
			{	int offset1 = (int)((i1)*intervaly*intervalx+(j1)*intervalx+k1);
			volumedatArrayBinary->SetTuple1(offset1,thedata[offset1]);//(maxdist-100));///sqrt(pow((float)intervalx,2)+pow((float)intervaly,2)+pow((float)intervalz,2)));//(int)((float)offset1*255.0/(intervaly*intervalx)));//*255/(intervaly*intervalx));
			}
		}
	}
	delete thedata;

			vtkImageData *imageData3 = vtkImageData::New();
			imageData3->SetDimensions(inputvol->GetDimensions());
			imageData3->SetSpacing(inputvol->GetSpacing());
			imageData3->SetOrigin(inputvol->GetOrigin());
	imageData3->SetScalarTypeToFloat();// >SetScalarTypeToUnsignedChar();
	imageData3->SetNumberOfScalarComponents(1);
	imageData3->AllocateScalars();
	imageData3->GetPointData()->CopyAllOn();
	imageData3->GetPointData()->CopyScalarsOn();
	imageData3->GetPointData()->SetScalars(volumedatArrayBinary);

	return imageData3;

}

vtkImageData* AffineRegistration::GeneratevolfrpmTP(vtkImageData* inputvol, vtkPolyData * polydatasze)
{

	//first generate a distance volume
	int num_components;
	float inputval,maxdist;

	int subId = 0;
	vtkIdType cellid;

	//distance map for the registered patient
	vtkCellLocator *cellLocatorreg = vtkCellLocator::New();
	cellLocatorreg->SetDataSet(polydatasze);
	cellLocatorreg->BuildLocator();

	int imageextent[6];
	int numcp = 8;
	double spacing[3];
	inputvol->GetExtent(imageextent);
	inputvol->GetSpacing(spacing);
	double voloriginx, voloriginy,voloriginz;
	inputvol->GetOrigin(voloriginx, voloriginy,voloriginz);
	//also get the distance between pixels


	//the maximum distance is from the middle to the edge
	int intervalx=(int)(imageextent[1]);//-minszeval[0])/4+1;
	int intervaly=(int)(imageextent[3]);//-minszeval[2])/4+1;
	int intervalz=(int)(imageextent[5]);//-minszeval[4])/4+1;
	maxdist=sqrt(pow(((float)intervalx),2)+pow(((float)intervaly),2)+pow(((float)intervalz),2));

	//initliaze volmodel to store
	//voldata storedatasourcereg;
	//storedatasourcereg.maxdist=maxdist;
	//storedatasourcereg.num_components=intervalx*intervaly*intervalz;

	int max_offset = intervalx*intervaly*intervalz;//(int)((intervalx-1)*intervaly+(intervaly-1)+(intervalz-1)*intervaly*intervalx);

	vtkDataArray *volumedatArray = vtkFloatArray::New();
	vtkDataArray *volumedatArrayBinary = vtkFloatArray::New();

	int i11=0, j11=0, k11=0;

	for( i11=0;i11<intervalz;i11++)
	{
		for( j11=0;j11<intervaly;j11++)
		{
			for( k11=0;k11<intervalx;k11++)
			{
				volumedatArray->InsertNextTuple1(0);
				volumedatArrayBinary->InsertNextTuple1(0);
			}
		}
	}

	float *thedata=new float[intervalx*intervaly*intervalz];

	for(int i1=0;i1<(intervalz);i1=i1+1) //(intervalz-4)varying this makes x-y work as a rectangle fine,, so i1*intervaly*intervalx is good
	{
		for(int j1=0;j1<(intervaly);j1=j1+4) 
		{
			for(int k1=0;k1<(intervalx);k1=k1+4)//intervalx, varying this makes z-y work as a rectangle fine, +k1 is good
			{	
				//find the distance to the closest point in sze
				// Compute the distance d between the point m and the closest point c on the surface.
				double theclosestpoint[3], mindist;
				double m[3],d;
				m[0]=(float)k1*spacing[0]+voloriginx;
				m[1]=(float)j1*spacing[1]+voloriginy;
				m[2]=(float)i1*spacing[2]+voloriginz;
				cellLocatorreg->FindClosestPoint(m ,theclosestpoint, cellid, subId, d);

				for(int jj = 0;jj<4;jj++)
				{
					for(int kk = 0;kk<4;kk++)
					{
						int offset1 = (int)((i1)*intervaly*intervalx+(j1+jj)*intervalx+k1+kk);
						if(offset1 <= max_offset)
						{
							m[0]=(float)(k1+kk)*spacing[0]+voloriginx;
							m[1]=(float)(j1+jj)*spacing[1]+voloriginy;
							m[2]=(float)i1*spacing[2]+voloriginz;

							d=sqrt(pow((theclosestpoint[0]-m[0]),2)+pow((theclosestpoint[1]-m[1]),2)+pow((theclosestpoint[2]-m[2]),2));
							volumedatArray->SetTuple1(offset1,d);
						}
					}
				}
			}
		}
	}

	int num_components2 = intervalx*intervaly*intervalz;
	float *thedata2=new float[num_components2];

	for(int i1=0;i1<num_components2;i1++)
	{
		//vol_model->InsertNextTuple1(255);
		thedata2[i1]=0;

	}

	//now binarise volume
	for(int i1=0;i1<(intervalz);i1=i1+1) //(intervalz-4)varying this makes x-y work as a rectangle fine,, so i1*intervaly*intervalx is good
	{
		for(int j1=0;j1<(intervaly);j1=j1+1) 
		{
			bool inside=false;
			float previousdist=1000000000;
			float thedist;
			for(int k1=0;k1<(intervalx);k1=k1+1)//intervalx, varying this makes z-y work as a rectangle fine, +k1 is good
			{	
				int offset1 = (int)((i1)*intervaly*intervalx+(j1)*intervalx+k1);
				thedist=volumedatArray->GetTuple1(offset1);
				if(thedist>previousdist) //we have reached a min
					inside=!inside;
				if(inside)
				{
					volumedatArrayBinary->SetTuple1(offset1,255);
					thedata2[offset1]=255;
				}
				else
					thedata2[offset1]=0;
				previousdist=thedist;
			}}}


	std::ofstream myFilereg ("tpvolume.bin", ios::out | ios::binary);
	myFilereg.write ((char*) &num_components2, sizeof(int));
	myFilereg.write ((char*) thedata2, num_components2*sizeof(float));//2* sizeof(int)+storedatasource.num_components*sizeof(float));
	myFilereg.close();


	vtkImageData* theTPvol = vtkImageData::New();
	theTPvol->SetOrigin(inputvol->GetOrigin()); //without this nothingness appears
	theTPvol->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[4],imageextent[5]);
	theTPvol->SetSpacing(inputvol->GetSpacing());//(mri_spacing[0],mri_spacing[1],mri_spacing[2]); 
	theTPvol->SetScalarTypeToFloat();
	theTPvol->AllocateScalars();
	theTPvol->GetPointData()->CopyAllOn();
	theTPvol->GetPointData()->CopyScalarsOn();
	theTPvol->GetPointData()->SetScalars(volumedatArrayBinary);

	cellLocatorreg->Delete();
	volumedatArray->Delete();
	volumedatArrayBinary->Delete();


	//save the volume cause it is long

	return theTPvol;
}
