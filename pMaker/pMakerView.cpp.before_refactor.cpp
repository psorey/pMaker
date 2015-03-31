// pMakerView.cpp : implementation of the CpMakerView class
//

#include "stdafx.h"
#include "pMaker.h"
#include "pMakerDoc.h"
#include "pMakerView.h"
#include "Extruder.h"
#include "Flattener.h"
#include "FractalTree.h"

//#include <ruby.h>

#include "direct.h"
#include "pMaker.h"

#include "GetFloatDialog.h"
#include "GetScalesDialog.h"
#include "GetStringDialog.h"
#include "MarkerDialog.h"
#include "ExaminerDialog.h"
#include "ReadDXF.h"
#include "HoleDialog.h"
#include "SegmentDlg.h"
#include "GetIntegerDialog.h"
#include "OneIntegerDlg.h"
#include "FractalTree.h"
#include "WhichBranchDialog.h"
#include "FractalTreeDialog.h"
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/manips/SoTransformBoxManip.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/details/SoDetail.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/SoPath.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CpMakerView, CView)

BEGIN_MESSAGE_MAP(CpMakerView, CView)
	//{{AFX_MSG_MAP(CpMakerView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_VIEW_SELECTIONMODE, OnViewViewmodesSelectionmode)
	ON_COMMAND(ID_VIEW_VIEWINGMODE, OnViewViewmodesViewingmode)
	ON_COMMAND(ID_X_SEC_BUTTON, OnXSecButton)
	ON_COMMAND(ID_TWIST_BUTTON, OnTwistButton)
	ON_WM_RBUTTONDBLCLK()
	ON_COMMAND(ID_ADD_BACKGROUND, OnAddBackground)
	ON_COMMAND(ID_DELETE_BACKGROUND, OnDeleteBackground)
	ON_COMMAND(ID_FILE_DXFTOIV, OnFileDxftoiv)
	ON_COMMAND(ID_HSCALE_BUTTON, OnHscaleButton)
	ON_COMMAND(ID_VSCALE_BUTTON, OnVscaleButton)
	ON_COMMAND(ID_SAVELOFT, OnSaveLoft)
	ON_COMMAND(ID_REMOVELOFT, OnRemoveLoft)
	ON_WM_CHAR()
	ON_COMMAND(ID_THREE_D_BUTTON, OnThreeDButton)
	ON_COMMAND(ID_SAVE_CONSTRUCTION_DATA, OnSaveConstructionData)
	ON_COMMAND(ID_LOAD_CONSTRUCTION_DATA, OnLoadConstructionData)
	ON_COMMAND(ID_FLATTEN, OnFlatten)
	ON_COMMAND(ID_FLATTEN_POLYLINE, OnFlattenPolyline)
	ON_COMMAND(ID_EDIT_BEGIN_END, OnEditBeginEnd)
	ON_COMMAND(ID_EDIT_MULTIPLES, OnEditMultiples)
	ON_COMMAND(ID_EDIT_TRANSFORM_BOX_ON, OnEditTransformBoxOn)
	ON_COMMAND(ID_EDIT_TRANSFORM_BOX_OFF, OnEditTransformBoxOff)
	ON_COMMAND(ID_FILE_SAVE_MULTIIPLES, OnFileSaveMultiiples)
	ON_COMMAND(ID_EDIT_OFFSET, OnEditOffset)
	ON_COMMAND(ID_EDIT_ADD_HOLES, OnEditAddHoles)
	ON_COMMAND(ID_EDIT_CLIPPLANE, OnEditClipPlane)
	ON_COMMAND(ID_EDIT_CLIPPLANEOFF, OnEditClipPlaneOff)
	ON_COMMAND(ID_ANTISQUISH, OnAntisquish)
	ON_COMMAND(ID_FILE_FLATTENIMPORTEDCOORDS, OnFlattenImportedCoords)
	ON_COMMAND(ID_FLATTEN_TWO, OnFlattenTwoLines)
	ON_COMMAND(ID_VERT_EQ_HORIZ, OnVertEqHoriz)
	ON_COMMAND(ID_ADD_BACKGND_SOLID, OnAddBackgndSolid)
	ON_COMMAND(ID_SEARCH_BACKGROUND, OnSearchBackground)
	ON_COMMAND(ID_USE_SEGMENT_DIALOG, OnUseSegmentDialog)
	ON_COMMAND(ID_REMOVE_TRANSFORM, OnRemoveTransform)
	ON_COMMAND(ID_USE_MARKERS, OnUseMarkers)
	ON_COMMAND(ID_EDIT_ORIENT_TO_REFERENCE_LINE, OnOrientToReferenceLine)
	ON_COMMAND(ID_TREES_TEST_TREE, OnTreesTestTree)
	ON_COMMAND(ID_TREES_SAVEEXTRUSIONANDCENTERLINE, OnTreesSaveextrusionandcenterline)
    ON_COMMAND(ID_TREES_SAVETREEPARAMETERS, &CpMakerView::OnTreesSavetreeparameters)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

    ON_COMMAND(ID_FILE_CHANGE_LOFT_SCALE, &CpMakerView::OnChangeLoftScale)
    ON_COMMAND(ID_FLATTEN_ALL_LEVELS, &CpMakerView::OnFlattenAllLevels)
END_MESSAGE_MAP()


// #define GENERATE_CENTERLINES              // generates a centerline (output: centerlines_e.iv) for each extruded side
// #define ORIENT_TO_REFERENCE_CENTERLINE    //
// #define USE_SECOND_SCALE







/////////////////////////////////////////////////////////////////////////////


CpMakerView::CpMakerView()
{
	viewer = NULL;
    TRACE("made it here -- constructor\n");
    //SoDB::init();

// for remembering values between trees
    fNumLevels = 7;
    fRotationAngle = 60.0;
    fScaleFactor = .74;
    fInfluenceIntensity = 0.0;
    fInfluenceDirection = 180.0; // y direction, 
    fCenterlineScale    = 0.0;
    fLeftSideFile = "";
    fRightSideFile = "";

}

CpMakerView::~CpMakerView()
{
    if (viewer != NULL)
        delete viewer;
    if (fExtruder != NULL)
        delete fExtruder;
	threeDCoords->unref();
	sectionCoords->unref();
    hScaleCoords->unref();    
	vScaleCoords->unref();
	twistCoords->unref();
    if (root != NULL)
        root->unref();
    fTransformBox->unref();
}

BOOL CpMakerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}

// CpMakerView printing

BOOL CpMakerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CpMakerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CpMakerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}
#ifdef _DEBUG
void CpMakerView::AssertValid() const
{
	CView::AssertValid();
}
void CpMakerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CpMakerDoc* CpMakerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CpMakerDoc)));
	return (CpMakerDoc*)m_pDocument;
}
#endif //_DEBUG

void CpMakerView::OnDraw(CDC* /*pDC*/)
{    TRACE("made it here -- on_draw\n");
	CpMakerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    if (viewer == NULL)
    {
        TRACE("made it here too\n");
        viewer = new SoWinExaminerViewer( m_hWnd );
        //viewer->setDecoration(TRUE);
        //SoSeparator *root = GetDocument()->root;
        //viewer->setSceneGraph((SoNode*)root);
        //viewer->setBackgroundColor(SbColor(.7, .8, .9));
        viewer->setBackgroundColor(SbColor(.1, .1, .1));
        
        WINDOWPLACEMENT p;
        memset(&p, 0, sizeof(p));
        p.length = sizeof(WINDOWPLACEMENT);
        p.showCmd = SW_SHOWMAXIMIZED;
        SetWindowPlacement(&p);
    }
}

// CpMakerView message handlers
//////////////////////////////////////////////////////////////////////////////////////////

void CpMakerView::OnInitialUpdate()
{
TRACE("made it here -- on_initial_update\n");
	CpMakerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
    if (viewer == NULL)
    {
        TRACE("made it here too\n");
        viewer = new SoWinExaminerViewer( m_hWnd );;
        viewer->setBackgroundColor(SbColor(.7, .8, .9));
        viewer->setBackgroundColor(SbColor(.1, .1, .1));
        WINDOWPLACEMENT p;
        memset(&p, 0, sizeof(p));
        p.length = sizeof(WINDOWPLACEMENT);
        p.showCmd = SW_SHOWMAXIMIZED;
        SetWindowPlacement(&p);
        fExtruder = new Extruder; // here???
    }
	// clip plane nodes...(for cutting sections of loft object)
	fClipPlaneSep = NULL;
	fClipPlaneCoords = NULL;
	fClipPlaneFaces = NULL;
	//SoTransform*	fClipPlaneTransform;
	fClipPlaneManip = NULL;
	fAntiSquish = TRUE;
    fMarkerDialog = NULL;
	fClipIntersectionCoords = NULL;
	fClipIntersectionLines = NULL;
	fOffset = 0.0;
	fNumRotatedCopies = 1;
	threeDDlg = NULL;
	vertDlg = NULL;
	horizDlg = NULL;
	vScaleDlg = NULL;
	hScaleDlg = NULL;
	twistDlg = NULL;
	sectionDlg = NULL;
#ifdef GENERATE_CENTERLINES
    midlineCoords = new SoCoordinate3;
    midlineCoords->ref();
    midlineLineSet = new SoLineSet;
    midlineLineSet->ref();
#endif



    threeDCoords = new SoCoordinate3;                       // start out with three collinear points
	threeDCoords->ref();
    threeDCoords->point.setValue(SbVec3f(0,0,0));
    threeDCoords->point.set1Value(1, SbVec3f(1,0,0));
    threeDCoords->point.set1Value(2, SbVec3f(2,0,0));
	threeDCoords->setName("threeDCoords");

	sectionCoords = new SoCoordinate3;
	sectionCoords->ref();
	sectionCoords->setName("sectionCoords");
    sectionCoords->point.setValue(SbVec3f(-1, 1, 0));
    sectionCoords->point.set1Value(1, SbVec3f(1, 1, 0));
    sectionCoords->point.set1Value(2, SbVec3f(1, -1, 0));
    sectionCoords->point.set1Value(3, SbVec3f(-1, -1, 0));
    sectionCoords->point.set1Value(4, SbVec3f(-1, 1, 0));    // section is a closed square defined by five points


	hScaleCoords = new SoCoordinate3;
	hScaleCoords->ref();
	hScaleCoords->setName("hScaleCoords");
	vScaleCoords = new SoCoordinate3;
	vScaleCoords->ref();
	vScaleCoords->setName("vScaleCoords");
	twistCoords = new SoCoordinate3;
	twistCoords->ref();
	twistCoords->setName("twistCoords");

	hScaleCoords->point.setValue(SbVec3f(0,1,0));
    hScaleCoords->point.set1Value(1,SbVec3f(1,1,0));
	vScaleCoords->point.setValue(SbVec3f(0,1,0));
    vScaleCoords->point.set1Value(1,SbVec3f(1,1,0));
	twistCoords->point.setValue(SbVec3f(0,1,0));
	twistCoords->point.set1Value(1, SbVec3f(1,1,0));

	threeDSplineCoords = NULL;
	vertSplineCoords = NULL;
	horizSplineCoords = NULL;
	vScaleSplineCoords = NULL;
	hScaleSplineCoords = NULL;
	twistSplineCoords = NULL;
	sectionSplineCoords = NULL;

	root = new SoSeparator;
	root->ref();
	loftRoot = new SoSeparator;
	root->addChild(loftRoot);
    treeRoot = new SoSeparator;
    root->addChild(treeRoot);

	fMultipleSep = new SoSeparator;
	root->addChild(fMultipleSep);

	//loftPathCoords = new SoCoordinate3;
	//loftPathCoords = new SoCoordinate3;
	//loftScaleCoords = new SoCoordinate3;
   // loftScale2Coords = new SoCoordinate3;
   // loftScale2Center = new SoCoordinate3;
   // loftScale2Center->point.setValue(SbVec3f(0,0,0));

	SoNormalBinding* nBinding = new SoNormalBinding;
	nBinding->value.setValue(SoNormalBinding::PER_FACE);
	root->addChild(nBinding);
	
    //SoShapeHints* newHints = new SoShapeHints;
	//newHints->shapeType.setValue(SoShapeHints::UNKNOWN_SHAPE_TYPE);
	//newHints->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);
	//loftRoot->addChild(newHints);

	fTransformBox = new SoTransformBoxManip;
	fTransformBox->ref();

	//loftRoot->addChild(loftCoords);
	//loftRoot->addChild(loftFaces);
	//loftCoords->point.deleteValues(0);
	//loftFaces->coordIndex.deleteValues(0);
    backgroundSep = new SoSeparator;   // make background the last node on root
	root->addChild(backgroundSep);

    fMarkerTopSeparator = new SoSeparator;
    root->addChild(fMarkerTopSeparator);
	viewer->setSceneGraph(root);

	fUpperLowerDialog = NULL;
	fIsEqualScale = TRUE;

   SoEventCallback *myEventCallback = new SoEventCallback;
   root->addChild(myEventCallback);

    myEventCallback->addEventCallback(
        SoLocation2Event::getClassTypeId(),
        mouseMoved, this);

    /*myEventCallback->addEventCallback(
       SoLocation2Event::getClassTypeId(),
       mouseClick, this); */
    
    fScreenText      = new SoText2;
    fScreenFont      = new SoFont;
    fScreenTextSep   = new SoSeparator;
    fScreenTransform = new SoTransform;

    root->addChild(fScreenTextSep);
    fScreenTextSep->addChild(fScreenTransform);
    fScreenTextSep->addChild(fScreenFont);
    fScreenTextSep->addChild(fScreenText);
    fScreenFont->name.setValue(SbString("Times New Roman:Bold"));
    fScreenFont->size.setValue(24);	

    fSearchBackground = FALSE;
    fSegmentDlg = NULL;
	CView::OnInitialUpdate();
}

void CpMakerView::OnDestroy()
{
	CView::OnDestroy();
}

int CpMakerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;	
	return 0;
}

BOOL CpMakerView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CpMakerView::OnEditCut() 
{
}

void CpMakerView::OnUpdateEditCut(CCmdUI* pCmdUI)
{
}

void CpMakerView::OnEditCopy() 
{
}

void CpMakerView::OnUpdateEditCopy(CCmdUI* pCmdUI)
{
}

void CpMakerView::OnEditPaste() 
{	
}

void CpMakerView::OnUpdateEditPaste(CCmdUI* pCmdUI)
{
}

void CpMakerView::OnViewViewmodesSelectionmode() 
{
}

void CpMakerView::OnViewViewmodesViewingmode() 
{
}

void CpMakerView::OnXSecButton() 
{
    sectionDlg = new CExaminerDialog(CString("Cross-Section"), sectionCoords, sectionSplineCoords, this);
    sectionDlg->Create();
}

void CpMakerView::OnTwistButton() 
{
	// twistDlg = new CExaminerDialog(CString("Twist Profile"), twistCoords, this, FALSE);
	twistDlg = new CExaminerDialog(CString("Twist Profile"), twistCoords, twistSplineCoords, this);
    twistDlg->Create();
}

void CpMakerView::OnHscaleButton() 
{
	// hScaleDlg = new CExaminerDialog(CString("Horizontal Scale Profile"), hScaleCoords, this, FALSE);
	hScaleDlg = new CExaminerDialog(CString("Horizontal Scale Profile"), hScaleCoords, hScaleSplineCoords, this);
	hScaleDlg->Create();
}

void CpMakerView::OnVscaleButton() 
{
	// vScaleDlg = new CExaminerDialog(CString("Vertical Scale Profile"), vScaleCoords, this, FALSE);
	vScaleDlg = new CExaminerDialog(CString("Vertical Scale Profile"), vScaleCoords, vScaleSplineCoords, this);
	vScaleDlg->Create();
}

void CpMakerView::OnThreeDButton() 
{
	// threeDDlg = new CExaminerDialog(CString("3d Alignment"), threeDCoords, this, FALSE);
	threeDDlg = new CExaminerDialog(CString("3d Alignment"), threeDCoords, threeDSplineCoords, this);
	threeDDlg->Create();
}

void CpMakerView::OnAddBackground() 
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

  addFileDialog.m_ofn.lpstrTitle = LPCTSTR("load a background file...");
  int nModal = addFileDialog.DoModal();
  CString m_strAddFile;

    if (nModal == IDOK) 
    {
        m_strAddFile = addFileDialog.GetFileName();
        SetCurrentDirectory(addFileDialog.GetFolderPath());

	    SoInput myInput;
	    myInput.openFile(m_strAddFile);

       SoDrawStyle* style = new SoDrawStyle;
       SoMaterial*  material = new SoMaterial;
       material->setOverride(TRUE);
       style->setOverride(TRUE);
       backgroundSep->addChild(style);
       style->style.setValue(SoDrawStyle::LINES);
       material->diffuseColor.setValue(SbColor(.99,.99,.99));
       backgroundSep->addChild(material);
       backgroundSep->addChild(SoDB::readAll(&myInput));
       myInput.closeFile(); 
    }	
}

void CpMakerView::OnAddBackgndSolid() 
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

  addFileDialog.m_ofn.lpstrTitle = LPCTSTR("load a background file...");
  int nModal = addFileDialog.DoModal();
  CString m_strAddFile;

    if (nModal == IDOK) {
        m_strAddFile = addFileDialog.GetFileName();
        SetCurrentDirectory(addFileDialog.GetFolderPath());
	    SoInput myInput;
	    myInput.openFile(m_strAddFile);

       SoDrawStyle* style = new SoDrawStyle;
       SoMaterial*  material = new SoMaterial;
       material->setOverride(TRUE);
       style->setOverride(TRUE);
       backgroundSep->addChild(style);
       //style->style.setValue(SoDrawStyle::LINES);
       material->diffuseColor.setValue(SbColor(.33,.55,.77));
       backgroundSep->addChild(material);
       backgroundSep->addChild(SoDB::readAll(&myInput));
       myInput.closeFile(); 
    }	
}

void CpMakerView::OnDeleteBackground() 
{
	backgroundSep->removeAllChildren();	
}

void CpMakerView::OnFileDxftoiv() 
{
	ReadDXF readDXF(NULL);
	readDXF.readFile();
}

void CpMakerView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	this->OnShowDialog();
	CView::OnRButtonDblClk(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///  Doing the Extrusion   ///
//////////////////////////////



bool CpMakerView::isUsingVScale(void) 
{
    if(vScaleCoords->point[0] == SbVec3f(1.0,0.0,0.0) && vScaleCoords->point[1] == SbVec3f(1.0,1.0,0.0))
        return false;
    
    return true;
}



void CpMakerView::OnShowDialog() // call this to regenerate the extrusion
{
	//this->make3dPath();
	//this->makeLoftObject();
    
    this->loftRoot->removeAllChildren();
    SoSeparator * branch_sep;
    if((vScaleCoords->point[1] - vScaleCoords->point[0]).length() - 1.0 < 0.001) {
        branch_sep = fExtruder->extrude(this->sectionCoords,this->threeDCoords, this->hScaleCoords, this->hScaleCoords, this->twistCoords, 1.0, 0.0, false );
    } else {
        branch_sep = fExtruder->extrude(this->sectionCoords,this->threeDCoords, this->hScaleCoords, this->vScaleCoords, this->twistCoords, 1.0, 0.0, false );
    }
    //SoSeparator * branch_sep = fExtruder->extrude(this->sectionCoords,this->threeDCoords, this->hScaleCoords, this->vScaleCoords, 1.0, 0.0, false );
    loftRoot->addChild(branch_sep);
    TRACE("OnShowDialog()\n");   
}
//#ifdef USE_LOCAL_EXTRUDER
void CpMakerView::make3dPath() 
{
	if (NULL == threeDCoords)   /// !!! OCT 2004
		return;

	if (threeDCoords->point.getNum() <= 1)
	{
		BOOL    bAddFileDialog = TRUE;
		LPCTSTR lpszFilter = NULL;
		LPCTSTR lpszDefExt = "iv";
		LPCTSTR lpszFileName = "*.iv";
		DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
		CWnd    *pParentWnd = NULL;

		CFileDialog addFileDialog(bAddFileDialog,
			lpszDefExt, lpszFileName, dwFlags,
			lpszFilter, pParentWnd);

		addFileDialog.m_ofn.lpstrTitle = LPCTSTR("Load Loft PATH...");
		int nModal = addFileDialog.DoModal();
		CString m_strAddFile;

		if (nModal != IDOK) return;
   		
		m_strAddFile = addFileDialog.GetFileName();
        SetCurrentDirectory(addFileDialog.GetFolderPath());
		SoInput myInput;
		myInput.openFile(m_strAddFile);

		// add the file to existing nodes...
		SoSeparator *tempSep = new SoSeparator;
		tempSep->ref();
		tempSep->addChild(SoDB::readAll(&myInput));
		myInput.closeFile();
		SoCoordinate3* pathCoords = (SoCoordinate3 *)CpMakerView::findChildOfType(
			tempSep, SoCoordinate3::getClassTypeId());
		threeDCoords->copyFieldValues(pathCoords);
      tempSep->unref();
	}
	loftPathCoords->copyFieldValues(threeDCoords);
	for (int k = 0; k < loftPathCoords->point.getNum(); k++)
	{
		SbVec3f point = loftPathCoords->point[k];
		TRACE("%f  %f  %f\n", point[0], point[1], point[2]);
	}
	int numThreeDCoords = threeDCoords->point.getNum();
	loftScaleCoords->point.deleteValues(0, -1);
   loftScale2Coords->point.deleteValues(0,-1);

////////////////////////////////////////////
#ifdef  USE_SECOND_SCALE

   loftScale2Coords->point.setValue(vScaleCoords->point[0][1],
		     vScaleCoords->point[0][1], 1.0);

#endif  // USE_SECOND_SCALE
/////////////////////////////////////////////
   if (fIsEqualScale)     // !!! OCt 2004
		loftScaleCoords->point.setValue(hScaleCoords->point[0][1],
		     hScaleCoords->point[0][1], 1.0); 
   else
      loftScaleCoords->point.setValue(hScaleCoords->point[0][1],
		     vScaleCoords->point[0][1], 1.0);

	// get total length of horiz line...
	float totalLength = 0;
	for (int i = 1; i < numThreeDCoords; i++)
	{
		totalLength +=	dist(threeDCoords->point[i], threeDCoords->point[i - 1]);
	}
	float currentLength = 0;
	for (int i = 1; i < numThreeDCoords; i++)
	{
		currentLength += dist(threeDCoords->point[i], threeDCoords->point[i - 1]);
		//float z = interpolateZ(currentLength);
		SbVec3f sc = interpolateScale(currentLength / totalLength);
		loftScaleCoords->point.set1Value(i, sc);
////////////////////////////////////////////
#ifdef  USE_SECOND_SCALE
      sc = interpolateScale2(currentLength / totalLength);
      loftScale2Coords->point.set1Value(i, sc);
#endif  // USE_SECOND_SCALE
//////////////////////////////////////////////
	}
	return;
}


void CpMakerView::makeLoftObject() 
{
    //loftCoords->point.deleteValues(0);
   // loftFaces->coordIndex.deleteValues(0);

    int numShapeVertices = sectionCoords->point.getNum();
    int numPathVertices  = loftPathCoords->point.getNum();

    SbRotation rot;
    int loftCoordsCount = 0;

#ifdef GENERATE_CENTERLINES
    int midlineCoordsCount = 0;
#endif

    // generate coordinate points...
    for (int i = 0; i < numPathVertices; i++)
    {
        // apply scale, rotation and translation to shape vertices...
        SbMatrix mat;
        mat.makeIdentity();

        // rescale the section to give true corners...
        if (/*fAntiSquish && */ i != 0 && i != numPathVertices - 1 )
        {
            float deflection1 = atan2 (( loftPathCoords->point[i][1] - loftPathCoords->point[i-1][1]) , 
                (  loftPathCoords->point[i][0] - loftPathCoords->point[i-1][0] ));
            float deflection2 = atan2 (( loftPathCoords->point[i+1][1] - loftPathCoords->point[i][1]) , 
                (  loftPathCoords->point[i+1][0] - loftPathCoords->point[i][0] ));
            deflection1 -= deflection2;
            deflection1 /= 2;
            SbMatrix mat00;
            float scaleFactor = fabs(1/cos(deflection1));
            mat00.setScale(SbVec3f(scaleFactor, 1, 1));
            mat *= mat00;
        }
        int count = 0;
        SbMatrix mat0;
        mat0.setScale(loftScaleCoords->point[i]);
        mat *= mat0;

        // do the scale thing first, so offsetting is simpler...
        SoMFVec3f scaledCoords;
        scaledCoords.deleteValues(0, -1);
        int scaledCoordsCount = 0;
        for (int j = 0; j < numShapeVertices; j++) {
            SbVec3f result;
            mat.multVecMatrix(sectionCoords->point[j], result);
            scaledCoords.set1Value(scaledCoordsCount++, result);			
        }
////////////////////////
#ifdef USE_SECOND_SCALE		
        SbMatrix scale2mat;
        scale2mat.identity();
        SbRotation rotNone;
        //         setTransform(
        //         const SbVec3f         &translation,
        //         const SbRotation      &rotation,
        //         const SbVec3f         &scaleFactor,
        //         const SbRotation      &scaleOrientation,
        //         const SbVec3f         &center);     

        SbVec3f center = SbVec3f(-7.5, 12.65,0);
        SbVec3f scale2 = loftScale2Coords->point[i];
        SbVec3f newCenter;
        mat.multVecMatrix(center, newCenter);
        SbVec3f translation = SbVec3f(0,0,0);
        scale2mat.setTransform(translation,rotNone,scale2,rotNone, newCenter);
        for (j = 0; j < numShapeVertices; j++) {
            SbVec3f result;
            scale2mat.multVecMatrix(scaledCoords[j], result);
            scaledCoords.set1Value(j, result);			
        }
#endif USE_SECOND_SCALE			
///////////////////////

    fClosedShape = FALSE;
    if ((fabs(scaledCoords[0][0] - scaledCoords[numShapeVertices - 1][0]) < .01) &&
        (fabs(scaledCoords[0][1] - scaledCoords[numShapeVertices - 1][1]) < .01)  )
            {  fClosedShape = TRUE; }

    // do an offset if desired...
    // float fOffset = 1;
    SoMFVec3f offsetCoords;
    offsetCoords.deleteValues(0, -1);
    if (fOffset == 0) {
        for( int f = 0; f < scaledCoordsCount; f++)
            offsetCoords.set1Value(f, scaledCoords[f]);	
    } else {
        SoMFVec3f tempOffsetCoords;
        tempOffsetCoords.deleteValues(0, -1);
        int tempOffsetCoordsCount = 0;
        // generate 2 offset endpoints for each segment...
        for (int j = 0; j < numShapeVertices; j++)
        {
            float theta = GetTheta(scaledCoords[j], scaledCoords[j+1]) + 1.570796;
            SbVec3f resultCoords;
            resultCoords = CalcVectorPoint(scaledCoords[j], fOffset, theta);
            tempOffsetCoords.set1Value(tempOffsetCoordsCount++, resultCoords);
            resultCoords = CalcVectorPoint(scaledCoords[j + 1], fOffset, theta);
            tempOffsetCoords.set1Value(tempOffsetCoordsCount++, resultCoords);
        }
        TRACE("tempOffsetCoordsCount = %d   vertices = %d\n", tempOffsetCoordsCount, numShapeVertices);

        // intersect the segments for the offset shape...
        int offsetCoordsCount = 0;
        offsetCoords.deleteValues(0, -1);

        SbVec3f intersectionPt;

        // set the first coord assuming figure is not closed...
        offsetCoords.set1Value(offsetCoordsCount++, tempOffsetCoords[0]);
        for (int j = 0; j < numShapeVertices - 1; j++)
        {
            // get intersections for the rest...
            int k = j * 2;
            intersectionPt = GetIntersection(tempOffsetCoords[k], tempOffsetCoords[k + 1], 
	        tempOffsetCoords[k + 2], tempOffsetCoords[k + 3]);
            offsetCoords.set1Value(offsetCoordsCount++, intersectionPt);
        }
        // set the last coord assuming figure is not closed...
        offsetCoords.set1Value(offsetCoordsCount++, tempOffsetCoords[tempOffsetCoordsCount - 1]);
        if (fClosedShape) {
            intersectionPt = GetIntersection(tempOffsetCoords[0], tempOffsetCoords[1], 
	        tempOffsetCoords[tempOffsetCoordsCount - 2], tempOffsetCoords[tempOffsetCoordsCount - 1]);
            offsetCoords.set1Value(0, intersectionPt);
            offsetCoords.set1Value(offsetCoordsCount - 2, intersectionPt);
        }	
    }
    // finished scaling and offsetting...so continue matrix operations...
    SbMatrix mat1;
    SbRotation rot3(SbVec3f(1,0,0), 1.5708);
    mat1.setRotate(rot3);

    SbMatrix mat2;
    SbVec3f translate = loftPathCoords->point[i];
    mat2.setTranslate(translate);

    // get current rotation...

    ///////////////////////////////////////////////////////////////////
    #ifdef ORIENT_TO_REFERENCE_CENTERLINE
    rot = this->interpolatePRY2_Align_with_Reference_Centerline(i);
    #else        
    if (0 == i)
    rot = this->getPRY(loftPathCoords->point[i], loftPathCoords->point[i+1]);
    else if (i == numPathVertices - 1)
    rot = this->getPRY(loftPathCoords->point[i-1], loftPathCoords->point[i]);
    else
    rot = this->interpolatePRY2(
    loftPathCoords->point[i - 1], 
    loftPathCoords->point[i],
    loftPathCoords->point[i + 1]);
    #endif // ORIENT_TO_REFERENCE_CENTERLINE
    ///////////////////////////////////////////////////////////////////////////////

    SbMatrix mat3;
    mat3.setRotate(rot);

    //mat  /= mat0; // already did this...
    SbMatrix finalMat;
    finalMat.makeIdentity();
    finalMat *= mat1;
    finalMat *= mat3;
    finalMat *= mat2;

    ///////////////////////////////////////
    #ifdef GENERATE_CENTERLINES

    SoCoordinate3* tempCoords = new SoCoordinate3;
    //tempCoords->ref();
    tempCoords->point.deleteValues(0,-1);
    //tempCoords->point.setNum(2000);

    #endif
    ///////////////////////////////////////
    // generate the lofted shape...
    for (int j = 0; j < numShapeVertices; j++)
    {
    SbVec3f result;
    finalMat.multVecMatrix(offsetCoords[j], result);
    loftCoords->point.set1Value(loftCoordsCount++, result);

    //////////////////////////////////////////////
    #ifdef GENERATE_CENTERLINES

    // generate the lofted centerlines...
    tempCoords->point.set1Value(j, result);
    if (j > 0)
    {
    SbVec3f point1 = result;
    SbVec3f point2 = tempCoords->point[j-1];
    SbVec3f midpoint = SbVec3f( 
    point1[0] + ((point2[0] - point1[0]) * .5),
    point1[1] + ((point2[1] - point1[1]) * .5),
    point1[2] + ((point2[2] - point1[2]) * .5));
    midlineCoords->point.set1Value(midlineCoordsCount++, midpoint);
    TRACE("midlineCoordsCount = %d\n", midlineCoordsCount);

    }
    //tempCoords->unref();
    #endif 
    ////////////////////////////////////////////
    }
    }

    // now generate Indexed Triangle Strip coordinate indices...
    int numShapeCoords = sectionCoords->point.getNum();
    int numPathCoords  = loftPathCoords->point.getNum();

    int numSides = numShapeCoords - 1;   /// this varies with geometry...
    int num = loftCoords->point.getNum();
    int count = 0;

    if (fUpperLowerDialog == NULL)
    {
    fLowerLimit = 0;
    fUpperLimit = loftPathCoords->point.getNum()- 1;
    }
    else
    {
    fLowerLimit = fUpperLowerDialog->fInteger1;
    fUpperLimit = fUpperLowerDialog->fInteger2;
    }

    /*
    for(int j = 0; j < numShapeCoords - 1; j++)
    {
    for (int i = j + fLowerLimit; i < fUpperLimit; i++) {
    loftFaces->coordIndex.set1Value(count++, i);
    loftFaces->coordIndex.set1Value(count++, i + 1);
    i += numShapeCoords - 1;
    }
    loftFaces->coordIndex.set1Value(count++, -1);
    }
    */

    if (fUpperLimit == loftPathCoords->point.getNum())
    fUpperLimit--;


    #ifdef GENERATE_CENTERLINES

    SoCoordinate3 coords[10];
    for(i = 0; i < 10; i++)
    {
    coords[i].ref();
    coords[i].point.deleteValues(0,-1);
    }

    for(int j = 0; j < numSides; j++)
    {
    //for(int k = j; k < loftCoords->point.getNum(); k += numSides)
    for(int k = j; k < midlineCoords->point.getNum(); k += numSides)
    {
    coords[j].point.set1Value(coords[j].point.getNum(), midlineCoords->point[k]);
    }     
    }
    SoWriteAction wa;
    wa.getOutput()->openFile("centerlines_e.iv");

    for(i=0; i<numSides; i++)
    //for(i=0; i<1; i++)
    {
    wa.apply(&coords[i]);
    //wa.apply(midlineCoords);
    SoLineSet* lineSet = new SoLineSet;
    lineSet->ref();
    wa.apply(lineSet);
    lineSet->unref();
    }
    wa.getOutput()->closeFile();

    #endif

    for( int jj = 0; jj < numShapeCoords - 1; jj++)
    {
    for (int i = jj + fLowerLimit * numShapeCoords; i < jj + fUpperLimit * numShapeCoords; i++) 
    {
    loftFaces->coordIndex.set1Value(count++, i);
    loftFaces->coordIndex.set1Value(count++, i + 1);
    loftFaces->coordIndex.set1Value(count++, i + numShapeCoords);
    loftFaces->coordIndex.set1Value(count++, -1);

    loftFaces->coordIndex.set1Value(count++, i + 1);
    loftFaces->coordIndex.set1Value(count++, i + numShapeCoords + 1);
    loftFaces->coordIndex.set1Value(count++, i + numShapeCoords);
    loftFaces->coordIndex.set1Value(count++, -1);

    i += numShapeCoords - 1;
    }
    }
}

/*
float CpMakerView::interpolateZ(float length)
{
   TRACE("interpolate z\n");
	// we take the y-value and interpolate a z-value...
	int num = vertCoords->point.getNum();
	float offset =	vertCoords->point[0][0];  // assures that profile starts at 0
	for (int i = 0; i < num; i++)
	{
		if (vertCoords->point[i][0] - offset >= length)
		{
			if (i == 0)
				return vertCoords->point[i][1];

			float x2 = vertCoords->point[i][0] - offset;
			float x1 = vertCoords->point[i - 1][0] - offset;
			float ratio = (length - x1) / (x2 - x1);
			float z2 = vertCoords->point[i][1];
			float z1 = vertCoords->point[i - 1][1];
			float z = (z2 - z1) * ratio + z1;
			return z;
		}
	}
	return vertCoords->point[num-1][1]; // return the last "z" value if horizontal
	                                    // line is longer than vertical line.
}
*/


SbVec3f CpMakerView::interpolateScale(float length /* really a ratio of length : full-length  */)
{
	//TRACE("length = %f\n", length);
	// we take the y-value and interpolate a z-value...
     TRACE("interpolateScale\n");
	int num = hScaleCoords->point.getNum();
	float hLength = length * hScaleCoords->point[num-1][0];
	float vScale = 1;
	float hScale = 1;
	if (num < 2) hScale = hScaleCoords->point[0][1];
	else if (length >= 1)
		hScale = hScaleCoords->point[num - 1][1];
	else 
	{
		for (int i = 0; i < num; i++)
		{
			if (hScaleCoords->point[i][0] == hLength)
			{
				hScale = hScaleCoords->point[i][1];
				TRACE("case 3 !!!!!!\n");
				i = num;
			}
			else if (hScaleCoords->point[i][0] > hLength)
			{
				if (i == 0)
				{
					hScale = hScaleCoords->point[i][1];
					i = num;
				}
				else 
				{
					float x2 = hScaleCoords->point[i][0];
					float x1 = hScaleCoords->point[i - 1][0];
					float ratio = (length - x1) / (x2 - x1);
					float z2 = hScaleCoords->point[i][1];
					float z1 = hScaleCoords->point[i - 1][1];
					float z = (z2 - z1) * ratio + z1;
					hScale = z;
					//TRACE("hScale = %f\n", hScale);
					i = num;
				}
			}
		}
	}
	if (fIsEqualScale)       // Oct 2004 !!!!!
   {
		vScale = hScale;
      return SbVec3f(hScale, vScale, 1);
   }
 
   //vscalecoords
	//////////////////////////////////////////////////////
TRACE ("using VSCALE coords at the wrong time");
	num = vScaleCoords->point.getNum();
	float vLength = length * vScaleCoords->point[num-1][0];
	if (num < 2) return SbVec3f(hScale,vScale,1);
	for (int i = 0; i < num; i++)
	{
		if (vScaleCoords->point[i][0] >= length)
		{
			if (i == 0)
			{
				vScale = vScaleCoords->point[0][0];
				i = num;
			}
			else 
			{
				float x2 = vScaleCoords->point[i][0];
				float x1 = vScaleCoords->point[i - 1][0];
				float ratio = (length - x1) / (x2 - x1);
				float z2 = vScaleCoords->point[i][1];
				float z1 = vScaleCoords->point[i - 1][1];
				float z = (z2 - z1) * ratio + z1;
				vScale = z;
				i = num;
			}
		}
	}	
	return SbVec3f(hScale, vScale, 1);
}

////////////////////////////////////////////////////////////////////////////
#ifdef USE_SECOND_SCALE
SbVec3f CpMakerView::interpolateScale2(float length /* really a ratio of length : full-length  */)
{
	//TRACE("length = %f\n", length);
	// we take the y-value and interpolate a z-value...

	int num = vScaleCoords->point.getNum();
	float hLength = length * vScaleCoords->point[num-1][0];
	float vScale = 1;
	if (num < 2) vScale = vScaleCoords->point[0][1];
	else if (length >= 1)
		vScale = vScaleCoords->point[num - 1][1];
	else 
	{
		for (int i = 0; i < num; i++)
		{
			if (vScaleCoords->point[i][0] == hLength)
			{
				vScale = vScaleCoords->point[i][1];
				TRACE("case 3 !!!!!!\n");
				i = num;
			}
			else if (vScaleCoords->point[i][0] > hLength)
			{
				if (i == 0)
				{
					vScale = vScaleCoords->point[i][1];
					i = num;
				}
				else 
				{
					float x2 = vScaleCoords->point[i][0];
					float x1 = vScaleCoords->point[i - 1][0];
					float ratio = (length - x1) / (x2 - x1);
					float z2 = vScaleCoords->point[i][1];
					float z1 = vScaleCoords->point[i - 1][1];
					float z = (z2 - z1) * ratio + z1;
					vScale = z;
					//TRACE("vScale = %f\n", vScale);
					i = num;
				}
			}
		}
	}
   return SbVec3f(vScale, vScale, 1);
}
#endif // USE_SECOND_SCALE
///////////////////////////////////////////

double CpMakerView::GetTheta(SbVec3f pt1, SbVec3f pt2)
{
         TRACE("GetTheta1\n");
	return  atan2 (( pt2[1] - pt1[1]) , ( pt2[0] - pt1[0] ));   
}


double CpMakerView::GetTheta(double ptAx, double ptAy, double ptBx, double ptBy)
{
         TRACE("GetTheta2\n");
	return atan2 (( ptBy - ptAy ) , ( ptBx - ptAx));
}

double CpMakerView::GetTheta(double sideX, double sideY)
{
         TRACE("GetTheta3\n");
	return atan2 (sideY, sideX);
}

SbRotation CpMakerView::interpolatePRY(SbVec3f pt1, SbVec3f pt2, SbVec3f pt3)
{
         TRACE("interpolatePRY\n");
	// doing it this way eliminates any roll...
	SbRotation horizRot1(SbVec3f(0,1,0), SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0));
	SbRotation horizRot2(SbVec3f(0,1,0), SbVec3f(pt3[0] - pt2[0], pt3[1] - pt2[1], 0));

	TRACE("theta 1 = %f   theta 2 = %f  \n", GetTheta(pt2, pt1), GetTheta(pt3, pt2));
	float theta1 = GetTheta(pt2, pt1); // + 1.5708;
	float theta2 = GetTheta(pt3, pt2); // + 1.5708;

	// if one is positive and one is negative, i.e., straddles 180/-180...
	if (fabs(theta1 - theta2) > 3.1415926)
	{
		if (theta1 < 0) theta1 = 3.1415926 + (3.1415926 + theta1);
		if (theta2 < 0) theta2 = 3.1415926 + (3.1415926 + theta2);
		TRACE("fixed the theta problem\n");
	}
	
	//SbRotation horizRot1(0, 0, 1, theta1);
	//SbRotation horizRot2(0, 0, 1, theta2);

	SbRotation vertRot1(SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0), pt2 - pt1);
	SbRotation vertRot2(SbVec3f(pt3[0] - pt2[0], pt3[1] - pt2[1], 0), pt3 - pt2);

	horizRot1 *= vertRot1;
	horizRot2 *= vertRot2;
	SbRotation result = SbRotation::slerp(horizRot1, horizRot2, .5);

	return result;
}


SbRotation CpMakerView::interpolatePRY2(SbVec3f pt1, SbVec3f pt2, SbVec3f pt3)
{
             TRACE("interpolatePRY2\n");
	// doing it this way eliminates any roll...

	SbRotation horizRot1(SbVec3f(0,1,0), SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0));
	SbRotation horizRot2(SbVec3f(0,1,0), SbVec3f(pt3[0] - pt2[0], pt3[1] - pt2[1], 0));

	SbRotation vertRot1(SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0), pt2 - pt1);
	SbRotation vertRot2(SbVec3f(pt3[0] - pt2[0], pt3[1] - pt2[1], 0), pt3 - pt2);

	//SbRotation rollRot1(SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0), pt2 - pt1);
	//SbRotation rollRot2(SbVec3f(pt3[0] - pt2[0], pt3[1] - pt2[1], 0), pt3 - pt2);

	horizRot1 *= vertRot1;
	horizRot2 *= vertRot2;
	SbRotation result = SbRotation::slerp(horizRot1, horizRot2, .5);   //  !!!!! Oct 2004
	//SbRotation result = horizRot1;   // !!!!!  
	return result;
}


SbRotation CpMakerView::interpolatePRY2_Align_with_Reference_Centerline(int i)
{
             TRACE("interpolatePRY2 align\n");
///////////////////////////////////////////////////////////////////
#ifdef ORIENT_TO_REFERENCE_CENTERLINE
   
   //SbRotation dummy;
   //dummy.identity();
   //return dummy;


   SbVec3f pt1;
   SbVec3f pt2;
   SbVec3f pt3;
   SbRotation rot;
	int numPathVertices  = loftPathCoords->point.getNum();
   if (i == 0)
   {
		rot = this->getPRY(loftPathCoords->point[i], loftPathCoords->point[i+1]);
      return rot;
   }
	else if (i == numPathVertices - 1)
   {
		rot = this->getPRY(loftPathCoords->point[i-1], loftPathCoords->point[i]);
      return rot;
   }
	else
   {
		pt1 = loftPathCoords->point[i - 1]; 
		pt2 = loftPathCoords->point[i];
		pt3 = loftPathCoords->point[i + 1];
   
      SbVec3f centerlinePoint = pt2;
      SbVec3f referenceVector = pt2 - fReferenceCenterlineCoords->point[i];
      // take the angle from 0,0,-1 to moved reference point to get rotation and apply rotation.
      // using constructor: SbRotation(const SbVec3f &rotateFrom, const SbVec3f &rotateTo)
      SbRotation rotTowardReference(SbVec3f(0,-1,0), referenceVector);
   
      // find the new z vector direction...
      SbVec3f srcVec(0,0,1);
      SbVec3f dstVec;
      rotTowardReference.multVec(srcVec, dstVec);

      // rotate the current perpendicular toward the interpolated direction of the current extrusion
	   SbRotation horizRot1(SbVec3f(0,1,0), SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0));
	   SbRotation horizRot2(SbVec3f(0,1,0), SbVec3f(pt3[0] - pt2[0], pt3[1] - pt2[1], 0));

	   SbRotation vertRot1(SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0), pt2 - pt1);
	   SbRotation vertRot2(SbVec3f(pt3[0] - pt2[0], pt3[1] - pt2[1], 0), pt3 - pt2);

 	   horizRot1 *= vertRot1;
	   horizRot2 *= vertRot2;
	   SbRotation result = SbRotation::slerp(horizRot1, horizRot2, .5);   //  !!!!! Oct 2004
      
      return result;

      SbVec3f srcVec2(0,0,1);
      SbVec3f dstVec2;
      result.multVec(srcVec2, dstVec2);

      SbRotation adjustRot = SbRotation(dstVec, dstVec2);

      //rotTowardReference *= adjustRot;
      return rotTowardReference;
   }
}
#else
////////////////////////////////////////////////////////////////////      // move current centerline point to 0,0.  Move current reference centerline point by same amount.
   SbRotation dummy;
   dummy.identity();
   return dummy;
}
#endif 

SbRotation CpMakerView::interpolatePRY3(SbVec3f pt1, SbVec3f pt2, SbVec3f pt3)
{
             TRACE("interpolatePRY3\n");
	//SbRotation rot1(pt2 - pt1);
	//SbRotation rot2(pt3 - pt2);
	//SbRotation result = SbRotation::slerp(rot1, rot2, .5);
	SbRotation result;
	return result;
}


SbRotation CpMakerView::getPRY(SbVec3f pt1, SbVec3f pt2)
{
    TRACE("getPRY\n");
	SbRotation horizRot1(SbVec3f(0,1,0), SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0));
	SbRotation vertRot1(SbVec3f(pt2[0] - pt1[0], pt2[1] - pt1[1], 0), pt2 - pt1);
	horizRot1 *= vertRot1;
	return horizRot1;
}

//#endif // USE_LOCAL_EXTRUDER


void CpMakerView::OnSaveLoft() 
{
  BOOL    bAddFileDialog = FALSE;
  LPCTSTR lpszFilter = NULL;
  LPCTSTR lpszDefExt = "_e.iv";
  LPCTSTR lpszFileName = "*_e.iv";
  DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  CWnd    *pParentWnd = NULL;
    
  CFileDialog addFileDialog(bAddFileDialog,
      lpszDefExt, lpszFileName, dwFlags,
      lpszFilter, pParentWnd);

  addFileDialog.m_ofn.lpstrTitle = LPCTSTR("Save the Loft Object...");
  int nModal = addFileDialog.DoModal();
  CString m_strAddFile;

    if (nModal == IDOK) {
        m_strAddFile = addFileDialog.GetFileName();
        SetCurrentDirectory(addFileDialog.GetFolderPath());
		SoWriteAction wa;
		wa.getOutput()->openFile(m_strAddFile);
		if (fClipPlaneSep != NULL)
		{
			SoPath* xformPath = this->FindPathToNode(loftRoot, fClipPlaneManip);
			xformPath->ref();
			SoTransform* newTrans = new SoTransform;
			fClipPlaneManip->replaceManip(xformPath, newTrans);
			//fLoftTransform = newTrans;
			xformPath->unref();
		}
		wa.apply(loftRoot); 
        wa.apply(treeRoot);
		wa.getOutput()->closeFile();
    }	
}

void CpMakerView::OnRemoveLoft() 
{
	//loftFaces->coordIndex.setValue(-1);
	//loftCoords->point.deleteValues(0,-1);
    treeRoot->removeAllChildren();
    loftRoot->removeAllChildren();
}

void CpMakerView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CpMakerView::OnSaveConstructionData() 
{
	BOOL    bAddFileDialog = FALSE;
	LPCTSTR lpszFilter = NULL;
	LPCTSTR lpszDefExt = "ldf";
	LPCTSTR lpszFileName = "*.ldf";
	DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd    *pParentWnd = NULL;
 
	CFileDialog addFileDialog(bAddFileDialog,
		lpszDefExt, lpszFileName, dwFlags,
		lpszFilter, pParentWnd);

	addFileDialog.m_ofn.lpstrTitle = LPCTSTR("Save the Loft Construction Data...");
	int nModal = addFileDialog.DoModal();
	CString m_strAddFile;

	if (nModal != IDOK) return;
        
	m_strAddFile = addFileDialog.GetFileName();
    SetCurrentDirectory(addFileDialog.GetFolderPath());
	SoWriteAction wa;
	wa.getOutput()->openFile(m_strAddFile);
	wa.apply(sectionCoords);
	wa.apply(hScaleCoords);
	wa.apply(vScaleCoords);
	wa.apply(twistCoords);
	wa.apply(threeDCoords);
	wa.getOutput()->closeFile();
}

void CpMakerView::OnLoadConstructionData() 
{
	BOOL    bAddFileDialog = TRUE;
	LPCTSTR lpszFilter = NULL;
	LPCTSTR lpszDefExt = "ldf";
	LPCTSTR lpszFileName = "*.ldf";
	DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd    *pParentWnd = NULL;
 
	CFileDialog addFileDialog(bAddFileDialog,
		lpszDefExt, lpszFileName, dwFlags,
		lpszFilter, pParentWnd);

	addFileDialog.m_ofn.lpstrTitle = LPCTSTR("Load Loft Construction Data...");

	int nModal = addFileDialog.DoModal();
	if (nModal != IDOK) return;
	CString ldf_filename = addFileDialog.GetFileName();
    SetCurrentDirectory(addFileDialog.GetFolderPath());

    this->ReadLDF(ldf_filename, sectionCoords, 
                                threeDCoords, 
                                hScaleCoords, 
                                vScaleCoords,
                                twistCoords);
	
    
    // this->make3dPath();
	// this->makeLoftObject();
    loftRoot->removeAllChildren();  
    if((vScaleCoords->point[0] - vScaleCoords->point[vScaleCoords->point.getNum() -1]).length() - 1.0 < 0.001) {
        loftRoot->addChild(fExtruder->extrude(sectionCoords, threeDCoords, hScaleCoords, hScaleCoords, twistCoords, 1.0, 0.0, false));
    } else {
        loftRoot->addChild(fExtruder->extrude(sectionCoords, threeDCoords, hScaleCoords, vScaleCoords, twistCoords, 1.0, 0.0, false));
    }
}

SoNode * CpMakerView::findNodeByName(SoGroup *parent, char *name)
{
    SoSearchAction search;
    SoPath *path;
    SoNode *node;
    search.setName(SbName(name));
    search.setInterest(SoSearchAction::FIRST);
    search.apply(parent);
    path = search.getPath();
    if (path == NULL) return NULL;
    node = path->getTail();
    return node;
}

SoPath*
CpMakerView::FindPathToNode(SoGroup* parent, SoNode* node)
{
	SoSearchAction search;
	SoPath* path;
	search.setNode(node);
	search.apply(parent);
	path = search.getPath();
	//TRACE("length = %d\n", path->getLength());
	if (path == NULL) return NULL;
	path->ref();
	return path;
}
//
float
CpMakerView::dist (SbVec3f pt1, SbVec3f pt2)
{
	float   distance;
	distance = sqrt((pt2[0] - pt1[0])*(pt2[0] - pt1[0]) \
		  +(pt2[1] - pt1[1])*(pt2[1] - pt1[1]));
	return distance;
}

//#ifdef USE_LOCAL_EXTRUDER
/////////////////////////////////////////////////////////////////////////////////////
// for flattening shapes...

void CpMakerView::OnFlatten() //quadrilaterals
{
    fExtruder->extrude(sectionCoords, threeDCoords, hScaleCoords, vScaleCoords, twistCoords, 1, 0, true); 
    return;
}

double CpMakerView::GetVectorAngle(double a1, double a2, double a3, double b1, double b2, double b3)
{
	double lengthA  = sqrt( (a1 * a1) + (a2 * a2) + (a3 * a3) ); 	
	double lengthB  = sqrt( (b1 * b1) + (b2 * b2) + (b3 * b3) ); 	
	double dotAB    = a1 * b1 + a2 * b2 + a3 * b3;
	double cosAngle = dotAB / (lengthA * lengthB);
	double angle    = acos(cosAngle);
	return angle;
}


void CpMakerView::flatten (SoCoordinate3* tdCoords, SoCoordinate3* flatCoords)
{                     
	const double kPI = 3.1415926535897932384626433832795;
	double xplot1, xplot2, xplot3;
	double yplot1, yplot2, yplot3, yplot4;
	double side3;
	//double xside1;
	double x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
	double ang1, ang2, angA, angB;
	double r;
	//int count;

	xplot1 = 0.0;
	yplot1 = 0.0;
	
	int flatCount = 0;

	double angle1;
	double angle2;

	x1 = tdCoords->point[0][0];
	y1 = tdCoords->point[0][1];
	z1 = tdCoords->point[0][2];
	
	x2 = tdCoords->point[1][0];
	y2 = tdCoords->point[1][1];
	z2 = tdCoords->point[1][2];
	
	x3 = tdCoords->point[2][0];
	y3 = tdCoords->point[2][1];
	z3 = tdCoords->point[2][2];

	x4 = tdCoords->point[3][0];
	y4 = tdCoords->point[3][1];
	z4 = tdCoords->point[3][2];
	
	double a1 = x1 - x2;
	double a2 = y1 - y2;
	double a3 = z1 - z2;
	double b1 = x3 - x2;
	double b2 = y3 - y2;
	double b3 = z3 - z2;
	
	angle2 = GetVectorAngle(a1, a2, a3, b1, b2, b3);

	TRACE("angle = %f\n", angle2);

	double lengthA  = sqrt(a1*a1 + a2*a2 + a3*a3);
	double lengthB  = sqrt(b1*b1 + b2*b2 + b3*b3);

	double plotVectorADirection = kPI;
	double plotVectorBDirection = angle2;

	xplot2 = xplot1 - lengthA;
	yplot2 = yplot1;

	flatCoords->point.set1Value(flatCount++, SbVec3f(xplot1, yplot1, 0.0000));
	flatCoords->point.set1Value(flatCount++, SbVec3f(xplot2, yplot2, 0.0000));

	xplot3 = cos(angle2) * lengthB + xplot2;
	yplot3 = sin(angle2) * lengthB + yplot2;
	
	flatCoords->point.set1Value(flatCount++, SbVec3f(xplot3, yplot3, 0.0000));

	//count += 2;
	
	for (int i = 2; i < tdCoords->point.getNum()-1; i += 2)
	{
		a1 = x2 - x3;
		a2 = y2 - y3;
		a3 = z2 - z3;
		b1 = x4 - x3;
		b2 = y4 - y3;
		b3 = z4 - z3;

		angle1 = GetVectorAngle(a1,a2,a3,b1,b2,b3);
		TRACE("angle = %f\n", angle1);
		plotVectorADirection = plotVectorBDirection + kPI - angle1;

		xplot1 = xplot3;
		yplot1 = yplot3;

		x1 = x3;
		y1 = y3;
		z1 = z3;

		x2 = x4;
		y2 = y4;
		z2 = z4;

		x3 = tdCoords->point[i+2][0];
		y3 = tdCoords->point[i+2][1];
		z3 = tdCoords->point[i+2][2];
		
		x4 = tdCoords->point[i+3][0];
		y4 = tdCoords->point[i+3][1];
		z4 = tdCoords->point[i+3][2];
      		
		// make vectors out of A and B
		double a1 = x1 - x2;
		double a2 = y1 - y2;
		double a3 = z1 - z2;
		double b1 = x3 - x2;
		double b2 = y3 - y2;
		double b3 = z3 - z2;
		
		lengthA = sqrt(a1*a1 + a2*a2 + a3*a3);

		xplot2 = cos(plotVectorADirection) * lengthA + xplot1;
		yplot2 = sin(plotVectorADirection) * lengthA + yplot1;

		flatCoords->point.set1Value(flatCount++, SbVec3f(xplot2, yplot2, 0.0000));

		a1 = x1 - x2;
		a2 = y1 - y2;
		a3 = z1 - z2;

		b1 = x3 - x2;
		b2 = y3 - y2;
		b3 = z3 - z2;

		angle2 = GetVectorAngle(a1, a2, a3, b1, b2, b3);
		TRACE("angle = %f\n", angle2);
	
		plotVectorBDirection = plotVectorADirection - kPI + angle2; 
		lengthB  = sqrt(b1* b1 + b2 * b2 + b3 * b3);
		
		xplot3 = cos(plotVectorBDirection) * lengthB + xplot2;
		yplot3 = sin(plotVectorBDirection) * lengthB + yplot2;

		flatCoords->point.set1Value(flatCount++, SbVec3f(xplot3, yplot3, 0.0000));

		//count += 2;

	}
	SoSeparator*   tempSep    = new SoSeparator;
	tempSep->ref();
	SoCoordinate3* tempCoords = new SoCoordinate3;
	tempCoords->point.deleteValues(0,-1);
	SoLineSet*     tempLines  = new SoLineSet;
	tempSep->addChild(tempCoords);
	tempSep->addChild(tempLines);


	for (int i = 0; i < flatCoords->point.getNum(); i++)
	{
		tempCoords->point.set1Value(i, flatCoords->point[i]);
	}

	SoWriteAction wa;
	wa.getOutput()->openFile("flat.iv");
	wa.apply(tempSep);
	wa.getOutput()->closeFile();

	tempSep->unref();

	return;
}



double CpMakerView::hypotenuse ( double xside, double yside )
{
	double zside;

	zside = sqrt ((( xside * xside ) + ( yside * yside )));
	return zside;
}


void CpMakerView::OnFlattenPolyline() 
{
   // Flattener::flatten_polylines()
	// create new DXF file for flattened sides...
	BOOL    bAddFileDialog = FALSE;
	LPCTSTR lpszFilter = NULL;
	LPCTSTR lpszDefExt = "dxf";
	LPCTSTR lpszFileName = "*.dxf";
	DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd    *pParentWnd = NULL;

	CFileDialog addFileDialog(bAddFileDialog,
	  lpszDefExt, lpszFileName, dwFlags,
	  lpszFilter, pParentWnd);

	addFileDialog.m_ofn.lpstrTitle = LPCTSTR("create a flattened-sides DXF file...");
	int nModal = addFileDialog.DoModal();
	CString m_strAddFile;

	if (nModal != IDOK) return;
	m_strAddFile = addFileDialog.GetFileName();
    SetCurrentDirectory(addFileDialog.GetFolderPath());

	FILE* fp = fopen(m_strAddFile, "w");
	fprintf(fp,"  0\nSECTION\n  2\nENTITIES\n");

	int numSides = sectionCoords->point.getNum() - 1;
	int numPathCoords = threeDCoords->point.getNum();

	SoCoordinate3 *newCoords = new SoCoordinate3;
	newCoords->ref();
	SoCoordinate3* flatCoords = new SoCoordinate3;
	flatCoords->ref();
	
	SoSeparator* tempSep = new SoSeparator;
	tempSep->ref();
	tempSep->addChild(newCoords);
	SoLineSet *newLines = new SoLineSet;
	tempSep->addChild(newLines);
	int indexCount = 0;
	int index;

	for(int j = 0; j < numSides /*- 1*/; j++)  {
		int count = 0;
		newCoords->point.deleteValues(0, -1);
		flatCoords->point.deleteValues(0, -1);

		for ( int k = j; k < loftCoords->point.getNum() - 1; k += numSides + 1)  {
			if (j < numSides)  {
				newCoords->point.set1Value(count++, loftCoords->point[k]);
				newCoords->point.set1Value(count++, loftCoords->point[k + 1]);
			}
			else if (fClosedShape == TRUE)  {
				newCoords->point.set1Value(count++, loftCoords->point[k]);
				newCoords->point.set1Value(count++, loftCoords->point[k - numSides + 1]);
			}
		}

		flatten(newCoords, flatCoords);
		int numVertices = flatCoords->point.getNum();


	// test...
	SoSeparator*	testSep = new SoSeparator;
	SoLineSet*		testLines = new SoLineSet;
	SoCoordinate3*	testCoords = new SoCoordinate3;
	testSep->ref();
	testSep->addChild(testCoords);
	testSep->addChild(testLines);
	testCoords->point.deleteValues(0,-1);
	for (int kk = 0; kk < flatCoords->point.getNum(); kk++)
	{
		//TRACE("loftCoords->point[%d] = %f  %f  %f\n", k, loftCoords->point[k][0], loftCoords->point[k][1], loftCoords->point[k][2]);
		testCoords->point.set1Value(kk, flatCoords->point[kk]);
	}

	SoWriteAction wa;
	wa.getOutput()->openFile("test2.iv");
	wa.apply(testSep);
	wa.getOutput()->closeFile();

	testSep->unref();

		char layer[4];
		_itoa( j, layer, 10);
		fprintf(fp,"  0\nLWPOLYLINE\n  5\n21\n100\nAcDbEntity\n  8\n%s\n100\n", layer);
		fprintf(fp,"AcDbPolyline\n 90\n%9d\n 70\n   128\n 43\n0.0\n", numVertices );
		
		for (int i = 0; i < numVertices - 1; i+= 2)
		{		
			SbVec3f point = flatCoords->point[i];
			fprintf(fp," 10\n");
			fprintf(fp,"%f\n", point[0]);
			fprintf(fp," 20\n");
			fprintf(fp,"%f\n", point[1]);

		}
		for (int i = numVertices - 2; i > 0; i -= 2)
		{
			SbVec3f point = flatCoords->point[i];
			fprintf(fp," 10\n");
			fprintf(fp,"%f\n", point[0]);
			fprintf(fp," 20\n");
			fprintf(fp,"%f\n", point[1]);
		}
	}

	fprintf(fp,"0\nENDSEC\n  0\nEOF\n");
	fclose(fp);

	tempSep->unref();
	flatCoords->unref();
	newCoords->unref();
}	

//#endif //USE_LOCAL_EXTRUDER


SoNode *
CpMakerView::findChildOfType(SoGroup * parent, SoType typeId)
{
    SoSearchAction search;
    SoPath *path;
    SoNode *node;
    
    search.setType(typeId, FALSE);
    search.setInterest(SoSearchAction::FIRST);
    search.apply(parent);
    
    path = search.getPath();
    if (path == NULL) return NULL;
    node = path->getTail();
    return node;
}

void CpMakerView::OnEditBeginEnd() 
{
	fUpperLowerDialog = new CGetIntegerDialog("Set Limits","Lower Limit", "Upper Limit", this);
	fUpperLowerDialog->fInteger1 = 0;
	fLowerLimit = 0;
	fUpperLimit = loftPathCoords->point.getNum();
	fUpperLowerDialog->fInteger2 = fUpperLimit;
	fUpperLowerDialog->Create();
}

void CpMakerView::OnEditMultiples() 
{
	OneIntegerDlg iDlg;	
   fNumRotatedCopies = 1;
	iDlg.fInteger = fNumRotatedCopies;
	int ret = iDlg.DoModal();

	fMultipleSep->removeAllChildren();

	fNumRotatedCopies = iDlg.fInteger;
	float rotationIncrement = (3.141592653 * 2.0) / fNumRotatedCopies;
	TRACE("numcopies = %d\n", fNumRotatedCopies);
	//for (int i = 0; i < fNumRotatedCopies - 1; i++)
	for (int i = 0; i < fNumRotatedCopies; i++)
	{
		SoSeparator* tSep = new SoSeparator;
		SoTransform* tTrans = new SoTransform;
		
		float rotation = rotationIncrement * (i + 1);
		TRACE("rotation = %f\n", rotation);
		tTrans->rotation.setValue(SbRotation(SbVec3f(0, 0, 1), rotation));
		
		tSep->addChild(tTrans);
		tSep->addChild(loftRoot);
		fMultipleSep->addChild(tSep);
	}
}

void CpMakerView::OnEditTransformBoxOn() 
{
	SoPath* xformPath = this->FindPathToNode(root, fLoftTransform);
	int length = xformPath->getLength();
	SoNode* test = xformPath->getTail();
	if (xformPath->getTail() != fLoftTransform)
	{
		TRACE("not the correct node!\n");
		return;  // !!!
	}
	xformPath->ref();
   fTransformBox->replaceNode(xformPath);   
	xformPath->unref();
}

void CpMakerView::OnEditTransformBoxOff() 
{
	SoPath* xformPath = this->FindPathToNode(root, fTransformBox);
	xformPath->ref();
	SoTransform* newTrans = new SoTransform;
	fTransformBox->replaceManip(xformPath, newTrans);
	fLoftTransform = newTrans;
	xformPath->unref();
}

void CpMakerView::OnRemoveTransform() 
{
  // need error checking .. doesn't work when the xformBox is operative
	fLoftTransform->setToDefaults();

}


void CpMakerView::OnFileSaveMultiiples() 
{
  BOOL    bAddFileDialog = FALSE;
  LPCTSTR lpszFilter = NULL;
  LPCTSTR lpszDefExt = "_e.iv";
  LPCTSTR lpszFileName = "*_e.iv";
  DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
  CWnd    *pParentWnd = NULL;
    
  CFileDialog addFileDialog(bAddFileDialog,
      lpszDefExt, lpszFileName, dwFlags,
      lpszFilter, pParentWnd);

  addFileDialog.m_ofn.lpstrTitle = LPCTSTR("Save the Multiple Loft Object...");
  int nModal = addFileDialog.DoModal();
  CString m_strAddFile;

    if (nModal == IDOK) {
        m_strAddFile = addFileDialog.GetFileName();
        SetCurrentDirectory(addFileDialog.GetFolderPath());
		SoWriteAction wa;
		wa.getOutput()->openFile(m_strAddFile);
		//wa.apply(loftRoot);
		wa.apply(fMultipleSep);
		wa.getOutput()->closeFile();
    }	
}

SbVec3f CpMakerView::CalcVectorPoint(SbVec3f pt, float length, float theta)
{
	SbVec3f temp;
	temp[0] = pt[0] + length * cos(theta);
	temp[1] = pt[1] + length * sin(theta);
	temp[2] = 0.0;
	return temp;
}

SbVec3f CpMakerView::GetIntersection(SbVec3f ptA1, SbVec3f ptA2, SbVec3f ptB1, SbVec3f ptB2)
{
	// Segment intersection formulae; see the Antonio reference...
	float tx = ptA2[0] - ptA1[0];
	float ty = ptA2[1] - ptA1[1];
	float ex = ptB2[0] - ptB1[0];
	float ey = ptB2[1] - ptB1[1];
	float etx = ptA1[0] - ptB2[0];
	float ety = ptA1[1] - ptB2[1];
  
	float denom = ty * ex - tx * ey;
	ASSERT(denom != 0.0); 

	float split = (ey * etx - ex * ety) / denom;
	// Create the new point...
	SbVec3f newpoint;
	// Interpolate its coordinate and attributes... 
	//newpoint[i] = torg[i] + split * (tdest[i] - torg[i]);
	newpoint[0] = ptA1[0] + split * (ptA2[0] - ptA1[0]);
	newpoint[1] = ptA1[1] + split * (ptA2[1] - ptA1[1]);
	return newpoint;
}

void CpMakerView::OnEditOffset() 
{
	GetFloatDialog dlg;
	dlg.fFloat = fOffset;
	int ret = dlg.DoModal();
	if (ret != IDOK) return;

	fOffset = dlg.fFloat;
	//this->make3dPath();
	//this->makeLoftObject();
}

void CpMakerView::OnEditAddHoles() 
{
	// holes are added when flattening...
	HoleDialog dlg;
	dlg.fHoleOffset = fHoleOffset;
	dlg.fHoleSpacing = fHoleSpacing;
	int ret = dlg.DoModal();
	if (ret != IDOK) return;

	fHoleOffset = dlg.fHoleOffset;
	fHoleSpacing = dlg.fHoleSpacing;
}

void CpMakerView::OnEditClipPlane() 
{
	if (fClipPlaneSep == NULL)
	{
		fClipPlaneSep = new SoSeparator;
		fClipPlaneSep->ref();
		fClipPlaneCoords = new SoCoordinate3;
		fClipPlaneFaces = new SoFaceSet;
		fClipPlaneManip = new SoTransformBoxManip;
		fClipPlaneManip->rotation.setValue(SbVec3f(0,1,0), 1.5708);
		fClipPlaneManip->scaleFactor.setValue(SbVec3f(10,10,10));
		fClipIntersectionCoords = new SoCoordinate3;
		fClipIntersectionLines = new SoLineSet;
		
		SoShapeHints* hints = new SoShapeHints;
		hints->shapeType.setValue(SoShapeHints::UNKNOWN_SHAPE_TYPE);
		hints->vertexOrdering.setValue(SoShapeHints::COUNTERCLOCKWISE);
		fClipPlaneSep->addChild(hints);

		fClipIntersectionCoords->point.deleteValues(0, -1);
		fClipPlaneSep->addChild(fClipIntersectionCoords);
		fClipPlaneSep->addChild(fClipIntersectionLines);
		fClipPlaneCoords->point.deleteValues(0, -1);
		fClipPlaneSep->addChild(fClipPlaneCoords);
		
		fClipPlaneCoords->point.set1Value(0, SbVec3f(0,0,0));
		fClipPlaneCoords->point.set1Value(1, SbVec3f(0,1,0));
		fClipPlaneCoords->point.set1Value(2, SbVec3f(1,1,0));
		fClipPlaneCoords->point.set1Value(3, SbVec3f(1,0,0));

		fClipPlaneFaces->numVertices.setValue(-1);
		fClipPlaneSep->addChild(fClipPlaneManip);
		fClipPlaneSep->addChild(fClipPlaneFaces);

		loftRoot->insertChild(fClipPlaneSep, 0);
	}
}

void CpMakerView::OnEditClipPlaneOff() 
{	
	if (fClipPlaneSep != NULL)
	{
		fClipPlaneSep->removeAllChildren();
		fClipPlaneSep->unref();
		fClipPlaneSep = NULL;

		fClipPlaneCoords = NULL;
		fClipPlaneFaces = NULL;
		fClipPlaneManip = NULL;
		fClipIntersectionCoords = NULL;
		fClipIntersectionLines = NULL;
	}
}

void CpMakerView::OnAntisquish() 
{	
   // CHECK ing menu items:
	CWnd* parent = GetParent();
	CMenu* mmenu = parent->GetMenu();
	CMenu* submenu = mmenu->GetSubMenu(1);

	UINT state = submenu->GetMenuState(ID_ANTISQUISH, MF_BYCOMMAND);
   ASSERT(state != 0xFFFFFFFF);

   if (state & MF_CHECKED)
   {
      submenu->CheckMenuItem(ID_ANTISQUISH, MF_UNCHECKED | MF_BYCOMMAND);
      fAntiSquish = FALSE;
   }
   else
   {
      submenu->CheckMenuItem(ID_ANTISQUISH, MF_CHECKED | MF_BYCOMMAND);
      fAntiSquish = TRUE;
   }
}

void CpMakerView::OnFlattenImportedCoords() 
{
    // flatten a zig-zag line
	BOOL    	bAddFileDialog = TRUE;
	LPCTSTR 	lpszFilter = NULL;
	LPCTSTR 	lpszDefExt = "iv";
	LPCTSTR 	lpszFileName = "*.iv";
	DWORD   	dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd *   pParentWnd = NULL;

	CFileDialog addFileDialog(bAddFileDialog,
	  lpszDefExt, lpszFileName, dwFlags,
	  lpszFilter, pParentWnd);

	addFileDialog.m_ofn.lpstrTitle = LPCTSTR("load a zig-zag coordinate set to flatten...");
	int nModal = addFileDialog.DoModal();

	SoSeparator* tempNode = new SoSeparator;
	tempNode->ref();
	SoCoordinate3* inputCoords = new SoCoordinate3;
	inputCoords->point.deleteValues(0,-1);

    if (nModal != IDOK) return;
    CString m_strAddFile = addFileDialog.GetFileName();
    SetCurrentDirectory(addFileDialog.GetFolderPath());

    ////  read in position node...
    SoInput myInput;
    myInput.openFile(m_strAddFile);
    tempNode->addChild(SoDB::readAll(&myInput));
    myInput.closeFile();  
	  
    SoCoordinate3 *coords = (SoCoordinate3 *)findChildOfType(tempNode, 
	                 SoCoordinate3::getClassTypeId());

    for (int i = 0; i < coords->point.getNum(); i++) {
        inputCoords->point.set1Value(i, coords->point[i]);
	}

	tempNode->removeAllChildren();

	int numSides = 1;
	int numPathCoords = inputCoords->point.getNum();

	SoCoordinate3* flatCoords = new SoCoordinate3;
	flatCoords->ref();
	
	//SoSeparator* tempSep = new SoSeparator;
	//tempSep->ref();
	//tempSep->addChild(newCoords);
	//SoLineSet *newLines = new SoLineSet;
	//tempSep->addChild(newLines);
	//int indexCount = 0;
	//int index;

    CString dxfFilename = m_strAddFile.SpanExcluding(".");
	dxfFilename += "_flat.dxf";

	FILE* fp = fopen(dxfFilename, "w");
    if(NULL == fp){
        TRACE("could not open file %s\n", dxfFilename);
        return;
    }
	fprintf(fp,"  0\nSECTION\n  2\nENTITIES\n");

	int count = 0;
	//newCoords->point.deleteValues(0, -1);
	flatCoords->point.deleteValues(0, -1);

	flatten(inputCoords, flatCoords);
		
		//SoWriteAction wa;
		//CString m_strAddFile = addFileDialog.GetFileName();
        // SetCurrentDirectory(addFileDialog.GetFolderPath());
		//wa.getOutput()->openFile("test.iv");
		//wa.apply(tempSep);
		//wa.apply(flatCoords); 
		//wa.getOutput()->closeFile();
		
    int numVertices = flatCoords->point.getNum();

    fprintf(fp,"  0\nLWPOLYLINE\n  5\n444\n100\nAcDbEntity\n  8\n%s\n100\n", "0");
	fprintf(fp,"AcDbPolyline\n 90\n%9d\n 70\n   128\n 43\n0.0\n", numVertices);
	//for(int i = 0; i < numVertices - 3; i += 2)
    for(int i = 0; i < numVertices; i++)
	{
		SbVec3f point = flatCoords->point[i];
		fprintf(fp," 10\n");
		fprintf(fp,"%f\n", point[0]);
		fprintf(fp," 20\n");
		fprintf(fp,"%f\n", point[1]);
	}
	fprintf(fp,"  0\nENDSEC\n  0\nEOF\n");
	fclose(fp);

//	tempSep->unref();
	flatCoords->unref();
//	newCoords->unref();
}




void CpMakerView::OnFlattenTwoLines() 
{
// loads two lines, makes a zig-zag line from them to flatten...
// load the first line...
	BOOL    	bAddFileDialog = TRUE;
	LPCTSTR 	lpszFilter = NULL;
	LPCTSTR 	lpszDefExt = "iv";
	LPCTSTR 	lpszFileName = "*.iv";
	DWORD   	dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd *   pParentWnd = NULL;

	CFileDialog addFileDialog(bAddFileDialog,
	  lpszDefExt, lpszFileName, dwFlags,
	  lpszFilter, pParentWnd);

	addFileDialog.m_ofn.lpstrTitle = LPCTSTR("load the first coordinate set to flatten...");
	int nModal = addFileDialog.DoModal();

	SoSeparator* tempNode = new SoSeparator;
	tempNode->ref();
	SoCoordinate3* inputCoords1 = new SoCoordinate3;
	inputCoords1->point.deleteValues(0,-1);
	
	CString m_strAddFile;

   if (nModal != IDOK) return;

	m_strAddFile = addFileDialog.GetFileName();
    SetCurrentDirectory(addFileDialog.GetFolderPath());

	////  read in position node...
	SoInput myInput;
	myInput.openFile(m_strAddFile);
	tempNode->addChild(SoDB::readAll(&myInput));
	myInput.closeFile();  

	SoCoordinate3 *coords = (SoCoordinate3 *)findChildOfType(tempNode, 
					  SoCoordinate3::getClassTypeId());

	for (int i = 0; i < coords->point.getNum(); i++)
	{
		inputCoords1->point.set1Value(i, coords->point[i]);
	}
	tempNode->removeAllChildren();

// load the second line...

//	BOOL    	bAddFileDialog = TRUE;
//	LPCTSTR 	lpszFilter = NULL;
//	LPCTSTR 	lpszDefExt = "iv";
//	LPCTSTR 	lpszFileName = "*.iv";
//	DWORD   	dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
//	CWnd *   pParentWnd = NULL;

//	addFileDialog(bAddFileDialog,
//	  lpszDefExt, lpszFileName, dwFlags,
//	  lpszFilter, pParentWnd);
	lpszFilter = NULL;
	lpszDefExt = "iv";
	lpszFileName = "*.iv";
	addFileDialog.m_ofn.lpstrTitle = LPCTSTR("load the second coordinate set to flatten...");
	nModal = addFileDialog.DoModal();

	SoSeparator* tempNode2 = new SoSeparator;
	tempNode2->ref();
	SoCoordinate3* inputCoords2 = new SoCoordinate3;
	inputCoords2->point.deleteValues(0,-1);
	
   if (nModal != IDOK) return;

	m_strAddFile = addFileDialog.GetFileName();
    SetCurrentDirectory(addFileDialog.GetFolderPath());

	////  read in node...
	myInput;
	myInput.openFile(m_strAddFile);
	tempNode2->addChild(SoDB::readAll(&myInput));
	myInput.closeFile();  

	coords = (SoCoordinate3 *)findChildOfType(tempNode2, 
					  SoCoordinate3::getClassTypeId());

	for (int i = 0; i < coords->point.getNum(); i++)
	{
		inputCoords2->point.set1Value(i, coords->point[i]);
	}

	tempNode2->removeAllChildren();
	
	// now combine the lines...	
	SoCoordinate3 * zigCoords = new SoCoordinate3;
	zigCoords->point.deleteValues(0,-1);
	int zigPointCount = 0;
	
	for (int i = 0; i < inputCoords1->point.getNum(); i++)
	{
		if ( i >= inputCoords2->point.getNum()) continue;
		zigCoords->point.set1Value(zigPointCount++, inputCoords1->point[i]);
		zigCoords->point.set1Value(zigPointCount++, inputCoords2->point[i]);
	}
	
	///////	

   CString dxfFilename = m_strAddFile.SpanExcluding(".");
	dxfFilename += "Flat.dxf";

	FILE* fp = fopen(dxfFilename, "w");
	fprintf(fp,"  0\nSECTION\n  2\nENTITIES\n");

	TRACE("filename = %s\n", dxfFilename);

	SoCoordinate3* flatCoords = new SoCoordinate3;
	flatCoords->ref();
	flatCoords->point.deleteValues(0, -1);

	flatten(zigCoords, flatCoords);
	
	SoWriteAction wa;
	m_strAddFile = addFileDialog.GetFileName();
    SetCurrentDirectory(addFileDialog.GetFolderPath());
	wa.getOutput()->openFile("test.iv");
	wa.apply(flatCoords); 
	wa.getOutput()->closeFile();
	
	int numVertices = flatCoords->point.getNum();
	char layer[4];
	_itoa_s( 1, layer, 10);

#define FLAT_POLYLINES

#ifdef FLAT_POLYGONS
	for(i = 0; i < numVertices - 3; i += 2)
	{
		fprintf(fp,"  0\nLWPOLYLINE\n  5\n21\n100\nAcDbEntity\n  8\n%s\n100\n", layer);
		fprintf(fp,"AcDbPolyline\n 90\n%9d\n 70\n   128\n 43\n0.0\n", numVertices);
	
		SbVec3f point = flatCoords->point[i];
		fprintf(fp," 10\n");
		fprintf(fp,"%f\n", point[0]);
		fprintf(fp," 20\n");
		fprintf(fp,"%f\n", point[1]);

		point = flatCoords->point[i + 2];
		fprintf(fp," 10\n");
		fprintf(fp,"%f\n", point[0]);
		fprintf(fp," 20\n");
		fprintf(fp,"%f\n", point[1]);

		point = flatCoords->point[i + 3];
		fprintf(fp," 10\n");
		fprintf(fp,"%f\n", point[0]);
		fprintf(fp," 20\n");
		fprintf(fp,"%f\n", point[1]);

		point = flatCoords->point[i +1];
		fprintf(fp," 10\n");
		fprintf(fp,"%f\n", point[0]);
		fprintf(fp," 20\n");
		fprintf(fp,"%f\n", point[1]);

		point = flatCoords->point[i];
		fprintf(fp," 10\n");
		fprintf(fp,"%f\n", point[0]);
		fprintf(fp," 20\n");
		fprintf(fp,"%f\n", point[1]);
	}

#endif

#ifdef FLAT_POLYLINES

		fprintf(fp,"  0\nLWPOLYLINE\n  5\n21\n100\nAcDbEntity\n  8\n%s\n100\n", layer);
		fprintf(fp,"AcDbPolyline\n 90\n%9d\n 70\n   128\n 43\n0.0\n", numVertices / 2);
		
		for(int i = 0; i < numVertices; i += 2)
		{
			SbVec3f point = flatCoords->point[i];
			fprintf(fp," 10\n");
			fprintf(fp,"%f\n", point[0]);
			fprintf(fp," 20\n");
			fprintf(fp,"%f\n", point[1]);
		}
		fprintf(fp,"  0\nLWPOLYLINE\n  5\n21\n100\nAcDbEntity\n  8\n%s\n100\n", layer);
		fprintf(fp,"AcDbPolyline\n 90\n%9d\n 70\n   128\n 43\n0.0\n", numVertices / 2);
		
		for(int i = 1; i < numVertices; i += 2)
		{
			SbVec3f point = flatCoords->point[i];
			fprintf(fp," 10\n");
			fprintf(fp,"%f\n", point[0]);
			fprintf(fp," 20\n");
			fprintf(fp,"%f\n", point[1]);
		}
#endif

	fprintf(fp,"0\nENDSEC\n  0\nEOF\n");
	fclose(fp);
	flatCoords->unref();	
}


void CpMakerView::OnVertEqHoriz() 
{
   // CHECK ing menu items:
	CWnd* parent = GetParent();
	CMenu* mmenu = parent->GetMenu();
	CMenu* submenu = mmenu->GetSubMenu(1);

	UINT state = submenu->GetMenuState(ID_VERT_EQ_HORIZ, MF_BYCOMMAND);
   ASSERT(state != 0xFFFFFFFF);

   if (state & MF_CHECKED)
   {
      submenu->CheckMenuItem(ID_VERT_EQ_HORIZ, MF_UNCHECKED | MF_BYCOMMAND);
      fIsEqualScale = FALSE;
   }
   else
   {
      submenu->CheckMenuItem(ID_VERT_EQ_HORIZ, MF_CHECKED | MF_BYCOMMAND);
      fIsEqualScale = TRUE;
   }
   this->OnShowDialog();  // updates the 3d object...
}




void
CpMakerView::mouseMoved(void *viewer, SoEventCallback *eventCB)
{
    const SoEvent *ev = eventCB->getEvent();
    CpMakerView *theView = (CpMakerView*) viewer;
	if (ev->isOfType(SoLocation2Event::getClassTypeId()))
	{
        int segmentNumber = 0;
        int sideNumber    = 0;

		// see what, if anything, the mouse is over...
		const SoPickedPoint *picked_point;
		if ((picked_point = eventCB->getPickedPoint()) == NULL)  
        {
            theView->fScreenText->string.setValue("");
            if (theView->fSegmentDlg != NULL)
            {
                theView->fSegmentDlg->fSegment = 0;
                theView->fSegmentDlg->fSide    = 0;
            }
            eventCB->setHandled();
            return;
        }
		const SoPath *dpath = picked_point->getPath();
		if(dpath == NULL) 
        {
            theView->fScreenText->string.setValue("");
            if (theView->fSegmentDlg != NULL)
            {
                theView->fSegmentDlg->fSegment = 0;
                theView->fSegmentDlg->fSide    = 0;
                theView->fSegmentDlg->UpdateData(FALSE);
            }         
            eventCB->setHandled();
            return;
        }
        // project mouse position onto z=0 plane...
        SbVec3f	ptMouse;
        SbPlaneProjector projector;
        SbPlaneProjector *myProjector = &projector;
        SoPerspectiveCamera *camera = (SoPerspectiveCamera *)theView->viewer->getCamera();
        //SoWinExaminerViewer * eViewer = (SoWinExaminerViewer *)viewer;   //->m_pComponent;
        SoWinExaminerViewer * eViewer = (SoWinExaminerViewer *)theView->viewer;   //->m_pComponent;
        SbViewportRegion myRegion = eViewer->getViewportRegion(); 
        myProjector->setOrientToEye(FALSE);  
        float aR = myRegion.getViewportAspectRatio();
        SbViewVolume vV = camera->getViewVolume(aR);
        myProjector->setViewVolume(vV);
        SbVec2f mousePosition = ev->getNormalizedPosition(myRegion);
        SbVec3f projectedPosition = myProjector->project(mousePosition);

        const SoDetail *detail = picked_point->getDetail();
		SoNode *pickedNode = dpath->getTail();
        if(pickedNode->isOfType(SoIndexedFaceSet::getClassTypeId()))
		{
			int pathLength = dpath->getLength();
			SoGroup *parent = (SoGroup *)dpath->getNode(pathLength -2);
			int current_face = ((SoFaceDetail *)detail)->getFaceIndex();			

         // make sure we're getting what we want...
         if (dpath->containsNode(theView->backgroundSep) && theView->fSearchBackground != TRUE){
            if (theView->fSegmentDlg != NULL) {
                theView->fSegmentDlg->fSegment = 0;
                theView->fSegmentDlg->fSide    = 0;
                theView->fSegmentDlg->UpdateData(FALSE);
            }
            eventCB->setHandled();
            return;
         }
         if (!dpath->containsNode(theView->backgroundSep) && theView->fSearchBackground == TRUE)
         {
            if (theView->fSegmentDlg != NULL)
            {
               theView->fSegmentDlg->fSegment = 0;
               theView->fSegmentDlg->fSide    = 0;
               theView->fSegmentDlg->UpdateData(FALSE);
            }
            eventCB->setHandled();
            return;
        }
        SoIndexedFaceSet* indexedFaceSet = (SoIndexedFaceSet *)pickedNode;
        int numFaces = indexedFaceSet->coordIndex.getNum() / 4;
        int numPathVertices;
	    int numShapeVertices = theView->sectionCoords->point.getNum();
        if (dpath->containsNode(theView->backgroundSep))
        {
            int numSegments = numFaces / 2;
            int numSegmentsPerSide = numSegments / (numShapeVertices - 1);
            numPathVertices = numSegmentsPerSide + 1;
            //  numPathVertices = (numFaces / numShapeVertices -1) / 2 + 1;
            TRACE("numSegmentsPerSide = %d\nnumShapeVertices = %d\n", numSegmentsPerSide, numShapeVertices);
        }
        else
	        numPathVertices  = theView->loftPathCoords->point.getNum();

        int currentSection;
        current_face = (current_face / 2) ;
        int side = 0;

        for (side = 0; current_face > numPathVertices -2; side++)
        {       
            current_face -= (numPathVertices - 1);
        }
        if (theView->fSegmentDlg != NULL)
        {
            theView->fSegmentDlg->fSegment = current_face;
            theView->fSegmentDlg->fSide    = side + 1;
            theView->fSegmentDlg->UpdateData(FALSE);
        }

		//	char number[20];
		//  sprintf(number, "      %d", current_face);
		//	theView->fScreenText->string.setValue(SbString(number));
		//	theView->fScreenTransform->translation.setValue(projectedPosition);
	    }
        else
        {
            theView->fScreenText->string.setValue("");
            eventCB->setHandled();
            return;
        }
    }
    eventCB->setHandled();
}

void CpMakerView::OnSearchBackground() 
{
    // CHECK ing menu items:
	CWnd* parent = GetParent();
	CMenu* mmenu = parent->GetMenu();
	CMenu* submenu = mmenu->GetSubMenu(2);

	UINT state = submenu->GetMenuState(ID_SEARCH_BACKGROUND, MF_BYCOMMAND);
    ASSERT(state != 0xFFFFFFFF);

    if (state & MF_CHECKED)
    {
        submenu->CheckMenuItem(ID_SEARCH_BACKGROUND, MF_UNCHECKED | MF_BYCOMMAND);
        fSearchBackground = FALSE;
    }
    else
    {
        submenu->CheckMenuItem(ID_SEARCH_BACKGROUND, MF_CHECKED | MF_BYCOMMAND);
        fSearchBackground = TRUE;
    }
}

void CpMakerView::OnUseSegmentDialog() 
{
   if (fSegmentDlg == NULL)
      fSegmentDlg = new CSegmentDlg(this);	
   fSegmentDlg->Create();
}

int CpMakerView::getNumSides()
{
	int numShapeCoords = sectionCoords->point.getNum();
	int numPathCoords  = loftPathCoords->point.getNum();
	return numShapeCoords - 1;   /// this varies with geometry...
}

int CpMakerView::getNumPathCoords()
{
   return loftPathCoords->point.getNum();
}
//#ifdef USE_LOCAL_EXTRUDER
void CpMakerView::getSegmentCoords(int whichSegment, SoMFVec3f& segmentCoords)
{
   int numSides = this->getNumSides();
   for (int i = 0; i < numSides; i++)
   {  
      segmentCoords.set1Value(i, SbVec3f(loftCoords->point[whichSegment * (numSides +1) + i]));
   }
}
//#endif // USE_LOCAL_EXTRUDER

void CpMakerView::OnUseMarkers() 
{
   if (fMarkerDialog == NULL)
   {
      fMarkerDialog = new MarkerDialog(this);
   }
   fMarkerDialog->Create();
}

void CpMakerView::OnOrientToReferenceLine() 
{
/////////////////////////////////////////////////////////
#ifdef ORIENT_TO_REFERENCE_CENTERLINE

/*
	BOOL    	bAddFileDialog = TRUE;
	LPCTSTR 	lpszFilter = NULL;
	LPCTSTR 	lpszDefExt = "iv";
	LPCTSTR 	lpszFileName = "*.iv";
	DWORD   	dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd *   pParentWnd = NULL;

	CFileDialog addFileDialog(bAddFileDialog,
	  lpszDefExt, lpszFileName, dwFlags,
	  lpszFilter, pParentWnd);

	addFileDialog.m_ofn.lpstrTitle = LPCTSTR("load a reference centerline...");
	int nModal = addFileDialog.DoModal();

   if (nModal != IDOK) return;
*/	
   CString m_strAddFile;
   //m_strAddFile = addFileDialog.GetFileName();
   //SetCurrentDirectory(addFileDialog.GetFolderPath());
   m_strAddFile = "C:\\3000 ArtProjects\\008 LakeCity\\teapot model\\spoutCent.iv";   /// !!! april

   ////  read in position node...
   // set up the scene graph for viewing the reference centerline

   fReferenceCenterlineCoords = new SoCoordinate3;
   fReferenceCenterlineLine   = new SoLineSet;
   backgroundSep->addChild(fReferenceCenterlineCoords);
   backgroundSep->addChild(fReferenceCenterlineLine);
   
   SoMaterial* referenceMaterial = new SoMaterial;
   backgroundSep->addChild(referenceMaterial);
   referenceMaterial->diffuseColor.setValue(.99, .99, .99);
   SoInput myInput;
   myInput.openFile(m_strAddFile);

   SoSeparator* tempNode = new SoSeparator;
   tempNode->ref();
   tempNode->addChild(SoDB::readAll(&myInput));
   myInput.closeFile();  
  
   SoCoordinate3 *coords = (SoCoordinate3 *)findChildOfType(tempNode, 
	              SoCoordinate3::getClassTypeId());
  
	for (int i = 0; i < coords->point.getNum(); i++)
	   fReferenceCenterlineCoords->point.set1Value(i, coords->point[i]);

    tempNode->removeAllChildren();
    tempNode->unref();

#endif  // ORIENT_TO_REFERENCE_CENTERLINE
/////////////////////////////////////////
                                   /////
                                    ///
}

CString CpMakerView::GetFilename(CString def_extension, CString wildcard, CString dialog_title)
{
	BOOL    bAddFileDialog = TRUE;
	LPCTSTR lpszFilter = NULL;
	LPCTSTR lpszDefExt = def_extension.GetString(); // "_prototype.iv";
	LPCTSTR lpszFileName = wildcard.GetString();    // "*_prototype.iv";
	DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd    *pParentWnd = NULL;
 
	CFileDialog addFileDialog(bAddFileDialog,
		lpszDefExt, lpszFileName, dwFlags,
		lpszFilter, pParentWnd);

	addFileDialog.m_ofn.lpstrTitle = dialog_title.GetString();
	int nModal = addFileDialog.DoModal();
	CString m_strAddFile;
	if (nModal != IDOK) return CString("");
	m_strAddFile = addFileDialog.GetFileName();
    SetCurrentDirectory(addFileDialog.GetFolderPath());
    return m_strAddFile;
}

void CpMakerView::ReadLDF(CString filename, SoCoordinate3 * _sectionCoords, 
                                            SoCoordinate3 * _loftPathCoords, 
                                            SoCoordinate3 * _hScaleCoords, 
                                            SoCoordinate3 * _vScaleCoords,
                                            SoCoordinate3 * _twistCoords)
{
    if(NULL == _sectionCoords) { _sectionCoords  = new SoCoordinate3; }
    if(NULL == _loftPathCoords){ _loftPathCoords = new SoCoordinate3; }
    if(NULL == _hScaleCoords)  { _hScaleCoords   = new SoCoordinate3; }
    if(NULL == _vScaleCoords)  { _vScaleCoords   = new SoCoordinate3; }
    if(NULL == _twistCoords)   { _twistCoords    = new SoCoordinate3; }

	SoInput myInput;
    TRACE("filename = %s\n", filename);
	myInput.openFile(filename);
    if(myInput.isValidFile())
       ;// AfxMessageBox("found file!");
    else {
        AfxMessageBox("invalid Inventor file or file not found");
        return;
    }

    SoSeparator *tempSep = new SoSeparator;
	tempSep->ref();
    tempSep->addChild(SoDB::readAll(&myInput));
	myInput.closeFile();

    SoCoordinate3 *tSectionCoords = (SoCoordinate3 *)findNodeByName(
		tempSep, "sectionCoords");
 	SoCoordinate3 *tVScaleCoords = (SoCoordinate3 *)findNodeByName(
		tempSep, "vScaleCoords");
	SoCoordinate3 *tHScaleCoords = (SoCoordinate3 *)findNodeByName(
		tempSep, "hScaleCoords");
	SoCoordinate3 *tTwistCoords = (SoCoordinate3 *)findNodeByName(
		tempSep, "twistCoords");
	SoCoordinate3* tThreeDCoords = (SoCoordinate3*)findNodeByName(
		tempSep, "threeDCoords");    

    if(tSectionCoords != NULL) 
	    _sectionCoords->copyFieldValues(tSectionCoords);
	if(tHScaleCoords != NULL)
		_hScaleCoords->copyFieldValues(tHScaleCoords);
	if(tVScaleCoords != NULL)
		_vScaleCoords->copyFieldValues(tVScaleCoords);
	if(tTwistCoords != NULL)
		_twistCoords->copyFieldValues(tTwistCoords);
	if(tThreeDCoords != NULL)
		_loftPathCoords->copyFieldValues(tThreeDCoords);

    if(tSectionCoords != NULL) { TRACE("sectionCoords\n"); }
	if(tHScaleCoords != NULL) { TRACE("hScaleCoords\n"); }
	if(tVScaleCoords != NULL) { TRACE("vScaleCoords\n"); }
	if(tTwistCoords != NULL)  { TRACE("twistCoords\n"); }
	if(tThreeDCoords != NULL) { TRACE("loftPathCoords\n"); }
    tempSep->unref();
}

float sCenterlineScale[MAX_LEVELS] = { .9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.7, 1.7, 1.7 };           // ('path scale') Austin
//float sCenterlineScale[MAX_LEVELS] = { 1.0, 1.15, 1.15, 1.3, 1.3, 1.3, 1.6, 0, 0 };        // ('path scale') Maryland
float sThickness[MAX_LEVELS] =       { .25, .25, .1875, .1875, .1875, .120, .120, .120, .120 }; // steel thicknesses at each level Austin
float sRot[MAX_LEVELS] =              { 60, 60, 60, 60, 60, 60, 60, 60, 60 };
float sInfl[MAX_LEVELS] =               { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

void CpMakerView::OnTreesTestTree() // make fractal tree...
{
    this->treeRoot->removeAllChildren();

	SoCoordinate3 * _leftPathCoords  = new SoCoordinate3;     //                 // left branch centerline coords
	SoCoordinate3 * _rightPathCoords = new SoCoordinate3;     // = NULL;         // right branch centerline coords
	SoCoordinate3 * _shapeCoords     = new SoCoordinate3;     // = NULL;         // cross-section coords
    SoCoordinate3 * _scaleCoords     = new SoCoordinate3;     // = NULL;         // 'raw' scale coords
    SoCoordinate3 * _vscaleCoords    = new SoCoordinate3;     // = NULL;
    SoCoordinate3 * _twistCoords     = new SoCoordinate3;     // = NULL;

    _leftPathCoords->ref();
    _rightPathCoords->ref();
    _shapeCoords->ref();
    _scaleCoords->ref();
    _vscaleCoords->ref();
    _twistCoords->ref();
    
    // here we're assuming that left and right prototypes have identical scale and twist
    // ...probably will want to change that...
    // Get Left prototype LDF...
    
    while(fLeftSideFile == "" || fRightSideFile == "") {
        fLeftSideFile = this->GetFilename(CString("left.ldf"), CString("*left.ldf"), CString("Load LEFT Prototype..."));
        // then Right prototype LDF
        fRightSideFile = this->GetFilename(CString("right.ldf"), CString("*right.ldf"), CString("Load RIGHT Prototype..."));
    }
    
    // check for error!!!
    ReadLDF(fLeftSideFile, _shapeCoords, /**/_leftPathCoords/**/, _scaleCoords, _vscaleCoords, _twistCoords);
    ReadLDF(fRightSideFile, _shapeCoords, /**/_rightPathCoords/**/, _scaleCoords, _vscaleCoords, _twistCoords);
    
    FractalTreeDialog ftdlg;
    ftdlg.fLeftFilename_str     = fLeftSideFile;
    ftdlg.fRightFilename_str    = fRightSideFile;
    ftdlg.fNumLevels            = fNumLevels;
    ftdlg.fRotationAngle        = fRotationAngle;
    ftdlg.fScaleFactor          = fScaleFactor;
    ftdlg.fInfluenceIntensity   = fInfluenceIntensity;
    ftdlg.fInfluenceDirection   = fInfluenceDirection; // y direction, 
    ftdlg.fCenterlineScale      = fCenterlineScale;
    ftdlg.fUseLinearEffect      = fUseLinearEffect;
    ftdlg.fUseExponentialEffect = fUseExponentialEffect;
    ftdlg.fUseSpiralEffect      = fUseSpiralEffect;
    ftdlg.fUseThickness         = false;
    ftdlg.fUseInfluence         = false;
    ftdlg.fUseRot               = true;

    for(int i = 0; i < MAX_LEVELS; i++) {
        ftdlg.fPScale[i] = sCenterlineScale[i];
        ftdlg.fThick[i]  = sThickness[i];
        ftdlg.fRot[i]    = sRot[i];
        ftdlg.fInfl[i]   = sInfl[i];
    } 
    int ret = ftdlg.DoModal();
    if (ret != IDOK) return;
    
    fNumLevels             = ftdlg.fNumLevels;
    fRotationAngle         = ftdlg.fRotationAngle;
    fScaleFactor           = ftdlg.fScaleFactor;
    fInfluenceIntensity    = ftdlg.fInfluenceIntensity;
    fInfluenceDirection    = ftdlg.fInfluenceDirection;
    fCenterlineScale       = ftdlg.fCenterlineScale;
    fRightSideFile         = ftdlg.fRightFilename_str;
    fLeftSideFile          = ftdlg.fLeftFilename_str;
    fUseLinearEffect       = ftdlg.fUseLinearEffect;
    fUseExponentialEffect  = ftdlg.fUseExponentialEffect;
    fUseSpiralEffect       = ftdlg.fUseSpiralEffect;    
    bool use_thickness     = ftdlg.fUseThickness;
    bool use_rot           = ftdlg.fUseRot;
    bool use_infl          = ftdlg.fUseInfluence;
    
    float  typical_rotation =        ftdlg.fRotationAngle * .0174532925;
    float  ABScale =                 ftdlg.fScaleFactor;
    int    numLevels =               ftdlg.fNumLevels - 1;   // !!! make this make sense...
    float  influence_intensity =     ftdlg.fInfluenceIntensity;
    float  influence_direction =     ftdlg.fInfluenceDirection * .0174532925;

    for(int i = 0; i < MAX_LEVELS; i++) {
        sCenterlineScale[i] = ftdlg.fPScale[i];
        sThickness[i] = ftdlg.fThick[i];
        sRot[i] = ftdlg.fRot[i];
        sInfl[i] = ftdlg.fInfl[i];
    } 
    
	FractalTreeMaker treeMaker;
	
    treeMaker.MakeTree( treeRoot,  _shapeCoords,          // shape or cross-section coords
                                   _leftPathCoords,        // left branch path
                                   _rightPathCoords,       // right branch path
                                   _scaleCoords,           // assume vert = horiz, 0 to 1 scaling...
                                   _twistCoords,
                                   ABScale,                // actually B:A, scale of .8 means B is smaller than A
                                   sCenterlineScale,
                                   numLevels,              // levels of fractals
                                   typical_rotation,       // from one level to the next, in radians
                                   influence_intensity,    // zero to infinity
                                   influence_direction,    // in radians from y-vector(?))
                                   fLeftSideFile,
                                   fRightSideFile,
                                   fUseLinearEffect,
                                   fUseExponentialEffect,
                                   fUseSpiralEffect,
                                   sThickness,
                                   use_thickness,
                                   sRot,
                                   use_rot,
                                   sInfl,
                                   use_infl
   );
    _leftPathCoords->unref();
    _rightPathCoords->unref();
    _shapeCoords->unref();
    _scaleCoords->unref();
    _vscaleCoords->unref();
    _twistCoords->unref();
}

// save the left, and the right extrusion, then manually join the files...
void CpMakerView::OnTreesSaveextrusionandcenterline() 
{
	BOOL    bAddFileDialog = FALSE;
	LPCTSTR lpszFilter = NULL;
	LPCTSTR lpszDefExt = "_tree.iv";
	LPCTSTR lpszFileName = "*_tree.iv";
	DWORD   dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
	CWnd    *pParentWnd = NULL;
 
	CFileDialog addFileDialog(bAddFileDialog,
        lpszDefExt, lpszFileName, dwFlags,
        lpszFilter, pParentWnd);

	addFileDialog.m_ofn.lpstrTitle = LPCTSTR("Save the Loft Object and its centerline for TREES...");
	int nModal = addFileDialog.DoModal();
	CString m_strAddFile;

	if (nModal == IDOK) {
		m_strAddFile = addFileDialog.GetFileName();
        SetCurrentDirectory(addFileDialog.GetFolderPath());
		
        SoWriteAction wa;
		wa.getOutput()->openFile(m_strAddFile);
		CWhichBranchDialog dlg;
		dlg.fIsLeft = TRUE;
		int ret = dlg.DoModal();
		if (ret != IDOK)
			return;
		SbString string;
		if(dlg.fIsLeft == TRUE)
			string = "Left";
		else
			string = "Right";
		SbString string1 = string;
		SbString string2("BranchExtrusion");
		string1 += string2;
		SbName savedName = loftRoot->getName();
		loftRoot->setName(string1);
		wa.apply(loftRoot);			  // BranchExtrusion
		loftRoot->setName(savedName);	
		savedName = threeDCoords->getName();
		SbString string3("Centerline");
		string += string3;
		threeDCoords->setName(string);
		wa.apply(threeDCoords );      // Centerline
		threeDCoords->setName(savedName);
		wa.getOutput()->closeFile();
	}	
}


void CpMakerView::OnTreesSavetreeparameters()
{
    CString params_file = this->GetFilename(CString("params.txt"), CString("*params.txt"), CString("save tree PARAMS..."));
    FILE *fp = fopen(params_file, "w");
    char info_string[300];
    fprintf(fp,"levels: %f\n", fNumLevels);
    fprintf(fp,"rotation: %f\n", fRotationAngle);
    fprintf(fp,"scaleFactor: %f\n", fScaleFactor);
    fprintf(fp,"influenceIntensity: %f\n", fInfluenceIntensity);
    fprintf(fp,"influenceDirection: %f\n", fInfluenceDirection);
    fprintf(fp,"centerlineScale: %f\n", fCenterlineScale);
    fprintf(fp,"leftSideFile: %s\n", fLeftSideFile);
    fprintf(fp,"rightSideFile: %s\n", fRightSideFile);
    fclose(fp);
}


void CpMakerView::OnChangeLoftScale()
{
    float centerline_scale;
    float section_scale;
    float thickness;

    GetScalesDialog scDialog;
    int ret = scDialog.DoModal();
    
    section_scale = scDialog.fSecScale;
    centerline_scale = scDialog.fCentScale;
    thickness = scDialog.fThickness;
    
    SbMatrix section_scale_matrix;
    section_scale_matrix.makeIdentity();
    section_scale_matrix.setScale(section_scale);

    SbMatrix centerline_scale_matrix;
    centerline_scale_matrix.makeIdentity();
    centerline_scale_matrix.setScale(centerline_scale);

    SoCoordinate3 * scaled_section_coords = new SoCoordinate3;
    SoCoordinate3 * scaled_centerline_coords = new SoCoordinate3;

    for(int i = 0; i < sectionCoords->point.getNum(); i++)  {
        SbVec3f dst;
        section_scale_matrix.multVecMatrix(sectionCoords->point[i], dst);
        scaled_section_coords->point.set1Value(i, dst);
    }

    for(int i = 0; i < threeDCoords->point.getNum(); i++)  {
        SbVec3f dst;
        SbVec3f dst2;
        section_scale_matrix.multVecMatrix(threeDCoords->point[i], dst);
        centerline_scale_matrix.multVecMatrix(dst, dst2);
        scaled_centerline_coords->point.set1Value(i, dst2);
    }
    loftRoot->removeAllChildren();
    if(((vScaleCoords->point[1] - vScaleCoords->point[0]).length() - 1.0) < 0.001) {
        loftRoot->addChild(fExtruder->extrude(scaled_section_coords, scaled_centerline_coords, hScaleCoords, hScaleCoords, this->twistCoords,  1.0, thickness, true));
    } else {
        loftRoot->addChild(fExtruder->extrude(scaled_section_coords, scaled_centerline_coords, hScaleCoords, vScaleCoords,this->twistCoords,  1.0, thickness, true));
    }
}



void CpMakerView::OnFlattenAllLevels()
{
    GetStringDialog gsDialog;
    if(gsDialog.DoModal() != IDOK) return;

    CString filename_prefix = gsDialog.fString;
    for(int i = 0; i < fNumLevels; i++)  {

        float section_scale = (pow(fABRatio,i));
        float centerline_scale = sCenterlineScale[i];
        float thickness = sThickness[i];
    
        SbMatrix section_scale_matrix;
        section_scale_matrix.makeIdentity();
        section_scale_matrix.setScale(section_scale);

        SbMatrix centerline_scale_matrix;
        centerline_scale_matrix.makeIdentity();
        centerline_scale_matrix.setScale(centerline_scale);

        SoCoordinate3 * scaled_section_coords = new SoCoordinate3;
        SoCoordinate3 * scaled_centerline_coords = new SoCoordinate3;

        for(int j = 0; j < sectionCoords->point.getNum(); j++)  {
            SbVec3f dst;
            section_scale_matrix.multVecMatrix(sectionCoords->point[j], dst);
            scaled_section_coords->point.set1Value(j, dst);
        }

        for(int j = 0; j < threeDCoords->point.getNum(); j++)  {
            SbVec3f dst;
            SbVec3f dst2;
            section_scale_matrix.multVecMatrix(threeDCoords->point[j], dst);
            centerline_scale_matrix.multVecMatrix(dst, dst2);
            scaled_centerline_coords->point.set1Value(j, dst2);
        }
        loftRoot->removeAllChildren();
       // if(((vScaleCoords->point[1] - vScaleCoords->point[0]).length() - 1.0) < 0.001) {
        //    loftRoot->addChild(fExtruder->extrude(scaled_section_coords, scaled_centerline_coords, hScaleCoords, hScaleCoords, this->twistCoords,  1.0, thickness, true));
       // } else {
           // loftRoot->addChild(fExtruder->extrude(scaled_section_coords, scaled_centerline_coords, hScaleCoords, vScaleCoords,this->twistCoords,  1.0, thickness, true, filename_prefix));
        //}
    }

}


