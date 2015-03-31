// pMakerDoc.cpp : implementation of the CpMakerDoc class
//

#include "stdafx.h"
#include "pMaker.h"

#include "pMakerDoc.h"

#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoText2.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CpMakerDoc

IMPLEMENT_DYNCREATE(CpMakerDoc, CDocument)

BEGIN_MESSAGE_MAP(CpMakerDoc, CDocument)
END_MESSAGE_MAP()


// CpMakerDoc construction/destruction

CpMakerDoc::CpMakerDoc()
{
	//root = NULL;
}

CpMakerDoc::~CpMakerDoc()
{
    //if (root)
    //    root->unref();
}

BOOL CpMakerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}




// CpMakerDoc serialization

void CpMakerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		//FractalTreeSpec::Serialize();

	}
	else
	{
		// TODO: add loading code here
	}
}


// CpMakerDoc diagnostics

#ifdef _DEBUG
void CpMakerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CpMakerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CpMakerDoc commands
