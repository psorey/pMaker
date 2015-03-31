#include "stdafx.h"
#include "Interface.h"


/*
    Put this in OnActivateView() in the ViewClass...
      
    CFrameWnd *parentFrame = GetParentFrame();
    parentFrame->SetWindowText("Horizontal Alignment");
    
    theInterface->setView(this);
    theInterface->setCamera(IvfGetActiveCamera());
    theInterface->setViewer((SoWinExaminerViewer *)m_pComponent);
    theInterface->setWindowScale(H_SCALE, V_SCALE);

*/


Interface::Interface(CFrameWnd *frmWnd) 
{
    theFrame  = frmWnd;
    H_SCALE = V_SCALE = 1;
    //component = theComponent;
    //doc      = view->GetDocument();
    //viewer   = (SoWinExaminerViewer *)component;
    //camera   = (SoPerspectiveCamera *) component->IvfGetActiveCamera();
    //vpRegion = viewer->getViewportRegion();

}

void
Interface::setViewer(SoWinViewer *viewr)
{
    viewer = viewr;
}

SoWinViewer *
Interface::getViewer()
{
    return viewer;
}

void
Interface::setCamera(SoCamera *aCamera)
{
    camera = aCamera;
}

SoCamera*  
Interface::getCamera()
{
    camera = (SoCamera *)viewer->getCamera();
    return camera;
}

SbViewportRegion    
Interface::getViewportRegion() 
{
    return viewer->getViewportRegion(); 
}

SbViewVolume
Interface::getViewVolume()
{
    SbViewVolume vV = camera->getViewVolume();
    return vV;
}



void
Interface::setView(CView * view)
{
    theView = view;
}


CView *
Interface::getView(void)
{
    return theView;
}

void
Interface::sendMessage(int whichAlignment, int whichCurve)
{
    CPoint point;
    point.x = whichAlignment;
    point.y = whichCurve;
	theView->PostMessage(WM_LBUTTONDBLCLK, whichAlignment, whichCurve);
}

SbVec2s
Interface::getWindowSize()
{
    CRect tempRect;
    theFrame->GetClientRect(tempRect);
    int width  = tempRect.Width();
    int height = tempRect.Height();
    return SbVec2s(width, height);
}

void
Interface::setWindowScale(float hscale, float vscale)
{
    V_SCALE = vscale;
    H_SCALE = hscale;
}

SbVec2f
Interface::getWindowScale(void)
{
    SbVec2f temp = SbVec2f(H_SCALE,V_SCALE);
    return temp;
}   