/***************************************************************************
                          vtkO3Reader.h  -  description
                             -------------------
    begin                : Sun Feb 1 2004
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
#ifndef __vtkO3Reader_h
#define __vtkO3Reader_h

#include "vtkPolyDataSource.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"

class VTK_IO_EXPORT vtkO3Reader:public vtkPolyDataSource
{
public:
  static vtkO3Reader *New ();
    vtkTypeRevisionMacro (vtkO3Reader, vtkPolyDataSource);
  void PrintSelf (ostream & os, vtkIndent indent);

  // Description:
  // Specify file name of Wavefront .obj file.
    vtkSetStringMacro (FileName);
    vtkGetStringMacro (FileName);

  void SetNumberOfSides (int side_num);
  void SetRadiusFactor (double factor);
  void SetNumberOfSteps (int step);

  // Type = 0: produce the ligament region tube;
  // Type = 1: produce the vertebra models.
  // Type = 2: get each part of points;
  // Type = 3: deform a model to get vertebra model.
  void SetType(int type, int part=0, int position=0);
  void SetModel(vtkPolyData* model, vtkPoints* model_mark,int begin, int end);
    
protected:
    vtkO3Reader ();
   ~vtkO3Reader ();

  void Execute ();
  void ligament_Combine ();
  void vertebra_rib_ligament();
  void vertebra_Combine ();
  void get_points(int part, int position);
  void deform();

  char *FileName;

  // The Anatomical landmarks position for each vertebrae and rib.
  vtkPoints *T[17]; // vertebra;
  vtkPolyData *center;// store all the centroid point of the vertebrae.
  vtkPoints *RR[12]; // right side ribs
  vtkPoints *RL[12]; // left side ribs
  vtkPoints *sternum; 
  vtkPoints *landmark;
  vtkPoints *bassin;

  int type;
  int part;
  int position;
  //** defrom */
  vtkPolyData* model;
  vtkPoints* model_mark;
  int begin, end;
  
  int side_num;
  int step;
  double factor;

private:
    vtkO3Reader (const vtkO3Reader &);	// Not implemented.
  void operator= (const vtkO3Reader &);	// Not implemented.
};

#endif
