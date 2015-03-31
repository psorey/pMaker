#if !defined(AFX_HOLEDIALOG_H__A348DAC0_4980_11D5_8E1B_00E09871B324__INCLUDED_)
#define AFX_HOLEDIALOG_H__A348DAC0_4980_11D5_8E1B_00E09871B324__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HoleDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// HoleDialog dialog

class HoleDialog : public CDialog
{
// Construction
public:
	HoleDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(HoleDialog)
	enum { IDD = IDD_HOLE_DIALOG };
	float	fHoleSpacing;
	float	fHoleOffset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HoleDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(HoleDialog)
	afx_msg void OnChangeEditOffset();
	afx_msg void OnChangeEditSpacing();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOLEDIALOG_H__A348DAC0_4980_11D5_8E1B_00E09871B324__INCLUDED_)
