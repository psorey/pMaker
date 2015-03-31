// FractalTreeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "FractalTreeDialog.h"

// FractalTreeDialog dialog

IMPLEMENT_DYNAMIC(FractalTreeDialog, CDialog)

FractalTreeDialog::FractalTreeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(FractalTreeDialog::IDD, pParent)
    , fInfluenceIntensity(0)
    , fInfluenceDirection(0)
    , fCenterlineScale(0)
    , fUseLinearEffect(false)
    , fUseExponentialEffect(false)

{	
    //{{AFX_DATA_INIT(FractalTreeDialog)
    fNumLevels = 0;
    fRotationAngle = 0;
    fScaleFactor = 0;
	//}}AFX_DATA_INIT

}



FractalTreeDialog::~FractalTreeDialog()
{
}


void FractalTreeDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(FractalTreeDialog)
    DDX_Text(pDX, IDC_EDIT1, fNumLevels);
    DDX_Text(pDX, IDC_EDIT2, fRotationAngle);
    DDX_Text(pDX, IDC_EDIT3, fScaleFactor);
    DDX_Text(pDX, IDC_EDIT4, fInfluenceIntensity);
    DDX_Text(pDX, IDC_EDIT5, fInfluenceDirection);
    DDX_Text(pDX, IDC_EDIT6, fCenterlineScale);
    DDX_Text(pDX, IDC_LEFT_BRANCH, fLeftFilename_str);
    DDX_Text(pDX, IDC_EDIT8, fRightFilename_str);
    DDX_Check(pDX, IDC_CHECK_SPIRAL, fUseSpiralEffect);    
    DDX_Check(pDX, IDC_CHECK_EXPONENTIAL, fUseExponentialEffect);    
    DDX_Text(pDX, IDC_PSCALE_1, fPScale[0]);
    DDX_Text(pDX, IDC_THICK_1,  fThick[0]);
    DDX_Text(pDX, IDC_PSCALE_2, fPScale[1]);
    DDX_Text(pDX, IDC_THICK_2,  fThick[1]);
    DDX_Text(pDX, IDC_PSCALE_3, fPScale[2]);
    DDX_Text(pDX, IDC_THICK_3,  fThick[2]);
    DDX_Text(pDX, IDC_PSCALE_4, fPScale[3]);
    DDX_Text(pDX, IDC_THICK_4,  fThick[3]);
    DDX_Text(pDX, IDC_PSCALE_5, fPScale[4]);
    DDX_Text(pDX, IDC_THICK_5,  fThick[4]);
    DDX_Text(pDX, IDC_PSCALE_6, fPScale[5]);
    DDX_Text(pDX, IDC_THICK_6,  fThick[5]);
    DDX_Text(pDX, IDC_PSCALE_7, fPScale[6]);
    DDX_Text(pDX, IDC_THICK_7,  fThick[6]);



    //}}AFX_DATA_MAP
 
}


BEGIN_MESSAGE_MAP(FractalTreeDialog, CDialog)

END_MESSAGE_MAP()




