#if !defined(AFX_ONEINTEGERDLG_H__9072830E_CB61_4B46_A495_661DEF0874E4__INCLUDED_)
#define AFX_ONEINTEGERDLG_H__9072830E_CB61_4B46_A495_661DEF0874E4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OneIntegerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// OneIntegerDlg dialog

class OneIntegerDlg : public CDialog
{
// Construction
public:
	OneIntegerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(OneIntegerDlg)
	enum { IDD = IDD_GET_ONE_INTEGER_DLG };
	int		fInteger;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OneIntegerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OneIntegerDlg)
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONEINTEGERDLG_H__9072830E_CB61_4B46_A495_661DEF0874E4__INCLUDED_)
