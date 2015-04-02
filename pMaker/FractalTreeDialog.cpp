// FractalTreeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "FractalTreeDialog.h"

// FractalTreeDialog dialog

IMPLEMENT_DYNAMIC(FractalTreeDialog, CDialog)

FractalTreeDialog::FractalTreeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(FractalTreeDialog::IDD, pParent)
//    , fInfluenceIntensity(0)

, fScaleFactor(0)
{	
    //{{AFX_DATA_INIT(FractalTreeDialog)

    fUseCenterlineScale     = FALSE;
    fUseThickness           = FALSE;
    fUseInfluence           = FALSE; 
    fUseRot                 = FALSE;
    fUseSpiralEffect        = FALSE;
	fUseLinearEffect        = FALSE;
    fUseExponentialEffect   = FALSE;

	//}}AFX_DATA_INIT
}


FractalTreeDialog::FractalTreeDialog(FractalTreeSpec * fts)
{
    fFractalTreeSpec = fts;  // pointer to FractalTreeSpec object we're working on
}


FractalTreeDialog::~FractalTreeDialog()
{

}


void FractalTreeDialog::DoDataExchange(CDataExchange* pDX)
{
	TRACE("do data exchange\n");
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(FractalTreeDialog)

    DDX_Text(pDX, IDC_NUM_LEVELS,        fNumLevels);
    DDX_Text(pDX, IDC_LEFT_ABR,          fLeftABRatio);
    DDX_Text(pDX, IDC_RIGHT_ABR,         fRightABRatio);

    DDX_Text(pDX, IDC_INFL_DIR,          fInfluenceDirection);
    DDX_Text(pDX, IDC_LEFT_BRANCH,       fLeftFilename);
    DDX_Text(pDX, IDC_EDIT8,             fRightFilename);

    DDX_Check(pDX, IDC_CHECK_LINEAR,      fUseLinearEffect);
    DDX_Check(pDX, IDC_CHECK_SPIRAL,      fUseSpiralEffect);
    DDX_Check(pDX, IDC_CHECK_EXPONENTIAL, fUseExponentialEffect);
    DDX_Check(pDX, IDC_USE_INFL,          fUseInfluence);
    DDX_Check(pDX, IDC_USE_ROTATION,      fUseRot);
    DDX_Check(pDX, IDC_USE_THICKNESS,     fUseThickness);
    DDX_Check(pDX, IDC_USE_PATH_SCALE,    fUseCenterlineScale);

    DDX_Text(pDX, IDC_LSCALE_1,  fLCentScale[0]);
    DDX_Text(pDX, IDC_RSCALE_1,  fRCentScale[0]);
    DDX_Text(pDX, IDC_LTHICK_1,  fLThick[0]);
    DDX_Text(pDX, IDC_RTHICK_1,  fRThick[0]);
    DDX_Text(pDX, IDC_LROT_1,    fLRot[0]);
    DDX_Text(pDX, IDC_RROT_1,    fRRot[0]);
    DDX_Text(pDX, IDC_LINFL_1,   fLInfl[0]);
    DDX_Text(pDX, IDC_RINFL_1,   fRInfl[0]);

    DDX_Text(pDX, IDC_LSCALE_2,  fLCentScale[1]);
    DDX_Text(pDX, IDC_RSCALE_2,  fRCentScale[1]);
    DDX_Text(pDX, IDC_LTHICK_2,  fLThick[1]);
    DDX_Text(pDX, IDC_RTHICK_2,  fRThick[1]);
    DDX_Text(pDX, IDC_LROT_2,    fLRot[1]);
    DDX_Text(pDX, IDC_RROT_2,    fRRot[1]);
    DDX_Text(pDX, IDC_LINFL_2,   fLInfl[1]);
    DDX_Text(pDX, IDC_RINFL_2,   fRInfl[1]);

    DDX_Text(pDX, IDC_LSCALE_3,  fLCentScale[2]);
    DDX_Text(pDX, IDC_RSCALE_3,  fRCentScale[2]);
    DDX_Text(pDX, IDC_LTHICK_3,  fLThick[2]);
    DDX_Text(pDX, IDC_RTHICK_3,  fRThick[2]);
    DDX_Text(pDX, IDC_LROT_3,    fLRot[2]);
    DDX_Text(pDX, IDC_RROT_3,    fRRot[2]);
    DDX_Text(pDX, IDC_LINFL_3,   fLInfl[2]);
    DDX_Text(pDX, IDC_RINFL_3,   fRInfl[2]);

    DDX_Text(pDX, IDC_LSCALE_4,  fLCentScale[3]);
    DDX_Text(pDX, IDC_RSCALE_4,  fRCentScale[3]);
    DDX_Text(pDX, IDC_LTHICK_4,  fLThick[3]);
    DDX_Text(pDX, IDC_RTHICK_4,  fRThick[3]);
    DDX_Text(pDX, IDC_LROT_4,    fLRot[3]);
    DDX_Text(pDX, IDC_RROT_4,    fRRot[3]);
    DDX_Text(pDX, IDC_LINFL_4,   fLInfl[3]);
    DDX_Text(pDX, IDC_RINFL_4,   fRInfl[3]);

    DDX_Text(pDX, IDC_LSCALE_5,  fLCentScale[4]);
    DDX_Text(pDX, IDC_RSCALE_5,  fRCentScale[4]);
    DDX_Text(pDX, IDC_LTHICK_5,  fLThick[4]);
    DDX_Text(pDX, IDC_RTHICK_5,  fRThick[4]);
    DDX_Text(pDX, IDC_LROT_5,    fLRot[4]);
    DDX_Text(pDX, IDC_RROT_5,    fRRot[4]);
    DDX_Text(pDX, IDC_LINFL_5,   fLInfl[4]);
    DDX_Text(pDX, IDC_RINFL_5,   fRInfl[4]);

    DDX_Text(pDX, IDC_LSCALE_6,  fLCentScale[5]);
    DDX_Text(pDX, IDC_RSCALE_6,  fRCentScale[5]);
    DDX_Text(pDX, IDC_LTHICK_6,  fLThick[5]);
    DDX_Text(pDX, IDC_RTHICK_6,  fRThick[5]);
    DDX_Text(pDX, IDC_LROT_6,    fLRot[5]);
    DDX_Text(pDX, IDC_RROT_6,    fRRot[5]);
    DDX_Text(pDX, IDC_LINFL_6,   fLInfl[5]);
    DDX_Text(pDX, IDC_RINFL_6,   fRInfl[5]);

    DDX_Text(pDX, IDC_LSCALE_7,  fLCentScale[6]);
    DDX_Text(pDX, IDC_RSCALE_7,  fRCentScale[6]);
    DDX_Text(pDX, IDC_LTHICK_7,  fLThick[6]);
    DDX_Text(pDX, IDC_RTHICK_7,  fRThick[6]);
    DDX_Text(pDX, IDC_LROT_7,    fLRot[6]);
    DDX_Text(pDX, IDC_RROT_7,    fRRot[6]);
    DDX_Text(pDX, IDC_LINFL_7,   fLInfl[6]);
    DDX_Text(pDX, IDC_RINFL_7,   fRInfl[6]);

    DDX_Text(pDX, IDC_LSCALE_8,  fLCentScale[7]);
    DDX_Text(pDX, IDC_RSCALE_8,  fRCentScale[7]);
    DDX_Text(pDX, IDC_LTHICK_8,  fLThick[7]);
    DDX_Text(pDX, IDC_RTHICK_8,  fRThick[7]);
    DDX_Text(pDX, IDC_LROT_8,    fLRot[7]);
    DDX_Text(pDX, IDC_RROT_8,    fRRot[7]);
    DDX_Text(pDX, IDC_LINFL_8,   fLInfl[7]);
    DDX_Text(pDX, IDC_RINFL_8,   fRInfl[7]);

    DDX_Text(pDX, IDC_LSCALE_9,  fLCentScale[8]);
    DDX_Text(pDX, IDC_RSCALE_9,  fRCentScale[8]);
    DDX_Text(pDX, IDC_LTHICK_9,  fLThick[8]);
    DDX_Text(pDX, IDC_RTHICK_9,  fRThick[8]);
    DDX_Text(pDX, IDC_LROT_9,    fLRot[8]);
    DDX_Text(pDX, IDC_RROT_9,    fRRot[8]);
    DDX_Text(pDX, IDC_LINFL_9,   fLInfl[8]);
    DDX_Text(pDX, IDC_RINFL_9,   fRInfl[8]);
    DDX_Text(pDX, IDC_FACTOR_EDIT, fScaleFactor);
    //}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(FractalTreeDialog, CDialog)

    ON_BN_CLICKED(IDC_BROWSE_BRANCH,  &FractalTreeDialog::OnBnClickedBrowseBranch)
    ON_BN_CLICKED(IDC_BROWSE_BRANCH2, &FractalTreeDialog::OnBnClickedBrowseBranch2)
    ON_EN_CHANGE(IDC_FACTOR_EDIT,     &FractalTreeDialog::OnEnChangeFactorEdit)
    ON_BN_CLICKED(IDC_BROWSE_BRANCH3, &FractalTreeDialog::OnBnClickedBrowseBranch3)
    ON_BN_CLICKED(IDC_BUTTON1, &FractalTreeDialog::OnBnClickedButton1) // load file
    ON_BN_CLICKED(IDC_BUTTON2, &FractalTreeDialog::OnSaveFractalTreeSpec)
	ON_EN_CHANGE(IDC_LSCALE_1, &FractalTreeDialog::OnEnChangeLscale1)
	ON_EN_CHANGE(IDC_LSCALE_2, &FractalTreeDialog::OnEnChangeLscale2)
	ON_EN_CHANGE(IDC_LSCALE_3, &FractalTreeDialog::OnEnChangeLscale3)
	ON_EN_CHANGE(IDC_LSCALE_4, &FractalTreeDialog::OnEnChangeLscale4)
	ON_EN_CHANGE(IDC_LSCALE_5, &FractalTreeDialog::OnEnChangeLscale5)
	ON_EN_CHANGE(IDC_LSCALE_6, &FractalTreeDialog::OnEnChangeLscale6)
	ON_EN_CHANGE(IDC_LSCALE_7, &FractalTreeDialog::OnEnChangeLscale7)
	ON_EN_CHANGE(IDC_LSCALE_8, &FractalTreeDialog::OnEnChangeLscale8)
	ON_EN_CHANGE(IDC_LSCALE_9, &FractalTreeDialog::OnEnChangeLscale9)
	ON_EN_CHANGE(IDC_RSCALE_1, &FractalTreeDialog::OnEnChangeRscale1)
	ON_EN_CHANGE(IDC_RSCALE_2, &FractalTreeDialog::OnEnChangeRscale2)
	ON_EN_CHANGE(IDC_RSCALE_3, &FractalTreeDialog::OnEnChangeRscale3)
	ON_EN_CHANGE(IDC_RSCALE_4, &FractalTreeDialog::OnEnChangeRscale4)
	ON_EN_CHANGE(IDC_RSCALE_5, &FractalTreeDialog::OnEnChangeRscale5)
	ON_EN_CHANGE(IDC_RSCALE_6, &FractalTreeDialog::OnEnChangeRscale6)
	ON_EN_CHANGE(IDC_RSCALE_7, &FractalTreeDialog::OnEnChangeRscale7)
	ON_EN_CHANGE(IDC_RSCALE_8, &FractalTreeDialog::OnEnChangeRscale8)
	ON_EN_CHANGE(IDC_RSCALE_9, &FractalTreeDialog::OnEnChangeRscale9)
	ON_EN_CHANGE(IDC_LTHICK_1, &FractalTreeDialog::OnEnChangeLthick1)
	ON_EN_CHANGE(IDC_LTHICK_2, &FractalTreeDialog::OnEnChangeLthick2)
	ON_EN_CHANGE(IDC_LTHICK_3, &FractalTreeDialog::OnEnChangeLthick3)
	ON_EN_CHANGE(IDC_LTHICK_4, &FractalTreeDialog::OnEnChangeLthick4)
	ON_EN_CHANGE(IDC_LTHICK_5, &FractalTreeDialog::OnEnChangeLthick5)
	ON_EN_CHANGE(IDC_LTHICK_6, &FractalTreeDialog::OnEnChangeLthick6)
	ON_EN_CHANGE(IDC_LTHICK_7, &FractalTreeDialog::OnEnChangeLthick7)
	ON_EN_CHANGE(IDC_LTHICK_8, &FractalTreeDialog::OnEnChangeLthick8)
	ON_EN_CHANGE(IDC_LTHICK_9, &FractalTreeDialog::OnEnChangeLthick9)
	ON_EN_CHANGE(IDC_RTHICK_1, &FractalTreeDialog::OnEnChangeRthick1)
	ON_EN_CHANGE(IDC_RTHICK_2, &FractalTreeDialog::OnEnChangeRthick2)
	ON_EN_CHANGE(IDC_RTHICK_3, &FractalTreeDialog::OnEnChangeRthick3)
	ON_EN_CHANGE(IDC_RTHICK_4, &FractalTreeDialog::OnEnChangeRthick4)
	ON_EN_CHANGE(IDC_RTHICK_5, &FractalTreeDialog::OnEnChangeRthick5)
	ON_EN_CHANGE(IDC_RTHICK_6, &FractalTreeDialog::OnEnChangeRthick6)
	ON_EN_CHANGE(IDC_RTHICK_7, &FractalTreeDialog::OnEnChangeRthick7)
	ON_EN_CHANGE(IDC_RTHICK_8, &FractalTreeDialog::OnEnChangeRthick8)
	ON_EN_CHANGE(IDC_RTHICK_9, &FractalTreeDialog::OnEnChangeRthick9)
	ON_EN_CHANGE(IDC_LEFT_ABR, &FractalTreeDialog::OnEnChangeLeftAbr)
	ON_EN_CHANGE(IDC_RIGHT_ABR, &FractalTreeDialog::OnEnChangeRightAbr)
	ON_EN_CHANGE(IDC_NUM_LEVELS, &FractalTreeDialog::OnEnChangeNumLevels)
	ON_EN_CHANGE(IDC_LEFT_BRANCH, &FractalTreeDialog::OnEnChangeLeftBranch)
	ON_EN_CHANGE(IDC_EDIT8, &FractalTreeDialog::OnEnChangeEdit8)
END_MESSAGE_MAP()


void FractalTreeDialog::OnBnClickedBrowseBranch()
{
    BOOL    bAddFileDialog = TRUE;
    LPCTSTR lpszFilter = NULL;
    LPCTSTR lpszDefExt = NULL;
    LPCTSTR lpszFileName = "*left.ldf";
    DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CWnd    *pParentWnd = NULL;
    
    CFileDialog addFileDialog(bAddFileDialog,
        lpszDefExt, lpszFileName, dwFlags,
        lpszFilter, pParentWnd);

    addFileDialog.m_ofn.lpstrTitle = LPCTSTR("read in a Coordinate3 file...");
    int nModal = addFileDialog.DoModal();

   if (nModal == IDOK) {
		fLeftFilename = addFileDialog.GetFileName();
        SetCurrentDirectory(addFileDialog.GetFolderPath());
   }
   	UpdateData(FALSE);
}


void FractalTreeDialog::OnBnClickedBrowseBranch2()
{
		   TRACE("onBnClicked");
    BOOL    bAddFileDialog = TRUE;
    LPCTSTR lpszFilter = NULL;
    LPCTSTR lpszDefExt = NULL;
    LPCTSTR lpszFileName = "*_right.ldf";
    DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CWnd    *pParentWnd = NULL;
    
    CFileDialog addFileDialog(bAddFileDialog,
        lpszDefExt, lpszFileName, dwFlags,
        lpszFilter, pParentWnd);

    addFileDialog.m_ofn.lpstrTitle = LPCTSTR("read right branch file...");
    int nModal = addFileDialog.DoModal();

   if (nModal == IDOK) {
		fRightFilename = addFileDialog.GetFileName();
        SetCurrentDirectory(addFileDialog.GetFolderPath());
   }
   	UpdateData(FALSE);
}


void FractalTreeDialog::OnEnChangeFactorEdit()
{
   TRACE("onEnChangeFactorEdit");
   UpdateData(TRUE);
}


void FractalTreeDialog::OnBnClickedBrowseBranch3()  // multiply influences
{
TRACE("onBrowseBranch 3\n");   
    for(int i = 0; i < 9; i++) {
        fLInfl[i] *= fScaleFactor;
        fRInfl[i] *= fScaleFactor;
    }
   	UpdateData(FALSE);
}


CString FractalTreeDialog::GetFilename(CString def_extension, CString wildcard, CString dialog_title)
{
    BOOL    bAddFileDialog = TRUE;
    LPCTSTR lpszFilter = NULL;
    LPCTSTR lpszDefExt = def_extension.GetString(); // "_prototype.iv";
    LPCTSTR lpszFileName = wildcard.GetString();    // "*_prototype.iv";
    DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CWnd    *pParentWnd = NULL;
 
    CFileDialog addFileDialog(bAddFileDialog,
        lpszDefExt, lpszFileName, dwFlags,
        lpszFilter, pParentWnd);

    addFileDialog.m_ofn.lpstrTitle = dialog_title.GetString();
    int nModal = addFileDialog.DoModal();
    CString m_strAddFile;
    if (nModal != IDOK) return CString("");
    m_strAddFile = addFileDialog.GetFileName();
    SetCurrentDirectory(addFileDialog.GetFolderPath());
    return m_strAddFile;
}

void FractalTreeDialog::OnSaveFractalTreeSpec()
{
		TRACE("saving 2 file...\n");
    CString filename = this->GetFilename(".fts", "*.fts", "Save the Fractal Tree Spec...");
    CFile theFile;
    theFile.Open(_T(filename), CFile::modeCreate | CFile::modeWrite);
    CArchive archive(&theFile, CArchive::store);
    fFractalTreeSpec->Serialize(archive);
    archive.Close();
    theFile.Close();
}


void FractalTreeDialog::OnBnClickedButton1()
{
	TRACE("loading file...\n");
    CString filename = this->GetFilename(".fts", "*.fts", "Load a Fractal Tree Spec...");
    CFile theFile;
    theFile.Open(_T(filename), CFile::modeRead );
    CArchive archive(&theFile, CArchive::load);
    fFractalTreeSpec->Serialize(archive);
    archive.Close();
    theFile.Close();
}

void FractalTreeDialog::OnEnChangeLscale1()
{
  UpdateData(TRUE);
      fFractalTreeSpec->fLCentScale[0] = fLCentScale[0];
}


void FractalTreeDialog::OnEnChangeLscale2()
{
    UpdateData(TRUE);	
    fFractalTreeSpec->fLCentScale[1] = fLCentScale[1];
}


void FractalTreeDialog::OnEnChangeLscale3()
{
  UpdateData(TRUE);
        fFractalTreeSpec->fLCentScale[2] = fLCentScale[2];
}


void FractalTreeDialog::OnEnChangeLscale4()
{
  UpdateData(TRUE);
      fFractalTreeSpec->fLCentScale[3] = fLCentScale[3];
}


void FractalTreeDialog::OnEnChangeLscale5()
{
  UpdateData(TRUE);
      fFractalTreeSpec->fLCentScale[4] = fLCentScale[4];
}


void FractalTreeDialog::OnEnChangeLscale6()
{
  UpdateData(TRUE);
      fFractalTreeSpec->fLCentScale[6] = fLCentScale[5];
}


void FractalTreeDialog::OnEnChangeLscale7()
{
  UpdateData(TRUE);
      fFractalTreeSpec->fLCentScale[6] = fLCentScale[6];}

void FractalTreeDialog::OnEnChangeLscale8()
{
  UpdateData(TRUE);
      fFractalTreeSpec->fLCentScale[7] = fLCentScale[7];}


void FractalTreeDialog::OnEnChangeLscale9()
{
  UpdateData(TRUE);
  fFractalTreeSpec->fLCentScale[8] = fLCentScale[8];
}


void FractalTreeDialog::OnEnChangeRscale1()
{
  UpdateData(TRUE); 
    fFractalTreeSpec->fRCentScale[0] = fRCentScale[0];
}


void FractalTreeDialog::OnEnChangeRscale2()
{
  UpdateData(TRUE);
    fFractalTreeSpec->fRCentScale[1] = fRCentScale[1];
}


void FractalTreeDialog::OnEnChangeRscale3()
{
  UpdateData(TRUE);
      fFractalTreeSpec->fRCentScale[2] = fRCentScale[2];
}


void FractalTreeDialog::OnEnChangeRscale4()
{
  UpdateData(TRUE);
      fFractalTreeSpec->fRCentScale[3] = fRCentScale[3];
}


void FractalTreeDialog::OnEnChangeRscale5()
{
  UpdateData(TRUE);
        fFractalTreeSpec->fRCentScale[4] = fRCentScale[4];

}


void FractalTreeDialog::OnEnChangeRscale6()
{
  UpdateData(TRUE);
        fFractalTreeSpec->fRCentScale[5] = fRCentScale[5];

}


void FractalTreeDialog::OnEnChangeRscale7()
{
  UpdateData(TRUE);
        fFractalTreeSpec->fRCentScale[6] = fRCentScale[6];

}


void FractalTreeDialog::OnEnChangeRscale8()
{
  UpdateData(TRUE);
        fFractalTreeSpec->fRCentScale[7] = fRCentScale[7];

}


void FractalTreeDialog::OnEnChangeRscale9()
{
  UpdateData(TRUE);
        fFractalTreeSpec->fRCentScale[8] = fRCentScale[8];

}


void FractalTreeDialog::OnEnChangeLthick1()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeLthick2()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeLthick3()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeLthick4()
{
  UpdateData(TRUE);
}

void FractalTreeDialog::OnEnChangeLthick5()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeLthick6()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeLthick7()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeLthick8()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeLthick9()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeRthick1()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeRthick2()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeRthick3()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeRthick4()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeRthick5()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeRthick6()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeRthick7()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeRthick8()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeRthick9()
{
  UpdateData(TRUE);
}


void FractalTreeDialog::OnEnChangeLeftAbr()
{
    UpdateData(TRUE);
    fFractalTreeSpec->fLeftABRatio = fLeftABRatio;
}


void FractalTreeDialog::OnEnChangeRightAbr()
{
    UpdateData(TRUE);
    fFractalTreeSpec->fRightABRatio = fRightABRatio;
}


void FractalTreeDialog::OnEnChangeNumLevels()
{
	UpdateData(TRUE);
    fFractalTreeSpec->fNumLevels = fNumLevels;
}


void FractalTreeDialog::OnEnChangeLeftBranch()
{
	UpdateData(TRUE);
    fFractalTreeSpec->fLeftFilename = fLeftFilename;
}


void FractalTreeDialog::OnEnChangeEdit8()  // change Right Filename
{
	UpdateData(TRUE);
    fFractalTreeSpec->fRightFilename = fRightFilename;
}
