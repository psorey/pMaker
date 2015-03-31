// ivDialogDlg.h : header file
//

// BEGIN_IVWGEN
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/Win/viewers/SoWinPlaneViewer.h>
// END_IVWGEN

class SoLineSet;
class SoPointSet;
class SoCoordinate3;
class SoSeparator;
class LineManip2;
class Grid;
class Interface;
class SoSensor;
class SoTransform;
class SoText2;
class SoEventCallback;


/////////////////////////////////////////////////////////////////////////////
// CIvDialogDlg dialog

// BEGIN_IVWGEN
class CIvDialogDlg : public CDialog
// END_IVWGEN
{
// Construction
public:
	CIvDialogDlg(CWnd* pParent = NULL);	// standard constructor
	CIvDialogDlg(CString title, SoCoordinate3* _coords, CView* view, BOOL _showSlope);
	BOOL Create();
	~CIvDialogDlg();

	CString title;
	BOOL showSlope;
	CView* m_pView;
	SoCoordinate3* coords;
	SoLineSet* line;
	SoPointSet* points;
	SoSeparator* root;
	LineManip2* lineManip;
	Interface *theInterface;
	SoTransform* screenTrans;
	Grid* grid;
	SoSeparator* textSep;
	SoSeparator* bgSep;
	
	int H_SCALE, V_SCALE;

	static void		mouseMoved(void *, SoEventCallback *);
	static void		keyPress(void *, SoEventCallback *);
	static void		setButtonDown(void *, SoEventCallback *);
	static void		setButtonUp(void *, SoEventCallback *eventCB);
   static void		viewerCB(void *, void *);
	static void		sensorCB(void*, SoSensor*);

	// Dialog Data
	//{{AFX_DATA(CIvDialogDlg)
	enum { IDD = IDD_IVDIALOG_DIALOG };
	BOOL	m_bClosed;
	float	m_fVertExaggerate;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIvDialogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CIvDialogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClear();
	afx_msg void OnLoad();
	afx_msg void OnSave();
	afx_msg void OnCheckClosed();
	afx_msg void OnDxf();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLoadBackground();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
