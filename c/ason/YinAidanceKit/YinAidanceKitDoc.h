// YinAidanceKitDoc.h : interface of the CYinAidanceKitDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_YINAIDANCEKITDOC_H__F9714AEE_4D00_4C41_8EA0_C18A6AB8C0A1__INCLUDED_)
#define AFX_YINAIDANCEKITDOC_H__F9714AEE_4D00_4C41_8EA0_C18A6AB8C0A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CYinAidanceKitDoc : public CDocument
{
protected: // create from serialization only
	CYinAidanceKitDoc();
	DECLARE_DYNCREATE(CYinAidanceKitDoc)

// Attributes
public:
		
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYinAidanceKitDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CYinAidanceKitDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CYinAidanceKitDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_YINAIDANCEKITDOC_H__F9714AEE_4D00_4C41_8EA0_C18A6AB8C0A1__INCLUDED_)
