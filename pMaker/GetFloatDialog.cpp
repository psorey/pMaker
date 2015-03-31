// GetFloatDialog.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "GetFloatDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GetFloatDialog dialog


GetFloatDialog::GetFloatDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GetFloatDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(GetFloatDialog)
	fFloat = 0.0f;
	//}}AFX_DATA_INIT
}


void GetFloatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GetFloatDialog)
	DDX_Text(pDX, IDC_FLOAT_EDIT, fFloat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GetFloatDialog, CDialog)
	//{{AFX_MSG_MAP(GetFloatDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GetFloatDialog message handlers
