// SFCoinSet.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SFCoinSet.h"
#include "SFCoinSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSFCoinSetApp

BEGIN_MESSAGE_MAP(CSFCoinSetApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSFCoinSetApp construction

CSFCoinSetApp::CSFCoinSetApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSFCoinSetApp object

CSFCoinSetApp theApp;


// CSFCoinSetApp initialization

BOOL CSFCoinSetApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CSFCoinSetDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CSFCoinSetApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	//MessageBox(NULL,L"ExitInstance",NULL,MB_OK);
	return CWinApp::ExitInstance();
}
