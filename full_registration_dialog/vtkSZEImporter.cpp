#include "stdafx.h"
#include "vtkSZEImporter.h"
#include <vtkUnsignedCharArray.h>
#include <vtkCharArray.h>
#include <vtkActor.h>
#include <vtkByteSwap.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkLight.h>
#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkStripper.h>
#include <vtkImageImport.h>
#include <vtkLongArray.h>


vtkCxxRevisionMacro (vtkSZEImporter, "$Revision: 0.01 $");
vtkStandardNewMacro (vtkSZEImporter);


vtkSZEImporter::vtkSZEImporter ()
{
  this->FileName = NULL;
  this->FileFD = NULL;
  this->image = NULL;
  this->points = NULL;
  this->modelcell = NULL;
  this->textcell = NULL;
  this->texturecoord = NULL;

}

vtkSZEImporter::~vtkSZEImporter ()
{

  if (this->FileName)
    {
      delete[]this->FileName;
    }
  if (this->image)
    this->image->Delete ();
  if (this->points)
    this->points->Delete ();
  if (this->modelcell)
    this->modelcell->Delete ();
  if (this->textcell)
    this->textcell->Delete ();
  if (this->texturecoord)
    this->texturecoord->Delete ();

}

int
vtkSZEImporter::ImportBegin ()
{
  vtkDebugMacro (<<"Opening import file as ASCII");
  this->FileFD = fopen (this->FileName, "rb");
  if (this->FileFD == NULL)
    {
      vtkErrorMacro (<<"Unable to open file: " << this->FileName);
      return 0;
    }
  return this->ReadSZE ();
}

void
vtkSZEImporter::ImportEnd ()
{
  vtkDebugMacro (<<"Closing import file");
  if (this->FileFD != NULL)
    {
      fclose (this->FileFD);
    }
  this->FileFD = NULL;
}

int
vtkSZEImporter::ReadSZE ()
{

  char *space=new char[100];
  const int MAX_LINE = 1024;
  char line[MAX_LINE];  
  int i, j;
  long tmp;


  //import the image size
  //printf("==========read the image:============\n");
  for (i = 0; i < 10; i++)
    {
      fscanf (FileFD, " %s", space);
      //printf("%s\n",space);
    }

  int imagesize1 = 0;
  int imagesize2 = 0;
  fscanf (FileFD, "%s", space);
  imagesize1 = atoi (space);
  fscanf (FileFD, "%s", space);
  imagesize2 = atoi (space);
  int numofpixel = imagesize1 * imagesize2;
  dim1=imagesize1;
  dim2=imagesize2;
  vtkUnsignedCharArray *scalar = vtkUnsignedCharArray::New ();
  scalar->SetNumberOfTuples (numofpixel);
  scalar->SetNumberOfComponents (3);
  float color[3];

  char *buf=new char[100];
  textureTab=new long*[imagesize1];

  for (i = imagesize1 - 1; i >= 0; i--)
    {
		textureTab[i]=new long[imagesize2];
      for (j = 0; j < imagesize2; j++)
		{
			fscanf (FileFD, "%s", space);
			tmp = strtol (space, (char **) NULL, 16);
			textureTab[i][j]=tmp;
			color[0] = (tmp / 256 / 256 / 256) % 256;
			color[1] = (tmp / 256 / 256) % 256;
			color[2] = (tmp / 256) % 256;
			scalar->InsertTuple (i * imagesize2 + j, color);
		}
    }
	
  if (this->image)
    this->image->Delete ();
  this->image = vtkImageData::New ();
  this->image->SetScalarType (VTK_UNSIGNED_CHAR);
  this->image->SetNumberOfScalarComponents (3);
  image->SetSpacing (1, 1, 1);
  image->SetDimensions (imagesize1, imagesize2, 1);
  image->SetOrigin (0, 0, 0);
  this->image->GetPointData ()->SetScalars (scalar);
  scalar->Delete ();

  //==========HANDLE THE MODEL MESH====================
  while (fgets (line, MAX_LINE, FileFD) != NULL)
    {
		if (strncmp (line, "NbPolygon", 9)==0)
		{
			NbPolygon = atoi (&line[10]);
			fgets(line, MAX_LINE,FileFD);
			cout<<line<<endl;
			break;
		}
    }
    
  //  for (i = 0; i < 17; i++)
  //  {
  //      fscanf (FileFD, "%s", space);
  //      if (i == 15)
  //    	{
  //	      NbPolygon = atoi (&space[10]);
  //
  //    	}
  //  }

  if (this->modelcell)
    this->modelcell->Delete ();
  this->modelcell = vtkCellArray::New ();
  connect3D=new int*[NbPolygon];
  for (i = 0; i < NbPolygon; i++)
    {
		
      fscanf (FileFD, "%s", space);
      tmp = atoi (space);
	  connect3D[i]=new int[tmp+1];
	  connect3D[i][0]=tmp;
      if (tmp > 0)
	  {
			this->modelcell->InsertNextCell (tmp);
			for (j = 0; j < tmp; j++)
			{
				fscanf (FileFD, "%s", space);
				this->modelcell->InsertCellPoint (atoi (space));
				connect3D[i][j+1]=atoi (space);
			}
	   }
    }

  //=========HANDLE THE TEXTURE MESH================== 
  for (i = 0; i < 4; i++)
    {
      fscanf (FileFD, "%s", space);
      //printf("%s\n",space);
    }

  if (this->textcell)
    this->textcell->Delete ();
  this->textcell = vtkCellArray::New ();
  connectTex=new int*[NbPolygon];
  for (i = 0; i < NbPolygon; i++)
    {
      fscanf (FileFD, "%s", space);
      tmp = atoi (space);
	  connectTex[i]=new int[tmp+1];
	  connectTex[i][0]=tmp;
      if (tmp > 0)
	{
	  this->textcell->InsertNextCell (tmp);
	  for (j = 0; j < tmp; j++)
	    {
	      fscanf (FileFD, "%s", space);
	      this->textcell->InsertCellPoint (atoi (space));
		  connectTex[i][j+1]=atoi (space);
	    }
	}
    }

  //=========Handle the texture coordinate============

  //int NbTexture = 0;
  for (i = 0; i < 5; i++)
    {
      fscanf (FileFD, "%s", space);
      //printf("%s\n",space);
      if (i == 4)
	{
	  NbTexture = atoi (&space[10]);
	  cout<<NbTexture<<endl;
	  //    printf("the NbTexture :%d\n",NbTexture);
	}
    }


  if (this->texturecoord)
    this->texturecoord->Delete ();
  this->texturecoord = vtkFloatArray::New ();
  this->texturecoord->SetNumberOfComponents (2);
  this->texturecoord->SetNumberOfTuples (NbTexture);
  float uv[2];
  textureCoord=new float*[NbTexture];
  for (i = 0; i < NbTexture; i++)
    {
	  textureCoord[i]=new float[2];
      fscanf (FileFD, "%s", space);
      uv[0] = atof (space);
      textureCoord[i][0]=atof (space);
      fscanf (FileFD, "%s", space);
      uv[1] = atof (space);
	  textureCoord[i][1]=atof (space);
      this->texturecoord->SetTuple (i, uv);
    }


  //=============Handle the points coordinate==============
  if (this->points)
    this->points->Delete ();
  this->points = vtkPoints::New ();


  for (i = 0; i < 10; i++)
    {
      fscanf (FileFD, "%s", space);
      //printf("%s\n",space);
      if (i == 7)
	{
	  NbPoints = atoi (&space[4]);
	  //    printf("the NbPoints :%d\n",NbPoints);
	}
    }
  this->points->SetNumberOfPoints (NbPoints);
  float xyz[3];
  for (i = 0; i < NbPoints; i++)
    {
      fscanf (FileFD, "%s", space);
      xyz[0] = atof (space);
      fscanf (FileFD, "%s", space);
      xyz[1] = atof (space);
      fscanf (FileFD, "%s", space);
      xyz[2] = atof (space);
      this->points->InsertPoint (i, xyz);
    }

printf ("=== finish read==\n");
  return 1;
  
}

void
vtkSZEImporter::ImportActors (vtkRenderer * renderer)
{

  vtkPolyDataMapper *polyMapper = vtkPolyDataMapper::New ();
  vtkActor *actor = vtkActor::New ();
  vtkPolyData *polyData = vtkPolyData::New ();
  if (true)
    {
      vtkPoints *recon_points = vtkPoints::New ();
      double xyz[3];

      int numTex = this->texturecoord->GetNumberOfTuples ();

      recon_points->SetNumberOfPoints (numTex);

      int *relation = new int[numTex + 10];
      vtkIdType Mnum, *Mids;
      vtkIdType Tnum, *Tids;

      this->modelcell->InitTraversal ();
      this->textcell->InitTraversal ();

      int numCell = this->modelcell->GetNumberOfCells ();
      int numCell1 = this->textcell->GetNumberOfCells ();
      if (numCell != numCell1)
	{
	  printf (" dataset is wrong!\n ");
	  return;
	}

      int i, j;

      for (i = 0; i < numCell; i++)
	{
	  this->modelcell->GetNextCell (Mnum, Mids);
	  this->textcell->GetNextCell (Tnum, Tids);
	  if (Mnum != Tnum)
	    {
	      printf (" the Dataset is wrong");
	      return;
	    }
	  for (j = 0; j < Mnum; j++)
	  {
	    if (Tids[j] >= numTex)
		  {
		    printf ("something wrong!");
		    return;
		  }
	    relation[Tids[j]] = Mids[j];
	  }
	}


      for (i = 0; i < numTex; i++)
	{

	  this->points->GetPoint (relation[i], xyz);
	  recon_points->InsertPoint (i, xyz);
	}

      polyData->SetPoints (recon_points);
      polyData->SetPolys (this->textcell);
      polyData->GetPointData ()->SetTCoords (this->texturecoord);

      polyMapper->SetInput (polyData);
      vtkTexture *atext = vtkTexture::New ();
      atext->SetInput (this->image);
      actor->SetMapper (polyMapper);
      actor->SetTexture (atext);
      atext->Delete ();
      delete[]relation;
      recon_points->Delete ();
    }
  // without texture
  else
    {
      polyData->SetPoints (this->points);
      polyData->SetPolys (this->modelcell);
      polyMapper->SetInput (polyData);
      actor->SetMapper (polyMapper);
    }

  renderer->AddActor (actor);
  actor->Delete ();
  polyMapper->Delete ();
  polyData->Delete ();

}

void
vtkSZEImporter::PrepareData ()
{
  this->ImportBegin ();		//ImportBegin function include the ReadSZE().

  this->ImportEnd ();
}

vtkPolyData *
vtkSZEImporter::GetPolyData ()
{
  vtkPolyData *polyData = vtkPolyData::New ();
  polyData->SetPoints (this->points);
  polyData->SetPolys (this->modelcell);
  return polyData;
}

vtkPolyData *
vtkSZEImporter::GetPolyDataWithTextCoord ()
{
  vtkPolyData *polyData = vtkPolyData::New ();

  vtkPoints *recon_points = vtkPoints::New ();
  double xyz[3];

  int numTex = this->texturecoord->GetNumberOfTuples ();

  recon_points->SetNumberOfPoints (numTex);

  int *relation = new int[numTex + 10];
  vtkIdType Mnum, *Mids;
  vtkIdType Tnum, *Tids;

  this->modelcell->InitTraversal ();
  this->textcell->InitTraversal ();

  int numCell = this->modelcell->GetNumberOfCells ();
  int numCell1 = this->textcell->GetNumberOfCells ();
  //printf("numCell:%d\n",numCell);
  if (numCell != numCell1)
    {
      printf (" dataset is wrong!\n ");
      exit (0);
    }

  int i, j;

  for (i = 0; i < numCell; i++)
    {
      this->modelcell->GetNextCell (Mnum, Mids);
      this->textcell->GetNextCell (Tnum, Tids);
      if (Mnum != Tnum)
	{
	  printf (" the Dataset is wrong");
	  exit (0);
	}
      for (j = 0; j < Mnum; j++)
	{
	  if (Tids[j] >= numTex)
	    {
	      printf ("something wrong!");
	      exit (0);
	    }
	  relation[Tids[j]] = Mids[j];
	}
    }


  for (i = 0; i < numTex; i++)
    {
      this->points->GetPoint (relation[i], &xyz[0]);
      recon_points->InsertPoint (i, xyz);
    }

  polyData->SetPoints (recon_points);
  polyData->SetPolys (this->textcell);
  polyData->GetPointData ()->SetTCoords (this->texturecoord);

  delete[]relation;
  recon_points->Delete ();

  return polyData;
}

vtkImageData *
vtkSZEImporter::GetImageData ()
{

  return this->image;
}

void
vtkSZEImporter::ImportCameras (vtkRenderer * renderer)
{

}

void
vtkSZEImporter::ImportLights (vtkRenderer * renderer)
{

}

void
vtkSZEImporter::ImportProperties (vtkRenderer * vtkNotUsed (renderer))
{

}

void
vtkSZEImporter::PrintSelf (ostream & os, vtkIndent indent)
{
  this->Superclass::PrintSelf (os, indent);
  os << indent << "File Name: "
     << (this->FileName ? this->FileName : "(none)") << "\n";

}
 long **vtkSZEImporter::getTexture()
 {
	return textureTab;

 }
int vtkSZEImporter::getNbPolygon()
{
	return NbPolygon;
}
int **vtkSZEImporter::get3DConnect()
{
	return connect3D;
}
int vtkSZEImporter::getNbTexture()
{
	return NbTexture;
}

int **vtkSZEImporter::getConnectTex()
{
	return connectTex;
}
float **vtkSZEImporter::getTextureCoord()
{
	return textureCoord;
}
int vtkSZEImporter::getNbPoints()
{
	return NbPoints;
}
