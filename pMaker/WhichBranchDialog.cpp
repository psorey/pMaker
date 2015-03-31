// WhichBranchDialog.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "WhichBranchDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWhichBranchDialog dialog


CWhichBranchDialog::CWhichBranchDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CWhichBranchDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWhichBranchDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWhichBranchDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWhichBranchDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWhichBranchDialog, CDialog)
	//{{AFX_MSG_MAP(CWhichBranchDialog)
	ON_BN_CLICKED(IDC_RADIO_LEFT, OnRadioLeft)
	ON_BN_CLICKED(IDC_RADIO_RIGHT, OnRadioRight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWhichBranchDialog message handlers

void CWhichBranchDialog::OnRadioLeft() 
{
	fIsLeft = TRUE;
}

void CWhichBranchDialog::OnRadioRight() 
{
	fIsLeft = FALSE;
}
