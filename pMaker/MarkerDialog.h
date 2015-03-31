#if !defined(AFX_MARKERDIALOG_H__E9FA78F9_EC85_42C7_A193_8C50161ACBCF__INCLUDED_)
#define AFX_MARKERDIALOG_H__E9FA78F9_EC85_42C7_A193_8C50161ACBCF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MarkerDialog.h : header file
//

class CpMakerView;
class SoSphere;
class SoTransform;
class SoMaterial;
class SoCoordinate3;
class SoSeparator;


/////////////////////////////////////////////////////////////////////////////
// MarkerDialog dialog

class MarkerDialog : public CDialog
{
// Construction
public:
	MarkerDialog(CWnd* pParent = NULL);   // standard constructor
   MarkerDialog(CView *pview);
   BOOL Create();
   ~MarkerDialog(void);
// Dialog Data
	//{{AFX_DATA(MarkerDialog)
	enum { IDD = IDD_SHAPE_MARKER_DIALOG };
	UINT	fSegment;
	UINT	fSize;
	//}}AFX_DATA

   CpMakerView*   fView;
   int            fNumMarkers;
   SoSphere*      fMarkerSphere[12];
   SoTransform*   fMarkerTransform[12];
   SoMaterial*    fMarkerMaterial[12]; 
   SoSeparator*   fMarkerSeparator[12];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MarkerDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MarkerDialog)
	afx_msg void OnDeltaposSpinSegment(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpinSize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeSegment();
	afx_msg void OnChangeSize();
	afx_msg void OnChangeMarkerSize();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKERDIALOG_H__E9FA78F9_EC85_42C7_A193_8C50161ACBCF__INCLUDED_)
