#if !defined(AFX_WHICHBRANCHDIALOG_H__C9B724A8_F662_4A8E_9A6F_18A9D9FAAB11__INCLUDED_)
#define AFX_WHICHBRANCHDIALOG_H__C9B724A8_F662_4A8E_9A6F_18A9D9FAAB11__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// WhichBranchDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWhichBranchDialog dialog

class CWhichBranchDialog : public CDialog
{
// Construction
public:
	bool fIsLeft;
	CWhichBranchDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWhichBranchDialog)
	enum { IDD = IDD_WHICH_BRANCH_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWhichBranchDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWhichBranchDialog)
	afx_msg void OnRadioLeft();
	afx_msg void OnRadioRight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHICHBRANCHDIALOG_H__C9B724A8_F662_4A8E_9A6F_18A9D9FAAB11__INCLUDED_)
