#pragma once

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/SbPList.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoNurbsCurve.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoFont.h>

#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoSwitch.h>

#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/details/SoDetail.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/SoPath.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoComplexity.h>
#include "grid.h"

//#include "SbVec3fUtils.h"


//////////////////////////////////////////////////////////////////
//
//  class Spline  
//
///////////////////////////////////////////////////////////////////


class Spline {
public:

    SoDrawStyle             *splineStyle;
    SoSeparator             *parentSep;
    SoSeparator             *splineSep;
    SoVertexProperty        *splineVP; 
    SoComplexity            *complexity;
    SoNurbsCurve            *splineCurve;
    int                     framesPerKey;
    SbVec3f                 graphLocation;
    SoPointSet              *splinePoints;
    SoSeparator             *pointsSep;
    SoPickStyle             *pointsPick;

                            Spline(char *name, SoSeparator *parent, SbVec3f graphLocation);
    void                    createFromVertexProperty(SoVertexProperty *inputVP);
    void                    addSplinePoint(SbVec3f coord);
    void                    insertSplinePoint(void);
    void                    deleteSplinePoint(int whichPoint);
    void                    updateSpline(void);
    void                    getAllFramePositions(void);
};



//////////////////////////////////////////////////////////////////
//
//  class SplineManip  
//
///////////////////////////////////////////////////////////////////

class Interface;
class SplineManip {

public:
    enum   grabbed_thing {
		NOTHING,
		SPLINE_POINT,  
    };

    enum  edit_type {
		NONE,
		ADD_SPLINE_POINT,
		DELETE_SPLINE_POINT,
    };

    static  Interface           *theInterface;
    static  SoTransform         *screenTrans;
    static  Grid                *theGrid;

    static  SoEventCallback     *myEventCallback;
    static  SoGroup             *parentNode;
    static  SoSeparator         *alignmentNode;
    static  SoSeparator         *manipRoot;
    static  SoSeparator         *splineParent;
    static  SoMaterial          *current_material, *saved_material; // for highlighting
    //static SoCoordinate3      *currentCoordinate3;
    static  SoVertexProperty    *currentVP;
    static  SplineManip         *currentManip;
    static  Spline              *currentSpline;
    static  SbPList             *pAlignmentList;
    static  int                 grabbed;
    static  int                 mouse_down;
    static  SbVec2f             oldMousePosition;
    static  int                 current_curve;      // which curve in the current alignment
    static  int                 current_coord;      // which coordinate is current
    static  int                 alignment_count;    // how many alignments are in use
    static  int                 editType;
    static  int                 current_alignment;   // which alignment is currently selected
            Spline*             spline;
            int                 which_alignment;
            char                name[50];
            SoSeparator         *alignmentSubNode;
            SoSeparator         *backgroundSep;
            SoPointSet          *backgroundPoints;
            SoLineSet           *backgroundLines;
            SoVertexProperty    *backgroundVP;
            SoDrawStyle         *backgroundStyle;


                                SplineManip(SoGroup *parent, Interface *anInterface,const char *name);
            Spline*             getSpline();
            int                 containsNode(SoNode *testNode);
            SbVec3f             graphLocation;
            void                setUpSpline(const SoMFVec3f&);
    static  SoPath *            findPathToNode(SoNode *fromNode, SoNode *node);
    static  SoNode *            findChildOfType(SoGroup *parent, SoType nodeType);
    static  SoNode *            findNodeByName(SoGroup *parent, char *);
            void                writeToFile(const char *filename);
            void                setComplexity(float complex);
    static  void                setEditType(int type);
    static  void                setButtonDown(void *, SoEventCallback *eventCB);
    static  void                setButtonUp(void *,SoEventCallback *eventCB);
    static  void                mouseMoved(void *,SoEventCallback *eventCB);
    static  void                sendMessage(int whichAlignment, int whichSplinePoint);
};

