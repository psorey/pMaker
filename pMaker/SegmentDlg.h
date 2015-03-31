#if !defined(AFX_SEGMENTDLG_H__864AC536_83B9_4BC1_A35F_BBE87850DA34__INCLUDED_)
#define AFX_SEGMENTDLG_H__864AC536_83B9_4BC1_A35F_BBE87850DA34__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SegmentDlg.h : header file
//

class CpMakerView;

/////////////////////////////////////////////////////////////////////////////
// CSegmentDlg dialog

class CSegmentDlg : public CDialog
{
// Construction
public:
	CSegmentDlg(CWnd* pParent = NULL);   // standard constructor
   CSegmentDlg(CView *pview);
   BOOL Create();
   ~CSegmentDlg(void);
// Dialog Data
	//{{AFX_DATA(CSegmentDlg)
	enum { IDD = IDD_SEGMENT_DLG };
	int		fSegment;
	int		fSide;
	//}}AFX_DATA

   CpMakerView*   fView;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSegmentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSegmentDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEGMENTDLG_H__864AC536_83B9_4BC1_A35F_BBE87850DA34__INCLUDED_)
