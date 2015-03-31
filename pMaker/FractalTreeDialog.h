#pragma once
#include "afxwin.h"
#include "FractalTreeMaker.h"
#include "FractalTreeSpec.h"


class FractalTreeDialog : public CDialog
{
	DECLARE_DYNAMIC(FractalTreeDialog)

public:
	FractalTreeDialog(CWnd* pParent = NULL);   // standard constructor
    FractalTreeDialog(FractalTreeSpec *fts);
	virtual ~FractalTreeDialog();

// Dialog Data
	//{{AFX_DATA(FractalTreeDialog)
	enum { IDD = IDD_FRACTAL_TREE_DIALOG };

    FractalTreeSpec *fFractalTreeSpec;
    int              fNumLevels;
    float            fLeftABRatio;
    float            fRightABRatio;
    float            fInfluenceDirection;
    
    BOOL            fUseCenterlineScale;
    BOOL            fUseThickness;
    BOOL            fUseInfluence; 
    BOOL            fUseRot;
    BOOL	        fUseSpiralEffect;
	BOOL	        fUseLinearEffect;
    BOOL            fUseExponentialEffect;

    float           fLCentScale[MAX_LEVELS];
    float           fRCentScale[MAX_LEVELS];
    float           fLThick[MAX_LEVELS];
    float           fRThick[MAX_LEVELS];
    float           fLRot[MAX_LEVELS];
    float           fRRot[MAX_LEVELS];
    float           fLInfl[MAX_LEVELS];
    float           fRInfl[MAX_LEVELS];

    CString         fLeftFilename;
    CString         fRightFilename;
    CEdit           fLeftEdit;
    CEdit           fRightEdit;


	//}}AFX_DATA
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CString GetFilename(CString def_extension, CString wildcard, CString dialog_title);
    afx_msg void OnBnClickedBrowseBranch();
    afx_msg void OnBnClickedBrowseBranch2();
    afx_msg void OnEnChangeFactorEdit();
    afx_msg void OnBnClickedBrowseBranch3();
    float fScaleFactor;
    afx_msg void OnBnClickedButton1();
    afx_msg void OnSaveFractalTreeSpec();
	afx_msg void OnEnChangeLscale1();
	afx_msg void OnEnChangeLscale2();
	afx_msg void OnEnChangeLscale3();
	afx_msg void OnEnChangeLscale4();
	afx_msg void OnEnChangeLscale5();
	afx_msg void OnEnChangeLscale6();
	afx_msg void OnEnChangeLscale7();
	afx_msg void OnEnChangeLscale8();
	afx_msg void OnEnChangeLscale9();
	afx_msg void OnEnChangeRscale1();
	afx_msg void OnEnChangeRscale2();
	afx_msg void OnEnChangeRscale3();
	afx_msg void OnEnChangeRscale4();
	afx_msg void OnEnChangeRscale5();
	afx_msg void OnEnChangeRscale6();
	afx_msg void OnEnChangeRscale7();
	afx_msg void OnEnChangeRscale8();
	afx_msg void OnEnChangeRscale9();
	afx_msg void OnEnChangeLthick1();
	afx_msg void OnEnChangeLthick2();
	afx_msg void OnEnChangeLthick3();
	afx_msg void OnEnChangeLthick4();
	afx_msg void OnEnChangeLthick5();
	afx_msg void OnEnChangeLthick6();
	afx_msg void OnEnChangeLthick7();
	afx_msg void OnEnChangeLthick8();
	afx_msg void OnEnChangeLthick9();
	afx_msg void OnEnChangeRthick1();
	afx_msg void OnEnChangeRthick2();
	afx_msg void OnEnChangeRthick3();
	afx_msg void OnEnChangeRthick4();
	afx_msg void OnEnChangeRthick5();
	afx_msg void OnEnChangeRthick6();
	afx_msg void OnEnChangeRthick7();
	afx_msg void OnEnChangeRthick8();
	afx_msg void OnEnChangeRthick9();
};
