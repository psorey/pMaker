#include "stdafx.h"
#include "SplineManip.h"
#include "Interface.h"
#include "grid.h"


//////////////////////////////////////////////////////////////////
//
//  class SplineManip  
//
///////////////////////////////////////////////////////////////////



//////// static data members /////////////////////////

Interface       *SplineManip::theInterface;
SoTransform     *SplineManip::screenTrans;
Grid            *SplineManip::theGrid;
SoEventCallback *SplineManip::myEventCallback;
SoGroup         *SplineManip::parentNode;
SoSeparator     *SplineManip::alignmentNode;
SoSeparator     *SplineManip::manipRoot;
SoMaterial      *SplineManip::saved_material; // for highlighting
SoMaterial      *SplineManip::current_material; // for highlighting
//SoCoordinate3  *SplineManip::currentCoordinate3;
SoVertexProperty *SplineManip::currentVP;
//Curve           *SplineManip::currentCurve;
SplineManip      *SplineManip::currentManip;
SbPList         *SplineManip::pAlignmentList;
int             SplineManip::grabbed;
int             SplineManip::current_alignment;
SbVec2f         SplineManip::oldMousePosition;
int             SplineManip::mouse_down;
int             SplineManip::current_curve;      // which curve in the current alignment
int             SplineManip::current_coord;      // which coordinate is current
int             SplineManip::alignment_count;    // how many alignments are in use
int             SplineManip::editType;
Spline         *SplineManip::currentSpline;

/////////////////////////////////////////////////////////

SplineManip::SplineManip( SoGroup *parent, 
                        Interface *anInterface,
                        const char *theName)
{
    strcpy(name, theName);
    ///// test
   // V_SCALE = 1;
   // H_SCALE = 1;
        /////
    if(!parentNode) { // no SplineManips yet
        TRACE("SplineManip first time\n");
        theInterface        = anInterface;
        TRACE("Interface@curve = %d\n", theInterface);
        alignment_count     = 1;
        grabbed             = NOTHING;
        editType            = NONE;
        mouse_down          = 0;
        which_alignment     = alignment_count-1;
        currentManip        = NULL;
        //currentCurve        = NULL;
        currentVP           = NULL;
        saved_material = new SoMaterial;
        parentNode = parent;
        pAlignmentList = new SbPList;
        pAlignmentList->append(this);

        // create the unique alignment node...
        oldMousePosition = SbVec2f(0,0);
        alignmentNode = new SoSeparator;
        alignmentNode->ref();
        screenTrans = new SoTransform;
        alignmentNode->addChild(screenTrans);
        theGrid = new Grid(alignmentNode, theInterface);
        SoPickStyle *alignmentPick = new SoPickStyle;
        alignmentPick->style.setValue(SoPickStyle::UNPICKABLE);
        parent->addChild(alignmentNode);
        alignmentNode->addChild(alignmentPick);
        myEventCallback = new SoEventCallback;
        alignmentNode->addChild(myEventCallback);
        alignmentSubNode = new SoSeparator;
        
        alignmentNode->addChild(alignmentSubNode);
        splineX = new Spline(name, alignmentSubNode, SbVec3f(0,0,0)); // beginSta
        splineY = new Spline(name, alignmentSubNode, SbVec3f(0,0,0)); // beginSta
        splineZ = new Spline(name, alignmentSubNode, SbVec3f(0,0,0)); // beginSta
        
        myEventCallback->addEventCallback(
            SoMouseButtonEvent::getClassTypeId(), 
            setButtonDown, NULL);

        myEventCallback->addEventCallback(
            SoMouseButtonEvent::getClassTypeId(), 
            setButtonUp, NULL);

        myEventCallback->addEventCallback(
            SoLocation2Event::getClassTypeId(),
            mouseMoved, NULL);
    }
    else {
        alignmentSubNode = new SoSeparator;
        //alignmentSubNode->setName(name);
        alignmentNode->addChild(alignmentSubNode);
        splineX = new Spline(name, alignmentSubNode, SbVec3f(0,0,0)); // beginSta
        splineY = new Spline(name, alignmentSubNode, SbVec3f(0,0,0)); // beginSta
        splineZ = new Spline(name, alignmentSubNode, SbVec3f(0,0,0)); // beginSta
        SplineManip::alignment_count++;
        which_alignment = SplineManip::alignment_count -1;
        pAlignmentList->append(this);
    }
    backgroundSep       = new SoSeparator;
    //backgroundPick      = new SoPickStyle;
    backgroundStyle     = new SoDrawStyle;
    backgroundLines     = new SoLineSet;
    backgroundPoints    = new SoPointSet;
    backgroundVP        = new SoVertexProperty;
    backgroundVP->materialBinding.setValue(SoVertexProperty::PER_FACE);
    backgroundSep->ref();
    backgroundVP->orderedRGBA.setValue(0x888888ff);
    backgroundStyle->pointSize.setValue(5);
    backgroundSep->addChild(backgroundStyle);
    //backgroundSep->addChild(backgroundPick);
    backgroundSep->addChild(backgroundVP);
    backgroundSep->addChild(backgroundLines);
    //backgroundSep->addChild(backgroundPoints);
    alignmentSubNode->addChild(backgroundSep);
    //currentVP = spline->splineVP;
}

void
SplineManip::setButtonDown (void * whichSplineManip , SoEventCallback *eventCB)
{
   const SoEvent *ev = eventCB->getEvent();

   if(SO_MOUSE_PRESS_EVENT(ev, BUTTON2)) {
       mouse_down = 2;
   }

   if(SO_MOUSE_PRESS_EVENT(ev, BUTTON1)) {
       grabbed = NOTHING;
        SplineManip *manip; 
        mouse_down = 1;
	    const SoPickedPoint *picked_point;   
        //TRACE("curveManip name = %s\n", manip->name);

	    SbVec3f	ptMouse;
        SbPlaneProjector projector;
        SbPlaneProjector *myProjector = &projector;
        
	    SoPerspectiveCamera *camera = 
            (SoPerspectiveCamera *)theInterface->getCamera();
        SoWinExaminerViewer *viewer = (SoWinExaminerViewer *)
            theInterface->getViewer();
        //viewer->setBackgroundColor(SbColor(.3, .4, .5));
	    SbViewportRegion myRegion = theInterface->getViewportRegion(); 
	    myProjector->setOrientToEye(FALSE);  
        float aR = myRegion.getViewportAspectRatio();
        SbViewVolume vV = camera->getViewVolume(aR);
        myProjector->setViewVolume(vV);
	    
        SbVec2f mousePosition = ev->getNormalizedPosition(myRegion);
	    SbVec3f projectedPosition = myProjector->project(mousePosition);
        
        SbVec2f scale = theInterface->getWindowScale();
        float H_SCALE = scale[0];
        float V_SCALE = scale[1];
        
        projectedPosition[0] /= H_SCALE;
        projectedPosition[1] /= V_SCALE;
        projectedPosition[2] = 0.0;
        
        if ((picked_point = eventCB->getPickedPoint()) == NULL)  { 
            // nothing was picked...
	        grabbed = NOTHING;

            if(editType == ADD_SPLINE_POINT && currentSpline != NULL) {
                //Spline *aspline = currentManip->getSpline();
                //aspline->addSplinePoint(projectedPosition); 
                currentSpline->addSplinePoint(projectedPosition);
            }                        
            TRACE("grabbed NOTHING\n");
            return;
        }
        // SOMETHING was picked...
	    
	    const SoPath *dpath = picked_point->getPath();
	    const SoDetail *detail = picked_point->getDetail();
        int length = dpath->getLength();
        SoNode *pickedParent = dpath->getNode(length-3);
        currentVP = (SoVertexProperty *)findNodeByName((SoGroup *)pickedParent,
            "splineVP");

        if(currentVP == NULL) TRACE("null vp\n");
        SoNode *pickedNode = dpath->getTail();
        if(pickedNode->isOfType(SoPointSet::getClassTypeId())) {
            current_coord =
		        ((SoPointDetail *)detail)->getCoordinateIndex();			
            TRACE("current_coord = %d\n", current_coord);
            grabbed = SPLINE_POINT;
        }
        
        if (dpath->containsNode(alignmentNode)) {
            TRACE("contains alignmentNode\n");
            SoPath *tempPath;
            // which part of the Alignment was picked...
            tempPath = findPathToNode(alignmentNode, 
                pickedNode);
            
            SoNode *tempNode = tempPath->getNode(1);
            if(!tempNode->isOfType(SoSeparator::getClassTypeId())) {
                TRACE("SplineManip::setButtonDown:not an SoSeparator node\n");
                exit(0);
            }

            // which alignment was picked (which CurveMgr)...
            SoSeparator *tempSep = (SoSeparator *)tempPath->getNode(1);
            const char *tempName;
            SbName tempSbName = tempSep->getName();
            tempName = tempSbName.getString();

            // get a pointer to the current CurveManager for convenience...
            int i;
            int num = pAlignmentList->getLength();
            for(i=0; i<num; i++) {
                manip = (SplineManip *)((*(const SbPList *)pAlignmentList)[i]);
                if(dpath->containsNode(manip->alignmentSubNode)){
                    i=num;
                    current_alignment = manip->which_alignment;
                    currentManip = manip;
                    TRACE("current alignment = %d\n", current_alignment);
                }
            }
            for(i=0; i<3; i++) {
                if(dpath->containsNode(currentManip->splineX->splinePoints)) {
                    currentSpline = currentManip->splineX;
                }
                else if(dpath->containsNode(currentManip->splineY->splinePoints)) {
                    currentSpline = currentManip->splineY;
                }
                else if(dpath->containsNode(currentManip->splineZ->splinePoints)) {
                    currentSpline = currentManip->splineZ;
                }
            }

            TRACE("name = %s\n", manip->name);
            //currentManip = manip;
            //Spline *tSpline = manip->spline;
            if(editType == DELETE_SPLINE_POINT && currentSpline != NULL) {
                currentSpline->deleteSplinePoint(current_coord);
                currentSpline->updateSpline();
            }
        
            // find out if it's a Curve element or a CurveMgr element...
            SoNode *tempCurveNode = tempPath->getNode(3);
        }
    }
    eventCB->setHandled();
}


void
SplineManip::setButtonUp(void *whichSplineManip, SoEventCallback *eventCB)
{
    const SoEvent *ev = eventCB->getEvent();
    if(SO_MOUSE_RELEASE_EVENT(ev, BUTTON2)) {
        mouse_down = 0;
    }
    if(SO_MOUSE_RELEASE_EVENT(ev, BUTTON1)) {
	    mouse_down = 0;
        if(grabbed == NOTHING) {
            ;        
        }
        else {
            grabbed = NOTHING;
            //current_material->copyFieldValues(saved_material);
        }
    }
    eventCB->setHandled();
}

void
SplineManip::mouseMoved(void *whichSplineManip, SoEventCallback *eventCB)
{
    const SoEvent *ev = eventCB->getEvent();

	SplineManip *manip = currentManip;
    if (ev->isOfType(SoLocation2Event::getClassTypeId()) && mouse_down == 2 &&
          ev->wasShiftDown()) {
        
   	    SbVec3f	ptMouse;
        SbPlaneProjector projector;
        SbPlaneProjector *myProjector = &projector;
        
	    SoPerspectiveCamera *camera = 
            (SoPerspectiveCamera *)theInterface->getCamera();
        SoWinExaminerViewer *viewer = (SoWinExaminerViewer *)
            theInterface->getViewer();
        //viewer->setBackgroundColor(SbColor(.3, .4, .5));
	    SbViewportRegion myRegion = theInterface->getViewportRegion(); 
	    myProjector->setOrientToEye(FALSE);  
        float aR = myRegion.getViewportAspectRatio();
        SbViewVolume vV = camera->getViewVolume(aR);
        myProjector->setViewVolume(vV);
	    
        SbVec2f mousePosition = ev->getNormalizedPosition(myRegion);
	    SbVec3f projectedPosition = myProjector->project(mousePosition);

        if(oldMousePosition[1] == 0.0) {
            oldMousePosition = mousePosition;
            eventCB->setHandled();
            return;
        }

        SbVec2f winscale = SplineManip::theInterface->getWindowScale();
        float hscale = winscale[0];
        
        // getting bigger or smaller?...
        
        // TRACE("mouse = %f    old = %f\n", mousePosition[1], oldMousePosition[1]);
        if(mousePosition[1]>oldMousePosition[1]) hscale *= .9;
        else hscale *= 1.1;
        oldMousePosition = mousePosition;
        SplineManip::theInterface->setWindowScale(hscale, winscale[1]);
        SplineManip::screenTrans->scaleFactor.setValue(hscale, winscale[1], 1);
    }
    else if (ev->isOfType(SoLocation2Event::getClassTypeId()) && mouse_down == 1
          && grabbed != NOTHING ) {
        //TRACE("mouse moved\n");

	    SbVec3f	ptMouse;
        SbPlaneProjector projector;
        SbPlaneProjector *myProjector = &projector;
        
	    SoPerspectiveCamera *camera = 
            (SoPerspectiveCamera *)theInterface->getCamera();
        SoWinExaminerViewer *viewer = (SoWinExaminerViewer *)
            theInterface->getViewer();
        //viewer->setBackgroundColor(SbColor(.3, .4, .5));
	    SbViewportRegion myRegion = theInterface->getViewportRegion(); 
	    myProjector->setOrientToEye(FALSE);  
        float aR = myRegion.getViewportAspectRatio();
        SbViewVolume vV = camera->getViewVolume(aR);
        myProjector->setViewVolume(vV);
	    
        SbVec2f mousePosition = ev->getNormalizedPosition(myRegion);
	    SbVec3f projectedPosition = myProjector->project(mousePosition);
        
        SbVec2f scale = theInterface->getWindowScale();
        float H_SCALE = scale[0];
        float V_SCALE = scale[1];
        
        projectedPosition[0] /= H_SCALE;
        projectedPosition[1] /= V_SCALE;
        projectedPosition[2] = 0.0;
        if(grabbed == SPLINE_POINT) {
            currentVP->vertex.set1Value(current_coord, projectedPosition);
        }
    }
    eventCB->setHandled();
}

//CurveMgr*
//SplineManip::createNewAlignment(char *name, float beginSta)
//{
//    curveMgr[alignment_count] = new CurveMgr(name, alignmentNode, beginSta);
//    vcurveMgr[alignment_count] = new CurveMgr(name, valignmentNode,beginSta, 1);
//    alignment_count++;
//    return curveMgr[alignment_count - 1];
//}


//line*
//SplineManip::getSpline(void)
//{
   //eturn spline;
//}

/*
void            
SplineManip::setVertScale(float scale) {
    V_SCALE = scale;
    //for(int i = 0; i<curveMgr->num_curves; i++) {
        //SbVec3f temp = curveMgr->getCurve(i)->textTrans->scaleFactor.getValue();
        //temp[1] = 1/scale;
       // curveMgr->getCurve(i)->textTrans->scaleFactor.setValue(temp);  
    //}
}

void            
SplineManip::setHorizScale(float scale) {
    H_SCALE = scale;
    for(int i = 0; i<curveMgr->num_curves; i++) {
        SbVec3f temp = curveMgr->getCurve(i)->textTrans->scaleFactor.getValue();
        temp[0] = 1/scale;
        curveMgr->getCurve(i)->textTrans->scaleFactor.setValue(temp);  
    }
}
*/

SoPath *
SplineManip::findPathToNode(SoNode *fromNode, SoNode *node){
    
    SoSearchAction search;
    SoPath *path;
    
    search.setNode(node);
    search.setInterest(SoSearchAction::FIRST);
    search.apply(fromNode);
    
    path = search.getPath();
    if(path != NULL)
	path->ref();
    return path;
}

SoNode *
SplineManip::findChildOfType(SoGroup * parent, SoType typeId)
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

SoNode *
SplineManip::findNodeByName(SoGroup *parent, char *name)
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

void
SplineManip::setEditType(int type) 
{
    editType = type;
}

void
SplineManip::setUpSpline(const SoMFVec3f &coords)
{

   /*
   int vertexCount = 0;
   backgroundVP->vertex.deleteValues(0,-1);
    int num = coords.getNum();
    TRACE("num = %d\n", num);
    int i;
    for(i=0; i<num; i++) {
        backgroundVP->vertex.set1Value(i, SbVec3f(i, coords[i][0], 0.0));
        backgroundVP->orderedRGBA.set1Value(i,0xcc6600ff);
    }
    backgroundLines->numVertices.set1Value(0,num);
    
    int xinc = (num)/5;

    splineX->splineVP->vertex.deleteValues(0,-1);
    splineX->splineVP->orderedRGBA.setValue(0xffff00ff);
    splineX->splineCurve->numControlPoints.setValue(0);
    splineX->splineCurve->knotVector.deleteValues(0,-1);
    splineX->addSplinePoint(SbVec3f(  0, coords[0][0], 0));
    splineX->addSplinePoint(SbVec3f(  xinc, coords[xinc][0], 0));
    splineX->addSplinePoint(SbVec3f(2*xinc, coords[2*xinc][0], 0));
    splineX->addSplinePoint(SbVec3f(3*xinc, coords[3*xinc][0], 0));
    splineX->addSplinePoint(SbVec3f(4*xinc, coords[4*xinc][0], 0));
    splineX->addSplinePoint(SbVec3f(5*xinc, coords[num-1][0], 0));

    for(i=0; i<num; i++) {
        backgroundVP->vertex.set1Value(i+num, SbVec3f(i,coords[i][1], 0.0));
        backgroundVP->orderedRGBA.set1Value(i+num,0x66cc00ff);
    }
    backgroundLines->numVertices.set1Value(1,num);
    splineY->splineVP->vertex.deleteValues(0,-1);
    splineY->splineVP->orderedRGBA.setValue(0xffff00ff);
    splineY->splineCurve->numControlPoints.setValue(0);
    splineY->splineCurve->knotVector.deleteValues(0,-1);
    splineY->addSplinePoint(SbVec3f(  0, coords[0][1], 0));
    splineY->addSplinePoint(SbVec3f(  xinc, coords[xinc][1], 0));
    splineY->addSplinePoint(SbVec3f(2*xinc, coords[2*xinc][1], 0));
    splineY->addSplinePoint(SbVec3f(3*xinc, coords[3*xinc][1], 0));
    splineY->addSplinePoint(SbVec3f(4*xinc, coords[4*xinc][1], 0));
    splineY->addSplinePoint(SbVec3f(5*xinc, coords[num-1][1], 0));

    for(i=0; i<num; i++) {
        backgroundVP->vertex.set1Value(i+2*num, SbVec3f(i,coords[i][2], 0.0));
        backgroundVP->orderedRGBA.set1Value(i+2*num,0x6600ccff);
    }
    backgroundLines->numVertices.set1Value(2,num);
    splineZ->splineVP->vertex.deleteValues(0,-1);
    splineZ->splineVP->orderedRGBA.setValue(0xff00ffff);
    splineZ->splineCurve->numControlPoints.setValue(0);
    splineZ->splineCurve->knotVector.deleteValues(0,-1);
    splineZ->addSplinePoint(SbVec3f(  0, coords[0][2], 0));
    splineZ->addSplinePoint(SbVec3f(  xinc, coords[xinc][2], 0));
    splineZ->addSplinePoint(SbVec3f(2*xinc, coords[2*xinc][2], 0));
    splineZ->addSplinePoint(SbVec3f(3*xinc, coords[3*xinc][2], 0));
    splineZ->addSplinePoint(SbVec3f(4*xinc, coords[4*xinc][2], 0));
    splineZ->addSplinePoint(SbVec3f(5*xinc, coords[num-1][2], 0));


    SoWriteAction wa;
    wa.getOutput()->openFile("test.txt");
    wa.apply(alignmentNode); 
    wa.getOutput()->closeFile();    
    */

   int vertexCount = 0;
   backgroundVP->vertex.deleteValues(0,-1);
    int num = coords.getNum();
    TRACE("num = %d\n", num);
    int i;
    splineX->splineVP->vertex.deleteValues(0,-1);
    splineX->splineVP->orderedRGBA.setValue(0xffff00ff);
    splineX->splineCurve->numControlPoints.setValue(0);
    splineX->splineCurve->knotVector.deleteValues(0,-1);
    for(i=0; i<num; i++) {
        backgroundVP->vertex.set1Value(i, SbVec3f(i, coords[i][0], 0.0));
        splineX->addSplinePoint(SbVec3f(  i, coords[i][0], 0));
    }
    backgroundVP->orderedRGBA.set1Value(0,0xcc6600ff);
    backgroundLines->numVertices.set1Value(0,num);
    
    splineY->splineVP->vertex.deleteValues(0,-1);
    splineY->splineVP->orderedRGBA.setValue(0xffff00ff);
    splineY->splineCurve->numControlPoints.setValue(0);
    splineY->splineCurve->knotVector.deleteValues(0,-1);
    for(i=0; i<num; i++) {
        backgroundVP->vertex.set1Value(i+num, SbVec3f(i,coords[i][1], 0.0));
        splineY->addSplinePoint(SbVec3f(i, coords[i][1], 0));
    }
    backgroundVP->orderedRGBA.set1Value(1,0x66cc00ff);
    backgroundLines->numVertices.set1Value(1,num);

    splineZ->splineVP->vertex.deleteValues(0,-1);
    splineZ->splineVP->orderedRGBA.setValue(0xff00ffff);
    splineZ->splineCurve->numControlPoints.setValue(0);
    splineZ->splineCurve->knotVector.deleteValues(0,-1);
    for(i=0; i<num; i++) {
        backgroundVP->vertex.set1Value(i+2*num, SbVec3f(i,coords[i][2], 0.0));
        splineZ->addSplinePoint(SbVec3f(i, coords[i][2], 0));
    }
    backgroundVP->orderedRGBA.set1Value(2,0x6600ccff);
    backgroundLines->numVertices.set1Value(2,num);

    SoWriteAction wa;
    wa.getOutput()->openFile("test.txt");
    wa.apply(alignmentNode); 
    wa.getOutput()->closeFile();    
}

void
SplineManip::writeToFile(const char *filename) 
{
    SoCoordinate3 * tempCoords = new SoCoordinate3;
    tempCoords->ref();
    
    int num = splineX->splineVP->vertex.getNum();
    int i;
    for(i=0; i<num; i++) {
        tempCoords->point.set1Value(i, SbVec3f(
            splineX->splineVP->vertex[i][1],
            splineY->splineVP->vertex[i][1],
            splineZ->splineVP->vertex[i][1]));
    }
    SoWriteAction wa;
    wa.getOutput()->openFile(filename);
    wa.apply(tempCoords); 
    wa.getOutput()->closeFile();    
    tempCoords->unref();
}

void
SplineManip::setComplexity(float complex) {
    splineX->complexity->value.setValue(complex);
    splineY->complexity->value.setValue(complex);
    splineZ->complexity->value.setValue(complex);
}


//////////////////////////////////////////////////////////////////
//
//  class Spline  
//
///////////////////////////////////////////////////////////////////



Spline::Spline(char *name, SoSeparator *parent, SbVec3f _graphLocation) 
{    
    graphLocation = _graphLocation;

    splineStyle  = new SoDrawStyle;
    parentSep       = parent;
    splineSep     = new SoSeparator;
    complexity = new SoComplexity;
    splineSep->addChild(complexity);
    splineVP     = new SoVertexProperty;
    splineVP->setName(SbName("splineVP"));
    //splineVP->materialBinding.setValue(SoVertexProperty::PER_PART);
    splineCurve       = new SoNurbsCurve;
    pointsSep         = new SoSeparator;
    splinePoints      = new SoPointSet;
    pointsPick        = new SoPickStyle;
    pointsPick->style.setValue(SoPickStyle::SHAPE);
    pointsSep->addChild(pointsPick);
    pointsSep->addChild(splinePoints);
    splineStyle->pointSize.setValue(5);
    parent->addChild(splineSep);
    splineSep->addChild(splineStyle);
    splineSep->addChild(splineVP);
    splineSep->addChild(splineCurve);
    splineSep->addChild(pointsSep);

    splineVP->orderedRGBA.setValue(0xffaa33ff);
    splineCurve->knotVector.set1Value(0,0);
    splineCurve->knotVector.set1Value(1,0);
    splineCurve->knotVector.set1Value(2,0);
    splineCurve->knotVector.set1Value(3,0);
    splineCurve->knotVector.set1Value(4,1);
    splineCurve->knotVector.set1Value(5,1);
    splineCurve->knotVector.set1Value(6,1);
    splineCurve->knotVector.set1Value(7,1);
}

void
Spline::deleteSplinePoint(int whichPoint) 
{
    splineVP->vertex.deleteValues(whichPoint, 1); 
}

void
Spline::addSplinePoint(SbVec3f splinePointCoords)
{
    // add the coordinate to VertexProperty->vertex...
    int numCoords = splineVP->vertex.getNum();
    int i;
    for(i=0; i<numCoords; i++) {
        if(splineVP->vertex[i][0] > splinePointCoords[0]) {
            splineVP->vertex.insertSpace(i,1);
            splineVP->vertex.set1Value(i,splinePointCoords);
            i=numCoords+5;  // increment past num
        }
    }
    if(i == numCoords) {    // must be the biggest x-value...
        splineVP->vertex.set1Value(numCoords, splinePointCoords);
    }

    numCoords++;
    // update the nurbs curve fields...
    if(numCoords > 5) {
        int i;
        splineCurve->knotVector.setValue(0);
        splineCurve->knotVector.set1Value(0,0);
        splineCurve->knotVector.set1Value(1,0);
        splineCurve->knotVector.set1Value(2,0);
        splineCurve->knotVector.set1Value(3,0);
        for(i=1; i<=numCoords - 3; i++) {
            //TRACE("i = %d\n", i);
            splineCurve->knotVector.set1Value(i+3, i);
        }
        splineCurve->knotVector.set1Value(i+3,i-1);
        splineCurve->knotVector.set1Value(i+4,i-1);
        splineCurve->knotVector.set1Value(i+5,i-1);
        splineCurve->numControlPoints.setValue(numCoords);
    }
}

void
Spline::updateSpline(void) 
{
    int numCoords = splineVP->vertex.getNum();

    if(numCoords > 5) {
        int i;
        splineCurve->knotVector.setValue(0);
        splineCurve->knotVector.set1Value(0,0);
        splineCurve->knotVector.set1Value(1,0);
        splineCurve->knotVector.set1Value(2,0);
        splineCurve->knotVector.set1Value(3,0);
        for(i=1; i<=numCoords - 3; i++) {
            //TRACE("i = %d\n", i);
            splineCurve->knotVector.set1Value(i+3, i);
        }
        splineCurve->knotVector.set1Value(i+3,i-1);
        splineCurve->knotVector.set1Value(i+4,i-1);
        splineCurve->knotVector.set1Value(i+5,i-1);
        splineCurve->numControlPoints.setValue(numCoords);
    }
}

void
Spline::createFromVertexProperty(SoVertexProperty *inputVP) 
{
    int numCoords = inputVP->vertex.getNum();
    int i;
    // copy the vertex field...
    for(i=0; i<numCoords; i++) {
        splineVP->vertex.set1Value(i, inputVP->vertex[i]);
        addSplinePoint(inputVP->vertex[i]);
        TRACE("made it here\n");
    }
}

/*
Coordinate3 *
Spline::getAllFramePositions() 
{   
    SoCallbackAction ca;  
//    coords->point.deleteValues(0, -1);

    ca.addLineSegmentCallback(SoNurbsSpline::getClassTypeId(), 
            Spline::LineSegCB, NULL);
    //pDoc->complex->value = .01;
    ca.apply(splineEditSep);
    //pDoc->complex->value = .5;
   //TRACE("exited OnInfo\n");
    SoCoordinate3 *lineCoords = new SoCoordinate3;
    SoLineSet *lineSet = new SoLineSet;
    SoSeparator *lineSep = new SoSeparator;
    lineCoords->copyFieldValues(coords);
    lineSep->addChild(lineCoords);
    lineSep->addChild(lineSet);
    int num = lineCoords->point.getNum();
    int j=0;
    for(int i = 0; i<num; i+=2){
        lineSet->numVertices.set1Value(j++,2);
    }
    pDoc->eNode->addChild(lineSep);

}

void
Spline::LineSegCB(void *, SoCallbackAction *action, const SoPrimitiveVertex *v1,
          const SoPrimitiveVertex *v2)
{
    // fills coords with points at desired distances
    SbVec3f point = v1->getPoint();
    if(dist(current_point, point) > current_desired_distance) {
        current_point = point;
        TRACE("%f\t%f\t%f\n", point[0], point[1], point[2]);
        int count = coords->point.getNum();
        coords->point.set1Value(count,point);
        //coords->point.set1Value(count+1, point[0], point[1], point[2]-10);
    }
}
*/
/*
void
Spline::getAllFramePositions(void) 
// see drive/ScaleWindow.c++
// makes a .cam style posCoords and rotCoords description of the splineEdit...
{   
    int numPositions = splineCurveVP->vertex.getNum();
    int i;
    posCoords->point.deleteValues(0,-1);
    for(i=0; i<numPositions-1; i++) {
        // write position...
        SplinePoint *tempSplinePoint = getSplinePoint(i);
        posCoords->point.set1Value(i,tempSplinePoint->splinePointVP->vertex[0]);
        // write rotation...
        float angle1;
        float x1;
        float y1;
        float x2;
        float y2;
        SbVec3f pointAtPoint = tempSplinePoint->splinePointVP->vertex[1];
        SbVec3f vantagePoint = tempSplinePoint->splinePointVP->vertex[0];
        x1 = pointAtPoint[0];
        y1 = pointAtPoint[1];
        x2 = vantagePoint[0];
        y2 = vantagePoint[1];
        if (( x2 - x1 ) == 0 ) {
	        angle1 = 0;
        }
        else { 
	        angle1 =  atan2 (( y2 - y1 ), ( x2 - x1 ));  
        }
        if (angle1 < 0) {
                angle1 += (2 * PI);     
        }
        //rotCoords->point.set1Value(i, 0.0,  .885, angle1);    
   }

    SoWriteAction wa;
    wa.getOutput()->openFile("newCamera.pos");
    wa.apply(posCoords); 
    wa.getOutput()->closeFile();    
   
    wa.getOutput()->openFile("newCamera.rot");
    //wa.apply(rotCoords); 
    wa.getOutput()->closeFile();    
}


  */
