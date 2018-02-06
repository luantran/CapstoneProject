// full_registration_dialogDlg.cpp : implementation file
//
#include <afxwin.h>
#include "stdafx.h"
#include "full_registration_dialog.h"
#include "full_registration_dialogDlg.h"
#include "vtkGridTransform.h"
#include "vtkShortArray.h"
#include "BSplineRegistration.h"
#include "vtkPlane.h"
#include "AffineRegistration.h"
#include <vtkTransformToGrid.h>
#include <vtkDecimatePro.h>
#include <vtkExtractEdges.h>
#include "basisVaryingTPS.h"
#include "AffineRegistration.h"
#include <vtkImageStencil.h> 
#include <vtkImageActor.h> 
#include <vtkImageMapToColors.h> 
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolume16Reader.h>
#include "vtkLinearSubdivisionFilter.h"
#include "vtkInterpolatingSubdivisionFilter.h"

//#pragma comment(lib, "C:\\vtk561\\bin\\bin\\Debug\\vtkRendering.lib") // Left 2 unresolveds

//#pragma comment(lib, "C:\\vtk561\\bin\\bin\\Debug\\vtkCommon.lib") // Left 2 unresolveds

//#pragma comment(lib, "C:\\vtk561\\bin\\bin\\Debug\\\vtkGraphics.lib") // Left 1 unresolveds

//#pragma comment(lib, "C:\\vtk561\\bin\\bin\\Debug\\vtkFiltering.lib") // Left 0 unresolveds

#ifdef _DEBUG

#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cfull_registration_dialogDlg dialog




Cfull_registration_dialogDlg::Cfull_registration_dialogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cfull_registration_dialogDlg::IDD, pParent)
	, edtNumSlice(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cfull_registration_dialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECKxray, xraycheck);
	DDX_Control(pDX, IDC_CHECKtopo, topocheck);
	DDX_Control(pDX, IDC_CHECKextland, extlandcheck);
	DDX_Control(pDX, IDC_CHECKvertland, vertlandcheck);
	DDX_Control(pDX, IDC_CHECKmri, mricheck);
	DDX_Control(pDX, IDC_CHECKmrivert, mrivertcheck);
	DDX_Control(pDX, IDC_CHECKMRIsag, mrisagcheck);
	//DDX_Control(pDX, IDC_EDITstatus, mstatusedit);
	DDX_Control(pDX, IDC_SPINslice, p_spin);
	DDX_Text(pDX, IDC_EDITregslice, edtNumSlice);
}

BEGIN_MESSAGE_MAP(Cfull_registration_dialogDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTONloadxray, &Cfull_registration_dialogDlg::OnBnClickedButtonloadxray)
	ON_BN_CLICKED(IDC_BUTTONloadsurface, &Cfull_registration_dialogDlg::OnBnClickedButtonloadsurface)
	ON_BN_CLICKED(IDCANCEL, &Cfull_registration_dialogDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECKxray, &Cfull_registration_dialogDlg::OnBnClickedCheckxray)
	ON_BN_CLICKED(IDC_CHECKtopo, &Cfull_registration_dialogDlg::OnBnClickedChecktopo)
	ON_BN_CLICKED(IDC_CHECKextland, &Cfull_registration_dialogDlg::OnBnClickedCheckextland)
	ON_BN_CLICKED(IDC_BUTTONrigid, &Cfull_registration_dialogDlg::OnBnClickedButtonrigid)
	ON_BN_CLICKED(IDC_BUTTONMRIload, &Cfull_registration_dialogDlg::OnBnClickedButtonmriload)
	ON_BN_CLICKED(IDC_BUTTONarticulated, &Cfull_registration_dialogDlg::OnBnClickedButtonarticulated)
	ON_BN_CLICKED(IDC_CHECKvertland, &Cfull_registration_dialogDlg::OnBnClickedCheckvertland)
	ON_BN_CLICKED(IDC_CHECKMRIsag, &Cfull_registration_dialogDlg::OnBnClickedCheckmrisag)
	ON_BN_CLICKED(IDC_CHECKmrivert, &Cfull_registration_dialogDlg::OnBnClickedCheckmrivert)
	ON_BN_CLICKED(IDC_CHECKmri, &Cfull_registration_dialogDlg::OnBnClickedCheckmri)
	ON_BN_CLICKED(IDC_BUTTONarticulated2, &Cfull_registration_dialogDlg::OnBnClickedButtonarticulated2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINslice, &Cfull_registration_dialogDlg::OnDeltaposSpinslice)
	ON_BN_CLICKED(IDC_BUTTONsaveTP, &Cfull_registration_dialogDlg::OnBnClickedButtonsavetp)
	ON_BN_CLICKED(IDC_BUTTONsaveMRI, &Cfull_registration_dialogDlg::OnBnClickedButtonsavemri)
	ON_BN_CLICKED(IDC_BUTTONloadreg, &Cfull_registration_dialogDlg::OnBnClickedButtonloadreg)
	ON_BN_CLICKED(IDC_BUTTONstats, &Cfull_registration_dialogDlg::OnBnClickedButtonstats)
	ON_BN_CLICKED(IDC_BUTTONloadreg2, &Cfull_registration_dialogDlg::OnBnClickedButtonloadreg2)
	ON_BN_CLICKED(IDC_VolumeKappa, &Cfull_registration_dialogDlg::OnBnClickedVolumekappa)
END_MESSAGE_MAP()


// Cfull_registration_dialogDlg message handlers

BOOL Cfull_registration_dialogDlg::OnInitDialog()
{


	pSpin = reinterpret_cast<CSpinButtonCtrl *>(GetDlgItem(IDC_SPINslice)); 
	pSpin->SetRange32(0,0);
	pSpin->SetPos32(0);
	pSpin->UpdateData(TRUE);



	CDialog::OnInitDialog();
	xraycheck.EnableWindow(FALSE);
	topocheck.EnableWindow(FALSE);
	extlandcheck.EnableWindow(FALSE);
	vertlandcheck.EnableWindow(FALSE);
	mricheck.EnableWindow(FALSE);
	mrivertcheck.EnableWindow(FALSE);
	mrisagcheck.EnableWindow(FALSE);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);
	//if(!ProcessShellCommand(cmdInfo)){
	//	return FALSE;
	//}

		//load the render window
	aCamera = vtkSmartPointer<vtkCamera>::New();
	aRenderer = vtkSmartPointer<vtkRenderer>::New();
	renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(aRenderer); 
	iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	/*aCamera->SetViewUp (0, 0, -1);
	aCamera->SetPosition (0, 1, 0);
	aCamera->SetFocalPoint (0, 0, 0);
	aCamera->ComputeViewPlaneNormal();*/

	aRenderer->SetActiveCamera(aCamera);
	aRenderer->ResetCamera ();
	//aCamera->Dolly(1.5);
	aRenderer->SetBackground(0.3,0.3,0.0);
	renWin->SetSize(640, 480);
	renWin->Render();
//	aRenderer->ResetCameraClippingRange ();

//	loadMRI();
//	ShowMRI();
//	showtemp(); //is ok if dialog not opening. not doing for load,show either...
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Cfull_registration_dialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cfull_registration_dialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);

	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cfull_registration_dialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cfull_registration_dialogDlg::OnBnClickedButtonloadxray()
{

	
	unregisteredxray.polydata_wrllandmarks =  vtkPolyData::New ();
	unregisteredxray.wrllandmark_points=vtkPoints::New();
	//unregisteredxray = Transform::read_landmarks_xray("D:/patient_data_testing/modele_complet/2052364/2052364/2052364.o3", unregisteredxray);
	unregisteredxray = Transform::read_landmarks_xray(xraylandmarks, unregisteredxray);

	polydatawrl = Transform::read (thefilexray);
	thexraypeds=Transform::read_spinal_marks_xray8(thefilexray2); //was read_spinal_marks_xray

	xraycheck.EnableWindow(TRUE);
	xraycheck.SetCheck(1);
	extlandcheck.EnableWindow(TRUE);
	extlandcheck.SetCheck(1);
	showxray();
	showxraylandmarks();
	showxrayvertlandmarks();
	refreshscreen();
}

void Cfull_registration_dialogDlg::refreshscreen()
{
	aRenderer->ResetCamera ();
	//aRenderer->SetBackground(1,1,1);
	renWin->Render();
	aRenderer->ResetCameraClippingRange ();
}

void Cfull_registration_dialogDlg::OnBnClickedButtonloadsurface()
{ 
		unregisteredtopo.polydata_szelandmarks =  vtkPolyData::New ();
	unregisteredtopo.szelandmark_points=vtkPoints::New();
	unregisteredtopo = Transform::read_landmarks_topo(topofile, unregisteredtopo);
	unregisteredtopo.polydatasze = Transform::read (topofile);
	
	topocheck.EnableWindow(TRUE);
	topocheck.SetCheck(1);
	extlandcheck.EnableWindow(TRUE);
	extlandcheck.SetCheck(1);

	this->showtopo();
	showtopolandmarks();
	refreshscreen();
}

void Cfull_registration_dialogDlg::OnBnClickedCancel()
{
	aRenderer->RemoveAllProps();
	//		unregisteredtopo.polydata_szelandmarks->Delete();
	//	unregisteredtopo.szelandmark_points->Delete();
	//aRenderer->Delete();
	//renWin->Delete();
	OnCancel();
}

void Cfull_registration_dialogDlg::showxray()
{
	vtkPolyDataMapper *polyDataMapper = vtkPolyDataMapper::New();
	polyDataMapper->SetInput(polydatawrl);
	wrlactor = vtkActor::New();
	wrlactor->GetProperty()->SetAmbientColor(1.0,0.0,0.0);
	wrlactor->GetProperty()->SetAmbient(1);
	//wrlactor->GetProperty()->SetOpacity(0.99);
	wrlactor->SetMapper( polyDataMapper );
	polyDataMapper->Delete();
	//aRenderer->AddActor(wrlactor);
	refreshscreen();
}

void Cfull_registration_dialogDlg::loadMRI()
{

	//get the name of the readme file containing spacing info and file info
	num_mris=0;
	slicetobereg=0;
	slicetoshow=slicetobereg;
	char temp_string_wrl[200];

/*	thefilexray = "D:/patient_data_testing/modele_complet/1734048/1734048/1734048.wrl";
	thefilexray2 = "D:/patient_data_testing/modele_complet/1734048/1734048/1734048.wrl";
	xraylandmarks = "D:/patient_data_testing/modele_complet/1734048/1734048/1734048.o3";
	topofile = "D:/patient_data_testing/modele_complet/1734048/topo/15-07-2010/Reconstruction/1734048.sze";
	
	strcpy( patientname, "2353729to1734048");
	strcpy( patientpath, "D:/patient_data_testing/modele_complet/2353729to1734048/");
	std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/2353729to1734048/2353729to1734048_readme.txt");*/
	
	
	thefilexray = "D:/patient_data_testing/modele_complet/1734048/1734048/1734048.wrl";
	thefilexray2 = "D:/patient_data_testing/modele_complet/1734048/1734048/1734048.wrl";
	xraylandmarks = "D:/patient_data_testing/modele_complet/1734048/1734048/1734048.o3";
	topofile = "D:/patient_data_testing/modele_complet/1734048/topo/15-07-2010/Reconstruction/1734048.sze";
	
	strcpy( patientname, "1734048");
	strcpy( patientpath, "D:/patient_data_testing/modele_complet/1734048/");
	std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/1734048/1734048_readme.txt");
	
	//2353729to2052364 so fit 2353729 MRI into 2052364 topo and X-ray
	/*thefilexray = "D:/patient_data_testing/modele_complet/2052364/2052364/2052364.wrl";
	thefilexray2 = "D:/patient_data_testing/modele_complet/2052364/2052364/2052364.wrl";
	xraylandmarks = "D:/patient_data_testing/modele_complet/2052364/2052364/2052364.o3";
	topofile = "D:/patient_data_testing/modele_complet/2052364/topo/14-07-2010/Reconstruction/2052364.sze";
	
	strcpy( patientname, "2052364");
	strcpy( patientpath, "D:/patient_data_testing/modele_complet/2052364/");
	std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/2353729to2052364/2353729to2052364_readme.txt");*/

/*	thefilexray = "D:/patient_data_testing/modele_complet/2353729/2353729/2353729.wrl";
	thefilexray2 = "D:/patient_data_testing/modele_complet/2353729/2353729/2353729.wrl";
	xraylandmarks = "D:/patient_data_testing/modele_complet/2353729/2353729/2353729.o3";
	topofile = "D:/patient_data_testing/modele_complet/2353729/topo/08-06-2010/Reconstruction/2353729_debout.sze";

	//std::ifstream numbers_str2( input_filenamedicom3d.c_str() ); 
	strcpy( patientname, "2353729");
	strcpy( patientpath, "D:/patient_data_testing/modele_complet/2353729/");
	std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/2353729/2353729_readme.txt"); */

	
	
/*	thefilexray = "D:/patient_data_testing/modele_complet/2052364/2052364/2052364.wrl";
	thefilexray2 = "D:/patient_data_testing/modele_complet/2052364/2052364/2052364.wrl";
	xraylandmarks = "D:/patient_data_testing/modele_complet/2052364/2052364/2052364.o3";
	topofile = "D:/patient_data_testing/modele_complet/2052364/topo/14-07-2010/Reconstruction/2052364.sze";
	
	strcpy( patientname, "2052364");
	strcpy( patientpath, "D:/patient_data_testing/modele_complet/2052364/");
	std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/2052364/2052364_readme.txt");*/

		xray2_vertindex=-2; //start at L5
	xray3_vertindex=-2; //start at L4
	xray4_vertindex=-2;

	char thefilemriland[200];
	char temp_string2[200];

	 if(!numbers_str2) { 
    exit(1);
   } 
	
	numbers_str2.seekg(0, ios::beg);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &num_mris);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemri1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange1[0], &mrirange1[1], &mri_intensity1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray2_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemri2);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange2[0], &mrirange2[1], &mri_intensity2);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert2);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray3_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemri3);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange3[0], &mrirange3[1], &mri_intensity3);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert3);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray4_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f",temp_string2, &mri_spacing[0], &mri_spacing[1], &mri_spacing[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemriland);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, reptxfile);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, o3txfile);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &t[0], &t[1], &t[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &ox[0], &ox[1], &ox[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &oy[0], &oy[1], &oy[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &t2[0], &t2[1], &t2[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &ox2[0], &ox2[1], &ox2[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &oy2[0], &oy2[1], &oy2[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &t3[0], &t3[1], &t3[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &ox3[0], &ox3[1], &ox3[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &oy3[0], &oy3[1], &oy3[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf",temp_string2, &mrimv);

	//now for the axial MRIS
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemri_axial1);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange_axial1[0], &mrirange_axial1[1], &mri_intensity_axial1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemri_axial2);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange_axial2[0], &mrirange_axial2[1], &mri_intensity_axial2);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f",temp_string2, &mri_spacing_axial[0], &mri_spacing_axial[1], &mri_spacing_axial[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &t_axial1[0], &t_axial1[1], &t_axial1[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &ox_axial1[0], &ox_axial1[1], &ox_axial1[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &oy_axial1[0], &oy_axial1[1], &oy_axial1[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &t_axial2[0], &t_axial2[1], &t_axial2[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &ox_axial2[0], &ox_axial2[1], &ox_axial2[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &oy_axial2[0], &oy_axial2[1], &oy_axial2[2]);

	double *P=new double[3];
	int lala;

	//read the mri landmark data
	themripeds=Transform::read_spinal_marks_mri_tomo(thefilemriland);
	vtkFloatArray *values22 = vtkFloatArray::New();
	vtkPoints *P_points =vtkPoints::New();
	P_points = themripeds.polydata_mrilandmarks->GetPoints();
	values22->DeepCopy(P_points->GetData());

	vtkPoints *newmrilandmark_pointspart[17];

	for(lala=0;lala<17;lala++)	 
		newmrilandmark_pointspart[lala] = vtkPoints::New();

	for(lala=0;lala<themripeds.numelem;lala++)	
	{
		values22->GetTuple(lala,P);		////mri data
		if (P[0]!=0) //then real point so store
		{
			int numvert=(int)(lala/8);
			newmrilandmark_pointspart[numvert]->InsertNextPoint(P[0],P[1],P[2]); //starts with T1
		}
	}

	//themripeds.polydata_mrilandmarks->SetPoints(newmrilandmark_points1);
	for(int lala=0;lala<17;lala++)
	{
		mrivert_regvol[lala]= vtkMatrix4x4::New(); //for the articulated registration of the MRI
		mri_newlandmarkspart[lala]=vtkPolyData::New();
		mri_newlandmarkspart[lala]->SetPoints(newmrilandmark_pointspart[lala]); //bad name but ok
	}

	polydatawrl2vert.polydata = new vtkPolyData*[20];
	polydatawrl3vert.polydata = new vtkPolyData*[20];
	polydatawrl4vert.polydata = new vtkPolyData*[20];


	polydatawrl2vert = Transform::read_wrlsep(thefilemrivert1);  //thefilemrivert1
	for(int gg=0;gg<polydatawrl2vert.num_vertebrae;gg++) 
			wrlactor2[gg]=vtkActor::New();

	if (num_mris>1)
	{
		polydatawrl3vert = Transform::read_wrlsep(thefilemrivert2);
		for(int gg=0;gg<polydatawrl3vert.num_vertebrae;gg++)
			wrlactor3[gg]=vtkActor::New();
	}
	if (num_mris>2)
	{
		polydatawrl4vert = Transform::read_wrlsep(thefilemrivert3);
		for(int gg=0;gg<polydatawrl4vert.num_vertebrae;gg++)
			wrlactor4[gg]=vtkActor::New();
	}

	cp[0]=ox_axial1[1]*oy_axial1[2]-ox_axial1[2]*oy_axial1[1];//a2b3-a3b2;
	cp[1]=ox_axial1[2]*oy_axial1[0]-ox_axial1[0]*oy_axial1[2];//a3b1-a1b3;
	cp[2]=ox_axial1[0]*oy_axial1[1]-ox_axial1[1]*oy_axial1[0];//a1b2-a2b1;

	cp2[0]=ox_axial2[1]*oy_axial2[2]-ox_axial2[2]*oy_axial2[1];//a2b3-a3b2;
	cp2[1]=ox_axial2[2]*oy_axial2[0]-ox_axial2[0]*oy_axial2[2];//a3b1-a1b3;
	cp2[2]=ox_axial2[0]*oy_axial2[1]-ox_axial2[1]*oy_axial2[0];//a1b2-a2b1;

	double the_vtkmatrixmri1[16]=
	{oy_axial1[0],oy_axial1[1],oy_axial1[2],0, 
	ox_axial1[0],ox_axial1[1],ox_axial1[2],0, 
	cp[0],cp[1],cp[2],0, 
	0,0,0,1};

	voloriginmri1 = vtkMatrix4x4::New();
	voloriginmri1->Identity();
	voloriginmri1->DeepCopy(the_vtkmatrixmri1);
	voloriginmri1->SetElement(0,3,t_axial1[0]); 
	voloriginmri1->SetElement(1,3,t_axial1[1]); 
	voloriginmri1->SetElement(2,3,t_axial1[2]);

	double the_vtkmatrixmri2[16]= {oy_axial2[0],oy_axial2[1],oy_axial2[2],0,
		ox_axial2[0],ox_axial2[1],ox_axial2[2],0,
		cp2[0],cp2[1],cp2[2],0,
		0,0,0,1}; 
	voloriginmri2 = vtkMatrix4x4::New();
	voloriginmri2->Identity();
	voloriginmri2->DeepCopy(the_vtkmatrixmri2);
	voloriginmri2->SetElement(0,3,t_axial2[0]); 
	voloriginmri2->SetElement(1,3,t_axial2[1]); 
	voloriginmri2->SetElement(2,3,t_axial2[2]); 

	mri_dimx=312; mri_dimy=448;
	mri_dimz = mrirange_axial1[1] - mrirange_axial1[0] +1;
	v18ax =  vtkSmartPointer<vtkVolume16Reader>::New();
	v18ax->SetDataDimensions (mri_dimx,mri_dimy);
	v18ax->SetImageRange(1,28);//(mrirange_axial1[0],mrirange_axial1[1]); //(1,50);// 
	v18ax->SetDataByteOrderToLittleEndian(); 
	v18ax->SetFilePrefix(thefilemri_axial1);
	v18ax->SetDataSpacing(1.1161,1.1161,12);//(mri_spacing_axial[0],mri_spacing_axial[1],mri_spacing_axial[2]);
	v18ax->Update();

	imageData_ax1 = vtkSmartPointer<vtkImageData>::New();
	imageData_ax1->SetScalarTypeToUnsignedChar();
	imageData_ax1->SetNumberOfScalarComponents(1);
	imageData_ax1->AllocateScalars();
	imageData_ax1->DeepCopy(v18ax->GetOutput());
	imageData_ax1->Update();
			
	voloriginmri1display = vtkMatrix4x4::New();
	voloriginmri1display->Identity();
	voloriginmri1display->DeepCopy(voloriginmri1);
	voloriginmri1display->Invert();

	//the following is for use with vtkimageActor. no inversion necessary in this case
	volmri1final=vtkMatrix4x4::New();
	volmri1final->Identity();
	volmri1final->DeepCopy(voloriginmri1);
  
	v19ax = vtkVolume16Reader::New();
	v19ax->SetDataDimensions(312,448);
	v19ax->SetImageRange (mrirange_axial2[0],mrirange_axial2[1]); 
	v19ax->SetDataByteOrderToLittleEndian(); 
	v19ax->SetFilePrefix (thefilemri_axial2); //this must be registered
	v19ax->SetDataSpacing(1.1161,1.1161,12);//(mri_spacing_axial[0],mri_spacing_axial[1],mri_spacing_axial[2]);
	v19ax->SetDataOrigin(0,0,0);
	v19ax->Update(); 
		
	imageData_ax2 = vtkImageData::New();
	imageData_ax2->SetScalarTypeToUnsignedChar();
	imageData_ax2->SetNumberOfScalarComponents(1);
	imageData_ax2->AllocateScalars();
	imageData_ax2->GetPointData()->CopyAllOn();
	imageData_ax2->GetPointData()->CopyScalarsOn();
	imageData_ax2->DeepCopy(v19ax->GetOutput());
	imageData_ax2->Update();	

	vtkMatrix4x4 *voloriginmri2display = vtkMatrix4x4::New();
	voloriginmri2display->DeepCopy(voloriginmri2);
	voloriginmri2display->Invert(); //invert for reslice
	vtkTransform *volumeoriginaxial2=vtkTransform::New();
	volumeoriginaxial2->SetMatrix(voloriginmri2display);
	volumeoriginaxial2->Update();

	vtkImageReslice *resliceax2temp2 = vtkImageReslice::New();
	resliceax2temp2->SetInput(imageData_ax2);
	resliceax2temp2->InterpolateOn();
	resliceax2temp2->SetInterpolationModeToCubic();
	resliceax2temp2->SetResliceTransform(volumeoriginaxial2);
	resliceax2temp2->SetOutputSpacing(imageData_ax2->GetSpacing());
	resliceax2temp2->AutoCropOutputOn();
	resliceax2temp2->GetInput()->Update();
	resliceax2temp2->Update();
	resliceax2temp2->GetOutput()->UpdateInformation();

	imageData_ax2->DeepCopy( resliceax2temp2->GetOutput());
	imageData_ax2->Update();

	bwLutaxial = vtkSmartPointer<vtkLookupTable>::New();
	bwLutaxial->SetTableRange(0, 700);
	bwLutaxial->SetSaturationRange(0,0);
	bwLutaxial->SetHueRange(0,0);
	bwLutaxial->SetValueRange(0,1);
	bwLutaxial->Build(); 

	axialImageActor = vtkSmartPointer<vtkImageActor>::New();
	registered = false;
	CSpinButtonCtrl *pSpin = reinterpret_cast<CSpinButtonCtrl *>(GetDlgItem(IDC_SPINslice)); 
	pSpin->UpdateData(TRUE);
	pSpin->SetRange32(0,mri_dimz);
	pSpin->SetPos32(slicetoshow);
	pSpin->UpdateData(TRUE);

	CEdit *Display;
	char tttt[4];
	sprintf(tttt,"%d",slicetoshow);
	Display = reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDITspin));
	CStringW tempme(tttt);
	LPCTSTR tt3 = (LPCTSTR)tempme;
	Display->SetWindowTextW(tt3);

	mricheck.EnableWindow(TRUE);
	vertlandcheck.EnableWindow(TRUE);
	vertlandcheck.SetCheck(1);
	mrivertcheck.EnableWindow(TRUE);
	mrivertcheck.SetCheck(1);
	mricheck.SetCheck(1);
	mrisagcheck.SetCheck(0);
	mrisagcheck.EnableWindow(TRUE);
	//ShowMRIsag();

	showMRIverts();
	showmrivertlandmarks();
	ShowMRI();
	refreshscreen();
	//aRenderer->Render();
}

void Cfull_registration_dialogDlg::showtemp()
{

		num_mris=0;
	char temp_string_wrl[200];
	xray2_vertindex=-2; //start at L5
	xray3_vertindex=-2; //start at L4
	xray4_vertindex=-2;

	char thefilemriland[200], thefilemrivert1[200], thefilemrivert2[200], thefilemrivert3[200];
	char temp_string2[200];

		//std::ifstream numbers_str2( input_filenamedicom3d.c_str() ); 
	strcpy( patientname, "2353729");
	std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/2353729/2353729_readme.txt"); 

	 if(!numbers_str2) { 
    exit(1);
   } 
	
	numbers_str2.seekg(0, ios::beg);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &num_mris);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemri1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange1[0], &mrirange1[1], &mri_intensity1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray2_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemri2);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange2[0], &mrirange2[1], &mri_intensity2);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert2);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray3_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemri3);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange3[0], &mrirange3[1], &mri_intensity3);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert3);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray4_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f",temp_string2, &mri_spacing[0], &mri_spacing[1], &mri_spacing[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemriland);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, reptxfile);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, o3txfile);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &t[0], &t[1], &t[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &ox[0], &ox[1], &ox[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &oy[0], &oy[1], &oy[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &t2[0], &t2[1], &t2[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &ox2[0], &ox2[1], &ox2[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &oy2[0], &oy2[1], &oy2[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &t3[0], &t3[1], &t3[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &ox3[0], &ox3[1], &ox3[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &oy3[0], &oy3[1], &oy3[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf",temp_string2, &mrimv);

	//now for the axial MRIS
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemri_axial1);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange_axial1[0], &mrirange_axial1[1], &mri_intensity_axial1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemri_axial2);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange_axial2[0], &mrirange_axial2[1], &mri_intensity_axial2);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f",temp_string2, &mri_spacing_axial[0], &mri_spacing_axial[1], &mri_spacing_axial[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &t_axial1[0], &t_axial1[1], &t_axial1[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &ox_axial1[0], &ox_axial1[1], &ox_axial1[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &oy_axial1[0], &oy_axial1[1], &oy_axial1[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &t_axial2[0], &t_axial2[1], &t_axial2[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &ox_axial2[0], &ox_axial2[1], &ox_axial2[2]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%lf%lf%lf",temp_string2, &oy_axial2[0], &oy_axial2[1], &oy_axial2[2]);


	double *P=new double[3];
	int lala;

	//read the mri landmark data
	themripeds=Transform::read_spinal_marks_mri_tomo(thefilemriland);
	vtkFloatArray *values22 = vtkFloatArray::New();
	vtkPoints *P_points =vtkPoints::New();
	P_points = themripeds.polydata_mrilandmarks->GetPoints();
	values22->DeepCopy(P_points->GetData());

	vtkPoints *newmrilandmark_pointspart[17];

	for(lala=0;lala<17;lala++)	 
		newmrilandmark_pointspart[lala] = vtkPoints::New();

	for(lala=0;lala<themripeds.numelem;lala++)	
	{
		values22->GetTuple(lala,P);		////mri data
		if (P[0]!=0) //then real point so store
		{
			int numvert=(int)(lala/8);
			newmrilandmark_pointspart[numvert]->InsertNextPoint(P[0],P[1],P[2]); //starts with T1
		}
	}

	//themripeds.polydata_mrilandmarks->SetPoints(newmrilandmark_points1);
	for(int lala=0;lala<17;lala++)
	{
		mrivert_regvol[lala]= vtkMatrix4x4::New(); //for the articulated registration of the MRI
		mri_newlandmarkspart[lala]=vtkPolyData::New();
		mri_newlandmarkspart[lala]->SetPoints(newmrilandmark_pointspart[lala]); //bad name but ok
	}

	polydatawrl2vert.polydata = new vtkPolyData*[20];
	polydatawrl3vert.polydata = new vtkPolyData*[20];
	polydatawrl4vert.polydata = new vtkPolyData*[20];


	polydatawrl2vert = Transform::read_wrlsep(thefilemrivert1);  //thefilemrivert1
	for(int gg=0;gg<polydatawrl2vert.num_vertebrae;gg++) 
			wrlactor2[gg]=vtkActor::New();

	if (num_mris>1)
	{
		polydatawrl3vert = Transform::read_wrlsep(thefilemrivert2);
		for(int gg=0;gg<polydatawrl3vert.num_vertebrae;gg++)
			wrlactor3[gg]=vtkActor::New();
	}
	if (num_mris>2)
	{
		polydatawrl4vert = Transform::read_wrlsep(thefilemrivert3);
		for(int gg=0;gg<polydatawrl4vert.num_vertebrae;gg++)
			wrlactor4[gg]=vtkActor::New();
	}


	mricheck.EnableWindow(TRUE);
	vertlandcheck.EnableWindow(TRUE);
	vertlandcheck.SetCheck(1);
	mrivertcheck.EnableWindow(TRUE);
	mrivertcheck.SetCheck(1);
	mricheck.SetCheck(1);
	mrisagcheck.SetCheck(0);
	mrisagcheck.EnableWindow(TRUE);
	//ShowMRIsag();


	cp[0]=ox_axial1[1]*oy_axial1[2]-ox_axial1[2]*oy_axial1[1];//a2b3-a3b2;
	cp[1]=ox_axial1[2]*oy_axial1[0]-ox_axial1[0]*oy_axial1[2];//a3b1-a1b3;
	cp[2]=ox_axial1[0]*oy_axial1[1]-ox_axial1[1]*oy_axial1[0];//a1b2-a2b1;

	cp2[0]=ox_axial2[1]*oy_axial2[2]-ox_axial2[2]*oy_axial2[1];//a2b3-a3b2;
	cp2[1]=ox_axial2[2]*oy_axial2[0]-ox_axial2[0]*oy_axial2[2];//a3b1-a1b3;
	cp2[2]=ox_axial2[0]*oy_axial2[1]-ox_axial2[1]*oy_axial2[0];//a1b2-a2b1;

	double the_vtkmatrixmri1[16]=
	{oy_axial1[0],oy_axial1[1],oy_axial1[2],0, 
	ox_axial1[0],ox_axial1[1],ox_axial1[2],0, 
	cp[0],cp[1],cp[2],0, 
	0,0,0,1};

	voloriginmri1 = vtkMatrix4x4::New();
	voloriginmri1->Identity();
	voloriginmri1->DeepCopy(the_vtkmatrixmri1);
	voloriginmri1->SetElement(0,3,t_axial1[0]); 
	voloriginmri1->SetElement(1,3,t_axial1[1]); 
	voloriginmri1->SetElement(2,3,t_axial1[2]);

	double the_vtkmatrixmri2[16]= {oy_axial2[0],oy_axial2[1],oy_axial2[2],0,
		ox_axial2[0],ox_axial2[1],ox_axial2[2],0,
		cp2[0],cp2[1],cp2[2],0,
		0,0,0,1}; 
	voloriginmri2 = vtkMatrix4x4::New();
	voloriginmri2->Identity();
	voloriginmri2->DeepCopy(the_vtkmatrixmri2);
	voloriginmri2->SetElement(0,3,t_axial2[0]); 
	voloriginmri2->SetElement(1,3,t_axial2[1]); 
	voloriginmri2->SetElement(2,3,t_axial2[2]-2*mri_spacing_axial[2]-(int)((float)mri_spacing_axial[2]/2)); //looking at vitamin E capsule

	mri_dimx=312; mri_dimy=448;
	mri_dimz = mrirange_axial1[1] - mrirange_axial1[0] +1;
	v18ax =  vtkSmartPointer<vtkVolume16Reader>::New();
	v18ax->SetDataDimensions (mri_dimx,mri_dimy);//(64, 64);//
	v18ax->SetImageRange(1,28);//(mrirange_axial1[0],mrirange_axial1[1]); //(1,50);// 
	v18ax->SetDataByteOrderToLittleEndian(); 
	v18ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2353729\\AX_T1_UPPER_0010\\raw\\2353729");
//(thefilemri_axial1);//("C:\\vtk561\\Data\\fullHead\\headsq");//
	v18ax->SetDataSpacing (1, 1, 12);//SetDataSpacing(mri_spacing_axial[0],mri_spacing_axial[1],mri_spacing_axial[2]);
	//v18ax->SetDataOrigin(0,0,0);
	v18ax->Update();

	  vtkSmartPointer<vtkVolume16Reader> v16 =
    vtkSmartPointer<vtkVolume16Reader>::New();
  v16->SetDataDimensions (mri_dimx,mri_dimy);
  v16->SetImageRange (1,28);
  v16->SetDataByteOrderToLittleEndian();
  v16->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2353729\\AX_T1_UPPER_0010\\raw\\2353729");
  v16->SetDataSpacing (1, 1, 12); //works with this
 // 	v16->SetDataSpacing(mri_spacing_axial[0],mri_spacing_axial[1],mri_spacing_axial[2]);
  v16->Update();

	//vtkSmartPointer<vtkImageData> 
	imageData_ax1 = vtkSmartPointer<vtkImageData>::New();
//	imageData_ax1 = vtkImageData::New();
	imageData_ax1->SetScalarTypeToUnsignedChar();
	imageData_ax1->SetNumberOfScalarComponents(1);
	imageData_ax1->AllocateScalars();
	imageData_ax1->GetPointData()->CopyAllOn();
	imageData_ax1->GetPointData()->CopyScalarsOn();
	imageData_ax1->DeepCopy(v18ax->GetOutput());
	imageData_ax1->Update();
		
	//this one original transformation obtained from the file 
	voloriginmri1display = vtkMatrix4x4::New();
	voloriginmri1display->DeepCopy(voloriginmri1);
	voloriginmri1display->Invert();
	vtkTransform *volumeoriginaxial1=vtkTransform::New();
	volumeoriginaxial1->SetMatrix(voloriginmri1display);
	volumeoriginaxial1->Update();
	
	  double center[3]; 
  int extent[6];
    double *origin = new double[3];
double *spacing = new double[3];
origin = imageData_ax1->GetOrigin();
spacing = imageData_ax1->GetSpacing();
  imageData_ax1->GetExtent(extent);
  center[0] = origin[0] + spacing[0] * 0.5 * (double)(extent[0] + extent[1]); 
  center[1] = origin[1] + spacing[1] * 0.5 * (double)(extent[2] + extent[3]); 
  center[2] = origin[2] + spacing[2] * 0.5 * (double)(extent[4] + extent[5]); 
  

vtkMatrix4x4 *tempmat2 = vtkMatrix4x4::New();

	
tempmat2->DeepCopy(voloriginmri1display);
tempmat2->Invert();
tempmat2->SetElement(0, 3, center[0]); 
  tempmat2->SetElement(1, 3,center[1]); 
  tempmat2->SetElement(2, 3,center[2] ); 

	vtkImageReslice *resliceax1 = vtkImageReslice::New();
	resliceax1->SetInput(imageData_ax1);
	resliceax1->InterpolateOn();
	resliceax1->SetInterpolationModeToCubic();
	resliceax1->SetResliceTransform(volumeoriginaxial1);
	//resliceax1->SetResliceAxes(tempmat2);
	resliceax1->AutoCropOutputOn();
	resliceax1->GetInput()->Update();
	resliceax1->Update();

	
//	imageData_ax1 = vtkImageData::New();
	//imageData_ax1->SetScalarTypeToFloat(); //>SetScalarTypeToShort();// 
/*	imageData_ax1->SetNumberOfScalarComponents(1);
	imageData_ax1->AllocateScalars();
	imageData_ax1->GetPointData()->CopyAllOn();
	imageData_ax1->GetPointData()->CopyScalarsOn();
	imageData_ax1->DeepCopy( resliceax1->GetOutput());
	imageData_ax1->Update();*/
	
	volmri1final=vtkMatrix4x4::New();
	volmri1final->DeepCopy(voloriginmri1display);

	v19ax = vtkVolume16Reader::New();
	v19ax->SetDataDimensions(312,448);
	v19ax->SetImageRange (mrirange_axial2[0],mrirange_axial2[1]); 
	v19ax->SetDataByteOrderToLittleEndian(); 
	v19ax->SetFilePrefix (thefilemri_axial2); //this must be registered
	v19ax->SetDataSpacing(mri_spacing_axial[0],mri_spacing_axial[1],mri_spacing_axial[2]);
	v19ax->SetDataOrigin(0,0,0);
	v19ax->Update(); 
		
		imageData_ax2 = vtkImageData::New();
	imageData_ax2->SetScalarTypeToUnsignedChar();
	imageData_ax2->SetNumberOfScalarComponents(1);
	imageData_ax2->AllocateScalars();
	imageData_ax2->GetPointData()->CopyAllOn();
	imageData_ax2->GetPointData()->CopyScalarsOn();
	imageData_ax2->DeepCopy(v19ax->GetOutput());
	imageData_ax2->Update();
		

	vtkMatrix4x4 *voloriginmri2display = vtkMatrix4x4::New();
	voloriginmri2display->DeepCopy(voloriginmri2);
	voloriginmri2display->Invert();
	vtkTransform *volumeoriginaxial2=vtkTransform::New();
	volumeoriginaxial2->SetMatrix(voloriginmri2display);
	volumeoriginaxial2->Update();

	bwLutaxial = vtkSmartPointer<vtkLookupTable>::New();
	bwLutaxial->SetTableRange(0, 700);
	bwLutaxial->SetSaturationRange(0,0);
	bwLutaxial->SetHueRange(0,0);
	bwLutaxial->SetValueRange(0,1);
	bwLutaxial->Build(); 




vtkImageReslice *resliceax1temp4 = vtkImageReslice::New();
		resliceax1temp4->SetInput(imageData_ax2);//(tempimage);//tempimage2//imageData_ax1
		resliceax1temp4->InterpolateOn();
		resliceax1temp4->SetInterpolationModeToCubic();
		resliceax1temp4->SetResliceTransform(volumeoriginaxial2);//temp
		//resliceax1temp4->SetResliceAxes(tempmat);//(volmriarticulated1[closest_vert]);//works without this but slice is parallel
		//resliceax1temp4->SetOutputExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],curr_slice,curr_slice);
	//	resliceax1temp4->SetOutputOrigin(imageData_ax2->GetOrigin()); //good
	//	resliceax1temp4->SetOutputSpacing(imageData_ax2->GetSpacing()); //good
		//resliceax1temp4->WrapOn(); //cuts without this
	//	resliceax1temp4->SetOutputDimensionality(2);//not helping
	//	resliceax1temp4->AutoCropOutputOn();
		resliceax1temp4->GetInput()->Update();
		resliceax1temp4->Update();
		resliceax1temp4->GetOutput()->UpdateInformation();

	imageData_ax2->DeepCopy( resliceax1temp4->GetOutput());
	imageData_ax2->Update();


	registered = false;


	// do all of the rendering that we do in initdialog, then refresh to test
/*   aRenderer =    vtkSmartPointer<vtkRenderer>::New();
   renWin =    vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(aRenderer);
   iren =    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);
    aCamera =    vtkSmartPointer<vtkCamera>::New();

		  aRenderer->SetActiveCamera(aCamera);
  aRenderer->ResetCamera ();
    aRenderer->SetBackground(.2, .3, .4);
  renWin->SetSize(640, 480);
  	renWin->Render();
	aRenderer->ResetCameraClippingRange ();*/

	//ShowMRI(); //problem is evidently in here

  //now read data and refresh renderer
 /* int mri_dimx=312; int mri_dimy=448;

  vtkSmartPointer<vtkVolume16Reader> v16 =
    vtkSmartPointer<vtkVolume16Reader>::New();
  v16->SetDataDimensions (mri_dimx,mri_dimy);
  v16->SetImageRange (1,28);
  v16->SetDataByteOrderToLittleEndian();
  v16->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2353729\\AX_T1_UPPER_0010\\raw\\2353729");
  v16->SetDataSpacing (1, 1, 12);
  v16->Update();

  	vtkSmartPointer<vtkImageData> imageData_ax1 = vtkSmartPointer<vtkImageData>::New();
	imageData_ax1->SetScalarTypeToUnsignedChar();
	imageData_ax1->SetNumberOfScalarComponents(1);
	imageData_ax1->AllocateScalars();
	//imageData_ax1->GetPointData() >GetPointData()->CopyAllOn();
	//imageData_ax1->GetPointData()->CopyScalarsOn();
	imageData_ax1->DeepCopy(v16->GetOutput());
	imageData_ax1->Update();*/

	int theextent[6];
	imageData_ax1->GetExtent(theextent);
	vtkSmartPointer<vtkLookupTable> bwLutaxial = vtkSmartPointer<vtkLookupTable>::New();
	bwLutaxial->SetTableRange(0, 700);
	bwLutaxial->SetSaturationRange(0,0);
	bwLutaxial->SetHueRange(0,0);
	bwLutaxial->SetValueRange(0,1);
	bwLutaxial->Build(); 

	   vtkSmartPointer<vtkImageMapToColors> saggitalColors = vtkSmartPointer<vtkImageMapToColors>::New();
	   saggitalColors->SetInput(imageData_ax1); //(v16->GetOutput()); //this works!!
//	  saggitalColors->SetInputConnection(v16->GetOutputPort());
    saggitalColors->SetLookupTable(bwLutaxial);
	saggitalColors->Update();

	  axialImageActor = vtkSmartPointer<vtkImageActor>::New();
	  axialImageActor->SetInput(saggitalColors->GetOutput());
	  axialImageActor->SetDisplayExtent(0,mri_dimx-1, 0,mri_dimy-1, 1,1);
	  aRenderer->AddActor(axialImageActor);
	aRenderer->Render();

	   aRenderer->ResetCamera ();
    aRenderer->ResetCameraClippingRange ();
		
  // Initialize the event loop and then start it.
  iren->Initialize();
  iren->Start();

}

void Cfull_registration_dialogDlg::showtopo()
{
	//vtkGeneralTransform *tlandmarks = vtkGeneralTransform::New ();
	//tlandmarks->PostMultiply();
	//tlandmarks->RotateZ(90.0); //okkkkk 
	//tlandmarks->RotateX(-90.0);
	//tlandmarks->Inverse();

	//clip

	// Define the clipping plane x = 2.5
	vtkPlane *plane = vtkPlane::New();
	plane->SetNormal(1, 0, 0);
	plane->SetOrigin(2.5, 0, 0);

	//plane->SetNormal(0, 1, 0);//good for viewing results
	//plane->SetOrigin(0, 100, 0);//(200, 0, 0);

	// Clip the data with GenerateClippedOutputOn
/*	vtkClipPolyData *clip1 = vtkClipPolyData::New();
	clip1->SetInput(unregisteredtopo.polydatasze);
	clip1->SetClipFunction(plane);
	clip1->GenerateClippedOutputOn();

	vtkPlane *plane2 = vtkPlane::New();
	plane2->SetNormal(0, 1, 0);
	plane2->SetOrigin(500, 250, 0);
	vtkClipPolyData *clip2 = vtkClipPolyData::New();
	clip2->SetInput(clip1->GetOutput());
	clip2->SetClipFunction(plane2);
	clip2->GenerateClippedOutputOn();*/

			//decimate a lot
	vtkDecimatePro *decimate = vtkDecimatePro::New();
	decimate->SetInput(unregisteredtopo.polydatasze);
  decimate->SetTargetReduction(.95); //10% reduction (if there was 100 triangles, now there will be 90)
  decimate->Update();


	  vtkExtractEdges *extract = vtkExtractEdges::New();
extract->SetInput(decimate->GetOutput());
vtkPolyData *meshes = extract->GetOutput();
polydataszeshow = extract->GetOutput();



	vtkPolyDataMapper *polyDataMapper = vtkPolyDataMapper::New();
	polyDataMapper->SetInput(polydataszeshow);
	szeactor = vtkActor::New();
	//szeactor->GetProperty()->SetAmbientColor(1.0,0.5,0.0);
	szeactor->GetProperty()->SetAmbientColor(0.0,0.0,1.0);
	szeactor->GetProperty()->SetAmbient(1);
	//szeactor->GetProperty()->SetOpacity(0.9);
	szeactor->SetMapper( polyDataMapper );
	szeactor->RotateZ(-90); 
	szeactor->RotateX(90); 
	polyDataMapper->Delete();
	aRenderer->AddActor(szeactor);
	refreshscreen();
}

void Cfull_registration_dialogDlg::showxraylandmarks()
{
	int lala;
	double *tempme = new double[3];
	vtkSphereSource *sphere = vtkSphereSource::New();
	sphere->SetRadius(5);
	vtkGlyph3D *glyph2 = vtkGlyph3D::New();
	glyph2->SetInput(unregisteredxray.polydata_wrllandmarks);
	glyph2->SetSource(sphere->GetOutput());
	glyph2->SetScaleModeToScaleByScalar();
	vtkPolyDataMapper *spikeMapper2 = vtkPolyDataMapper::New();
	spikeMapper2->SetInput(glyph2->GetOutput());
	spikeActorwrl = vtkActor::New();
	spikeActorwrl->SetMapper(spikeMapper2);
	//spikeActorwrl->GetProperty()->SetAmbient(0.8);
	spikeActorwrl->GetProperty()->SetColor(1.0,0.0,0.0);
	aRenderer->AddActor(spikeActorwrl);

	vtkFollower *textActor2[110];
	int numelem = unregisteredxray.polydata_wrllandmarks->GetNumberOfPoints();
	for(lala=0;lala<numelem;lala++)	 //text
		textActor2[lala] = vtkFollower::New();
	vtkFloatArray *values33 = vtkFloatArray::New();
	vtkPoints *Q_points =vtkPoints::New();
	Q_points =  unregisteredxray.polydata_wrllandmarks->GetPoints();
	values33->DeepCopy(Q_points->GetData());
	for(lala=0;lala<numelem;lala++)
	{
		vtkVectorText *XText2 = vtkVectorText::New(); 
		XText2->SetText(unregisteredxray.the_landmark[lala].c_str());
		vtkTextProperty *textStyle = vtkTextProperty::New();
		textStyle->BoldOn();
		textStyle->SetFontSize( 16 );
		textStyle->SetColor( 1, 1, 0 );
		textStyle->SetFontFamilyToArial();

		vtkTextMapper *titleText = vtkTextMapper::New();
		titleText->SetInput( unregisteredxray.the_landmark[lala].c_str() );
		titleText->SetTextProperty( textStyle );

		vtkPolyDataMapper *XTextMapper2 = vtkPolyDataMapper::New();
		XTextMapper2->SetInput(XText2->GetOutput());

		textActor2[lala]->SetMapper(XTextMapper2);
		textActor2[lala]->SetScale(10,10,10);
		int thevert=(int)((float)lala/8.0);
		int thepoint=lala%8;
		//	values22->DeepCopy(mri_newlandmarkspart[thevert]->GetPoints()->GetData());
		//values33->GetTuple(lala,tempme);

		values33->GetTuple(lala,tempme);
		textActor2[lala]->SetPosition(tempme[0]+0.25, tempme[1]+0.25, tempme[2]+0.25);
		textActor2[lala]->GetProperty()->SetColor(1, 0, 0);

		textActor2[lala]->RotateX(90.0);
		aRenderer->AddActor(textActor2[lala]);
	}
//	refreshscreen();
}

void Cfull_registration_dialogDlg::showtopolandmarks()
{
	int lala;
	double tempme[3];
	vtkSphereSource *sphere = vtkSphereSource::New();
	sphere->SetRadius(5);
	vtkGlyph3D *glyph2 = vtkGlyph3D::New();
	glyph2->SetInput(unregisteredtopo.polydata_szelandmarks);
	glyph2->SetSource(sphere->GetOutput());
	glyph2->SetScaleModeToScaleByScalar();
	vtkPolyDataMapper *spikeMapper2 = vtkPolyDataMapper::New();
	spikeMapper2->SetInput(glyph2->GetOutput());
	spikeActorsze = vtkActor::New();
	spikeActorsze->SetMapper(spikeMapper2);
	//spikeActorsze->GetProperty()->SetAmbient(0.8);
	spikeActorsze->GetProperty()->SetColor(0.0,0.0,1.0);
	aRenderer->AddActor(spikeActorsze);

	//show the names of the landmarks
	vtkFollower *textActor[200];
	int numelem = unregisteredtopo.polydata_szelandmarks->GetNumberOfPoints();
	vtkFloatArray *values22 = vtkFloatArray::New();
	vtkPoints *Q_points =vtkPoints::New();
	Q_points =  unregisteredtopo.polydata_szelandmarks->GetPoints();
	values22->DeepCopy(Q_points->GetData());

	for(lala=0;lala<numelem;lala++)	 //text, i dont need for now
		textActor[lala] = vtkFollower::New();
	for(lala=0;lala<numelem;lala++)
	{
		vtkVectorText *XText = vtkVectorText::New(); 
		XText->SetText(unregisteredtopo.the_landmark[lala].c_str());
		vtkPolyDataMapper *XTextMapper = vtkPolyDataMapper::New();
		XTextMapper->SetInput(XText->GetOutput());
		textActor[lala]->SetMapper(XTextMapper);
		textActor[lala]->SetScale(10.0,10.0,10.0);
		values22->GetTuple(lala,tempme);
		textActor[lala]->SetPosition(tempme[0]+0.5, tempme[1]+0.5, tempme[2]+0.5);
		textActor[lala]->GetProperty()->SetColor(0, 0, 1);
		textActor[lala]->RotateX(90.0);
		//if(tempme[0] != 0)
		aRenderer->AddActor(textActor[lala]);
	}


	refreshscreen();
}
void Cfull_registration_dialogDlg::OnBnClickedCheckxray()
{
	BOOL bvisiblexray = xraycheck.GetCheck();
	BOOL bvisibletopo = topocheck.GetCheck();
	BOOL bvisibleextland = extlandcheck.GetCheck();
	BOOL bvisiblevertland = vertlandcheck.GetCheck();

	if (bvisiblexray)
	{
		aRenderer->AddActor(wrlactor);
		if (bvisibleextland)
			showxraylandmarks();
		if (bvisiblevertland)
			showxrayvertlandmarks();
	}
	else
	{
		aRenderer->RemoveAllProps();
		//add the ones that were visible. hack, i know
		DisplayVisibleActors();

	}
	refreshscreen();
}

void Cfull_registration_dialogDlg::OnBnClickedChecktopo()
{
	BOOL bvisiblexray = xraycheck.GetCheck();
	BOOL bvisibletopo = topocheck.GetCheck();
	BOOL bvisibleextland = extlandcheck.GetCheck();
	BOOL bvisiblevertland = vertlandcheck.GetCheck();

	if (bvisibletopo)
	{
		showtopo();
		if (bvisibleextland)
			showtopolandmarks();
	}
	else
	{
		aRenderer->RemoveAllProps();
		DisplayVisibleActors();
	}
	refreshscreen();
}

void Cfull_registration_dialogDlg::OnBnClickedCheckextland()
{
	BOOL bvisiblexray = xraycheck.GetCheck();
	BOOL bvisibletopo = topocheck.GetCheck();
	BOOL bvisibleextland = extlandcheck.GetCheck();
	BOOL bvisiblevertland = vertlandcheck.GetCheck();

	/*if (bvisibleextland)
	{
		if (bvisibletopo)
			showtopolandmarks();
		if (bvisiblexray)
			showxraylandmarks();
	}
	else
	{
		aRenderer->RemoveAllProps();
		//add the ones that were visible. hack, i know
		DisplayVisibleActors();
	}*/
	refreshscreen();
}

//display both mri and xray vertebral landmarks if they are visible
void Cfull_registration_dialogDlg::OnBnClickedCheckvertland()
{
	BOOL bvisiblevertlands = vertlandcheck.GetCheck();
	BOOL bvisiblexray = xraycheck.GetCheck();
	BOOL bvisiblemri = mricheck.GetCheck();

	if (bvisiblevertlands) //then add the vertebral landmarks of the MRI and xray to the display
	{
		if(bvisiblexray)
			showxrayvertlandmarks();
		if(bvisiblemri)
			showmrivertlandmarks();
	}
	else //remove all and add visible elements
	{
		aRenderer->RemoveAllProps();// >RemoveActor(this->spikeActorwrlvert);
		DisplayVisibleActors();
	}
	refreshscreen();
}

void Cfull_registration_dialogDlg::DisplayVisibleActors()
{
	BOOL bvisiblexray = xraycheck.GetCheck();
	BOOL bvisiblemri = mricheck.GetCheck();
	BOOL bvisiblevertlands = vertlandcheck.GetCheck();
	BOOL bvisiblemrivert = mrivertcheck.GetCheck();
	BOOL bvisiblemrisag = mrisagcheck.GetCheck();
	BOOL bvisibletopo = topocheck.GetCheck();
	BOOL bvisibleextland = extlandcheck.GetCheck();

	if(bvisiblexray)
	{
	//	aRenderer->AddActor(wrlactor);
		if(bvisibleextland)
			showxraylandmarks();
		if(bvisiblevertlands)
			showxrayvertlandmarks();
	}
	if(bvisibletopo)
	{
		aRenderer->AddActor(szeactor);
		if(bvisibleextland)
			showtopolandmarks();
	}

	if(bvisiblemri)
	{	//add all MRI slices
		//ShowMRI();
		if(bvisiblevertlands)
			showmrivertlandmarks();
		if(bvisiblemrisag)
			ShowMRIsag();



		if(bvisiblemrivert)
		{
			showMRIverts();
			//add the vertebrae 
		/*	for(int gg=0;gg<polydatawrl2vert.num_vertebrae;gg++)
			{
				aRenderer->AddActor(wrlactor2[gg]);
			}
			if (num_mris>1)
			{
				for(int gg=0;gg<polydatawrl3vert.num_vertebrae;gg++)
				{
				aRenderer->AddActor(wrlactor3[gg]);
				}
			}
			if (num_mris>2)
			{
				for(int gg=0;gg<polydatawrl4vert.num_vertebrae;gg++)
				{
				aRenderer->AddActor(wrlactor4[gg]);
				}
			}*/
			
		}
	}
}


void Cfull_registration_dialogDlg::showxrayvertlandmarks()
{
	vtkSphereSource *sphere = vtkSphereSource::New();
	sphere->SetRadius(5);
	vtkGlyph3D *glyph2 = vtkGlyph3D::New();
	glyph2->SetInput(thexraypeds.polydata_wrllandmarks);
	glyph2->SetSource(sphere->GetOutput());
	glyph2->SetScaleModeToScaleByScalar();
	vtkPolyDataMapper *spikeMapper2 = vtkPolyDataMapper::New();
	spikeMapper2->SetInput(glyph2->GetOutput());
	spikeActorwrlvert = vtkActor::New();
	spikeActorwrlvert->SetMapper(spikeMapper2);
	//spikeActorwrlvert->GetProperty()->SetAmbient(0.8);
	spikeActorwrlvert->GetProperty()->SetColor(1.0,1.0,1.0);
	aRenderer->AddActor(spikeActorwrlvert);
}

void Cfull_registration_dialogDlg::showmrivertlandmarks()
{
	vtkSphereSource *sphere = vtkSphereSource::New();
	sphere->SetRadius(5);
	vtkGlyph3D *glyphped1[17]; 
	vtkPolyDataMapper *spikeMapper2[17];

	for(int lala=0;lala<17;lala++) //starts with T1
	{
		glyphped1[lala] = vtkGlyph3D::New();
		glyphped1[lala]->SetInput(mri_newlandmarkspart[lala]);
		glyphped1[lala]->SetSource(sphere->GetOutput());
		glyphped1[lala]->SetScaleModeToScaleByScalar();
		glyphped1[lala]->SetColorModeToColorByScalar ();
		glyphped1[lala]->Update();

		spikeMapper2[lala]= vtkPolyDataMapper::New();
		spikeMapper2[lala]->SetInput(glyphped1[lala]->GetOutput());

		spikeMapper2[lala]->Update();
		spikeActormri[lala] = vtkActor::New();
		spikeActormri[lala]->SetMapper(spikeMapper2[lala]);
		spikeActormri[lala]->GetProperty()->SetColor(0.0,0.0,0.0);
		aRenderer->AddActor(spikeActormri[lala]);
	}
}

void Cfull_registration_dialogDlg::OnBnClickedButtonrigid()
{

	rigidverts=1;
	int curr_slice = 3;
	vtkMatrix4x4 *volo3[17]; vtkMatrix4x4 *volrep[17]; 
	float themat[9], thetrans[3]={0,0,0};
	char temp_string_wrl[200];

	//generate slice by slice image data
	int imageextent[6];
	imageData_ax1->GetExtent(imageextent);

	//unregisteredtopo = Transform::RegisterTopoTPS (unregisteredtopo, unregisteredxray);

	//register the mri verts to the xray verts using the artisulated model
	//matrices already obtained using matlab

	//load xray tx, 1st is T1 i think
	std::ifstream numbers_strtxo3( o3txfile );
	numbers_strtxo3.seekg(0, ios::beg);

	imageData_ax1->GetExtent(imageextent);

	for(int i=0;i<17;i++)
	{
		volo3[i]= vtkMatrix4x4::New();
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volo3[i]->SetElement(0,3,thetrans[0]); 
		volo3[i]->SetElement(1,3,thetrans[1]); 
		volo3[i]->SetElement(2,3,thetrans[2]);
		volo3[i]->SetElement(3,3,1);
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);
		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volo3[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}

	}

	std::ifstream numbers_strtxrep( reptxfile );
	numbers_strtxrep.seekg(0, ios::beg);
	//load mri tx, 1st is T1 i think
	for(int i=0;i<17;i++)
	{
		volrep[i]= vtkMatrix4x4::New();
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volrep[i]->SetElement(0,3,thetrans[0]); 
		volrep[i]->SetElement(1,3,thetrans[1]); 
		volrep[i]->SetElement(2,3,thetrans[2]);
		volrep[i]->SetElement(3,3,1);
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);

		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volrep[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}
		volrep[i]->Invert();
	}

	for(int gg=0;gg<17;gg++)
	{
		mrivert_regvol[gg]->Identity(); //start it with L5
		mrivert_regvol[gg]->Multiply4x4(volo3[gg],volrep[gg],mrivert_regvol[gg]); //i guess starts with L5 
		//so vol03 o volrep 1st tx after
	}

	//1st register the topo to the xray using tps and external landmarks
	//vtkThinPlateSplineTransform *Transtps= vtkThinPlateSplineTransform::New ();
	/*vtkLandmarkTransform *Transrigid = vtkLandmarkTransform::New ();
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
	unregisteredtopo.polydatasze=f44->GetOutput();

	vtkTransformPolyDataFilter *ftps_sze = vtkTransformPolyDataFilter::New ();
	ftps_sze->SetInput (unregisteredtopo.polydatasze); //original
	ftps_sze->SetTransform (Transrigid);
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
	ftps_szemarks->SetTransform (Transrigid);
	ftps_szemarks->Update ();
	unregisteredtopo.polydata_szelandmarks=ftps_szemarks->GetOutput();
	unregisteredtopo.szelandmark_points = unregisteredtopo.polydata_szelandmarks->GetPoints();
	*/

	unregisteredtopo = Transform::RegisterTopoTPS (unregisteredtopo, unregisteredxray);
	/// mri
	vtkLandmarkTransform *Transrigidmri = vtkLandmarkTransform::New ();
	Transrigidmri->SetSourceLandmarks (themripeds.polydata_mrilandmarks->GetPoints());
	int thtnumpointtest = themripeds.polydata_mrilandmarks->GetPoints()->GetNumberOfPoints();
	Transrigidmri->SetTargetLandmarks (thexraypeds.polydata_wrllandmarks->GetPoints());
	int thtnumpointtest2 = thexraypeds.polydata_wrllandmarks->GetPoints()->GetNumberOfPoints();
	Transrigidmri->SetModeToRigidBody();
	Transrigidmri->Update ();

	vtkMatrix4x4 *rigidTransfoMatrix = vtkMatrix4x4::New();
	Transrigidmri->GetMatrix(rigidTransfoMatrix);


	//got here
	for(int lala=0;lala<17;lala++)
	{
		//mri_newlandmarkspart[lala]=vtkPolyData::New();
		vtkTransform *movevert=vtkTransform::New();
		movevert->SetMatrix(mrivert_regvol[0]);//[16-lala] 
		vtkTransformPolyDataFilter *f123 = vtkTransformPolyDataFilter::New ();
		f123->SetInput (mri_newlandmarkspart[lala]); //original
		f123->SetTransform (movevert);//is t1 for initial
		f123->Update ();
		mri_newlandmarkspart[lala]->DeepCopy(f123->GetOutput()); 
	}


	voloriginmri1->Multiply4x4(mrivert_regvol[0],voloriginmri1,voloriginmri1);
	voloriginmri2->Multiply4x4(mrivert_regvol[0],voloriginmri2,voloriginmri2);

	vtkMatrix4x4 *volmriarticulated1[17];
	vtkMatrix4x4 *volmriarticulated2[17];
	for (int thth = 0; thth <17;thth++)
	{
		volmriarticulated1[thth]= vtkMatrix4x4::New();
		volmriarticulated1[thth]->Identity();
		volmriarticulated1[thth]->Multiply4x4(mrivert_regvol[thth],volmriarticulated1[thth],volmriarticulated1[thth]); //just a starting point, will remove
		volmriarticulated1[thth]->Invert();

		volmriarticulated2[thth]= vtkMatrix4x4::New();
		volmriarticulated2[thth]->Identity();
		volmriarticulated2[thth]->DeepCopy(volmriarticulated1[thth]);
	}
	vtkTransform *txarticulatedaxial1=vtkTransform::New();
	txarticulatedaxial1->SetMatrix(volmriarticulated1[0]);
	txarticulatedaxial1->Update();

	vtkImageReslice *resliceax1 = vtkImageReslice::New();
	vtkImageData *imageData_ax1rigid = vtkImageData::New();
	resliceax1->SetInput(imageData_ax1);
	resliceax1->InterpolateOn();
	resliceax1->SetInterpolationModeToCubic();
	resliceax1->SetResliceTransform(txarticulatedaxial1);
	resliceax1->SetOutputSpacing(imageData_ax1->GetSpacing());
	resliceax1->AutoCropOutputOn();
	resliceax1->GetInput()->Update();
	resliceax1->Update();
	imageData_ax1->DeepCopy( resliceax1->GetOutput());
	imageData_ax1->Update();

	aRenderer->RemoveAllProps();// >RemoveActor(this->spikeActorwrlvert);
	showtopo();
	DisplayVisibleActors();
	imageData_ax1->GetExtent(imageextent);
		vtkImageReslice *resliceax1temp4 = vtkImageReslice::New();
		resliceax1temp4->SetInput(imageData_ax1);//(tempimage);//tempimage2//imageData_ax1
		resliceax1temp4->InterpolateOn();
		resliceax1temp4->SetInterpolationModeToCubic();
	//	resliceax1temp4->SetResliceTransform(txarticulatedaxial2temp);//temp
		//resliceax1temp4->SetResliceAxes(tempmat);//(volmriarticulated1[closest_vert]);//works without this but slice is parallel
	//	resliceax1temp4->SetOutputExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],curr_slice,curr_slice);
			resliceax1temp4->SetOutputExtent(126,305,139,390,curr_slice,curr_slice);
		resliceax1temp4->SetOutputOrigin(imageData_ax1->GetOrigin());//(imageData_ax1rigid->GetOrigin()); //good, but why??
		resliceax1temp4->SetOutputSpacing(imageData_ax1->GetSpacing()); //good
		//resliceax1temp4->WrapOn(); //cuts without this
	//	resliceax1temp4->SetOutputDimensionality(2);//not helping
		resliceax1temp4->AutoCropOutputOn();
		resliceax1temp4->GetInput()->Update();
		resliceax1temp4->Update();
		/*	imageData_ax1perslice[0]=vtkImageData::New();
			imageData_ax1perslice[1]=vtkImageData::New();
			imageData_ax1perslice[2]=vtkImageData::New();
		imageData_ax1perslice[curr_slice]=vtkImageData::New();
		imageData_ax1perslice[curr_slice]=resliceax1->GetOutput();//resliceax1temp4->GetOutput();//tempimage2;//resliceax1temp4->GetOutput();
		imageData_ax1perslice[curr_slice]->Update();	
		//imageData_ax1perslice[curr_slice]->GetOrigin(finalSliceOrigin);

		*/
		imageData_ax1 = resliceax1->GetOutput();
		imageData_ax1->Update();
		imageData_ax1 = resliceax1temp4->GetOutput();
		planeWidgetX6[0]=vtkImagePlaneWidget::New();
		planeWidgetX6[0]->DisplayTextOn();
		planeWidgetX6[0]->SetInput(imageData_ax1);//(imageData_ax1perslice[ijk]);
		//planeWidgetX6[ijk]->AdjustState();
		planeWidgetX6[0]->SetPlaceFactor(0.2);
		planeWidgetX6[0]->UserControlledLookupTableOn();
		planeWidgetX6[0]->SetLookupTable(bwLutaxial);
		planeWidgetX6[0]->SetPlaneOrientationToZAxes();
		planeWidgetX6[0]->SetPicker(picker);
		planeWidgetX6[0]->SetKeyPressActivationValue('x');
		vtkProperty *prop1 = planeWidgetX6[0]->GetPlaneProperty();
		prop1->SetColor(1, 0, 0);
		planeWidgetX6[0]->SetInteractor( renWin->GetInteractor() );

			planeWidgetX6[0]->On();
	refreshscreen();
}

void Cfull_registration_dialogDlg::ShowMRI()
{

	////to visualize a deformation grid: http://www.itk.org/pipermail/insight-users/2010-January/034880.html
	
	vtlaxial1tx=vtkTransform::New();
	vtlaxial1tx->SetMatrix(volmri1final);//);//();// this is good. removed for testing where points are
	vtlaxial1tx->Update();

	//visualize
	bwLutaxial = vtkLookupTable::New();
	bwLutaxial->SetTableRange(0, 1200);
	bwLutaxial->SetSaturationRange(0,0);
	bwLutaxial->SetHueRange(0,0);
	bwLutaxial->SetValueRange(0,1);
	bwLutaxial->Build(); 

	picker = vtkCellPicker::New();
	picker->SetTolerance(0.005);

	vtkSmartPointer<vtkImageMapToColors> saggitalColors = vtkSmartPointer<vtkImageMapToColors>::New();
	int testextent[6];
	if(registered == TRUE)
	{
			vtlaxial1tx->SetMatrix(volmri1final);//(rigidTransfoMatrix2);//
			vtlaxial1tx->Update();
			
			saggitalColors->SetInput(tempimage2); //(tempimage2);//
			saggitalColors->SetLookupTable(bwLutaxial);
			saggitalColors->Update();
			tempimage2->GetExtent(testextent);
	}

	else
	{
	saggitalColors->SetInput(imageData_ax1);
    saggitalColors->SetLookupTable(bwLutaxial);
	saggitalColors->Update();
	imageData_ax1->GetExtent(testextent);

	}
		

  axialImageActor->SetInput(saggitalColors->GetOutput()); 
 axialImageActor->SetUserTransform(vtlaxial1tx);//(vtlaxial1tx); //good, rigidTransfoMatrix2is rigid
  axialImageActor->SetDisplayExtent(90,testextent[1]-90, 52,testextent[3]-90, slicetoshow,slicetoshow);
  //65,testextent[3]-75
//65,testextent[3]-80 need forward a bit
  //65,testextent[3]-90 the back is tight and still need a bit forward
  // 75,testextent[3]-90: same!
	 aRenderer->AddActor(axialImageActor);

	planeWidgetZ4= vtkImagePlaneWidget::New();
	planeWidgetZ4->DisplayTextOn();
	planeWidgetZ4->SetInput(imageData_ax2);
	planeWidgetZ4->UserControlledLookupTableOn();
	planeWidgetZ4->SetLookupTable(bwLutaxial);
	planeWidgetZ4->SetPlaneOrientationToZAxes();
	planeWidgetZ4->SetSliceIndex(0);
	planeWidgetZ4->SetPicker(picker);
	planeWidgetZ4->SetKeyPressActivationValue ('c');
	vtkProperty *prop3 = planeWidgetZ4->GetPlaneProperty();
	prop3->SetColor(0, 0, 0);

	//good
	planeWidgetZ4->SetInteractor( renWin->GetInteractor() );
	planeWidgetZ4->On();
	
aRenderer->Render();
renWin->Render();

	//   aRenderer->ResetCamera ();
  //  aRenderer->ResetCameraClippingRange ();

	//refreshscreen();
	//ShowMRIsag();

  // Initialize the event loop and then start it.
 // iren->Initialize();
 // iren->Start();
  
	  //renWin->AddRenderer(aRenderer);
	//   aRenderer->SetBackground(.9, .9, .9);
 //  aRenderer->AddActor(axialImageActor);
 // aRenderer->ResetCameraClippingRange ();
  
 //  refreshscreen();
  // Initialize the event loop and then start it.
 // iren->Initialize();
 // iren->Start();
}

void Cfull_registration_dialogDlg::ShowMRIsag()
{
	double cp2[3],cp[3];
	cp[0]=ox[1]*oy[2]-ox[2]*oy[1];//a2b3-a3b2;
	cp[1]=ox[2]*oy[0]-ox[0]*oy[2];//a3b1-a1b3;
	cp[2]=ox[0]*oy[1]-ox[1]*oy[0];//a1b2-a2b1;

	cp2[0]=ox2[1]*oy2[2]-ox2[2]*oy2[1];//a2b3-a3b2;
	cp2[1]=ox2[2]*oy2[0]-ox2[0]*oy2[2];//a3b1-a1b3;
	cp2[2]=ox2[0]*oy2[1]-ox2[1]*oy2[0];//a1b2-a2b1;

	//t3[2]=t3[2]+mrimv;
	cp3[0]=ox3[1]*oy3[2]-ox3[2]*oy3[1];//a2b3-a3b2;
	cp3[1]=ox3[2]*oy3[0]-ox3[0]*oy3[2];//a3b1-a1b3;
	cp3[2]=ox3[0]*oy3[1]-ox3[1]*oy3[0];//a1b2-a2b1;



	double the_vtkmatrixyy[16]= {oy2[0],oy2[1],oy2[2],0, ox2[0],ox2[1],ox2[2],0, cp2[0],cp2[1],cp2[2],0, 0,0,0,1}; ///?problem?
	vtkMatrix4x4 *volorigin2 = vtkMatrix4x4::New();
	volorigin2->DeepCopy(the_vtkmatrixyy);
	volorigin2->SetElement(0,3,t2[0]); //not precise
	volorigin2->SetElement(1,3,t2[1]); //good
	volorigin2->SetElement(2,3,t2[2]);
	volorigin2->Invert();
	vtkTransform *volumeorigin2=vtkTransform::New();
	volumeorigin2->SetMatrix(volorigin2);

	double the_vtkmatrixs9[16]= {oy3[0],oy3[1],oy3[2],0, ox3[0],ox3[1],ox3[2],0, cp3[0],cp3[1],cp3[2],0, 0,0,0,1}; 
	vtkMatrix4x4 *volorigin3 = vtkMatrix4x4::New();
	volorigin3->DeepCopy(the_vtkmatrixs9);
	volorigin3->SetElement(0,3,t3[0]); 
	volorigin3->SetElement(1,3,t3[1]);
	volorigin3->SetElement(2,3,t3[2]);
	volorigin3->Invert();
	vtkTransform *volumeorigin3=vtkTransform::New();
	volumeorigin3->SetMatrix(volorigin3);

			double the_vtkmatrix2[16]= 
	{oy[0],oy[1],oy[2],0, 
	ox[0],ox[1],ox[2],0, 
	cp[0],cp[1],cp[2],0, 
	0,0,0,1};

	vtkMatrix4x4 *volorigin = vtkMatrix4x4::New();
	volorigin->DeepCopy(the_vtkmatrix2);
	//volorigin->Identity();
	volorigin->SetElement(0,3,t[0]); 
	volorigin->SetElement(1,3,t[1]); 
	volorigin->SetElement(2,3,t[2]);
	volorigin->Invert(); //inverting is good
	vtkTransform *volumeorigin=vtkTransform::New();
	volumeorigin->SetMatrix(volorigin);
	volumeorigin->Update();

	vtkVolume16Reader *v18 = vtkVolume16Reader::New();
	v18->SetDataDimensions(704,704);// (512,512);//
	v18->SetImageRange (mrirange1[0],mrirange1[1]); //old file
	v18->SetDataByteOrderToLittleEndian(); 
	v18->SetFilePrefix (thefilemri1);
	v18->SetDataSpacing(0.568, 0.568, 3.6);//(0.497,0.497,3.9);//(0.6836,0.6836,3.9);////(mri_spacing[0],mri_spacing[1],mri_spacing[2]);
	v18->SetDataOrigin(0,0,0);
	//v18->SetTransform(volumeorigin);
	v18->Update();

	vtkSmartPointer<vtkImageReslice> reslice3 = vtkSmartPointer<vtkImageReslice>::New();
	reslice3->SetInput(v18->GetOutput());
	reslice3->InterpolateOn();
	reslice3->SetInterpolationModeToCubic();
	reslice3->SetResliceTransform(volumeorigin);
	//reslice3->SetResliceAxes(volorigin);
	reslice3->AutoCropOutputOn();
	reslice3->GetInput()->Update();
	reslice3->Update();

	vtkSmartPointer<vtkImageData> imageData3 = vtkSmartPointer<vtkImageData>::New();;
	imageData3->SetScalarTypeToUnsignedChar();
	imageData3->SetNumberOfScalarComponents(1);
	imageData3->AllocateScalars();
	imageData3->GetPointData()->CopyAllOn();
	imageData3->GetPointData()->CopyScalarsOn();
	imageData3->DeepCopy(reslice3->GetOutput());//(v18->GetOutput());//

	
	//visualize
	vtkSmartPointer<vtkLookupTable> bwLut2 = vtkSmartPointer<vtkLookupTable>::New();
	bwLut2->SetTableRange(0, 500);
	bwLut2->SetSaturationRange(0,0);
	bwLut2->SetHueRange(0,0);
	bwLut2->SetValueRange(0,1);
	bwLut2->Build(); 

	vtkCellPicker *picker = vtkCellPicker::New();
	picker->SetTolerance(0.005);


		planeWidgetX = vtkImagePlaneWidget::New();
	planeWidgetX->DisplayTextOn();
	planeWidgetX->SetInput(imageData3);
	planeWidgetX->UserControlledLookupTableOn();
	planeWidgetX->SetLookupTable(bwLut2);
	planeWidgetX->SetPlaneOrientationToXAxes(); //default is y axis i think
	planeWidgetX->SetSliceIndex(0);
	planeWidgetX->SetPicker(picker);
	planeWidgetX->SetKeyPressActivationValue('d');
	vtkProperty *prop1 = planeWidgetX->GetPlaneProperty();
	prop1->SetColor(1, 0, 0);

	planeWidgetY= vtkImagePlaneWidget::New();
	planeWidgetY->DisplayTextOn();
	planeWidgetY->SetInput(imageData3); 
	planeWidgetY->UserControlledLookupTableOn();
	planeWidgetY->SetLookupTable(bwLut2);
	planeWidgetY->SetPlaneOrientationToYAxes();
	planeWidgetY->SetSliceIndex(0);
	planeWidgetY->SetPicker(picker);
	planeWidgetY->SetKeyPressActivationValue('e');
	vtkProperty *prop2 = planeWidgetY->GetPlaneProperty();
	prop2->SetColor(0, 1, 0);


	planeWidgetZ= vtkImagePlaneWidget::New();
	planeWidgetZ->DisplayTextOn();
	planeWidgetZ->SetInput(imageData3);
	planeWidgetZ->UserControlledLookupTableOn();
	planeWidgetZ->SetLookupTable(planeWidgetZ->GetLookupTable());
	planeWidgetZ->SetPlaneOrientationToZAxes();
	planeWidgetZ->SetSliceIndex(0);
	planeWidgetZ->SetPicker(picker);
	planeWidgetZ->SetKeyPressActivationValue ('f');
	vtkProperty *prop3 = planeWidgetZ->GetPlaneProperty();
	prop3->SetColor(0, 0, 1);

	planeWidgetX->SetInteractor( renWin->GetInteractor() );
	planeWidgetX->On();
	planeWidgetY->SetInteractor( renWin->GetInteractor() );
	planeWidgetY->On();
	planeWidgetZ->SetInteractor( renWin->GetInteractor() );
	planeWidgetZ->On();
	//add a non-orthogonal slice
	 vtkImageMapToColors *sagittalColors = vtkImageMapToColors::New();
	 sagittalColors->SetInput(imageData3);//(imageData3);
	sagittalColors->SetLookupTable(bwLut2);
	sagittalColors->Update();

	//this works
	int theextent[6];
	imageData3->GetExtent(theextent);
  vtkImageActor *sagittal =  vtkImageActor::New();
  sagittal->SetInput(sagittalColors->GetOutput());
  sagittal->SetUserTransform(volumeorigin);
  sagittal->SetDisplayExtent(theextent[0],theextent[1],theextent[2],theextent[3],0,0);
  //aRenderer->AddActor(sagittal);

	/*
  vtkImageActor *sagittal =  vtkImageActor::New();
  sagittal->SetInput(reslice3->GetOutput());
 // sagittal->SetUserTransform(volumeorigin);
  reslice3->GetOutput()->GetExtent(theextent);
  sagittal->SetDisplayExtent(theextent[0],theextent[1], theextent[2],theextent[3],0,0);
  aRenderer->AddActor(sagittal);
  */
  
  /*
	if (num_mris>1)
	{
		
		vtkVolume16Reader *v19 = vtkVolume16Reader::New();
		v19->SetDataDimensions (704,704);
		v19->SetImageRange (mrirange2[0],mrirange2[1]); 
		v19->SetDataByteOrderToLittleEndian(); 
		v19->SetFilePrefix (thefilemri2); //this must be registered
		v19->SetDataSpacing(mri_spacing[0],mri_spacing[1],mri_spacing[2]);
		v19->SetDataOrigin(0,0,0);
		v19->Update();

		vtkImageReslice *reslice4 = vtkImageReslice::New();
		reslice4->SetInput(v19->GetOutput());
		reslice4->SetResliceTransform(volumeorigin2);
		reslice4->InterpolateOn();
		reslice4->SetInterpolationModeToCubic();
		reslice4->AutoCropOutputOn();
		reslice4->GetInput()->Update();
		reslice4->Update();

		vtkImageData *imageData4 = vtkImageData::New();
		imageData4->SetScalarTypeToUnsignedChar();
		imageData4->SetNumberOfScalarComponents(1);
		imageData4->AllocateScalars();
		imageData4->GetPointData()->CopyAllOn();
		imageData4->GetPointData()->CopyScalarsOn();
		imageData4->DeepCopy( reslice4->GetOutput());

		planeWidgetX2 = vtkImagePlaneWidget::New();
		planeWidgetX2->DisplayTextOn();
		planeWidgetX2->SetInput(imageData4);
		planeWidgetX2->UserControlledLookupTableOn();
		planeWidgetX2->SetLookupTable(bwLut2);
		planeWidgetX2->SetPlaneOrientationToXAxes();
		planeWidgetX2->SetSliceIndex(10);
		planeWidgetX2->SetPicker(picker);
		planeWidgetX2->SetKeyPressActivationValue('g');
		vtkProperty *prop4 = planeWidgetX2->GetPlaneProperty();
		prop4->SetColor(1, 1, 0);

		//middle mouse button to slice through
		planeWidgetY2= vtkImagePlaneWidget::New();
		planeWidgetY2->DisplayTextOn();
		planeWidgetY2->SetInput(imageData4); 
		planeWidgetY2->UserControlledLookupTableOn();
		planeWidgetY2->SetLookupTable(bwLut2);
		planeWidgetY2->SetPlaneOrientationToYAxes();
		planeWidgetY2->SetSliceIndex(10);
		planeWidgetY2->SetPicker(picker);
		planeWidgetY2->SetKeyPressActivationValue('h');
		vtkProperty *prop5 = planeWidgetY2->GetPlaneProperty();
		prop5->SetColor(1, 1, 1);


		planeWidgetZ2= vtkImagePlaneWidget::New();
		planeWidgetZ2->DisplayTextOn();
		planeWidgetZ2->SetInput(imageData4);
		planeWidgetZ2->UserControlledLookupTableOn();
		planeWidgetZ2->SetLookupTable(planeWidgetZ->GetLookupTable());
		planeWidgetZ2->SetPlaneOrientationToZAxes();
		planeWidgetZ2->SetSliceIndex(10);
		planeWidgetZ2->SetPicker(picker);
		planeWidgetZ2->SetKeyPressActivationValue ('i');
		vtkProperty *prop6 = planeWidgetZ2->GetPlaneProperty();
		prop6->SetColor(1, 0, 1);

		planeWidgetX2->SetInteractor( renWin->GetInteractor() );
		planeWidgetX2->On();
		planeWidgetY2->SetInteractor( renWin->GetInteractor() );
		planeWidgetY2->On();
	}
	*/
	if (num_mris>2)
	{
		vtkVolume16Reader *v20 = vtkVolume16Reader::New();
		v20->SetDataDimensions (704,704);
		v20->SetImageRange (mrirange3[0],mrirange3[1]); 
		v20->SetDataByteOrderToLittleEndian(); 
		v20->SetFilePrefix (thefilemri3); //this must be registered
		v20->SetDataSpacing(mri_spacing[0],mri_spacing[1],mri_spacing[2]);
		v20->SetDataOrigin(0,0,0);
		v20->Update();

		vtkImageReslice *reslice5 = vtkImageReslice::New();
		reslice5->SetInput(v20->GetOutput());
		reslice5->SetResliceTransform(volumeorigin3);
		reslice5->InterpolateOn();
		reslice5->SetInterpolationModeToCubic();
		reslice5->AutoCropOutputOn();
		reslice5->GetInput()->Update();
		reslice5->Update();

		vtkImageData *imageData5 = vtkImageData::New();
		imageData5->SetScalarTypeToUnsignedChar();
		imageData5->SetNumberOfScalarComponents(1);
		imageData5->AllocateScalars();
		imageData5->GetPointData()->CopyAllOn();
		imageData5->GetPointData()->CopyScalarsOn();
		imageData5->DeepCopy( reslice5->GetOutput());

		planeWidgetX3 = vtkImagePlaneWidget::New();
		planeWidgetX3->DisplayTextOn();
		planeWidgetX3->SetInput(imageData5);
		planeWidgetX3->UserControlledLookupTableOn();
		planeWidgetX3->SetLookupTable(bwLut2);
		planeWidgetX3->SetPlaneOrientationToXAxes();
		planeWidgetX3->SetSliceIndex(10);
		planeWidgetX3->SetPicker(picker);
		planeWidgetX3->SetKeyPressActivationValue('x');
		vtkProperty *prop7 = planeWidgetX3->GetPlaneProperty();
		prop7->SetColor(0, 0, 1);

		//middle mouse button to slice through

		planeWidgetY3= vtkImagePlaneWidget::New();
		planeWidgetY3->DisplayTextOn();
		planeWidgetY3->SetInput(imageData5); 
		planeWidgetY3->UserControlledLookupTableOn();
		planeWidgetY3->SetLookupTable(bwLut2);
		planeWidgetY3->SetPlaneOrientationToYAxes();
		planeWidgetY3->SetSliceIndex(10);
		planeWidgetY3->SetPicker(picker);
		planeWidgetY3->SetKeyPressActivationValue('y');
		vtkProperty *prop8 = planeWidgetY3->GetPlaneProperty();
		prop8->SetColor(0, 0, 0);


		planeWidgetZ3= vtkImagePlaneWidget::New();
		planeWidgetZ3->DisplayTextOn();
		planeWidgetZ3->SetInput(imageData5);
		planeWidgetZ3->UserControlledLookupTableOn();
		planeWidgetZ3->SetLookupTable(planeWidgetZ->GetLookupTable());
		planeWidgetZ3->SetPlaneOrientationToZAxes();
		planeWidgetZ3->SetSliceIndex(10);
		planeWidgetZ3->SetPicker(picker);
		planeWidgetZ3->SetKeyPressActivationValue ('z');
		vtkProperty *prop9 = planeWidgetZ3->GetPlaneProperty();
		prop9->SetColor(0, 1, 1);

		//planeWidgetX3->SetInteractor( renWin->GetInteractor() );
		//planeWidgetX3->On();
		//planeWidgetY3->SetInteractor( renWin->GetInteractor() );
		//planeWidgetY3->On();
		//planeWidgetZ3->SetInteractor( renWin->GetInteractor() );
		//planeWidgetZ3->On();
	}
}

void Cfull_registration_dialogDlg::showMRIverts()
{
	vtkTransform *rotate_sagt = vtkTransform::New ();
	rotate_sagt->PostMultiply();
	rotate_sagt->Translate(0,0,mrimv);

	for(int gg=0;gg<polydatawrl2vert.num_vertebrae;gg++)
	{
		//define the transformation as a concatenation of (rep-1)*o3
		vtkPolyDataMapper *polyDataMapper2 = vtkPolyDataMapper::New();
		polyDataMapper2->SetInput(polydatawrl2vert.polydata[gg]);
		//wrlactor2[gg]=vtkActor::New();
		wrlactor2[gg]->SetMapper( polyDataMapper2 );
		//wrlactor2[gg]->GetProperty()->SetAmbientColor(0.1,(float)(gg)/polydatawrl2vert.num_vertebrae,0.1);
		wrlactor2[gg]->GetProperty()->SetAmbientColor(1.0,0.0,0.0);
		wrlactor2[gg]->GetProperty()->SetAmbient(1);
		wrlactor2[gg]->GetProperty()->SetOpacity(1.0);
		if (rigidverts == 1) 
			wrlactor2[gg]->SetUserMatrix(mrivert_regvol[0]); 
		else if (rigidverts == 0) 
			wrlactor2[gg]->SetUserMatrix(mrivert_regvol[gg+xray2_vertindex]); 
		polyDataMapper2->Delete();
		aRenderer->AddActor(wrlactor2[gg]);

	}

	if (num_mris>1)
	{for(int gg=0;gg<(polydatawrl3vert.num_vertebrae);gg++)
	{
		vtkPolyDataMapper *polyDataMapper3 = vtkPolyDataMapper::New();
		polyDataMapper3->SetInput(polydatawrl3vert.polydata[gg]);
	//	wrlactor3[gg]= vtkActor::New();
		wrlactor3[gg]->SetMapper( polyDataMapper3);
		wrlactor3[gg]->GetProperty()->SetAmbientColor(1.0,0.0,0.0);
		wrlactor3[gg]->GetProperty()->SetAmbient(1);
		wrlactor3[gg]->GetProperty()->SetOpacity(1.0);			

		if (rigidverts == 1) 
			wrlactor3[gg]->SetUserMatrix(mrivert_regvol[0]); 	
		else if (rigidverts == 0) 
			wrlactor3[gg]->SetUserMatrix(mrivert_regvol[gg+xray3_vertindex]); 

		aRenderer->AddActor(wrlactor3[gg]);
		polyDataMapper3->Delete();
	}
	}

	if (num_mris>2)
	{
		for(int gg=0;gg<polydatawrl4vert.num_vertebrae;gg++)
		{
			vtkTransformPolyDataFilter *f112 = vtkTransformPolyDataFilter::New ();
			f112->SetInput (polydatawrl4vert.polydata[gg]); 
			f112->SetTransform (rotate_sagt);
			f112->Update ();
			polydatawrl4vert.polydata[gg]=f112->GetOutput();
			int killme=gg+xray4_vertindex;
			vtkPolyDataMapper *polyDataMapper4 = vtkPolyDataMapper::New();
			polyDataMapper4->SetInput(polydatawrl4vert.polydata[gg]);
			//wrlactor4[gg]= vtkActor::New();
			wrlactor4[gg]->SetMapper( polyDataMapper4);
			//wrlactor4[gg]->GetProperty()->SetAmbientColor(1.0,0.0,(float)(gg)/polydatawrl4vert.num_vertebrae);
			wrlactor4[gg]->GetProperty()->SetAmbientColor(1.0,0.0,0.0);
			wrlactor4[gg]->GetProperty()->SetAmbient(1);
			//wrlactor4[gg]->GetProperty()->SetOpacity(0.4);

			if (rigidverts == 1) 
				wrlactor4[gg]->SetUserMatrix(mrivert_regvol[0]); 	
			else if (rigidverts == 0) 
				wrlactor4[gg]->SetUserMatrix(mrivert_regvol[gg+xray4_vertindex]); 

			polyDataMapper4->Delete();
			aRenderer->AddActor(wrlactor4[gg]);
		}
	}
}

void Cfull_registration_dialogDlg::OnBnClickedButtonmriload()
{

	loadMRI();
	//	showMRIverts();
	//showmrivertlandmarks();
	ShowMRI();

	
}

void Cfull_registration_dialogDlg::OnBnClickedButtonarticulated()
{
	int base_slice=0;
	vtkMatrix4x4 *volo3[17]; vtkMatrix4x4 *volrep[17];vtkMatrix4x4 *vollocaldef[17]; vtkMatrix4x4 *vollocaldeftrans[17];
	float themat[9], thetrans[3]={0,0,0};
	char temp_string_wrl[200];

	//generate slice by slice image data
	int imageextent[6];
	imageData_ax1->GetExtent(imageextent);

	unregisteredtopo = Transform::RegisterTopoTPS (unregisteredtopo, unregisteredxray);

	//register the mri verts to the xray verts using the artisulated model
	//matrices already obtained using matlab

	//load xray tx, 1st is T1 i think
	std::ifstream numbers_strtxo3( o3txfile );
	numbers_strtxo3.seekg(0, ios::beg);

	imageData_ax1->GetExtent(imageextent);

	for(int i=0;i<17;i++)
	{
		volo3[i]= vtkMatrix4x4::New();
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volo3[i]->SetElement(0,3,thetrans[0]); 
		volo3[i]->SetElement(1,3,thetrans[1]); 
		volo3[i]->SetElement(2,3,thetrans[2]);
		volo3[i]->SetElement(3,3,1);
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);
		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volo3[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}

	}

	std::ifstream numbers_strtxrep( reptxfile );
	numbers_strtxrep.seekg(0, ios::beg);
	//load mri tx, 1st is T1 i think
	for(int i=0;i<17;i++)
	{
		volrep[i]= vtkMatrix4x4::New();
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volrep[i]->SetElement(0,3,thetrans[0]); 
		volrep[i]->SetElement(1,3,thetrans[1]); 
		volrep[i]->SetElement(2,3,thetrans[2]);
		volrep[i]->SetElement(3,3,1);
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);

		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volrep[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}
		volrep[i]->Invert();
	}

	for(int gg=0;gg<17;gg++)
	{
		mrivert_regvol[gg]->Identity(); //start it with L5
		mrivert_regvol[gg]->Multiply4x4(volo3[0],volrep[0],mrivert_regvol[gg]); //i guess starts with L5 
		//so vol03 o volrep 1st tx after
	}


	for(int lala=0;lala<17;lala++)
	{
		vtkTransform *movevert=vtkTransform::New();
		movevert->SetMatrix(mrivert_regvol[base_slice]);//[16-lala]);
		vtkTransformPolyDataFilter *f123 = vtkTransformPolyDataFilter::New ();
		f123->SetInput (mri_newlandmarkspart[lala]); 
		f123->SetTransform (movevert);
		f123->Update ();
		mri_newlandmarkspart[lala]->DeepCopy(f123->GetOutput()); 
	}

	vtkMatrix4x4 *volmriarticulated1[17];
	for (int thth = 0; thth <17;thth++)
	{
		volmriarticulated1[thth]= vtkMatrix4x4::New();
		volmriarticulated1[thth]->Identity();
		//	volmriarticulated1[thth]->SetElement(0,3,mrivert_regvol[16-thth]->GetElement(0,3)); 
		//volmriarticulated1[thth]->SetElement(1,3,mrivert_regvol[16-thth]->GetElement(1,3)); 
		//volmriarticulated1[thth]->SetElement(2,3,mrivert_regvol[16-thth]->GetElement(2,3));

		volmriarticulated1[thth]->Multiply4x4(mrivert_regvol[thth],volmriarticulated1[thth],volmriarticulated1[thth]); //just a starting point, will remove
		volmriarticulated1[thth]->Invert();
	}
	vtkTransform *txarticulatedaxial1=vtkTransform::New();
	txarticulatedaxial1->SetMatrix(volmriarticulated1[base_slice]);
	txarticulatedaxial1->Update();

	//imageData_ax1->GetExtent(imageextent); //extent = 0,447,0,311,0,26


	//at this point, the x and y values have been flipped due to the while loading
	vtkImageReslice *resliceax1 = vtkImageReslice::New();
	vtkImageData *imageData_ax1base = vtkImageData::New();
	resliceax1->SetInput(imageData_ax1);
	resliceax1->InterpolateOn();
	resliceax1->SetInterpolationModeToCubic();
	resliceax1->SetResliceTransform(txarticulatedaxial1);
	resliceax1->SetOutputSpacing(imageData_ax1->GetSpacing());
	resliceax1->AutoCropOutputOn();
	resliceax1->GetInput()->Update();
	resliceax1->Update();
	imageData_ax1base->DeepCopy( resliceax1->GetOutput());
	imageData_ax1base->Update();


	imageData_ax1->GetExtent(imageextent);

	vtkShortArray *scalars = vtkShortArray::New();
	vtkFloatArray *scalarsdeformation = vtkFloatArray::New();
	scalarsdeformation->SetNumberOfComponents(3);

	int newmri_dimz=imageextent[5];//(int) mri_dimz;///2,/4,/4
	int newmri_dimy=imageextent[3];//(int) mri_dimy;
	int newmri_dimx=imageextent[1];//(int) mri_dimx;
	for (int i=0;i<newmri_dimz;i++)
	{
		for (int j=0;j<newmri_dimy;j++)
		{
			for (int k=0;k<newmri_dimx;k++)
			{
				scalars->InsertNextTuple1(0);
				scalarsdeformation->InsertNextTuple3(0,0,0);
			}}}


	//this is where we calculate the non rigid part of the deformation

	//find the inverse of mrivert_regvol[0], the overall tx are L5 to T1
	//but volo3 is T1 to L5
	vtkMatrix4x4 *invGlobal = vtkMatrix4x4::New();
	invGlobal->DeepCopy(volmriarticulated1[base_slice]);//16 is T1
	invGlobal->Invert(); //needed
	///find the transformation of MRI to Xray without the global T0 transformation Tvi = Tvi o ((Tv1)^-1 o Tv1)
	//thus each vertebral tx is: Tvi o ((Tv1)^-1).  apply (Tv1)^-1 to each ready tx

	//overall transformation is :voloriginmri1display followed by volmriarticulated1
	for(int gg=0;gg<17;gg++)
	{
		vollocaldef[gg]= vtkMatrix4x4::New();
		vollocaldef[gg]->Identity(); //
		//vollocaldef[gg]->Multiply4x4(volmriarticuxcsaxdszslated1[gg],invGlobal,vollocaldef[gg]);//crap
		vollocaldef[gg]->Multiply4x4(volmriarticulated1[gg],mrivert_regvol[base_slice],vollocaldef[gg]); //0 being L5
		//vollocaldef[gg]->Multiply4x4(mrivert_regvol[gg],invGlobal,vollocaldef[gg]);
		vollocaldeftrans[gg]= vtkMatrix4x4::New();
		vollocaldeftrans[gg]->Identity();

		vollocaldeftrans[gg]= vtkMatrix4x4::New();
		vollocaldeftrans[gg]->Identity();
		vollocaldeftrans[gg]->SetElement(0,0,vollocaldef[gg]->GetElement(0,0)); 
		vollocaldeftrans[gg]->SetElement(0,1,vollocaldef[gg]->GetElement(0,1)); 
		vollocaldeftrans[gg]->SetElement(1,0,vollocaldef[gg]->GetElement(1,0)); 
		vollocaldeftrans[gg]->SetElement(1,1,vollocaldef[gg]->GetElement(1,1)); 

		vollocaldeftrans[gg]->SetElement(0,3,vollocaldef[gg]->GetElement(0,3)); 
		vollocaldeftrans[gg]->SetElement(1,3,vollocaldef[gg]->GetElement(1,3)); 
		vollocaldeftrans[gg]->SetElement(2,3,vollocaldef[gg]->GetElement(2,3));

	}

	//apply only the translation

	double mri1origin[3];
	imageData_ax1->GetOrigin(mri1origin);
	float current_voxel_location[3];
	scalars = (vtkShortArray *)(imageData_ax1->GetPointData()->GetScalars());

	double imagespacing[3];
	imageData_ax1->GetSpacing(imagespacing);

	int subID; 
	double *paraCoords = new double[3]; 
	double *xyzCoords  = new double[3]; 
	double *vtkcellweights = new double[4]; 

	double *Coordstopmiddle  = new double[3]; 
	double *Coordsbottommiddle  = new double[3]; 
	//find the linear tx T_{v-MRI-Xray} for each voxel
	int numnonzeros=0; //slice 17, closest to L2

	//find the top and bottom central points
	int middlez = (int)(newmri_dimz/2);
	int middley = (int)(newmri_dimy/2);
	int middlex = (int)(newmri_dimx/2);
	int offset11 = (int)((newmri_dimy*newmri_dimx*(newmri_dimz-1))+newmri_dimx*middley+middlex);
	subID = imageData_ax1->GetCell( offset11 )->GetParametricCenter(paraCoords); 
	int &subIDadd2 = subID; 
	imageData_ax1->GetCell(offset11)->EvaluateLocation(subIDadd2, paraCoords, Coordstopmiddle, vtkcellweights); 

	offset11 = (int)((newmri_dimy*newmri_dimx*0)+newmri_dimx*middley+middlex);
	subID = imageData_ax1->GetCell( offset11 )->GetParametricCenter(paraCoords); 
	int &subIDadd3 = subID; 
	imageData_ax1->GetCell(offset11)->EvaluateLocation(subIDadd3, paraCoords, Coordsbottommiddle, vtkcellweights); 

	numimages = newmri_dimz;
	//vtkImageData **imageData_ax1perslice = new vtkImageData*[newmri_dimz];
	imageData_ax1perslice = new vtkImageData*[newmri_dimz];
	vtkImageData *imageData_ax1perslice2 = vtkImageData::New();

	for (int i=0;i<newmri_dimz;i++) //actual right dimensions taken from imagedata: newmri_dimz
	{


		int closest_vert=1, second_vert=0; //closest is 13, second is 14
		float closest_dist2=10000000000000, second_dist2=10000000000000;

		offset11 = (int)((newmri_dimy*newmri_dimx*i)+newmri_dimx*middley+middlex);
		subID = imageData_ax1base->GetCell( offset11 )->GetParametricCenter(paraCoords); //imageData_ax1base is after rigid registration 
		int &subIDadd3 = subID; 
		imageData_ax1base->GetCell(offset11)->EvaluateLocation(subIDadd2, paraCoords, xyzCoords, vtkcellweights); 

		current_voxel_location[2]=xyzCoords[2];//(float)i*imagespacing[2]+mri1origin[2];//not nesseraciry it has an orientation...
		for(int hhh=0;hhh<17;hhh++) //starting at L5
		{
			//float tempdist=sqrt(pow((current_voxel_location[0]-volo3[16-hhh]->GetElement(0,3)),2)+pow((current_voxel_location[1]-volo3[16-hhh]->GetElement(1,3)),2)+pow((current_voxel_location[2]-volo3[16-hhh]->GetElement(2,3)),2));
			float tempdist=abs(xyzCoords[2]-volo3[hhh]->GetElement(2,3));//was 16-hhh
			if(tempdist<closest_dist2)
			{
				second_vert=closest_vert;
				second_dist2=closest_dist2;
				closest_vert=hhh; //13 = L2, 17 being L5, that makes no sense sine=ce overall 0 is L5
				closest_dist2=tempdist;
			}
			else if(tempdist<second_dist2)
			{
				second_vert=hhh;
				second_dist2=tempdist;
			}
		}


		imageData_ax1->GetExtent(imageextent);
		//add this to the transformation: voloriginmri1
		vtkTransform *txarticulatedaxial1temp=vtkTransform::New();
		//txarticulatedaxial1temp->SetMatrix(volmriarticulated1[closest_vert]);///this is good!!!!!!
		txarticulatedaxial1temp->SetMatrix(volmriarticulated1[0]);
		txarticulatedaxial1temp->Update();

		vtkImageData *tempimage= vtkImageData::New();

		vtkImageReslice *resliceax1temp = vtkImageReslice::New();
		resliceax1temp->SetInput(imageData_ax1);
		resliceax1temp->InterpolateOn();
		resliceax1temp->SetInterpolationModeToCubic();
		resliceax1temp->SetResliceTransform(txarticulatedaxial1temp);
		//resliceax1temp->SetOutputExtent(imageData_ax1->GetExtent());//imageData_ax1perslice[i]->GetExtent());
		resliceax1temp->SetOutputSpacing(imageData_ax1->GetSpacing());
		resliceax1temp->AutoCropOutputOn();
		resliceax1temp->GetInput()->Update();
		resliceax1temp->Update();
		tempimage->DeepCopy( resliceax1temp->GetOutput());
		tempimage->Update();

		tempimage->GetExtent(imageextent);
		vtkImageReslice *resliceax1temp2 = vtkImageReslice::New();
		imageData_ax1perslice[i] = vtkImageData::New();
		resliceax1temp2->SetInput(tempimage);
		resliceax1temp2->InterpolateOn();
		resliceax1temp2->SetInterpolationModeToCubic();
		//resliceax1temp2->SetResliceTransform(txarticulatedaxial1);//(txarticulatedaxial1temp);
		resliceax1temp2->SetOutputExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],i,i);
		//resliceax1temp2->SetOutputExtent(imageData_ax1->GetExtent());//(imageData_ax1perslice2->GetExtent());//imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[4],imageextent[5]);
		resliceax1temp2->SetOutputOrigin(tempimage->GetOrigin());
		resliceax1temp2->SetOutputSpacing(imageData_ax1->GetSpacing());
		resliceax1temp2->AutoCropOutputOn();
		resliceax1temp2->GetInput()->Update();
		resliceax1temp2->Update();
		imageData_ax1perslice[i]=resliceax1temp2->GetOutput();
		imageData_ax1perslice[i]->Update();	





	}


	vtkTransform *txarticulatedaxial11tx=vtkTransform::New();
	txarticulatedaxial11tx->SetMatrix(volmriarticulated1[0]);
	txarticulatedaxial11tx->Update();

	vtkImageReslice *resliceax111 = vtkImageReslice::New();
	resliceax111->SetInput(imageData_ax1);
	resliceax111->InterpolateOn();
	resliceax111->SetInterpolationModeToCubic();
	resliceax111->SetResliceTransform(txarticulatedaxial11tx);
	resliceax111->SetOutputSpacing(imageData_ax1->GetSpacing());
	resliceax111->AutoCropOutputOn();
	resliceax111->GetInput()->Update();
	resliceax111->Update();
	imageData_ax1->DeepCopy( resliceax111->GetOutput());
	imageData_ax1->Update();
	//imageData_ax1->GetExtent(imageextent);

	CSpinButtonCtrl *pSpin = reinterpret_cast<CSpinButtonCtrl *>(GetDlgItem(IDC_SPINslice)); 
	pSpin->SetRange32(1,numimages);

	aRenderer->RemoveAllProps();
	showtopo();
	DisplayVisibleActors();

	for (int ijk=5; ijk<6;ijk+=1)
	{
		planeWidgetX6[ijk]=vtkImagePlaneWidget::New();
		planeWidgetX6[ijk]->DisplayTextOn();
		planeWidgetX6[ijk]->SetInput(imageData_ax1perslice[ijk]);
		planeWidgetX6[ijk]->UserControlledLookupTableOn();
		planeWidgetX6[ijk]->SetLookupTable(bwLutaxial);
		planeWidgetX6[ijk]->SetPlaneOrientationToZAxes();
		//planeWidgetX6[ijk]->SetSliceIndex(3);
		planeWidgetX6[ijk]->SetPicker(picker);
		planeWidgetX6[ijk]->SetKeyPressActivationValue('x');
		vtkProperty *prop1 = planeWidgetX6[ijk]->GetPlaneProperty();
		prop1->SetColor(1, 0, 0);
		planeWidgetX6[ijk]->SetInteractor( renWin->GetInteractor() );
		planeWidgetX6[ijk]->On();


	}
	refreshscreen();

}


/*
void Cfull_registration_dialogDlg::OnBnClickedButtonarticulated()
{
int base_slice=0;
vtkMatrix4x4 *volo3[17]; vtkMatrix4x4 *volrep[17];vtkMatrix4x4 *vollocaldef[17]; vtkMatrix4x4 *vollocaldeftrans[17];	vtkMatrix4x4 *volmri_init;
vtkMatrix4x4 *vollocaldeftrans2[17];
float themat[9], thetrans[3]={0,0,0};
char temp_string_wrl[200];

//generate slice by slice image data
int imageextent[6];
imageData_ax1->GetExtent(imageextent);




//1st register the topo to the xray using tps and external landmarks
unregisteredtopo = Transform::RegisterTopoTPS (unregisteredtopo, unregisteredxray);

//register the mri verts to the xray verts using the artisulated model
//matrices already obtained using matlab

//load xray tx, 1st is T1 i think
std::ifstream numbers_strtxo3( o3txfile );
numbers_strtxo3.seekg(0, ios::beg);

for(int i=0;i<17;i++)
{
volo3[i]= vtkMatrix4x4::New();
numbers_strtxo3.getline(temp_string_wrl,200); 
sscanf_s (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
volo3[i]->SetElement(0,3,thetrans[0]); 
volo3[i]->SetElement(1,3,thetrans[1]); 
volo3[i]->SetElement(2,3,thetrans[2]);
volo3[i]->SetElement(3,3,1);
numbers_strtxo3.getline(temp_string_wrl,200); 
sscanf_s (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);
for(int ab=0;ab<3;ab++)
{
for(int bb=0;bb<3;bb++)
volo3[i]->SetElement(ab,bb,themat[ab+bb*3]); 
}

}

std::ifstream numbers_strtxrep( reptxfile );
numbers_strtxrep.seekg(0, ios::beg);
//load mri tx, 1st is T1 i think
for(int i=0;i<17;i++)
{
volrep[i]= vtkMatrix4x4::New();
numbers_strtxrep.getline(temp_string_wrl,200); 
sscanf (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
volrep[i]->SetElement(0,3,thetrans[0]); 
volrep[i]->SetElement(1,3,thetrans[1]); 
volrep[i]->SetElement(2,3,thetrans[2]);
volrep[i]->SetElement(3,3,1);
numbers_strtxrep.getline(temp_string_wrl,200); 
sscanf (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);

for(int ab=0;ab<3;ab++)
{
for(int bb=0;bb<3;bb++)
volrep[i]->SetElement(ab,bb,themat[ab+bb*3]); 
}
volrep[i]->Invert();
}

for(int gg=0;gg<17;gg++)
{
mrivert_regvol[gg]->Identity(); //start it with L5
mrivert_regvol[gg]->Multiply4x4(volo3[gg],volrep[gg],mrivert_regvol[gg]); //i guess starts with L5 
//so vol03 o volrep 1st tx after
}


for(int lala=0;lala<17;lala++)
{
vtkTransform *movevert=vtkTransform::New();
movevert->SetMatrix(mrivert_regvol[base_slice]);//[16-lala]);
vtkTransformPolyDataFilter *f123 = vtkTransformPolyDataFilter::New ();
f123->SetInput (mri_newlandmarkspart[lala]); 
f123->SetTransform (movevert);
f123->Update ();
mri_newlandmarkspart[lala]->DeepCopy(f123->GetOutput()); 
}

//MRI for each point, register it as a weighted sum of the topo and articulated model
//functions depending on distance.

//start with rigid registration equal to global articulated transformation of L5
voloriginmri2->Multiply4x4(mrivert_regvol[base_slice],voloriginmri2,voloriginmri2);

//transformation originale
vtkMatrix4x4 *volmriarticulated1[17];
for (int thth = 0; thth <17;thth++)
{
volmriarticulated1[thth]= vtkMatrix4x4::New();
volmriarticulated1[thth]->Identity();
//	volmriarticulated1[thth]->SetElement(0,3,mrivert_regvol[16-thth]->GetElement(0,3)); 
//volmriarticulated1[thth]->SetElement(1,3,mrivert_regvol[16-thth]->GetElement(1,3)); 
//volmriarticulated1[thth]->SetElement(2,3,mrivert_regvol[16-thth]->GetElement(2,3));

volmriarticulated1[thth]->Multiply4x4(mrivert_regvol[thth],volmriarticulated1[thth],volmriarticulated1[thth]); //just a starting point, will remove
volmriarticulated1[thth]->Invert();
}
vtkTransform *txarticulatedaxial1=vtkTransform::New();
txarticulatedaxial1->SetMatrix(volmriarticulated1[base_slice]);
txarticulatedaxial1->Update();

//imageData_ax1->GetExtent(imageextent); //extent = 0,447,0,311,0,26


//at this point, the x and y values have been flipped due to the while loading
vtkImageReslice *resliceax1 = vtkImageReslice::New();
resliceax1->SetInput(imageData_ax1);
resliceax1->InterpolateOn();
resliceax1->SetInterpolationModeToCubic();
resliceax1->SetResliceTransform(txarticulatedaxial1);
resliceax1->SetOutputSpacing(imageData_ax1->GetSpacing());
resliceax1->AutoCropOutputOn();
resliceax1->GetInput()->Update();
resliceax1->Update();
//imageData_ax1->DeepCopy( resliceax1->GetOutput());
//imageData_ax1->Update();
//imageData_ax1->GetExtent(imageextent);

float def[3];
double voxelvalue[3];

vtkShortArray *scalars = vtkShortArray::New();
vtkFloatArray *scalarsdeformation = vtkFloatArray::New();
scalarsdeformation->SetNumberOfComponents(3);

int newmri_dimz=imageextent[5];//(int) mri_dimz;///2,/4,/4
int newmri_dimy=imageextent[3];//(int) mri_dimy;
int newmri_dimx=imageextent[1];//(int) mri_dimx;
for (int i=0;i<newmri_dimz;i++)
{
for (int j=0;j<newmri_dimy;j++)
{
for (int k=0;k<newmri_dimx;k++)
{
scalars->InsertNextTuple1(0);
scalarsdeformation->InsertNextTuple3(0,0,0);
}}}

//this is where we calculate the non rigid part of the deformation

//find the inverse of mrivert_regvol[0], the overall tx are L5 to T1
//but volo3 is T1 to L5
vtkMatrix4x4 *invGlobal = vtkMatrix4x4::New();
invGlobal->DeepCopy(volmriarticulated1[base_slice]);//16 is T1
invGlobal->Invert(); //needed
///find the transformation of MRI to Xray without the global T0 transformation Tvi = Tvi o ((Tv1)^-1 o Tv1)
//thus each vertebral tx is: Tvi o ((Tv1)^-1).  apply (Tv1)^-1 to each ready tx

//overall transformation is :voloriginmri1display followed by volmriarticulated1
for(int gg=0;gg<17;gg++)
{
vollocaldef[gg]= vtkMatrix4x4::New();
vollocaldef[gg]->Identity(); //
//vollocaldef[gg]->Multiply4x4(volmriarticulated1[gg],invGlobal,vollocaldef[gg]);//crap
vollocaldef[gg]->Multiply4x4(volmriarticulated1[gg],mrivert_regvol[base_slice],vollocaldef[gg]); //0 being L5
//vollocaldef[gg]->Multiply4x4(mrivert_regvol[gg],invGlobal,vollocaldef[gg]);
vollocaldeftrans[gg]= vtkMatrix4x4::New();
vollocaldeftrans[gg]->Identity();

vollocaldeftrans[gg]= vtkMatrix4x4::New();
vollocaldeftrans[gg]->Identity();
vollocaldeftrans[gg]->SetElement(0,0,vollocaldef[gg]->GetElement(0,0)); 
vollocaldeftrans[gg]->SetElement(0,1,vollocaldef[gg]->GetElement(0,1)); 
vollocaldeftrans[gg]->SetElement(1,0,vollocaldef[gg]->GetElement(1,0)); 
vollocaldeftrans[gg]->SetElement(1,1,vollocaldef[gg]->GetElement(1,1)); 

vollocaldeftrans[gg]->SetElement(0,3,vollocaldef[gg]->GetElement(0,3)); 
vollocaldeftrans[gg]->SetElement(1,3,vollocaldef[gg]->GetElement(1,3)); 
vollocaldeftrans[gg]->SetElement(2,3,vollocaldef[gg]->GetElement(2,3));

}

//apply only the translation

float mri1origin[3];
imageData_ax1->GetOrigin(mri1origin);
float current_voxel_location[3];
scalars = (vtkShortArray *)(imageData_ax1->GetPointData()->GetScalars());

float imagespacing[3];
imageData_ax1->GetSpacing(imagespacing);

int subID, cellNum; 
float *paraCoords = new float[3]; 
float *xyzCoords  = new float[3]; 
float *vtkcellweights = new float[4]; 

float *Coordstopmiddle  = new float[3]; 
float *Coordsbottommiddle  = new float[3]; 
//find the linear tx T_{v-MRI-Xray} for each voxel
int numnonzeros=0; //slice 17, closest to L2

//find the top and bottom central points
int middlez = (int)(newmri_dimz/2);
int middley = (int)(newmri_dimy/2);
int middlex = (int)(newmri_dimx/2);
int offset11 = (int)((newmri_dimy*newmri_dimx*(newmri_dimz-1))+newmri_dimx*middley+middlex);
subID = imageData_ax1->GetCell( offset11 )->GetParametricCenter(paraCoords); 
int &subIDadd2 = subID; 
imageData_ax1->GetCell(offset11)->EvaluateLocation(subIDadd2, paraCoords, Coordstopmiddle, vtkcellweights); 

offset11 = (int)((newmri_dimy*newmri_dimx*0)+newmri_dimx*middley+middlex);
subID = imageData_ax1->GetCell( offset11 )->GetParametricCenter(paraCoords); 
int &subIDadd3 = subID; 
imageData_ax1->GetCell(offset11)->EvaluateLocation(subIDadd3, paraCoords, Coordsbottommiddle, vtkcellweights); 

vtkTransform *m_transformtemp = vtkTransform::New();
m_transformtemp->Identity();
m_transformtemp->PostMultiply();//tporientation[2]
//m_transform2->Translate(-zax[0],-zax[1],-xyzCoords[2]);//(-zax[0],-zax[1],-zax[2]);//tporientation[2]
m_transformtemp->Translate(-Coordstopmiddle[0],-Coordstopmiddle[1],-(Coordstopmiddle[2]+Coordsbottommiddle[2])/2);
//m_transform2->RotateWXYZ(30,0,0,1);//cp[0],cp[1],cp[2]);//Coordstopmiddle[0]-Coordsbottommiddle[0],Coordstopmiddle[1]-Coordsbottommiddle[1],Coordstopmiddle[2]-Coordsbottommiddle[2]);//cp2[1],cp2[2],cp2[3]);
//m_transform2->RotateZ(90);
//m_transform2->Translate(Coordstopmiddle[0],Coordstopmiddle[1],0);
//m_transform2->Translate(Coordstopmiddle[0],Coordstopmiddle[1],0);//(Coordstopmiddle[2]+Coordsbottommiddle[2])/2);//(zax[0],zax[1],xyzCoords[2]);//(xyzCoords[0],xyzCoords[1],xyzCoords[2]);//
m_transformtemp->Update();


//vtkImageReslice *resliceax1 = vtkImageReslice::New();
//resliceax1->SetInput(imageData_ax1);
//resliceax1->InterpolateOn();
//resliceax1->SetInterpolationModeToCubic();
//resliceax1->SetResliceTransform(m_transformtemp);
//resliceax1->AutoCropOutputOn();
//resliceax1->GetInput()->Update();
//resliceax1->Update();
//imageData_ax1=resliceax1->GetOutput();

int numimages = newmri_dimz;
vtkImageData **imageData_ax1perslice = new vtkImageData*[newmri_dimz];

for (int i=0;i<newmri_dimz;i++) //actual right dimensions taken from imagedata: newmri_dimz
{


int closest_vert=1, second_vert=0; //closest is 13, second is 14
float closest_dist2=10000000000000, second_dist2=10000000000000;

offset11 = (int)((newmri_dimy*newmri_dimx*i)+newmri_dimx*middley+middlex);
subID = imageData_ax1->GetCell( offset11 )->GetParametricCenter(paraCoords); 
int &subIDadd3 = subID; 
imageData_ax1->GetCell(offset11)->EvaluateLocation(subIDadd2, paraCoords, xyzCoords, vtkcellweights); 

current_voxel_location[2]=xyzCoords[2];//(float)i*imagespacing[2]+mri1origin[2];//not nesseraciry it has an orientation...
for(int hhh=0;hhh<17;hhh++) //starting at L5
{
//float tempdist=sqrt(pow((current_voxel_location[0]-volo3[16-hhh]->GetElement(0,3)),2)+pow((current_voxel_location[1]-volo3[16-hhh]->GetElement(1,3)),2)+pow((current_voxel_location[2]-volo3[16-hhh]->GetElement(2,3)),2));
float tempdist=abs(current_voxel_location[2]-volo3[hhh]->GetElement(2,3));//was 16-hhh
if(tempdist<closest_dist2)
{
second_vert=closest_vert;
second_dist2=closest_dist2;
closest_vert=hhh; //13 = L2, 17 being L5, that makes no sense sine=ce overall 0 is L5
closest_dist2=tempdist;
}
else if(tempdist<second_dist2)
{
second_vert=hhh;
second_dist2=tempdist;
}
}
//	vtkTransform *m_transform = vtkTransform::New();
//	m_transform->SetMatrix(vollocaldef[closest_vert-1]);//(vollocaldeftrans[closest_vert]);//this last one works
//	m_transform->Update();

vtkTransform *m_transform2 = vtkTransform::New();
//m_transform2->SetMatrix(vollocaldeftrans[closest_vert-1]);//(vollocaldef[second_vert]);////(vollocaldef[second_vert]);



vtkTransform *txarticulatedaxial1temp=vtkTransform::New();
txarticulatedaxial1temp->SetMatrix(vollocaldef[closest_vert]);
txarticulatedaxial1temp->Identity();
txarticulatedaxial1temp->Update();

imageData_ax1->GetExtent(imageextent);
vtkImageReslice *resliceax1temp = vtkImageReslice::New();
resliceax1temp->SetInput(imageData_ax1);
resliceax1temp->InterpolateOn();
resliceax1temp->SetInterpolationModeToCubic();
//resliceax1temp->SetResliceTransform(txarticulatedaxial1);//(txarticulatedaxial1temp);
resliceax1temp->SetOutputExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],i,i);
resliceax1temp->SetOutputSpacing(imageData_ax1->GetSpacing());
resliceax1temp->AutoCropOutputOn();
resliceax1temp->GetInput()->Update();
resliceax1temp->Update();
imageData_ax1perslice[i]=vtkImageData::New();
imageData_ax1perslice[i]->DeepCopy( resliceax1temp->GetOutput());
imageData_ax1perslice[i]->Update();	

double orientation[3];
double theposition[3];

//find new z axis by transforming point 0,0,10k
float *tptemp  = new float[3];
float *tporientation  = new float[4];
///	float zax[3] = {volo3[closest_vert-1]->GetElement(0,3)-volo3[closest_vert]->GetElement(0,3),volo3[closest_vert-1]->GetElement(1,3)-volo3[closest_vert]->GetElement(1,3),volo3[closest_vert-1]->GetElement(2,3)-volo3[closest_vert]->GetElement(2,3)};
float zax[3] = {volo3[closest_vert]->GetElement(0,3),volo3[closest_vert]->GetElement(1,3),volo3[closest_vert]->GetElement(2,3)};
///axis is in here..

//find the location of the middle point

txarticulatedaxial1->GetOrientationWXYZ(tporientation);
txarticulatedaxial1->GetPosition(theposition);
//m_transform->TransformPoint(zax, tptemp); 

//Coordstopmiddle[0]=volo3[0]->GetElement(0,3);
//Coordstopmiddle[1]=volo3[0]->GetElement(1,3);
//Coordstopmiddle[2]=volo3[0]->GetElement(2,3);

m_transform2->Identity();
m_transform2->PostMultiply();//tporientation[2]
//m_transform2->Translate(-zax[0],-zax[1],-xyzCoords[2]);//(-zax[0],-zax[1],-zax[2]);//tporientation[2]
//m_transform2->Translate(-Coordstopmiddle[0],-Coordstopmiddle[1],0);//-(Coordstopmiddle[2]+Coordsbottommiddle[2])/2);
//m_transform2->RotateWXYZ(30,0,0,1);//cp[0],cp[1],cp[2]);//Coordstopmiddle[0]-Coordsbottommiddle[0],Coordstopmiddle[1]-Coordsbottommiddle[1],Coordstopmiddle[2]-Coordsbottommiddle[2]);//cp2[1],cp2[2],cp2[3]);
//	m_transform2->RotateZ(20);
//m_transform2->Translate(Coordstopmiddle[0],Coordstopmiddle[1],0);
//m_transform2->Translate(Coordstopmiddle[0],Coordstopmiddle[1],0);//(Coordstopmiddle[2]+Coordsbottommiddle[2])/2);//(zax[0],zax[1],xyzCoords[2]);//(xyzCoords[0],xyzCoords[1],xyzCoords[2]);//
m_transform2->Update();


for (int j=0;j<(newmri_dimy);j=j+1)
{
for (int k=0;k<(newmri_dimx);k=k+1)
{

int offset1 = (int)((newmri_dimy*newmri_dimx*i)+newmri_dimx*j+k);
float curval =  scalars->GetTuple1(offset1);

//if(curval>6) ////////////////add check if the MRi value >0
//  {
numnonzeros++;

subID = imageData_ax1->GetCell( offset1 )->GetParametricCenter(paraCoords); 
int &subIDadd = subID; 
imageData_ax1->GetCell(offset1)->EvaluateLocation(subIDadd, paraCoords, xyzCoords, vtkcellweights); 

current_voxel_location[0]=(float)(xyzCoords[0]);//k*imagespacing[0]+mri1origin[0];
current_voxel_location[1]=(float)(xyzCoords[1]);//j*imagespacing[1]+mri1origin[1];
current_voxel_location[2]=(float)(xyzCoords[2]);//i*imagespacing[2]+mri1origin[2];
//closest_dist=sqrt(pow((current_voxel_location[0]-volo3[16-closest_vert]->GetElement(0,3)),2)+pow((current_voxel_location[1]-volo3[16-closest_vert]->GetElement(1,3)),2)+pow((current_voxel_location[2]-volo3[16-closest_vert]->GetElement(2,3)),2));
//float closest_dist=sqrt(pow((current_voxel_location[0]-volo3[16-closest_vert]->GetElement(0,3)),2)+pow((current_voxel_location[1]-volo3[16-closest_vert]->GetElement(1,3)),2)+pow((current_voxel_location[2]-volo3[16-closest_vert]->GetElement(2,3)),2));
//float second_dist=sqrt(pow((current_voxel_location[0]-volo3[16-second_vert]->GetElement(0,3)),2)+pow((current_voxel_location[1]-volo3[16-second_vert]->GetElement(1,3)),2)+pow((current_voxel_location[2]-volo3[16-second_vert]->GetElement(2,3)),2));

//calculate new translation using t1
float *tp1 = new float[3];
float *tp2  = new float[3];
m_transform2->TransformPoint(current_voxel_location, tp1); 
//m_transform2->TransformPoint(tp1, tp2); 

//	float *tp2  = new float[3];//= m_transform2->TransformFloatPoint(current_voxel_location);//(current_voxel_location[0], current_voxel_location[1], current_voxel_location[2]); 
//	m_transform2->TransformPoint(current_voxel_location, tp2); 
//now tp and tp2 have the new suggested locations of the voxel
float tp3[3];
float total_dist=second_dist2+closest_dist2;

//calc average tx as (newlocation-old)*poids
//		tp3[0]= (tp1[0]*(second_dist2)+tp2[0]*(closest_dist2))/total_dist;
//	tp3[1]= (tp1[1]*(second_dist2)+tp2[1]*(closest_dist2))/total_dist;
//	tp3[2]= (tp1[2]*(second_dist2)+tp2[2]*(closest_dist2))/total_dist;

def[0] = (tp1[0] - current_voxel_location[0]);//*tempdistedge/(tempdistedge+tempdistvert);
def[1] = (tp1[1] - current_voxel_location[1]);//*tempdistedge/(tempdistedge+tempdistvert);
def[2] = (tp1[2] - current_voxel_location[2]);//*tempdistedge/(tempdistedge+tempdistvert);

//this here should work but doesnt
//def[0] = volmriarticulated1[closest_vert]->GetElement(0,3)+mrivert_regvol[10]->GetElement(0,3);
//def[1] = volmriarticulated1[closest_vert]->GetElement(1,3)+mrivert_regvol[10]->GetElement(1,3);
//def[2] = volmriarticulated1[closest_vert]->GetElement(2,3)+mrivert_regvol[10]->GetElement(2,3);


//	def[0] = volmriarticulated1[closest_vert]->GetElement(0,3)-volmriarticulated1[10]->GetElement(0,3);
//	def[1] = volmriarticulated1[closest_vert]->GetElement(1,3)-volmriarticulated1[10]->GetElement(1,3);
//	def[2] = volmriarticulated1[closest_vert]->GetElement(2,3)-volmriarticulated1[10]->GetElement(2,3);

int offset13 = (int)((newmri_dimy*newmri_dimx*i)+newmri_dimx*(j)+(k));
//scalarsdeformation->SetTuple3(offset13,0,0,0);
scalarsdeformation->SetTuple3(offset13,def[0],def[1],def[2]);
//}
}
}
}

vtkImageData* deformationgrid=vtkImageData::New();
imageData_ax1->GetExtent(imageextent);
deformationgrid->SetOrigin(imageData_ax1->GetOrigin()); 
deformationgrid->SetSpacing(imageData_ax1->GetSpacing()); 
deformationgrid->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[4],imageextent[5]);
deformationgrid->SetScalarTypeToFloat();
deformationgrid->SetNumberOfScalarComponents(3);
deformationgrid->AllocateScalars();
deformationgrid->GetPointData()->CopyAllOn();
deformationgrid->GetPointData()->CopyScalarsOn();
deformationgrid->GetPointData()->SetScalars(scalarsdeformation);
deformationgrid->Update();

//update MRI before optimizing for Bspline
vtkGridTransform* deformationtransform=vtkGridTransform::New();
deformationtransform->SetDisplacementScale(1);
deformationtransform->SetDisplacementGrid(deformationgrid);
deformationtransform->SetInterpolationModeToCubic();
deformationtransform->Update();
vtkImageReslice* reslicedeformation=vtkImageReslice::New();
reslicedeformation->SetInput(imageData_ax1);
reslicedeformation->SetOutputSpacing(imageData_ax1->GetSpacing());
//reslicedeformation->SetOutputExtent( imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[4],imageextent[5]);
reslicedeformation->SetOutputExtent(0,newmri_dimx,0,newmri_dimy,0,newmri_dimz);
reslicedeformation->SetResliceTransform(deformationtransform); 
reslicedeformation->SetInterpolationModeToLinear();
reslicedeformation->Update(); 
//imageData_ax1->SetExtent(0,newmri_dimx,0,newmri_dimy,0,newmri_dimz);
//imageData_ax1 = reslicedeformation->GetOutput();

//make TPvolume
vtkImageData* TPvolume=vtkImageData::New();
TPvolume->SetOrigin(imageData_ax1->GetOrigin()); 
TPvolume->SetSpacing(imageData_ax1->GetSpacing());
TPvolume->SetExtent(imageextent[0],imageextent[1],imageextent[2],imageextent[3],imageextent[4],imageextent[5]);
TPvolume->SetScalarTypeToFloat();
TPvolume->SetNumberOfScalarComponents(3);
//TPvolume=BSplineRegistration::GeneratevolfrpmTP(imageData_ax1, unregisteredtopo.polydatasze);
TPvolume=BSplineRegistration::ReadGeneratedTPVolume(imageData_ax1);
//do the Bspline optimization here so that the deformation transform can include deformationgrid +bsplinedeformationgrid
// imageData_ax1 = BSplineRegistration::RegisterBspline(imageData_ax1,TPvolume,mri_newlandmarkspart);

//need some kind of a refinement at this point. not a bad idea to use nikos's stuff.
//but that might throw me back into a depression.




//append all the imagedata into 1
vtkImageAppend *filter1=vtkImageAppend::New();
for(int iii=0;iii<numimages;iii++)
filter1->AddInput(imageData_ax1perslice[iii]);

filter1->Update();
//imageData_ax1 = filter1->GetOutput();

imageData_ax1->DeepCopy(imageData_ax1perslice[0]);
imageData_ax1->SetExtent(imageData_ax1perslice[0]->GetExtent());
imageData_ax1->SetSpacing(imagespacing);
//filter1 = vtk.vtkImageAppend()    
//      filter1.AddInput(slice2)
//    filter1.AddInput(slice1)
//  filter1.AddInput(slice2)
// filter1.SetAppendAxis(2)
//filter1.Update()       
//image2 = filter1.GetOutput() <-- has extent(0, 511, 0, 511, 2,4)



aRenderer->RemoveAllProps();
showtopo();
DisplayVisibleActors();
refreshscreen();
}
*/


void Cfull_registration_dialogDlg::OnBnClickedCheckmrisag()
{
	// Add the sagittal MRIs
	BOOL bvisiblemri = mricheck.GetCheck();
	BOOL bvisiblemrisag = mrisagcheck.GetCheck();

	if((bvisiblemrisag)&& (bvisiblemri))
		ShowMRIsag();
	else
	{
		aRenderer->RemoveAllProps();
		//add the ones that were visible. hack, i know
		DisplayVisibleActors();

	}
	refreshscreen();
}

void Cfull_registration_dialogDlg::OnBnClickedCheckmrivert()
{
	BOOL bvisiblemri = mricheck.GetCheck();
	BOOL bvisiblemrivert = mrivertcheck.GetCheck();

	if((bvisiblemrivert) && (bvisiblemri))
	{
		//add the vertebrae 
		for(int gg=0;gg<polydatawrl2vert.num_vertebrae;gg++)
		{
			aRenderer->AddActor(wrlactor2[gg]);
		}
		if (num_mris>1)
		{
			for(int gg=0;gg<polydatawrl3vert.num_vertebrae;gg++)
			{
				aRenderer->AddActor(wrlactor3[gg]);
			}
			if (num_mris>2)
			{
				for(int gg=0;gg<polydatawrl4vert.num_vertebrae;gg++)
				{
					aRenderer->AddActor(wrlactor4[gg]);
				}
			}
		}
	}
	else
	{
		aRenderer->RemoveAllProps();
		//add the ones that were visible. hack, i know
		DisplayVisibleActors();

	}
	refreshscreen();	
}

void Cfull_registration_dialogDlg::OnBnClickedCheckmri()
{
	BOOL bvisiblemri = mricheck.GetCheck();
	BOOL bvisiblevertlands = vertlandcheck.GetCheck();
	BOOL bvisiblemrivert = mrivertcheck.GetCheck();
	BOOL bvisiblemrisag = mrisagcheck.GetCheck();
	BOOL bvisibleextland = extlandcheck.GetCheck();

	if(bvisiblemri)
	{
		ShowMRI();
		if(bvisiblevertlands)
			showmrivertlandmarks();
		if(bvisiblemrisag)
			ShowMRIsag();
		if(bvisiblemrivert)
		{
			//add the vertebrae 
			for(int gg=0;gg<polydatawrl2vert.num_vertebrae;gg++)
			{
				aRenderer->AddActor(wrlactor2[gg]);
			}
			if (num_mris>1)
			{
				for(int gg=0;gg<polydatawrl3vert.num_vertebrae;gg++)
				{
					aRenderer->AddActor(wrlactor3[gg]);
				}
				if (num_mris>2)
				{
					for(int gg=0;gg<polydatawrl4vert.num_vertebrae;gg++)
					{
						aRenderer->AddActor(wrlactor4[gg]);
					}
				}
			}
		}
	}
	else
	{
		aRenderer->RemoveAllProps();
		DisplayVisibleActors();
	}
}




void Cfull_registration_dialogDlg::OnBnClickedButtonarticulated2()
{
	char patnname[200], segdir[200],  artmarkers[200];
/*	strcpy(patnname,"D:/patient_data_testing/modele_complet/2353729to1734048/registerednew/2353729to1734048b");
	strcpy(segdir,"D:/full_registration_dialogNew/Debug/segmentations/2353729to1734048bslice");
	strcpy(artmarkers,"2353729to1734048_artmarkers.txt");
	*/
	strcpy(patnname,"D:/patient_data_testing/modele_complet/1734048/registerednew/1734048b");
	strcpy(segdir,"D:/full_registration_dialogNew/Debug/segmentations/1734048bslice");
	strcpy(artmarkers,"1734048_artmarkers.txt");
	
	/*strcpy(patnname,"D:/patient_data_testing/modele_complet/2353729/registerednew/2353729");
	strcpy(segdir,"D:/full_registration_dialogNew/Debug/segmentations/2353729slice");
	strcpy(artmarkers,"2353729_artmarkers.txt");*/
	
	bool thefullon = true;// false ; //
	slicetobereg=19; 
	int numslicestoresult =1;
	
	
	slicetoshow=slicetobereg;
	char sliceme[3], fileforpoints[200], fileforpointsTP[200], TPslicetps[200], TPslicerigid[200], TPsliceart[200], MRIslicetps[200], MRIslicerigid[200], MRIsliceart[200], registeredText[200] ;
	int base_slice=0;
	vtkMatrix4x4 *volo3[17]; vtkMatrix4x4 *volrep[17];vtkMatrix4x4 *volrepreal[17];vtkMatrix4x4 *vollocaldef[17]; vtkMatrix4x4 *vollocaldeftrans[17];	vtkMatrix4x4 *volmri_init;
	vtkMatrix4x4 *vollocaldeftrans2[17];
	float themat[9], thetrans[3]={0,0,0};
	char temp_string_wrl[200];
	int imageextent[6];
	imageData_ax1->GetExtent(imageextent);
	vtkImageData *stencilmask = vtkImageData::New();
	rigidverts=0;
	tempimage2 = vtkImageData::New();
	int basevert=0; //slice 0 is at beggining of 2,  basevert=3 works well, no reslice

	/**************************************
	register the surface topography using thinplate splines
	*************************************************/
	unregisteredtopo = Transform::RegisterTopoTPS (unregisteredtopo, unregisteredxray);

	/**************************************
	Real rigid registration parameters
	*************************************************/
	vtkLandmarkTransform *Transrigidmri2 = vtkLandmarkTransform::New ();
	Transrigidmri2->SetSourceLandmarks (themripeds.polydata_mrilandmarks->GetPoints());
	int thtnumpointtest = themripeds.polydata_mrilandmarks->GetPoints()->GetNumberOfPoints();
	Transrigidmri2->SetTargetLandmarks (thexraypeds.polydata_wrllandmarks->GetPoints());
	int thtnumpointtest2 = thexraypeds.polydata_wrllandmarks->GetPoints()->GetNumberOfPoints();
	Transrigidmri2->SetModeToRigidBody();
	Transrigidmri2->Update ();
	rigidTransfoMatrix2 = vtkMatrix4x4::New();
	Transrigidmri2->GetMatrix(rigidTransfoMatrix2);

	/**************************************
	register the MRI vertebrae to the xray verts using the  
	articulated model matrices already obtained using matlab
	*************************************************/
	
	//load xray transformations, 1st is T1 i think
	std::ifstream numbers_strtxo3( o3txfile );
	numbers_strtxo3.seekg(0, ios::beg);

	for(int i=0;i<17;i++)
	{
		volo3[i]= vtkMatrix4x4::New();
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);
		
	for(int ab=0;ab<3;ab++)
			volo3[i]->SetElement(ab,3,thetrans[ab]); 
		volo3[i]->SetElement(3,3,1);

		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volo3[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}

	}

	std::ifstream numbers_strtxrep( reptxfile );
	numbers_strtxrep.seekg(0, ios::beg);

	//load mri transformations, 1st is T1 i think
	for(int i=0;i<17;i++)
	{
		volrep[i]= vtkMatrix4x4::New();
		volrepreal[i]= vtkMatrix4x4::New();
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);

		for(int ab=0;ab<3;ab++)
			volrep[i]->SetElement(ab,3,thetrans[ab]); 
		volrep[i]->SetElement(3,3,1);

		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volrep[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}

		for(int ab=0;ab<4;ab++)
		{
			for(int bb=0;bb<4;bb++)
				volrepreal[i]->SetElement(ab,bb,volrep[i]->GetElement(ab,bb)); 
		}


		volrep[i]->Invert();
	}

	//concatenate the MRI and XRay transformations
	for(int gg=0;gg<17;gg++)
	{
		mrivert_regvol[gg]->Identity(); //start it with L5, vol03 o volrep 1st tx after
		mrivert_regvol[gg]->Multiply4x4(volo3[gg],volrep[gg],mrivert_regvol[gg]); 
	}

	//transform the MRI landmarks
	for(int lala=0;lala<17;lala++)
	{
		vtkTransform *movevert=vtkTransform::New();
		movevert->SetMatrix(mrivert_regvol[16-lala]);
		vtkTransformPolyDataFilter *f123 = vtkTransformPolyDataFilter::New ();
		f123->SetInput (mri_newlandmarkspart[lala]); 
		f123->SetTransform (movevert);
		f123->Update ();
		mri_newlandmarkspart[lala]->DeepCopy(f123->GetOutput()); 
	}

	//Invert the transformation so it can be used for the MRI slices
	vtkMatrix4x4 *volmriarticulated1[17];
	for (int thth = 0; thth <17;thth++)
	{
		volmriarticulated1[thth]= vtkMatrix4x4::New();
		volmriarticulated1[thth]->Identity();
		volmriarticulated1[thth]->Multiply4x4(mrivert_regvol[thth],volmriarticulated1[thth],volmriarticulated1[thth]); 
		volmriarticulated1[thth]->Invert();
	}


	  	//transform only to get to MRI loading world space, pre-articulated tx
		vtkTransform *txoriginalaxial=vtkTransform::New();
		txoriginalaxial->Identity();
		txoriginalaxial->PostMultiply();
		txoriginalaxial->SetMatrix(voloriginmri1display); //only original tx, inverted
		txoriginalaxial->Update();
	
		//temporary in order to get a slice at the appropriate place, mri pre-articulated tx
		tempimage= vtkImageData::New();
		vtkImageReslice *resliceax1temp = vtkImageReslice::New();
		resliceax1temp->SetInput(imageData_ax1);
		resliceax1temp->InterpolateOn();
		resliceax1temp->SetInterpolationModeToCubic();
		resliceax1temp->SetResliceTransform(txoriginalaxial);
		resliceax1temp->SetOutputSpacing(imageData_ax1->GetSpacing());
		resliceax1temp->AutoCropOutputOn();
		resliceax1temp->GetInput()->Update();
		resliceax1temp->Update(); 
		
		tempimage->SetScalarTypeToShort();// 
		tempimage->SetNumberOfScalarComponents(1);
		tempimage->AllocateScalars();
		tempimage->GetPointData()->CopyAllOn();
		tempimage->GetPointData()->CopyScalarsOn();
		tempimage->DeepCopy( resliceax1temp->GetOutput()); 
		tempimage = resliceax1temp->GetOutput();
		tempimage->Update();
		tempimage->GetExtent(imageextent);

	vtkFloatArray *scalarsdeformation = vtkFloatArray::New();
	scalarsdeformation->SetNumberOfComponents(3);
	vtkDataArray *scalarsdeformationtemp = vtkShortArray::New();
	scalarsdeformationtemp->SetNumberOfComponents(1);

	int newmri_dimz=imageextent[5]+1;
	int newmri_dimy=imageextent[3]+1;
	int newmri_dimx=imageextent[1]+1;

	for (int i=0;i<newmri_dimz;i++)
	{
		for (int j=0;j<newmri_dimy;j++)
		{
			for (int k=0;k<newmri_dimx;k++)
			{
				scalarsdeformation->InsertNextTuple3(0,0,0);
				scalarsdeformationtemp->InsertNextTuple1(0);
			}}}


	//double current_voxel_location[3];

	int subID, cellNum; 
	double *paraCoords = new double[3]; 
	double *xyzCoords  = new double[3]; 
	double *paraCoords2 = new double[3]; 
	double *xyzCoords2  = new double[3]; 
	double *vtkcellweights = new double[4]; 

	//find the top and bottom central points
	int middlez = (int)(newmri_dimz/2);
	int middley = (int)(newmri_dimy/2);
	int middlex = (int)(newmri_dimx/2);
	int offset11 = (int)((newmri_dimy*newmri_dimx*(newmri_dimz-1))+newmri_dimx*middley+middlex);

	numimages = newmri_dimz;


	
	for (int curr_slice=slicetobereg;curr_slice<(slicetobereg+numslicestoresult);curr_slice++) 
	{

		 std::sprintf(sliceme,"%d",curr_slice);
		strcpy(fileforpoints,segdir);
		strcat(fileforpoints,sliceme);
	
		strcpy(TPsliceart,patnname);
		strcat(TPsliceart,"TPslice");
		strcat(TPsliceart,sliceme);

		if (thefullon == true)
		{
		strcat(fileforpoints,"resampledseg.ppm");
		strcat(TPsliceart,"articulatedseg.ppm");
		}
		else
		{
		strcat(fileforpoints,"resampled.ppm");
		strcat(TPsliceart,"articulated.ppm");
		}

		strcpy(TPslicetps,patnname);
		strcat(TPslicetps,"TPslice");
		strcat(TPslicetps,sliceme);
		strcat(TPslicetps,"tps.ppm");

		strcpy(TPslicerigid,patnname);
		strcat(TPslicerigid,"TPslice");
		strcat(TPslicerigid,sliceme);
		strcat(TPslicerigid,"rigid.ppm");

	strcpy(MRIslicetps,patnname);
	strcat(MRIslicetps, "MRIslice");
	strcat(MRIslicetps,sliceme);
	strcat(MRIslicetps,"tps.ppm");

	strcpy(MRIslicerigid,patnname);
	strcat(MRIslicerigid, "MRIslice");
	strcat(MRIslicerigid,sliceme);
	strcat(MRIslicerigid,"rigid.ppm");

	strcpy(MRIsliceart, patnname);
    strcat(MRIsliceart, "MRIslice");
	strcat(MRIsliceart,sliceme);
	strcat(MRIsliceart,"articulated.ppm");

	strcpy(registeredText, patnname);
	strcat(registeredText, "regReadmeslice");
	strcat(registeredText,sliceme);
	strcat(registeredText,".txt");

		currentsaveslice = curr_slice;
		int closest_vert=0; 
		float closest_dist2=10000000000000;

		/***********************************
		Find the closest vertebra to the center of the current slice
		***********************************/
		offset11 = (int)((newmri_dimy*newmri_dimx*curr_slice)+newmri_dimx*middley+middlex);
		subID = tempimage->GetCell( offset11 )->GetParametricCenter(paraCoords); //before rigid registration 
		double *bounds = new double[6];	
		tempimage->GetCell(offset11)->GetBounds(bounds); //before rigid registration
		xyzCoords[0]=bounds[0];xyzCoords[1]=bounds[2];xyzCoords[2]=bounds[4];

		for(int hhh=0;hhh<17;hhh++) //starting at L5
		{
			//float tempdist=abs(xyzCoords[2]-volo3[hhh]->GetElement(2,3));//was 16-hhh
			float tempdist=abs(xyzCoords[2]-volrepreal[hhh]->GetElement(2,3));
			if(tempdist<closest_dist2)
			{
				closest_vert=hhh; //13 = L2, 17 being L5, that makes no sense sine=ce overall 0 is L5... NEW: L4=1, so, good!
				closest_dist2=tempdist;
			}
		}
		
		//concatenate with overall transformation, just the articulated, which is great, since we transformed tempimage to real MRI space
		volmri1final->Identity();
		volmri1final->Multiply4x4(mrivert_regvol[closest_vert],volmri1final,volmri1final);


	float *tttemp = new float[3];
		float **angleMRIdistances;
		angleMRIdistances = new float*[3];
	for(int i = 0;i <3;i++)
		angleMRIdistances[i] = new float[360];
		
	

		vtkPoints *topoSlicePoints=	vtkPoints::New();
		vtkPoints *mriSlicePoints=	vtkPoints::New();
		vtkPolyData *topoSlicePdata = vtkPolyData::New();
		mriSlicePdata = vtkPolyData::New();

		if(thefullon == false)
		{
				AffineRegistration::GenerateMRISlices( tempimage, curr_slice,fileforpoints);
				AffineRegistration::GenerateTPslice(topoSlicePdata,unregisteredtopo.polydatasze, tempimage, curr_slice, mrivert_regvol[closest_vert], TPsliceart );
				//exit(1);
		}
		else
		{
			AffineRegistration::GeneratenewRegistrationPoints(unregisteredtopo.polydatasze, tempimage, curr_slice,volrepreal[closest_vert] , volo3[closest_vert],mrivert_regvol[closest_vert],topoSlicePdata, mriSlicePdata, fileforpoints, TPsliceart);

			
//cut the MRI arms using the segmentation
AffineRegistration::CutMRIarms( tempimage, curr_slice,fileforpoints);

		//edge points should be in the space of the unregistered MRI space i.e. tempimage with no transformation  
		vtkPoints *mriedgepoints=vtkPoints::New();
		vtkPolyData *MRIedgeData = vtkPolyData::New();
		AffineRegistration::GetnewMRIEdgePoints(tempimage, curr_slice,volrepreal[closest_vert], mrivert_regvol[closest_vert], angleMRIdistances,MRIedgeData, fileforpoints, TPsliceart);
		

	vtkPoints *mriedgepoints2=vtkPoints::New();
	for(int io = 0; io<360;io++)
	{
		mriedgepoints2->InsertNextPoint(angleMRIdistances[0][io],angleMRIdistances[1][io],angleMRIdistances[2][io]); 
	}

	mriedgepoints2->InsertNextPoint(angleMRIdistances[0][0],angleMRIdistances[1][0],angleMRIdistances[2][0]); 
	
	MRIedgeData->SetPoints(mriedgepoints2);



	//////apply transformation to the closest vertebra to use for distance purposes when calculating tps
		vtkTransform *txarticulatedaxial2temp222=vtkTransform::New();
		//txarticulatedaxial2temp222->SetMatrix(mrivert_regvol[closest_vert]);
		txarticulatedaxial2temp222->Identity();
		txarticulatedaxial2temp222->Update();
		vtkTransformPolyDataFilter *fvertsurface = vtkTransformPolyDataFilter::New ();
		if(polydatawrl2vert.num_vertebrae>=closest_vert)
			fvertsurface->SetInput (polydatawrl2vert.polydata[closest_vert]); //original
		else //check in polydata3
			fvertsurface->SetInput (polydatawrl3vert.polydata[closest_vert-xray3_vertindex]); 
		fvertsurface->SetTransform (txarticulatedaxial2temp222);
		fvertsurface->Update ();
		vtkPolyData *transformedVert=fvertsurface->GetOutput();
		
		vtkThinPlateSplineTransform *transform = vtkThinPlateSplineTransform::New(); 
		transform->SetSourceLandmarks(topoSlicePdata->GetPoints());// mriSlicePdata
		transform->SetTargetLandmarks(mriSlicePdata->GetPoints()); // topoSlicePdata
	//	transform->SetBasisToR2LogR();
		transform->SetBasisToR(); //this works for upper slices
		//transform->SetSigma(0.5);
		transform->Update();




	double *thespaceing = new double[3];
	thespaceing = tempimage->GetSpacing();
	tempimage->GetExtent(imageextent);

	//Convert the thin plate spline to a grid 
	vtkTransformToGrid *transformToGrid = vtkTransformToGrid::New(); 
	transformToGrid->SetInput(transform); 
	transformToGrid->SetGridExtent(0,(int)((float)imageextent[1]/1.0),0,(int)((float)imageextent[3]/1.0), 0,(int)((float)imageextent[5]/1.0)); 
	transformToGrid->SetGridOrigin(tempimage->GetOrigin()); //important
	transformToGrid->SetGridSpacing(thespaceing[0]*1, thespaceing[1]*1,thespaceing[2]*1); //makes a huge difference
	transformToGrid->SetGridScalarTypeToFloat(); 
	transformToGrid->Update();

		vtkImageData* deformationgrid=vtkImageData::New();
		deformationgrid->SetScalarTypeToFloat();
		deformationgrid->SetNumberOfScalarComponents(3);
		deformationgrid->AllocateScalars();
deformationgrid->GetPointData()->CopyAllOn();
deformationgrid->GetPointData()->CopyScalarsOn();
deformationgrid->SetExtent(tempimage->GetExtent());
deformationgrid = transformToGrid->GetOutput();

	//apply new TPS
	vtkDataArray *scalarsdeformation2 = vtkFloatArray::New();
	vtkDataArray *scalarsdeformationabs = vtkShortArray::New();
	scalarsdeformation2->SetNumberOfComponents(3);
	scalarsdeformationabs->SetNumberOfComponents(1);
	scalarsdeformation2 = transformToGrid->GetOutput()->GetPointData()->GetScalars();
	int gridextent[6];
	deformationgrid->GetExtent(gridextent);
	double *current_def = new double[3];

		//change space from xray to MRI
		double *avertpoint = new double[3];
		mri_newlandmarkspart[16-closest_vert]->GetPoints()->GetData()->GetTuple(0,avertpoint);	//this is in xray space now
		float radious = sqrt(pow(avertpoint[0]-volo3[closest_vert]->GetElement(0,3),2)+pow(avertpoint[1]-volo3[closest_vert]->GetElement(1,3),2));
	   basisVaryingTPS *theTps = new basisVaryingTPS;
	   theTps->SetNumLandmarks(mriSlicePdata->GetNumberOfPoints());
	   theTps->SetSourcePoints(mriSlicePdata);
	   theTps->SetTargetPoints(topoSlicePdata);//
	   theTps->setVertMiddle((double)(volrepreal[closest_vert]->GetElement(0,3)),(double)(volrepreal[closest_vert]->GetElement(1,3)),(double)(volrepreal[closest_vert]->GetElement(2,3)));
	   theTps->SetEdgePoints(angleMRIdistances);
	   theTps->setVertebralRadius((double)radious);//*1.4); 
	 // deformationgrid = theTps->DeformGrid(deformationgrid);
	  deformationgrid = theTps->DeformGridrBody(deformationgrid); ///good, removed for testing def text output.
	//   mriSlicePdata = theTps->DeformPoints(mriSlicePdata); 
	
	   //scalarsdeformation2 = transformToGrid->GetOutput()->GetPointData()->GetScalars(); ///whyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
	   for(int iii=0;iii<scalarsdeformation2->GetNumberOfTuples();iii++)
		   scalarsdeformationabs->InsertNextTuple1(0);

	//   tempimage = AffineRegistration::loadPPMvol2(tempimage, fileforpoints, curr_slice);
	/*  double *abedef = new double[3];
	   double maxdef = 0.0, newdef=0.0,multratio=0.0;
	   for(int iii=0;iii<scalarsdeformationabs->GetNumberOfTuples();iii++)
	   {
		   abedef=scalarsdeformation2->GetTuple3(iii);
		   newdef = sqrt(pow(abedef[0],2)+pow(abedef[1],2)+pow(abedef[2],2));
		   if(newdef>maxdef)
			  maxdef =  newdef; 
	   }
	   multratio = 1200.0/maxdef;
	   float multratio2 = 10.0;
	   
	   vtkImageData *ydeform = vtkImageData::New();
	   vtkDataArray *scalarsxdeform = vtkShortArray::New();
	   vtkDataArray *scalarsydeform = vtkShortArray::New();
	   scalarsxdeform->SetNumberOfComponents(1);
	    //save the output of scalarsdeformation2 in an image
	   // try writing file in text.
	   int outimageextent[6];
	   tempimage->GetExtent(outimageextent);
	   outimageextent[1]+=1;
	   outimageextent[3]+=1;

		std::ofstream xdeformOS("D:\\dicom_matlab\\xdeformnewnow_slice16.txt");
		std::ofstream ydeformOS("D:\\dicom_matlab\\ydeformnewnow_slice16.txt");

		int dimss[2];
		int width = outimageextent[1]; //already +1 aBOVE
		int height = outimageextent[3];
		dimss[0] = width;
		dimss[1] = height;
		//save the size of the image as x then y

		  int height2 = height;
		int width2 = width ;
		xdeformOS <<width2<<" " <<height2<<" "; 
		ydeformOS <<width2<<" " <<height2<<" "; 
   for(int iii=0;iii<scalarsdeformation2->GetNumberOfTuples();iii++)
	   {
		   abedef=scalarsdeformation2->GetTuple3(iii);
		  // scalarsdeformationabs->SetTuple1(iii, (int)(sqrt(pow(abedef[0],2)+pow(abedef[1],2)+pow(abedef[2],2))*multratio));
		    scalarsdeformationabs->SetTuple1(iii, (int)abs(abedef[0]*multratio));
			scalarsxdeform->InsertNextTuple1((int)(abedef[0]));
			scalarsydeform->InsertNextTuple1((int)(abedef[1]));
	   }


   int offset2;

     int sizedata = width2*height2;
   double *forxdeformFile = new double[sizedata];
   double *forydeformFile = new double[sizedata];
   //save only the right slice in binary file

     for(int i = 0; i<height2 ; i++)
  {
	    for(int j = 0; j<width2 ; j++)
		 {
			 //int offset1 = (int)((outimageextent[1]*outimageextent[3]*(curr_slice))+outimageextent[3]*(int)i+(int)j);
			 int offset1 = (int)((width2*height2*(curr_slice))+width2*(int)i+(int)j); //curr_slice

			 abedef=scalarsdeformation2->GetTuple3(offset1);
			 offset2 = (int)(width2*(int)i+(int)j); //the problem must be here*** elements out of order.

			  forxdeformFile[offset2] = abedef[0];
			  forydeformFile[offset2] = abedef[1];
			  xdeformOS <<abedef[0]<<" " ; 
			  ydeformOS <<abedef[1]<<" " ; 
		}
  }

	 xdeformOS.close();
	 ydeformOS.close();

	 int thedims[3];
	 	tempimage->GetDimensions(thedims);
		unsigned char *thevaluetemp = new unsigned char[3];
  width = thedims[0];
  height = thedims[1];
  float *floatval = new float[3];

	   vtkImageData *xdeform = vtkImageData::New();
	   xdeform->SetScalarTypeToShort();
		xdeform->SetNumberOfScalarComponents(1);
		xdeform->SetExtent(tempimage->GetExtent());
		xdeform->AllocateScalars();
		xdeform->GetPointData()->CopyAllOn();
		xdeform->GetPointData()->CopyScalarsOn();

			   ydeform->SetScalarTypeToShort();
		ydeform->SetNumberOfScalarComponents(1);
		ydeform->SetExtent(tempimage->GetExtent());
		ydeform->AllocateScalars();
		ydeform->GetPointData()->CopyAllOn();
		ydeform->GetPointData()->CopyScalarsOn();

	   xdeform->GetPointData()->SetScalars(scalarsxdeform);
	   ydeform->GetPointData()->SetScalars(scalarsydeform);
	   */

	  //exit(1);

		vtkGridTransform *gridTransform = vtkGridTransform::New(); 
		gridTransform->SetDisplacementGrid(deformationgrid); 
		gridTransform->SetInterpolationModeToCubic();
		gridTransform->Update();
		
		tempimage2->SetScalarTypeToShort();
		tempimage2->SetNumberOfScalarComponents(1);
		tempimage2->AllocateScalars();
		tempimage2->GetPointData()->CopyAllOn();
		tempimage2->GetPointData()->CopyScalarsOn();

		tempimage2->SetOrigin(tempimage->GetOrigin());
		tempimage->GetExtent(imageextent);
		vtkImageReslice *resliceax1temp3 = vtkImageReslice::New();
		resliceax1temp3->SetInput(tempimage);
		resliceax1temp3->InterpolateOn();
		resliceax1temp3->SetInterpolationModeToCubic();
		resliceax1temp3->SetResliceTransform(gridTransform); 
		resliceax1temp3->SetOutputOrigin(tempimage->GetOrigin());
		resliceax1temp3->SetOutputExtent(tempimage->GetExtent());
		resliceax1temp3->AutoCropOutputOn();
		resliceax1temp3->GetInput()->Update();
		resliceax1temp3->Update();

		tempimage2 = resliceax1temp3->GetOutput();
		tempimage2->GetExtent(imageextent);		
	//	tempimage2->GetPointData()->SetScalars(scalarsdeformationabs);
				
		//retransform points to display them
		vtkTransform *articulatedImageTransform=vtkTransform::New();
		articulatedImageTransform->SetMatrix(mrivert_regvol[closest_vert]);//(rigidTransfoMatrix2);//
		articulatedImageTransform->Update();

		
		vtkPolyData *tempdata2 = vtkPolyData::New();
		tempdata2->SetPoints(topoSlicePdata->GetPoints());
		fvertsurface->SetInput (tempdata2); //original
		fvertsurface->SetTransform (articulatedImageTransform);
		fvertsurface->Update ();
		tempdata2=fvertsurface->GetOutput();
		tempdata2->DeepCopy(fvertsurface->GetOutput());
		topoSlicePdata->SetPoints(tempdata2->GetPoints());

		///save all data
		//tilt topo in opposite ariculated registration, in order to put in same space as unregistered MRI
/*		AffineRegistration::GenerateTPslice(topoSlicePdata, unregisteredtopo.polydatasze, tempimage2, curr_slice, mrivert_regvol[closest_vert], TPslicetps);
		AffineRegistration::GenerateTPslice(topoSlicePdata, unregisteredtopo.polydatasze, tempimage, curr_slice, rigidTransfoMatrix2 , TPslicerigid); //mrivert_regvol[0]
		//AffineRegistration::GenerateTPslice(unregisteredtopo.polydatasze, tempimage, curr_slice, volo3[closest_vert],mrivert_regvol[closest_vert], TPsliceart );
		AffineRegistration::savePPM(tempimage,MRIsliceart , curr_slice);
		AffineRegistration::savePPM(tempimage2,MRIslicetps, curr_slice); 
		AffineRegistration::savePPM(tempimage, MRIslicerigid, curr_slice); //keep same MRI but give topo a different reverse tx
		*/
		vtkPolyData *tempdata = vtkPolyData::New();
		tempdata->SetPoints(mriSlicePdata->GetPoints());
		fvertsurface->SetInput (tempdata); //original
		fvertsurface->SetTransform (articulatedImageTransform);
		fvertsurface->Update ();
		tempdata=fvertsurface->GetOutput();
		tempdata->DeepCopy(fvertsurface->GetOutput());
		mriSlicePdata->SetPoints(tempdata->GetPoints());

		vtkSphereSource *sphere = vtkSphereSource::New();
		sphere->SetRadius(5);
		vtkGlyph3D *glyph2 = vtkGlyph3D::New();
		glyph2->SetInput(topoSlicePdata);
		glyph2->SetSource(sphere->GetOutput());
		glyph2->SetScaleModeToScaleByScalar();
		vtkPolyDataMapper *spikeMapper2 = vtkPolyDataMapper::New();
		spikeMapper2->SetInput(glyph2->GetOutput());
		spikeActorslice= vtkActor::New();
		spikeActorslice->SetMapper(spikeMapper2);
		spikeActorslice->GetProperty()->SetColor(0.0,1.0,0.0); //green is topo
		
		vtkGlyph3D *glyph3 = vtkGlyph3D::New();
		glyph3->SetInput(mriSlicePdata);//(MRIedgeData);//mriSlicePdata//MRIedgeData//regmriSlicePdata//closestpolys
		glyph3->SetSource(sphere->GetOutput());
		glyph3->SetScaleModeToScaleByScalar();
		vtkPolyDataMapper *spikeMapper3 = vtkPolyDataMapper::New();
		spikeMapper3->SetInput(glyph3->GetOutput());
		spikeActorslice2= vtkActor::New();
		spikeActorslice2->SetMapper(spikeMapper3);
		spikeActorslice2->GetProperty()->SetColor(1.0,0.0,0.0);//red is mri

		tempimage2->GetSpacing(thespaceing);
		double *intensityrange = new double[2];
		tempimage2->GetScalarRange(intensityrange);
/*		std::ofstream MRIoutput(registeredText,std::ios::app); 
		MRIoutput <<"num_sagmris "<< 2 <<endl;
MRIoutput <<"mrivert1= "<<thefilemrivert1<<endl;
MRIoutput <<"mrivert2= "<< thefilemrivert2<<endl;
MRIoutput <<"mrivert3= "<< thefilemrivert3<<endl;
MRIoutput <<"vertindex1= "<< xray2_vertindex<<endl;
MRIoutput <<"vertindex2= "<< xray3_vertindex<<endl;
MRIoutput <<"vertindex2= "<< xray4_vertindex<<endl;
MRIoutput <<"reptxfile= "<< reptxfile<<endl;
MRIoutput <<"o3txfile= "<< o3txfile<<endl; 
string mri_registered_marker;

	mri_registered_marker.assign(patientpath);
	mri_registered_marker.append(patientname);
	mri_registered_marker.append("_artmarkers.txt"); //artmarkers
	//MRIoutput <<"mrimarkers= "<<mri_registered_marker.c_str()<<endl; 
	MRIoutput <<"mrimarkers= "<<artmarkers<<endl; 
	MRIoutput <<"mri1= " << MRIslicetps << endl; 
	MRIoutput <<"mri_xyrange_intensity= " << imageextent[1]<<" "<< imageextent[3]<<" "<< mri_intensity_axial1 <<endl;
	MRIoutput <<"mri_spacing_slice= 1.116 1.116 14 " <<curr_slice<<endl; 
	MRIoutput <<"transfoori " << voloriginmri1display->GetElement(0,0) <<" "<< voloriginmri1display->GetElement(0,1) <<" "<<voloriginmri1display->GetElement(0,2) <<" "<<voloriginmri1display->GetElement(0,3) << endl;
	MRIoutput <<"transfo " << voloriginmri1display->GetElement(1,0) <<" "<< voloriginmri1display->GetElement(1,1) <<" "<<voloriginmri1display->GetElement(1,2) <<" "<<voloriginmri1display->GetElement(1,3) << endl;
	MRIoutput <<"transfo " << voloriginmri1display->GetElement(2,0) <<" "<< voloriginmri1display->GetElement(2,1) <<" "<<voloriginmri1display->GetElement(2,2) <<" "<<voloriginmri1display->GetElement(2,3) << endl;
	MRIoutput <<"transfo " << voloriginmri1display->GetElement(3,0) <<" "<< voloriginmri1display->GetElement(3,1) <<" "<<voloriginmri1display->GetElement(3,2) <<" "<<voloriginmri1display->GetElement(3,3) << endl;	

	MRIoutput <<"transfoart " << volmri1final->GetElement(0,0) <<" "<< volmri1final->GetElement(0,1) <<" "<<volmri1final->GetElement(0,2) <<" "<<volmri1final->GetElement(0,3) << endl;
	MRIoutput <<"transfo " << volmri1final->GetElement(1,0) <<" "<< volmri1final->GetElement(1,1) <<" "<<volmri1final->GetElement(1,2) <<" "<<volmri1final->GetElement(1,3) << endl;
	MRIoutput <<"transfo " << volmri1final->GetElement(2,0) <<" "<< volmri1final->GetElement(2,1) <<" "<<volmri1final->GetElement(2,2) <<" "<<volmri1final->GetElement(2,3) << endl;
	MRIoutput <<"transfo " << volmri1final->GetElement(3,0) <<" "<< volmri1final->GetElement(3,1) <<" "<<volmri1final->GetElement(3,2) <<" "<<volmri1final->GetElement(3,3) << endl;	
	MRIoutput.close();
	
	*/
	//save the markers
	double *P = new double[3];

	
	
	std::ofstream mri_registered_landmarks(artmarkers);// mri_registered_marker.c_str() );
		for(int lala=0;lala<17;lala++)	
		{
			for(int lala2=0;lala2<8;lala2++)	
			{
				P = mri_newlandmarkspart[lala]->GetPoints()->GetData()->GetTuple(lala2);	
				mri_registered_landmarks <<P[0]<<" " <<P[1]<<" " <<P[2]<<" " <<endl; 			
			}
		}
		mri_registered_landmarks.close();
		
	}
	}

	registered = true;

	//DisplayVisibleActors();
	aRenderer->RemoveAllProps();
	//showxray();
	refreshscreen();
	showtopo();
	
	ShowMRI();
	showMRIverts();
	//showxraylandmarks();
	showxrayvertlandmarks();
	showmrivertlandmarks();
	
	//aRenderer->AddActor(spikeActorslice);
	//aRenderer->AddActor(spikeActorslice2);
	refreshscreen();
}


void Cfull_registration_dialogDlg::OnDeltaposSpinslice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	//*pResult = 0;

	CSpinButtonCtrl *pSpin = reinterpret_cast<CSpinButtonCtrl *>(GetDlgItem(IDC_SPINslice)); 
	pSpin->UpdateData(TRUE);

	int Pos = pSpin->GetPos32();
	int delta = 	pNMUpDown->iDelta;

	if(((Pos+delta)>=0) && ((Pos+delta)<mri_dimz))
	{
		Pos+=delta;
		
		if(registered == TRUE)
		{
			slicetoshow = theslice[Pos];
			//int preslice = theslice[Pos-delta];
			aRenderer->RemoveActor (axialImageActor2[Pos-delta]);
			aRenderer->AddActor(axialImageActor2[Pos]);
		}
		else
		{
			slicetoshow = Pos;
			int testextent[6];
			vtkSmartPointer<vtkImageMapToColors> saggitalColors = vtkSmartPointer<vtkImageMapToColors>::New();
			saggitalColors->SetInput(imageData_ax1);
		 saggitalColors->SetLookupTable(bwLutaxial);
		saggitalColors->Update();
		imageData_ax1->GetExtent(testextent);

	axialImageActor->SetInput(saggitalColors->GetOutput());
	 axialImageActor->SetUserTransform(vtlaxial1tx);
	 axialImageActor->SetDisplayExtent(0,testextent[1], 0,testextent[3], slicetoshow,slicetoshow);

	    
	 aRenderer->AddActor(axialImageActor);
		}
		
	CEdit *Display;
	char tttt[4];
	sprintf(tttt,"%d",slicetoshow);

	Display = reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDITspin));
	CStringW tempme(tttt);
	LPCTSTR tt3 = (LPCTSTR)tempme;
	Display->SetWindowTextW(tt3);
	}
}



/****************
weoghted tx


/*	float **angleMRIdistances;
		angleMRIdistances = new float*[3];
	for(int i = 0;i <3;i++)
		angleMRIdistances[i] = new float[360];

	AffineRegistration::GetMRIEdgePoints(tempimage, 3,volo3[closest_vert], angleMRIdistances);

		for (int iii=0;iii<=gridextent[5];iii++)
		{
		for (int j=0;j<=gridextent[3];j++)
		{
			for (int k=0;k<=gridextent[1];k++)
			{
				int offsetdeformation = (int)(((gridextent[5]+1)*(gridextent[3]+1)*(iii))+(gridextent[3]+1)*j+k);
				tttemp = scalarsdeformation2->GetTuple3(offsetdeformation);
	
				//find out location of current point
				deformationgrid->GetCell(offsetdeformation)->GetBounds(bounds);
				xyzCoords[0]=bounds[0];xyzCoords[1]=bounds[2];xyzCoords[2]=bounds[4];
				//find in bounds of vert surface
				float dvert, dtopo, dedge, pcoords[3], weights[3];
				float theclosestpoint[3];
				vtkIdType cellId;
				 int subId,subID2;
				 float *maxval = new float[1];
				 maxval[0]=0.0;
				cellId = transformedVert->FindCell(xyzCoords, NULL, 0, .1, subId, pcoords, weights);

			//	if (cellId>=0) //inside so set deformation to 0
			//	{
			//		scalarsdeformation2->SetTuple3(offsetdeformation,0,0,0);
			//		int offsetMRItemp = (int)((iii*2)*(newmri_dimy)*(newmri_dimx)+((int)j*2)*(newmri_dimx)+(int)k*2);	
			//		tempimage->GetPointData()->SetTuple(offsetMRItemp ,maxval);
			//	}
			//	else
			//	{
					//need to calculate distance from vert surface
					//cellLocatorrigidbody->FindClosestPoint(xyzCoords ,theclosestpoint, cellId, subID2, dvert);
				
				//dvert=sqrt(pow((xyzCoords[0]-volo3[closest_vert]->GetElement(0,3)),2)+pow((xyzCoords[1]-volo3[closest_vert]->GetElement(1,3)),2));
					//need to have topo distance
				//	int offsetTP = (int)((iii*2)*(newmri_dimy+1)*(newmri_dimx+1)+((int)j*2)*(newmri_dimx+1)+(int)k*2);	
			//		dtopo = scalarsTP->GetTuple1(offsetTP);
									
			//		float warpratio = dvert/(dvert+dtopo);
			//		scalarsdeformation2->SetTuple3(offsetdeformation,warpratio*tttemp[0],warpratio*tttemp[1],warpratio*tttemp[2]);
			//	}	

				//find distance from point to vert
				float xdist = xyzCoords[0]-volo3[closest_vert]->GetElement(0,3);
				float ydist = xyzCoords[1]-volo3[closest_vert]->GetElement(1,3);
				dvert=sqrt(pow(xdist,2)+pow(ydist,2));

				//find angle from vertebral center to point  zc    
				int pointangle = (int)(atan2 (ydist,xdist) * 180 / 3.1416); 

				//use angle to find distance from point to endge  angleMRIdistances[][]
				dedge = sqrt(pow(xyzCoords[0]-angleMRIdistances[0][pointangle],2)+pow(xyzCoords[0]-angleMRIdistances[1][pointangle],2));
				float warpratio =  1;//dvert/(dvert+dedge);
				//
				scalarsdeformation2->SetTuple3(offsetdeformation,warpratio*tttemp[0],warpratio*tttemp[1],warpratio*tttemp[2]);

			}}}*/




void Cfull_registration_dialogDlg::OnBnClickedButtonsavetp()
{
		//registered surface topography
	string path2, path3;
	path2.assign("D:/patient_data_testing/modele_complet/");
	path2.append(patientname);
	path2.append("/registerednew/");
	path2.append(patientname);
	path2.append("registeredtopo.sze");
	Transform::save (unregisteredtopo.polydatasze, path2);


	//save the points in .ext format
	path3.assign("D:/patient_data_testing/modele_complet/");
	path3.append(patientname);
	path3.append("/registerednew/");
	path3.append(patientname);
	path3.append("registeredtopo.ext");

	std::ofstream numbers_str2(path3.c_str()); 

	 if(!numbers_str2) { 
    exit(1);
   } 

	string line1, line2;
	line1.assign("Objet:	Vertebre_L5");
	line2.assign("Grid_3D: # ----- tag ------ x --------- y --------- z --------- t -----");
	
	numbers_str2<<line1<<endl;
	numbers_str2<<line2<<endl;

	double *P= new double[3];
	for(int i = 0; i<unregisteredtopo.numelem; i++)
	{

		unregisteredtopo.szelandmark_points->GetData()->GetTuple(i,P);	
		
		numbers_str2 <<unregisteredtopo.the_landmark[i]<<" "<<P[0]<<" " <<P[1]<<" " <<P[2]<<" " <<endl; 
		
	}
}


void Cfull_registration_dialogDlg::OnBnClickedButtonsavemri()
{
		 char sliceme[3];
	 sprintf(sliceme,"%d",currentsaveslice);
	double *P = new double[3];
	string path;
	path.assign("D:/patient_data_testing/modele_complet/");
	path.append(patientname);
	path.append("/registered/");
	path.append(patientname);
	//global patient name is saved in: patientname
	char mritextfile[200];
	strcpy( mritextfile, path.c_str());
	strcat(mritextfile,"reg_readme");
	strcat(mritextfile,sliceme);
	strcat(mritextfile,".txt");
	std::ofstream numbers_str2(mritextfile); 

	 if(!numbers_str2) { 
    exit(1);
   } 
	
	 char mri1seriesname[200],mri2seriesname[200], mrivert1name[200],mrivert2name[200] ,mrivert3name[200];

	 strcpy( mri1seriesname, path.c_str());
	 strcat(mri1seriesname,"_UPPERreg");
	 strcpy(mri2seriesname, path.c_str());
	 strcat(mri2seriesname,"_LOWERreg");
	 strcpy(mrivert1name, path.c_str());
	 strcat(mrivert1name,"reg1.wrl");
	 strcpy(mrivert2name, path.c_str());
	 if(num_mris>1)
		strcat(mrivert2name,"reg2.wrl");
	 else strcat(mrivert2name,"reg1.wrl");
	 strcpy(mrivert3name, path.c_str());
	 if(num_mris>2)
		strcat(mrivert3name,"reg3.wrl");
	 else strcat(mrivert3name,"reg1.wrl"); //usless

	 string mri_registered_marker;
	 mri_registered_marker.assign( path.c_str());
	mri_registered_marker.append("_artmarkers.txt");

		string themrslicefile;
	
	themrslicefile.assign(mri1seriesname);
	
	themrslicefile.append(sliceme);
	themrslicefile.append(".ppm");

	numbers_str2 << "num_sagmris "<<num_mris<<endl; //number of sagittal mris
	numbers_str2 << "mrivert1= "<<mrivert1name<<endl; //mrivert1
	numbers_str2 << "mrivert2= "<<mrivert2name<<endl;//mrivert2
	numbers_str2 << "mrivert3= "<<mrivert3name<<endl;//mrivert3
	numbers_str2 << "vertindex1= "<<xray2_vertindex<<endl;//mrivert3
	numbers_str2 << "vertindex2= "<<xray3_vertindex<<endl;//mrivert3
	numbers_str2 << "vertindex2= "<<xray4_vertindex<<endl;//mrivert3
	numbers_str2 << "reptxfile= "<<reptxfile<<endl;//mrivert3
	numbers_str2 << "o3txfile= "<<o3txfile<<endl;//mrivert3
	numbers_str2 << "mrimarkers= "<<mri_registered_marker.c_str()<<endl;
	numbers_str2 << "mri1= "<<themrslicefile<<endl;


	//save the actual raw data
	  double *mriorigin=new double[3];
	  double spacing[3];
	  int mriextent[6];
	  imageData_ax1perslice[currentsaveslice]->GetOrigin(mriorigin);
	  imageData_ax1perslice[currentsaveslice]->GetExtent(mriextent);
	  imageData_ax1perslice[currentsaveslice]->GetSpacing(spacing);
	  	  
	//1 mri to save
	numbers_str2 << "mri_xyrange_intensity= "<<mriextent[1]+1<<" "<<mriextent[3]+1<<" "<<mri_intensity_axial1<< endl;
	numbers_str2 << "mri_spacing= "<<spacing[0]<<" "<<spacing[1]<<" "<<spacing[2]<< endl;
	numbers_str2 << "mri_location= "<<finalSliceOrigin[0]<<" "<<finalSliceOrigin[1]<<" "<<finalSliceOrigin[2]<< endl;
	numbers_str2 << "mricoloriginmri1= "<<voloriginmri1display->GetElement(0,0)<<" "<<voloriginmri1display->GetElement(0,1)<<" "<<voloriginmri1display->GetElement(0,2)<<" "<<voloriginmri1display->GetElement(0,3)<< endl;
	numbers_str2 << "mricoloriginmri1= "<<voloriginmri1display->GetElement(1,0)<<" "<<voloriginmri1display->GetElement(1,1)<<" "<<voloriginmri1display->GetElement(1,2)<<" "<<voloriginmri1display->GetElement(1,3)<< endl;
	numbers_str2 << "mricoloriginmri1= "<<voloriginmri1display->GetElement(2,0)<<" "<<voloriginmri1display->GetElement(2,1)<<" "<<voloriginmri1display->GetElement(2,2)<<" "<<voloriginmri1display->GetElement(2,3)<< endl;
	numbers_str2 << "mricoloriginmri1= "<<voloriginmri1display->GetElement(3,0)<<" "<<voloriginmri1display->GetElement(3,1)<<" "<<voloriginmri1display->GetElement(3,2)<<" "<<voloriginmri1display->GetElement(3,3)<< endl;
	
	numbers_str2 << "volmriarticulated1 = "<<volmri1final->GetElement(0,0)<<" "<<volmri1final->GetElement(0,1)<<" "<<volmri1final->GetElement(0,2)<<" "<<volmri1final->GetElement(0,3)<< endl;
	numbers_str2 << "volmriarticulated1 = "<<volmri1final->GetElement(1,0)<<" "<<volmri1final->GetElement(1,1)<<" "<<volmri1final->GetElement(1,2)<<" "<<volmri1final->GetElement(1,3)<< endl;
	numbers_str2 << "volmriarticulated1 = "<<volmri1final->GetElement(2,0)<<" "<<volmri1final->GetElement(2,1)<<" "<<volmri1final->GetElement(2,2)<<" "<<volmri1final->GetElement(2,3)<< endl;
	numbers_str2 << "volmriarticulated1 = "<<volmri1final->GetElement(3,0)<<" "<<volmri1final->GetElement(3,1)<<" "<<volmri1final->GetElement(3,2)<<" "<<volmri1final->GetElement(3,3)<< endl;

	numbers_str2 << "mrimv= "<<mrimv<<endl;
	numbers_str2.close();
	 
	//now the honor of saving the raw data
	AffineRegistration::savePPM(imageData_ax1perslice[currentsaveslice],themrslicefile.c_str(), 0); 

	//registered MRI vertebrae as wrl (in same way as they were read separate)
	//no longer necessary as i save the transfos
	showMRIverts();
	Transform::savewrlsep(wrlactor2, mrivert1name, polydatawrl2vert.num_vertebrae);
	if(num_mris>1)
		Transform::savewrlsep(wrlactor3, mrivert2name, polydatawrl3vert.num_vertebrae);
	if(num_mris>2)
		Transform::savewrlsep(wrlactor4, mrivert3name, polydatawrl3vert.num_vertebrae);


	std::ofstream mri_registered_landmarks( mri_registered_marker.c_str() );
	for(int lala = 0; lala<17; lala++)
	{
		for(int thetup = 0; thetup<8; thetup++)
		{
			mri_newlandmarkspart[lala]->GetPoints()->GetData()->GetTuple(thetup,P);	
		mri_registered_landmarks <<P[0]<<" " <<P[1]<<" " <<P[2]<<" " <<endl; 
		}
	}
	mri_registered_landmarks.close();



	//save the mri registered tps points cause serve for locating slice
	std::ofstream mri_tps_landmarks( "tps_landmarks.txt");

	for(int thetup = 0; thetup<mriSlicePdata->GetNumberOfPoints(); thetup++)
		{
			mriSlicePdata->GetPoints()->GetData()->GetTuple(thetup,P);	
		mri_tps_landmarks <<P[0]<<" " <<P[1]<<" " <<P[2]<<" " <<endl; 
		}
	mri_tps_landmarks.close();
}


void Cfull_registration_dialogDlg::OnBnClickedButtonloadreg()
{
	 vtkSmartPointer<vtkImageActor> axialImageActor = vtkSmartPointer<vtkImageActor>::New();

	vtkMatrix4x4 *volo3[17]; vtkMatrix4x4 *volrep[17];
	vtkMatrix4x4 *vollocaldeftrans2[17];
	vtkMatrix4x4  *volototal[30];
	vtkMatrix4x4 *voloriginmridisplay;
	vtkMatrix4x4 *volmrifinal[30];
	vtkTransform *totalTrasform[30];

	float themat[9], thetrans[3]={0,0,0};
	//mrimv= 260;
	rigidverts=0;
	// xray and topo as per the usual, the topo should have .ext by now
	unregisteredtopo.polydata_szelandmarks =  vtkPolyData::New ();
	unregisteredtopo.szelandmark_points=vtkPoints::New();
//	unregisteredtopo = Transform::read_landmarks_topo("D:/patient_data_testing/modele_complet/2353729/results/2353729registeredtopo.sze", unregisteredtopo);
	unregisteredtopo.polydatasze = Transform::read ("D:/patient_data_testing/modele_complet/2052364/registered/2052364registeredtopo.sze");

	int numcomponents = 3;
	//now load xray
	const char *thefilexray = "D:/patient_data_testing/modele_complet/2353729/2353729/2353729.wrl";
	unregisteredxray.polydata_wrllandmarks =  vtkPolyData::New ();
	unregisteredxray.wrllandmark_points=vtkPoints::New();
	unregisteredxray = Transform::read_landmarks_xray("D:/patient_data_testing/modele_complet/2052364/2052364/2052364.o3", unregisteredxray);
	polydatawrl = Transform::read (thefilexray);
	thexraypeds=Transform::read_spinal_marks_xray8("D:/patient_data_testing/modele_complet/2052364/2052364/2052364.wrl"); //was read_spinal_marks_xray

	//now load mri
	num_mris=0;
	char temp_string_wrl[200];
	char thefilemriland[200], thefilemrivert1[200], thefilemrivert2[200], thefilemrivert3[200];
	char temp_string2[200], thefilemrippm[30][200];
	float matrixorigin[16],  matrixarticulated[30][16];
	std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/2052364/2052364regReadmesall.txt"); 
	
	 if(!numbers_str2)
	 { 
    exit(1);
   } 
	 int num_axslices=0;
	 numbers_str2.seekg(0, ios::beg);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &num_mris);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert2);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert3);
	xray2_vertindex=-2; //start at L5
	xray3_vertindex=-2; //start at L4
	xray4_vertindex=-2;
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray2_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray3_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray4_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, reptxfile);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, o3txfile);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemriland);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[0], &matrixorigin[1], &matrixorigin[2], &matrixorigin[3]);//element(0,0), (0,1), (0,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[4], &matrixorigin[5], &matrixorigin[6], &matrixorigin[7]);//element(1,0), (1,1), (1,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[8], &matrixorigin[9], &matrixorigin[10], &matrixorigin[11]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[12], &matrixorigin[13], &matrixorigin[14], &matrixorigin[15]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &num_axslices);

	mri_dimz = 	 num_axslices;
	for (int imageindex = 0; imageindex < num_axslices; imageindex++)
	{
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrippm[imageindex]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange1[0], &mrirange1[1], &mri_intensity1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%d",temp_string2, &mri_spacing[0], &mri_spacing[1], &mri_spacing[2], &theslice[imageindex]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][0], &matrixarticulated[imageindex][1], &matrixarticulated[imageindex][2], &matrixarticulated[imageindex][3]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][4], &matrixarticulated[imageindex][5], &matrixarticulated[imageindex][6], &matrixarticulated[imageindex][7]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][8], &matrixarticulated[imageindex][9], &matrixarticulated[imageindex][10], &matrixarticulated[imageindex][11]);
		numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][12], &matrixarticulated[imageindex][13], &matrixarticulated[imageindex][14], &matrixarticulated[imageindex][15]); //matrixarticulated

	MRIregistered[imageindex] = vtkImageData::New();
	 MRIregistered[imageindex]->SetNumberOfScalarComponents(numcomponents);
	 MRIregistered[imageindex]  = AffineRegistration::loadPPMvol(thefilemrippm[imageindex], 0);
	 MRIregistered[imageindex]->SetSpacing(mri_spacing[0], mri_spacing[1], mri_spacing[2] );
	
	}
	numbers_str2.close();

	std::ifstream numbers_strtxo3( o3txfile );
	numbers_strtxo3.seekg(0, ios::beg);
	for(int i=0;i<17;i++)
	{
		volo3[i]= vtkMatrix4x4::New();
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volo3[i]->SetElement(0,3,thetrans[0]); 
		volo3[i]->SetElement(1,3,thetrans[1]); 
		volo3[i]->SetElement(2,3,thetrans[2]);
		volo3[i]->SetElement(3,3,1);
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);
		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volo3[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}

	}
	numbers_strtxo3.close();
	std::ifstream numbers_strtxrep( reptxfile );
	numbers_strtxrep.seekg(0, ios::beg);
	//load mri tx, 1st is T1 i think
	for(int i=0;i<17;i++)
	{
		volrep[i]= vtkMatrix4x4::New();
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volrep[i]->SetElement(0,3,thetrans[0]); 
		volrep[i]->SetElement(1,3,thetrans[1]); 
		volrep[i]->SetElement(2,3,thetrans[2]);
		volrep[i]->SetElement(3,3,1);
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);

		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volrep[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}
		volrep[i]->Invert();
	}

	for(int gg=0;gg<17;gg++)
	{
		mrivert_regvol[gg] = vtkMatrix4x4::New();
		mrivert_regvol[gg]->Identity(); //start it with L5
		mrivert_regvol[gg]->Multiply4x4(volo3[gg],volrep[gg],mrivert_regvol[gg]); //i guess starts with L5 
		//so vol03 o volrep 1st tx after
	}

	numbers_strtxrep.close();


	vtkMatrix4x4 *volmriarticulated1[17];
	vtkMatrix4x4 *volmriarticulated2[17];
	for (int thth = 0; thth <17;thth++)
	{
		volmriarticulated1[thth]= vtkMatrix4x4::New();
		volmriarticulated1[thth]->Identity();
		volmriarticulated1[thth]->Multiply4x4(mrivert_regvol[thth],volmriarticulated1[thth],volmriarticulated1[thth]); //just a starting point, will remove
		volmriarticulated1[thth]->Invert();

		volmriarticulated2[thth]= vtkMatrix4x4::New();
		volmriarticulated2[thth]->Identity();
		volmriarticulated2[thth]->DeepCopy(volmriarticulated1[thth]);
	}


	//read the verts
	polydatawrl2vert.polydata = new vtkPolyData*[20];
	polydatawrl3vert.polydata = new vtkPolyData*[20];
	polydatawrl4vert.polydata = new vtkPolyData*[20];


	polydatawrl2vert = Transform::read_wrlsep(thefilemrivert1);  //thefilemrivert1
	for(int gg=0;gg<polydatawrl2vert.num_vertebrae;gg++) 
			wrlactor2[gg]=vtkActor::New();

	if (num_mris>1)
	{
		polydatawrl3vert = Transform::read_wrlsep(thefilemrivert2);
		for(int gg=0;gg<polydatawrl3vert.num_vertebrae;gg++)
			wrlactor3[gg]=vtkActor::New();
	}
	if (num_mris>2)
	{
		polydatawrl4vert = Transform::read_wrlsep(thefilemrivert3);
		for(int gg=0;gg<polydatawrl4vert.num_vertebrae;gg++)
			wrlactor4[gg]=vtkActor::New();
	}

	//read the landmarks
	vtkFloatArray *values22 = vtkFloatArray::New();
	vtkPoints *newmrilandmark_pointspart[17];
	float P[3], p1, p2, p3;
	for(int lala=0;lala<17;lala++)	 
		newmrilandmark_pointspart[lala] = vtkPoints::New();

		std::ifstream numbers_str3(thefilemriland); 
			 if(!numbers_str3)
		 exit(1);
			
	 numbers_str3.seekg(0, ios::beg);
	for(int lala=0;lala<17;lala++)	
	{
		for(int theped=0;theped<8;theped++)	
		{
			numbers_str3.getline(temp_string_wrl,200); 
			sscanf (temp_string_wrl,"%f%f%f", &p1, &p2,&p3);
			newmrilandmark_pointspart[lala]->InsertNextPoint(p1,p2,p3); //starts with T1
		}
	}
	numbers_str3.close();
	
	for(int lala=0;lala<17;lala++)
	{
		mri_newlandmarkspart[lala]=vtkPolyData::New();
		mri_newlandmarkspart[lala]->SetPoints(newmrilandmark_pointspart[lala]); //bad name but ok
	}
	

	//read the image
	mri_dimx=312; mri_dimy=448;
//	mri_dimz = mrirange_axial1[1] - mrirange_axial1[0] +1;
	v18ax =  vtkSmartPointer<vtkVolume16Reader>::New();
	v18ax->SetDataDimensions (mri_dimx,mri_dimy);//(64, 64);//
	v18ax->SetImageRange(1,28);//(mrirange_axial1[0],mrirange_axial1[1]); //
	v18ax->SetDataByteOrderToLittleEndian(); 
	v18ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2052364\\axial_upper\\raw\\2052364");
//(thefilemri_axial1);
	v18ax->SetDataSpacing(1.116,1.116,12);//(mri_spacing_axial[0],mri_spacing_axial[1],mri_spacing_axial[2]);
	v18ax->Update();

	imageData_ax1 = vtkSmartPointer<vtkImageData>::New();
	imageData_ax1->SetScalarTypeToUnsignedChar();
	imageData_ax1->SetNumberOfScalarComponents(1);
	imageData_ax1->AllocateScalars();
	imageData_ax1->DeepCopy(v18ax->GetOutput());
	imageData_ax1->Update();

	voloriginmridisplay =  vtkMatrix4x4::New();
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
		{
		voloriginmridisplay->SetElement(i,j,matrixorigin[i*4+j]);
		}
	}


	//store images in displayable volume
	for (int imageindex = 0; imageindex < num_axslices; imageindex++)
	{
	volmrifinal[imageindex] = vtkMatrix4x4::New();
	volototal[imageindex]= vtkMatrix4x4::New();
		
	//when loaded: //element(0,0), (0,1), (0,2), (0,3)
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
		{
		volmrifinal[imageindex]->SetElement(i,j,matrixarticulated[imageindex][i*4+j]);
		}
	}



	//overall transfo
	volototal[imageindex]->Identity();
	volototal[imageindex]->Multiply4x4(voloriginmridisplay, volmrifinal[imageindex], volototal[imageindex]);


	totalTrasform[imageindex]=vtkTransform::New();
	totalTrasform[imageindex]->SetMatrix(volototal[imageindex]);
	totalTrasform[imageindex]->Update();

			//get the whole volume at the right place
	int theoutputextent[6];
		tempimage= vtkImageData::New();
		vtkImageReslice *resliceax1temp = vtkImageReslice::New();
		resliceax1temp->SetInput(imageData_ax1);
		resliceax1temp->InterpolateOn();
		resliceax1temp->SetInterpolationModeToCubic();
		resliceax1temp->SetResliceTransform(totalTrasform[imageindex]);
		resliceax1temp->SetOutputSpacing(imageData_ax1->GetSpacing());
		resliceax1temp->AutoCropOutputOn();
		resliceax1temp->GetInput()->Update();
		resliceax1temp->Update(); 
		
		tempimage2= vtkImageData::New();
		tempimage2->SetScalarTypeToShort();// 
		tempimage2->SetNumberOfScalarComponents(1);
		tempimage2->AllocateScalars();
		tempimage2->GetPointData()->CopyAllOn();
		tempimage2->GetPointData()->CopyScalarsOn();
		tempimage2->DeepCopy( resliceax1temp->GetOutput()); 
		tempimage2 = resliceax1temp->GetOutput();
		tempimage2->Update();
	
		//load the registered image at the right slice of tempimage2
		//assumption is that tempimag2e is at the right location here...
		 tempimage2 = AffineRegistration::loadPPMvol2(tempimage2, thefilemrippm[imageindex], theslice[imageindex]);
		
		// MRIregistered[0] as a reslice at a specific cut...
		 int theextent[6];
		 tempimage2->GetExtent(theextent);

	int testextent[6];
	tempimage->GetExtent(testextent);
	
	imageData_ax1->GetExtent(theextent);
	MRIregistered[imageindex]->GetExtent(theextent);

	int newmri_dimx = theextent[1];
	int newmri_dimy = theextent[3];
	int subID; 
	double *paraCoords = new double[3]; 
	int offset11 = (int)((newmri_dimy*newmri_dimx*theslice[imageindex]));
	double xyzCoords[6];
	subID = tempimage2->GetCell( offset11 )->GetParametricCenter(paraCoords); //before rigid registration 
	double *bounds = new double[6];	
	tempimage2->GetCell(offset11)->GetBounds(bounds); //before rigid registration
	xyzCoords[0]=bounds[0];xyzCoords[1]=bounds[2];xyzCoords[2]=bounds[4];
	//theslice[imageindex] is the slice number of what we want
	tempimage2->GetExtent(theoutputextent);
	vtkImageReslice *resliceax1temp2 = vtkImageReslice::New();
		resliceax1temp2->SetInput(tempimage2);
		resliceax1temp2->SetOutputSpacing(tempimage2->GetSpacing());
		resliceax1temp2->GetOutputExtent(theoutputextent);
		resliceax1temp2->SetOutputOrigin(xyzCoords[0],xyzCoords[1],xyzCoords[2]);
		resliceax1temp2->SetOutputExtent(theoutputextent[0], theoutputextent[1],theoutputextent[2],theoutputextent[3],0,0);
		//resliceax1temp2->SetOutputDimensionality(2);
		resliceax1temp2->AutoCropOutputOn();
		//resliceax1temp->WrapOn();
		resliceax1temp2->GetInput()->Update();
		resliceax1temp2->Update(); 

		MRIregistered[imageindex]->DeepCopy( resliceax1temp2->GetOutput()); 
		MRIregistered[imageindex] = resliceax1temp2->GetOutput();
		MRIregistered[imageindex]->Update();

	bwLutaxial = vtkLookupTable::New();
	bwLutaxial->SetTableRange(0, mri_intensity1);//100
	bwLutaxial->SetSaturationRange(0,0);
	bwLutaxial->SetHueRange(0,0);
	bwLutaxial->SetValueRange(0,1);
	bwLutaxial->Build(); 

	}


	planeWidgetZ5= vtkImagePlaneWidget::New();
	planeWidgetZ5->DisplayTextOn();
	planeWidgetZ5->SetInput(MRIregistered[0]);//((resliceax1temp2->GetOutput()));

	planeWidgetZ5->UserControlledLookupTableOn();
	planeWidgetZ5->SetLookupTable(bwLutaxial);
	planeWidgetZ5->SetPlaneOrientationToZAxes();
	planeWidgetZ5->SetSliceIndex(0);//theslice
	vtkProperty *prop4 = planeWidgetZ5->GetPlaneProperty();
	prop4->SetColor(1, 0, 0);
	planeWidgetZ5->SetInteractor( renWin->GetInteractor() );
	planeWidgetZ5->On();

	
	int currentpos=0;
	slicetoshow = theslice[currentpos];
	CSpinButtonCtrl *pSpin = reinterpret_cast<CSpinButtonCtrl *>(GetDlgItem(IDC_SPINslice)); 
	pSpin->SetRange32(currentpos,currentpos+num_axslices-1);
	pSpin->SetPos32(currentpos);
	pSpin->UpdateData(TRUE);

	CEdit *Display;
	char tttt[4];
	sprintf(tttt,"%d",slicetoshow);
	Display = reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDITspin));
	CStringW tempme(tttt);
	LPCTSTR tt3 = (LPCTSTR)tempme;
	Display->SetWindowTextW(tt3);



		//read the mri registered tps points cause serve for locating slice
	/*vtkPoints *mrislicepoints = vtkPoints::New();
	std::ifstream mri_tps_landmarks( "tps_landmarks.txt");
	mri_tps_landmarks.seekg(0, ios::beg);

	for(int thetup = 0; thetup< 12; thetup++)
		{
			mri_tps_landmarks.getline(temp_string_wrl,200); 
			sscanf (temp_string_wrl,"%f%f%f", &p1, &p2,&p3);
		mrislicepoints->InsertNextPoint(p1,p2,p3);
		}
	mri_tps_landmarks.close();
	mriSlicePdata = vtkPolyData::New();
	mriSlicePdata->SetPoints(mrislicepoints);

		vtkSphereSource *sphere = vtkSphereSource::New();
		sphere->SetRadius(3);

		vtkGlyph3D *glyph2 = vtkGlyph3D::New();
		glyph2->SetInput(mriSlicePdata);
		glyph2->SetSource(sphere->GetOutput());
		glyph2->SetScaleModeToScaleByScalar();
		vtkPolyDataMapper *spikeMapper2 = vtkPolyDataMapper::New();
		spikeMapper2->SetInput(glyph2->GetOutput());
		spikeActorslice= vtkActor::New();
		spikeActorslice->SetMapper(spikeMapper2);
		spikeActorslice->GetProperty()->SetColor(0.0,0.0,1.0); //green is topo
		aRenderer->AddActor(spikeActorslice);*/

	/*vtkPolyDataMapper *polyDataMapper2 = vtkPolyDataMapper::New();
		polyDataMapper2->SetInput(polydatawrlmeri1);
		vtkActor *mrivert1Actor=vtkActor::New();
		mrivert1Actor->SetMapper( polyDataMapper2 );
		mrivert1Actor->GetProperty()->SetAmbientColor(0.0,1.0,0.0);
		mrivert1Actor->GetProperty()->SetAmbient(1);
		mrivert1Actor->GetProperty()->SetOpacity(0.8);
		polyDataMapper2->Delete();
		aRenderer->AddActor(mrivert1Actor);

			vtkPolyDataMapper *polyDataMapper3 = vtkPolyDataMapper::New();
		polyDataMapper3->SetInput(polydatawrlmeri2);
		vtkActor *mrivert2Actor=vtkActor::New();
		mrivert2Actor->SetMapper( polyDataMapper3 );
		mrivert2Actor->GetProperty()->SetAmbientColor(0.0,1.0,1.0);
		mrivert2Actor->GetProperty()->SetAmbient(1);
		mrivert2Actor->GetProperty()->SetOpacity(0.8);
		polyDataMapper3->Delete();
		aRenderer->AddActor(mrivert2Actor);*/

		showmrivertlandmarks();	
		showMRIverts();
	topocheck.EnableWindow(TRUE);
	topocheck.SetCheck(1);
	extlandcheck.EnableWindow(TRUE);
	extlandcheck.SetCheck(1);
	


	this->showtopo();
//	showtopolandmarks();

	xraycheck.EnableWindow(TRUE);
	xraycheck.SetCheck(1);
	extlandcheck.EnableWindow(TRUE);
	extlandcheck.SetCheck(1);
	//showxray();
	//showxraylandmarks();
	showxrayvertlandmarks();
	
//	aRenderer->AddActor(axialImageActor);

	//double e[3];
	//aRenderer->DisplayToWorld();
	//aRenderer->GetWorldPoint(e);
	/*
renWin->GetEventPosition(e);
> > renderer2->SetDisplayPoint(e[0], e[1], 0);
> > renderer2->DisplayToWorld();

	
	vtkRenderer::SetDisplayPoint(pd);
vtkRenderer::DisplayToWorld();
vtkRenderer::GetWorldPoint(pw);*/
	aRenderer->Render();
	refreshscreen();
}


void Cfull_registration_dialogDlg::OnBnClickedButtonstats()
{
	//int curr_slice =edtNumSlice;//13;
	 int curr_slice = GetDlgItemInt(IDC_EDITregslice, NULL, TRUE);
	char sliceme[3];
	char sliceme2[3];
	char TPslicetps[200], TPslicerigid[200], TPsliceart[200], MRIslicetps[200], MRIslicerigid[200], MRIsliceart[200], patnname[200];
	char patnameMRI[200], patnameTP[200], patnname2[200];
	std::sprintf(sliceme,"%d",curr_slice);
	std::sprintf(sliceme2,"%d",curr_slice);
	strcpy(patnname,"D:/patient_data_testing/modele_complet/1734048/registerednew/1734048");
	//strcpy(patnname2,"D:/patient_data_testing/modele_complet/2052364/registerednew/2052364b");
	//strcpy(patnname,"D:/patient_data_testing/modele_complet/2052364/registerednew/2052364b");

	strcpy(patnname2,"D:/patient_data_testing/modele_complet/2353729to1734048/registerednew/2353729to1734048");
	//strcpy(patnname2,"D:/patient_data_testing/modele_complet/1734048/registerednew/1734048");

	strcpy(patnameMRI,patnname);
	strcat(patnameMRI,"MRIslice");
	strcat(patnameMRI,sliceme);
	strcpy(patnameTP,patnname);
	//strcat(patnameTP,"TPslice");
	strcat(patnameTP,"MRIslice");
	strcat(patnameTP,sliceme);

	strcpy(TPslicetps,patnname2);
	//strcat(TPslicetps,"TPslice");
	strcat(TPslicetps,"MRIslice");
	strcat(TPslicetps,sliceme2);
	strcat(TPslicetps,"tpsfatseg.ppm");

	strcpy(TPslicerigid,patnname2);
	strcat(TPslicerigid,"TPslice");
	//strcat(TPslicerigid,"MRIslice");
	strcat(TPslicerigid,sliceme2);
	strcat(TPslicerigid,"rigidseg.ppm");

	strcpy(TPsliceart,patnname2);
	strcat(TPsliceart,"TPslice");
	//strcat(TPsliceart,"MRIslice");
	strcat(TPsliceart,sliceme2);
	strcat(TPsliceart,"articulatedseg.ppm");

	strcpy(MRIslicetps,patnname);
	strcat(MRIslicetps, "MRIslice");
	strcat(MRIslicetps,sliceme);
	strcat(MRIslicetps,"tpsfatseg.ppm");

	strcpy(MRIslicerigid,patnname);
	strcat(MRIslicerigid, "MRIslice");
	strcat(MRIslicerigid,sliceme);
	strcat(MRIslicerigid,"rigidseg.ppm");

	strcpy(MRIsliceart, patnname);
    strcat(MRIsliceart, "MRIslice");
	strcat(MRIsliceart,sliceme);
	strcat(MRIsliceart,"articulatedseg.ppm");

	//open TP segmentation and get size
	vtkImageData* TPimage = vtkImageData::New();
	TPimage = AffineRegistration::loadPPM(TPslicetps);

	//open MRI segmentation and save in imagedata
	vtkImageData* MRimage = vtkImageData::New();
	MRimage = AffineRegistration::loadPPM(MRIslicetps);

	int imageextent[6];
	MRimage->GetExtent(imageextent);

	int sizex = imageextent[1]+1;
	int sizey = imageextent[3]+1;

	//store the values in array
	vtkFloatArray *scalarsTP = vtkFloatArray::New();
	vtkFloatArray *scalarsMRI = vtkFloatArray::New();
	scalarsTP=  (vtkFloatArray *)TPimage->GetPointData()->GetScalars();
	scalarsMRI=  (vtkFloatArray *)MRimage->GetPointData()->GetScalars();
	//kappa = 2*TP/(2*TP+FP+FN)
	int *numelem = new int[3]; 
	float kappaproposed = AffineRegistration::getkappa(scalarsMRI , scalarsTP, sizex, sizey, numelem  );

	
	
	//now for rigid
	//open TP segmentation and get size
	vtkImageData* TPimagerigid = vtkImageData::New();
	TPimagerigid = AffineRegistration::loadPPM(TPslicerigid);//("debudkappa1.ppm");//
		
	
	//open MRI segmentation and save in imagedata
	vtkImageData* MRimagerigid = vtkImageData::New();
	MRimagerigid = AffineRegistration::loadPPM(MRIslicerigid);//("debudkappa2.ppm");//
	
	int imageextentrigid[6];
	MRimagerigid->GetExtent(imageextentrigid);

	 sizex = imageextentrigid[1]+1;
	 sizey = imageextentrigid[3]+1;

	//store the values in array
	vtkFloatArray *scalarsTPrigid = vtkFloatArray::New();
	vtkFloatArray *scalarsMRIrigid = vtkFloatArray::New();
	scalarsTPrigid=  (vtkFloatArray *)TPimagerigid->GetPointData()->GetScalars();
	scalarsMRIrigid=  (vtkFloatArray *)MRimagerigid->GetPointData()->GetScalars();
	//kappa = 2*TP/(2*TP+FP+FN)
	float kapparigid = AffineRegistration::getkappa(scalarsMRIrigid , scalarsTPrigid, sizex, sizey, numelem);


	//now for articulated
		//open TP segmentation and get size
	vtkImageData* TPimageart = vtkImageData::New();
	TPimageart = AffineRegistration::loadPPM(TPsliceart);

	//open MRI segmentation and save in imagedata
	vtkImageData* MRimageart = vtkImageData::New();
	MRimageart = AffineRegistration::loadPPM(MRIsliceart);
	
	int imageextentart[6];
	MRimageart->GetExtent(imageextentart);

	 sizex = imageextentart[1]+1;
	 sizey = imageextentart[3]+1;

	//store the values in array
	vtkFloatArray *scalarsTPart = vtkFloatArray::New();
	vtkFloatArray *scalarsMRIart = vtkFloatArray::New();
	scalarsTPart=  (vtkFloatArray *)TPimageart->GetPointData()->GetScalars();
	scalarsMRIart=  (vtkFloatArray *)MRimageart->GetPointData()->GetScalars();
	//kappa = 2*TP/(2*TP+FP+FN)
	float kappaart = AffineRegistration::getkappa(scalarsMRIart , scalarsTPart, sizex, sizey, numelem );



	char themessproposed[20], themessrigid[20], themessart[20], final[200]; 
	sprintf(themessrigid,"%f",kapparigid);
	sprintf(themessart,"%f",kappaart);
	sprintf(themessproposed,"%f",kappaproposed);
	strcpy(final, "rigid= ");
	strcat(final,themessrigid); 
	strcat(final, " articulated= ");
	strcat(final,themessart); 
	strcat(final, " proposed= ");
	strcat(final,themessproposed); 
	//creat a popup window with the kappa value
	//AfxMessageBox(_T(themess));
	CString yoyo(final);//themessrigid);
	//LPCSTR yaya = (LPCSTR)yoyo.c_str();
	LPCTSTR yaya2 = (LPCTSTR)yoyo;
	AfxMessageBox(yaya2);

}



void Cfull_registration_dialogDlg::OnBnClickedButtonloadreg2()
{
	// new results, with unparallel transformations
	registered=TRUE;

	vtkMatrix4x4 *volo3[17]; vtkMatrix4x4 *volrep[17];
	vtkMatrix4x4 *vollocaldeftrans2[17];
	vtkMatrix4x4  *volototal[40];
	vtkMatrix4x4 *voloriginmridisplay;
	vtkMatrix4x4 *voloriginmridisplay2;
	vtkMatrix4x4 *volmrifinal[40];
	vtkTransform *totalTrasform[40];

	float themat[9], thetrans[3]={0,0,0};
	//mrimv= 260;
	rigidverts=0;
	// xray and topo as per the usual, the topo should have .ext by now
	unregisteredtopo.polydata_szelandmarks =  vtkPolyData::New ();
	unregisteredtopo.szelandmark_points=vtkPoints::New();
//	unregisteredtopo = Transform::read_landmarks_topo("D:/patient_data_testing/modele_complet/2353729/results/2353729registeredtopo.sze", unregisteredtopo);
	
	int numcomponents = 3;
	//now load xray
	
	//now load mri
	num_mris=0;
	int index_new_vol=0;
	char temp_string_wrl[200];
	char thefilemriland[200], thefilemrivert1[200], thefilemrivert2[200], thefilemrivert3[200];
	char temp_string2[200], thefilemrippm[40][200];
	float matrixorigin[16],  matrixorigin2[16], matrixarticulated[40][16];
	char topo[200], thefilexray[200], xraymarks[200], spinalmarks[200];
	std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/2052364/2052364regReadmesallnew.txt"); 
	//std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/2353729/2353729regReadmesallnew.txt"); 
	//std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/1734048/1734048regReadmesallnew.txt"); 
	
	 if(!numbers_str2)
	 { 
    exit(1);
   } 
	 int num_axslices=0;
	 numbers_str2.seekg(0, ios::beg);
	 /////Get the new file info included 
	 	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &num_mris);
	 numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, topo);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilexray);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, xraymarks);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, spinalmarks);	
	 /////

	//const char *thefilexray = "D:/patient_data_testing/modele_complet/2353729/2353729/2353729.wrl";
	unregisteredxray.polydata_wrllandmarks =  vtkPolyData::New ();
	unregisteredxray.wrllandmark_points=vtkPoints::New();
	unregisteredxray = Transform::read_landmarks_xray(xraymarks, unregisteredxray);//"D:/patient_data_testing/modele_complet/2052364/2052364/2052364.o3"
	polydatawrl = Transform::read (thefilexray);
	thexraypeds=Transform::read_spinal_marks_xray8(spinalmarks); //was read_spinal_marks_xray //"D:/patient_data_testing/modele_complet/2052364/2052364/2052364.wrl"
	unregisteredtopo.polydatasze = Transform::read (topo);//"D:/patient_data_testing/modele_complet/2052364/registerednew/2052364registeredtopo.sze");


	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert2);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert3);
	xray2_vertindex=-2; //start at L5
	xray3_vertindex=-2; //start at L4
	xray4_vertindex=-2;
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray2_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray3_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray4_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, reptxfile);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, o3txfile);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemriland);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[0], &matrixorigin[1], &matrixorigin[2], &matrixorigin[3]);//element(0,0), (0,1), (0,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[4], &matrixorigin[5], &matrixorigin[6], &matrixorigin[7]);//element(1,0), (1,1), (1,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[8], &matrixorigin[9], &matrixorigin[10], &matrixorigin[11]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[12], &matrixorigin[13], &matrixorigin[14], &matrixorigin[15]);
numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin2[0], &matrixorigin2[1], &matrixorigin2[2], &matrixorigin2[3]);//element(0,0), (0,1), (0,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin2[4], &matrixorigin2[5], &matrixorigin2[6], &matrixorigin2[7]);//element(1,0), (1,1), (1,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin2[8], &matrixorigin2[9], &matrixorigin2[10], &matrixorigin2[11]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin2[12], &matrixorigin2[13], &matrixorigin2[14], &matrixorigin2[15]);

	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d",temp_string2, &num_axslices, &index_new_vol);

	mri_dimz = 	 num_axslices;
	for (int imageindex = 0; imageindex < num_axslices; imageindex++)
	{
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrippm[imageindex]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange1[0], &mrirange1[1], &mri_intensity1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%d",temp_string2, &mri_spacing[0], &mri_spacing[1], &mri_spacing[2], &theslice[imageindex]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][0], &matrixarticulated[imageindex][1], &matrixarticulated[imageindex][2], &matrixarticulated[imageindex][3]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][4], &matrixarticulated[imageindex][5], &matrixarticulated[imageindex][6], &matrixarticulated[imageindex][7]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][8], &matrixarticulated[imageindex][9], &matrixarticulated[imageindex][10], &matrixarticulated[imageindex][11]);
		numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][12], &matrixarticulated[imageindex][13], &matrixarticulated[imageindex][14], &matrixarticulated[imageindex][15]); //matrixarticulated

	}
	numbers_str2.close();

	std::ifstream numbers_strtxo3( o3txfile );
	numbers_strtxo3.seekg(0, ios::beg);
	for(int i=0;i<17;i++)
	{
		volo3[i]= vtkMatrix4x4::New();
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volo3[i]->SetElement(0,3,thetrans[0]); 
		volo3[i]->SetElement(1,3,thetrans[1]); 
		volo3[i]->SetElement(2,3,thetrans[2]);
		volo3[i]->SetElement(3,3,1);
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);
		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volo3[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}

	}
	numbers_strtxo3.close();
	std::ifstream numbers_strtxrep( reptxfile );
	numbers_strtxrep.seekg(0, ios::beg);
	//load mri tx, 1st is T1 i think
	for(int i=0;i<17;i++)
	{
		volrep[i]= vtkMatrix4x4::New();
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volrep[i]->SetElement(0,3,thetrans[0]); 
		volrep[i]->SetElement(1,3,thetrans[1]); 
		volrep[i]->SetElement(2,3,thetrans[2]);
		volrep[i]->SetElement(3,3,1);
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);

		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volrep[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}
		volrep[i]->Invert();
	}

	for(int gg=0;gg<17;gg++)
	{
		mrivert_regvol[gg] = vtkMatrix4x4::New();
		mrivert_regvol[gg]->Identity(); //start it with L5
		mrivert_regvol[gg]->Multiply4x4(volo3[gg],volrep[gg],mrivert_regvol[gg]); //i guess starts with L5 
		//so vol03 o volrep 1st tx after
	}

	numbers_strtxrep.close();


	vtkMatrix4x4 *volmriarticulated1[17];
	vtkMatrix4x4 *volmriarticulated2[17];
	for (int thth = 0; thth <17;thth++)
	{
		volmriarticulated1[thth]= vtkMatrix4x4::New();
		volmriarticulated1[thth]->Identity();
		volmriarticulated1[thth]->Multiply4x4(mrivert_regvol[thth],volmriarticulated1[thth],volmriarticulated1[thth]); //just a starting point, will remove
		volmriarticulated1[thth]->Invert();

		volmriarticulated2[thth]= vtkMatrix4x4::New();
		volmriarticulated2[thth]->Identity();
		volmriarticulated2[thth]->DeepCopy(volmriarticulated1[thth]);
	}


	//read the verts
	polydatawrl2vert.polydata = new vtkPolyData*[20];
	polydatawrl3vert.polydata = new vtkPolyData*[20];
	polydatawrl4vert.polydata = new vtkPolyData*[20];


	polydatawrl2vert = Transform::read_wrlsep(thefilemrivert1);  //thefilemrivert1
	for(int gg=0;gg<polydatawrl2vert.num_vertebrae;gg++) 
			wrlactor2[gg]=vtkActor::New();

	if (num_mris>1)
	{
		polydatawrl3vert = Transform::read_wrlsep(thefilemrivert2);
		for(int gg=0;gg<polydatawrl3vert.num_vertebrae;gg++)
			wrlactor3[gg]=vtkActor::New();
	}
	if (num_mris>2)
	{
		polydatawrl4vert = Transform::read_wrlsep(thefilemrivert3);
		for(int gg=0;gg<polydatawrl4vert.num_vertebrae;gg++)
			wrlactor4[gg]=vtkActor::New();
	}

	//read the landmarks
	vtkFloatArray *values22 = vtkFloatArray::New();
	vtkPoints *newmrilandmark_pointspart[17];
	float P[3], p1, p2, p3;
	for(int lala=0;lala<17;lala++)	 
		newmrilandmark_pointspart[lala] = vtkPoints::New();

		std::ifstream numbers_str3(thefilemriland); 
			 if(!numbers_str3)
		 exit(1);
			
	 numbers_str3.seekg(0, ios::beg);
	for(int lala=0;lala<17;lala++)	
	{
		for(int theped=0;theped<8;theped++)	
		{
			numbers_str3.getline(temp_string_wrl,200); 
			sscanf (temp_string_wrl,"%f%f%f", &p1, &p2,&p3);
			newmrilandmark_pointspart[lala]->InsertNextPoint(p1,p2,p3); //starts with T1
		}
	}
	numbers_str3.close();
	
	for(int lala=0;lala<17;lala++)
	{
		mri_newlandmarkspart[lala]=vtkPolyData::New();
		mri_newlandmarkspart[lala]->SetPoints(newmrilandmark_pointspart[lala]); //bad name but ok
	}
	

	//read the image
	mri_dimx=312; mri_dimy=448;
	v18ax =  vtkSmartPointer<vtkVolume16Reader>::New();
	v18ax->SetDataDimensions (mri_dimx,mri_dimy);//(64, 64);//
	v18ax->SetImageRange(1,28);//(mrirange_axial1[0],mrirange_axial1[1]); //
	v18ax->SetDataByteOrderToLittleEndian(); 
	v18ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2052364\\axial_lower\\raw\\2052364");
	//v18ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2353729\\AX_T1_LOWER_0011\\raw\\2353729");
	v18ax->SetDataSpacing(1.116,1.116,12);//(mri_spacing_axial[0],mri_spacing_axial[1],mri_spacing_axial[2]);
	v18ax->Update();

	imageData_ax1 = vtkSmartPointer<vtkImageData>::New();
	imageData_ax1->SetScalarTypeToUnsignedChar();
	imageData_ax1->SetNumberOfScalarComponents(1);
	imageData_ax1->AllocateScalars();
	imageData_ax1->DeepCopy(v18ax->GetOutput());
	imageData_ax1->Update();

	v19ax =  vtkSmartPointer<vtkVolume16Reader>::New();
	v19ax->SetDataDimensions (mri_dimx,mri_dimy);//(64, 64);//
	v19ax->SetImageRange(1,28);//(mrirange_axial1[0],mrirange_axial1[1]); //
	v19ax->SetDataByteOrderToLittleEndian(); 
	//v19ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2353729\\AX_T1_UPPER_0010\\raw\\2353729");
	v19ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2052364\\axial_upper\\raw\\2052364");
	//D:\\patient_data_testing\\modele_complet\\2052364\\axial_upper\\raw\\2052364
	v19ax->SetDataSpacing(1.116,1.116,12);//(mri_spacing_axial[0],mri_spacing_axial[1],mri_spacing_axial[2]);
	v19ax->Update();

	imageData_ax2 = vtkSmartPointer<vtkImageData>::New();
	imageData_ax2->SetScalarTypeToUnsignedChar();
	imageData_ax2->SetNumberOfScalarComponents(1);
	imageData_ax2->AllocateScalars();
	imageData_ax2->DeepCopy(v19ax->GetOutput());
	imageData_ax2->Update();
	
	voloriginmridisplay =  vtkMatrix4x4::New();
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
			voloriginmridisplay->SetElement(i,j,matrixorigin[i*4+j]);
	}

		voloriginmridisplay2 =  vtkMatrix4x4::New();
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
			voloriginmridisplay2->SetElement(i,j,matrixorigin2[i*4+j]);
	}

	vtkTransform *txoriginalaxial=vtkTransform::New();
		txoriginalaxial->Identity();
		txoriginalaxial->PostMultiply();
		txoriginalaxial->SetMatrix(voloriginmridisplay2); //only original tx, inverted
		//txoriginalaxial->Inverse();
		txoriginalaxial->Update();

		tempimage= vtkImageData::New();
		vtkImageReslice *resliceax1temp = vtkImageReslice::New();
		resliceax1temp->SetInput(imageData_ax1);
		resliceax1temp->InterpolateOn();
		resliceax1temp->SetInterpolationModeToCubic();
		resliceax1temp->SetResliceTransform(txoriginalaxial);
		resliceax1temp->SetOutputSpacing(imageData_ax1->GetSpacing());
		resliceax1temp->AutoCropOutputOn();
		resliceax1temp->GetInput()->Update();
		resliceax1temp->Update(); 
		
		tempimage->SetScalarTypeToShort();// 
		tempimage->SetNumberOfScalarComponents(1);
		tempimage->AllocateScalars();
		tempimage->GetPointData()->CopyAllOn();
		tempimage->GetPointData()->CopyScalarsOn();
		tempimage->DeepCopy( resliceax1temp->GetOutput()); 
		tempimage = resliceax1temp->GetOutput();
		tempimage->Update();

		//for the second set of data
		vtkTransform *txoriginalaxial2=vtkTransform::New();
		txoriginalaxial2->Identity();
		txoriginalaxial2->PostMultiply();
		txoriginalaxial2->SetMatrix(voloriginmridisplay); //only original tx, inverted
		//txoriginalaxial->Inverse();
		txoriginalaxial2->Update();

		tempimage2= vtkImageData::New();
		vtkImageReslice *resliceax1temp2 = vtkImageReslice::New();
		resliceax1temp2->SetInput(imageData_ax2);
		resliceax1temp2->InterpolateOn();
		resliceax1temp2->SetInterpolationModeToCubic();
		resliceax1temp2->SetResliceTransform(txoriginalaxial2);
		resliceax1temp2->SetOutputSpacing(imageData_ax2->GetSpacing());
		resliceax1temp2->AutoCropOutputOn();
		resliceax1temp2->GetInput()->Update();
		resliceax1temp2->Update(); 
		
		tempimage2->SetScalarTypeToShort();// 
		tempimage2->SetNumberOfScalarComponents(1);
		tempimage2->AllocateScalars();
		tempimage2->GetPointData()->CopyAllOn();
		tempimage2->GetPointData()->CopyScalarsOn();
		tempimage2->DeepCopy( resliceax1temp2->GetOutput()); 
		tempimage2 = resliceax1temp2->GetOutput();
		tempimage2->Update();

	//store images in image actor
	for (int imageindex = 0; imageindex < num_axslices; imageindex++)
	{
		//right here we need a clause s.t. if we reached the imageindex of the upper volume, load tempimage2 instead of tempimage
		/*if (imageindex == index_new_vol)
		{
			tempimage->SetScalarTypeToShort();// 
			tempimage->SetNumberOfScalarComponents(1);
			tempimage->AllocateScalars();
			tempimage->GetPointData()->CopyAllOn();
			tempimage->GetPointData()->CopyScalarsOn();
			tempimage->DeepCopy( resliceax1temp2->GetOutput()); 
			tempimage = resliceax1temp2->GetOutput();
			tempimage->Update();
		}*/

	volmrifinal[imageindex] = vtkMatrix4x4::New();
	volototal[imageindex]= vtkMatrix4x4::New();

	//when loaded: //element(0,0), (0,1), (0,2), (0,3)
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
		{
		volmrifinal[imageindex]->SetElement(i,j,matrixarticulated[imageindex][i*4+j]);
		}
	}

	//overall transfo
	//volototal[imageindex]->Identity();
	//volototal[imageindex]->Multiply4x4(voloriginmridisplay, volmrifinal[imageindex], volototal[imageindex]);
	//volmrifinal[imageindex]->Invert();
	totalTrasform[imageindex]=vtkTransform::New();
	totalTrasform[imageindex]->SetMatrix(volmrifinal[imageindex]);//(volototal[imageindex]);
	totalTrasform[imageindex]->Update();

	bwLutaxial = vtkLookupTable::New();
	bwLutaxial->SetTableRange(0, mri_intensity1);//100
	bwLutaxial->SetSaturationRange(0,0);
	bwLutaxial->SetHueRange(0,0);
	bwLutaxial->SetValueRange(0,1);
	bwLutaxial->Build(); 

	//load the registered image at the right slice of imagedata
	//use specific volume depending on whether we are upper volume or lower...
		int testextent[6];
	vtkSmartPointer<vtkImageMapToColors> saggitalColors = vtkSmartPointer<vtkImageMapToColors>::New();
	int theextent[6];
	saggitalColors->SetLookupTable(bwLutaxial);
	if (imageindex < index_new_vol) //lower
	{
		tempimage = AffineRegistration::loadPPMvol2(tempimage, thefilemrippm[imageindex], theslice[imageindex]);
		tempimage->GetExtent(theextent);
		saggitalColors->SetInput(tempimage);
	}
	else
	{
		tempimage2 = AffineRegistration::loadPPMvol2(tempimage2, thefilemrippm[imageindex], theslice[imageindex]);
		tempimage2->GetExtent(theextent);
		saggitalColors->SetInput(tempimage2);
	}


	saggitalColors->Update();
	tempimage->GetExtent(testextent);
	axialImageActor2[imageindex] = vtkSmartPointer<vtkImageActor>::New();
	axialImageActor2[imageindex]->SetInput(saggitalColors->GetOutput());
	axialImageActor2[imageindex]->SetUserTransform(totalTrasform[imageindex]);
	axialImageActor2[imageindex]->SetDisplayExtent(0,testextent[1], 0,testextent[3], theslice[imageindex],theslice[imageindex]);
	}

	 aRenderer->AddActor(axialImageActor2[0]);
	
	int currentpos=0;
	slicetoshow = theslice[currentpos];
	CSpinButtonCtrl *pSpin = reinterpret_cast<CSpinButtonCtrl *>(GetDlgItem(IDC_SPINslice)); 
	pSpin->SetRange32(currentpos,currentpos+num_axslices-1);
	pSpin->SetPos32(currentpos);
	pSpin->UpdateData(TRUE);

	CEdit *Display;
	char tttt[4];
	sprintf(tttt,"%d",slicetoshow);
	Display = reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDITspin));
	CStringW tempme(tttt);
	LPCTSTR tt3 = (LPCTSTR)tempme;
	Display->SetWindowTextW(tt3);

	showmrivertlandmarks();	
	showMRIverts();
	this->showtopo();
	showxrayvertlandmarks();

/*	topocheck.EnableWindow(TRUE);
	topocheck.SetCheck(1);
	extlandcheck.EnableWindow(TRUE);
	extlandcheck.SetCheck(1);
	xraycheck.EnableWindow(TRUE);
	xraycheck.SetCheck(1);
	extlandcheck.EnableWindow(TRUE);
	extlandcheck.SetCheck(1);*/
	
	aRenderer->Render();
	refreshscreen();
}


void Cfull_registration_dialogDlg::OnBnClickedVolumekappa()
{
	char sliceme[3];
	char sliceme2[3];
	char TPslicetps[200], TPslicerigid[200], TPsliceart[200], MRIslicetps[200], MRIslicerigid[200], MRIsliceart[200], patnname[200];
	char patnameMRI[200], patnameTP[200], patnname2[200];
	registered=TRUE;
	vtkMatrix4x4 *volo3[17]; vtkMatrix4x4 *volrep[17], *volrepinv[17];
	vtkMatrix4x4 *vollocaldeftrans2[17];
	vtkMatrix4x4  *volototal[40];
	vtkMatrix4x4 *voloriginmridisplay;
	vtkMatrix4x4 *voloriginmridisplay2;
	vtkMatrix4x4 *volmrifinal[40];
	vtkTransform *totalTrasform[40];
	vtkTransform *totalTrasforminvslice[40];
	float themat[9], thetrans[3]={0,0,0};
	//mrimv= 260;
	rigidverts=0;
	
	vtkImageData *imagedata0transfomed2= vtkImageData::New();
	vtkImageData *imagedataindextransfomed= vtkImageData::New();
	 vtkImageData *imagedata0transfomed1slice = vtkImageData::New();

	unregisteredtopo.polydata_szelandmarks =  vtkPolyData::New ();
	unregisteredtopo.szelandmark_points=vtkPoints::New();
	int numcomponents = 3;
	num_mris=0;
	int index_new_vol=0;
	 int num_axslices=0;
	char temp_string_wrl[200];
	char thefilemriland[200], thefilemrivert1[200], thefilemrivert2[200], thefilemrivert3[200];
	char temp_string2[200], thefilemrippm[40][200];
	float matrixorigin[16],  matrixorigin2[16], matrixarticulated[40][16];
	char topo[200], thefilexray[200], xraymarks[200], spinalmarks[200];

	int segmented=0;

	
	//std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/1734048/1734048regReadmekappavol.txt"); 
	std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/2052364/2052364regReadmekappavol.txt"); 
	//std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/2353729/2353729regReadmekappavol.txt"); 
	//strcpy(patnname,"D:/patient_data_testing/modele_complet/kappavol/1734048");
	strcpy(patnname,"D:/patient_data_testing/modele_complet/kappavol/2052364");
	//strcpy(patnname,"D:/patient_data_testing/modele_complet/kappavol/2353729");
	 if(!numbers_str2)
	 { 
		exit(1);
	 } 
	
	 numbers_str2.seekg(0, ios::beg);
	 /////Get the new file info included 
	 numbers_str2.getline(temp_string_wrl,200); 

	 sscanf (temp_string_wrl,"%s%d",temp_string2, &num_mris);
	 numbers_str2.getline(temp_string_wrl,200); 
	 sscanf (temp_string_wrl,"%s%s",temp_string2, topo);	
	 numbers_str2.getline(temp_string_wrl,200); 
	 sscanf (temp_string_wrl,"%s%s",temp_string2, thefilexray);	
	 numbers_str2.getline(temp_string_wrl,200); 
	 sscanf (temp_string_wrl,"%s%s",temp_string2, xraymarks);	
	 numbers_str2.getline(temp_string_wrl,200); 
	 sscanf (temp_string_wrl,"%s%s",temp_string2, spinalmarks);	

	//const char *thefilexray = "D:/patient_data_testing/modele_complet/2353729/2353729/2353729.wrl";
	unregisteredxray.polydata_wrllandmarks =  vtkPolyData::New ();
	unregisteredxray.wrllandmark_points=vtkPoints::New();
	unregisteredxray = Transform::read_landmarks_xray(xraymarks, unregisteredxray);//"D:/patient_data_testing/modele_complet/2052364/2052364/2052364.o3"
	polydatawrl = Transform::read (thefilexray);
	thexraypeds=Transform::read_spinal_marks_xray8(spinalmarks); //was read_spinal_marks_xray //"D:/patient_data_testing/modele_complet/2052364/2052364/2052364.wrl"
	unregisteredtopo.polydatasze = Transform::read (topo);//"D:/patient_data_testing/modele_complet/2052364/registerednew/2052364registeredtopo.sze");

	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert2);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert3);
	xray2_vertindex=-2; //start at L5
	xray3_vertindex=-2; //start at L4
	xray4_vertindex=-2;
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray2_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray3_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray4_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, reptxfile);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, o3txfile);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemriland);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[0], &matrixorigin[1], &matrixorigin[2], &matrixorigin[3]);//element(0,0), (0,1), (0,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[4], &matrixorigin[5], &matrixorigin[6], &matrixorigin[7]);//element(1,0), (1,1), (1,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[8], &matrixorigin[9], &matrixorigin[10], &matrixorigin[11]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[12], &matrixorigin[13], &matrixorigin[14], &matrixorigin[15]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin2[0], &matrixorigin2[1], &matrixorigin2[2], &matrixorigin2[3]);//element(0,0), (0,1), (0,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin2[4], &matrixorigin2[5], &matrixorigin2[6], &matrixorigin2[7]);//element(1,0), (1,1), (1,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin2[8], &matrixorigin2[9], &matrixorigin2[10], &matrixorigin2[11]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin2[12], &matrixorigin2[13], &matrixorigin2[14], &matrixorigin2[15]);

	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d",temp_string2, &num_axslices, &index_new_vol);

	mri_dimz = 	 num_axslices;
	for (int imageindex = 0; imageindex < num_axslices; imageindex++)
	{
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrippm[imageindex]);//put a _seg before the .ppm
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange1[0], &mrirange1[1], &mri_intensity1);	
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%f%f%f%d",temp_string2, &mri_spacing[0], &mri_spacing[1], &mri_spacing[2], &theslice[imageindex]);
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][0], &matrixarticulated[imageindex][1], &matrixarticulated[imageindex][2], &matrixarticulated[imageindex][3]);
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][4], &matrixarticulated[imageindex][5], &matrixarticulated[imageindex][6], &matrixarticulated[imageindex][7]);
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][8], &matrixarticulated[imageindex][9], &matrixarticulated[imageindex][10], &matrixarticulated[imageindex][11]);
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][12], &matrixarticulated[imageindex][13], &matrixarticulated[imageindex][14], &matrixarticulated[imageindex][15]); //matrixarticulated

	}
	numbers_str2.close();

	if (segmented ==0)
	{
	std::ifstream numbers_strtxo3( o3txfile );
	numbers_strtxo3.seekg(0, ios::beg);
	for(int i=0;i<17;i++)
	{
		volo3[i]= vtkMatrix4x4::New();
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volo3[i]->SetElement(0,3,thetrans[0]); 
		volo3[i]->SetElement(1,3,thetrans[1]); 
		volo3[i]->SetElement(2,3,thetrans[2]);
		volo3[i]->SetElement(3,3,1);
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);
		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volo3[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}

	}
	numbers_strtxo3.close();
	std::ifstream numbers_strtxrep( reptxfile );
	numbers_strtxrep.seekg(0, ios::beg);

	//load mri tx, 1st is T1 i think
	for(int i=0;i<17;i++)
	{
		volrep[i]= vtkMatrix4x4::New();
		volrepinv[i]= vtkMatrix4x4::New();
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volrep[i]->SetElement(0,3,thetrans[0]); 
		volrep[i]->SetElement(1,3,thetrans[1]); 
		volrep[i]->SetElement(2,3,thetrans[2]);
		volrep[i]->SetElement(3,3,1);
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);

		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
			{
				volrep[i]->SetElement(ab,bb,themat[ab+bb*3]); 
				volrepinv[i]->SetElement(ab,bb,themat[ab+bb*3]); 
			}
		}
		volrep[i]->Invert();
	}

	for(int gg=0;gg<17;gg++)
	{
		mrivert_regvol[gg] = vtkMatrix4x4::New();
		mrivert_regvol[gg]->Identity(); //start it with L5
		mrivert_regvol[gg]->Multiply4x4(volo3[gg],volrep[gg],mrivert_regvol[gg]); //i guess starts with L5 
		//so vol03 o volrep 1st tx after
	}

	numbers_strtxrep.close();

	vtkMatrix4x4 *volmriarticulated1[17];
	vtkMatrix4x4 *volmriarticulated2[17];
	for (int thth = 0; thth <17;thth++)
	{
		volmriarticulated1[thth]= vtkMatrix4x4::New();
		volmriarticulated1[thth]->Identity();
		volmriarticulated1[thth]->Multiply4x4(mrivert_regvol[thth],volmriarticulated1[thth],volmriarticulated1[thth]); //just a starting point, will remove
		volmriarticulated1[thth]->Invert();

		volmriarticulated2[thth]= vtkMatrix4x4::New();
		volmriarticulated2[thth]->Identity();
		volmriarticulated2[thth]->DeepCopy(volmriarticulated1[thth]);
	}


	//read the verts
	polydatawrl2vert.polydata = new vtkPolyData*[20];
	polydatawrl3vert.polydata = new vtkPolyData*[20];
	polydatawrl4vert.polydata = new vtkPolyData*[20];


	polydatawrl2vert = Transform::read_wrlsep(thefilemrivert1);  //thefilemrivert1
	for(int gg=0;gg<polydatawrl2vert.num_vertebrae;gg++) 
			wrlactor2[gg]=vtkActor::New();

	if (num_mris>1)
	{
		polydatawrl3vert = Transform::read_wrlsep(thefilemrivert2);
		for(int gg=0;gg<polydatawrl3vert.num_vertebrae;gg++)
			wrlactor3[gg]=vtkActor::New();
	}
	if (num_mris>2)
	{
		polydatawrl4vert = Transform::read_wrlsep(thefilemrivert3);
		for(int gg=0;gg<polydatawrl4vert.num_vertebrae;gg++)
			wrlactor4[gg]=vtkActor::New();
	}

	//read the landmarks
	vtkFloatArray *values22 = vtkFloatArray::New();
	vtkPoints *newmrilandmark_pointspart[17];
	float P[3], p1, p2, p3;
	for(int lala=0;lala<17;lala++)	 
		newmrilandmark_pointspart[lala] = vtkPoints::New();

	std::ifstream numbers_str3(thefilemriland); 
	if(!numbers_str3)
		exit(1);

	numbers_str3.seekg(0, ios::beg);
	for(int lala=0;lala<17;lala++)	
	{
		for(int theped=0;theped<8;theped++)	
		{
			numbers_str3.getline(temp_string_wrl,200); 
			sscanf (temp_string_wrl,"%f%f%f", &p1, &p2,&p3);
			newmrilandmark_pointspart[lala]->InsertNextPoint(p1,p2,p3); //starts with T1
		}
	}
	numbers_str3.close();

	for(int lala=0;lala<17;lala++)
	{
		mri_newlandmarkspart[lala]=vtkPolyData::New();
		mri_newlandmarkspart[lala]->SetPoints(newmrilandmark_pointspart[lala]); //bad name but ok
	}

	bwLutaxial = vtkLookupTable::New();
	bwLutaxial->SetTableRange(0, 400);//100
	bwLutaxial->SetSaturationRange(0,0);
	bwLutaxial->SetHueRange(0,0);
	bwLutaxial->SetValueRange(0,1);
	bwLutaxial->Build(); 

	bwLutaxial2 = vtkLookupTable::New();
	bwLutaxial2->SetTableRange(0, mri_intensity1*4);//100
	bwLutaxial2->SetSaturationRange(0,0);
	bwLutaxial2->SetHueRange(0,0);
	bwLutaxial2->SetValueRange(0,1);
	bwLutaxial2->Build(); 

	//read the original lower volume
	mri_dimx=312; mri_dimy=448;
	v18ax =  vtkSmartPointer<vtkVolume16Reader>::New();
	v18ax->SetDataDimensions (mri_dimx,mri_dimy);//(64, 64);//
	v18ax->SetImageRange(1,28);//(mrirange_axial1[0],mrirange_axial1[1]); //
	v18ax->SetDataByteOrderToLittleEndian(); 
	//v18ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\1734048\\axial_lower\\raw\\1734048");
	v18ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2052364\\axial_lower\\raw\\2052364");
	//v18ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2353729\\AX_T1_LOWER_0011\\raw\\2353729");
	v18ax->SetDataSpacing(1.116,1.116,12);//(mri_spacing_axial[0],mri_spacing_axial[1],mri_spacing_axial[2]);
	v18ax->Update();

	imageData_ax1 = vtkSmartPointer<vtkImageData>::New();
	imageData_ax1->SetScalarTypeToUnsignedChar();
	imageData_ax1->SetNumberOfScalarComponents(1);
	imageData_ax1->AllocateScalars();
	imageData_ax1->DeepCopy(v18ax->GetOutput());
	imageData_ax1->Update();

	//read the original upper volume
	v19ax =  vtkSmartPointer<vtkVolume16Reader>::New();
	v19ax->SetDataDimensions (mri_dimx,mri_dimy);//(64, 64);//
	v19ax->SetImageRange(1,28);//(mrirange_axial1[0],mrirange_axial1[1]); //
	v19ax->SetDataByteOrderToLittleEndian(); 
	//v19ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2353729\\AX_T1_UPPER_0010\\raw\\2353729");
	v19ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2052364\\axial_upper\\raw\\2052364");
	//v19ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\1734048\\axial_upper\\raw\\1734048");
	//D:\\patient_data_testing\\modele_complet\\2052364\\axial_upper\\raw\\2052364
	v19ax->SetDataSpacing(1.116,1.116,12);//(mri_spacing_axial[0],mri_spacing_axial[1],mri_spacing_axial[2]);
	v19ax->Update();

	imageData_ax2 = vtkSmartPointer<vtkImageData>::New();
	imageData_ax2->SetScalarTypeToUnsignedChar();
	imageData_ax2->SetNumberOfScalarComponents(1);
	imageData_ax2->AllocateScalars();
	imageData_ax2->DeepCopy(v19ax->GetOutput());
	imageData_ax2->Update();
	
	voloriginmridisplay =  vtkMatrix4x4::New();
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
			voloriginmridisplay->SetElement(i,j,matrixorigin[i*4+j]);
	}

		voloriginmridisplay2 =  vtkMatrix4x4::New();
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
			voloriginmridisplay2->SetElement(i,j,matrixorigin2[i*4+j]);
	}

	/******************************************
	Transform volume to its real 3D location
	******************************************/
	vtkTransform *txoriginalaxial=vtkTransform::New();
	txoriginalaxial->Identity();
	txoriginalaxial->PostMultiply();
	txoriginalaxial->SetMatrix(voloriginmridisplay2); //only original tx, inverted
	//txoriginalaxial->Inverse();
	txoriginalaxial->Update();

	tempimage= vtkImageData::New();
	vtkImageReslice *resliceax1temp = vtkImageReslice::New();
	resliceax1temp->SetInput(imageData_ax1);
	resliceax1temp->InterpolateOn();
	resliceax1temp->SetInterpolationModeToCubic();
	resliceax1temp->SetResliceTransform(txoriginalaxial);
	resliceax1temp->SetOutputSpacing(imageData_ax1->GetSpacing());
	resliceax1temp->AutoCropOutputOn();
	resliceax1temp->GetInput()->Update();
	resliceax1temp->Update(); 
		
	tempimage->SetScalarTypeToShort();// 
	tempimage->SetNumberOfScalarComponents(1);
	tempimage->AllocateScalars();
	tempimage->GetPointData()->CopyAllOn();
	tempimage->GetPointData()->CopyScalarsOn();
	tempimage->DeepCopy( resliceax1temp->GetOutput()); 
	tempimage = resliceax1temp->GetOutput();
	tempimage->Update();

	/******************************************
	Transform 2nd volume to its real 3D location
	******************************************/
	vtkTransform *txoriginalaxial2=vtkTransform::New();
	txoriginalaxial2->Identity();
	txoriginalaxial2->PostMultiply();
	txoriginalaxial2->SetMatrix(voloriginmridisplay); //only original tx, inverted
	//txoriginalaxial->Inverse();
	txoriginalaxial2->Update();

	tempimage2= vtkImageData::New();
	vtkImageReslice *resliceax1temp2 = vtkImageReslice::New();
	resliceax1temp2->SetInput(imageData_ax2);
	resliceax1temp2->InterpolateOn();
	resliceax1temp2->SetInterpolationModeToCubic();
	resliceax1temp2->SetResliceTransform(txoriginalaxial2);
	resliceax1temp2->SetOutputSpacing(imageData_ax2->GetSpacing());
	resliceax1temp2->AutoCropOutputOn();
	resliceax1temp2->GetInput()->Update();
	resliceax1temp2->Update(); 
		
	tempimage2->SetScalarTypeToShort();// 
	tempimage2->SetNumberOfScalarComponents(1);
	tempimage2->AllocateScalars();
	tempimage2->GetPointData()->CopyAllOn();
	tempimage2->GetPointData()->CopyScalarsOn();
	tempimage2->DeepCopy( resliceax1temp2->GetOutput()); 
	tempimage2 = resliceax1temp2->GetOutput();
	tempimage2->Update();
	
	volmrifinal[0] = vtkMatrix4x4::New();
	//when loaded: //element(0,0), (0,1), (0,2), (0,3)
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
		{
		volmrifinal[0]->SetElement(i,j,matrixarticulated[0][i*4+j]);
		}
	}

	/**************************************************************
	First, create a volume along the zeroth slice transformed, 
	this volume will be used to store final values 
	**************************************************************/


	//supersize it, volume of 3mm slices
	double currentspacing2[3];
	double currentspacing[3];
	
	int currentextent[6];
	int currentextent2[6];
	tempimage->GetExtent(currentextent);
	tempimage->GetSpacing(currentspacing);

	int extentimagedata0[6];


	/**********************************************************************************************
	For each slice,  load the registered slice transform it, find edge points and add them to 
	a surface to be generated. Then supersample the surface.
	***********************************************************************************************/
	vtkPoints *overallMRIedgepoints = vtkPoints::New();
	vtkPoints *mriedgepoints=vtkPoints::New();
	vtkPolyData *MRIedgeData = vtkPolyData::New();
	vtkSphereSource *sphere = vtkSphereSource::New();
	sphere->SetRadius(5);

	for (int imageindex = 0; imageindex < num_axslices; imageindex++) //num_axslices
	{
		volmrifinal[imageindex] = vtkMatrix4x4::New();
		volototal[imageindex]= vtkMatrix4x4::New();

		/******************************************************************************
		load the registered image at the right slice of imagedata
		use specific volume depending on whether we are upper volume or lower...
		******************************************************************************/
		int testextent[6];
		vtkSmartPointer<vtkImageMapToColors> saggitalColors = vtkSmartPointer<vtkImageMapToColors>::New();
		int theextent[6];
		saggitalColors->SetLookupTable(bwLutaxial);
		
		if (imageindex < index_new_vol) //lower volume being used
		{
			tempimage = AffineRegistration::loadPPMvol2(tempimage, thefilemrippm[imageindex], theslice[imageindex]);
			tempimage->GetExtent(theextent);
			saggitalColors->SetInput(tempimage);
		}
		else
		{
			tempimage = AffineRegistration::loadPPMvol2(tempimage2, thefilemrippm[imageindex], theslice[imageindex]);
			tempimage->GetExtent(theextent);
			saggitalColors->SetInput(tempimage);
		}


		/***************************************************
		Find edge points for the slice, transform points, 
		using slice transfo, then insert in overall points
		***************************************************/
		//when loaded: //element(0,0), (0,1), (0,2), (0,3)
		for(int i = 0;i<4;i++)
		{
			for(int j = 0;j<4;j++)
			{
				volmrifinal[imageindex]->SetElement(i,j,matrixarticulated[imageindex][i*4+j]);
			}
		}

		//overall transfo for the slice as saved during registration
		totalTrasform[imageindex]=vtkTransform::New();
		totalTrasform[imageindex]->SetMatrix(volmrifinal[imageindex]);//(volototal[imageindex]);
		totalTrasform[imageindex]->Update();

		totalTrasforminvslice[imageindex]=vtkTransform::New();
		totalTrasforminvslice[imageindex]->SetMatrix(volmrifinal[imageindex]);
		totalTrasforminvslice[imageindex]->Inverse();

		int extentimagedataindex[6];
		int tomod = 0;
		vtkFloatArray *scalarsMRI = vtkFloatArray::New();
		tempimage->GetExtent(extentimagedataindex); //
		vtkTransformPolyDataFilter *fvertsurface = vtkTransformPolyDataFilter::New ();
		

		int closest_vert=0, closest_dist2=10000, offset11;
		double xyzCoords[3];
		double *paraCoords = new double[3]; 
		/***********************************
		Find the closest vertebra to the center of the current slice
		***********************************/
			int middlez = (int)((extentimagedataindex[5]+1)/2);
			int middley = (int)((extentimagedataindex[3]+1)/2);
			int middlex = (int)((extentimagedataindex[1]+1)/2);
		  offset11= (int)(((extentimagedataindex[3]+1)*(extentimagedataindex[1]+1)*theslice[imageindex])+(extentimagedataindex[1]+1)*middley+middlex);
		int subID = tempimage->GetCell( offset11 )->GetParametricCenter(paraCoords); //before rigid registration 
		double *bounds = new double[6];	
		tempimage->GetCell(offset11)->GetBounds(bounds); //before rigid registration
		xyzCoords[0]=bounds[0];xyzCoords[1]=bounds[2];xyzCoords[2]=bounds[4];

		for(int hhh=0;hhh<17;hhh++) //starting at L5
		{
			//float tempdist=abs(xyzCoords[2]-volo3[hhh]->GetElement(2,3));//was 16-hhh
			float tempdist=abs(xyzCoords[2]-volrepinv[hhh]->GetElement(2,3));
			if(tempdist<closest_dist2)
			{
				closest_vert=hhh; //13 = L2, 17 being L5, that makes no sense sine=ce overall 0 is L5... NEW: L4=1, so, good!
				closest_dist2=tempdist;
			}
		}

		//find edge data
		///could have used bool AffineRegistration::GetMRIEdgePoints(vtkImageData* movingimage, int slicenumber, vtkMatrix4x4 *XrayVert, float **anglepoints,vtkPoints *mriEdgePoints, const char *filename)
		mriSlicePdata = vtkPolyData::New();
		AffineRegistration::GenerateonlyMRIContourPoints(tempimage, theslice[imageindex],volrepinv[closest_vert],mrivert_regvol[closest_vert], mriSlicePdata, thefilemrippm[imageindex]);

		//transform the edge data
		/****************************************************************
		transform the points  (supposed to be where>0 in right slice
		using the same transformation as the image actor
		****************************************************************/
		fvertsurface->SetInput (mriSlicePdata); //original
		fvertsurface->SetTransform (totalTrasform[imageindex]);//(totalTrasforminvslice);//
		fvertsurface->Update ();
		mriSlicePdata=fvertsurface->GetOutput();
		mriSlicePdata->DeepCopy(fvertsurface->GetOutput()); //the points are at the right slice after transformation

		//insert edge data into overall points
		double worldcenter[3];
		vtkPoints *transformedpoints = mriSlicePdata->GetPoints();
		for (int iii = 0; iii< transformedpoints->GetNumberOfPoints(); iii++)
		{
			transformedpoints->GetPoint(iii, worldcenter);
			mriedgepoints->InsertNextPoint(worldcenter[0],worldcenter[1],worldcenter[2]);
		}


		saggitalColors->Update();
		tempimage->GetExtent(testextent);
		axialImageActor2[imageindex] = vtkSmartPointer<vtkImageActor>::New();
		axialImageActor2[imageindex]->SetInput(saggitalColors->GetOutput());
		axialImageActor2[imageindex]->SetUserTransform(totalTrasform[imageindex]);
		axialImageActor2[imageindex]->SetDisplayExtent(0,testextent[1], 0,testextent[3], theslice[imageindex],theslice[imageindex]);
	}
	
		MRIedgeData->SetPoints(mriedgepoints);
		//or set neighbours as one before and one above
		vtkCellArray *modelcell = vtkCellArray::New ();
		
		//vtkFloatArray *edgepointsArray = vtkFloatArray::New();
		//edgepointsArray = (vtkFloatArray *)(mriedgepoints->GetPoint);

		 for (int i = 0; i < (num_axslices-1); i++) 
		 {
			 for (int j = 0; j < 35; j++) 
			 {
   				//the polygon is point[12*i+j], point[12*(i+1)+j] (up), point[12*i+j+1] (next) 
                modelcell->InsertNextCell (3);

				modelcell->InsertCellPoint(36*i+j);///mriedgepoints->GetPoint(12*i+j)); //(point[12*i+j]);
				modelcell->InsertCellPoint (36*(i+1)+j);
				modelcell->InsertCellPoint (36*i+(j+1)%35);

				if(i>0)
				{
				modelcell->InsertNextCell(3);

				modelcell->InsertCellPoint(36*i+j);
				modelcell->InsertCellPoint (36*(i-1)+j);
				if(j>0)
					modelcell->InsertCellPoint (36*i+j-1);
				else //connect 0 and 34
					modelcell->InsertCellPoint (36*i+34);

				}
			}
		 }

		MRIedgeData->SetPolys(modelcell);//szepolydata_modelaffine->GetPolys());

		//generate an overall volume from the edgepoints
		vtkTriangleFilter *triangulate = vtkTriangleFilter::New ();
  triangulate->SetInput (MRIedgeData);
  triangulate->PassLinesOn ();
  triangulate->Update (); //perfect

  /****************************************************************************
  supersample the data, then use it to generate slices 
  in the same way as original topo
  ***************************************************************************/

     vtkLinearSubdivisionFilter *subdivisionFilter = vtkLinearSubdivisionFilter::New();
	 subdivisionFilter->SetInput(triangulate->GetOutput());
	 subdivisionFilter->SetNumberOfSubdivisions(1);
	 subdivisionFilter->Update();

	 vtkPolyData *MRIsurface = vtkPolyData::New();
	 MRIsurface =  subdivisionFilter->GetOutput();
	 vtkPolyData *topoSlicePdata = vtkPolyData::New();

	 /************************************
	 For each level of the overall MRI, 3mm appart,
	 get MRI and topo slices
	 ***********************************/ 
	 	vtkTransform	*totalTrasforminv=vtkTransform::New();
	totalTrasforminv->SetMatrix(volmrifinal[0]);//(volototal[imageindex]);
	totalTrasforminv->Inverse();

	 for(int sliceindex=0; sliceindex < 0;sliceindex++) //extentimagedata0[5] //num_axslices
	 {

		 		vtkImageReslice *reslice= vtkImageReslice::New();
				reslice->SetInput(tempimage);
				reslice->InterpolateOn();
				reslice->SetInterpolationModeToCubic();
				reslice->SetResliceTransform(totalTrasforminvslice[sliceindex]);//(totalTrasforminv); // wrong transform
				//reslice->SetOutputSpacing(currentspacing[0], currentspacing[1], currentspacing[2]/4);
				//reslice->SetOutputExtent(0, currentextent[1], 0, currentextent[3], 0, currentextent[5]*4);
				reslice->AutoCropOutputOn();
				reslice->GetInput()->Update();
				reslice->Update(); 

				vtkImageData *imagedata0transfomed= vtkImageData::New();
				imagedata0transfomed->SetScalarTypeToShort();// 
				imagedata0transfomed->SetNumberOfScalarComponents(1);
				imagedata0transfomed->AllocateScalars();
				imagedata0transfomed->GetPointData()->CopyAllOn();
				imagedata0transfomed->GetPointData()->CopyScalarsOn();
				imagedata0transfomed->DeepCopy( reslice->GetOutput()); 
				imagedata0transfomed = reslice->GetOutput();
				imagedata0transfomed->Update();

				imagedata0transfomed->GetExtent(currentextent2);
				imagedata0transfomed->GetSpacing(currentspacing2);

/*				vtkImageReslice *reslice2= vtkImageReslice::New();
				reslice2->SetInput(imagedata0transfomed);
				reslice2->InterpolateOn();
				reslice2->SetInterpolationModeToCubic();
				reslice->SetOutputSpacing(currentspacing2[0], currentspacing2[1], currentspacing2[2]/4);
				reslice->SetOutputExtent(0, currentextent2[1], 0, currentextent2[3], 0, currentextent2[5]*4);
				reslice2->AutoCropOutputOn();
				reslice2->GetInput()->Update();
				reslice2->Update(); 

				
				imagedata0transfomed2->SetScalarTypeToShort();// 
				imagedata0transfomed2->SetNumberOfScalarComponents(1);
				imagedata0transfomed2->AllocateScalars();
				imagedata0transfomed2->GetPointData()->CopyAllOn();
				imagedata0transfomed2->GetPointData()->CopyScalarsOn();
				imagedata0transfomed2->DeepCopy( reslice2->GetOutput()); 
				imagedata0transfomed2 = reslice2->GetOutput();
				imagedata0transfomed2->Update();

				imagedata0transfomed->Delete();*/

				/***********************************
		Find the closest vertebra to the center of the current slice
		***********************************/
		 		int closest_vert=0, closest_dist2=10000, offset11;
		double xyzCoords[3];
		double *paraCoords = new double[3]; 
		int extentimagedataindex[6];
		imagedata0transfomed->GetExtent(extentimagedataindex);
			int middlez = (int)((extentimagedataindex[5]+1)/2);
			int middley = (int)((extentimagedataindex[3]+1)/2);
			int middlex = (int)((extentimagedataindex[1]+1)/2);

		  offset11= (int)(((extentimagedataindex[3]+1)*(extentimagedataindex[1]+1)*theslice[sliceindex])+(extentimagedataindex[1]+1)*middley+middlex);
		int subID = tempimage->GetCell( offset11 )->GetParametricCenter(paraCoords); //before rigid registration 
		double *bounds = new double[6];	
		tempimage->GetCell(offset11)->GetBounds(bounds); //before rigid registration
		xyzCoords[0]=bounds[0];xyzCoords[1]=bounds[2];xyzCoords[2]=bounds[4];

		for(int hhh=0;hhh<17;hhh++) //starting at L5
		{
			//float tempdist=abs(xyzCoords[2]-volo3[hhh]->GetElement(2,3));//was 16-hhh
			float tempdist=abs(xyzCoords[2]-volrepinv[hhh]->GetElement(2,3));
			if(tempdist<closest_dist2)
			{
				closest_vert=hhh; //13 = L2, 17 being L5, that makes no sense sine=ce overall 0 is L5... NEW: L4=1, so, good!
				closest_dist2=tempdist;
			}
		}

				/********************************************
				Now each image index is responsible for 4 slices, so save all those
				********************************************/

				for(int subsliceindex = 0; subsliceindex<4; subsliceindex++)
				{
					int tosave_index = sliceindex*4+subsliceindex;
	vtkImageReslice *reslice1slice= vtkImageReslice::New();
	reslice1slice->SetInput(imagedata0transfomed);
	reslice1slice->InterpolateOn();
	reslice1slice->SetInterpolationModeToCubic();
	reslice1slice->SetOutputSpacing(currentspacing2[0], currentspacing2[1], currentspacing2[2]/4);
	reslice1slice->SetOutputExtent(0, currentextent2[1], 0, currentextent2[3],tosave_index, tosave_index);
	reslice1slice->AutoCropOutputOn();
	reslice1slice->GetInput()->Update();
	reslice1slice->Update(); 

	
	imagedata0transfomed1slice->SetScalarTypeToShort();// 
	imagedata0transfomed1slice->SetNumberOfScalarComponents(1);
	imagedata0transfomed1slice->AllocateScalars();
	imagedata0transfomed1slice->GetPointData()->CopyAllOn();
	imagedata0transfomed1slice->GetPointData()->CopyScalarsOn();
	imagedata0transfomed1slice->DeepCopy( reslice1slice->GetOutput()); 
	//imagedata0transfomed1slice->SetExtent(0, currentextent2[1], 0, currentextent2[3],0, 0);
	imagedata0transfomed1slice = reslice1slice->GetOutput();
	imagedata0transfomed1slice->Update();

		 std::sprintf(sliceme,"%d",tosave_index);

		strcpy(TPsliceart,patnname);
		strcat(TPsliceart,"TPslice");
		strcat(TPsliceart,sliceme);
		strcat(TPsliceart,"volkappa.ppm");

		vtkMatrix4x4 *identityMatrix = vtkMatrix4x4::New();
		identityMatrix->Identity();
		AffineRegistration::GenerateTPslice(topoSlicePdata,unregisteredtopo.polydatasze, imagedata0transfomed1slice, 0, identityMatrix, TPsliceart );

	 //transform the MRI volume in the opposite way that it is done in the MRI slice generation code
	 	vtkTransform *noregtopotx = vtkTransform::New();
		noregtopotx->RotateZ(-90); 
		noregtopotx->RotateX(90); 
		noregtopotx->Update();

	 	vtkTransform *noregtopotxinv = vtkTransform::New();
		noregtopotxinv->RotateZ(-90); 
		noregtopotxinv->RotateX(90); 
		noregtopotxinv->Inverse();
		noregtopotxinv->Update();

		vtkTransformPolyDataFilter *fpre = vtkTransformPolyDataFilter::New ();
		fpre->SetInput (MRIsurface);
		fpre->SetTransform (noregtopotxinv);
		fpre->Update ();
		vtkPolyData *tildtedMRIsurface=vtkPolyData::New();
		tildtedMRIsurface	=fpre->GetOutput();

		//generate slice
				strcpy(MRIsliceart,patnname);
		strcat(MRIsliceart,"MRIslice");
		strcat(MRIsliceart,sliceme);
		strcat(MRIsliceart,"volkappa.ppm");
		AffineRegistration::GenerateTPslice(topoSlicePdata,tildtedMRIsurface, imagedata0transfomed1slice, 0, identityMatrix, MRIsliceart );

				}
	 }


  vtkExtractEdges *extract = vtkExtractEdges::New();
  extract->SetInput(subdivisionFilter->GetOutput()); //(triangulate->GetOutput ());
vtkPolyData *meshes = extract->GetOutput();
vtkPolyData *polydataszeshow2 = vtkPolyData::New();
polydataszeshow2 = extract->GetOutput();



	vtkPolyDataMapper *polyDataMapper = vtkPolyDataMapper::New();
	polyDataMapper->SetInput(polydataszeshow2);

  //show the surface generated
//	polyDataMapper->SetInput(triangulate->GetOutput ());
	vtkActor *szeactor2 = vtkActor::New();
	szeactor2->GetProperty()->SetAmbientColor(1.0,0.0,0.0);
	szeactor2->SetMapper( polyDataMapper );
	polyDataMapper->Delete();
	aRenderer->AddActor(szeactor2);

		//the points are at the 8th slice of imagedataindextransformed, but not at the right 3D place
		//is the 8th slice of imagedataindextransformed the wrong thing? Seems so. 12th slice fits well
		///the points are now at the right slice.
		vtkGlyph3D *glyph3 = vtkGlyph3D::New();
		glyph3->SetInput(MRIedgeData);//(MRIedgeData);//mriSlicePdata//MRIedgeData//regmriSlicePdata//closestpolys
		glyph3->SetSource(sphere->GetOutput());
		glyph3->SetScaleModeToScaleByScalar();
		vtkPolyDataMapper *spikeMapper3 = vtkPolyDataMapper::New();
		spikeMapper3->SetInput(glyph3->GetOutput());
		spikeActorslice2= vtkActor::New();
		spikeActorslice2->SetMapper(spikeMapper3);
		spikeActorslice2->GetProperty()->SetColor(1.0,0.0,0.0);//red is mri



	aRenderer->AddActor(spikeActorslice2);

	//imagedataindextransfomed places the slice at the right place.. imagedata0transfomed maps the wrong thing to the wrong place
	//does it?
	imagedata0transfomed1slice = AffineRegistration::loadPPMvol2(imagedata0transfomed1slice, MRIsliceart, 0);
			vtkSmartPointer<vtkImageMapToColors> saggitalColors2 = vtkSmartPointer<vtkImageMapToColors>::New();
		saggitalColors2->SetLookupTable(bwLutaxial);
		saggitalColors2->SetInput(tempimage);//(imagedata0transfomed1slice);
		saggitalColors2->Update();
	planeWidgetZ4= vtkImagePlaneWidget::New();
	planeWidgetZ4->DisplayTextOn();
	planeWidgetZ4->SetInput(imagedata0transfomed1slice);//(imagedataindextransfomed);//(imagedata0transfomed);
	planeWidgetZ4->UserControlledLookupTableOn();
	planeWidgetZ4->SetLookupTable(bwLutaxial);
	planeWidgetZ4->SetPlaneOrientationToZAxes();
	//planeWidgetZ4->SetSliceIndex(0);
	//planeWidgetZ4->SetPicker(picker);
	//planeWidgetZ4->SetKeyPressActivationValue ('c');
	vtkProperty *prop3 = planeWidgetZ4->GetPlaneProperty();
	prop3->SetColor(0, 0, 0);

	//good
	planeWidgetZ4->SetInteractor( renWin->GetInteractor() );
	planeWidgetZ4->On();
	
		imagedata0transfomed1slice->GetExtent(currentextent2);
		axialImageActor2[0] = vtkSmartPointer<vtkImageActor>::New();
		axialImageActor2[0]->SetInput(saggitalColors2->GetOutput());
		axialImageActor2[0]->SetUserTransform(totalTrasform[0]);
		//axialImageActor2[0]->SetDisplayExtent(0,currentextent2[1], 0,currentextent2[3], theslice[0],theslice[0]);
		axialImageActor2[0]->SetDisplayExtent(0,currentextent2[1], 0,currentextent2[3], 0,0);
		//	aRenderer->AddActor(axialImageActor2[0]); //is creating deadlock
	int currentpos=0;
	slicetoshow = theslice[currentpos];
	CSpinButtonCtrl *pSpin = reinterpret_cast<CSpinButtonCtrl *>(GetDlgItem(IDC_SPINslice)); 
	pSpin->SetRange32(currentpos,currentpos+num_axslices-1);
	pSpin->SetPos32(currentpos);
	pSpin->UpdateData(TRUE);

CEdit *Display;
	char tttt[4];
/*		sprintf(tttt,"%d",slicetoshow);
	Display = reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDITspin));
	CStringW tempme(tttt);
	LPCTSTR tt3 = (LPCTSTR)tempme;
	Display->SetWindowTextW(tt3);*/

	//showmrivertlandmarks();	
	showMRIverts();
	this->showtopo();
 	//showxrayvertlandmarks();

	/*	topocheck.EnableWindow(TRUE);
	topocheck.SetCheck(1);
	extlandcheck.EnableWindow(TRUE);
	extlandcheck.SetCheck(1);
	xraycheck.EnableWindow(TRUE);
	xraycheck.SetCheck(1);
	extlandcheck.EnableWindow(TRUE);
	extlandcheck.SetCheck(1);*/

	aRenderer->Render();
	refreshscreen();
	}
	else //segmentations done so calculate kappa over the entire volume at once
	{
		int TP = 0, FP=0, FN=0;
		//open a file to spew out per slice kappa values
		char kapparesults[200];
		strcpy(kapparesults,patnname);
		strcat(kapparesults,"volumekappas");
		strcat(kapparesults,sliceme);
		strcat(kapparesults,".txt");

		std::ofstream MRIoutput(kapparesults,std::ios::app); 

		for(int sliceindex=0; sliceindex < (num_axslices*4);sliceindex++)//num_axslices
		{
			 std::sprintf(sliceme,"%d",sliceindex);
		strcpy(MRIsliceart,patnname);
		strcat(MRIsliceart,"MRIslice");
		strcat(MRIsliceart,sliceme);
		strcat(MRIsliceart,"volkappaseg.ppm");
	
		strcpy(TPsliceart,patnname);
		strcat(TPsliceart,"TPslice");
		strcat(TPsliceart,sliceme);
		strcat(TPsliceart,"volkappaseg.ppm");

		//open TP segmentation and get size
	vtkImageData* TPimageart = vtkImageData::New();
	TPimageart = AffineRegistration::loadPPM(TPsliceart);

	//open MRI segmentation and save in imagedata
	vtkImageData* MRimageart = vtkImageData::New();
	MRimageart = AffineRegistration::loadPPM(MRIsliceart);

	int imageextentart[6];
	MRimageart->GetExtent(imageextentart);

	int sizex = imageextentart[1]+1;
	int sizey = imageextentart[3]+1;

	//store the values in array
	vtkFloatArray *scalarsTPart = vtkFloatArray::New();
	vtkFloatArray *scalarsMRIart = vtkFloatArray::New();
	scalarsTPart=  (vtkFloatArray *)TPimageart->GetPointData()->GetScalars();
	scalarsMRIart=  (vtkFloatArray *)MRimageart->GetPointData()->GetScalars();
	//kappa = 2*TP/(2*TP+FP+FN)
	int *numelem = new int[3];
	float kappaart = AffineRegistration::getkappa(scalarsMRIart , scalarsTPart, sizex, sizey, numelem );

	//save the value of kappa in a txt file for each subslice
	MRIoutput <<kappaart<<endl;

	TP += numelem[0];
	FP += numelem[1];
	FN +=  numelem[2];
	
		}
		MRIoutput.close();

		float thekappa = (float)(2*TP)/((float)(2*TP+ FP + FN));

	char themessproposed[20], themessrigid[20], themessart[20], final[200]; 
	sprintf(themessproposed,"%f",thekappa);
	strcpy(final, " proposed volume kappa= ");
	strcat(final,themessproposed); 
	//creat a popup window with the kappa value
	//AfxMessageBox(_T(themess));
	CString yoyo(final);//themessrigid);
	//LPCSTR yaya = (LPCSTR)yoyo.c_str();
	LPCTSTR yaya2 = (LPCTSTR)yoyo;
	AfxMessageBox(yaya2);
}



}


void Cfull_registration_dialogDlg::OnBnClickedVolumekappabackkkk()
{
	char sliceme[3];
	char sliceme2[3];
	char TPslicetps[200], TPslicerigid[200], TPsliceart[200], MRIslicetps[200], MRIslicerigid[200], MRIsliceart[200], patnname[200];
	char patnameMRI[200], patnameTP[200], patnname2[200];
	registered=TRUE;
	vtkMatrix4x4 *volo3[17]; vtkMatrix4x4 *volrep[17];
	vtkMatrix4x4 *vollocaldeftrans2[17];
	vtkMatrix4x4  *volototal[40];
	vtkMatrix4x4 *voloriginmridisplay;
	vtkMatrix4x4 *voloriginmridisplay2;
	vtkMatrix4x4 *volmrifinal[40];
	vtkTransform *totalTrasform[40];
	float themat[9], thetrans[3]={0,0,0};
	//mrimv= 260;
	rigidverts=0;
	vtkImageData *imagedata0transfomed= vtkImageData::New();
	vtkImageData *imagedataindextransfomed= vtkImageData::New();

	unregisteredtopo.polydata_szelandmarks =  vtkPolyData::New ();
	unregisteredtopo.szelandmark_points=vtkPoints::New();
	int numcomponents = 3;
	num_mris=0;
	int index_new_vol=0;
	 int num_axslices=0;
	char temp_string_wrl[200];
	char thefilemriland[200], thefilemrivert1[200], thefilemrivert2[200], thefilemrivert3[200];
	char temp_string2[200], thefilemrippm[40][200];
	float matrixorigin[16],  matrixorigin2[16], matrixarticulated[40][16];
	char topo[200], thefilexray[200], xraymarks[200], spinalmarks[200];


	std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/2052364/2052364regReadmekappavol.txt"); 
	//std::ifstream numbers_str2("D:/patient_data_testing/modele_complet/2353729/2353729regReadmesallnew.txt"); 
	
	 if(!numbers_str2)
	 { 
		exit(1);
	 } 
	
	 numbers_str2.seekg(0, ios::beg);
	 /////Get the new file info included 
	 numbers_str2.getline(temp_string_wrl,200); 

	 sscanf (temp_string_wrl,"%s%d",temp_string2, &num_mris);
	 numbers_str2.getline(temp_string_wrl,200); 
	 sscanf (temp_string_wrl,"%s%s",temp_string2, topo);	
	 numbers_str2.getline(temp_string_wrl,200); 
	 sscanf (temp_string_wrl,"%s%s",temp_string2, thefilexray);	
	 numbers_str2.getline(temp_string_wrl,200); 
	 sscanf (temp_string_wrl,"%s%s",temp_string2, xraymarks);	
	 numbers_str2.getline(temp_string_wrl,200); 
	 sscanf (temp_string_wrl,"%s%s",temp_string2, spinalmarks);	

	//const char *thefilexray = "D:/patient_data_testing/modele_complet/2353729/2353729/2353729.wrl";
	unregisteredxray.polydata_wrllandmarks =  vtkPolyData::New ();
	unregisteredxray.wrllandmark_points=vtkPoints::New();
	unregisteredxray = Transform::read_landmarks_xray(xraymarks, unregisteredxray);//"D:/patient_data_testing/modele_complet/2052364/2052364/2052364.o3"
	polydatawrl = Transform::read (thefilexray);
	thexraypeds=Transform::read_spinal_marks_xray8(spinalmarks); //was read_spinal_marks_xray //"D:/patient_data_testing/modele_complet/2052364/2052364/2052364.wrl"
	unregisteredtopo.polydatasze = Transform::read (topo);//"D:/patient_data_testing/modele_complet/2052364/registerednew/2052364registeredtopo.sze");

	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert1);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert2);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrivert3);
	xray2_vertindex=-2; //start at L5
	xray3_vertindex=-2; //start at L4
	xray4_vertindex=-2;
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray2_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray3_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d",temp_string2, &xray4_vertindex);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, reptxfile);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, o3txfile);	
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemriland);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[0], &matrixorigin[1], &matrixorigin[2], &matrixorigin[3]);//element(0,0), (0,1), (0,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[4], &matrixorigin[5], &matrixorigin[6], &matrixorigin[7]);//element(1,0), (1,1), (1,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[8], &matrixorigin[9], &matrixorigin[10], &matrixorigin[11]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin[12], &matrixorigin[13], &matrixorigin[14], &matrixorigin[15]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin2[0], &matrixorigin2[1], &matrixorigin2[2], &matrixorigin2[3]);//element(0,0), (0,1), (0,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin2[4], &matrixorigin2[5], &matrixorigin2[6], &matrixorigin2[7]);//element(1,0), (1,1), (1,2)
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin2[8], &matrixorigin2[9], &matrixorigin2[10], &matrixorigin2[11]);
	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixorigin2[12], &matrixorigin2[13], &matrixorigin2[14], &matrixorigin2[15]);

	numbers_str2.getline(temp_string_wrl,200); 
	sscanf (temp_string_wrl,"%s%d%d",temp_string2, &num_axslices, &index_new_vol);

	mri_dimz = 	 num_axslices;
	for (int imageindex = 0; imageindex < num_axslices; imageindex++)
	{
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%s",temp_string2, thefilemrippm[imageindex]);//put a _seg before the .ppm
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%d%d%d",temp_string2, &mrirange1[0], &mrirange1[1], &mri_intensity1);	
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%f%f%f%d",temp_string2, &mri_spacing[0], &mri_spacing[1], &mri_spacing[2], &theslice[imageindex]);
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][0], &matrixarticulated[imageindex][1], &matrixarticulated[imageindex][2], &matrixarticulated[imageindex][3]);
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][4], &matrixarticulated[imageindex][5], &matrixarticulated[imageindex][6], &matrixarticulated[imageindex][7]);
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][8], &matrixarticulated[imageindex][9], &matrixarticulated[imageindex][10], &matrixarticulated[imageindex][11]);
		numbers_str2.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%s%f%f%f%f",temp_string2, &matrixarticulated[imageindex][12], &matrixarticulated[imageindex][13], &matrixarticulated[imageindex][14], &matrixarticulated[imageindex][15]); //matrixarticulated

	}
	numbers_str2.close();

	std::ifstream numbers_strtxo3( o3txfile );
	numbers_strtxo3.seekg(0, ios::beg);
	for(int i=0;i<17;i++)
	{
		volo3[i]= vtkMatrix4x4::New();
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volo3[i]->SetElement(0,3,thetrans[0]); 
		volo3[i]->SetElement(1,3,thetrans[1]); 
		volo3[i]->SetElement(2,3,thetrans[2]);
		volo3[i]->SetElement(3,3,1);
		numbers_strtxo3.getline(temp_string_wrl,200); 
		sscanf_s (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);
		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volo3[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}

	}
	numbers_strtxo3.close();
	std::ifstream numbers_strtxrep( reptxfile );
	numbers_strtxrep.seekg(0, ios::beg);

	//load mri tx, 1st is T1 i think
	for(int i=0;i<17;i++)
	{
		volrep[i]= vtkMatrix4x4::New();
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f",&thetrans[0],&thetrans[1],&thetrans[2]);
		volrep[i]->SetElement(0,3,thetrans[0]); 
		volrep[i]->SetElement(1,3,thetrans[1]); 
		volrep[i]->SetElement(2,3,thetrans[2]);
		volrep[i]->SetElement(3,3,1);
		numbers_strtxrep.getline(temp_string_wrl,200); 
		sscanf (temp_string_wrl,"%f%f%f%f%f%f%f%f%f",&themat[0],&themat[1],&themat[2],&themat[3],&themat[4],&themat[5],&themat[6],&themat[7],&themat[8]);

		for(int ab=0;ab<3;ab++)
		{
			for(int bb=0;bb<3;bb++)
				volrep[i]->SetElement(ab,bb,themat[ab+bb*3]); 
		}
		volrep[i]->Invert();
	}

	for(int gg=0;gg<17;gg++)
	{
		mrivert_regvol[gg] = vtkMatrix4x4::New();
		mrivert_regvol[gg]->Identity(); //start it with L5
		mrivert_regvol[gg]->Multiply4x4(volo3[gg],volrep[gg],mrivert_regvol[gg]); //i guess starts with L5 
		//so vol03 o volrep 1st tx after
	}

	numbers_strtxrep.close();

	vtkMatrix4x4 *volmriarticulated1[17];
	vtkMatrix4x4 *volmriarticulated2[17];
	for (int thth = 0; thth <17;thth++)
	{
		volmriarticulated1[thth]= vtkMatrix4x4::New();
		volmriarticulated1[thth]->Identity();
		volmriarticulated1[thth]->Multiply4x4(mrivert_regvol[thth],volmriarticulated1[thth],volmriarticulated1[thth]); //just a starting point, will remove
		volmriarticulated1[thth]->Invert();

		volmriarticulated2[thth]= vtkMatrix4x4::New();
		volmriarticulated2[thth]->Identity();
		volmriarticulated2[thth]->DeepCopy(volmriarticulated1[thth]);
	}


	//read the verts
	polydatawrl2vert.polydata = new vtkPolyData*[20];
	polydatawrl3vert.polydata = new vtkPolyData*[20];
	polydatawrl4vert.polydata = new vtkPolyData*[20];


	polydatawrl2vert = Transform::read_wrlsep(thefilemrivert1);  //thefilemrivert1
	for(int gg=0;gg<polydatawrl2vert.num_vertebrae;gg++) 
			wrlactor2[gg]=vtkActor::New();

	if (num_mris>1)
	{
		polydatawrl3vert = Transform::read_wrlsep(thefilemrivert2);
		for(int gg=0;gg<polydatawrl3vert.num_vertebrae;gg++)
			wrlactor3[gg]=vtkActor::New();
	}
	if (num_mris>2)
	{
		polydatawrl4vert = Transform::read_wrlsep(thefilemrivert3);
		for(int gg=0;gg<polydatawrl4vert.num_vertebrae;gg++)
			wrlactor4[gg]=vtkActor::New();
	}

	//read the landmarks
	vtkFloatArray *values22 = vtkFloatArray::New();
	vtkPoints *newmrilandmark_pointspart[17];
	float P[3], p1, p2, p3;
	for(int lala=0;lala<17;lala++)	 
		newmrilandmark_pointspart[lala] = vtkPoints::New();

	std::ifstream numbers_str3(thefilemriland); 
	if(!numbers_str3)
		exit(1);

	numbers_str3.seekg(0, ios::beg);
	for(int lala=0;lala<17;lala++)	
	{
		for(int theped=0;theped<8;theped++)	
		{
			numbers_str3.getline(temp_string_wrl,200); 
			sscanf (temp_string_wrl,"%f%f%f", &p1, &p2,&p3);
			newmrilandmark_pointspart[lala]->InsertNextPoint(p1,p2,p3); //starts with T1
		}
	}
	numbers_str3.close();

	for(int lala=0;lala<17;lala++)
	{
		mri_newlandmarkspart[lala]=vtkPolyData::New();
		mri_newlandmarkspart[lala]->SetPoints(newmrilandmark_pointspart[lala]); //bad name but ok
	}

	bwLutaxial = vtkLookupTable::New();
	bwLutaxial->SetTableRange(0, mri_intensity1);//100
	bwLutaxial->SetSaturationRange(0,0);
	bwLutaxial->SetHueRange(0,0);
	bwLutaxial->SetValueRange(0,1);
	bwLutaxial->Build(); 

	bwLutaxial2 = vtkLookupTable::New();
	bwLutaxial2->SetTableRange(0, mri_intensity1*4);//100
	bwLutaxial2->SetSaturationRange(0,0);
	bwLutaxial2->SetHueRange(0,0);
	bwLutaxial2->SetValueRange(0,1);
	bwLutaxial2->Build(); 

	//read the original lower volume
	mri_dimx=312; mri_dimy=448;
	v18ax =  vtkSmartPointer<vtkVolume16Reader>::New();
	v18ax->SetDataDimensions (mri_dimx,mri_dimy);//(64, 64);//
	v18ax->SetImageRange(1,28);//(mrirange_axial1[0],mrirange_axial1[1]); //
	v18ax->SetDataByteOrderToLittleEndian(); 
	v18ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2052364\\axial_lower\\raw\\2052364");
	//v18ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2353729\\AX_T1_LOWER_0011\\raw\\2353729");
	v18ax->SetDataSpacing(1.116,1.116,12);//(mri_spacing_axial[0],mri_spacing_axial[1],mri_spacing_axial[2]);
	v18ax->Update();

	imageData_ax1 = vtkSmartPointer<vtkImageData>::New();
	imageData_ax1->SetScalarTypeToUnsignedChar();
	imageData_ax1->SetNumberOfScalarComponents(1);
	imageData_ax1->AllocateScalars();
	imageData_ax1->DeepCopy(v18ax->GetOutput());
	imageData_ax1->Update();

	//read the original upper volume
	v19ax =  vtkSmartPointer<vtkVolume16Reader>::New();
	v19ax->SetDataDimensions (mri_dimx,mri_dimy);//(64, 64);//
	v19ax->SetImageRange(1,28);//(mrirange_axial1[0],mrirange_axial1[1]); //
	v19ax->SetDataByteOrderToLittleEndian(); 
	//v19ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2353729\\AX_T1_UPPER_0010\\raw\\2353729");
	v19ax->SetFilePrefix ("D:\\patient_data_testing\\modele_complet\\2052364\\axial_upper\\raw\\2052364");
	//D:\\patient_data_testing\\modele_complet\\2052364\\axial_upper\\raw\\2052364
	v19ax->SetDataSpacing(1.116,1.116,12);//(mri_spacing_axial[0],mri_spacing_axial[1],mri_spacing_axial[2]);
	v19ax->Update();

	imageData_ax2 = vtkSmartPointer<vtkImageData>::New();
	imageData_ax2->SetScalarTypeToUnsignedChar();
	imageData_ax2->SetNumberOfScalarComponents(1);
	imageData_ax2->AllocateScalars();
	imageData_ax2->DeepCopy(v19ax->GetOutput());
	imageData_ax2->Update();
	
	voloriginmridisplay =  vtkMatrix4x4::New();
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
			voloriginmridisplay->SetElement(i,j,matrixorigin[i*4+j]);
	}

		voloriginmridisplay2 =  vtkMatrix4x4::New();
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
			voloriginmridisplay2->SetElement(i,j,matrixorigin2[i*4+j]);
	}

	/******************************************
	Transform volume to its real 3D location
	******************************************/
	vtkTransform *txoriginalaxial=vtkTransform::New();
	txoriginalaxial->Identity();
	txoriginalaxial->PostMultiply();
	txoriginalaxial->SetMatrix(voloriginmridisplay2); //only original tx, inverted
	//txoriginalaxial->Inverse();
	txoriginalaxial->Update();

	tempimage= vtkImageData::New();
	vtkImageReslice *resliceax1temp = vtkImageReslice::New();
	resliceax1temp->SetInput(imageData_ax1);
	resliceax1temp->InterpolateOn();
	resliceax1temp->SetInterpolationModeToCubic();
	resliceax1temp->SetResliceTransform(txoriginalaxial);
	resliceax1temp->SetOutputSpacing(imageData_ax1->GetSpacing());
	resliceax1temp->AutoCropOutputOn();
	resliceax1temp->GetInput()->Update();
	resliceax1temp->Update(); 
		
	tempimage->SetScalarTypeToShort();// 
	tempimage->SetNumberOfScalarComponents(1);
	tempimage->AllocateScalars();
	tempimage->GetPointData()->CopyAllOn();
	tempimage->GetPointData()->CopyScalarsOn();
	tempimage->DeepCopy( resliceax1temp->GetOutput()); 
	tempimage = resliceax1temp->GetOutput();
	tempimage->Update();

	/******************************************
	Transform 2nd volume to its real 3D location
	******************************************/
	vtkTransform *txoriginalaxial2=vtkTransform::New();
	txoriginalaxial2->Identity();
	txoriginalaxial2->PostMultiply();
	txoriginalaxial2->SetMatrix(voloriginmridisplay); //only original tx, inverted
	//txoriginalaxial->Inverse();
	txoriginalaxial2->Update();

	tempimage2= vtkImageData::New();
	vtkImageReslice *resliceax1temp2 = vtkImageReslice::New();
	resliceax1temp2->SetInput(imageData_ax2);
	resliceax1temp2->InterpolateOn();
	resliceax1temp2->SetInterpolationModeToCubic();
	resliceax1temp2->SetResliceTransform(txoriginalaxial2);
	resliceax1temp2->SetOutputSpacing(imageData_ax2->GetSpacing());
	resliceax1temp2->AutoCropOutputOn();
	resliceax1temp2->GetInput()->Update();
	resliceax1temp2->Update(); 
		
	tempimage2->SetScalarTypeToShort();// 
	tempimage2->SetNumberOfScalarComponents(1);
	tempimage2->AllocateScalars();
	tempimage2->GetPointData()->CopyAllOn();
	tempimage2->GetPointData()->CopyScalarsOn();
	tempimage2->DeepCopy( resliceax1temp2->GetOutput()); 
	tempimage2 = resliceax1temp2->GetOutput();
	tempimage2->Update();
	
	volmrifinal[0] = vtkMatrix4x4::New();
	//when loaded: //element(0,0), (0,1), (0,2), (0,3)
	for(int i = 0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
		{
		volmrifinal[0]->SetElement(i,j,matrixarticulated[0][i*4+j]);
		}
	}

	/**************************************************************
	First, create a volume along the zeroth slice transformed, 
	this volume will be used to store final values 
	**************************************************************/
	vtkTransform	*totalTrasforminv=vtkTransform::New();
	totalTrasforminv->SetMatrix(volmrifinal[0]);//(volototal[imageindex]);
	totalTrasforminv->Inverse();

	vtkImageReslice *reslice0transfomed = vtkImageReslice::New();
	reslice0transfomed->SetInput(tempimage);
	reslice0transfomed->InterpolateOn();
	reslice0transfomed->SetInterpolationModeToCubic();
	reslice0transfomed->SetResliceTransform(totalTrasforminv);
	reslice0transfomed->SetOutputSpacing(tempimage->GetSpacing());
	reslice0transfomed->AutoCropOutputOn();
	reslice0transfomed->GetInput()->Update();
	reslice0transfomed->Update(); 

	imagedata0transfomed->SetScalarTypeToShort();// 
	imagedata0transfomed->SetNumberOfScalarComponents(1);
	imagedata0transfomed->AllocateScalars();
	imagedata0transfomed->GetPointData()->CopyAllOn();
	imagedata0transfomed->GetPointData()->CopyScalarsOn();
	imagedata0transfomed->DeepCopy( reslice0transfomed->GetOutput()); 
	imagedata0transfomed = reslice0transfomed->GetOutput();
	imagedata0transfomed->Update();

	int extentimagedata0[6];
	imagedata0transfomed->GetExtent(extentimagedata0);

	/********************************
	set all pixels of the final volume to 0
	*********************************/
	vtkFloatArray *scalarsimagedata0 = vtkFloatArray::New();
	scalarsimagedata0 = (vtkFloatArray *)(imagedata0transfomed->GetPointData()->GetScalars());
	for(int i11=0;i11<extentimagedata0[5];i11++)
	{
		for(int j11=0;j11<extentimagedata0[3];j11++)
		{
			for (int k11=0;k11<extentimagedata0[1];k11++)
			{
				int offsetMRI = (int)((extentimagedata0[3]*extentimagedata0[1]*i11)+extentimagedata0[1]*(int)j11+(int)k11);
				scalarsimagedata0->SetTuple1(offsetMRI,0);
			}
		}
	} 

	/**********************************************************************************************
	For each slice,  load the registered slice transform it and place it in the overall volume
	***********************************************************************************************/
	for (int imageindex = 0; imageindex < 1; imageindex++) //num_axslices
	{
		volmrifinal[imageindex] = vtkMatrix4x4::New();
		volototal[imageindex]= vtkMatrix4x4::New();

		/******************************************************************************
		load the registered image at the right slice of imagedata
		use specific volume depending on whether we are upper volume or lower...
		******************************************************************************/
		int testextent[6];
		vtkSmartPointer<vtkImageMapToColors> saggitalColors = vtkSmartPointer<vtkImageMapToColors>::New();
		int theextent[6];
		saggitalColors->SetLookupTable(bwLutaxial);
		
		if (imageindex < index_new_vol) //lower volume being used
		{
			tempimage = AffineRegistration::loadPPMvol2(tempimage, thefilemrippm[imageindex], theslice[imageindex]);
			tempimage->GetExtent(theextent);
			saggitalColors->SetInput(tempimage);
		}
		else
		{
			tempimage = AffineRegistration::loadPPMvol2(tempimage2, thefilemrippm[imageindex], theslice[imageindex]);
			tempimage->GetExtent(theextent);
			saggitalColors->SetInput(tempimage);
		}

		/************************
		problem::: the volume does not change orientation after transformation. I need to do the same thing
		to the slice actor... cant do that
		I need to transform each pixel at a time if > 0
		************************/

		/***************************************************
		Transform the volume to the registered location of the 
		appropriate slice. The following has not worked. Instead, insert a point where value>0, transform points,
		then find equivalent location in overall volume
		***************************************************/
		//when loaded: //element(0,0), (0,1), (0,2), (0,3)
		for(int i = 0;i<4;i++)
		{
			for(int j = 0;j<4;j++)
			{
				volmrifinal[imageindex]->SetElement(i,j,matrixarticulated[imageindex][i*4+j]);
			}
		}

		//overall transfo for the slice as saved during registration
		totalTrasform[imageindex]=vtkTransform::New();
		totalTrasform[imageindex]->SetMatrix(volmrifinal[imageindex]);//(volototal[imageindex]);
		totalTrasform[imageindex]->Update();

		vtkTransform *totalTrasforminvslice=vtkTransform::New();
		totalTrasforminvslice->SetMatrix(volmrifinal[imageindex]);
		totalTrasforminvslice->Inverse();

		vtkImageReslice *resliceimageindextx = vtkImageReslice::New();
		resliceimageindextx->SetInput(tempimage);
		resliceimageindextx->InterpolateOn();
		resliceimageindextx->SetInterpolationModeToCubic();
		resliceimageindextx->SetResliceTransform(totalTrasforminv);
		resliceimageindextx->SetOutputSpacing(tempimage->GetSpacing());
		resliceimageindextx->AutoCropOutputOn();
		resliceimageindextx->GetInput()->Update();
		resliceimageindextx->Update(); 

		imagedataindextransfomed->SetScalarTypeToShort();// 
		imagedataindextransfomed->SetNumberOfScalarComponents(1);
		imagedataindextransfomed->AllocateScalars();
		imagedataindextransfomed->GetPointData()->CopyAllOn();
		imagedataindextransfomed->GetPointData()->CopyScalarsOn();
		imagedataindextransfomed->DeepCopy( resliceimageindextx->GetOutput()); 
		imagedataindextransfomed = resliceimageindextx->GetOutput();
		imagedataindextransfomed->Update();

		/**********************************************************************
		For each pixel of the tranformed slice. if value > 0, find corresponding 
		pixel in volume0 and set it to that..
		**********************************************************************/
		int extentimagedataindex[6];
		int tomod = 0;
		vtkPoints *mriedgepoints=vtkPoints::New();
		vtkPolyData *MRIedgeData = vtkPolyData::New();
		vtkFloatArray *scalarsMRI = vtkFloatArray::New();
		tempimage->GetExtent(extentimagedataindex); //
		vtkSphereSource *sphere = vtkSphereSource::New();
		vtkTransformPolyDataFilter *fvertsurface = vtkTransformPolyDataFilter::New ();
		sphere->SetRadius(5);

		//scalarsMRI = (vtkFloatArray *)(imagedataindextransfomed->GetPointData()->GetScalars());	
		scalarsMRI = (vtkFloatArray *)(tempimage->GetPointData()->GetScalars());	
		for(int j11=0;j11<(extentimagedataindex[3]);j11++)
		{
			for (int k11=0;k11<(extentimagedataindex[1]);k11++)
			{
				//get the value from the slice of the transformed MRI
					
				int offsetMRI1 = (int)(((extentimagedataindex[3]+1)*(extentimagedataindex[1]+1)*(theslice[imageindex]))+(extentimagedataindex[1]+1)*(int)j11+(int)k11);
				//if value>0
				float MRIval =  scalarsMRI->GetTuple1(offsetMRI1);
				if (MRIval> 0)
				{

					//find coordinates of pixel of transformed slice image in real world
					double *bounds = new double[6];
					//below -1 works when offset 1 has no +1
					int offsetMRI2 = (int)(((extentimagedataindex[3])*(extentimagedataindex[1])*(theslice[imageindex]))+(extentimagedataindex[1])*(int)j11+(int)k11);
					//int offsetMRI2 = (int)(((theextent[3])*(theextent[1])*(theslice[imageindex]))+(theextent[1])*(int)j11+(int)k11);

					//ODDD that it is giving me alignment of imagedata0 imagedataindextransfomed is actually not different in alignment from imagedata0
					//the transformation does not change the alignment of the volume. you knew that already

					//imagedataindextransfomed->GetCell(offsetMRI2)->GetBounds(bounds);
					tempimage->GetCell(offsetMRI2)->GetBounds(bounds);

					//find coordinates of real world in volume world for overall volume
					double *worldcenter = new double[3];
					worldcenter[0]=(float)(bounds[0]+1); //removing +1 messes up location of pixels axially 
					worldcenter[1]=(float)(bounds[2]+1);
					worldcenter[2]=(float)(bounds[4]+1); //they are at the right world coords


					double *paraCoords = new double[3]; 
					int theimagecoordMRI[3];

					//need to be transformed first
					//worldcenter and theimagecoordMRI correspond well. worldcenter are at the wrong place though
					//imagedata0transfomed->ComputeStructuredCoordinates(worldcenter,theimagecoordMRI,paraCoords);
					//set coordinate value in overall volume to 1
					//int offsetMRI1 = (int)(((extentimagedataindex[3]+1)*(extentimagedataindex[1]+1)*(theslice[imageindex]))+(extentimagedataindex[1]+1)*(int)j11+(int)k11);

					//int offsetMRIoriginal = (int)(((extentimagedata0[3]+1)*(extentimagedata0[1]+1)*theimagecoordMRI[2])+(extentimagedata0[1]+1)*(int)theimagecoordMRI[1]+(int)theimagecoordMRI[0]);
				
					//set scalar to 1
					//scalarsimagedata0->SetTuple1(offsetMRIoriginal,MRIval);
					
					//if ((theimagecoordMRI[2]>= 0) &&(theimagecoordMRI[1]>= 0) &&(theimagecoordMRI[0]>= 0))
					//{

						//scalarsimagedata0->SetTuple1(offsetMRIoriginal,MRIval);
						//add points to a polydata for verification
						if((tomod%10)==0)
							mriedgepoints->InsertNextPoint(worldcenter[0],worldcenter[1],worldcenter[2]);
						tomod++;
					//}
				}
			}
		}


		MRIedgeData->SetPoints(mriedgepoints);

		/****************************************************************
		transform the points  (supposed to be where>0 in right slice
		using the same transformation as the image actor
		****************************************************************/
		fvertsurface->SetInput (MRIedgeData); //original
		fvertsurface->SetTransform (totalTrasform[imageindex]);//(totalTrasforminvslice);//
		fvertsurface->Update ();
		MRIedgeData=fvertsurface->GetOutput();
		MRIedgeData->DeepCopy(fvertsurface->GetOutput()); //the points are at the right slice after transformation

		
		/****************************************************************
		after the points are transformed, find their location in
		the volume0 and set value to 1
		****************************************************************/
		double *temptxpoint = new double[3];
		vtkPoints *transformedpoints = MRIedgeData->GetPoints();
		for (int iii = 0; iii< transformedpoints->GetNumberOfPoints(); iii++)
		{
			//get 3D location of point
			temptxpoint = transformedpoints->GetPoint(iii);
			//find index in volume 0
			double *paraCoords = new double[3]; 
			int theimagecoordMRI[3];
			imagedata0transfomed->ComputeStructuredCoordinates(temptxpoint,theimagecoordMRI,paraCoords);
			//int offsetMRI2 = (int)(((extentimagedata0[3])*(extentimagedata0[1])*(theimagecoordMRI[2]))+(extentimagedata0[1])*(theimagecoordMRI[1])+(theimagecoordMRI[0]));
			int offsetMRI2 = (int)(((extentimagedata0[3]+1)*(extentimagedata0[1]+1)*(theimagecoordMRI[2])+((int)(theimagecoordMRI[1]))*(extentimagedata0[3]+1)+(int)theimagecoordMRI[0]));	
			//int offsetTP = (int)((slicenumber)*(newmri_dimy)*(newmri_dimx)+((int)(theimagecoordMRI[1]))*(newmri_dimx)+(int)theimagecoordMRI[0]);	
			//set scalar to 1
			scalarsimagedata0->SetTuple1(offsetMRI2,255);
		} 

		//this here is not entirely working in both cases unfortunately
		for(int j11=0;j11<extentimagedata0[3];j11++)
		{
			for (int k11=0;k11<extentimagedata0[1];k11++)
			{
				//this one makes it diagonal
				//int offsetMRI = (int)(((extentimagedata0[3]+1)*(extentimagedata0[1]+1)*(12)+((int)(j11))*(extentimagedata0[3]+1)+(int)k11));	
				//this one works better but has some wierd overal issues
				int offsetMRI = (int)((extentimagedata0[3]*extentimagedata0[1]*12)+extentimagedata0[1]*(int)j11+(int)k11);
				scalarsimagedata0->SetTuple1(offsetMRI,255);
			}
		}


		//the points are at the 8th slice of imagedataindextransformed, but not at the right 3D place
		//is the 8th slice of imagedataindextransformed the wrong thing? Seems so. 12th slice fits well
		///the points are now at the right slice.
		vtkGlyph3D *glyph3 = vtkGlyph3D::New();
		glyph3->SetInput(MRIedgeData);//(MRIedgeData);//mriSlicePdata//MRIedgeData//regmriSlicePdata//closestpolys
		glyph3->SetSource(sphere->GetOutput());
		glyph3->SetScaleModeToScaleByScalar();
		vtkPolyDataMapper *spikeMapper3 = vtkPolyDataMapper::New();
		spikeMapper3->SetInput(glyph3->GetOutput());
		spikeActorslice2= vtkActor::New();
		spikeActorslice2->SetMapper(spikeMapper3);
		spikeActorslice2->GetProperty()->SetColor(1.0,0.0,0.0);//red is mri

		saggitalColors->Update();
		tempimage->GetExtent(testextent);
		axialImageActor2[imageindex] = vtkSmartPointer<vtkImageActor>::New();
		axialImageActor2[imageindex]->SetInput(saggitalColors->GetOutput());
		axialImageActor2[imageindex]->SetUserTransform(totalTrasform[imageindex]);
		axialImageActor2[imageindex]->SetDisplayExtent(0,testextent[1], 0,testextent[3], theslice[imageindex],theslice[imageindex]);
	}

	//update the overall volume with the new scalars
	imagedata0transfomed->GetPointData()->SetScalars(scalarsimagedata0);


	aRenderer->AddActor(axialImageActor2[0]);
	//aRenderer->AddActor(spikeActorslice2);

	//imagedataindextransfomed places the slice at the right place.. imagedata0transfomed maps the wrong thing to the wrong place
	//does it?
	planeWidgetZ4= vtkImagePlaneWidget::New();
	planeWidgetZ4->DisplayTextOn();
	planeWidgetZ4->SetInput(imagedata0transfomed);//(imagedataindextransfomed);//(imagedata0transfomed);
	planeWidgetZ4->UserControlledLookupTableOn();
	planeWidgetZ4->SetLookupTable(bwLutaxial);
	planeWidgetZ4->SetPlaneOrientationToZAxes();
	//planeWidgetZ4->SetSliceIndex(0);
	//planeWidgetZ4->SetPicker(picker);
	//planeWidgetZ4->SetKeyPressActivationValue ('c');
	vtkProperty *prop3 = planeWidgetZ4->GetPlaneProperty();
	prop3->SetColor(0, 0, 0);

	//good
	planeWidgetZ4->SetInteractor( renWin->GetInteractor() );
	planeWidgetZ4->On();
	int currentpos=0;
	slicetoshow = theslice[currentpos];
	CSpinButtonCtrl *pSpin = reinterpret_cast<CSpinButtonCtrl *>(GetDlgItem(IDC_SPINslice)); 
	pSpin->SetRange32(currentpos,currentpos+num_axslices-1);
	pSpin->SetPos32(currentpos);
	pSpin->UpdateData(TRUE);

	CEdit *Display;
	char tttt[4];
	sprintf(tttt,"%d",slicetoshow);
	Display = reinterpret_cast<CEdit *>(GetDlgItem(IDC_EDITspin));
	CStringW tempme(tttt);
	LPCTSTR tt3 = (LPCTSTR)tempme;
	Display->SetWindowTextW(tt3);

	showmrivertlandmarks();	
	showMRIverts();
	this->showtopo();
	showxrayvertlandmarks();

	/*	topocheck.EnableWindow(TRUE);
	topocheck.SetCheck(1);
	extlandcheck.EnableWindow(TRUE);
	extlandcheck.SetCheck(1);
	xraycheck.EnableWindow(TRUE);
	xraycheck.SetCheck(1);
	extlandcheck.EnableWindow(TRUE);
	extlandcheck.SetCheck(1);*/

	aRenderer->Render();
	refreshscreen();
	/*
	strcpy(patnname,"D:/patient_data_testing/modele_complet/1734048/registerednew/1734048");
	//strcpy(patnname2,"D:/patient_data_testing/modele_complet/2052364/registerednew/2052364b");
	//strcpy(patnname,"D:/patient_data_testing/modele_complet/2052364/registerednew/2052364b");

	strcpy(patnname2,"D:/patient_data_testing/modele_complet/2353729to1734048/registerednew/2353729to1734048");
	//strcpy(patnname2,"D:/patient_data_testing/modele_complet/1734048/registerednew/1734048");

	strcpy(patnameMRI,patnname);
	strcat(patnameMRI,"MRIslice");
	strcat(patnameMRI,sliceme);
	strcpy(patnameTP,patnname);
	//strcat(patnameTP,"TPslice");
	strcat(patnameTP,"MRIslice");
	strcat(patnameTP,sliceme);

	strcpy(TPslicetps,patnname2);
	//strcat(TPslicetps,"TPslice");
	strcat(TPslicetps,"MRIslice");
	strcat(TPslicetps,sliceme2);
	strcat(TPslicetps,"tpsfatseg.ppm");

	strcpy(TPslicerigid,patnname2);
	strcat(TPslicerigid,"TPslice");
	//strcat(TPslicerigid,"MRIslice");
	strcat(TPslicerigid,sliceme2);
	strcat(TPslicerigid,"rigidseg.ppm");

	strcpy(TPsliceart,patnname2);
	strcat(TPsliceart,"TPslice");
	//strcat(TPsliceart,"MRIslice");
	strcat(TPsliceart,sliceme2);
	strcat(TPsliceart,"articulatedseg.ppm");

	strcpy(MRIslicetps,patnname);
	strcat(MRIslicetps, "MRIslice");
	strcat(MRIslicetps,sliceme);
	strcat(MRIslicetps,"tpsfatseg.ppm");

	strcpy(MRIslicerigid,patnname);
	strcat(MRIslicerigid, "MRIslice");
	strcat(MRIslicerigid,sliceme);
	strcat(MRIslicerigid,"rigidseg.ppm");

	strcpy(MRIsliceart, patnname);
	strcat(MRIsliceart, "MRIslice");
	strcat(MRIsliceart,sliceme);
	strcat(MRIsliceart,"articulatedseg.ppm");

	//open TP segmentation and get size
	vtkImageData* TPimage = vtkImageData::New();
	TPimage = AffineRegistration::loadPPM(TPslicetps);

	//open MRI segmentation and save in imagedata
	vtkImageData* MRimage = vtkImageData::New();
	MRimage = AffineRegistration::loadPPM(MRIslicetps);

	int imageextent[6];
	MRimage->GetExtent(imageextent);

	int sizex = imageextent[1]+1;
	int sizey = imageextent[3]+1;

	//store the values in array
	vtkFloatArray *scalarsTP = vtkFloatArray::New();
	vtkFloatArray *scalarsMRI = vtkFloatArray::New();
	scalarsTP=  (vtkFloatArray *)TPimage->GetPointData()->GetScalars();
	scalarsMRI=  (vtkFloatArray *)MRimage->GetPointData()->GetScalars();
	//kappa = 2*TP/(2*TP+FP+FN)
	float kappaproposed = AffineRegistration::getkappa(scalarsMRI , scalarsTP, sizex, sizey );



	//now for rigid
	//open TP segmentation and get size
	vtkImageData* TPimagerigid = vtkImageData::New();
	TPimagerigid = AffineRegistration::loadPPM(TPslicerigid);//("debudkappa1.ppm");//


	//open MRI segmentation and save in imagedata
	vtkImageData* MRimagerigid = vtkImageData::New();
	MRimagerigid = AffineRegistration::loadPPM(MRIslicerigid);//("debudkappa2.ppm");//

	int imageextentrigid[6];
	MRimagerigid->GetExtent(imageextentrigid);

	sizex = imageextentrigid[1]+1;
	sizey = imageextentrigid[3]+1;

	//store the values in array
	vtkFloatArray *scalarsTPrigid = vtkFloatArray::New();
	vtkFloatArray *scalarsMRIrigid = vtkFloatArray::New();
	scalarsTPrigid=  (vtkFloatArray *)TPimagerigid->GetPointData()->GetScalars();
	scalarsMRIrigid=  (vtkFloatArray *)MRimagerigid->GetPointData()->GetScalars();
	//kappa = 2*TP/(2*TP+FP+FN)
	float kapparigid = AffineRegistration::getkappa(scalarsMRIrigid , scalarsTPrigid, sizex, sizey );


	//now for articulated
	//open TP segmentation and get size
	vtkImageData* TPimageart = vtkImageData::New();
	TPimageart = AffineRegistration::loadPPM(TPsliceart);

	//open MRI segmentation and save in imagedata
	vtkImageData* MRimageart = vtkImageData::New();
	MRimageart = AffineRegistration::loadPPM(MRIsliceart);

	int imageextentart[6];
	MRimageart->GetExtent(imageextentart);

	sizex = imageextentart[1]+1;
	sizey = imageextentart[3]+1;

	//store the values in array
	vtkFloatArray *scalarsTPart = vtkFloatArray::New();
	vtkFloatArray *scalarsMRIart = vtkFloatArray::New();
	scalarsTPart=  (vtkFloatArray *)TPimageart->GetPointData()->GetScalars();
	scalarsMRIart=  (vtkFloatArray *)MRimageart->GetPointData()->GetScalars();
	//kappa = 2*TP/(2*TP+FP+FN)
	float kappaart = AffineRegistration::getkappa(scalarsMRIart , scalarsTPart, sizex, sizey );



	char themessproposed[20], themessrigid[20], themessart[20], final[200]; 
	sprintf(themessrigid,"%f",kapparigid);
	sprintf(themessart,"%f",kappaart);
	sprintf(themessproposed,"%f",kappaproposed);
	strcpy(final, "rigid= ");
	strcat(final,themessrigid); 
	strcat(final, " articulated= ");
	strcat(final,themessart); 
	strcat(final, " proposed= ");
	strcat(final,themessproposed); 
	//creat a popup window with the kappa value
	//AfxMessageBox(_T(themess));
	CString yoyo(final);//themessrigid);
	//LPCSTR yaya = (LPCSTR)yoyo.c_str();
	LPCTSTR yaya2 = (LPCTSTR)yoyo;
	AfxMessageBox(yaya2);*/
}
