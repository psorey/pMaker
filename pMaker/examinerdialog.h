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
class PManip;
class PEntityMgr;
class PEntity;
class PNurbsCurve;


/////////////////////////////////////////////////////////////////////////////
// CExaminerDialog dialog

class CExaminerDialog : public CDialog //, public SoWinExaminerViewer
{
// Construction
public:
	CExaminerDialog(CWnd* pParent = NULL);	// standard constructor
	CExaminerDialog(CString title, SoCoordinate3* _coords, CView* view, BOOL _showSlope);
    CExaminerDialog(CString title, SoCoordinate3* _coords, SoCoordinate3* _splineCoords, CView* view);

	BOOL Create();
	~CExaminerDialog();

    SoWinPlaneViewer    *viewer;
	CString             title;
	BOOL                showSlope;
	CView               *m_pView;
	SoCoordinate3       *coords;
	SoCoordinate3       *splineCoords;
	SoLineSet           *line;
	SoPointSet          *points;
	SoSeparator         *root;
	LineManip2          *lineManip;
	Interface           *theInterface;
	SoTransform         *screenTrans;
	Grid                *grid;
	SoSeparator         *textSep;
	SoSeparator         *bgSep;
	PManip              *fManip;
	PNurbsCurve         *fNurbsCurve;
	
	int H_SCALE, V_SCALE;

	static void		mouseMoved(void *, SoEventCallback *);
	static void		keyPress(void *, SoEventCallback *);
	static void		setButtonDown(void *, SoEventCallback *);
	static void		setButtonUp(void *, SoEventCallback *eventCB);
    static void		viewerCB(void *, void *);
	static void		sensorCB(void*, SoSensor*);

	// Dialog Data
	//{{AFX_DATA(CExaminerDialog)
	enum { IDD = IDD_IVDIALOG_DIALOG };
	BOOL	m_bClosed;
	float	m_fVertExaggerate;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExaminerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CExaminerDialog)
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
	afx_msg void OnReverse();
	afx_msg void OnDxfOut();
    afx_msg void OnSplineButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnStnClickedViewer();
};
