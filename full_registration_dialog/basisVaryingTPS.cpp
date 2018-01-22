#include "StdAfx.h"
#include "basisVaryingTPS.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>


 #define TINY 1.0e-20; 
using namespace boost::numeric::ublas;
typedef permutation_matrix<std::size_t> pmatrix;

basisVaryingTPS::basisVaryingTPS(void)
{
	this->P = NULL;
	this->V = NULL;
	this->nNumLandmarks = 0;
	this->W = NULL;
//	this->EdgePoints = vtkPoints::New();

	this->angleMRIdistances = new float*[3];
	for(int i = 0;i <3;i++)
		this->angleMRIdistances[i] = new float[360];
}


basisVaryingTPS::~basisVaryingTPS(void)
{
}

void basisVaryingTPS::setVertMiddle(double x,double y, double z)
{
	this->vertebralCenter[0] = x;
	this->vertebralCenter[1] = y;
	this->vertebralCenter[2] = z;
}

void basisVaryingTPS::SetNumLandmarks(int theNum)
{
	this->nNumLandmarks = theNum;
}

void basisVaryingTPS::SetSourcePoints(vtkPolyData * sourcePoints)
{
	this->P = new double[nNumLandmarks][4]; //row then column

	vtkFloatArray *values = vtkFloatArray::New();
	vtkPoints *P_points =vtkPoints::New();
	P_points = sourcePoints->GetPoints();
	values->DeepCopy(P_points->GetData());
	for(int i=0;i<nNumLandmarks;i++)
	{
		values->GetTuple(i,P[i]);//row then column
		P[i][3]=P[i][2];
		P[i][2]=P[i][1];
		P[i][1]=P[i][0];
		P[i][0]=1;
	}
}

void basisVaryingTPS::SetTargetPoints(vtkPolyData * targetPoints)
{
	this->V = new double[nNumLandmarks][3];
	vtkFloatArray *values2 = vtkFloatArray::New();
	vtkPoints *Q_points =vtkPoints::New();
	Q_points = targetPoints->GetPoints();
	values2->DeepCopy(Q_points->GetData());

	for(int i=0;i<nNumLandmarks;i++)
	{
		values2->GetTuple(i,V[i]);
	/*	this->V[i][0] = (double)temp[0];
		this->V[i][1] = (double)temp[1];
		this->V[i][2] = (double)temp[2];*/
	}
}


void basisVaryingTPS::setVertebralRadius(double theradius)
{

	vertebralRadius = theradius;
}

void basisVaryingTPS::SetEdgePoints(float** theEdgePoints)
{
		//this->angleMRIdistances = new float*[3];
	for(int i = 0;i <3;i++)
	{
		for(int j = 0; j<360;j++)
		{
			this->angleMRIdistances[i][j]= theEdgePoints[i][j];
		}
	}
}


void basisVaryingTPS::ComputeWeights()
{

double **R_squared = new double*[nNumLandmarks];
double **K = new double*[nNumLandmarks];
double **L = new double*[nNumLandmarks+4]; 
double **Y = new double*[nNumLandmarks+4]; 
this->W = new double*[nNumLandmarks+4]; 
double **invL = new double*[nNumLandmarks+4]; 

for (int i=0; i< nNumLandmarks; i++)
{
	R_squared[i]= new double[nNumLandmarks];
	K[i]= new double[nNumLandmarks];
}

for (int i=0; i< nNumLandmarks+4; i++)
{
	L[i]= new double[nNumLandmarks+4];
	invL[i]= new double[nNumLandmarks+4];
	this->W[i]= new double[3];
	Y[i]=new double[3];
}

	for(int i=0;i<nNumLandmarks;i++)
	{
	for(int j=0;j<nNumLandmarks;j++)
	{
		R_squared[i][j]=pow(this->P[i][1]-this->P[j][1],2.0) + pow(this->P[i][2]-this->P[j][2],2.0)+pow(this->P[i][3]-this->P[j][3],2.0);
		
		double thelog;
		if (R_squared[i][j]==0)
			thelog=0;
		else thelog = log(R_squared[i][j]);

		K[i][j]=R_squared[i][j]*thelog;
		L[i][j]=K[i][j];
	}
	}


for(int i=0;i<nNumLandmarks;i++)
{
	for(int j=0;j<4;j++)
	{
		L[i][nNumLandmarks+j]=this->P[i][j];
		L[nNumLandmarks+j][i]=this->P[i][j];
	}
	}

//add the zero natrix, 4 by 4 matrix here, this part is ok
for(int i=0;i<4;i++)
{
	for(int j=0;j<4;j++)
		L[nNumLandmarks+j][nNumLandmarks+i]=0;
}


//useless but whatevever , V is really V_transpose
for(int i=0;i<nNumLandmarks;i++)
{
	for(int j=0;j<3;j++)
		Y[i][j]= V[i][j];
}

for(int i=0;i<4;i++)
{
	for(int j=0;j<3;j++)
		Y[nNumLandmarks+i][j]= 0;
}

int sizeL=nNumLandmarks+4;
invL = inverse(L, sizeL);

//multiply the inverse of L with Y to find the weights...
this->W=matrix_mult(invL, Y, sizeL , 3);
/*
for(int i=0;i<sizeL;i++)
{
	for(int j=0;j<3;i++)
	{
		double wval = W[j][i];
		int caca=1;
	}
}
*/
//try with boost
matrix<double> P2(nNumLandmarks,4); //a n by 4: row then column
matrix<double> L2(nNumLandmarks+4, nNumLandmarks+4);
matrix<double> W2(4, nNumLandmarks+4);
matrix<double> Y2(nNumLandmarks+4,4);
matrix<double> mtx_invl(nNumLandmarks+4, nNumLandmarks+4);

for(int i = 0; i<nNumLandmarks;i++)
{
	for(int j = 0; j<4;j++)
		P2(i,j) = P[i][j];
}

for(int i = 0; i<nNumLandmarks;i++)
{
	for(int j = 0; j<3;j++)
		Y2(i,j) = V[i][j];
}

for(int i = 0; i<4;i++)
{
	for(int j = 0; j<3;j++)
		Y2(i+nNumLandmarks,j) =0;
}

for(int i=0;i<nNumLandmarks;i++)
{
	for(int j=0;j<nNumLandmarks;j++)
	{
		L2(i,j)=K[i][j];
	}
}

for(int i=0;i<nNumLandmarks;i++)
{
	for(int j=0;j<4;j++)
	{
		L2(i,j+nNumLandmarks)=P2(i,j);
		L2(j+nNumLandmarks,i)=P2(i,j);
	}
}

for(int i=0;i<4;i++)
{
	for(int j=0;j<4;j++)
		L2(i+nNumLandmarks,j+nNumLandmarks)=0;
}

	InvertMatrix(L2,mtx_invl);
	//multiply invl by matx_v
	W2=prod(mtx_invl,Y2); //this isnt working

	//store w's
	for(int i=0;i<nNumLandmarks+4;i++)
	{
		for(int j=0;j<3;j++)
		{
			this->W[i][j] = W2(i,j) ;
		}
	}

}

vtkPolyData * basisVaryingTPS::DeformPoints(vtkPolyData * sourcePoints)
{
	int i,j;
	vtkPoints * newlandmark_points = vtkPoints::New();
double *r_squared = new double[nNumLandmarks];
	double **Q_new= new double*[nNumLandmarks];

	for (i=0; i< nNumLandmarks; i++)
		Q_new[i]= new double[3];

	double thelog;
	for(i=0;i<nNumLandmarks;i++)
	{
		for(j=0;j<3;j++)
			Q_new[i][j]=0;

		for(j=0;j<nNumLandmarks;j++)
		{
			r_squared[j]=pow(P[i][1]-P[j][1],2)+pow(P[i][2]-P[j][2],2)+pow(P[i][3]-P[j][3],2);	
			if (r_squared[j]==0)
				thelog=0;
			else thelog = log(r_squared[j]);

			Q_new[i][0]+=r_squared[j]*thelog*W[j][0];
			Q_new[i][1]+=r_squared[j]*thelog*W[j][1];
			Q_new[i][2]+=r_squared[j]*thelog*W[j][2];
		}

		Q_new[i][0]+=W[nNumLandmarks][0]+W[nNumLandmarks+1][0]*P[i][1]+W[nNumLandmarks+2][0]*P[i][2]+W[nNumLandmarks+3][0]*P[i][3];
		Q_new[i][1]+=W[nNumLandmarks][1]+W[nNumLandmarks+1][1]*P[i][1]+W[nNumLandmarks+2][1]*P[i][2]+W[nNumLandmarks+3][1]*P[i][3];
		Q_new[i][2]+=W[nNumLandmarks][2]+W[nNumLandmarks+1][2]*P[i][1]+W[nNumLandmarks+2][2]*P[i][2]+W[nNumLandmarks+3][2]*P[i][3];

		newlandmark_points->InsertNextPoint(Q_new[i][0],Q_new[i][1],Q_new[i][2]);
		//newlandmark_points->InsertNextPoint(P[i][1],P[i][2],P[i][3]);
}

vtkPolyData *outlandmarks =vtkPolyData::New();
outlandmarks->SetPoints(newlandmark_points);

return outlandmarks;
}

vtkImageData* basisVaryingTPS::DeformGridrBody(vtkImageData* inputGrid)
{
//apply a tps registration to each point in the grid. 
	//return inputGrid;
	//first get all the grid info.
	int gridextent[6];
	float *tttemp = new float[3];
	double *bounds = new double[6];
	double *xyzCoords  = new double[3];
	inputGrid->GetExtent(gridextent);
//	inputGrid->SetScalarTypeToFloat();
//	inputGrid->SetNumberOfScalarComponents(3);

	//vtkFloatArray *scalarsdeformation2 = vtkFloatArray::New();
	vtkSmartPointer<vtkDataArray> scalarsdeformation2= vtkSmartPointer<vtkFloatArray>::New();
	scalarsdeformation2->SetNumberOfComponents(3);
	scalarsdeformation2 = inputGrid->GetPointData()->GetScalars();


		for (int iii=0;iii<=gridextent[5];iii++) //=gridextent[5]
		{
		for (int jjj=0;jjj<=gridextent[3];jjj++)
		{
			for (int kkk=0;kkk<=(gridextent[1]);kkk++)
			{
				
				int offsetdeformation = (int)(((gridextent[3]+1)*(gridextent[1]+1)*(iii))+(gridextent[1]+1)*jjj+kkk);
				int offsetgrid = (int)(((gridextent[3])*(gridextent[1])*(iii))+(gridextent[1])*jjj+kkk);
				//tttemp = scalarsdeformation2->GetTuple3(offsetdeformation);
				inputGrid->GetCell(offsetgrid)->GetBounds(bounds);
				xyzCoords[0]=(double)bounds[0];xyzCoords[1]=(double)bounds[2];xyzCoords[2]=(double)bounds[4];
				//scalarsdeformation2->SetTuple3(offsetdeformation,0.5,0.5,0.5); //here not changing
				double *current_def = new double[3];
				current_def = scalarsdeformation2->GetTuple3(offsetdeformation);
				double xdist = xyzCoords[0]-this->vertebralCenter[0];
				double ydist = xyzCoords[1]-this->vertebralCenter[1];

				//find right angle depending on quandrant. right now, angle is between -90 and 90 x is positive
				int pointangle = (int)((atan2 ((abs)(ydist),(abs)(xdist)) * 180 / 3.1416)); //+0.5

			/*	if ((pointangle%3) ==1)
					pointangle-=1;
				if ((pointangle%3) ==2)
					pointangle-=2;*/
				//assuming it's all between 0 and 90
				if((xdist < 0) && (ydist <= 0))
				{
					//3rd quadrant and atan is between 0 and 90, should be between 180 and 270 so add 180
					pointangle+=180;
				}

				if((xdist < 0) && (ydist > 0))
				{
					//2nd quandrant atan between 90 and 0, should be between 90 and 180
					pointangle=-pointangle+ 180;
				}
				if((xdist >= 0) && (ydist < 0))
				{
					//4th quandrant atan between 90 and 0, should be between 270 and 360
					pointangle=-pointangle+360;
				}
				

			/*	if(((int)xdist == 0) && (ydist > 0))
					pointangle=90;
				if((xdist < 0) && ((int)ydist == 0))
					pointangle=180;

				if(((int)xdist == 0) && (ydist < 0))
					pointangle=270;*/ 
				if((xdist >= 0) && (abs(ydist) < 2)) 
					pointangle=0;
				if((xdist < 0) && (abs(ydist) < 2)) 
					pointangle=180;
							
				//use angle to find distance from point to endge  angleMRIdistances[][]
				double dvert=sqrt(pow(xdist,2)+pow(ydist,2));//xdist = xyzCoords[0]-this->vertebralCenter[0]; ydist = xyzCoords[1]-this->vertebralCenter[1];
				double dedge = sqrt(pow(xyzCoords[0]-(double)angleMRIdistances[0][pointangle],2)+pow(xyzCoords[1]-(double)angleMRIdistances[1][pointangle],2));
				double dedgevert = sqrt(pow(this->vertebralCenter[0]-(double)angleMRIdistances[0][pointangle],2)+pow(this->vertebralCenter[1]-(double)angleMRIdistances[1][pointangle],2));
				
				//vertebralRadius=0;
				//adapted warpratio taking into account if i am inside vertebral radius
				float warpratio = (float)(dvert-vertebralRadius)/ (float)(dvert+dedge-vertebralRadius);
				if( dvert<vertebralRadius) 
					warpratio=0.0;

				if(dvert>dedgevert) //we are outside the border of the MRI
					warpratio=1.0;


				scalarsdeformation2->SetTuple3(offsetdeformation,warpratio*(float)(current_def[0]),warpratio*(float)(current_def[1]),warpratio*(float)(current_def[2]));
			//scalarsdeformation2->SetTuple3(offsetdeformation,(float)pointangle,(float)pointangle,(float)pointangle);
		
			}
		}
	}

		//set the deformation grid info
//		inputGrid->AllocateScalars();
//		inputGrid->GetPointData()->CopyAllOn();
//		inputGrid->GetPointData()->CopyScalarsOn();
		inputGrid->GetPointData()->SetScalars(scalarsdeformation2);
		inputGrid->Update();

		return inputGrid;
}

vtkImageData* basisVaryingTPS::DeformGrid(vtkImageData* inputGrid)
{
//apply a tps registration to each point in the grid. 
	//return inputGrid;
	//first get all the grid info.
	int gridextent[6];
	float *tttemp = new float[3];
	double *bounds = new double[6];
	double *xyzCoords  = new double[3];
	double deformedPoint[3];
	inputGrid->GetExtent(gridextent);
	inputGrid->SetScalarTypeToFloat();
	inputGrid->SetNumberOfScalarComponents(3);

	vtkDataArray *scalarsdeformation2 = vtkFloatArray::New();
	scalarsdeformation2->SetNumberOfComponents(3);
	scalarsdeformation2 = inputGrid->GetPointData()->GetScalars();

		double **R_squared_forlog = new double*[nNumLandmarks];
double **R_squared = new double*[nNumLandmarks];
double **K = new double*[nNumLandmarks];
double **Q_new= new double*[nNumLandmarks];

double *r_squared = new double[nNumLandmarks];
double thelog;
		for (int iii=0;iii<=gridextent[5];iii++) //=gridextent[5]
		{
		for (int jjj=0;jjj<=gridextent[3];jjj++)
		{
			for (int kkk=0;kkk<=(gridextent[1]);kkk++)
			{
				int offsetdeformation = (int)(((gridextent[3]+1)*(gridextent[1]+1)*(iii))+(gridextent[1]+1)*jjj+kkk);
				int offsetgrid = (int)(((gridextent[3])*(gridextent[1])*(iii))+(gridextent[1])*jjj+kkk);
				//tttemp = scalarsdeformation2->GetTuple3(offsetdeformation);
				inputGrid->GetCell(offsetgrid)->GetBounds(bounds);
				xyzCoords[0]=(double)bounds[0];xyzCoords[1]=(double)bounds[2];xyzCoords[2]=(double)bounds[4];


				for(int j=0;j<3;j++)
					deformedPoint[j]=0;

				for(int j=0;j<nNumLandmarks;j++)
				{
						r_squared[j]=pow(xyzCoords[0]-P[j][1],2)+pow(xyzCoords[1]-P[j][2],2)+pow(xyzCoords[2]-P[j][3],2);	
						if (r_squared[j]==0)
							thelog=0;
						else thelog = log(r_squared[j]);
						deformedPoint[0]+=r_squared[j]*thelog*this->W[j][0];
						deformedPoint[1]+=r_squared[j]*thelog*this->W[j][1];
						deformedPoint[2]+=r_squared[j]*thelog*this->W[j][2];
				}

				deformedPoint[0]+=W[nNumLandmarks][0]+W[nNumLandmarks+1][0]*xyzCoords[0]+W[nNumLandmarks+2][0]*xyzCoords[1]+W[nNumLandmarks+3][0]*xyzCoords[2];
				deformedPoint[1]+=W[nNumLandmarks][1]+W[nNumLandmarks+1][1]*xyzCoords[0]+W[nNumLandmarks+2][1]*xyzCoords[1]+W[nNumLandmarks+3][1]*xyzCoords[2];
				deformedPoint[2]+=W[nNumLandmarks][2]+W[nNumLandmarks+1][2]*xyzCoords[0]+W[nNumLandmarks+2][2]*xyzCoords[1]+W[nNumLandmarks+3][2]*xyzCoords[2];

			//	scalarsdeformation2->SetTuple3(offsetdeformation,deformedPoint[0]-xyzCoords[0],deformedPoint[1]-xyzCoords[1],deformedPoint[2]-xyzCoords[2]);
			scalarsdeformation2->SetTuple3(offsetdeformation,(float)(deformedPoint[0]-xyzCoords[0]),(float)(deformedPoint[1]-xyzCoords[1]),(float)(deformedPoint[2]-xyzCoords[2]));

			}
		}
	}

		//set the deformation grid info
		inputGrid->AllocateScalars();
		inputGrid->GetPointData()->CopyAllOn();
		inputGrid->GetPointData()->CopyScalarsOn();
		inputGrid->GetPointData()->SetScalars(scalarsdeformation2);
		inputGrid->Update();

		return inputGrid;
}

double** basisVaryingTPS::matrix_mult(double **invL, double **Y, int sizeL, int sizeY)
{
	   int i, j,k;

	  double **result;
	  result = new double*[sizeL];
	   for (i=0; i<sizeL; i++)
			   result[i] = new double[sizeY];
//Matrices are sizeL by sizeL multiplied by sizeL by SizeY, result: sizeL by sizeY


for(i=0; i<sizeL;i++)
{
for(j=0; j<sizeY;j++)
{
	result[i][j]=0;
	for(k=0;k<sizeL;k++)
		result[i][j]+=invL[i][k]*Y[k][j];
}}
  return result;
}

void basisVaryingTPS::ludcmp(double** a, int n, int *indx, double *d) {
	   int i, imax, j, k;
	   double big, dum, sum, temp;
	   double *vv;

	   vv = new double[n];
	   *d=1.0;
	   for (i=0; i<n; i++) {
			   big=0.0;
			   for (j=0; j<n; j++) {
					   if ((temp=fabs(a[i][j]))>big) big=temp;
			   }
			   if (big==0.0) {
					   printf("Singular matrix in routine");
					   exit(1);
			   }
			   vv[i]=1.0/big;
	   }
	   for (j=0; j<n; j++) {
			   for (i=0; i<j; i++) {
					   sum=a[i][j];
					   for (k=0; k<i; k++) sum-=a[i][k]*a[k][j];
					   a[i][j]=sum;
			   }
			   big=0.0;
			   for (i=j; i<n; i++) {
					   sum=a[i][j];
					   for (k=0; k<j; k++)  sum-=a[i][k]*a[k][j];

					   a[i][j]=sum;
					   if ((dum=vv[i]*fabs(sum)) >= big) {
							   big=dum;
							   imax=i;
					   }
			   }
			   if (j!=imax) {
					   for (k=0; k<n; k++) {
							   dum=a[imax][k];
							   a[imax][k]=a[j][k];
							   a[j][k]=dum;
					   }
					   *d = -(*d);
					   vv[imax]=vv[j];
			   }
			   indx[j]=imax;
			   if (a[j][j]==0.0) a[j][j]=TINY;
			   if (j!=n-1) {
					   dum=1.0/a[j][j];
					   for (i=j+1; i<n; i++) a[i][j]*=dum;
			   }
	   }
	   delete []vv;
}

void basisVaryingTPS::lubksb(double **a, int n, int *indx, double b[]) {
	   int i,ii=0, ip, j;
	   double sum;

	   for (i=0; i<n; i++) {
			   ip=indx[i];
			   sum=b[ip];
			   b[ip]=b[i];
			   if (i)
					   for (j=ii; j<=i-1; j++) sum-=a[i][j]*b[j];
			   else if (sum) ii=i;
			   b[i]=sum;
	   }
	   for (i=n-1; i>=0; i--) {
			   sum=b[i];
			   for (j=i+1; j<n; j++) sum-=a[i][j]*b[j];
			   b[i]=sum/a[i][i];
	   }
}


double** basisVaryingTPS::inverse(double **a, int n) {
	   double **y, **res;
	   int i,j;
	   double d;
	   int *index = new int[n];
	   double *col = new double[n];

	   y = new double*[n];
	   for (i=0; i<n; i++)
			   y[i] = new double[n];

	   res = new double*[n];
	   for (i=0; i<n; i++)
			   res[i] = new double[n];

	   for (i=0; i<n; i++)
			   for (j=0; j<n; j++)
					   res[i][j]=a[j][i];

	   ludcmp(res, n, index, &d);
	   for (j=0; j<n; j++) {
			   for (i=0; i<n; i++) col[i]=0.0;
			   col[j]=1.0;
			   lubksb(res, n, index, col);
			   for (i=0; i<n; i++) y[i][j]=col[i];
	   }

	   for (i=0; i<n; i++)
			   for (j=0; j<n; j++)
					   res[i][j]=y[j][i];

	   for (i=0; i<n; i++)
			   delete[] y[i];
	   delete[] y;

	   return res;
}

