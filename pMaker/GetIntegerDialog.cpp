// GetIntegerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "GetIntegerDialog.h"
#include "PMakerView.h"

#include <Inventor/nodes/SoCoordinate3.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetIntegerDialog dialog


/////////////////////////////////////////////////////////////////////////////
// CGetIntegerDialog dialog

CGetIntegerDialog::CGetIntegerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGetIntegerDialog::IDD, pParent)
{

}


CGetIntegerDialog::CGetIntegerDialog(CString title,
	CString upperLabel, CString lowerLabel, CpMakerView* pMakerView)
	: CDialog(CGetIntegerDialog::IDD)
{
	//{{AFX_DATA_INIT(CGetIntegerDialog)
	fInteger1 = 0;
	fInteger2 = 0;
	//}}AFX_DATA_INIT
	fTitle = title;
	fUpperLabel = upperLabel;
	fLowerLabel = lowerLabel;
	fMakerView = pMakerView;

}

int CGetIntegerDialog::Create(void)
{
	return CDialog::Create(CGetIntegerDialog::IDD);
}

void CGetIntegerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetIntegerDialog)
	DDX_Text(pDX, IDC_INTEGER_EDIT, fInteger1);
	DDX_Text(pDX, IDC_INTEGER_EDIT2, fInteger2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetIntegerDialog, CDialog)
	//{{AFX_MSG_MAP(CGetIntegerDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_EN_CHANGE(IDC_INTEGER_EDIT, OnChangeIntegerEdit)
	ON_EN_CHANGE(IDC_INTEGER_EDIT2, OnChangeIntegerEdit2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetIntegerDialog message handlers

BOOL CGetIntegerDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_STATIC_UPPER, LPCTSTR(fUpperLabel));
	SetDlgItemText(IDC_STATIC_LOWER, LPCTSTR(fLowerLabel));
	this->SetWindowText(fTitle);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGetIntegerDialog::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if(pNMUpDown->iDelta < 0) 
		fInteger1++;
	else
		fInteger1--;

	// fix out of range errors...
	if(fInteger1 < 0) fInteger1 = 0; 
//	if(fInteger1 >= fMakerView->fExtruder->fLoftPathCoords->point.getNum()) !!!
//		fInteger1 = fMakerView->fExtruder->fLoftPathCoords->point.getNum(); !!!
	 
	 UpdateData(FALSE);
	*pResult = 0;
	//fMakerView->makeLoftObject();
}

void CGetIntegerDialog::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if(pNMUpDown->iDelta < 0) 
		fInteger2++;
	else 
		fInteger2--;

	// fix out of range errors...
	if(fInteger2 < 0) fInteger2 = 0; 
//	if(fInteger2 >= fMakerView->fExtruder->fLoftPathCoords->point.getNum())
//		fInteger2 = fMakerView->fExtruder->fLoftPathCoords->point.getNum();	
	
	 UpdateData(FALSE);
	*pResult = 0;
	//fMakerView->makeLoftObject();
}

void CGetIntegerDialog::OnChangeIntegerEdit() 
{
	UpdateData(TRUE);
	// fix out of range errors...
	if(fInteger1 < 0) fInteger1 = 0; 
//	if(fInteger1 >= fMakerView->fExtruder->fLoftPathCoords->point.getNum())
//		fInteger1 = fMakerView->fExtruder->fLoftPathCoords->point.getNum();
	 
	UpdateData(FALSE);
//	fMakerView->makeLoftObject();
}

void CGetIntegerDialog::OnChangeIntegerEdit2() 
{
	UpdateData(TRUE);
	// fix out of range errors...
	if(fInteger2 < 0) fInteger2 = 0; 
//	if(fInteger2 >= fMakerView->fExtruder->fLoftPathCoords->point.getNum())  //!!!
//		fInteger2 = fMakerView->fExtruder->fLoftPathCoords->point.getNum();	 // !!!
	
	UpdateData(FALSE);
//	fMakerView->makeLoftObject();
}
