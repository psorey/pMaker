#pragma once
//
// A very specialized node; this manipulator completely takes care of
// editing a set of line segments.
//

#include <Inventor/SbLinear.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/fields/SoFieldData.h>
#include <Inventor/nodes/SoSubNode.h>


class SbColor;
class SbPlaneProjector;
class SbLineProjector;
class SbVec3f;
class SoCoordinate3;
class SoDrawStyle;
class SoHandleEventAction;
class SoLineSet;
class SoLocation2Event;
class SoMaterial;
class SoPath;
class SoPointSet;
class SoSwitch;
class SoTransform;
class SoMaterial;

class LineManip2 : public SoSeparator
{
    //SO_NODE_HEADER(LineManip2);

  public:
    LineManip2();
    virtual ~LineManip2();

    //
    // Return the coordinates being edited.  Put a data sensor on
    // these to get notified whenever they are changed.
    //
    SoCoordinate3 *getCoordinate3();
    void setCoordinate3( SoCoordinate3 *newNode );

  //
  // THIS MUST BE CALLED BEFORE THE CLASS IS USED!
  // (SoInteraction::init() calls this for all of the built-in
  // manipulator)
  //
    static void initClass();

    //
    // Get rid of any hilights.  This can be called when the mouse
    // leaves the window, for example.
    //
    virtual void removeHilights();

    //
    // Handle an event.  This is meant to be internal only.
    //
    virtual void handleEvent(SoHandleEventAction *);

    //
    // Controls how big the feedback is.  Default is
    // 0.05.
    //
    virtual void setHilightSize(float);

    // Sets the normal to the projection plane.
    void setPlaneNormal( const SbVec3f &newNormal );

  SoEXTENDER public:

    // Records the viewport size and viewVolume at the time when the
    // manipulator grabs the events.
    // Automatically called by the handleEventAction when this manip 
    // grabs events with ha->setGrabber(this)
    // Also turns renderCaching off during the time we are the grabber.
    virtual void grabEventsSetup();
    virtual void grabEventsCleanup();

//#ifdef __C_PLUS_PLUS_2
//    enum LineManipPart {
//#else
    enum Part {
//#endif
	POINTS,
	LINES,
	ENDPOINTS,
	NOTHING,
    };

  private:
    SoCoordinate3 *coord, *hilightCoord;
    int current_coord;
    float hilightSize;
    SbVec3f current_position;
    SoPointSet *pset;
    SoDrawStyle *drawStyle;
    SoLineSet *lset;
    SbPlaneProjector *planeProj;
    SbLineProjector  *lineProj;

	 SoMaterial *lineMaterial;

    SoTransform *hilightTransform;
    SoMaterial *hilightMaterial;
    SoSwitch *hilightSwitch;

//#ifdef __C_PLUS_PLUS_2
//    LineManipPart whichPart();
//#else
    Part whichPart();
//#endif
    SbBool dragStart();
    SbBool remove();
    void updateProjectors( const SbVec3f &curPt );
    void projectMouse(SbVec3f &);
    
    SbBool locateHilight();
    void moveCoord();
    void initHilightStuff();
    void hilightVertex(const SbVec3f &, const SbColor &);
    void hilightLine(int, const SbVec3f &, const SbColor &);

    SbVec3f planeNormal;

    static SbColor locateColor;
    static SbColor pickColor;


    CacheEnabled savedRenderCachingVal;
    SbViewVolume        myViewVolume;
    SbViewportRegion    myVpRegion;
    SoHandleEventAction *myHandleEventAction;
    void extractViewingParams( SoHandleEventAction *);
};
