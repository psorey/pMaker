// pMakerView.cpp : implementation of the CpMakerView class
//

#include "stdafx.h"
#include "pMaker.h"
#include "pMakerDoc.h"
#include "pMakerView.h"
#include "Extruder.h"
#include "Flattener.h"
#include "FractalTreeDialog.h"
#include "FractalTreeMaker.h"
#include "FractalTreeSpec.h"
#include <Inspect.h>
#include <WriteDXF.h>
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
#include <Inventor/actions/SoToVRML2Action.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/manips/SoTransformBoxManip.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoFont.h>
#include <Inventor/VRMLnodes/SoVRMLGroup.h>
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
//    ON_COMMAND(ID_FLATTEN_POLYLINE, OnFlattenPolyline)
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
//    ON_COMMAND(ID_FLATTEN_TWO, OnFlattenTwoLines)
    ON_COMMAND(ID_VERT_EQ_HORIZ, OnVertEqHoriz)
    ON_COMMAND(ID_ADD_BACKGND_SOLID, OnAddBackgndSolid)
    ON_COMMAND(ID_SEARCH_BACKGROUND, OnSearchBackground)
    ON_COMMAND(ID_USE_SEGMENT_DIALOG, OnUseSegmentDialog)
    ON_COMMAND(ID_REMOVE_TRANSFORM, OnRemoveTransform)
    ON_COMMAND(ID_USE_MARKERS, OnUseMarkers)
//    ON_COMMAND(ID_EDIT_ORIENT_TO_REFERENCE_LINE, OnOrientToReferenceLine)
    ON_COMMAND(ID_TREES_TEST_TREE, OnTreesTestTree)
    //}}AFX_MSG_MAP
    // Standard printing commands
    ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

    ON_COMMAND(ID_FILE_CHANGE_LOFT_SCALE, &CpMakerView::OnChangeLoftScale)
    ON_COMMAND(ID_FLATTEN_ALL_LEVELS, &CpMakerView::OnFlattenAllLevels)
    ON_COMMAND(ID_TREES_LOADTREESPECFROMFILE, &CpMakerView::OnLoadTreeSpecFile)
    ON_COMMAND(ID_FILE_LOADMULTIPLECROSS, &CpMakerView::OnLoadMultSections)
    ON_COMMAND(ID_FILE_SAVE_AS, &CpMakerView::OnFileSaveAs)
	ON_COMMAND(ID_FILE_EXTRUDEALONGMULTIPLELINES, &CpMakerView::OnFileExtrudealongmultiplelines)
END_MESSAGE_MAP()


// #define GENERATE_CENTERLINES              // generates a centerline (output: centerlines_e.iv) for each extruded side
// #define ORIENT_TO_REFERENCE_CENTERLINE    //
// #define USE_SECOND_SCALE







/////////////////////////////////////////////////////////////////////////////


CpMakerView::CpMakerView()
{
    viewer = NULL;
    fFractalTreeSpec = NULL;
    fExtruder = NULL;
	fFlattener = NULL;
    //SoDB::init();
}

CpMakerView::~CpMakerView()
{
    if (viewer != NULL)            delete viewer;
    if (fFractalTreeSpec != NULL)  delete fFractalTreeSpec;
    if (fExtruder != NULL)         delete fExtruder;
	if (fFlattener != NULL)         delete fFlattener;

    threeDCoords->unref();
    sectionCoords->unref();
    hScaleCoords->unref();    
    vScaleCoords->unref();
    twistCoords->unref();
    if (root != NULL)               root->unref();
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
    if (viewer == NULL) {
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
    TRACE("CpMakerView::OnInitialUpdate()\n");
    CpMakerDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (viewer == NULL) {
        TRACE("first-time init\n");
        viewer = new SoWinExaminerViewer( m_hWnd );;
        viewer->setBackgroundColor(SbColor(.7, .8, .9));
        viewer->setBackgroundColor(SbColor(.1, .1, .1));
        WINDOWPLACEMENT p;
        memset(&p, 0, sizeof(p));
        p.length = sizeof(WINDOWPLACEMENT);
        p.showCmd = SW_SHOWMAXIMIZED;
        SetWindowPlacement(&p);
        fExtruder = new Extruder; // here???
		fFlattener = new Flattener; 
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
    twistCoords->point.setValue(SbVec3f(0,0,0));
    twistCoords->point.set1Value(1, SbVec3f(1,0,0));

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

    SoNormalBinding* nBinding = new SoNormalBinding;
    nBinding->value.setValue(SoNormalBinding::PER_FACE);
    root->addChild(nBinding);

    fTransformBox = new SoTransformBoxManip;
    fTransformBox->ref();

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

    /*  myEventCallback->addEventCallback(
       SoLocation2Event::getClassTypeId(),
       mouseClick, this);   */
    
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
    //  hScaleDlg = new CExaminerDialog(CString("Horizontal Scale Profile"), hScaleCoords, this, FALSE);
    hScaleDlg = new CExaminerDialog(CString("Horizontal Scale Profile"), hScaleCoords, hScaleSplineCoords, this);
    hScaleDlg->Create();
}

void CpMakerView::OnVscaleButton() 
{
    //  vScaleDlg = new CExaminerDialog(CString("Vertical Scale Profile"), vScaleCoords, this, FALSE);
    vScaleDlg = new CExaminerDialog(CString("Vertical Scale Profile"), vScaleCoords, vScaleSplineCoords, this);
    vScaleDlg->Create();
}

void CpMakerView::OnThreeDButton() 
{
    //  threeDDlg = new CExaminerDialog(CString("3d Alignment"), threeDCoords, this, FALSE);
    threeDDlg = new CExaminerDialog(CString("3d Alignment"), threeDCoords, threeDSplineCoords, this);
    threeDDlg->Create();
}

void CpMakerView::OnAddBackground() 
{
    CString filename = this->GetFilename(".iv", "*.iv", "load a background file..."); 
    if(filename != "")  {
        SoInput myInput;
        myInput.openFile(filename);
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
    CString filename = this->GetFilename(".iv", "*.iv", "load a background file..."); 
    if(filename != "") {
        SoInput myInput;
        myInput.openFile(filename);

       SoDrawStyle* style = new SoDrawStyle;
       SoMaterial*  material = new SoMaterial;
       material->setOverride(TRUE);
       style->setOverride(TRUE);
       backgroundSep->addChild(style);
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
	/*
    ReadDXF readDXF(NULL);
    readDXF.readFile();
	*/
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
	// examiner dialog updates nurbs curve polyline, then:
    this->loftRoot->removeAllChildren();
    SoSeparator * branch_sep;
    if(abs((const long)((vScaleCoords->point[1] - vScaleCoords->point[0]).length() - 1.0)) < 0.001) {
        branch_sep = fExtruder->extrude(this->sectionCoords,this->threeDCoords, this->hScaleCoords, this->hScaleCoords, this->twistCoords, false );
    } else {
        branch_sep = fExtruder->extrude(this->sectionCoords,this->threeDCoords, this->hScaleCoords, this->vScaleCoords, this->twistCoords, false );
    }
    loftRoot->addChild(branch_sep);
    TRACE("OnShowDialog()\n");   
}


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

      SoToVRML2Action tovrml2;
      tovrml2.apply(loftRoot);
      SoVRMLGroup *newroot = tovrml2.getVRML2SceneGraph();
      newroot->ref();
      SoOutput out;
      out.openFile("out.wrl");
      out.setHeaderString("#VRML V2.0 utf8");
      SoWriteAction wra(&out);
      wra.apply(newroot);
      out.closeFile();
      newroot->unref();
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
    
    loftRoot->removeAllChildren();
    if((vScaleCoords->point[0] - vScaleCoords->point[vScaleCoords->point.getNum() -1]).length() - 1.0 < 0.001) {
        loftRoot->addChild(fExtruder->extrude(sectionCoords, threeDCoords, hScaleCoords, hScaleCoords, twistCoords, false));
    } else {
        loftRoot->addChild(fExtruder->extrude(sectionCoords, threeDCoords, hScaleCoords, vScaleCoords, twistCoords, false));
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
    fUpperLimit = fExtruder->fLoftPathCoords->point.getNum();
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
	CString filename = GetFilename(".iv", "*.iv", "load a zig-zag coordinate set to flatten...");
	if (strcmp(filename, "") == 0) return;

    SoSeparator* tempNode = new SoSeparator;
    tempNode->ref();
    SoCoordinate3* inputCoords = new SoCoordinate3;
    inputCoords->point.deleteValues(0,-1);
    SoInput myInput;
    myInput.openFile(filename);
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

    CString dxfFilename = filename.SpanExcluding(".");
    dxfFilename += "_flat.dxf";

    FILE * fp = fopen(dxfFilename, "w");
    if(NULL == fp){
        TRACE("could not open file %s\n", dxfFilename);
        return;
    }
	WriteDXF * write_dxf = new WriteDXF(fp);



//    fprintf(fp,"  0\nSECTION\n  2\nENTITIES\n");

    int count = 0;
    //newCoords->point.deleteValues(0, -1);
    flatCoords->point.deleteValues(0, -1);

    fFlattener->flatten(inputCoords, flatCoords);  
	write_dxf->WriteCoords(flatCoords);
	/*
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

	*/
    fclose(fp);

//	tempSep->unref();
    flatCoords->unref();
//	newCoords->unref();
}


/*
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
    for (int i = 0; i < coords->point.getNum(); i++) {
        inputCoords1->point.set1Value(i, coords->point[i]);
    }
    tempNode->removeAllChildren();

// load the second line...
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
    for (int i = 0; i < coords->point.getNum(); i++) {
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

*/
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
            numPathVertices  = theView->fExtruder->fLoftPathCoords->point.getNum();

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
    if (state & MF_CHECKED) {
        submenu->CheckMenuItem(ID_SEARCH_BACKGROUND, MF_UNCHECKED | MF_BYCOMMAND);
        fSearchBackground = FALSE;
    }else {
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
    int numPathCoords  = fExtruder->fLoftPathCoords->point.getNum();
    return numShapeCoords - 1;   /// this varies with geometry...
}

int CpMakerView::getNumPathCoords()
{
   return fExtruder->fLoftPathCoords->point.getNum();
}
/*
//#ifdef USE_LOCAL_EXTRUDER
void CpMakerView::getSegmentCoords(int whichSegment, SoMFVec3f& segmentCoords)
{
   int numSides = this->getNumSides();
   for (int i = 0; i < numSides; i++) {  
      segmentCoords.set1Value(i, SbVec3f(loftCoords->point[whichSegment * (numSides +1) + i]));
   }
}
//#endif // USE_LOCAL_EXTRUDER
*/


void CpMakerView::OnUseMarkers() 
{
   if (fMarkerDialog == NULL) {
      fMarkerDialog = new MarkerDialog(this);
   }
   fMarkerDialog->Create();
}
/*
void CpMakerView::OnOrientToReferenceLine() 
{
/////////////////////////////////////////////////////////
#ifdef ORIENT_TO_REFERENCE_CENTERLINE

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
}
*/
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

int CpMakerView::ReadCoordinate3(CString filename, SoCoordinate3 *coords)  
{
    int return_value = 0; // not successful...
    if(NULL == coords) { coords  = new SoCoordinate3; }
    SoInput myInput;
    myInput.openFile(filename);
    if(myInput.isValidFile())
       ; // AfxMessageBox("found file!");
    else {
        AfxMessageBox("invalid Inventor file or file not found");
        return return_value;
    }
    SoSeparator *tempSep = new SoSeparator;
    tempSep->ref();
    tempSep->addChild(SoDB::readAll(&myInput));
    myInput.closeFile();
    SoCoordinate3 * t_coords = (SoCoordinate3 *)CpMakerView::findChildOfType(tempSep, SoCoordinate3::getClassTypeId());    
    if(t_coords != NULL) {
        coords->copyFieldValues(t_coords); 
        return_value = 1; // success
    }
    tempSep->unref();
    return return_value;
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
    if(tHScaleCoords  != NULL) { TRACE("hScaleCoords\n");  }
    if(tVScaleCoords  != NULL) { TRACE("vScaleCoords\n");  }
    if(tTwistCoords   != NULL) { TRACE("twistCoords\n");   }
    if(tThreeDCoords  != NULL) { TRACE("loftPathCoords\n");  }
    
    tempSep->unref();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//   global values for setting default TREE settings...
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

int      numLevels              = 7;
float    leftABRatio            = .74;
float    rightABRatio           = .74;
float    influenceIntensity     = 0.0;
float    influenceDirection     = 180.0; // y direction, 
CString  leftFilename           = "C:\\3000 ArtProjects\\048 Austin\\tree\\test_left.ldf";
CString  rightFilename          = "C:\\3000 ArtProjects\\048 Austin\\tree\\test_right.ldf";
BOOL     useCenterlineScale     = TRUE;
BOOL     useThickness           = FALSE;
BOOL     useInfluence           = TRUE; 
BOOL     useRotation            = TRUE;
BOOL     useSpiralEffect        = FALSE;
BOOL	 useLinearEffect        = TRUE;
BOOL     useExponentialEffect   = FALSE;
int      useInfluenceType = FractalTreeSpec::LINEAR;

float    leftCenterlineScale[MAX_LEVELS]  = { .9,  1.0,  1.1,   1.2,   1.3,   1.4,  1.7, 1.7, 1.7   };    // ('path scale') Austin
float    rightCenterlineScale[MAX_LEVELS] = { .9,  1.0,  1.1,   1.2,   1.3,   1.4,  1.7, 1.7, 1.7   };    // ('path scale') Austin
float    leftThickness[MAX_LEVELS]        = { .25, .25, .1875, .1875, .1875, .120, .120, .120, .120 };    // steel thicknesses at each level Austin
float    rightThickness[MAX_LEVELS]       = { .25, .25, .1875, .1875, .1875, .120, .120, .120, .120 };    // steel thicknesses at each level Austin
float    leftRotation[MAX_LEVELS]         = { 60,   60,   60,    60,    60,   60,   60,   60,   60  };
float    rightRotation[MAX_LEVELS]        = { 60,   60,   60,    60,    60,   60,   60,   60,   60  };
float    leftInfluence[MAX_LEVELS]        = { 1,     1,    1,     1,     1,    1,    1,    1,    1  };
float    rightInfluence[MAX_LEVELS]       = { 1,     1,    1,     1,     1,    1,    1,    1,    1  };


FractalTreeSpec * saved_tree_spec = NULL;  // global!! we use this to retain values between tree generations

void CpMakerView::OnLoadTreeSpecFile()
{
    CString filename = GetFilename(".fts", "*.fts", "Load a Fractal Tree Spec...");
	if (strcmp(filename, "") == 0) return;
    CFile theFile;
    theFile.Open(_T(filename), CFile::modeRead );
    CArchive archive(&theFile, CArchive::load);
    saved_tree_spec->Serialize(archive);
    archive.Close();
    theFile.Close();
}

void CpMakerView::OnTreesTestTree() // menu: TREES/MAKE TREES    make fractal tree...
{
    this->treeRoot->removeAllChildren();

    if(fFractalTreeSpec == NULL)  
        fFractalTreeSpec = new FractalTreeSpec();

    if(saved_tree_spec == NULL) {
        saved_tree_spec = new FractalTreeSpec();
        this->OnLoadTreeSpecFile();
		//TRACE("loaded tree spec file\n");
    }
    
    // else we already have a saved_tree_spec...
    fFractalTreeSpec->copyValues(saved_tree_spec);

    FractalTreeDialog ftdlg;

//	TRACE("saved_tree_spec: value = %f\n",saved_tree_spec->fLCentScale[0]);
//	TRACE("fFractalTreeSpec: value = %f\n",fFractalTreeSpec->fLCentScale[0]);
    ftdlg.fRightFilename        = fFractalTreeSpec->fRightFilename;
    ftdlg.fNumLevels            = fFractalTreeSpec->fNumLevels;
    ftdlg.fLeftABRatio          = fFractalTreeSpec->fLeftABRatio;
    ftdlg.fRightABRatio         = fFractalTreeSpec->fRightABRatio;
    ftdlg.fUseCenterlineScale   = fFractalTreeSpec->fUseCenterlineScale;
    ftdlg.fUseThickness         = fFractalTreeSpec->fUseThickness;
    ftdlg.fUseInfluence         = fFractalTreeSpec->fUseInfluence; 
    ftdlg.fUseRot               = fFractalTreeSpec->fUseRotation;
    ftdlg.fInfluenceDirection   = fFractalTreeSpec->fInfluenceDirection;
    ftdlg.fUseSpiralEffect      = fFractalTreeSpec->fUseSpiralEffect;
    ftdlg.fUseLinearEffect      = fFractalTreeSpec->fUseLinearEffect;
    ftdlg.fUseExponentialEffect = fFractalTreeSpec->fUseExponentialEffect;
    ftdlg.fFractalTreeSpec      = fFractalTreeSpec;

	//TRACE("PMakerView::before changes: %f\n", fFractalTreeSpec->fLCentScale[0]);
	for(int i = 0; i < 9; i++) {
        ftdlg.fLCentScale[i]    = fFractalTreeSpec->fLCentScale[i];
        ftdlg.fRCentScale[i]    = fFractalTreeSpec->fRCentScale[i];
        ftdlg.fLThick[i]        = fFractalTreeSpec->fLThick[i];
        ftdlg.fRThick[i]        = fFractalTreeSpec->fRThick[i];
        ftdlg.fLRot[i]          = fFractalTreeSpec->fLRot[i];
        ftdlg.fRRot[i]          = fFractalTreeSpec->fRRot[i];
        ftdlg.fLInfl[i]         = fFractalTreeSpec->fLInfl[i];
        ftdlg.fRInfl[i]         = fFractalTreeSpec->fRInfl[i];
    }


    ftdlg.fLeftFilename         = fFractalTreeSpec->fLeftFilename;
    ftdlg.fRightFilename        = fFractalTreeSpec->fRightFilename;
    ftdlg.fNumLevels            = fFractalTreeSpec->fNumLevels;
    ftdlg.fLeftABRatio          = fFractalTreeSpec->fLeftABRatio;
    ftdlg.fRightABRatio         = fFractalTreeSpec->fRightABRatio;
    ftdlg.fUseCenterlineScale   = fFractalTreeSpec->fUseCenterlineScale;
    ftdlg.fUseThickness         = fFractalTreeSpec->fUseThickness;
    ftdlg.fUseInfluence         = fFractalTreeSpec->fUseInfluence; 
    ftdlg.fUseRot               = fFractalTreeSpec->fUseRotation;
    ftdlg.fInfluenceDirection   = fFractalTreeSpec->fInfluenceDirection;
    ftdlg.fUseSpiralEffect      = fFractalTreeSpec->fUseSpiralEffect;
    ftdlg.fUseLinearEffect      = fFractalTreeSpec->fUseLinearEffect;
    ftdlg.fUseExponentialEffect = fFractalTreeSpec->fUseExponentialEffect;
    ftdlg.fFractalTreeSpec      = fFractalTreeSpec;

	//TRACE("PMakerView::after changes: %f\n", fFractalTreeSpec->fLCentScale[0]);
	for(int i = 0; i < 9; i++) {
        ftdlg.fLCentScale[i]    = fFractalTreeSpec->fLCentScale[i];
        ftdlg.fRCentScale[i]    = fFractalTreeSpec->fRCentScale[i];
        ftdlg.fLThick[i]        = fFractalTreeSpec->fLThick[i];
        ftdlg.fRThick[i]        = fFractalTreeSpec->fRThick[i];
        ftdlg.fLRot[i]          = fFractalTreeSpec->fLRot[i];
        ftdlg.fRRot[i]          = fFractalTreeSpec->fRRot[i];
        ftdlg.fLInfl[i]         = fFractalTreeSpec->fLInfl[i];
        ftdlg.fRInfl[i]         = fFractalTreeSpec->fRInfl[i];
    }

    int ret = ftdlg.DoModal();
    if (ret == IDOK) {
        fFractalTreeSpec->fLeftFilename         = ftdlg.fLeftFilename;
        fFractalTreeSpec->fRightFilename        = ftdlg.fRightFilename;
        fFractalTreeSpec->fNumLevels            = ftdlg.fNumLevels;
        fFractalTreeSpec->fLeftABRatio          = ftdlg.fLeftABRatio;
        fFractalTreeSpec->fRightABRatio         = ftdlg.fRightABRatio;
        fFractalTreeSpec->fUseCenterlineScale   = ftdlg.fUseCenterlineScale;
        fFractalTreeSpec->fUseThickness         = ftdlg.fUseThickness;
        fFractalTreeSpec->fUseInfluence         = ftdlg.fUseInfluence; 
        fFractalTreeSpec->fUseRotation          = ftdlg.fUseRot;
        fFractalTreeSpec->fInfluenceDirection   = ftdlg.fInfluenceDirection;
        fFractalTreeSpec->fUseSpiralEffect      = ftdlg.fUseSpiralEffect;
        fFractalTreeSpec->fUseLinearEffect      = ftdlg.fUseLinearEffect;
        fFractalTreeSpec->fUseExponentialEffect = ftdlg.fUseExponentialEffect;

        for(int i = 0; i < 9; i++) {
            fFractalTreeSpec->fLCentScale[i]    = ftdlg.fLCentScale[i];
            fFractalTreeSpec->fRCentScale[i]    = ftdlg.fRCentScale[i];
            fFractalTreeSpec->fLThick[i]        = ftdlg.fLThick[i]; 
            fFractalTreeSpec->fRThick[i]        = ftdlg.fRThick[i];
            fFractalTreeSpec->fLRot[i]          = ftdlg.fLRot[i];
            fFractalTreeSpec->fRRot[i]          = ftdlg.fRRot[i];
            fFractalTreeSpec->fLInfl[i]         = ftdlg.fLInfl[i];
            fFractalTreeSpec->fRInfl[i]         = ftdlg.fRInfl[i];
        } 
		//TRACE("after changes: %f\n", fFractalTreeSpec->fLCentScale[0]);
        // read all the SoCoordinat3 sets from file...
        // currently shapeCoords are same for left and right!
        ReadLDF(fFractalTreeSpec->fLeftFilename, 
                fFractalTreeSpec->fShapeCoords, 
                fFractalTreeSpec->fLeftCentCoords, 
                fFractalTreeSpec->fLeftHScaleCoords, 
                fFractalTreeSpec->fLeftVScaleCoords,  
                fFractalTreeSpec->fLeftTwistCoords);

        ReadLDF(fFractalTreeSpec->fRightFilename, 
                fFractalTreeSpec->fShapeCoords, 
                fFractalTreeSpec->fRightCentCoords, 
                fFractalTreeSpec->fRightHScaleCoords, 
                fFractalTreeSpec->fRightVScaleCoords,  
                fFractalTreeSpec->fRightTwistCoords);
        
        saved_tree_spec->copyValues(fFractalTreeSpec);
		//TRACE("saved_tree_spec should reflect new %f\n", saved_tree_spec->fLCentScale[0]);
		//fFractalTreeSpec->listSpecs("c:\\my_specs.txt");
        // generate the tree...
        FractalTreeMaker treeMaker(treeRoot, fFractalTreeSpec);
        treeMaker.MakeTree( treeRoot, fFractalTreeSpec);
    }
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
        loftRoot->addChild(fExtruder->extrude_fractal(scaled_section_coords, scaled_centerline_coords, hScaleCoords, hScaleCoords, this->twistCoords,  1.0, thickness, true));
    } else {
        loftRoot->addChild(fExtruder->extrude_fractal(scaled_section_coords, scaled_centerline_coords, hScaleCoords, vScaleCoords,this->twistCoords,  1.0, thickness, true));
    }
}


void CpMakerView::OnFlattenAllLevels()
{

}


void CpMakerView::OnLoadMultSections()
{
    SoCoordinate3* section_1_coords = new SoCoordinate3;
    SoCoordinate3* section_2_coords = new SoCoordinate3;
    section_1_coords->ref();
    section_2_coords->ref();

    CString section_1_filename = this->GetFilename(".iv", "*.iv", "First Section");
    if(0 == this->ReadCoordinate3(section_1_filename, section_1_coords)) {
        AfxMessageBox("invalid Inventor file or file not found"); 
        section_1_coords->unref();
        section_2_coords->unref();
        return;
    }
    CString section_2_filename = this->GetFilename(".iv", "*.iv", "Second Section");
    if(0 == this->ReadCoordinate3(section_2_filename, section_2_coords)) {
        section_1_coords->unref();
        section_2_coords->unref();
        AfxMessageBox("invalid Inventor file or file not found");  
        return;
    }
    CGetIntegerDialog int_dialog;
    int num_sides = section_2_coords->point.getNum() -1;
    int_dialog.fInteger1 = num_sides;
    int_dialog.fInteger2 = 0;
    int ret = int_dialog.DoModal();
    if(ret != IDOK) {
        section_1_coords->unref();
        section_2_coords->unref();
        return;
    }
    num_sides = int_dialog.fInteger1;

    Extruder     * my_extruder = new Extruder();
    SoSeparator  * extrusion = my_extruder->extrude_using_multiple_sections(section_1_coords, section_2_coords, num_sides);
    loftRoot->addChild(extrusion);
    section_1_coords->unref();
    section_2_coords->unref();
    delete my_extruder;
}


void CpMakerView::OnFileSaveAs()
{
    CString filename = this->GetFilename(".fts", "*.fts", "Save the Fractal Tree Spec...");
    CFile theFile;
    theFile.Open(_T(filename), CFile::modeCreate | CFile::modeWrite);
    CArchive archive(&theFile, CArchive::store);
    fFractalTreeSpec->Serialize(archive);
    archive.Close();
    theFile.Close();
}



void CpMakerView::OnFlatten() //
{
    fExtruder->extrude(sectionCoords, threeDCoords, hScaleCoords, vScaleCoords, twistCoords, true ); 
    return;
}




void CpMakerView::OnFileExtrudealongmultiplelines()
{
	// TODO: Add your command handler code here
}
