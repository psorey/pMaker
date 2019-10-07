// ivDialogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "ExaminerDialog.h"  
#include "ReadDXF.h"
#include <WriteDXF.h>

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

#include <PEntity.h>
#include <PEntityMgr.h>
#include <PManip.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CExaminerDialog dialog

CExaminerDialog::CExaminerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CExaminerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExaminerDialog)
	m_bClosed = FALSE;
	m_fVertExaggerate = 0.0f;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CExaminerDialog::CExaminerDialog(CString _title, SoCoordinate3 *_coords, CView* view, BOOL _showSlope)
{
	TRACE("used first constructor\n");
	coords = _coords;
	showSlope = _showSlope;
	m_fVertExaggerate = 50;
	title = _title;
	m_pView = view;
	m_bClosed = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	fNurbsCurve = NULL;
    splineCoords = NULL;
	fManip = NULL; // !!!
}

CExaminerDialog::CExaminerDialog(CString _title, SoCoordinate3 *_coords, SoCoordinate3* _splineCoords, CView* view)
{
	TRACE("used second constructor\n");
    viewer = NULL;
	coords = _coords;
	splineCoords = _splineCoords;
	showSlope = FALSE;
	m_fVertExaggerate = 1;
	title = _title;
	m_pView = view;
	m_bClosed = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    fNurbsCurve = NULL;
	fManip = NULL; // !!!
	//fManip = new PManip(SoSeparator* parentRoot, SoWinViewer* viewer);
}

CExaminerDialog::~CExaminerDialog()
{
	coords = NULL;
}

BOOL CExaminerDialog::Create()
{
	return CDialog::Create(CExaminerDialog::IDD);
}

void CExaminerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExaminerDialog)
	DDX_Check(pDX, IDC_CHECK_CLOSED, m_bClosed);
	DDX_Text(pDX, IDC_VERT_EXAG, m_fVertExaggerate);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExaminerDialog, CDialog)
	//{{AFX_MSG_MAP(CExaminerDialog)
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
	ON_BN_CLICKED(IDC_REVERSE, OnReverse)
	ON_BN_CLICKED(IDC_DXF_OUT, OnDxfOut)
	ON_BN_CLICKED(IDC_SPLINE_BUTTON, OnSplineButton)
	//}}AFX_MSG_MAP
    ON_STN_CLICKED(IDC_VIEWER, &CExaminerDialog::OnStnClickedViewer)
END_MESSAGE_MAP()

// // // // // ///////////////////////////////////////////////////////////////////
// CExaminerDialog message handlers

BOOL CExaminerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CWnd *pWnd = GetDlgItem(IDC_VIEWER) ;
    viewer = new SoWinPlaneViewer(pWnd->m_hWnd);

	CMenu* pSysMenu = GetSystemMenu(TRUE);
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
	viewer->setSceneGraph(root);
    viewer->addFinishCallback((SoWinViewerCB *)CExaminerDialog::viewerCB,this);            
	viewer->viewAll();
	H_SCALE = V_SCALE = 1;
    CFrameWnd *parentFrame = GetParentFrame();
    
    // the one instance of Interface...
    theInterface = new Interface(parentFrame);
    screenTrans = new SoTransform;
    root->addChild(screenTrans);
	bgSep = new SoSeparator;
    SoSeparator* backgroundSep = new SoSeparator;
    SoPickStyle* bgPick = new SoPickStyle;
    bgPick->style.setValue(SoPickStyle::UNPICKABLE);
	root->addChild(backgroundSep);
    backgroundSep->addChild(bgSep);
    bgSep->addChild(bgPick);
    grid = new Grid(backgroundSep, theInterface); 
    
	theInterface->setView((CView*)this);
    theInterface->setCamera(viewer->getCamera());
    theInterface->setViewer(viewer);
    theInterface->setWindowScale(H_SCALE, V_SCALE);
	
	lineManip = new LineManip2;
    root->addChild(lineManip);
	if(splineCoords != NULL && splineCoords->point.getNum() > 5) {
		TRACE("made it here\n");
		coords->point.deleteValues(0, -1);
		for(int i = 0; i < splineCoords->point.getNum(); i++)
			coords->point.set1Value(i, splineCoords->point[i]);
	}
	lineManip->setCoordinate3(coords);
    viewer->viewAll();
    if(showSlope)  {
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

void CExaminerDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
		CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExaminerDialog::OnPaint() 
{
	if (IsIconic())  {
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
	else  {
		CRect rect;
		GetClientRect(&rect);
		int x = rect.Width();
		int y = rect.Height();
		viewer->setSize(SbVec2s(x - 25, y - 25));
		CWnd *pWnd = GetDlgItem(IDC_VIEWER) ;
		pWnd->MoveWindow(5, 40, x-5, y-20, TRUE);
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CExaminerDialog::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void
CExaminerDialog::viewerCB(void *theView, void *)
{ 
	CExaminerDialog* view = (CExaminerDialog*) theView;
	view->grid->drawGrid();
}

void CExaminerDialog::OnClear() 
{
	coords->point.deleteValues(0, -1);
	coords->point.set1Value(0, SbVec3f(0, 1, 0));
	coords->point.set1Value(1, SbVec3f(1, 1, 0));
	lineManip->setCoordinate3(coords);
}

void CExaminerDialog::OnLoad() 
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
        SetCurrentDirectory(addFileDialog.GetFolderPath());
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
		for (i = 0; i < num; i++)  {
			coords->point.set1Value(i, tempCoords->point[i]);
		}
		if (TRUE == m_bClosed && coords->point[i] != coords->point[0])
			coords->point.set1Value( i + 1, coords->point[0]);
		else if (FALSE == m_bClosed && coords->point[i] == coords->point[0])
			coords->point.deleteValues(i, -1);
   }
}


void CExaminerDialog::OnSave() 
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
	 TRACE("here\n");
    addFileDialog.m_ofn.lpstrTitle = LPCTSTR("save Coordinate3 file...");
    int nModal = addFileDialog.DoModal();

	if (nModal == IDOK) {
		SoWriteAction wa;
		CString m_strAddFile = addFileDialog.GetFileName();
        SetCurrentDirectory(addFileDialog.GetFolderPath());
		wa.getOutput()->openFile(m_strAddFile);
		//wa.apply(root);
        SoLineSet * newLineSet = new SoLineSet;
        newLineSet->ref();
        if(PManip::sSelectedEntity == NULL){
			wa.apply(coords); 
            wa.apply(newLineSet);
        }
		else if (PManip::sSelectedEntity->GetType() == PEntity::kNurbsCurve)  // save the nurbs control points, not the polyline points
		{
			TRACE("CExaminerDialog::OnSave()\n");
			PNurbsCurve * nurbsCurve = (PNurbsCurve*)PManip::sSelectedEntity;
			wa.apply(nurbsCurve->fCoords);
			/*
				int num = nurbsCurve->fCoords->point.getNum();
				for(int i=0; i < num; i++)
				{
					coords->point.set1Value(1, nurbsCurve->fCoords->point[i]);
				}
			*/
		}
        else {
            wa.apply(coords);
            wa.apply(newLineSet);
        }
        newLineSet->unref();
		wa.getOutput()->closeFile();	
    }
    
}

void CExaminerDialog::OnCheckClosed() 
{
	UpdateData(TRUE);
	
	int i = coords->point.getNum() - 1;
	/* if (TRUE == m_bClosed && coords->point[i] != coords->point[0])
		coords->point.set1Value( i + 1, coords->point[0]);
	else if (FALSE == m_bClosed && coords->point[i] == coords->point[0])
		coords->point.deleteValues(i, -1);
		*/
	coords->point.set1Value(i + 1, coords->point[0]);
}

void CExaminerDialog::sensorCB(void* data, SoSensor* sensor)
{
	CExaminerDialog *dlg = (CExaminerDialog*) data;
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
	if(1) {
		TRACE("made it above post message rbutton...\n");
		// save the nurbs curve? 
        // dlg->fNurbsCurve->GetPolylineCoords(); // 8/4/2016   !!!

	}
	dlg->m_pView->PostMessage(WM_RBUTTONDBLCLK, 0, 0);


}

void CExaminerDialog::OnDxf() 
{
	/*
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
		*/
}

void CExaminerDialog::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{   // this now controls num points of spline rendering (?), not vert exagg.
    // actually, I don't think that is implemented here...but in the main menu?
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(TRUE);
    float x = m_fVertExaggerate;
    float y;

    if(pNMUpDown->iDelta < 0) {
        m_fVertExaggerate += 10;
    }
    else {
		 m_fVertExaggerate -= 10;
    }
    UpdateData(FALSE);	
	 *pResult = 0;
}

void CExaminerDialog::OnLoadBackground() 
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
        SetCurrentDirectory(addFileDialog.GetFolderPath());
		SoInput myInput;
		myInput.openFile(filename);
		bgSep->addChild(SoDB::readAll(&myInput));
		myInput.closeFile(); 
	}
}


void CExaminerDialog::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CExaminerDialog::OnReverse() // reverse coordinate order
{
	SoCoordinate3* tempCoords = new SoCoordinate3;
	tempCoords->ref();
	int numValues = coords->point.getNum();
	for (int i = 0; i < numValues; i++)
	{
		tempCoords->point.set1Value(i, coords->point[numValues - (i + 1)]);
	}
	coords->copyFieldValues(tempCoords);
	tempCoords->unref();
}

void CExaminerDialog::OnDxfOut() 
{
	BOOL    bAddFileDialog = FALSE;
    LPCTSTR lpszFilter = NULL;
    LPCTSTR lpszDefExt = "dxf";
    LPCTSTR lpszFileName = "*.dxf";
    DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    CWnd    *pParentWnd = NULL;
    
    CFileDialog addFileDialog(bAddFileDialog,
        lpszDefExt, lpszFileName, dwFlags,
        lpszFilter, pParentWnd);
    addFileDialog.m_ofn.lpstrTitle = LPCTSTR("save DXF file...");
    int nModal = addFileDialog.DoModal();
	if (nModal != IDOK) return;
    CString filename = addFileDialog.GetFileName();
    SetCurrentDirectory(addFileDialog.GetFolderPath());
    FILE* fp = fopen(filename, "w");
	if (fp == NULL) return;
	WriteDXF * writeDXF = new WriteDXF(fp); // opens the file and writes the begin section
    writeDXF->WriteBeginDXF();
	int numVertices = coords->point.getNum();
	char layer[2] = "0";
	// _itoa( j, layer, 10);
    //writeDXF->WriteLWPOLYLINEHeader(layer, numVertices); 
    writeDXF->Write3dPOLYLINEHeader(layer, numVertices); 
	for (int i = 0; i < numVertices; i++) {		
		SbVec3f point = coords->point[i];
        writeDXF->Write3dPOLYLINEPoint(point);
	}
	writeDXF->WriteSEQUEND();
    writeDXF->WriteDXFEndsec();
    writeDXF->WriteEndDXF();
	fclose(fp);

}

void CExaminerDialog::OnSplineButton() 
{
	PNurbsCurve* newNurbsCurve = new PNurbsCurve;
	TRACE("goes past OnSplineButton()\n");
	PEntity* newEntity = fManip->sEntityMgr->AddEntity(newNurbsCurve, PEntity::kNurbsCurve);
	newNurbsCurve->fShowPolyline = TRUE;
	newNurbsCurve->fNumPolylineDivisions = 55;

	int num = coords->point.getNum();
	for (int i = 0; i < num; i++)
	{
		newNurbsCurve->AddPoint(coords->point[i]);
	}
	TRACE("goes past OnSplineButton()\n");
    fNurbsCurve = newNurbsCurve;
	newNurbsCurve->UnHighlight();	
	PManip::sShowNurbsPolylines = TRUE; /// !!!!
	newNurbsCurve->SetConnectedCoords(coords, splineCoords);
	newNurbsCurve->Update();
	TRACE("goes past newNurbsCurve->Update()\n");
	fManip->SetEditType(PManip::kMovePoint);
	PEntity::sPolylineDivisions = 55;   /// !!!! need to input from dialog    uncommented this 2012
	PEntity::sPolylineGenerationDistance = .05; // uncommented this 2012
	lineManip->ref();   // so we can re-attach it later...
	root->removeChild(lineManip);	
	root->addChild(fNurbsCurve->GetNode()); // !!! just added this
	//coords = fNurbsCurve->GetCoords();
	m_pView->PostMessage(WM_RBUTTONDBLCLK, 0, 0);
}

void CExaminerDialog::OnStnClickedViewer()
{
    // TODO: Add your control notification handler code here
}
