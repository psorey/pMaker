// pMakerView.h : interface of the CpMakerView class
//
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include "resource.h"
#pragma once

class FractalTreeMaker;
class FractalTreeSpec;
class CpMakerDoc;
class CIvDialogDlg;
class CExaminerDialog;
class SoCoordinate3;
class SoIndexedTriangleStripSet;
class SoMaterial;
class SoSeparator;
class SoType;
class SbVec3f;	
class SoSensor;
class CGetIntegerDialog;
class SoTransform;
class SoTransformBoxManip;
class SbRotation;
class SoFaceSet;
class SoLineSet;
class SoIndexedFaceSet;
class SoText2;
class SoFont;
class CSegmentDlg;
class MarkerDialog;
class SoMFVec3f;
class SoEventCallback;
class Extruder;
class Flattener;



class CpMakerView : public CView
{
protected: // create from serialization only
	CpMakerView();
	DECLARE_DYNCREATE(CpMakerView)

// Attributes
public:
	CpMakerDoc*             GetDocument() const;
    CpMakerView*            pMakerView;
	BOOL                    fAntiSquish;
    SoWinExaminerViewer     *viewer;
    Extruder                *fExtruder;
	Flattener               *fFlattener;

	CExaminerDialog*	    threeDDlg;
	CExaminerDialog*	    vertDlg;
	CExaminerDialog*	    horizDlg;
	CExaminerDialog*	    hScaleDlg;
	CExaminerDialog*	    vScaleDlg;
	CExaminerDialog*	    twistDlg;
	CExaminerDialog*	    sectionDlg;
	CGetIntegerDialog*	    fUpperLowerDialog;
	int                     fUpperLimit;
	int		                fLowerLimit;	    // limit of face-creation along length of extrusion...
	int		                fNumRotatedCopies;
	float	                fOffset;		    // offset the surface from "normal"
	bool	                fClosedShape;
	bool	                fIsEqualScale;      // vScale = hScale if TRUE

	// holes can be added while flattening...
	float	                fHoleOffset;	    // distance to center of hole from edge
	float	                fHoleSpacing;	    // hole-to-hole spacing
    SoCoordinate3*          midlineCoords;  // test for generating midline lines 3/05 !!!
    SoLineSet*              midlineLineSet;
    SoSeparator*            fMarkerTopSeparator;
    MarkerDialog*           fMarkerDialog;
    SoLineSet*              fReferenceCenterlineLine;
    SoCoordinate3*          fReferenceCenterlineCoords;
    SoCoordinate3*          threeDCoords;
	SoCoordinate3*          vScaleCoords;
	SoCoordinate3*          hScaleCoords;
	SoCoordinate3*          twistCoords;
	SoCoordinate3*          sectionCoords;  // change to shapeCoords
	SoCoordinate3*          threeDSplineCoords;
	SoCoordinate3*          vertSplineCoords;
	SoCoordinate3*          horizSplineCoords;
	SoCoordinate3*          vScaleSplineCoords;
	SoCoordinate3*          hScaleSplineCoords;
	SoCoordinate3*          twistSplineCoords;
	SoCoordinate3*          sectionSplineCoords;
	
	// clip plane nodes...(for cutting sections of loft object)
	SoSeparator*			fClipPlaneSep;
	SoCoordinate3*			fClipPlaneCoords;
	SoFaceSet*				fClipPlaneFaces;
	//SoTransform*			fClipPlaneTransform;
	SoTransformBoxManip*	fClipPlaneManip;
	SoCoordinate3*			fClipIntersectionCoords;
	SoLineSet*				fClipIntersectionLines;
	SoSeparator*			root;
	SoSeparator*			fMultipleSep;
	SoTransform*			fLoftTransform;
	SoTransformBoxManip*	fTransformBox;
	SoSeparator*			loftRoot;
    SoSeparator*            treeRoot;
//	SoCoordinate3*			loftCoords;         /// not using these three
//	SoIndexedFaceSet*		loftFaces;          ///
//	SoMaterial*				loftMaterial;       ///
//	SoCoordinate3*			loftPathCoords; 
//	SoCoordinate3*			loftScaleCoords;    /// combined vertCoords and horizCoords;
//  SoCoordinate3*          loftScale2Coords;   /// for 2nd level scaling
//  SoCoordinate3*          loftScale2Center;

	SoSeparator*			backgroundSep;
    bool                    fSearchBackground;
    SoTransform*            fScreenTransform;
    SoText2*                fScreenText;
    SoFont*                 fScreenFont;
    SoSeparator*            fScreenTextSep;
    CSegmentDlg*            fSegmentDlg;

    // for remembering values between trees, and to pass to FractalTreeMaker
    FractalTreeSpec *       fFractalTreeSpec;

// Operations
public:
//    static	void		    sensorCB(void *data, SoSensor *sensor);
    static  void            mouseMoved(void *,SoEventCallback *eventCB);
    static	SoNode*		    findChildOfType(SoGroup * parent, SoType typeId);
			SoNode*		    findNodeByName(SoGroup *parent, char *name);
			SoPath*		    FindPathToNode(SoGroup* parent, SoNode* node);
	static	float           dist(SbVec3f pt1, SbVec3f pt2);
//	static	SbRotation	    interpolatePRY(SbVec3f pt1, SbVec3f pt2, SbVec3f pt3);
//	static	SbRotation	    interpolatePRY2(SbVec3f pt1, SbVec3f pt2, SbVec3f pt3);
//	static	SbRotation	    interpolatePRY3(SbVec3f pt1, SbVec3f pt2, SbVec3f pt3);
//	static	SbRotation	    getPRY(SbVec3f pt1, SbVec3f pt2);
//            SbRotation      interpolatePRY2_Align_with_Reference_Centerline(int whichCenterlinePoint);  
//            float		    interpolateZ(float);
//	        SbVec3f		    interpolateScale(float);
//            SbVec3f         interpolateScale2(float); // for USE_SECOND_SCALE
//			void		    make3dPath();
//			void		    makeLoftObject();				
//			void		    flatten(SoCoordinate3* tdCoords, SoCoordinate3* flatCoords);
//	static	double		    hypotenuse( double xside, double yside );
			void		    setUpperLower(int upper, int lower) { fUpperLimit = upper; fLowerLimit = lower; }
//	static	double		    GetTheta(SbVec3f pt1, SbVec3f pt2);
//	static  double          GetTheta(double ptAx, double ptAy, double ptBx, double ptBy);
//	static  double          GetTheta(double sideX, double sideY);
			SbVec3f		    GetIntersection(SbVec3f ptA1, SbVec3f ptA2,SbVec3f ptB1, SbVec3f ptB2);
			SbVec3f		    CalcVectorPoint(SbVec3f pt, float length, float theta);
//	static  double          GetVectorAngle(double a1, double a2, double a3, double b1, double b2, double b3);
//            void            getSegmentCoords(int whichSegment, SoMFVec3f& segmentCoords);
            int             getNumPathCoords();
            int             getNumSides();   // used in marker dialog...
            CString         GetFilename(CString def_ext, CString wildcard, CString dialogTitle);
            void            ReadLDF(CString filename, SoCoordinate3 * _sectionCoords, // read loft's definitions from .ldf file
                                                      SoCoordinate3 * _loftPathCoords, 
                                                      SoCoordinate3 * _hscaleCoords, 
                                                      SoCoordinate3 * _vscaleCoords,  // currently assuming vscale == scale !!!
                                                      SoCoordinate3 * _twistCoords);
            int            ReadCoordinate3(CString filename, SoCoordinate3 *coords);
            bool           isUsingVScale(void);
// for testing:
            void           trace_SoCoordinate3(char * label, SoCoordinate3 * coords);

// Overrides
public:
	virtual void OnInitialUpdate();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CpMakerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


protected:
	//{{AFX_MSG(CpMakerView)
	afx_msg void OnDestroy();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnViewViewmodesSelectionmode();
	afx_msg void OnViewViewmodesViewingmode();
	afx_msg void OnXSecButton();
	afx_msg void OnTwistButton();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnAddBackground();
	afx_msg void OnDeleteBackground();
	afx_msg void OnFileDxftoiv();
	afx_msg void OnHscaleButton();
	afx_msg void OnVscaleButton();
	afx_msg void OnSaveLoft();
	afx_msg void OnShowDialog();
	afx_msg void OnRemoveLoft();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnThreeDButton();
	afx_msg void OnSaveConstructionData();
	afx_msg void OnLoadConstructionData();
	afx_msg void OnFlatten();
//	afx_msg void OnFlattenPolyline();
	afx_msg void OnEditBeginEnd();
	afx_msg void OnEditMultiples();
	afx_msg void OnEditTransformBoxOn();
	afx_msg void OnEditTransformBoxOff();
	afx_msg void OnFileSaveMultiiples();
	afx_msg void OnEditOffset();
	afx_msg void OnEditAddHoles();
	afx_msg void OnEditClipPlane();
	afx_msg void OnEditClipPlaneOff();
	afx_msg void OnAntisquish();
	afx_msg void OnFlattenImportedCoords();
//	afx_msg void OnFlattenTwoLines();
	afx_msg void OnVertEqHoriz();
	afx_msg void OnAddBackgndSolid();
	afx_msg void OnSearchBackground();
	afx_msg void OnUseSegmentDialog();
	afx_msg void OnRemoveTransform();
	afx_msg void OnUseMarkers();
//	afx_msg void OnOrientToReferenceLine();
	afx_msg void OnTreesTestTree();

public:
    afx_msg void OnChangeLoftScale();
    afx_msg void OnFlattenAllLevels();
    afx_msg void OnLoadTreeSpecFile();
    afx_msg void OnLoadMultSections();
    afx_msg void OnFileSaveAs();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileExtrudealongmultiplelines();
};

#ifndef _DEBUG  // debug version in pMakerView.cpp
inline CpMakerDoc* CpMakerView::GetDocument() const
   { return reinterpret_cast<CpMakerDoc*>(m_pDocument); }
#endif

