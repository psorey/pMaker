// GetStringDialog.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "GetStringDialog.h"


// GetStringDialog dialog

IMPLEMENT_DYNAMIC(GetStringDialog, CDialog)

GetStringDialog::GetStringDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GetStringDialog::IDD, pParent)
{

}

GetStringDialog::~GetStringDialog()
{
}

void GetStringDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GetStringDialog)
	DDX_Text(pDX, IDC_PREFIX, fString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GetStringDialog, CDialog)
END_MESSAGE_MAP()


// GetStringDialog message handlers
