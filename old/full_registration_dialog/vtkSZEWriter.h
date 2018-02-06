/***************************************************************************
                          vtkSZEWriter.h  -  description
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
#ifndef __vtkSZEWriter_h
#define __vtkSZEWriter_h

#include <vtkPolyDataWriter.h>

class VTK_IO_EXPORT vtkSZEWriter:public vtkPolyDataWriter
{
public:
  static vtkSZEWriter *New ();
    vtkTypeRevisionMacro (vtkSZEWriter, vtkPolyDataWriter);
  virtual void PrintSelf (ostream & os, vtkIndent indent);
  void setInputData(int c_dim1,int c_dim2,long **c_textureTab, int c_NbPolygon, int **c_connect3D,int c_NbTexture,int  **c_connectTex,float **c_textureCoord,int c_NbPoints);

protected:
    vtkSZEWriter ();
   ~vtkSZEWriter ()
  {
  };

  void WriteData ();
  void WriteSZE (vtkPoints * pts, vtkCellArray * polys);
 
private:
  vtkSZEWriter (const vtkSZEWriter &);	// Not implemented.
  void operator= (const vtkSZEWriter &);	// Not implemented.
  int dim1;
  int dim2;
  long **textureTab;
  int NbPolygon;
  int **connect3D;
  int NbTexture;
  int **connectTex;
  float **textureCoord;
  int NbPoints;
	
};

#endif
