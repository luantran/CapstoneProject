#include <vtkImageData.h>
#include <vtkTexture.h>
#include <vtkPolyDataMapper.h>
#include <vtkBMPReader.h>
#include <vtkFloatArray.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkImageImport.h>


// .NAME vtkSZEImporter - imports Inspeck Inc. *.sze ascii files.
// .SECTION Description
// vtkSZEImporter imports Inspeck Inc. *.sze files into vtk.

// .SECTION See Also
// vtkImporter


#ifndef __vtkSZEImporter_h
#define __vtkSZEImporter_h

#include <vtkImporter.h>

class vtkPolyData;
class vtkImageData;
class vtkTexture;

class VTK_HYBRID_EXPORT vtkSZEImporter:public vtkImporter
{
public:
  static vtkSZEImporter *New ();

  vtkTypeRevisionMacro (vtkSZEImporter, vtkImporter);
  void PrintSelf (ostream & os, vtkIndent indent);

  // Description:
  // Specify the name of the file to read.
    vtkSetStringMacro (FileName);
    vtkGetStringMacro (FileName);

  // Description:
  // Specify    
  void ShowTexture (bool show)
  {
    TextureOn = show;
  };

  // Description:
  // Return the file pointer to the open file.
  FILE *GetFileFD ()
  {
    return this->FileFD;
  };

  vtkPolyData *GetPolyData ();
  vtkPolyData *GetPolyDataWithTextCoord ();
  vtkImageData *GetImageData ();
  void getImageDim(int &ImDim1,int &ImDim2);
  long **getTexture();
  int **get3DConnect();
  int getNbPolygon();
  int getNbTexture();
  int **getConnectTex();
  float **getTextureCoord();
  int getNbPoints();
  void PrepareData ();

protected:
  vtkSZEImporter ();
  ~vtkSZEImporter ();

  virtual int ImportBegin ();
  virtual void ImportEnd ();
  virtual void ImportActors (vtkRenderer * renderer);
  virtual void ImportCameras (vtkRenderer * renderer);
  virtual void ImportLights (vtkRenderer * renderer);
  virtual void ImportProperties (vtkRenderer * renderer);

  int ReadSZE ();

  bool TextureOn;
  char *FileName;
  FILE *FileFD;
  long **textureTab;
  int dim1,dim2;
  int NbPolygon;
  int **connect3D;
  int NbTexture;
  int **connectTex;
  float **textureCoord;
  int NbPoints;
  vtkImageData *image;
  vtkPoints *points;
  vtkCellArray *modelcell;
  vtkCellArray *textcell;
  vtkFloatArray *texturecoord;

private:
  vtkSZEImporter (const vtkSZEImporter &);	// Not implemented.
  void operator= (const vtkSZEImporter &);	// Not implemented.
  
};

#endif
