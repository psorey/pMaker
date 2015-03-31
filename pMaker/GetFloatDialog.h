#if !defined(AFX_GETFLOATDIALOG_H__2C2A16E0_487F_11D5_8E1B_00E09871B324__INCLUDED_)
#define AFX_GETFLOATDIALOG_H__2C2A16E0_487F_11D5_8E1B_00E09871B324__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GetFloatDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GetFloatDialog dialog

class GetFloatDialog : public CDialog
{
// Construction
public:
	GetFloatDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GetFloatDialog)
	enum { IDD = IDD_GET_FLOAT_DIALOG };
	float	fFloat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GetFloatDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GetFloatDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETFLOATDIALOG_H__2C2A16E0_487F_11D5_8E1B_00E09871B324__INCLUDED_)
