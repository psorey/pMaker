#pragma once


// GetStringDialog dialog

class GetStringDialog : public CDialog
{
	DECLARE_DYNAMIC(GetStringDialog)

public:
	GetStringDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~GetStringDialog();

// Dialog Data
	enum { IDD = IDD_GET_STRING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CString fString;
};
