// ivDialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "ivDialogDlg.h"  
#include "ReadDXF.h"

#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/Win/viewers/SoWinPlaneViewer.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/sensors/SoNodeSensor.h>


#include "LineManip.h"
#include "Grid.h"
#include "Interface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CIvDialogDlg dialog

CIvDialogDlg::CIvDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIvDialogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIvDialogDlg)
	m_bClosed = FALSE;
	m_fVertExaggerate = 0.0f;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CIvDialogDlg::CIvDialogDlg(CString _title, SoCoordinate3 *_coords, CView* view, BOOL _showSlope)
{
	coords = _coords;
	showSlope = _showSlope;
	m_fVertExaggerate = 1;
	title = _title;
	m_pView = view;
	m_bClosed = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CIvDialogDlg::~CIvDialogDlg()
{
	coords = NULL;
}

BOOL CIvDialogDlg::Create()
{
	return CDialog::Create(CIvDialogDlg::IDD);
}


void CIvDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIvDialogDlg)
	DDX_Check(pDX, IDC_CHECK_CLOSED, m_bClosed);
	DDX_Text(pDX, IDC_VERT_EXAG, m_fVertExaggerate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIvDialogDlg, CDialog)
	//{{AFX_MSG_MAP(CIvDialogDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_CHECK_CLOSED, OnCheckClosed)
	ON_BN_CLICKED(IDC_DXF, OnDxf)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_LOAD_BG, OnLoadBackground)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIvDialogDlg message handlers

BOOL CIvDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
/*
// BEGIN_IVWGEN
	static int cArgs[]= {
		TRUE,		// Decoration
		FALSE,	// URL Display
		FALSE,					// Viewpoints
		FALSE};		// URL Fetch

	CIvfApp::IvfInitSoWin( this );

	CWnd *pWnd = GetDlgItem(IDC_VIEWER) ;
	IvfCreateComponent(pWnd,(void *)cArgs);
// END_IVWGEN
*/
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	//ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	//ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(TRUE);
	//strAboutMenu.LoadString(IDS_ABOUTBOX);
	//if (!strAboutMenu.IsEmpty())
//	{
//		pSysMenu->AppendMenu(MF_SEPARATOR);
		//pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
//	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//////////////
	SetWindowText(title);

	root = new SoSeparator;
	line = new SoLineSet;
	points = new SoPointSet;
	SoDrawStyle* style = new SoDrawStyle;
	SoMaterial*  material = new SoMaterial;
	style->pointSize.setValue(7);
	material->diffuseColor.setValue(SbColor(1,.5,0));
	root->ref();

	//root->addChild(coords);
	//root->addChild(style);
	//root->addChild(line);
	//root->addChild(material);
	//root->addChild(points);

	SoNodeSensor* sensor = new SoNodeSensor(sensorCB, this);
	sensor->attach(coords);
	SoEventCallback *myEventCallback = new SoEventCallback;
	root->addChild(myEventCallback);

	myEventCallback->addEventCallback(
	  SoMouseButtonEvent::getClassTypeId(), 
	  setButtonDown, this);

	myEventCallback->addEventCallback(
		SoMouseButtonEvent::getClassTypeId(), 
		setButtonUp, this);

	myEventCallback->addEventCallback(
		SoLocation2Event::getClassTypeId(), 
		mouseMoved, this);

	//myEventCallback->addEventCallback(
	//	SoKeyboardEvent::getClassTypeId(), 
	//	keyPress, this);


	SoWinPlaneViewer* viewer = new SoWinPlaneViewer;
	viewer->setSceneGraph(root);
    viewer->addFinishCallback((SoWinViewerCB *)CIvDialogDlg::viewerCB,this);            
	viewer->viewAll();

	 H_SCALE = V_SCALE = 1;

    CFrameWnd *parentFrame = GetParentFrame();
    //CIvfComponent *theComponent = (CIvfComponent *)m_pComponent;
    
    // the one instance of Interface...
    theInterface = new Interface(parentFrame);
    
    screenTrans = new SoTransform;
    root->addChild(screenTrans);
	 bgSep = new SoSeparator;
	 root->addChild(bgSep);

    grid = new Grid(root, theInterface);
    
	theInterface->setView((CView*)this);
    theInterface->setCamera(viewer->getCamera());
    theInterface->setViewer(viewer);
    theInterface->setWindowScale(H_SCALE, V_SCALE);
	
	lineManip = new LineManip2;
   root->addChild(lineManip);
	lineManip->setCoordinate3(coords);

	if(showSlope)
	{
		textSep = new SoSeparator;
		SoFont* textFont = new SoFont;
		textFont->name.setValue(SbString("Courier New:Bold"));
		textFont->size.setValue(25);
		root->addChild(textFont);
		root->addChild(textSep);
	}

	//////////////
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIvDialogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	//if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	//{
	//	CAboutDlg dlgAbout;
	//	dlgAbout.DoModal();
//	}
	//else
	//{
		CDialog::OnSysCommand(nID, lParam);
	//}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CIvDialogDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		int x = rect.Width();
		int y = rect.Height();

		SoWinPlaneViewer * eViewer = new SoWinPlaneViewer;
		eViewer->setSize(SbVec2s(x - 25, y - 25));
		//SbViewportRegion region = eViewer->getViewportRegion();

		//region.setWindowSize(SbVec2s(x-25, y-25));	 // this seems to have caused a problem
		//eViewer->setViewportRegion(region); // this also seems unnecessary...

		CWnd *pWnd = GetDlgItem(IDC_VIEWER) ;
		pWnd->MoveWindow(5, 20, x-5, y-20, TRUE);

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CIvDialogDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void
CIvDialogDlg::mouseMoved( void *viewer, SoEventCallback *eventCB)
{
}

void
CIvDialogDlg::setButtonDown( void *viewer, SoEventCallback *eventCB)
{
}

void
CIvDialogDlg::setButtonUp( void *viewer, SoEventCallback *eventCB)
{
}

void
CIvDialogDlg::viewerCB(void *theView, void *)
{ 
	CIvDialogDlg* view = (CIvDialogDlg*) theView;
	view->grid->drawGrid();
}

void CIvDialogDlg::OnClear() 
{
	coords->point.deleteValues(1, -1);	
	lineManip->setCoordinate3(coords);
}

void CIvDialogDlg::OnLoad() 
{
    BOOL    bAddFileDialog = TRUE;
    LPCTSTR lpszFilter = NULL;
    LPCTSTR lpszDefExt = NULL;
    LPCTSTR lpszFileName = "*.iv";
    DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CWnd    *pParentWnd = NULL;
    
    CFileDialog addFileDialog(bAddFileDialog,
        lpszDefExt, lpszFileName, dwFlags,
        lpszFilter, pParentWnd);

    addFileDialog.m_ofn.lpstrTitle = LPCTSTR("read in a Coordinate3 file...");
    int nModal = addFileDialog.DoModal();

   if (nModal == IDOK) {
		CString filename = addFileDialog.GetFileName();
		SoInput myInput;
		myInput.openFile(filename);
		SoSeparator *newSep = new SoSeparator;
		newSep->ref();
		newSep->addChild(SoDB::readAll(&myInput));
		myInput.closeFile(); 

		SoSearchAction search;
		SoPath *path;
		SoNode *node;
    
		search.setType(SoCoordinate3::getClassTypeId(), FALSE);
		search.setInterest(SoSearchAction::FIRST);
		search.apply(newSep);
    
		path = search.getPath();
		if (path == NULL) return;
		node = path->getTail();
		SoCoordinate3 *tempCoords = (SoCoordinate3*)node;
		if (tempCoords == NULL) return;

		// copy new coords...
		int num = tempCoords->point.getNum();
		coords->point.deleteValues(0, -1);
		int i;
		for ( i = 0; i < num; i++)
		{
			coords->point.set1Value(i, tempCoords->point[i]);
		}
		if (TRUE == m_bClosed && coords->point[i] != coords->point[0])
			coords->point.set1Value( i + 1, coords->point[0]);
		else if (FALSE == m_bClosed && coords->point[i] == coords->point[0])
			coords->point.deleteValues(i, -1);
    }	
}

void CIvDialogDlg::OnSave() 
{
    BOOL    bAddFileDialog = FALSE;
    LPCTSTR lpszFilter = NULL;
    LPCTSTR lpszDefExt = NULL;
    LPCTSTR lpszFileName = "*.iv";
    DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CWnd    *pParentWnd = NULL;
    
    CFileDialog addFileDialog(bAddFileDialog,
        lpszDefExt, lpszFileName, dwFlags,
        lpszFilter, pParentWnd);

    addFileDialog.m_ofn.lpstrTitle = LPCTSTR("save a Coordinate3 file...");
    int nModal = addFileDialog.DoModal();

	if (nModal == IDOK) {
		SoWriteAction wa;
		CString m_strAddFile = addFileDialog.GetFileName();
		wa.getOutput()->openFile(m_strAddFile);
		//wa.apply(root); 
		wa.apply(coords); 
		wa.getOutput()->closeFile();
    }		 
}

void CIvDialogDlg::OnCheckClosed() 
{
	UpdateData(TRUE);
	int i = coords->point.getNum() - 1;
	if (TRUE == m_bClosed && coords->point[i] != coords->point[0])
		coords->point.set1Value( i + 1, coords->point[0]);
	else if (FALSE == m_bClosed && coords->point[i] == coords->point[0])
		coords->point.deleteValues(i, -1);
}

void CIvDialogDlg::sensorCB(void* data, SoSensor* sensor)
{

	CIvDialogDlg *dlg = (CIvDialogDlg*) data;
	if(TRUE == dlg->showSlope)
	{
		dlg->textSep->removeAllChildren();
		for (int i = 1; i < dlg->coords->point.getNum(); i++)
		{
			SoSeparator* tempSep = new SoSeparator;
			SoTransform *textTrans = new SoTransform;
			SoText2* tempText = new SoText2;
			char number[20];
			float slope = (dlg->coords->point[i][1] - dlg->coords->point[i - 1][1]) / (dlg->coords->point[i][0] - dlg->coords->point[i - 1][0]);
			sprintf(number, "%.2f%", slope * 100);
			tempText->string.setValue(SbString(number));
			SbVec3f temp = dlg->coords->point[i];
			temp += dlg->coords->point[i - 1];
			temp /= 2;
			textTrans->translation.setValue(temp);
			tempSep->addChild(textTrans);
			tempSep->addChild(tempText);
			dlg->textSep->addChild(tempSep);
		}
	}
	dlg->m_pView->PostMessage(WM_RBUTTONDBLCLK, 0, 0);
}

void CIvDialogDlg::OnDxf() 
{
	ReadDXF readDXF(NULL);
	CString ivFile = readDXF.readFile();

	SoInput myInput;
	SoSeparator *tempSep = new SoSeparator;
	tempSep->ref();
	myInput.openFile(ivFile);
	tempSep->addChild(SoDB::readAll(&myInput));
	myInput.closeFile(); 

	SoSearchAction search;
	SoPath *path;
	SoNode *node;
 
	search.setType(SoCoordinate3::getClassTypeId(), FALSE);
	search.setInterest(SoSearchAction::FIRST);
	search.apply(tempSep);
 
	path = search.getPath();
	if (path == NULL) return;
	node = path->getTail();
	SoCoordinate3 *tempCoords = (SoCoordinate3*)node;
	if (tempCoords == NULL) return;

	// copy new coords...
	int num = tempCoords->point.getNum();
	coords->point.deleteValues(0, -1);
	int i;
	for (i = 0; i < num; i++)
	{
		coords->point.set1Value(i, tempCoords->point[i]);
	}
	if (TRUE == m_bClosed && coords->point[i] != coords->point[0])
		coords->point.set1Value( i + 1, coords->point[0]);
	else if (FALSE == m_bClosed && coords->point[i] == coords->point[0])
		coords->point.deleteValues(i, -1);	
}

void CIvDialogDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(TRUE);
    float x = m_fVertExaggerate;
    float y;

    if(pNMUpDown->iDelta < 0) {
        if     (x < .0499)    y= .05;
        else if(x < .0999)     y= .1;
        else if(x < .199)      y= .2;
        else if(x < .499)       y= .5;
        else if(x < 0.99)     y= 1.0;
        else if(x < 1.49)        y= 2.0;
        else if(x < 4.99)    y= 5.0;
        else if(x < 9.99)   y= 10.0;
        else if(x < 19.99)  y= 20.0;
        else if(x < 39.99)  y= 40.0;
        else y =  100;
        m_fVertExaggerate = y;
		  V_SCALE = y;
    }
    else {
        if(x <.0499)        y= .02;
        else if(x<.0999)    y= .05;
        else if(x<.199)    y= .1;
        else if(x<.499)      y= .2;
        else if(x<.99)     y= .5;
        else if(x<1.49)    y= .5;
        else if(x<4.99)   y= 1;
        else if(x<9.99)  y= 2;
        else if(x<19.99) y= 5.0;
        else if(x<39.99)y= 10.0;
		  else if(x<100)  y= 20;
        else y = 40;
        m_fVertExaggerate = y;
		  V_SCALE = y;
    }
    UpdateData(FALSE);	
	 screenTrans->scaleFactor.setValue(H_SCALE, V_SCALE, 1);
	 
	//SbVec2f winscale = SplineManip::theInterface->getWindowScale();
	//float hscale = winscale[0];

	// getting bigger or smaller?...

	// TRACE("mouse = %f    old = %f\n", mousePosition[1], oldMousePosition[1]);
	//if(mousePosition[1]>oldMousePosition[1]) hscale *= .9;
	//else hscale *= 1.1;
	//oldMousePosition = mousePosition;
	//SplineManip::theInterface->setWindowScale(hscale, winscale[1]);
   //SplineManip::screenTrans->scaleFactor.setValue(hscale, winscale[1], 1);
	 
	 *pResult = 0;
}

void CIvDialogDlg::OnLoadBackground() 
{
    BOOL    bAddFileDialog = TRUE;
    LPCTSTR lpszFilter = NULL;
    LPCTSTR lpszDefExt = NULL;
    LPCTSTR lpszFileName = "*.iv;*.dxf";
    DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CWnd    *pParentWnd = NULL;
    
    CFileDialog addFileDialog(bAddFileDialog,
        lpszDefExt, lpszFileName, dwFlags,
        lpszFilter, pParentWnd);

    addFileDialog.m_ofn.lpstrTitle = LPCTSTR("read in an IV background file...");
    int nModal = addFileDialog.DoModal();

   if (nModal == IDOK) {
		CString filename = addFileDialog.GetFileName();
		SoInput myInput;
		myInput.openFile(filename);
		bgSep->addChild(SoDB::readAll(&myInput));
		myInput.closeFile(); 
	}
}


void CIvDialogDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}
