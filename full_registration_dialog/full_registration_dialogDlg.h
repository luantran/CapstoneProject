// full_registration_dialogDlg.h : header file
//

#pragma once

#include "transform.h"
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
#include <vtkWin32OpenGLRenderWindow.h>
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
//#include <vtkGdcmReader.h>
//#include <vtkDICOMImageReader.h>
#include <vtkPlanes.h>
#include <vtkIterativeClosestPointTransform.h>
#include <vtkColorTransferFunction.h>
#include <vtkImplicitModeller.h>
#include <vtkImageCast.h>
#include <vtkImageEuclideanDistance.h>
#include <cmath>
#include "afxwin.h"
#include "afxcmn.h"
#include <vtkSmartPointer.h>

// Cfull_registration_dialogDlg dialog
class Cfull_registration_dialogDlg : public CDialog
{
	// Construction
public:
	Cfull_registration_dialogDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	enum { IDD = IDD_FULL_REGISTRATION_DIALOG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	// Implementation
protected:
	HICON m_hIcon;



	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	vtkSmartPointer<vtkRenderWindowInteractor> iren;
	vtkSmartPointer<vtkRenderer> aRenderer;
	vtkSmartPointer<vtkCamera> aCamera;
	vtkSmartPointer<vtkRenderWindow> renWin;
	vtkPolyData * polydatawrl;
	vtkPolyData * polydatasze;
	vtkPolyData * polydataszeshow;
	vtkPolyData * polydatawrlreg;
	vtkPolyData * polydataszereg;
	topo_data unregisteredtopo;
	xray_data unregisteredxray;
	landmark_data_mri themripeds;
	landmark_data_mri thexraypeds;
	vtkPolyData *mri_newlandmarkspart[17];
	vtkPolyData *mriSlicePdata;
	rigid_vertebrae polydatawrl2vert; 
	rigid_vertebrae polydatawrl3vert; 
	rigid_vertebrae polydatawrl4vert; 
	int mri_dimx, mri_dimy, mri_dimz, num_mris, mrirange1[2], mrirange2[2], mrirange3[2],mrirange_axial1[2],mrirange_axial2[2], mri_intensity1,mri_intensity2,mri_intensity3, mri_intensity_axial1,mri_intensity_axial2;
	double t[3], ox[3],	 oy[3],  t2[3],  ox2[3], oy2[3],  t3[3], ox3[3], oy3[3], cp3[3],  mrimv ;
	double t_axial1[3],ox_axial1[3],oy_axial1[3],t_axial2[3],ox_axial2[3],oy_axial2[3];
	double mri_spacing[3], mri_spacing_axial[3];
	char thefilemri1[200], thefilemri2[200], thefilemri3[200], thefilemri_axial1[200],thefilemri_axial2[200],thefilemrivert2[200], thefilemrivert3[200], thefilemrivert1[200];
	double cp2[3],cp[3];
	char patientname[200];
	char patientpath[200];
	const char *thefilexray;
	const char *thefilexray2;
	const char *xraylandmarks;
	const char *topofile;
	int slicetobereg;
	int slicetoshow;
	char reptxfile[200], o3txfile[200];
	int xray2_vertindex; //start at L5
	int xray3_vertindex; //start at L4
	int xray4_vertindex;
	int numimages; 
	int rigidverts;
	int theslice[40];
	int currentsaveslice;
	double finalSliceOrigin[3];
	bool registered;
	vtkMatrix4x4 *mrivert_regvol[17];
	vtkMatrix4x4 *voloriginmri1;
	vtkMatrix4x4 *voloriginmri2;
	vtkMatrix4x4 *voloriginmri1display; 
	vtkMatrix4x4 *volmri1final; //keep track of tx to store at end
vtkImageData* testderivative;
	vtkImageData **imageData_ax1perslice;
	vtkImageData *imageData_ax1rigid;
	vtkImageData *tempimage;
	vtkImageData *tempimage2;
	vtkImageData* TPvolume;
	afx_msg void OnBnClickedCancel();
	vtkActor *szeactor;
	vtkActor *wrlactor;
	vtkActor *spikeActorsze;
	vtkActor *spikeActorslice;
	vtkActor *spikeActorslice2;
	vtkActor *spikeActorwrl;
	vtkActor *spikeActorwrlvert;
	vtkActor *spikeActormrivert;
	vtkActor *spikeActormri[17];
	vtkActor *wrlactor2[17];
	vtkActor *wrlactor3[17];
	vtkActor *wrlactor4[17];
	vtkSmartPointer<vtkImageActor> axialImageActor;
	 vtkSmartPointer<vtkImageActor> axialImageActor2[40];
	CSpinButtonCtrl *pSpin;
	vtkImagePlaneWidget *planeWidgetX6[40];
	vtkImagePlaneWidget *planeWidgetX;
	vtkImagePlaneWidget *planeWidgetY;
	vtkImagePlaneWidget *planeWidgetZ;
	vtkImagePlaneWidget *planeWidgetX2;
	vtkImagePlaneWidget *planeWidgetY2;
	vtkImagePlaneWidget *planeWidgetZ2;
	vtkImagePlaneWidget *planeWidgetX3;
	vtkImagePlaneWidget *planeWidgetY3;
	vtkImagePlaneWidget *planeWidgetZ3;
	vtkImagePlaneWidget *planeWidgetX4;
	vtkImagePlaneWidget *planeWidgetY4;
	vtkImagePlaneWidget *planeWidgetZ4;
	vtkImagePlaneWidget *planeWidgetX5;
	vtkImagePlaneWidget *planeWidgetY5;
	vtkImagePlaneWidget *planeWidgetZ5;
	
	vtkTransform *volumeoriginaxial2;
	vtkTransform *vtlaxial1tx;
	//vtkLandmarkTransform *Transrigid2;
		vtkMatrix4x4 *rigidTransfoMatrix2; 
	vtkImageReslice *reslice3;
	
	vtkImageReslice *resliceax2;
	vtkImageData *imageData3 ;
	vtkSmartPointer<vtkLookupTable> bwLutaxial;
	vtkSmartPointer<vtkLookupTable> bwLutaxial2;
	vtkSmartPointer<vtkImageData> imageData_ax1;
	vtkSmartPointer<vtkImageData> imageData_ax2;
	//vtkImageData *imageData_ax2;
	vtkImageData *imageData_ax3;
	vtkImageData* MRIregistered[40];
	vtkCellPicker *picker;
	 vtkSmartPointer<vtkVolume16Reader> v18ax;
	  vtkSmartPointer<vtkVolume16Reader> v19ax;
	//vtkVolume16Reader *v19ax;
	
	CButton xraycheck;
	CButton topocheck;
	CButton extlandcheck;
	CButton vertlandcheck;
	CButton mricheck;
	CButton mrivertcheck;
	CButton mrisagcheck;
	CEdit mstatusedit;
	CSpinButtonCtrl p_spin;

	afx_msg void OnBnClickedChecktopo();
	afx_msg void OnBnClickedCheckxray();
	afx_msg void OnBnClickedCheckextland();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButtonrigid();
	afx_msg void OnBnClickedButtonmriload();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButtonarticulated();
	afx_msg void OnBnClickedCheckvertland();
	afx_msg void OnBnClickedCheckmrisag();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButtonloadxray();
	afx_msg void OnBnClickedButtonloadsurface();
	afx_msg void OnBnClickedVolumekappabackkkk();

	void ShowMRIsag();
	void showxrayvertlandmarks();
	void showmrivertlandmarks();
	void showxray();
	void showtopo();
	void showxraylandmarks();
	void showtopolandmarks();
	void refreshscreen();
	void ShowMRI();
	void showMRIverts();
	void DisplayVisibleActors();
	void loadMRI();
	void showtemp();


	afx_msg void OnBnClickedCheckmrivert();
	afx_msg void OnBnClickedCheckmri();
	afx_msg void OnBnClickedButtonarticulated2();
	afx_msg void OnDeltaposSpinslice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonsavetp();
	afx_msg void OnBnClickedButtonsavemri();
	afx_msg void OnBnClickedButtonloadreg();
	afx_msg void OnBnClickedButtonstats();
	afx_msg void OnBnClickedButtonloadreg2();
	int edtNumSlice;
	afx_msg void OnBnClickedVolumekappa();
};
