/***************************************************************************
                          vtkSZEWriter.cpp  -  description
                             -------------------
    begin                : Thu Jan 29 2004
    copyright            : (C) 2004 by Qinghu LIao
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
#include "stdafx.h"
#include "vtkSZEWriter.h"

#include <vtkCellArray.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkTriangle.h>

vtkCxxRevisionMacro (vtkSZEWriter, "$Revision: 0.01 $");
vtkStandardNewMacro (vtkSZEWriter);

vtkSZEWriter::vtkSZEWriter ()
{
  this->FileType = VTK_ASCII;
}

void
vtkSZEWriter::WriteData ()
{
cout<<"pendant write"<<endl;
  vtkPoints *pts;
  vtkCellArray *polys;
  vtkPolyData *input = this->GetInput ();
  cout<<"pendant write 2"<<endl;
  polys = input->GetPolys ();
  pts = input->GetPoints ();
  cout<<"pendant write 3"<<endl;
  if (pts == NULL || polys == NULL)
    {
      vtkErrorMacro (<<"No data to write!");
      return;
    }

  if (this->FileName == NULL)
    {
      vtkErrorMacro (<<"Please specify FileName to write");
      return;
    }
cout<<"pendant write 4"<<endl;
  this->WriteSZE (pts, polys);

}


void
vtkSZEWriter::WriteSZE (vtkPoints * pts, vtkCellArray * polys)
{
  FILE *fp;
  double *v1;
  vtkIdType npts = 0;
  vtkIdType *indx = 0;

  if ((fp = fopen (this->FileName, "w")) == NULL)
    {
      vtkErrorMacro (<<"Couldn't open file: " << this->FileName);
      return;
    }
    char *model_name=new char[5];
	strcpy(model_name,"test");
	//char *model_name=new char[strlen(this->FileName)-8];
	//strncpy(model_name,this->FileName,strlen(this->FileName)-4);
	
  //  Write header
	cout<<"in WriteSZE"<<endl;
  vtkDebugMacro ("Writing ASCII .sze file");
  fprintf (fp, "BEGIN SZE ASCII\n");
  fprintf (fp, "NbNet=1\n");
  fprintf (fp, "BEGIN IMAGES\n");
  fprintf (fp, "NbImage=1\n");
  fprintf (fp, "BEGIN IMAGE %s\n",model_name);
  fprintf (fp, "%i\t%i\n",dim1,dim2);
	char *buf=new char[100];
   for (int i = dim1 - 1; i >= 0; i--)
    {
      for (int j = 0; j < dim2; j++)
		{
			if(textureTab[i][j]==0)
				fprintf (fp,"0X");
				
			fprintf (fp,"%#0-8.8X ",textureTab[i][j]);
			
		}
		fprintf (fp,"\n");
		
	}
   cout<<"in WriteSZE 2 "<<endl;
  fprintf (fp, "\nEND IMAGE %s\n",model_name);
  fprintf (fp, "END IMAGES\n");
  fprintf (fp, "BEGIN NET %s\n",model_name);
  fprintf (fp, "BEGIN FLAG\n");
  fprintf (fp, "flag=3\n");
  fprintf (fp, "END FLAG\n");
  fprintf (fp, "BEGIN MESH\n");
  fprintf (fp, "NbPolygon=%i\n",NbPolygon);
  fprintf (fp, "SizePolygon=%i\n",4*NbPolygon);

  cout <<"NbPolygon="<<NbPolygon<<endl;

  for (int i=0;i<NbPolygon;i++)
  {
	 fprintf (fp, "%i\n",connect3D[i][0]);
	 for(int j=1;j<connect3D[i][0]+1;j++)
		fprintf (fp, "%i ",connect3D[i][j]);
	 fprintf (fp, "\n");
  }

  cout<<"in WriteSZE 2.5"<<endl;
  fprintf (fp, "END MESH\n");
  fprintf (fp, "BEGIN MESH_TEX\n");
    //NbPolygon=0;//////////////3remove
   for (int i=0;i<NbPolygon;i++)
  {
	 fprintf (fp, "%i\n",connectTex[i][0]);
	 for(int j=1;j<connectTex[i][0]+1;j++)
		fprintf (fp, "%i ",connectTex[i][j]);
	 fprintf (fp, "\n");
  }
   cout<<"in WriteSZE 2.75"<<endl;
  fprintf (fp, "END MESH_TEX\n");
  fprintf (fp, "BEGIN TEXTURE\n");
  fprintf (fp, "NbTexture=%i\n",NbTexture);
  for (int i=0;i<NbTexture;i++)
	fprintf (fp, "%6f %6f\n",textureCoord[i][0],textureCoord[i][1]);
  fprintf (fp, "END TEXTURE\n");
   fprintf (fp,
	   "2 XPos YPos Zpos Row=1 Col=%i Zmin=339999995214436420000000000000000000000.000000 File=0\n",NbPoints);
cout<<"in WriteSZE 3"<<endl;
  int num = pts->GetNumberOfPoints ();
  for (int i = 0; i < num; i++)
    {
      v1 = pts->GetPoint (i);
      fprintf (fp, "%.6g %.6g %.6g\n", v1[0], v1[1], v1[2]);
    }
  fprintf (fp, "END NET %s\n",model_name);
  fprintf (fp, "END SZE ASCII\n");


 /* fprintf (fp, "BEGIN NET spine\n");
  fprintf (fp, "BEGIN FLAG\n");
  fprintf (fp, "flag=3\n");
  fprintf (fp, "END FLAG\n");
  fprintf (fp, "BEGIN MESH\n");
  fprintf (fp, "NbPolygon=%d\n", polys->GetNumberOfCells ());
  fprintf (fp, "SizePolygon=%d\n", polys->GetNumberOfCells () * 4);
  //  Write out triangle polygons.  In not a triangle polygon, only first
  //  three vertices are written.
  //fprintf (fp, "\n# Total Number of faces: %d\n", polys->GetNumberOfCells ());
  for (polys->InitTraversal (); polys->GetNextCell (npts, indx);)
    {
      fprintf (fp, "3\n");
      fprintf (fp, "%d %d %d \n", indx[0], indx[1], indx[2]);
    }
  fprintf (fp, "END MESH\n");
  fprintf (fp,
	   "2 XPos YPos Zpos Row=1 Col=38008 Zmin=339999995214436420000000000000000000000.000000 File=0\n");

  int num = pts->GetNumberOfPoints ();
  for (int i = 0; i < num; i++)
    {
      v1 = pts->GetPoint (i);
      fprintf (fp, "%.6g %.6g %.6g\n", v1[0], v1[1], v1[2]);
    }
  fprintf (fp, "END NET spine\n");
  fprintf (fp, "END SZE ASCII\n");*/

  fclose (fp);
}


//----------------------------------------------------------------------------
void
vtkSZEWriter::PrintSelf (ostream & os, vtkIndent indent)
{
  this->Superclass::PrintSelf (os, indent);
}

 void vtkSZEWriter::setInputData(int c_dim1,int c_dim2,long **c_textureTab,int c_NbPolygon, int **c_connect3D,int c_NbTexture,int  **c_connectTex,float **c_textureCoord,int c_NbPoints)
 {
	dim1=c_dim1;	
	dim2=c_dim2;
	textureTab=c_textureTab;
	NbPolygon=c_NbPolygon;
	connect3D=c_connect3D;
	NbTexture=c_NbTexture;
	connectTex=c_connectTex;
	textureCoord=c_textureCoord;
	NbPoints=c_NbPoints;
 }