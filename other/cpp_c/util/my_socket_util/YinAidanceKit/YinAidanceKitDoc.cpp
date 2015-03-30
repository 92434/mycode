// YinAidanceKitDoc.cpp : implementation of the CYinAidanceKitDoc class
//

#include "stdafx.h"
#include "YinAidanceKit.h"
#include "YinAidanceKitDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYinAidanceKitDoc

IMPLEMENT_DYNCREATE(CYinAidanceKitDoc, CDocument)

BEGIN_MESSAGE_MAP(CYinAidanceKitDoc, CDocument)
	//{{AFX_MSG_MAP(CYinAidanceKitDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYinAidanceKitDoc construction/destruction

CYinAidanceKitDoc::CYinAidanceKitDoc()
{


}

CYinAidanceKitDoc::~CYinAidanceKitDoc()
{

}

BOOL CYinAidanceKitDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	((CEditView*)m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CYinAidanceKitDoc serialization

void CYinAidanceKitDoc::Serialize(CArchive& ar)
{
	// CEditView contains an edit control which handles all serialization
	((CEditView*)m_viewList.GetHead())->SerializeRaw(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CYinAidanceKitDoc diagnostics

#ifdef _DEBUG
void CYinAidanceKitDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CYinAidanceKitDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CYinAidanceKitDoc commands
