#include "stdafx.h"
#include "BSplineRegistration.h"
#include <string>
 #include <math.h>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

 #define TINY 1.0e-20; 

#ifndef _CRT_SECURE_NO_DEPRECATE
#  define _CRT_SECURE_NO_DEPRECATE
 #endif 

//===vtk===========
#include <vtkActor.h>
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
#include <vtkSTLWriter.h>
#include <vtkLoopSubdivisionFilter.h>
#include <vtkSurfaceReconstructionFilter.h>
#include <vtkThinPlateSplineTransform.h>
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
#include <vtkOutlineFilter.h>
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
#include <vtkPlanes.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkColorTransferFunction.h>
#include <vtkImplicitModeller.h>
#include <vtkImageCast.h>
#include <vtkImageEuclideanDistance.h>
#include <vtkImageGaussianSmooth.h>
#include "vtkGridTransform.h"
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



using namespace boost::numeric::ublas;
typedef permutation_matrix<std::size_t> pmatrix;

//vtkImageData* BSplineRegistration::RegisterBspline(vtkImageData* movingimage,int moving_index, int target_index)
vtkImageData* BSplineRegistration::RegisterBspline(vtkImageData* movingimage,vtkImageData * TPvolume,vtkPolyData *mri_newlandmarkspart[17])
{

	//create initial grid of control points, say 8 by 8
	//Spacing equivalent to the 2^(maximum number of refinement steps)
	
int imageextent[6];
int numcp = 8;
movingimage->GetExtent(imageextent);
vtkImageData* movingimageregistered = vtkImageData::New();

movingimageregistered->SetOrigin(movingimage->GetOrigin()); //without this nothingness appears
movingimageregistered->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[4],imageextent[5]);
movingimageregistered->SetSpacing(movingimage->GetSpacing());//(mri_spacing[0],mri_spacing[1],mri_spacing[2]); 
movingimageregistered->SetScalarTypeToFloat();
int xdist=(int)(imageextent[1]-imageextent[0])/(numcp-1);
int ydist=(int)(imageextent[3]-imageextent[2])/(numcp-1);




//Apply code for each slice at a time
//compute initial control node spacing
//for(int slicenumber=0;slicenumber<imageextent[5];slicenumber++)
for(int slicenumber=3;slicenumber<4;slicenumber++)
{
	matrix<double> control_points(numcp*numcp, 2);
		for(int v = 0; v < numcp; ++v)
		{
			for(int u = 0; u < numcp; ++u)
			{
				/*if (u<(numcp/2))
					control_points(v*numcp+u,0) = 0;
				else
					control_points(v*numcp+u,0) = 1;
				if (v<(numcp/2))
					control_points(v*numcp+u,1) = 0;
				else
					control_points(v*numcp+u,1) = 1;*/
				control_points(v*numcp+u,0) = 2.0;//u* (double)((int)xdist/10.0);
				control_points(v*numcp+u,1) = 2.0;//v* (double)((int)ydist/10.0);
			}
		}

		movingimageregistered=ApplyBsplineDeformation(control_points,movingimage,numcp,slicenumber);
	
		//calculate dTdPhi
		matrix<double> dTdPhiMatrixx(imageextent[1]*imageextent[3],numcp*numcp);
		matrix<double> dTdPhiMatrixy(imageextent[1]*imageextent[3],numcp*numcp);
		finddTdPhi(control_points,numcp,imageextent[1],imageextent[3],dTdPhiMatrixx,dTdPhiMatrixy);

		//find the optimal B-spline transform coefficients
	int theiteration=0;
	matrix<double> theoverlapderivative(imageextent[1],imageextent[3]);

		//testing gaussian to be removed!!!!!
	vtkImageData* cacatemp = vtkImageData::New();
	cacatemp->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],0,1);

	 while (theiteration<5)//% && (prevnorm/norm(dCdPhifornorm) <1)
	 {

		 //calculate the similarity metric
		// cacatemp = CalculateImageDerivative(movingimage,3,1);
		FindOverlapDerivative(movingimage,TPvolume,theoverlapderivative,slicenumber);
		//cacatemp = CalculateImageDerivative(movingimage,3,1);
		//calculate dBdT, convolution of moving image
		vtkImageData* dBdTimagedatax = vtkImageData::New();
		dBdTimagedatax->SetOrigin(movingimage->GetOrigin()); //without this nothingness appears
		dBdTimagedatax->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],0,1);
		dBdTimagedatax->SetSpacing(movingimage->GetSpacing());//(mri_spacing[0],mri_spacing[1],mri_spacing[2]); 
		dBdTimagedatax->SetScalarTypeToFloat();
		dBdTimagedatax = CalculateImageDerivative(movingimage,3,1);
	
		vtkImageData* dBdTimagedatay = vtkImageData::New();
		dBdTimagedatay->SetOrigin(movingimage->GetOrigin()); //without this nothingness appears
		dBdTimagedatay->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],0,1);
		dBdTimagedatay->SetSpacing(movingimage->GetSpacing());//(mri_spacing[0],mri_spacing[1],mri_spacing[2]); 
		dBdTimagedatay->SetScalarTypeToFloat();
		dBdTimagedatay = CalculateImageDerivative(movingimage,slicenumber,2);

		 //calculate dCdPhi = 1/norm(i) sum_i (dC/dB * dB/dT * dT/dphi)
		matrix<double> dCdPhix(numcp*numcp, 1);
		matrix<double> dCdPhiy(numcp*numcp, 1);
		FinddCdPhi(numcp, theoverlapderivative,dBdTimagedatax,dBdTimagedatax,dTdPhiMatrixx,dTdPhiMatrixy,dCdPhix,dCdPhiy);

		//calculate the norm of dCdPhi
		double thenormx = norm_1(dCdPhix);
		double thenormy = norm_1(dCdPhiy);


		 //coeffy(j1,j2)= squeeze(coeffy(j1,j2))-squeeze((themu.*dCdPhi(2,j1,j2))./norm(dCdPhifornorm)) ;
		for(int v = 0; v < numcp; ++v)
		{
			for(int u = 0; u < numcp; ++u)
			{
				control_points(v*numcp+u,0) = control_points(v*numcp+u,0)-0.05*dCdPhix(v*numcp+u,0)/thenormx;
				control_points(v*numcp+u,1) = control_points(v*numcp+u,1)-0.05*dCdPhiy(v*numcp+u,0)/thenormy;
			}
		}

		//apply transformation
		movingimageregistered=ApplyBsplineDeformation(control_points,movingimage,numcp,slicenumber);
		 theiteration++;
	 }

}

	//transform the image using the grid

	return movingimageregistered;
}


bool BSplineRegistration::FinddCdPhi(int numcp, ublas::matrix<double>& theoverlapderivative,vtkImageData* dBdTimagedatax,vtkImageData* dBdTimagedatay,ublas::matrix<double>& dTdPhiMatrixx,ublas::matrix<double>& dTdPhiMatrixy,ublas::matrix<double>& dCdPhix,ublas::matrix<double>& dCdPhiy)
{

	//calculate dCdPhi = 1/norm(i) sum_i (dC/dB * dB/dT * dT/dphi)
	int imageextent[6];
	dBdTimagedatax->GetExtent(imageextent);
vtkFloatArray *scalarsdBdTx = vtkFloatArray::New();
scalarsdBdTx = (vtkFloatArray *)(dBdTimagedatax->GetPointData()->GetScalars());
vtkFloatArray *scalarsdBdTy = vtkFloatArray::New();
scalarsdBdTy = (vtkFloatArray *)(dBdTimagedatay->GetPointData()->GetScalars());

	for(int v = 0; v < numcp; ++v)
	{
		for(int u = 0; u < numcp; ++u)
		{
			double sumx=0,sumy=0;
			int offset2 = (int)((v)*numcp+u);
			for(int j1=0;j1<(imageextent[3]);j1=j1+1) 
			{
				for(int k1=0;k1<(imageextent[1]);k1=k1+1)
				{
					int offset1 = (int)((j1)*imageextent[1]+k1);
				
					float curvalMovingx =  scalarsdBdTx->GetTuple1(offset1);////?/only 1 drxn
					float curvalMovingy =  scalarsdBdTy->GetTuple1(offset1);
					sumx+=theoverlapderivative(k1,j1)*curvalMovingx*dTdPhiMatrixx(offset1,offset2);
					sumy+=theoverlapderivative(k1,j1)*curvalMovingy*dTdPhiMatrixy(offset1,offset2);									
				}
			}
			
			//calculate the norm
			double thenorm = (double)(imageextent[1])*(double)(imageextent[3]);
		//	double thenormx = norm_1(dCdPhix);
		//	double thenormy = norm_1(dCdPhiy);
	for(int v = 0; v < numcp; ++v)
	{
		for(int u = 0; u < numcp; ++u)
		{
			dCdPhix((v)*numcp+u,0)=dCdPhix((v)*numcp+u,0)/thenorm;
			dCdPhiy((v)*numcp+u,0)=dCdPhix((v)*numcp+u,0)/thenorm;
		}}

		}
	}

	return true;
}


bool BSplineRegistration::finddTdPhi(ublas::matrix<double>& control_points,int numcp,int xsize,int ysize,ublas::matrix<double>& dTdPhiMatrix_x,ublas::matrix<double>& dTdPhiMatrix_y)
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
					evaluate_Bspline_basis(j1,xsize ,numcp,hx,theNipvals); 
					for(int u = 0; u < numcp; ++u)
					{
						int offset2 = (int)((v)*numcp+u);
						dTdPhiMatrix_x(offset1,offset2)=theNipvals(0,u);
					}}

					for(int u = 0; u < numcp; ++u)
					{
					evaluate_Bspline_basis(k1,ysize ,numcp,hy,theNipvals); 
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

vtkImageData* BSplineRegistration::CalculateImageDerivative(vtkImageData* movingimage, int slicenumber, int direction)
{
	int imageextent[6];
	movingimage->GetExtent(imageextent);
	vtkImageData* imagederivative = vtkImageData::New();
		imagederivative->SetOrigin(movingimage->GetOrigin()); //without this nothingness appears
		imagederivative->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],0,1);
		imagederivative->SetSpacing(movingimage->GetSpacing());//(mri_spacing[0],mri_spacing[1],mri_spacing[2]); 
		imagederivative->SetScalarTypeToFloat();

		vtkImageGaussianSmooth *GaussianSmooth = vtkImageGaussianSmooth::New();
        GaussianSmooth->SetInput(movingimage);
		GaussianSmooth->SetDimensionality(2); 
		GaussianSmooth->SetStandardDeviation(1,0);

		if(direction==1)
			GaussianSmooth->SetStandardDeviation(1,0);
		else
			GaussianSmooth->SetStandardDeviation(0,1);
        GaussianSmooth->SetRadiusFactor(3);
        GaussianSmooth->Update();
        //YourOutputObject->SetInput( GaussianSmooth->GetOutput() );
        //GaussianSmooth->Delete();

		//imagederivative->setin
		return GaussianSmooth->GetOutput();
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

bool BSplineRegistration::FindOverlapDerivative(vtkImageData* movingimage,vtkImageData* TPvolume, ublas::matrix<double>& overlapderivative, int theSliceNumber)
{
	double overlap = 0.0;
	int imageextent[6];
	movingimage->GetExtent(imageextent);

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
		//get the values for the 2 volumes
		float curvalMoving =  scalarsMoving->GetTuple1(offset1);
		float curvalTP=  scalarsTP->GetTuple1(offset1);
		
		//if both values > 5 then overlap+1, but keep in mind the arms!
		if(((curvalMoving>5) &&(curvalTP<5)) ||((curvalMoving<5) &&(curvalTP>5)))
		{
			overlap+=1;
			overlapderivative(k1,j1)=2;
		}
		else
			overlapderivative(k1,j1)=0;

	}}}

	//delete the float arrays
	//scalarsTP->Delete();
	//scalarsMoving->Delete();
	return true;//2*overlap;
}

vtkImageData* BSplineRegistration::ReadGeneratedTPVolume(vtkImageData* inputvol)
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

void BSplineRegistration::GeneratevolfrpmTP(vtkImageData* inputvol, vtkImageData* theTPvol, int curr_slice, vtkPolyData * polydatasze)
{

	//first generate a distance volume
	int num_components;
	float inputval,maxdist;
	int imageextent[6];
	int subId = 0;
	int cellid = -1;

	//rotate the sze properly. since it is rotated in actor, probably should change that***/

	vtkGeneralTransform *tnoreg44 = vtkGeneralTransform::New ();
	tnoreg44->RotateZ(-90.0); //invert the body back
	tnoreg44->RotateX(90.0);
	vtkTransformPolyDataFilter *f44 = vtkTransformPolyDataFilter::New ();
	f44->SetInput (polydatasze);
	f44->SetTransform (tnoreg44);
	f44->Update ();
	polydatasze=f44->GetOutput();
	//distance map for the registered patient
	vtkCellLocator *cellLocatorreg = vtkCellLocator::New();
	cellLocatorreg->SetDataSet(polydatasze);
	cellLocatorreg->BuildLocator();

	theTPvol->GetExtent(imageextent);//dim = 420:so 0 to 419 inclusive //changed form inputvol

	//the maximum distance is from the middle to the edge
	int intervalx=(int)(imageextent[1]); //from 0-419 to 1-420
	int intervaly=(int)(imageextent[3]);
	int intervalz=(int)(imageextent[5]); 
	maxdist=sqrt(pow(((float)intervalx),2)+pow(((float)intervaly),2)+pow(((float)intervalz),2));

	int midx = (int)(imageextent[1]/2);
	int midy = (int)(imageextent[3]/2);

	int max_offset = intervalx*intervaly*intervalz;//(int)((intervalx-1)*intervaly+(intervaly-1)+(intervalz-1)*intervaly*intervalx);

	
	vtkDataArray *volumedatArray = vtkFloatArray::New();
	vtkDataArray *volumedatArrayBinary = vtkFloatArray::New();

	int i11=0, j11=0, k11=0;

	for( i11=0;i11<=intervalz;i11++)
	{
		for( j11=0;j11<=intervaly;j11++)
		{
			for( k11=0;k11<=intervalx;k11++)
			{
				volumedatArray->InsertNextTuple1(0);
				volumedatArrayBinary->InsertNextTuple1(0);
			}
		}
	}

	float *thedata=new float[intervalx*intervaly*intervalz];
	//theTPvol->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[4],imageextent[5]);//were +1 work
	/* theTPvol = vtkImageData::New();
	theTPvol = inputvol;
	theTPvol->SetOrigin(inputvol->GetOrigin()); //without this nothingness appears
	theTPvol->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[4],imageextent[5]);
	theTPvol->SetSpacing(inputvol->GetSpacing());//(mri_spacing[0],mri_spacing[1],mri_spacing[2]); */
	//theTPvol->DeepCopy(inputvol);

	//this works perfectly fine
	/*	for(int i1=0;i1<1;i1=i1+1) //(intervalz)//(intervalz-4)varying this makes x-y work as a rectangle fine,, so i1*intervaly*intervalx is good
	{
		for(int j1=0;j1<15;j1=j1+1) 
		{
			for(int k1=0;k1<intervalx;k1=k1+1)//intervalx, varying this makes z-y work as a rectangle fine, +k1 is good
			{	
					float* pixel = static_cast<float*>(theTPvol->GetScalarPointer(k1,j1,i1));
					pixel[0] = 200.0;	
			}
		}
	}*/

	for(int i1=curr_slice;i1<curr_slice+2;i1=i1+1) //(intervalz)//(intervalz-4)varying this makes x-y work as a rectangle fine,, so i1*intervaly*intervalx is good
	{
		for(int j1=0;j1<(intervaly-10);j1=j1+10) //intervaly+1 works well when +=1
		{
			for(int k1=0;k1<(intervalx-10);k1=k1+10)//for(int k1=0;k1<(intervalx-49);k1=k1+50)
			{	
				//find the distance to the closest point in sze
				// Compute the distance d between the point m and the closest point c on the surface.
				double theclosestpoint[3], mindist;
				double m[3],d;
			
				int subID, cellNum; 
				double *paraCoords = new double[3]; 
				double *xyzCoords  = new double[3]; 
				float *vtkcellweights = new float[4]; 
				
				int offset2 = (int)((i1)*(intervaly+1)*(intervalx+1)+(j1)*(intervalx+1)+k1);

				//int *coords= new int[3];
				//coords[0]=k1;coords[1]= j1;coords[2]= i1;
				//vtkIdType id = theTPvol->ComputeCellId(coords);
				//subID = theTPvol->GetCell(offset2)->GetParametricCenter(paraCoords);
				//int &subIDadd3 = subID; 
				//theTPvol->GetCell(offset2)->EvaluateLocation(subIDadd3, paraCoords, xyzCoords, vtkcellweights);
				int coords[3];
				coords[0] = k1; coords[1] = j1; coords[2] = i1;
				double *bounds = new double[6];
				vtkIdType id6 = theTPvol->ComputeCellId(coords); //theTPvol changed from inputvol
				inputvol->GetCell(id6)->GetBounds(bounds);
				xyzCoords[0]=bounds[0];xyzCoords[1]=bounds[2];xyzCoords[2]=bounds[4];
				subID = theTPvol->GetCell(id6)->GetParametricCenter(paraCoords); //theTPvol changed from inputvol
				int &subIDadd3 = subID; 
				cellLocatorreg->FindClosestPoint(xyzCoords ,theclosestpoint, id6, subID, d);

				//coords[0] = 205; coords[1] = 269; coords[2] = i1;
				//id6 = inputvol->ComputeCellId(coords);
				//inputvol->GetCell(id6)->GetBounds(bounds);
				//theclosestpoint[0]=bounds[0];theclosestpoint[1]=bounds[2];theclosestpoint[2]=bounds[4];
				//theclosestpoint[0]=-25;theclosestpoint[1]=-16;theclosestpoint[2]=211;
				for(int jj = 0;jj<10;jj++)
				{
					for(int kk = 0;kk<10;kk++)
					{
						int offset1 = (int)((i1)*(intervaly+1)*(intervalx+1)+(j1+jj)*(intervalx+1)+k1+kk);
						if(offset1 <= max_offset)
						{
							
							coords[0] = k1+kk; coords[1] = j1+jj; coords[2] = i1;
							vtkIdType id3 = inputvol->ComputeCellId(coords);
							theTPvol->GetCell(id3)->GetBounds(bounds); //changed from inputvol
							xyzCoords[0]=bounds[0];xyzCoords[1]=bounds[2];xyzCoords[2]=bounds[4];

							//coords[0]=k1+kk;coords[1]= j1+jj;coords[2]= i1;
							//vtkIdType id3 = inputvol->ComputeCellId(coords);
							d=sqrt(pow((xyzCoords[0]-theclosestpoint[0]),2)+pow((xyzCoords[1]-theclosestpoint[1]),2));//+pow((theclosestpoint[2]-xyzCoords[2]),2));
							//d=xyzCoords[0];
							//d = (float)k1;
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
		for(int j1=10;j1<(intervaly-10);j1=j1+1) 
		{
			bool inside=false;
			float previousdist=1000000000;
			float thedist, previousdist2,previousdist3, nextdist, nextdist2, nextdist3;
			int lastswitch = 0;
			for(int k1=10;k1<(intervalx-10);k1=k1+1)//intervalx, varying this makes z-y work as a rectangle fine, +k1 is good
			{	
				int offset1 = (int)((i1)*(intervaly+1)*(intervalx+1)+(j1)*(intervalx+1)+k1);
				previousdist=volumedatArray->GetTuple1(offset1-1);
				previousdist2 = volumedatArray->GetTuple1(offset1-3);
				previousdist3= volumedatArray->GetTuple1(offset1-7);
				thedist=volumedatArray->GetTuple1(offset1);
				nextdist=volumedatArray->GetTuple1(offset1+1);
				nextdist2 = volumedatArray->GetTuple1(offset1+3);
				nextdist3 = volumedatArray->GetTuple1(offset1+7);
				if((thedist<3.0) && (thedist<previousdist) &&  (thedist<previousdist2)&&  (thedist<previousdist3)&&(thedist<nextdist)&&  (thedist<nextdist2)&&  (thedist<nextdist3))//we have reached a min
				{
					if((lastswitch < (k1-50)) )
					{
						inside=!inside;
					}
				}
				if(inside)
				{
					volumedatArrayBinary->SetTuple1(offset1,255);
					//thedata2[offset1]=255;
				}
				//else
				//	thedata2[offset1]=0;
				previousdist=thedist;
			}}}

			
	/*std::ofstream myFilereg ("tpvolume.bin", ios::out | ios::binary);
	myFilereg.write ((char*) &num_components2, sizeof(int));
	myFilereg.write ((char*) thedata2, num_components2*sizeof(float));//2* sizeof(int)+storedatasource.num_components*sizeof(float));
	myFilereg.close();
	*/
//	theTPvol->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[4],imageextent[5]);

	theTPvol->SetScalarTypeToFloat();
	theTPvol->AllocateScalars();
	theTPvol->GetPointData()->CopyAllOn();
	theTPvol->GetPointData()->CopyScalarsOn();
	theTPvol->GetPointData()->SetScalars(volumedatArray);//(volumedatArrayBinary);
	
	cellLocatorreg->Delete();
	volumedatArray->Delete();
	volumedatArrayBinary->Delete();


	//save the volume cause it is long
//	return true;
//	return theTPvol;
}

vtkImageData* BSplineRegistration::ApplyBsplineDeformation(ublas::matrix<double>& control_points_overallv,vtkImageData* movingimage,int numcp,int moving_index)
{


	//use the basis functions to calculate deformations for every grid point

	int ordre=4; //p+1
	int p=ordre-1; //dergre de la courbe (==ordre?)
	int n=numcp-1;//nombre de points de conrole-1 (n+1 points)
	int m=n+p+1; //il y m+1 noeuds
	int nb_vectNodal=m+1;//ou points de controle + ordre
	matrix<double> Nip(n+1, n+1);
	matrix<double> invNip(n+1, n+1);
	matrix<double> vecteur_nodal_u(nb_vectNodal, n+1);
	matrix<double> vecteur_nodal_v(nb_vectNodal, n+1);

	int uinterval,vinterval;
	int imageextent[6];
	movingimage->GetExtent(imageextent);
	int newmri_dimz=imageextent[5];//(int) mri_dimz;///2,/4,/4
	int newmri_dimy=imageextent[3];//(int) mri_dimy;
	int newmri_dimx=imageextent[1];//(int) mri_dimx;
	int numrows = newmri_dimy;//imageextent[3]-imageextent[2];
	int numcols = newmri_dimx;//imageextent[1]-imageextent[0];

	matrix<double> ul(numcols, 1);
	matrix<double> vl(numrows, 1);
	matrix<double> Cu(numrows, 2);
	double *vals=new double[ordre];
	matrix<double> Cutemp(1, 2);
	matrix<double> Cutemp2(1, 2);
	vtkFloatArray *scalarsdeformation = vtkFloatArray::New();
	scalarsdeformation->SetNumberOfComponents(3);


	for (int i=0;i<newmri_dimz;i++)
	{
		for (int j=0;j<newmri_dimy;j++)
		{
			for (int k=0;k<newmri_dimx;k++)
			{

				scalarsdeformation->InsertNextTuple3(0,0,0);
			}}}


	//control_points_overall(index_section*(n+1)+i, 0)=control_points(i,0);
	for ( int index_section=0; index_section<numcp; index_section++ )
	{//v*numcp+u
		for ( int bb=0; bb<4; bb++ )
		{
			vecteur_nodal_u(bb,index_section)=0;
			vecteur_nodal_u(bb+n+1,index_section)=1;
		}

		for ( int bb=0; bb<n+1-ordre; bb++ ) /////i<n+1 in archembaults`s code Rinteration*(n+1)+j, 0
		{//index_section*(n+1)+i, 0
			vecteur_nodal_u(bb+4,index_section)=control_points_overallv(index_section*(n+1)+bb, 0)/control_points_overallv(index_section*(n+1)+numcp-1, 0);//(double)i/(double)(n+1-ordre);
		}

		for ( int bb=0; bb<4; bb++ )
		{
			vecteur_nodal_v(bb,index_section)=0;
			vecteur_nodal_v(bb+n+1,index_section)=1;
		}

		for (int bb=0; bb<n+1-ordre; bb++ ) /////i<n+1 in archembaults`s code
		{//problem when bb=1
			double tempval=control_points_overallv(numcp*(n)+numcp-1, 1);
			vecteur_nodal_v(bb+4,index_section)=control_points_overallv(bb*(n+1)+index_section, 1)/tempval;//(double)i/(double)(n+1-ordre);
		}
	}


	for(int i=0;i<numcols;i++)
		ul(i,0)=(double)i/(double)(numcols);

	matrix<double> Nipu(1, n+1); //one row on matrix, for one parameter at a time
	matrix<double> Ril(n+1, 2);
	matrix<double> Riltemp(1, 2);
	int interpi, interpj;
	double hx = (double)numcols/(double)numcp;
	double hy = (double)numrows/(double)numcp;

	for(interpi=0;interpi<numrows;interpi++) //if only one of them dnt work
	{
		for(interpj=0;interpj<numcols;interpj++)
		{
			matrix<double> control_pointsx(n+1, 1);
			matrix<double> control_pointsy(n+1, 1);
			matrix<double> Niprows(1, numcp);
			matrix<double> Nipcols(1, numcp);
			double deformationx=0, deformationy=0;
			matrix<double> Cutemp2(1, 1);

			//calculate the row basis functions and for each row, multiply a row of cp by colum of  basis functions
			evaluate_Bspline_basis(interpj,numcols ,numcp,hx,Niprows); 
			for (int ycontrolpoint=0;ycontrolpoint<=n;ycontrolpoint++)
			{
				//do this for each row
				int baseme = ycontrolpoint*(n+1);
				for (int xcontrolpoint=0;xcontrolpoint<=n;xcontrolpoint++) 
					control_pointsx(xcontrolpoint,0)=control_points_overallv(baseme+xcontrolpoint, 0); //i really dont think it's vinterval

				 Cutemp2=prod(Niprows,control_pointsx);
				 deformationx+=Cutemp2(0,0);
			}

			evaluate_Bspline_basis(interpi,numrows ,numcp,hy,Nipcols); 
			for (int xcontrolpoint=0;xcontrolpoint<=n;xcontrolpoint++) 
			{
				for (int ycontrolpoint=0;ycontrolpoint<=n;ycontrolpoint++)
					control_pointsy(ycontrolpoint,0)=control_points_overallv(ycontrolpoint*(n+1)+xcontrolpoint, 1);

				Cutemp2=prod(Nipcols,control_pointsy);
				 deformationy+=Cutemp2(0,0);
			}
			/*for (int xcontrolpoint=0;xcontrolpoint<=n;xcontrolpoint++) 
			{
			int ggg=0;
			control_points(ggg,0)=control_points_overallv(ycontrolpoint*(n+1)+xcontrolpoint, 0); //i really dont think it's vinterval
			control_points(ggg,1)=control_points_overallv(ycontrolpoint*(n+1)+xcontrolpoint, 1);
			valsx=evaluate_Bspline_basis(ordre,n,nb_vectNodal,vecteur_nodal_u,ul(interpi,0)); 
			def[0]+=control_points(ggg,0)*vals[0];
			}*/

//deformationx=100.0;
//deformationy=100.0;
		int offset13 = (int)(numcols*numrows*moving_index+(numcols*interpj)+interpi); //offset tested and working
		scalarsdeformation->SetTuple3(offset13,(float)(deformationx),(float)(deformationy),0);
		//scalarsdeformation->SetTuple3(offset13,100,100,0);
		/*
		//find u and v intervals
		uinterval=trouver_index_intervalle( ul(interpi,0), ordre, n, nb_vectNodal,vecteur_nodal_u )-ordre+1;
		vinterval=trouver_index_intervalle( vl(interpj,0), ordre, n, nb_vectNodal,vecteur_nodal_v)-ordre+1;
		//find N vector for u drxn
		matrix<double> Nipuv(1, n+1);
		for(int j=0;j<=n;j++)
		Nipuv(0,j)=0;

		vals=evaluate_Bspline_basis(ordre,n,nb_vectNodal,vecteur_nodal_u,ul(interpi,0)); 

		for (int j=0;j<ordre;j++)
		Nipuv(0,uinterval+j)=vals[j];

		//get control points for a particular v
		int whichcntrlpnt = (int)(vl(interpj,0)*(n+1));

		for (int Rinteration=0;Rinteration<=n;Rinteration++)
		{
		for (int j=0;j<=n;j++) //j is u , the problem seems to be here ...
		{
		control_points(j,0)=control_points_overallv(Rinteration*(n+1)+j, 0); //i really dont think it's vinterval
		control_points(j,1)=control_points_overallv(Rinteration*(n+1)+j, 1);
		//	control_points(j,2)=control_points_overallv(Rinteration*(n+1)+j, 2);
		//the u one : index_section*(n+1)+i
		}
		//calculate intermediate Ril VECTOR
		Riltemp=prod(Nipuv,control_points); //is 1 row 3 columns
		Ril(Rinteration,0)=Riltemp(0,0);
		Ril(Rinteration,1)=Riltemp(0,1);
		//	Ril(Rinteration,2)=Riltemp(0,2);
		}

		//find N vector for v drxn
		for(int j=0;j<=n;j++)
		Nipuv(0,j)=0;
		vals=evaluate_Bspline_basis(ordre,n,nb_vectNodal,vecteur_nodal_v,vl(interpj,0)); //vecteur_param_ubardont matter	
		for (int j=0;j<ordre;j++)
		{
		double valj = vals[j];
		Nipuv(0,vinterval+j)=vals[j];
		}
		//Nipuv(0,uinterval+j)=vals[j];
		//Cu=Nip(u)*Ril
		//control points in different direction (particular u)
		//whichcntrlpnt = (int)(ul(interpi,0)*(n+1));

		Cutemp2=prod(Nipuv,Ril); 
		//scalarsdeformation->InsertNextPoint(Cutemp2(0, 0), Cutemp2(0, 1),Cutemp2(0, 2));
		//int offset13 = (int)((newmri_dimy*newmri_dimx*i)+newmri_dimx*(j+jjj)+(k+kkk));

		int offset13 = (int)(numcols*numrows*moving_index+(numcols*interpj)+interpi); //offset tested and working
		scalarsdeformation->SetTuple3(offset13,Cutemp2(0, 0),Cutemp2(0, 1),0);
		*/

}
}

	vtkImageData* deformationgrid=vtkImageData::New();
deformationgrid->SetOrigin(movingimage->GetOrigin()); //without this nothingness appears
deformationgrid->SetSpacing(movingimage->GetSpacing());
deformationgrid->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[4],imageextent[5]);
deformationgrid->SetScalarTypeToFloat();
 deformationgrid->SetNumberOfScalarComponents(3);
	 deformationgrid->AllocateScalars();
	deformationgrid->GetPointData()->CopyAllOn();
	deformationgrid->GetPointData()->CopyScalarsOn();
	deformationgrid->GetPointData()->SetScalars(scalarsdeformation);
	deformationgrid->Update();

movingimage->GetExtent(imageextent);

	vtkGridTransform* deformationtransform=vtkGridTransform::New();
 deformationtransform->SetDisplacementGrid(deformationgrid);
 deformationtransform->SetInterpolationModeToCubic();
 deformationtransform->Update();
 vtkImageReslice* reslicedeformation=vtkImageReslice::New();
 reslicedeformation->SetInput(movingimage);
 reslicedeformation->SetOutputSpacing(movingimage->GetSpacing());
 reslicedeformation->SetOutputExtent(imageextent);//0,newmri_dimx,0,newmri_dimy,0,newmri_dimz);
  reslicedeformation->SetResliceTransform(deformationtransform); 
 reslicedeformation->SetInterpolationModeToLinear();
 reslicedeformation->Update(); 
 //imageData_ax1->SetExtent(0,newmri_dimx,0,newmri_dimy,0,newmri_dimz);
 movingimage = reslicedeformation->GetOutput();

 return movingimage;
}

//vals=evaluate_Bspline_basis(ordre,n,nb_vectNodal,vecteur_nodal_u,ul(interpi,0)); 
bool BSplineRegistration::evaluate_Bspline_basis(int theinterpi,int thenumcols ,int thenumcp, int cpdistance, ublas::matrix<double>& Nipvals)
{
	//Niprows(1, numcp) = evaluate_Bspline_basis(int theinterpi,int thenumcols ,int thenumcp)
	//int p = ordre-1;
	//double *valeurs=new double[ordre];
	//	int depart = BSplineRegistration::trouver_index_intervalle(t, ordre, n, nb_vectNodal,vecteur_nodal );
	double mumu = (double)theinterpi/(double)cpdistance;
	//matrix<double> Nipvals(1, thenumcp);
	for (int jj =0;jj<thenumcp;jj++)
	{
		double argum = mumu-(double)jj;
		if(abs(argum)>=2)
			Nipvals(0,jj)=0;
		else if (argum <= -1)
			Nipvals(0,jj)=0.5*pow(argum,2) +2*argum+2;
		else if (argum <= 0)
			Nipvals(0,jj)=-1.5*pow(argum,2) -2*argum;
		else if (argum <= 1)
			Nipvals(0,jj)=1.5*pow(argum,2) -2*argum;
		else
			Nipvals(0,jj)=-0.5*pow(argum,2) +2*argum-2;
	}
/*
valeurs[0]=1.0/6.0;
valeurs[1]=2.0/3.0;
valeurs[2]=1.0/6.0;
valeurs[3]=0;*/

return true;
}

/*
bool BSplineRegistration::evaluate_Bspline_basis2(int theinterpi,int thenumcols ,int thenumcp, int cpdistance, ublas::matrix<double>& Nipvals)
{
	//Niprows(1, numcp) = evaluate_Bspline_basis(int theinterpi,int thenumcols ,int thenumcp)
	//int p = ordre-1;
	//double *valeurs=new double[ordre];
	//	int depart = BSplineRegistration::trouver_index_intervalle(t, ordre, n, nb_vectNodal,vecteur_nodal );
	double mumu = (double)theinterpi/(double)cpdistance;
	//matrix<double> Nipvals(1, thenumcp);
	for (int jj =0;jj<thenumcp;jj++)
	{
		double argum = mumu-(double)jj;
		if(abs(argum)>=2)
			Nipvals(0,jj)=0;
		else if (argum <= -1)
			Nipvals(0,jj)=0.5*pow(argum,2) +2*argum+2;
		else if (argum <= 0)
			Nipvals(0,jj)=-1.5*pow(argum,2) -2*argum;
		else if (argum <= 1)
			Nipvals(0,jj)=1.5*pow(argum,2) -2*argum;
		else
			Nipvals(0,jj)=-0.5*pow(argum,2) +2*argum-2;
	}

return true;
}
*/

//les 2 fonctions ci-dessous sont copiées de la librairie opgeom
/*
double* BSplineRegistration::evaluate_Bspline_basis( int ordre, int n, int nb_vectNodal, ublas::matrix<double>& vecteur_nodal, double t)
	{
	//   int depart = trouver_noeud1( t );
	int depart = BSplineRegistration::trouver_index_intervalle(t, ordre, n, nb_vectNodal,vecteur_nodal );
	double *valeurs=new double[ordre];
	double *left=new double[ordre];
	double *right=new double[ordre];

	double yaya1=vecteur_nodal(6,0);
	double yaya2=vecteur_nodal(7,0);
	double yaya3=vecteur_nodal(8,0);
	double yaya4=vecteur_nodal(9,0);
	double yaya5=vecteur_nodal(10,0);
	double yaya6=vecteur_nodal(11,0);

	valeurs[0]=1.0;
	int p = ordre-1;
	for( int j=1; j<=p;j++)
		{
		left [j]=t-vecteur_nodal(depart+1-j,0);
		right[j]=vecteur_nodal(depart+j,0)-t;
		double saved = 0.0;
		for (int r=0;r<j;r++)
			{
			double temp = valeurs[r]/(right[r+1]+left[j-r]);
			valeurs[r] = saved + right[r+1]*temp;
			saved = left[j-r]*temp;
			}
		valeurs[j] = saved;
		}
	return valeurs;
	}
*/
/**
* Trouve l'intervalle du vecteur nodal qui contient la valeur t
*/


int BSplineRegistration::trouver_index_intervalle( double t, int ordre, int n, int nb_vectNodal, ublas::matrix<double> vecteur_nodal )
	{
	//valider_parametre( t );
	int dernier_noeud = nb_vectNodal - ordre;
	for( int iknot = ordre-1; iknot < dernier_noeud; ++iknot )
	{
		if( vecteur_nodal(iknot+1,0) > t )
			return iknot;
	}

	return dernier_noeud - 1;
	}