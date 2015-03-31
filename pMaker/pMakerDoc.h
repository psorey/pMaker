// pMakerDoc.h : interface of the CpMakerDoc class
//

class SoSeparator;
#pragma once


class CpMakerDoc : public CDocument
{
protected: // create from serialization only
	CpMakerDoc();
	DECLARE_DYNCREATE(CpMakerDoc)

// Attributes
public:
   // SoSeparator *root;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CpMakerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


