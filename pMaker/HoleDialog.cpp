// HoleDialog.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "HoleDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HoleDialog dialog


HoleDialog::HoleDialog(CWnd* pParent /*=NULL*/)
	: CDialog(HoleDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(HoleDialog)
	fHoleSpacing = 0.0f;
	fHoleOffset = 0.0f;
	//}}AFX_DATA_INIT
}


void HoleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HoleDialog)
	DDX_Text(pDX, IDC_EDIT_SPACING, fHoleSpacing);
	DDX_Text(pDX, IDC_EDIT_OFFSET, fHoleOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HoleDialog, CDialog)
	//{{AFX_MSG_MAP(HoleDialog)
	ON_EN_CHANGE(IDC_EDIT_OFFSET, OnChangeEditOffset)
	ON_EN_CHANGE(IDC_EDIT_SPACING, OnChangeEditSpacing)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HoleDialog message handlers

void HoleDialog::OnChangeEditOffset() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}

void HoleDialog::OnChangeEditSpacing() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}
