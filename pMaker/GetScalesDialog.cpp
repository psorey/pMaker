// GetScalesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "GetScalesDialog.h"


// GetScalesDialog dialog

IMPLEMENT_DYNAMIC(GetScalesDialog, CDialog)

GetScalesDialog::GetScalesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(GetScalesDialog::IDD, pParent)
    , fSecScale(0)
    , fCentScale(0)
    , fThickness(0)
{

}

GetScalesDialog::~GetScalesDialog()
{
}


void GetScalesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GetScalesDialog)
	DDX_Text(pDX, IDC_CENT_SCALE, fCentScale);
	DDX_Text(pDX, IDC_SEC_SCALE, fSecScale);
	DDX_Text(pDX, IDC_THICKNESS, fThickness);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GetScalesDialog, CDialog)
END_MESSAGE_MAP()


// GetScalesDialog message handlers
