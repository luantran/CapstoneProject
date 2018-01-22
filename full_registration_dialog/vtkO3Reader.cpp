/***************************************************************************
                          vtlO3Reader.cpp  -  description
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
#include "stdafx.h"

#include "vtkO3Reader.h"
#include "vtkAppendPolyData.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkDoubleArray.h"
#include "vtkMath.h"
#include "vtkCardinalSpline.h"
#include "vtkTubeFilter.h"
#include "vtkCellArray.h"
#include "vtkThinPlateSplineTransform.h"
#include "vtkGeneralTransform.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkMath.h"


vtkCxxRevisionMacro (vtkO3Reader, "$Revision: 0.01 $");
vtkStandardNewMacro (vtkO3Reader);

// Description:
// Instantiate object with NULL filename.
vtkO3Reader::vtkO3Reader ()
{
  this->FileName = NULL;
  this->step = 10;
  this->factor = 1.0;
  this->side_num = 10;
  this->type = 0;
  // Initialize the vtkPoints Object for the observed points.
  int i;
  for( i = 0; i< 17; i++)
    T[i] = vtkPoints::New();
  for( i = 0; i< 12; i++)
    RR[i] = vtkPoints::New();
  for( i = 0; i< 12; i++)
    RL[i] = vtkPoints::New();

  sternum = vtkPoints::New();
  landmark = vtkPoints::New ();
  bassin = vtkPoints::New();
  center = vtkPolyData::New ();
}

vtkO3Reader::~vtkO3Reader ()
{
  if (this->FileName)
    {
      delete[]this->FileName;
      this->FileName = NULL;
    }
  int i;
  for( i = 0; i< 17; i++)
    T[i]->Delete();
  for( i = 0; i< 12; i++)
    RR[i]->Delete();
  for( i = 0; i< 12; i++)
    RL[i]->Delete();

  sternum->Delete();  
  landmark->Delete ();
  bassin->Delete();
  center->Delete ();
}

void
vtkO3Reader::Execute ()
{
  if (!this->FileName)
    {
      vtkErrorMacro (<<"A FileName must be specified.");
      return;
    }

  FILE *in = fopen (this->FileName, "r");

  if (in == NULL)
    {
      vtkErrorMacro (<<"File " << this->FileName << " not found");
      return;
    }

  vtkDebugMacro (<<"Reading file");

  int everything_ok = 1;	// (true)   (use of this flag avoids early return and associated memory leak)
  // -- work through the file line by line, assigning into the above six structures as appropriate --

  const int MAX_LINE = 1024;
  char line[MAX_LINE];
  char Objet[MAX_LINE], t1[MAX_LINE], t[MAX_LINE];
  float xyz[3];
  vtkPoints *curr_set = NULL;

  //printf("begin to read\n");
  while (everything_ok && fgets (line, MAX_LINE, in) != NULL)
    {
      if (strncmp (line, "#", 1) == 0)
	  {
	  }
      if (strncmp (line, "Dossier:", 8) == 0)
	{
	}
      else if (strncmp (line, "Date:", 5) == 0)
	{
	}
      else if (strncmp (line, "Source:", 7) == 0)
	{
	}
      else if (strncmp (line, "History:", 8) == 0)
	{
	}
      else if (strncmp (line, "Grid_3D", 7) == 0)
	{
	}
      else if (strncmp (line, "Line_3D", 8) == 0)
	{
	}
      else if (strncmp (line, "Objet:", 6) == 0)
	{
	  sscanf (line, "Objet: %s", Objet);
	  //printf("%s\n",Objet);
	  if (strcmp (Objet, "Vertebre_T1") == 0)
	    curr_set = T[0];
	  else if (strcmp (Objet, "Vertebre_T2") == 0)
	    curr_set = T[1];
	  else if (strcmp (Objet, "Vertebre_T3") == 0)
	    curr_set = T[2];
	  else if (strcmp (Objet, "Vertebre_T4") == 0)
	    curr_set = T[3];
	  else if (strcmp (Objet, "Vertebre_T5") == 0)
	    curr_set = T[4];
	  else if (strcmp (Objet, "Vertebre_T6") == 0)
	    curr_set = T[5];
	  else if (strcmp (Objet, "Vertebre_T7") == 0)
	    curr_set = T[6];
	  else if (strcmp (Objet, "Vertebre_T8") == 0)
	    curr_set = T[7];
	  else if (strcmp (Objet, "Vertebre_T9") == 0)
	    curr_set = T[8];
	  else if (strcmp (Objet, "Vertebre_T10") == 0)
	    curr_set = T[9];
	  else if (strcmp (Objet, "Vertebre_T11") == 0)
	    curr_set = T[10];
	  else if (strcmp (Objet, "Vertebre_T12") == 0)
	    curr_set = T[11];

	  else if (strcmp (Objet, "Vertebre_L1") == 0)
	    curr_set = T[12];
	  else if (strcmp (Objet, "Vertebre_L2") == 0)
	    curr_set = T[13];
	  else if (strcmp (Objet, "Vertebre_L3") == 0)
	    curr_set = T[14];
	  else if (strcmp (Objet, "Vertebre_L4") == 0)
	    curr_set = T[15];
	  else if (strcmp (Objet, "Vertebre_L5") == 0)
	    curr_set = T[16];

	  else if (strcmp (Objet, "Cote_1_Gauche") == 0)
	    curr_set = RL[0];
	  else if (strcmp (Objet, "Cote_2_Gauche") == 0)
	    curr_set = RL[1];
	  else if (strcmp (Objet, "Cote_3_Gauche") == 0)
	    curr_set = RL[2];
	  else if (strcmp (Objet, "Cote_4_Gauche") == 0)
	    curr_set = RL[3];
	  else if (strcmp (Objet, "Cote_5_Gauche") == 0)
	    curr_set = RL[4];
	  else if (strcmp (Objet, "Cote_6_Gauche") == 0)
	    curr_set = RL[5];
	  else if (strcmp (Objet, "Cote_7_Gauche") == 0)
	    curr_set = RL[6];
	  else if (strcmp (Objet, "Cote_8_Gauche") == 0)
	    curr_set = RL[7];
	  else if (strcmp (Objet, "Cote_9_Gauche") == 0)
	    curr_set = RL[8];
	  else if (strcmp (Objet, "Cote_10_Gauche") == 0)
	    curr_set = RL[9];
	  else if (strcmp (Objet, "Cote_11_Gauche") == 0)
	    curr_set = RL[10];
	  else if (strcmp (Objet, "Cote_12_Gauche") == 0)
	    curr_set = RL[11];

	  else if (strcmp (Objet, "Cote_1_Droite") == 0)
	    curr_set = RR[0];
	  else if (strcmp (Objet, "Cote_2_Droite") == 0)
	    curr_set = RR[1];
	  else if (strcmp (Objet, "Cote_3_Droite") == 0)
	    curr_set = RR[2];
	  else if (strcmp (Objet, "Cote_4_Droite") == 0)
	    curr_set = RR[3];
	  else if (strcmp (Objet, "Cote_5_Droite") == 0)
	    curr_set = RR[4];
	  else if (strcmp (Objet, "Cote_6_Droite") == 0)
	    curr_set = RR[5];
	  else if (strcmp (Objet, "Cote_7_Droite") == 0)
	    curr_set = RR[6];
	  else if (strcmp (Objet, "Cote_8_Droite") == 0)
	    curr_set = RR[7];
	  else if (strcmp (Objet, "Cote_9_Droite") == 0)
	    curr_set = RR[8];
	  else if (strcmp (Objet, "Cote_10_Droite") == 0)
	    curr_set = RR[9];
	  else if (strcmp (Objet, "Cote_11_Droite") == 0)
	    curr_set = RR[10];
	  else if (strcmp (Objet, "Cote_12_Droite") == 0)
	    curr_set = RR[11];
      
	  else if (strcmp (Objet, "Sternum") == 0)
	    curr_set = sternum;     
	  else if (strcmp (Objet, "Capteurs") == 0)
	    curr_set = landmark;
	  else if (strcmp (Objet, "Bassin") == 0)
	    curr_set = bassin;            
	  else
	    {
	      // can't recongnize the object
	      vtkErrorMacro (<<"Error in reading file");
	      everything_ok = 0;	// (false)
	    }
	}
      else
	{
	  if ((curr_set != NULL)
	      &&
	      (sscanf (line, "%s %f %f %f %s", t1, xyz, xyz + 1, xyz + 2, t)
	       == 5))
	    {
	      curr_set->InsertNextPoint (xyz);
	    }
	}
    }

  fclose (in);
  
  switch(type){
    case 0:  // ligament modeling  
     ligament_Combine();
     break;
    case 1:  // Vertebra modeling
     vertebra_Combine();
     break;
    case 2:  //get_points
     get_points(part, position);
     break;
    case 3:
     deform();
     break;
    case 4: // extract landmark points
     get_points(3,0);     
     break;
    case 5: // ligament between rib and vertebra.
     vertebra_rib_ligament();
     break;
     
  }
}

bool
GetCenter (vtkPoints * set, double p[3], double &radius, int startId, int endId)
{

  if ((set == NULL) && (set->GetNumberOfPoints () == 0))
    return false;
  int num = endId - startId + 1;
  
  double tmp[3], r;
  p[0] = 0.0;
  p[1] = 0.0;
  p[2] = 0.0;

  for (int i = startId; i <= endId; i++)
    {
      set->GetPoint (i, tmp);
      p[0] += tmp[0];
      p[1] += tmp[1];
      p[2] += tmp[2];
    }

  p[0] = p[0] / num;
  p[1] = p[1] / num;
  p[2] = p[2] / num;

  r = 0.0;
  for (int i = 0; i < num; i++)
    {
      set->GetPoint (i, tmp);
      r += sqrt (vtkMath::Distance2BetweenPoints (tmp, p));
    }

  radius = r / num;
  return true;
}

void
vtkO3Reader::ligament_Combine ()
{
  vtkPoints *center_p = vtkPoints::New ();
  vtkDoubleArray *center_r = vtkDoubleArray::New ();
  double p[3], r;

  int i;
  for( i = 0; i < 17; i ++){
    if (!GetCenter (T[i], p, r,0,5))
      vtkErrorMacro (<<"Error in combine");
    center_p->InsertNextPoint (p);
    center_r->InsertNextValue (r);
  }  

  center->SetPoints (center_p);
  center->GetPointData ()->SetScalars (center_r);

  vtkCardinalSpline *spx = vtkCardinalSpline::New ();
  vtkCardinalSpline *spy = vtkCardinalSpline::New ();
  vtkCardinalSpline *spz = vtkCardinalSpline::New ();
  vtkCardinalSpline *spr = vtkCardinalSpline::New ();
  int inum = center_p->GetNumberOfPoints ();
  for (int i = 0; i < inum; i++)
    {
      center_p->GetPoint (i, p);
      spx->AddPoint (i, p[0]);
      spy->AddPoint (i, p[1]);
      spz->AddPoint (i, p[2]);
      spr->AddPoint (i, center_r->GetValue (i));
    }

  int onum = inum * step;
  double t;
  vtkPolyData *spine_polydata = vtkPolyData::New ();
  vtkPoints *spine_points = vtkPoints::New ();
  vtkDoubleArray *spine_scalar = vtkDoubleArray::New ();
  vtkCellArray *spine_lines = vtkCellArray::New ();
  spine_lines->InsertNextCell (onum);
  for (int i = 0; i < onum; i++)
    {
      t = 1.0 * i * (inum - 1) / (onum - 1);
      spine_points->InsertNextPoint (spx->Evaluate (t), spy->Evaluate (t),
				     spz->Evaluate (t));
      spine_scalar->InsertNextValue (spr->Evaluate (t) * this->factor);
      spine_lines->InsertCellPoint (i);
    }
  spine_polydata->SetPoints (spine_points);
  spine_polydata->SetLines (spine_lines);
  spine_polydata->GetPointData ()->SetScalars (spine_scalar);

  // if side_num > 2, then output the tube, or output the center line.
  if(this->side_num > 2)
  {
     vtkTubeFilter *profileTubes = vtkTubeFilter::New ();
     profileTubes->SetNumberOfSides (this->side_num);
     profileTubes->SetInput (spine_polydata);
//     profileTubes->SetVaryRadiusToVaryRadiusByAbsoluteScalar ();
     profileTubes->CappingOn ();
     profileTubes->Update ();
     this->GetOutput ()->DeepCopy (profileTubes->GetOutput ());
     profileTubes->Delete ();
  }else
     this->GetOutput ()->DeepCopy (spine_polydata);  
     
  spine_lines->Delete ();
  spine_points->Delete ();
  spine_scalar->Delete ();
  spine_polydata->Delete ();
  spr->Delete ();
  spx->Delete ();
  spy->Delete ();
  spz->Delete ();
  center_p->Delete ();
  center_r->Delete ();
}

void
vtkO3Reader::vertebra_rib_ligament()
{
  vtkPoints* newpts = vtkPoints::New();
  vtkDoubleArray* newsca = vtkDoubleArray::New();
  vtkPolyData* polydata = vtkPolyData::New();

  polydata->SetPoints(newpts);
  polydata->GetPointData()->SetScalars(newsca);
  
  double p[3], r;

  int i;
  for( i = 0; i < 17; i ++){
    GetCenter (T[i], p, r,0,5);
    newpts->InsertNextPoint (p);
    newsca->InsertNextValue (r*2);
  }
  
  this->GetOutput()->DeepCopy (polydata);
  polydata->Delete();
  newpts->Delete();
  newsca->Delete(); 
}


void
vtkO3Reader::vertebra_Combine ()
{
  vtkPoints *center_p = vtkPoints::New ();
  vtkDoubleArray *center_r = vtkDoubleArray::New ();
  double p[3], r;


  for(int i = 0; i < 17; i ++){
    if (!GetCenter (T[i], p, r,0,5))
      vtkErrorMacro (<<"Error in combine");
    center_p->InsertNextPoint (T[i]->GetPoint(5));
    center_p->InsertNextPoint (T[i]->GetPoint(4));    
    center_r->InsertNextValue (r);
    center_r->InsertNextValue (r);
  }

  center->SetPoints (center_p);
  center->GetPointData ()->SetScalars (center_r);

  vtkCardinalSpline *spx = vtkCardinalSpline::New ();
  vtkCardinalSpline *spy = vtkCardinalSpline::New ();
  vtkCardinalSpline *spz = vtkCardinalSpline::New ();
  vtkCardinalSpline *spr = vtkCardinalSpline::New ();
  int inum = 34;
  for (int i = 0; i < inum; i++)
    {
      center_p->GetPoint (i, p);
      spx->AddPoint (i, p[0]);
      spy->AddPoint (i, p[1]);
      spz->AddPoint (i, p[2]);
      spr->AddPoint (i, center_r->GetValue (i));
    }

  int onum = inum * step;
  double t;
  vtkAppendPolyData *append = vtkAppendPolyData::New();

  for ( int k = 0; k < 17; k++)
  {
      vtkPolyData *spine_polydata = vtkPolyData::New ();
      vtkPoints *spine_points = vtkPoints::New ();
      vtkDoubleArray *spine_scalar = vtkDoubleArray::New ();
      vtkCellArray *spine_lines = vtkCellArray::New ();
      spine_lines->InsertNextCell (2*step);
      for (int i = 2*k*step; i < 2*(k+1)*step; i++)
        {
          t = 1.0 * i * (inum - 1) / (onum - 1);
          spine_points->InsertNextPoint (spx->Evaluate (t), spy->Evaluate (t),
    				     spz->Evaluate (t));
          spine_scalar->InsertNextValue (spr->Evaluate (t) * this->factor);
          spine_lines->InsertCellPoint (i-2*k*step);
        }

      spine_polydata->SetPoints (spine_points);
      spine_polydata->SetLines (spine_lines);
      spine_polydata->GetPointData ()->SetScalars (spine_scalar);
      vtkTubeFilter *profileTubes = vtkTubeFilter::New ();
      profileTubes->SetNumberOfSides (this->side_num);
      profileTubes->SetInput (spine_polydata);
//      profileTubes->SetVaryRadiusToVaryRadiusByAbsoluteScalar (); //rola remove.... odd!!!!!
      profileTubes->CappingOn ();
      profileTubes->Update ();
      append->AddInput(profileTubes->GetOutput());
      append->Update();
      profileTubes->Delete ();
      spine_lines->Delete ();
      spine_points->Delete ();
      spine_scalar->Delete ();
      spine_polydata->Delete ();
  }
  this->GetOutput ()->DeepCopy (append->GetOutput ());
  append->Delete();
  spr->Delete ();
  spx->Delete ();
  spy->Delete ();
  spz->Delete ();
  center_p->Delete ();
  center_r->Delete ();
}

void
vtkO3Reader::get_points(int part, int position)
{
  vtkPolyData* polydata = vtkPolyData::New();

  switch(part){
    case 0: //vertebra
      if(position>16)
        vtkErrorMacro (<<"Vertebra Postion shouldn't greater than 16");
      polydata->SetPoints(T[position]);
      break;
    case 1: //right side rib        
      if(position > 11)
        vtkErrorMacro (<<"<Rib Postion shouldn't greater than 11");
      polydata->SetPoints(RR[position]);
      break;
    case 2: //left side rib
      if(position > 11)
        vtkErrorMacro (<<"<Rib Postion shouldn't greater than 11");
      polydata->SetPoints(RL[position]);
      break;
    case 3: // landmark
      polydata->SetPoints(landmark);
      break;
  }                       
  this->GetOutput ()->DeepCopy (polydata);
  polydata->Delete();         
}

void
vtkO3Reader::deform()
{
  if(this->model==NULL)
    vtkErrorMacro (<<"set model first!");
  vtkAppendPolyData *append = vtkAppendPolyData::New();
  
  for( int i = begin; i<= end; i++){
      vtkThinPlateSplineTransform *Trans0 = vtkThinPlateSplineTransform::New ();
      vtkGeneralTransform *t1 = vtkGeneralTransform::New ();
      t1->SetInput (Trans0);
      vtkTransformPolyDataFilter *f11 = vtkTransformPolyDataFilter::New ();
      f11->SetInput (this->model);
      f11->SetTransform (t1);

      Trans0->SetSourceLandmarks (this->model_mark);
      Trans0->SetTargetLandmarks (T[i]);
      Trans0->SetBasis (VTK_RBF_R);
      Trans0->Update ();
      t1->Update();    
      f11->Update ();
      append->AddInput(f11->GetOutput());
      append->Update();
      f11->Delete ();
      t1->Delete ();
      Trans0->Delete ();    
  }
  this->GetOutput()->DeepCopy(append->GetOutput());
  append->Delete();
}  

void
vtkO3Reader::SetModel(vtkPolyData* model, vtkPoints* model_mark, int begin , int end)
{
  this->model = model;
  this->model_mark = model_mark;
  this->begin = begin;
  this->end = end;  
}

void
vtkO3Reader::SetType(int type,int part,int position)
{
  this->type = type;
  this->part = part;
  this->position = position;
}
  
void
vtkO3Reader::SetNumberOfSides (int side_num)
{
  this->side_num = side_num;
}

void
vtkO3Reader::SetRadiusFactor (double factor)
{
  this->factor = factor;
}

void
vtkO3Reader::SetNumberOfSteps (int step)
{
  this->step = step;
}


void
vtkO3Reader::PrintSelf (ostream & os, vtkIndent indent)
{
  this->Superclass::PrintSelf (os, indent);

  os << indent << "File Name: "
    << (this->FileName ? this->FileName : "(none)") << "\n";

}

