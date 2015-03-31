#pragma once


// GetScalesDialog dialog

class GetScalesDialog : public CDialog
{
	DECLARE_DYNAMIC(GetScalesDialog)

public:
	GetScalesDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~GetScalesDialog();

// Dialog Data
	enum { IDD = IDD_GET_SCALES_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    float fSecScale;
    float fCentScale;
    float fThickness;
};
