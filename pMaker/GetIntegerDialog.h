#if !defined(AFX_GETINTEGERDIALOG_H__4993D620_07FE_11D4_8E1B_E921A3CCEAF6__INCLUDED_)
#define AFX_GETINTEGERDIALOG_H__4993D620_07FE_11D4_8E1B_E921A3CCEAF6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GetIntegerDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetIntegerDialog dialog

class CpMakerView;

class CGetIntegerDialog : public CDialog
{
// Construction
public:
	CGetIntegerDialog(CWnd* pParent = NULL);   // standard constructor

	CGetIntegerDialog(CString title,
		CString upperLabel, CString lowerLabel, CpMakerView* view); // standard constructor
	int Create(void);
// Dialog Data
	//{{AFX_DATA(CGetIntegerDialog)
	enum { IDD = IDD_GET_INTEGER_DIALOG };
	int		        fInteger1;
	int		        fInteger2;
	//}}AFX_DATA

	CString	        fUpperLabel;
	CString	        fLowerLabel;
	int		        fInitialValue;
	CString	        fTitle;
	CpMakerView*    fMakerView;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetIntegerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGetIntegerDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeIntegerEdit();
	afx_msg void OnChangeIntegerEdit2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETINTEGERDIALOG_H__4993D620_07FE_11D4_8E1B_E921A3CCEAF6__INCLUDED_)
