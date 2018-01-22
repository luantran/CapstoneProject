// full_registration_dialog.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cfull_registration_dialogApp:
// See full_registration_dialog.cpp for the implementation of this class
//

class Cfull_registration_dialogApp : public CWinApp
{
public:
	Cfull_registration_dialogApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cfull_registration_dialogApp theApp;