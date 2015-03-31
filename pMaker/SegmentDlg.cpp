// SegmentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "SegmentDlg.h"
#include "pMakerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSegmentDlg dialog


CSegmentDlg::CSegmentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSegmentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSegmentDlg)
	fSegment = 0;
	fSide = 0;
	//}}AFX_DATA_INIT
}

CSegmentDlg::CSegmentDlg(CView *pview)
{
   fSegment = 0;
   fSide    = 0;
   fView    = (CpMakerView *)pview;
}

CSegmentDlg::~CSegmentDlg(void)
{
   fView->fSegmentDlg = NULL;
}

void CSegmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSegmentDlg)
	DDX_Text(pDX, IDC_EDIT_SEGMENT, fSegment);
	DDX_Text(pDX, IDC_EDIT_SIDE, fSide);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSegmentDlg, CDialog)
	//{{AFX_MSG_MAP(CSegmentDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSegmentDlg message handlers
BOOL
CSegmentDlg::Create()
{
    return CDialog::Create(CSegmentDlg::IDD);
}