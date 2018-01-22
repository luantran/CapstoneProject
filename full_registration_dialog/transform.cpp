/***************************************************************************
						  transform.cpp  -  description
							 -------------------
	begin                : Tue Dec 16 2003
	copyright            : (C) 2003 by Liao Qinghu
	email                : qing-hu.liao@polymtl.ca

						Rola Harmouche
						Functions for registration of multimodal images
						called by tps.cpp
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//#import "C:/My_Documents/centrale/tps/Debug/vtkMetaImageWriter.o"

#include "stdafx.h"
#include "transform.h"
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
//#include <vtkCleanPolyData.h>
//#include <vtkContourFilter.h>
#include <vtkDataArray.h>
#include <vtkDataSetReader.h>
//#include <vtkDataSetSurfaceFilter.h>
#include <vtkDelaunay3D.h>
//#include <vtkDelaunay2D.h>
//#include <vtkDecimatePro.h>
#include <vtkFloatArray.h>
//#include <vtkFeatureEdges.h>
//#include <vtkGeometryFilter.h>
#include <vtkGeneralTransform.h>
#include <vtkGenericCell.h>
#include <vtkLandmarkTransform.h>
#include <vtkIdList.h>
#include <vtkSystemIncludes.h>
#include <vtkIntArray.h>
#include <vtkImageData.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
//#include <vtkMergePoints.h>
//#include <vtkMeshQuality.h>
//#include <vtkOBBTree.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
//#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPointData.h>
#include <vtkPointLocator.h>
#include <vtkPoints.h>
#include <vtkRenderer.h>
//#include <vtkReverseSense.h>
//#include <vtkSmoothPolyDataFilter.h>
//#include <vtkSTLReader.h>
//#include <vtkSTLWriter.h>
//#include <vtkLoopSubdivisionFilter.h>
//#include <vtkSurfaceReconstructionFilter.h>
#include <vtkThinPlateSplineTransform.h>
#include <vtkTransformPolyDataFilter.h>
//#include <vtkTriangleFilter.h>
//#include <vtkTetra.h>
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
//#include <vtkConeSource.h>
//#include <vtkCubeSource.h>
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
//#include <vtkCellPicker.h> 
//#include <vtk/vtkGdcmReader.h>
//#include <vtk/vtkDICOMImageReader.h>
#include <vtkPlanes.h>
//#include <vtkIterativeClosestPointTransform.h>
#include <vtkColorTransferFunction.h>
#include <vtkImplicitModeller.h>
#include <vtkImageCast.h>
#include <vtkImageEuclideanDistance.h>
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

#include <assert.h> //matt code
#include <iomanip>

int dim[2];
long **textureTab;
int **connect3D;
int NbPolygon;
int **connectTex;
int NbTexture;
float **textureCoord;
int NbPoints;
using namespace std;

//================IO Convert Part===========================================

enum
{ i_sze, i_vtk, i_vrml, i_obj, i_stl, i_3ds, i_o3, i_ext};
static int input_type = i_vtk;
enum
{ o_vtk, o_wrl, o_obj, o_smesh, o_sze};
static int output_type = o_vtk;
  
topo_data
Transform::RegisterTopoTPS (topo_data unregisteredtopo, xray_data unregisteredxray)
{
	vtkThinPlateSplineTransform *Transtps= vtkThinPlateSplineTransform::New ();
	Transtps->SetSourceLandmarks (unregisteredtopo.polydata_szelandmarks->GetPoints());
	Transtps->SetTargetLandmarks (unregisteredxray.polydata_wrllandmarks->GetPoints());

	Transtps->SetBasisToR2LogR();
	Transtps->Update ();


		vtkLandmarkTransform *Transrigid = vtkLandmarkTransform::New ();
	Transrigid->SetSourceLandmarks (unregisteredtopo.polydata_szelandmarks->GetPoints());
	Transrigid->SetTargetLandmarks (unregisteredxray.polydata_wrllandmarks->GetPoints());
	Transrigid->SetModeToRigidBody();
	Transrigid->Update ();

	//rotate by this much, transform then rotate back
	vtkGeneralTransform *tnoreg44 = vtkGeneralTransform::New ();
	tnoreg44->RotateZ(-90.0); //invert the body back
	tnoreg44->RotateX(90.0);
	vtkTransformPolyDataFilter *f44 = vtkTransformPolyDataFilter::New ();
	f44->SetInput (unregisteredtopo.polydatasze);
	 f44->SetTransform (tnoreg44);
	f44->Update ();
	unregisteredtopo.polydatasze=f44->GetOutput(); //might be problematic

	vtkTransformPolyDataFilter *ftps_sze = vtkTransformPolyDataFilter::New ();
	ftps_sze->SetInput (unregisteredtopo.polydatasze); //original
	ftps_sze->SetTransform (Transtps);
	ftps_sze->Update ();
	unregisteredtopo.polydatasze=ftps_sze->GetOutput();

	//now rotate back
	vtkGeneralTransform *tnoreg45 = vtkGeneralTransform::New ();
	tnoreg45->RotateZ(-90.0); //invert the body back
	tnoreg45->RotateX(90.0);
	tnoreg45->Inverse();
		vtkTransformPolyDataFilter *f45 = vtkTransformPolyDataFilter::New ();
	f45->SetInput (unregisteredtopo.polydatasze);
	 f45->SetTransform (tnoreg45);
	f45->Update ();
	unregisteredtopo.polydatasze=f45->GetOutput();

	vtkTransformPolyDataFilter *ftps_szemarks = vtkTransformPolyDataFilter::New ();
	ftps_szemarks->SetInput (unregisteredtopo.polydata_szelandmarks); //original
	ftps_szemarks->SetTransform (Transtps);
	ftps_szemarks->Update ();
	unregisteredtopo.polydata_szelandmarks=ftps_szemarks->GetOutput();
	unregisteredtopo.szelandmark_points = unregisteredtopo.polydata_szelandmarks->GetPoints();

	return unregisteredtopo;
}


//vtkMatrix4x4** 
/*vtkMatrix4x4 ** Transform::readArticulatedTranfos(char o3txfile[])
{
	char temp_string_wrl[200];
	float thetrans[3]={0,0,0};
	float themat[9];
std::ifstream numbers_strtxo3( o3txfile );
	numbers_strtxo3.seekg(0, ios::beg);

	vtkMatrix4x4 **volo3 = *vtkMatrix4x4[17];

for(int i=0;i<17;i++)
	{
		volo3[i]= vtkMatrix4x4::New();
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volo3[i]->SetElement(0,3,thetrans[0]); 
		volo3[i]->SetElement(1,3,thetrans[1]); 
		volo3[i]->SetElement(2,3,thetrans[2]);
		volo3[i]->SetElement(3,3,1);
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);
		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volo3[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}
		
	}

return volo3;
}*/

vtkPolyData *
Transform::read (string input_filename)
{
  string::size_type len, pos;
  // Determine the format of the input file by the extension
  len = input_filename.length ();
  pos = input_filename.rfind (".", len);

	  string ext = input_filename.substr (pos + 1, len - pos);
	  if ((ext == "sze") || (ext == "SZE"))
				input_type = i_sze;
		else if ((ext == "vtk") || (ext == "VTK"))
				input_type = i_vtk;
	  else if ((ext == "landmark") || (ext == "LANDMARK"))
				input_type = i_vtk;
	  else if ((ext == "obj") || (ext == "OBJ"))
				input_type = i_obj;
	  else if ((ext == "o3") || (ext == "O3"))
				input_type = i_o3;
	  else if ((ext == "wrl") || (ext == "WRL"))
				input_type = i_vrml;
	  else if ((ext == "ext") || (ext == "EXT"))
				input_type = i_ext;
	  else
	{
	  cout << "The input filename haven't correct extension format darn it" << endl;
	  exit (0);
	}

  // use polydata to store the data from the input.
  vtkPolyData *polydata = vtkPolyData::New ();
  vtkImageData *imageData = vtkImageData::New ();
  // read the data.
  vtkSZEImporter *szereader;
  vtkPolyDataReader *polyReader;
  vtkVRMLImporter *vrmlimport;
  vtkAppendPolyData *append;
  vtkActorCollection *collection;
  vtkActor *actor;
  vtkTriangleFilter *triangulate;
   switch (input_type)
	{
	case i_sze:
	  szereader = vtkSZEImporter::New ();
	  szereader->SetFileName (input_filename.c_str ());
	  szereader->Update ();
	  polydata->DeepCopy (szereader->GetPolyData ());
	  imageData->DeepCopy(szereader->GetImageData());
	  szereader->GetImageData()->GetDimensions(dim);
	  textureTab=szereader->getTexture();
		
	  connect3D=szereader->get3DConnect();
	  NbPolygon=szereader->getNbPolygon();
	  connectTex=szereader->getConnectTex();
	  NbTexture=szereader->getNbTexture();
	  textureCoord=szereader->getTextureCoord();
	  NbPoints=szereader->getNbPoints();
	  szereader->Delete ();
	break;
	case i_vtk:
	  polyReader = vtkPolyDataReader::New ();
	  polyReader->SetFileName (input_filename.c_str ());
	  polyReader->Update ();
	  polydata->DeepCopy (polyReader->GetOutput ());
	  polyReader->Delete ();
	  break;
	case i_vrml:
	  vrmlimport = vtkVRMLImporter::New ();
	  vrmlimport->SetFileName (input_filename.c_str ());
	  vrmlimport->Read ();
	  append = vtkAppendPolyData::New ();
	  collection = vrmlimport->GetRenderer ()->GetActors ();
	  collection->InitTraversal ();
	  actor = collection->GetNextActor ();
	  int i=0;
	  while (actor != NULL)
	{	actor->GetProperty()->SetAmbientColor(i,1-i,0.0);
	actor->GetProperty()->SetAmbient(1);
	  append->AddInput ((vtkPolyData *) actor->GetMapper ()->GetInput ());
	  actor = collection->GetNextActor ();
	  i=i+0.05;
	}
	  append->Update ();
	  polydata->DeepCopy (append->GetOutput ());
	  append->Delete ();
	  vrmlimport->Delete ();
	  break;  
	}

  triangulate = vtkTriangleFilter::New ();
  triangulate->SetInput (polydata);
  triangulate->PassLinesOn ();
  triangulate->Update ();

  return triangulate->GetOutput ();
}



rigid_vertebrae
Transform::read_wrlsep (string input_filename)
{
	string::size_type len, pos;
	// Determine the format of the input file by the extension
	len = input_filename.length ();
	pos = input_filename.rfind (".", len);
	string ext = input_filename.substr (pos + 1, len - pos);

	input_type = i_vrml;				
	cout<<"reading vrml file:"<< input_filename.c_str()<<endl;
	// use polydata to store the data from the input.
	//vtkPolyData **polydata = *vtkPolyData::New ();
	vtkPolyData *polydata2 = vtkPolyData::New ();
	//vtkPolyData **polydata = new vtkPolyData*[20];
	rigid_vertebrae thevertebrae;
	thevertebrae.polydata=new vtkPolyData*[50];
	thevertebrae.num_vertebrae=0;
	vtkImageData *imageData = vtkImageData::New ();
	// read the data.
	//vtkSZEImporter *szereader;
	vtkPolyDataReader *polyReader;
	vtkVRMLImporter *vrmlimport;
	vtkAppendPolyData *append;
	vtkActorCollection *collection;
	vtkActor *actor;
	vtkTriangleFilter *triangulate;
	cout<<"importing vrml"<<endl;
	vrmlimport = vtkVRMLImporter::New ();
	vrmlimport->SetFileName (input_filename.c_str ());
	vrmlimport->Read ();
	append = vtkAppendPolyData::New ();
	collection = vrmlimport->GetRenderer ()->GetActors ();
	collection->InitTraversal ();
	actor = collection->GetNextActor ();
	int i=0;
	cout<<"importing vrml some more"<<endl;
	for(i=0;i<50;i++)
		thevertebrae.polydata[i] = vtkPolyData::New ();
	i=0;
	int j=0;
	while ((actor != NULL) && (j<50))
	{	
		//actor->GetProperty()->SetAmbientColor(i,1-i,0.0);
		//actor->GetProperty()->SetAmbient(1);
		append->RemoveAllInputs();
		append->AddInput ((vtkPolyData *) actor->GetMapper ()->GetInput ());
		actor = collection->GetNextActor ();
		//polydata->DeepCopy (append->GetOutput ())
		// polydata2=(vtkPolyData *) actor->GetMapper ()->GetInput ();
		 // cout<<"yyyy:"<<j<<endl;
		//polydata[0]->DeepCopy (polydata2);
		append->Update ();
		thevertebrae.polydata[j]->DeepCopy (append->GetOutput ());
		//cout<<"j="<<j<<endl;
		triangulate = vtkTriangleFilter::New ();
		triangulate->SetInput (thevertebrae.polydata[j]);
		triangulate->PassLinesOn ();
		triangulate->Update ();

		thevertebrae.polydata[j]=triangulate->GetOutput ();

		//cout<<"yyyy2:"<<j<<endl;
		j++;
		thevertebrae.num_vertebrae++;
	}
	//	  cout<<"out of loop"<<j<<endl;
	append->Update ();
	//polydata[0]->DeepCopy (append->GetOutput ());
	i=i+0.02;
	j=j+1;


	//append->Update ();
	// polydata->DeepCopy (append->GetOutput ());
	//append->Delete ();
	vrmlimport->Delete ();
	cout<<"out of loop2    "<<j<<endl;
	//return polydata;
	return thevertebrae;
	//  return triangulate->GetOutput ();
}


/******************************
Transform::read_spinal_marks_mri
Inputs: MRI filename for which the landmarks are to be read 
********************************/

landmark_data_mri
Transform::read_spinal_marks_mri(string input_filenamemri)
{
double   x_mri, y_mri, z_mri;
string::size_type len, pos; 	
char  temp_string2[200],temp_string3[200];
int num_lines =0, i=0,n;
string mri_marker_file;

len = input_filenamemri.length ();
pos = input_filenamemri.rfind (".", len);
string ext = input_filenamemri.substr (pos + 1, len - pos);
string base_namemri = input_filenamemri.substr (0, pos);
string extension3 = ".rep";
string extension5 = "wrl";
char(*vert)[200] = new char[17][200];
strcpy_s(vert[0], "Vertebre_T1");
strcpy_s(vert[1], "Vertebre_T2"); 
strcpy_s(vert[2], "Vertebre_T3"); 
strcpy_s(vert[3], "Vertebre_T4"); 
strcpy_s(vert[4], "Vertebre_T5"); 
strcpy_s(vert[5], "Vertebre_T6"); 
strcpy_s(vert[6], "Vertebre_T7"); 
strcpy_s(vert[7], "Vertebre_T8"); 
strcpy_s(vert[8], "Vertebre_T9"); 
strcpy_s(vert[9], "Vertebre_T10"); 
strcpy_s(vert[10],"Vertebre_T11"); 
strcpy_s(vert[11],"Vertebre_T12"); 
strcpy_s(vert[12],"Vertebre_L1"); 
strcpy_s(vert[13],"Vertebre_L2"); 
strcpy_s(vert[14],"Vertebre_L3"); 
strcpy_s(vert[15],"Vertebre_L4"); 
strcpy_s(vert[16],"Vertebre_L5"); 
strcpy_s(vert[17],"end"); 
char(*ped)[200] = new char[6][200];
strcpy_s(ped[0], "Ped_Inf_D");
strcpy_s(ped[1], "Ped_Inf_G");
strcpy_s(ped[2], "Ped_Sup_D");
strcpy_s(ped[3], "Ped_Sup_G");
strcpy_s(ped[4], "Plat_Inf_Cent");
strcpy_s(ped[5], "Plat_Sup_Cent");
mri_marker_file = base_namemri+extension3;
cout << "mri marker file = " << mri_marker_file <<endl;
landmark_data_mri the_outputdata;
the_outputdata.polydata_mrilandmarks = vtkPolyData::New ();
the_outputdata.mrilandmark_points=vtkPoints::New();

//read the wrl file into buffer
std::ifstream numbers_str2( mri_marker_file.c_str() );
numbers_str2.seekg(0, ios::beg);
int line_countmri=0;

//read lines into string buffer
char(*temp_string_wrl2)[200] = new char[line_countmri][200];
char temp_string_wrl[200];
int num_linesmri=0;
int num_points=0;
int vert_index=0;
int ped_index=0;

the_outputdata.the_landmark = new string[108];

while(!numbers_str2.eof()) 
	  {
	  numbers_str2.getline(temp_string_wrl,200); 
	  strcpy_s(temp_string2,"-2");
	  strcpy_s(temp_string3,"-2");
			sscanf (temp_string_wrl,"%s%s",temp_string2, temp_string3);
			//if(strlen(vert[vert_index])  == strlen(temp_string2)) 
				n=memcmp ( vert[vert_index], temp_string2, strlen(vert[vert_index]) );  		
			if(n==0)
				{
				vert_index++;
				ped_index=0;
				}
			if(strlen(ped[ped_index])  == strlen(temp_string2)) 
			{
				n=memcmp ( ped[ped_index], temp_string2, strlen(ped[ped_index]) );  		
			if((n==0) && (strlen(temp_string2) > 1))
				{		
			  //store the data
			  sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &x_mri, &y_mri, &z_mri);
			  the_outputdata.mrilandmark_points->InsertNextPoint(x_mri, y_mri, z_mri);
			   strcpy_s(temp_string3,vert[vert_index-1]);
			   strcat_s(temp_string3,ped[ped_index]);
			  the_outputdata.the_landmark[num_points].append(temp_string3);
			  num_points++;
			  ped_index++;
			 
				}}}
numbers_str2.close();
the_outputdata.polydata_mrilandmarks->SetPoints(the_outputdata.mrilandmark_points);         
the_outputdata.numelem=num_points; 

return the_outputdata;
	}

landmark_data_mri
Transform::read_spinal_marks_mri_tomo(string mri_marker_file)
{
double   x_mri, y_mri, z_mri;
string::size_type len, pos; 	
char  temp_string2[200],temp_string3[200],temp_string4[200];
int num_lines =0, i=0,n;

char(*vert)[200] = new char[17][200];
strcpy_s(vert[0], "T1");
strcpy_s(vert[1], "T2"); 
strcpy_s(vert[2], "T3"); 
strcpy_s(vert[3], "T4"); 
strcpy_s(vert[4], "T5"); 
strcpy_s(vert[5], "T6"); 
strcpy_s(vert[6], "T7"); 
strcpy_s(vert[7], "T8"); 
strcpy_s(vert[8], "T9"); 
strcpy_s(vert[9], "T10"); 
strcpy_s(vert[10],"T11"); 
strcpy_s(vert[11],"T12"); 
strcpy_s(vert[12],"L1"); 
strcpy_s(vert[13],"L2"); 
strcpy_s(vert[14],"L3"); 
strcpy_s(vert[15],"L4"); 
strcpy_s(vert[16],"L5"); 
//strcpy_s(vert[17],"end"); 
char(*ped)[200] = new char[8][200]; //was 12, 20
strcpy_s(ped[0], "Plat_Inf_Ant");
strcpy_s(ped[1], "Plat_Inf_D");
strcpy_s(ped[2], "Plat_Inf_G");
strcpy_s(ped[3], "Plat_Inf_Post");
strcpy_s(ped[4], "Plat_Sup_Ant");
strcpy_s(ped[5], "Plat_Sup_D");
strcpy_s(ped[6], "Plat_Sup_G");
strcpy_s(ped[7], "Plat_Sup_Post");
/*strcpy_s(ped[0], "Ped_Inf_D");
strcpy_s(ped[1], "Ped_Inf_G");
strcpy_s(ped[2], "Ped_Sup_D");
strcpy_s(ped[3], "Ped_Sup_G");
strcpy_s(ped[4], "Plat_Inf_Cent");
strcpy_s(ped[5], "Plat_Sup_Cent");
strcpy_s(ped[6], "Plat_Inf_D");
strcpy_s(ped[7], "Plat_Inf_G");
strcpy_s(ped[8], "Plat_Sup_D");
strcpy_s(ped[9], "Plat_Sup_G");
strcpy_s(ped[10], "Plat_Sup_Ant");
strcpy_s(ped[11], "Plat_Inf_Ant");*/

cout << "mri marker file = " << mri_marker_file <<endl;
//C:/My_Documents/patient_data_testing/1792684/serie3/points_IRM_serie3.scp
landmark_data_mri the_outputdata;
the_outputdata.polydata_mrilandmarks = vtkPolyData::New ();
the_outputdata.mrilandmark_points=vtkPoints::New();

//read the wrl file into buffer
std::ifstream numbers_str2( mri_marker_file.c_str() );
numbers_str2.seekg(0, ios::beg);
int line_countmri=0;

//read lines into string buffer
char(*temp_string_wrl2)[200] = new char[line_countmri][200];
char temp_string_wrl[200];
int num_linesmri=0;
int num_points=0;
int vert_index=0;
int ped_index=0;

the_outputdata.the_landmark = new string[204];
string verts;
string peds;
string thetemp;

//char *tocompare=new char[200];
char tocompare[200];
strcpy_s(tocompare,"Point:"); 
//char *tocompare2=new char[200];
char tocompare2[200];
strcpy_s(tocompare2,"create"); 

//initialize all points to 0:
for(i=0;i<17;i++)
	{
		for(int j=0;j<8;j++)
			{
				the_outputdata.mrilandmark_points->InsertPoint(i*8+j,0, 0, 0);
				strcpy_s(temp_string3,vert[i]);
				strcat_s(temp_string3,ped[j]);
				the_outputdata.the_landmark[i*8+j].append(temp_string3);
		}
}

while(!numbers_str2.eof()) 
	  {
	  numbers_str2.getline(temp_string_wrl,200); 

	  strcpy_s(temp_string2,"-2");
	  strcpy_s(temp_string3,"-2");
			sscanf (temp_string_wrl,"%s%s%s",temp_string2, temp_string3,temp_string4);
			if((strcmp (temp_string2,tocompare)==0) &&(strcmp (temp_string4,tocompare2)==0))
			{//cout<<temp_string_wrl<<endl;
			thetemp =temp_string3;
			//strncpy(thetemp, temp_string2,strlen(temp_string2));
			len = thetemp.length ();
			pos = thetemp.rfind ("-", len);
			verts = thetemp.substr (1, pos-1);
			peds = thetemp.substr (pos + 1, len - pos-2); //removing one each side for the ""
			for(i=0;i<17;i++)
			{
				
				n=strcmp (verts.c_str(), vert[i]);  
				//cout<<verts.c_str()<<"  " <<vert[i]<<endl;
				if((n==0) && (strlen(temp_string2) > 1))
				{
					//cout<<"right vert"<<endl;
					//got the vertebrae rigt so look for the pedicule
					for(int j=0;j<8;j++)
					{//cout<<peds.c_str()<<"  " <<ped[j]<<endl;
						n=strcmp (peds.c_str(), ped[j]);
						if((n==0) && (strlen(temp_string2) > 1))
						{
							//cout<<"right ped"<<endl;
			  //store the data
			  sscanf (temp_string_wrl,"%s%s%s%lf%lf%lf",temp_string2, temp_string2,temp_string2,&x_mri, &y_mri, &z_mri);
			  the_outputdata.mrilandmark_points->InsertPoint(i*8+j,x_mri, y_mri, z_mri);
				  //->InsertNextPoint(x_mri, y_mri, z_mri);
			   strcpy_s(temp_string3,vert[i]);
			   strcat_s(temp_string3,ped[j]);
			  //the_outputdata.the_landmark[num_points].append(temp_string3);
			  num_points++;
			  cout<<the_outputdata.the_landmark[i*8+j]<<" "<<x_mri<<" "<<y_mri<<" "<<z_mri<<endl;
								}
					}
				}
			}
			}
}


			
numbers_str2.close();
the_outputdata.polydata_mrilandmarks->SetPoints(the_outputdata.mrilandmark_points);         
the_outputdata.numelem=num_points; 
//the_outputdata.numelem=136;//204 
return the_outputdata;
}

landmark_data_mri
Transform::read_spinal_marks_mri_tomo_all(string mri_marker_file)
{
double   x_mri, y_mri, z_mri;
string::size_type len, pos; 	
char temp_string2[200],temp_string3[200],temp_string4[200];
int num_lines =0, i=0;


//C:/My_Documents/patient_data_testing/1792684/serie3/points_IRM_serie3.scp
landmark_data_mri the_outputdata;
the_outputdata.polydata_mrilandmarks = vtkPolyData::New ();
the_outputdata.mrilandmark_points=vtkPoints::New();

//read the wrl file into buffer
std::ifstream numbers_str2( mri_marker_file.c_str() );
numbers_str2.seekg(0, ios::beg);
int line_countmri=0;

//read lines into string buffer
char(*temp_string_wrl2)[200] = new char[line_countmri][200];
char temp_string_wrl[200];
int num_linesmri=0;
int num_points=0;
int vert_index=0;
int ped_index=0;

the_outputdata.the_landmark = new string[204];
string verts;
string peds;
string thetemp;

//char *tocompare=new char[200];
char tocompare[200];
char tocompare2[200];
strcpy_s(tocompare,"Point:"); 
//char *tocompare2=new char[200];
strcpy_s(tocompare2,"create"); 

//initialize all points to 0:
for(i=0;i<17;i++)
	{
		for(int j=0;j<12;j++)
			{
				the_outputdata.mrilandmark_points->InsertPoint(i*12+j,0, 0, 0);
		}
}
i=0;

while(!numbers_str2.eof()) 
	  {
	  numbers_str2.getline(temp_string_wrl,200); 
//cout<<temp_string_wrl<<endl;
	  strcpy_s(temp_string2,"-2");
	  strcpy_s(temp_string3,"-2");
			sscanf (temp_string_wrl,"%s%s%s",temp_string2, temp_string3,temp_string4);
			if((strcmp (temp_string2,tocompare)==0) &&(strcmp (temp_string4,tocompare2)==0))
			{
			thetemp =temp_string3;
			//strncpy(thetemp, temp_string2,strlen(temp_string2));
			len = thetemp.length ();
			pos = thetemp.rfind ("-", len);
			verts = thetemp.substr (1, pos-1);
			peds = thetemp.substr (pos + 1, len - pos-2); //removing one each side for the ""

			  //store the data
			  sscanf (temp_string_wrl,"%s%s%s%lf%lf%lf",temp_string2, temp_string2,temp_string2,&x_mri, &y_mri, &z_mri);
			  the_outputdata.mrilandmark_points->InsertPoint(i,x_mri, y_mri, z_mri);
				i++;
			  the_outputdata.the_landmark[num_points].append(temp_string3);
			  num_points++;
			 // cout<<"i="<<i<<"  "<<the_outputdata.the_landmark[num_points]<<" "<<x_mri<<" "<<y_mri<<" "<<z_mri<<endl;
			}
}


			
numbers_str2.close();
the_outputdata.polydata_mrilandmarks->SetPoints(the_outputdata.mrilandmark_points);         
the_outputdata.numelem=num_points; 
//the_outputdata.numelem=204; 
cout<<"nm points="<<the_outputdata.numelem<<endl;
return the_outputdata;
}







landmark_data_mri
Transform::read_spinal_marks_xray_peds(string input_filenamewrl)
{
double   x_wrl, y_wrl, z_wrl;
string::size_type len, pos; 	
char temp_string2[200],temp_string3[200];
int num_lines =0, i=0,n;
string wrl_marker_file;

len = input_filenamewrl.length ();
pos = input_filenamewrl.rfind (".", len);
string ext = input_filenamewrl.substr (pos + 1, len - pos);
string base_namewrl = input_filenamewrl.substr (0, pos);
string extension3 = ".o3";
string extension5 = "wrl";
/*
char(*vert)[200] = new char[18][200];
strcpy_s(vert[0], "Vertebre_T1");
strcpy_s(vert[1], "Vertebre_T2"); 
strcpy_s(vert[2], "Vertebre_T3"); 
strcpy_s(vert[3], "Vertebre_T4"); 
strcpy_s(vert[4], "Vertebre_T5"); 
strcpy_s(vert[5], "Vertebre_T6"); 
strcpy_s(vert[6], "Vertebre_T7"); 
strcpy_s(vert[7], "Vertebre_T8"); 
strcpy_s(vert[8], "Vertebre_T9"); 
strcpy_s(vert[9], "Vertebre_T10"); 
strcpy_s(vert[10],"Vertebre_T11"); 
strcpy_s(vert[11],"Vertebre_T12"); 
strcpy_s(vert[12],"Vertebre_L1"); 
strcpy_s(vert[13],"Vertebre_L2"); 
strcpy_s(vert[14],"Vertebre_L3"); 
strcpy_s(vert[15],"Vertebre_L4"); 
strcpy_s(vert[16],"Vertebre_L5"); 
strcpy_s(vert[17],"end"); 
char(*ped)[200] = new char[6][200];
strcpy_s(ped[0], "Ped_Inf_D");
strcpy_s(ped[1], "Ped_Inf_G");
strcpy_s(ped[2], "Ped_Sup_D");
strcpy_s(ped[3], "Ped_Sup_G");
strcpy_s(ped[4], "Plat_Inf_Cent");
strcpy_s(ped[5], "Plat_Sup_Cent");
*/
char(*vert2)[100] = new char[17][100];
strcpy_s(vert2[0], "T1");
strcpy_s(vert2[1], "T2"); 
strcpy_s(vert2[2], "T3"); 
strcpy_s(vert2[3], "T4"); 
strcpy_s(vert2[4], "T5"); 
strcpy_s(vert2[5], "T6"); 
strcpy_s(vert2[6], "T7"); 
strcpy_s(vert2[7], "T8"); 
strcpy_s(vert2[8], "T9"); 
strcpy_s(vert2[9], "T10"); 
strcpy_s(vert2[10],"T11"); 
strcpy_s(vert2[11],"T12"); 
strcpy_s(vert2[12],"L1"); 
strcpy_s(vert2[13],"L2"); 
strcpy_s(vert2[14],"L3"); 
strcpy_s(vert2[15],"L4"); 
strcpy_s(vert2[16],"L5"); 

char(*vert)[100] = new char[17][100];
strcpy_s(vert[0], "Vertebre_T1");
strcpy_s(vert[1], "Vertebre_T2"); 
strcpy_s(vert[2], "Vertebre_T3"); 
strcpy_s(vert[3], "Vertebre_T4"); 
strcpy_s(vert[4], "Vertebre_T5"); 
strcpy_s(vert[5], "Vertebre_T6"); 
strcpy_s(vert[6], "Vertebre_T7"); 
strcpy_s(vert[7], "Vertebre_T8"); 
strcpy_s(vert[8], "Vertebre_T9"); 
strcpy_s(vert[9], "Vertebre_T10"); 
strcpy_s(vert[10],"Vertebre_T11"); 
strcpy_s(vert[11],"Vertebre_T12"); 
strcpy_s(vert[12],"Vertebre_L1"); 
strcpy_s(vert[13],"Vertebre_L2"); 
strcpy_s(vert[14],"Vertebre_L3"); 
strcpy_s(vert[15],"Vertebre_L4"); 
strcpy_s(vert[16],"Vertebre_L5"); 
//strcpy_s(vert[17],"end"); 
char(*ped)[100] = new char[6][100]; //was 12, 20
strcpy_s(ped[0], "Ped_Inf_D");
strcpy_s(ped[1], "Ped_Inf_G");
strcpy_s(ped[2], "Ped_Sup_D");
strcpy_s(ped[3], "Ped_Sup_G");
strcpy_s(ped[4], "Plat_Inf_Cent");
strcpy_s(ped[5], "Plat_Sup_Cent");

wrl_marker_file = base_namewrl+extension3;
cout << "wrl marker file = " << wrl_marker_file <<endl;
landmark_data_mri the_outputdata;
the_outputdata.polydata_wrllandmarks= vtkPolyData::New ();
the_outputdata.wrllandmark_points=vtkPoints::New();

//read the wrl file into buffer
std::ifstream numbers_str2( wrl_marker_file.c_str() );
numbers_str2.seekg(0, ios::beg);
int line_countwrl=0;

//read lines into string buffer
char(*temp_string_wrl2)[200] = new char[line_countwrl][200];
char temp_string_wrl[200];
int num_lineswrl=0;
int num_points=0;
int vert_index=0;
int ped_index=0;

the_outputdata.the_landmark = new string[136];

while(!numbers_str2.eof()) 
	  {
	  numbers_str2.getline(temp_string_wrl,200); 
	  strcpy_s(temp_string2,"-2");
	  strcpy_s(temp_string3,"-2");
			sscanf (temp_string_wrl,"%s%s",temp_string2, temp_string3);

				n=memcmp ( vert[vert_index], temp_string3, strlen(vert[vert_index]) );  		
			if(n==0) //reached a new vertebrae
				{		
				vert_index++;
				ped_index=0;
				}
			if(strlen(ped[ped_index])  == strlen(temp_string2)) 
			{
				n=memcmp ( ped[ped_index], temp_string2, strlen(ped[ped_index]) );  		
			if((n==0) && (strlen(temp_string2) > 1))
				{
			  //store the data
			  sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &x_wrl, &y_wrl, &z_wrl);
			  the_outputdata.wrllandmark_points->InsertNextPoint(x_wrl, y_wrl, z_wrl);
			   strcpy_s(temp_string3,vert2[vert_index-1]);

			   strcat_s(temp_string3,ped[ped_index]);
			  the_outputdata.the_landmark[num_points].append(temp_string3);
			  num_points++;
			  ped_index++;
				}}}
numbers_str2.close();
the_outputdata.polydata_wrllandmarks->SetPoints(the_outputdata.wrllandmark_points);         
the_outputdata.numelem=num_points; 
cout <<"donegetting xray points:"<<the_outputdata.numelem <<endl;

return the_outputdata;
	}

landmark_data_mri
Transform::read_spinal_marks_xray8(string input_filenamewrl)
{
double   x_wrl, y_wrl, z_wrl;
string::size_type len, pos; 	
char  temp_string2[200],temp_string3[200];
int num_lines =0, i=0,n;
string wrl_marker_file;

len = input_filenamewrl.length ();
pos = input_filenamewrl.rfind (".", len);
string ext = input_filenamewrl.substr (pos + 1, len - pos);
string base_namewrl = input_filenamewrl.substr (0, pos);
string extension3 = ".o3";
string extension5 = "wrl";

char(*vert)[200] = new char[18][200];
strcpy_s(vert[0], "Vertebre_T1");
strcpy_s(vert[1], "Vertebre_T2"); 
strcpy_s(vert[2], "Vertebre_T3"); 
strcpy_s(vert[3], "Vertebre_T4"); 
strcpy_s(vert[4], "Vertebre_T5"); 
strcpy_s(vert[5], "Vertebre_T6"); 
strcpy_s(vert[6], "Vertebre_T7"); 
strcpy_s(vert[7], "Vertebre_T8"); 
strcpy_s(vert[8], "Vertebre_T9"); 
strcpy_s(vert[9], "Vertebre_T10"); 
strcpy_s(vert[10],"Vertebre_T11"); 
strcpy_s(vert[11],"Vertebre_T12"); 
strcpy_s(vert[12],"Vertebre_L1"); 
strcpy_s(vert[13],"Vertebre_L2"); 
strcpy_s(vert[14],"Vertebre_L3"); 
strcpy_s(vert[15],"Vertebre_L4"); 
strcpy_s(vert[16],"Vertebre_L5"); 
strcpy_s(vert[17],"end"); 

char(*ped)[100] = new char[8][100]; //was 12, 20
/*strcpy_s(ped[0], "Plat_Inf_Ant"); ///goood!!
strcpy_s(ped[1], "Plat_Inf_D");
strcpy_s(ped[2], "Plat_Inf_G");
strcpy_s(ped[3], "Plat_Inf_Post");
strcpy_s(ped[4], "Plat_Sup_Ant");
strcpy_s(ped[5], "Plat_Sup_D");
strcpy_s(ped[6], "Plat_Sup_G");
strcpy_s(ped[7], "Plat_Sup_Post");*/

strcpy_s(ped[0], "Plat_Inf_Ant");
strcpy_s(ped[1], "Plat_Inf_D");
strcpy_s(ped[2], "Plat_Inf_G");
strcpy_s(ped[3], "Plat_Inf_Post");
strcpy_s(ped[4], "Plat_Sup_Ant");
strcpy_s(ped[5], "Plat_Sup_D");
strcpy_s(ped[6], "Plat_Sup_G");
strcpy_s(ped[7], "Plat_Sup_Post");

/*char(*vert2)[100] = new char[17][100];
strcpy_s(vert2[0], "T1");
strcpy_s(vert2[1], "T2"); 
strcpy_s(vert2[2], "T3"); 
strcpy_s(vert2[3], "T4"); 
strcpy_s(vert2[4], "T5"); 
strcpy_s(vert2[5], "T6"); 
strcpy_s(vert2[6], "T7"); 
strcpy_s(vert2[7], "T8"); 
strcpy_s(vert2[8], "T9"); 
strcpy_s(vert2[9], "T10"); 
strcpy_s(vert2[10],"T11"); 
strcpy_s(vert2[11],"T12"); 
strcpy_s(vert2[12],"L1"); 
strcpy_s(vert2[13],"L2"); 
strcpy_s(vert2[14],"L3"); 
strcpy_s(vert2[15],"L4"); 
strcpy_s(vert2[16],"L5"); 

char(*vert)[100] = new char[17][100];
strcpy_s(vert[0], "Vertebre_T1");
strcpy_s(vert[1], "Vertebre_T2"); 
strcpy_s(vert[2], "Vertebre_T3"); 
strcpy_s(vert[3], "Vertebre_T4"); 
strcpy_s(vert[4], "Vertebre_T5"); 
strcpy_s(vert[5], "Vertebre_T6"); 
strcpy_s(vert[6], "Vertebre_T7"); 
strcpy_s(vert[7], "Vertebre_T8"); 
strcpy_s(vert[8], "Vertebre_T9"); 
strcpy_s(vert[9], "Vertebre_T10"); 
strcpy_s(vert[10],"Vertebre_T11"); 
strcpy_s(vert[11],"Vertebre_T12"); 
strcpy_s(vert[12],"Vertebre_L1"); 
strcpy_s(vert[13],"Vertebre_L2"); 
strcpy_s(vert[14],"Vertebre_L3"); 
strcpy_s(vert[15],"Vertebre_L4"); 
strcpy_s(vert[16],"Vertebre_L5"); 
//strcpy_s(vert[17],"end"); 
char(*ped)[100] = new char[8][100]; //was 12, 20
strcpy_s(ped[0], "Plat_Inf_Ant");
strcpy_s(ped[1], "Plat_Inf_D");
strcpy_s(ped[2], "Plat_Inf_G");
strcpy_s(ped[3], "Plat_Inf_Post");
strcpy_s(ped[4], "Plat_Sup_Ant");
strcpy_s(ped[5], "Plat_Sup_D");
strcpy_s(ped[6], "Plat_Sup_G");
strcpy_s(ped[7], "Plat_Sup_Post");
*/
wrl_marker_file = base_namewrl+extension3;
cout << "wrl marker file = " << wrl_marker_file <<endl;
landmark_data_mri the_outputdata;
the_outputdata.polydata_wrllandmarks= vtkPolyData::New ();
the_outputdata.wrllandmark_points=vtkPoints::New();

//read the wrl file into buffer
std::ifstream numbers_str2( wrl_marker_file.c_str() );
numbers_str2.seekg(0, ios::beg);
int line_countwrl=0;

//read lines into string buffer
char(*temp_string_wrl2)[200] = new char[line_countwrl][200];
char temp_string_wrl[200];
int num_lineswrl=0;
int num_points=0;
int vert_index=1;
int ped_index=0;

the_outputdata.the_landmark = new string[136];

while((!numbers_str2.eof()) && ((vert_index <17) || (ped_index<8) ))
	  {
	  numbers_str2.getline(temp_string_wrl,200); 
	  strcpy_s(temp_string2,"-2");
	  strcpy_s(temp_string3,"-2");
			sscanf (temp_string_wrl,"%s%s",temp_string2, temp_string3);

				n=memcmp ( vert[vert_index], temp_string3, strlen(vert[vert_index]) );  		
			if(n==0) //reached a new vertebrae
				{		
				vert_index++;
				ped_index=0;
				}
			if(strlen(ped[ped_index])  == strlen(temp_string2)) 
			{
				n=memcmp ( ped[ped_index], temp_string2, strlen(ped[ped_index]) );  		
			if((n==0) && (strlen(temp_string2) > 1))
				{
			  //store the data
					//cout<<"point: "<<temp_string2<<" number: "<<num_points<<endl;	
			  sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &x_wrl, &y_wrl, &z_wrl);
			  the_outputdata.wrllandmark_points->InsertNextPoint(x_wrl, y_wrl, z_wrl);
			   strcpy_s(temp_string3,vert[vert_index-1]);	
			   strcat_s(temp_string3,ped[ped_index]);
			  the_outputdata.the_landmark[num_points].append(temp_string3);
			  num_points++;
			  ped_index++;
				cout<<"stored point: "<<x_wrl<<" "<<y_wrl<<" "<<z_wrl<<" "<<temp_string3<<endl;
				}}}
cout<<"done points"<<endl;
numbers_str2.close();
the_outputdata.polydata_wrllandmarks->SetPoints(the_outputdata.wrllandmark_points);         
the_outputdata.numelem=num_points; 
cout <<"donegetting xray points:"<<the_outputdata.numelem <<endl;

return the_outputdata;
	}



landmark_data_mri
Transform::read_spinal_marks_xray(string input_filenamewrl)
{
double   x_wrl, y_wrl, z_wrl;
string::size_type len, pos; 	
char temp_string2[200],temp_string3[200];
int num_lines =0, i=0,n;
string wrl_marker_file;

len = input_filenamewrl.length ();
pos = input_filenamewrl.rfind (".", len);
string ext = input_filenamewrl.substr (pos + 1, len - pos);
string base_namewrl = input_filenamewrl.substr (0, pos);
string extension3 = ".o3";
string extension5 = "wrl";

char(*vert)[200] = new char[18][200];
strcpy_s(vert[0], "Vertebre_T1");
strcpy_s(vert[1], "Vertebre_T2"); 
strcpy_s(vert[2], "Vertebre_T3"); 
strcpy_s(vert[3], "Vertebre_T4"); 
strcpy_s(vert[4], "Vertebre_T5"); 
strcpy_s(vert[5], "Vertebre_T6"); 
strcpy_s(vert[6], "Vertebre_T7"); 
strcpy_s(vert[7], "Vertebre_T8"); 
strcpy_s(vert[8], "Vertebre_T9"); 
strcpy_s(vert[9], "Vertebre_T10"); 
strcpy_s(vert[10],"Vertebre_T11"); 
strcpy_s(vert[11],"Vertebre_T12"); 
strcpy_s(vert[12],"Vertebre_L1"); 
strcpy_s(vert[13],"Vertebre_L2"); 
strcpy_s(vert[14],"Vertebre_L3"); 
strcpy_s(vert[15],"Vertebre_L4"); 
strcpy_s(vert[16],"Vertebre_L5"); 
strcpy_s(vert[17],"end"); 
char(*ped)[200] = new char[6][200];
strcpy_s(ped[0], "Ped_Inf_D");
strcpy_s(ped[1], "Ped_Inf_G");
strcpy_s(ped[2], "Ped_Sup_D");
strcpy_s(ped[3], "Ped_Sup_G");
strcpy_s(ped[4], "Plat_Inf_Cent");
strcpy_s(ped[5], "Plat_Sup_Cent");

wrl_marker_file = base_namewrl+extension3;
landmark_data_mri the_outputdata;
the_outputdata.polydata_wrllandmarks= vtkPolyData::New ();
the_outputdata.wrllandmark_points=vtkPoints::New();

//read the wrl file into buffer
std::ifstream numbers_str2( wrl_marker_file.c_str() );
numbers_str2.seekg(0, ios::beg);
int line_countwrl=0;

//read lines into string buffer
char(*temp_string_wrl2)[200] = new char[line_countwrl][200];
char temp_string_wrl[200];
int num_lineswrl=0;
int num_points=0;
int vert_index=0;
int ped_index=0;

the_outputdata.the_landmark = new string[136];

while(!numbers_str2.eof()) 
	  {
	  numbers_str2.getline(temp_string_wrl,200); 
	  strcpy_s(temp_string2,"-2");
	  strcpy_s(temp_string3,"-2");
			sscanf (temp_string_wrl,"%s%s",temp_string2, temp_string3);

				n=memcmp ( vert[vert_index], temp_string3, strlen(vert[vert_index]) );  		
			if(n==0) //reached a new vertebrae
				{		
				vert_index++;
				ped_index=0;
				}
			if(strlen(ped[ped_index])  == strlen(temp_string2)) 
			{
				n=memcmp ( ped[ped_index], temp_string2, strlen(ped[ped_index]) );  		
			if((n==0) && (strlen(temp_string2) > 1))
				{
			  //store the data
			  sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &x_wrl, &y_wrl, &z_wrl);
			  the_outputdata.wrllandmark_points->InsertNextPoint(x_wrl, y_wrl, z_wrl);
			   strcpy_s(temp_string3,vert[vert_index-1]);

			   strcat_s(temp_string3,ped[ped_index]);
			  the_outputdata.the_landmark[num_points].append(temp_string3);
			  num_points++;
			  ped_index++;
				}}}
numbers_str2.close();
the_outputdata.polydata_wrllandmarks->SetPoints(the_outputdata.wrllandmark_points);         
the_outputdata.numelem=num_points; 
cout <<"donegetting xray points:"<<the_outputdata.numelem <<endl;

return the_outputdata;
	}


topo_data
Transform::read_landmarks_topo (string input_filenamesze, topo_data thedata)
{
	//topo_data thedata;
	thedata.polydata_szelandmarks =  vtkPolyData::New ();
	thedata.szelandmark_points=vtkPoints::New();
	double  x_sze, y_sze, z_sze;
	  string::size_type len, pos; 	
char temp_string[200], temp_string2[200], temp_string3[200],c;
int num_lines =0, n,n2,j, line_count=0, num_points=0;
string sze_marker_file;

len = input_filenamesze.length ();
pos = input_filenamesze.rfind (".", len);
string ext = input_filenamesze.substr (pos + 1, len - pos);

string base_name = input_filenamesze.substr (0, pos);
string extension4 = ".ext";
string extension6 = "sze";

 sze_marker_file = base_name+extension4;

// find out number of lines
std::ifstream numbers_strsze( sze_marker_file.c_str() );
numbers_strsze.seekg(0, ios::beg);
numbers_strsze.seekg(0, ios::end);
pos = numbers_strsze.tellg();
while(pos != 0)
   {
	   pos -= 1;
		numbers_strsze.seekg(pos);
		c = numbers_strsze.peek();
		if(c == '\n') 
		{
			pos-=1;line_count++;
		}
	 }

numbers_strsze.seekg(0, ios::beg);


char(*temp_string_sze2)[200] = new char[line_count][200];

for(num_lines=0;num_lines<line_count;num_lines++)
	{
		  numbers_strsze.getline(temp_string_sze2[num_lines],200);
	}
numbers_strsze.close();


thedata.the_landmark = new string[line_count];
//parse string between spaces
	for(j=0;j<line_count;j++)
	  {
		  sscanf (temp_string_sze2[j],"%s ",temp_string);
		sprintf_s(temp_string2,"P");
		sprintf_s(temp_string3,"Sternum");

				n=memcmp ( temp_string, temp_string2, strlen(temp_string2) );  
				n2=memcmp ( temp_string, temp_string3, strlen(temp_string3) );  

			if(((n==0) && (strlen(temp_string) > 1)) || ((n2==0) && (strlen(temp_string) > 1)))
		  {
			  //store the data
			  sscanf (temp_string_sze2[j],"%s%lf%lf%lf",temp_string,&x_sze, &y_sze, &z_sze);
			  thedata.szelandmark_points->InsertNextPoint(x_sze, y_sze, z_sze);
			  thedata.the_landmark[num_points].append(temp_string);
			   num_points++;
		  }
	  }
numbers_strsze.close();
thedata.polydata_szelandmarks->SetPoints(thedata.szelandmark_points);
thedata.numelem=num_points; 

return thedata;
}

xray_data
Transform::read_landmarks_xray (string input_filenamewrl, xray_data thedata)
{
	//topo_data thedata;
	thedata.polydata_wrllandmarks =  vtkPolyData::New ();
	thedata.wrllandmark_points=vtkPoints::New();
	double  x_wrl, y_wrl, z_wrl;
	  string::size_type len, pos; 	
char temp_string[200], temp_string2[200], temp_string3[200],temp_string4[200],temp_stringyaya[200],c;
int num_lines =0, n,n2,j, line_count=0, num_points=0;
string wrl_marker_file;

len = input_filenamewrl.length ();
pos = input_filenamewrl.rfind (".", len);
string ext = input_filenamewrl.substr (pos + 1, len - pos);

string base_name = input_filenamewrl.substr (0, pos);
string extension4 = ".o3";
string extension6 = "wrl";

 wrl_marker_file = base_name+extension4;

// find out number of lines
std::ifstream numbers_strwrl( wrl_marker_file.c_str() );
numbers_strwrl.seekg(0, ios::beg);
numbers_strwrl.seekg(0, ios::end);
pos = numbers_strwrl.tellg();
while(pos != 0)
   {
	   pos -= 1;
		numbers_strwrl.seekg(pos);
		c = numbers_strwrl.peek();
		if(c == '\n') 
		{
			pos-=1;line_count++;
		}
	 }

num_points=0;

numbers_strwrl.seekg(0, ios::beg);
char(*temp_string_wrl2)[200] = new char[line_count][200];

for(num_lines=0;num_lines<line_count;num_lines++)
	{
		  numbers_strwrl.getline(temp_string_wrl2[num_lines],200);
//		  num_lineswrl++;
	}
numbers_strwrl.close();

//start the reading, no need to bother wth files no more
thedata.the_landmark = new string[line_count];
		sprintf_s(temp_string2,"P");
		sprintf_s(temp_string3,"Sternum");
		sprintf_s(temp_string4,"Capteurs");
///first get to Objet:	Capteurs 
		int foundit=1;
		int start=0;
		strcpy_s(temp_string,"-2");
		strcpy_s(temp_stringyaya,"-2");
		  sscanf (temp_string_wrl2[start],"%s%s",temp_stringyaya, temp_string);
		  foundit=memcmp ( temp_string, temp_string4, strlen(temp_string4) );
	while ((start<line_count) && (foundit!=0))
	{
		  strcpy_s(temp_string,"-2");
		  strcpy_s(temp_stringyaya,"-2");
		  sscanf (temp_string_wrl2[start],"%s%s",temp_stringyaya, temp_string);
		  foundit=memcmp ( temp_string, temp_string4, strlen(temp_string4) );
		  start++;
	}
//parse string between spaces
	for(j=start;j<line_count;j++)
	  {
		  strcpy_s(temp_string,"-2");
		  sscanf (temp_string_wrl2[j],"%s ",temp_string);

				n=memcmp ( temp_string, temp_string2, strlen(temp_string2) );  
				n2=memcmp ( temp_string, temp_string3, strlen(temp_string3) );  

			if(((n==0) && (strlen(temp_string) > 1)) || ((n2==0) && (strlen(temp_string) > 1)))
		  {
			  //store the data
			  sscanf (temp_string_wrl2[j],"%s%lf%lf%lf",temp_string,&x_wrl, &y_wrl, &z_wrl);
			  thedata.wrllandmark_points->InsertNextPoint(x_wrl, y_wrl, z_wrl);
			  thedata.the_landmark[num_points].append(temp_string);
			   num_points++;
		  }
	  }
numbers_strwrl.close();
thedata.polydata_wrllandmarks->SetPoints(thedata.wrllandmark_points);
thedata.numelem=num_points; 

return thedata;
}

void Transform::saveMRImage (vtkImageData *theimage)
{
}

void Transform::savewrlsep (vtkActor **thevertebrae, string output_filename, int numverts)
{
  string::size_type len, pos;

  // Determine the format of the output file by the extension
  len = output_filename.length ();
  pos = output_filename.rfind (".", len);
  if (pos == string::npos)
	{
	  cout << "The output filename haven't correct extension format" << endl;
	  exit (0);
	}
  else
	{
	  string ext = output_filename.substr (pos + 1, len - pos);

	 if ((ext == "wrl") || (ext == "WRL"))
				output_type = o_wrl;
	  else
	{
	  cout << "The output filename haven't correct extension format" <<
		endl;
	  exit (0);
	}
	}


  // Write the data
  vtkPolyDataWriter *polyWriter;
  vtkSZEWriter *szeWriter;
  vtkRenderer *renderer = vtkRenderer::New();
  for(int i = 0; i<numverts; i++)
  {
	/*	vtkPolyDataMapper *polyDataMapper = vtkPolyDataMapper::New();
		polyDataMapper->SetInput(thewrlverts.polydata[i]);
		vtkActor *actor = vtkActor::New();
		actor->SetMapper( polyDataMapper );
		polyDataMapper->Delete();*/


		renderer->AddActor( thevertebrae[i] );
		//thevertebrae[i]

  }
		//actor->Delete(); //deleted for visualization purposes
		vtkRenderWindow *renderWindow = vtkRenderWindow::New();
		renderWindow->AddRenderer( renderer );
		//renderer->Delete();
  
		vtkVRMLExporter *vrmlexport = vtkVRMLExporter::New ();
		vrmlexport->SetFileName (output_filename.c_str ());
		vrmlexport->SetInput( renderWindow );
		//renderWindow->Delete();

		vrmlexport->Write();    

		//added recently for visualisation
		// an interactor
  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renderWindow);

 // render an image (lights and cameras are created automatically)
  renderWindow->Render();

  // begin mouse interaction
  iren->Start();

}


void Transform::save (vtkPolyData * polydata, string output_filename)
{
  if (polydata == NULL)
	{
	  cout << "Error: These is not data in the input file" << endl;
	  exit (-1);
	}

	cout << "In save" << endl;
  string::size_type len, pos;

  // Determine the format of the output file by the extension
  len = output_filename.length ();
  pos = output_filename.rfind (".", len);
  if (pos == string::npos)
	{
	  cout << "The output filename haven't correct extension format" << endl;
	  exit (0);
	}
  else
	{
	  string ext = output_filename.substr (pos + 1, len - pos);

	  if ((ext == "vtk") || (ext == "VTK"))
				output_type = o_vtk;
	  else if ((ext == "landmark") || (ext == "LANDMARK"))
				output_type = o_vtk;
	  else if ((ext == "wrl") || (ext == "WRL"))
				output_type = o_wrl;
	  else if ((ext == "sze") || (ext == "SZE"))
				output_type = o_sze;
	  else
	{
	  cout << "The output filename haven't correct extension format" <<
		endl;
	  exit (0);
	}
	}


  // Write the data
  vtkPolyDataWriter *polyWriter;
  vtkSZEWriter *szeWriter;

  switch (output_type)
	{
	case o_vtk:
	  polyWriter = vtkPolyDataWriter::New ();
	  polyWriter->SetFileName (output_filename.c_str ());
	  polyWriter->SetInput (polydata);
	  polyWriter->Write ();
	  polyWriter->Delete ();
	  break;
		
	case o_sze:
		cout << "In sze case statement" << endl;
	  szeWriter = vtkSZEWriter::New ();
	  szeWriter->SetFileName (output_filename.c_str ());
	  szeWriter->SetInput (polydata);
	  cout << "In sze case statement inputted polydata" << endl;
	  szeWriter->setInputData(dim[0],dim[1],textureTab,NbPolygon,connect3D,NbTexture,connectTex,textureCoord,NbPoints);
	  cout << "In sze case statement about to write" << endl;
	  szeWriter->Write ();
	  cout << "In sze case statement written" << endl;
//	  szeWriter->Delete ();
	  break;    
	case o_wrl:
		cout << "In wrl case statement changed to save separate" <<
		endl;
		vtkPolyDataMapper *polyDataMapper = vtkPolyDataMapper::New();
		polyDataMapper->SetInput(polydata);
		vtkActor *actor = vtkActor::New();
		actor->SetMapper( polyDataMapper );
		polyDataMapper->Delete();

		vtkRenderer *renderer = vtkRenderer::New();
		renderer->AddActor( actor );
		//actor->Delete(); //deleted for visualization purposes
		vtkRenderWindow *renderWindow = vtkRenderWindow::New();
		renderWindow->AddRenderer( renderer );
		//renderer->Delete();

		cout << "Exporting vrml files" <<
		endl;
		vtkVRMLExporter *vrmlexport = vtkVRMLExporter::New ();
		vrmlexport->SetFileName (output_filename.c_str ());
		vrmlexport->SetInput( renderWindow );
		//renderWindow->Delete();
		cout << "Writing vrml files" <<
		endl;
		vrmlexport->Write();    

		//added recently for visualisation
		// an interactor
  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renderWindow);

 // render an image (lights and cameras are created automatically)
  renderWindow->Render();

  // begin mouse interaction
  iren->Start();

	break;
	}
cout << "out of case statement" <<endl;
}



int
Transform::convert (string input_filename, string output_filename)
{
	cout << "in convert mode" << endl;
  save (read (input_filename), output_filename);
  return 1;
}

float
Transform::compare (vtkPolyData * polydata1, vtkPolyData * polydata2,
					bool bidirection, bool clip, float *range)
{
  int subId, num1=0, num2=0, total=0;
  vtkIdType cellId, i;
  double dist;
  double *p1 = new double[3];
  double *ptline = new double[3];
  float sum_distance1 = 0.0f;
  float sum_distance2 = 0.0f;
	
  num1 = polydata1->GetNumberOfPoints ();
  vtkCellLocator *cellLocator = vtkCellLocator::New ();
  cellLocator->SetDataSet (polydata2);
  cellLocator->BuildLocator ();
  vtkGenericCell *cell = vtkGenericCell::New ();
  
  for (i = 0; i < num1; i++)
	{
	  polydata1->GetPoints ()->GetPoint (i, p1);
	  if ( clip && (p1[1] > range[0] || p1[1] < range[1]))
	continue;
	  cellLocator->FindClosestPoint (p1, ptline, cell, cellId, subId, dist);
	  sum_distance1 += sqrt (dist);
	  total++;
	}
  
  if(bidirection){
	num2 = polydata2->GetNumberOfPoints ();
	cellLocator->SetDataSet (polydata1);		
	cellLocator->BuildLocator ();
	
	for (i = 0; i < num2; i++)
		{
		polydata2->GetPoints ()->GetPoint (i, p1);
		if ( clip && (p1[1] > range[0] || p1[1] < range[1]))
	  continue;     	
	cellLocator->FindClosestPoint (p1, ptline, cell, cellId, subId, dist);
		sum_distance2 += sqrt (dist);
	total++;
	}
  }
	
	  
  cell->Delete ();
  cellLocator->Delete ();

  cout << "    # Average (100%, Bidirectional :" << bidirection 
		 << ", Clip:" << clip <<"): " 
		 << (sum_distance1+sum_distance2) / total << endl; 
  return (sum_distance1+sum_distance2) / total;
}

float
Transform::compare (string input_filename1, string input_filename2,
			string output_filename)
{
  vtkPolyData *PolyData1 = read (input_filename1);
  vtkPolyData *PolyData2 = read (input_filename2);
  vtkFloatArray *D_Scalar = vtkFloatArray::New ();
  int subId, num;
  vtkIdType cellId, i;
  double dist;
  double *p1 = new double[3];
  double *ptline = new double[3];
  double sum_distance = 0.0f;

  num = PolyData1->GetNumberOfPoints ();
  D_Scalar->SetNumberOfValues (num);

  vtkCellLocator *cellLocator = vtkCellLocator::New ();
  cellLocator->SetDataSet (PolyData2);
  cellLocator->BuildLocator ();
  vtkGenericCell *cell = vtkGenericCell::New ();
  for (i = 0; i < num; i++)
	{
	  PolyData1->GetPoints ()->GetPoint (i, p1);
	  cellLocator->FindClosestPoint (p1, ptline, cell, cellId, subId, dist);
	  D_Scalar->SetValue (i, sqrt (dist));
	  sum_distance += sqrt (dist);
	}
  PolyData1->GetPointData ()->SetScalars (D_Scalar);
  //save (PolyData1, output_filename);
  PolyData1->Delete ();
  PolyData2->Delete ();
  D_Scalar->Delete ();
  cell->Delete ();
  cellLocator->Delete ();
  cout << "# The average distance : " << sum_distance / num << endl;
  return sum_distance / num;
}

