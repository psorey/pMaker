// OneIntegerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "OneIntegerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OneIntegerDlg dialog


OneIntegerDlg::OneIntegerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OneIntegerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(OneIntegerDlg)
	fInteger = 0;
	//}}AFX_DATA_INIT
}


void OneIntegerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OneIntegerDlg)
	DDX_Text(pDX, IDC_EDIT1, fInteger);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OneIntegerDlg, CDialog)
	//{{AFX_MSG_MAP(OneIntegerDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OneIntegerDlg message handlers

void OneIntegerDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if(pNMUpDown->iDelta < 0) 
		fInteger++;
	else
		fInteger--;

	// fix out of range errors...
	if(fInteger < 0) fInteger = 0; 

	 UpdateData(FALSE);
	
	//fMakerView->makeLoftObject();
	
	*pResult = 0;
}
