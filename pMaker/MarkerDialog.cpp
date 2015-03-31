// MarkerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "pMaker.h"
#include "pMakerView.h"
#include "MarkerDialog.h"

#include <Inventor/SbLinear.h>

#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoLineSet.h>
#include <Inventor/nodes/SoNurbsCurve.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoOrthographicCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/nodes/SoPointSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoText2.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/manips/SoTransformBoxManip.h>
#include <Inventor/manips/SoHandleBoxManip.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/details/SoDetail.h>
#include <Inventor/details/SoFaceDetail.h>
#include <Inventor/details/SoPointDetail.h>
#include <Inventor/details/SoLineDetail.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoViewportRegionElement.h>
#include <Inventor/events/SoKeyboardEvent.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/events/SoLocation2Event.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/projectors/SbPlaneProjector.h>
#include <Inventor/projectors/SbLineProjector.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MarkerDialog dialog


MarkerDialog::MarkerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(MarkerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(MarkerDialog)
	fSegment = 0;
	fSize = 0;
	//}}AFX_DATA_INIT
}

MarkerDialog::MarkerDialog(CView *pview)
{
   fSegment = 1;
   fSize    = 1;
   fView    = (CpMakerView *)pview;
}

MarkerDialog::~MarkerDialog(void)
{
   TRACE("delete\n");
   for(int i = 0; i < fNumMarkers; i++)
   {
      fMarkerSeparator[i]->removeAllChildren();
      fMarkerSeparator[i]->unref();
   }
   fView->fMarkerDialog = NULL;
}


BOOL
MarkerDialog::Create()
{
    return CDialog::Create(MarkerDialog::IDD);
}

void MarkerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MarkerDialog)
	DDX_Text(pDX, IDC_SEGMENT, fSegment);
	DDX_Text(pDX, IDC_MARKER_SIZE, fSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MarkerDialog, CDialog)
	//{{AFX_MSG_MAP(MarkerDialog)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SEGMENT, OnDeltaposSpinSegment)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SIZE, OnDeltaposSpinSize)
	ON_EN_CHANGE(IDC_SEGMENT, OnChangeSegment)
	ON_EN_CHANGE(IDC_SIZE, OnChangeSize)
	ON_EN_CHANGE(IDC_MARKER_SIZE, OnChangeMarkerSize)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MarkerDialog message handlers

void MarkerDialog::OnDeltaposSpinSegment(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
   if(pNMUpDown->iDelta > 0)
   {
      fSegment-- ; 
      if (fSegment < 0) fSegment = 0;
   }
   else
   {
      fSegment++ ;
   }
   

   fNumMarkers = fView->getNumSides();
   SoMFVec3f tempCoords;
   //fView->getSegmentCoords(fSegment, tempCoords);//!!!

    for (int i = 0; i < fNumMarkers; i++)
    {
         fMarkerTransform[i]->translation.setValue(tempCoords[i]);
    }
   UpdateData(FALSE);
	*pResult = 0;
}

void MarkerDialog::OnDeltaposSpinSize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

   if(pNMUpDown->iDelta > 0)
   {
      fSize-- ; 
      if (fSize < 0) fSize = 0;
   }
   else
   {
      fSize++ ;
   }

   fNumMarkers = fView->getNumSides();
    for (int i = 0; i < fNumMarkers; i++)
    {
         fMarkerTransform[i]->scaleFactor.setValue(fSize *.1, fSize *.1, fSize*.1);
    }
   UpdateData(FALSE);
	*pResult = 0;
}


void MarkerDialog::OnChangeSegment() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
   UpdateData(TRUE);
	
}

void MarkerDialog::OnChangeSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

void MarkerDialog::OnChangeMarkerSize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here
	
}

BOOL MarkerDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	fNumMarkers = fView->getNumSides();
   SoMFVec3f tempCoords;
   //fView->getSegmentCoords(fSegment, tempCoords); //!!!

    for (int i = 0; i < fNumMarkers; i++)
    {
         fMarkerSphere[i] = new SoSphere;
         fMarkerTransform[i] = new SoTransform;
         fMarkerMaterial[i] = new SoMaterial; 
         fMarkerSeparator[i] = new SoSeparator;
         fMarkerSeparator[i]->addChild(fMarkerTransform[i]);
         fMarkerSeparator[i]->addChild(fMarkerMaterial[i]);
         fMarkerSeparator[i]->addChild(fMarkerSphere[i]);
         fMarkerSeparator[i]->ref();
         fView->fMarkerTopSeparator->addChild(fMarkerSeparator[i]);

         fMarkerMaterial[i]->diffuseColor.setValue(.99, 0, 0);
         fMarkerTransform[i]->translation.setValue(tempCoords[i]);
         fMarkerTransform[i]->scaleFactor.setValue(fSize * .1, fSize * .1, fSize * .1);

    }
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MarkerDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	   TRACE("OnDestroy\n");
   for(int i = 0; i < fNumMarkers; i++)
   {
      fMarkerSeparator[i]->removeAllChildren();
      fMarkerSeparator[i]->unref();
   }
   fView->fMarkerDialog = NULL;
	
}

BOOL MarkerDialog::DestroyWindow() 
{
	   TRACE("OnDestroyWindow\n");
   for(int i = 0; i < fNumMarkers; i++)
   {
      fMarkerSeparator[i]->removeAllChildren();
      fMarkerSeparator[i]->unref();
   }
   fView->fMarkerDialog = NULL;	
	return CDialog::DestroyWindow();
}

void MarkerDialog::OnClose() 
{
	   TRACE("OnClose\n");
   for(int i = 0; i < fNumMarkers; i++)
   {
      fMarkerSeparator[i]->removeAllChildren();
      fMarkerSeparator[i]->unref();
   }
   fView->fMarkerDialog = NULL;	
	CDialog::OnClose();
}
