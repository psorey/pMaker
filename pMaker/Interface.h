#include <Inventor/Win/viewers/SoWinPlaneViewer.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/SbViewportRegion.h>

#ifndef __INTERFACE_
#define __INTERFACE_

class CProadView;
class CProadDoc;
class CIvfComponent;
class CurveManip;

class Interface{

private:
    SoWinViewer         *viewer;
    SoCamera            *camera;
    //CIvfComponent       *component;
    SbViewportRegion    vpRegion;
    CView               *theView;
    CFrameWnd           *theFrame;
    float                 V_SCALE;
    float                 H_SCALE;

public:
    Interface(CFrameWnd *parentFrame);

    void                 setViewer(SoWinViewer *);
    SoWinViewer* getViewer();
    void                 setCamera(SoCamera*);
    SoCamera*            getCamera();         
    SbViewportRegion     getViewportRegion();
    void                 sendMessage(int whichAlignment, int whichCurve);
    SbViewVolume         getViewVolume();
    //CIvfComponent*       getComponent();
    void                 setView(CView *);
    CView*               getView();
    SbVec2s              getWindowSize();
    void                 setWindowScale(float hscale, float vscale);
    SbVec2f              getWindowScale();
};

#endif