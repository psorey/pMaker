#pragma once
#include "afxwin.h"


// FractalTreeDialog dialog

class FractalTreeDialog : public CDialog
{
	DECLARE_DYNAMIC(FractalTreeDialog)

public:
	FractalTreeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~FractalTreeDialog();

// Dialog Data
	//{{AFX_DATA(FractalTreeDialog)
	enum { IDD = IDD_FRACTAL_TREE_DIALOG };
	float	m_fScaleLowLimit;
	float	m_fScaleHighLimit;
	BOOL	fUseSpiralEffect;
	BOOL	fUseLinearEffect;
    BOOL    fUseExponentialEffect;
    int     fNumLevels;
    float   fRotationAngle;
    float   fScaleFactor;
    char    fPrototypeName;
    float   fInfluenceIntensity;
    float   fDirection; //
    float   fInfluenceDirection;
    float   fCenterlineScale;
   
    float fPScale[7];
    float fThick[7];


    CString fLeftFilename_str;
    CString fRightFilename_str;
    CEdit   fLeftEdit;
    CEdit   fRightEdit;


	//}}AFX_DATA
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:


};
