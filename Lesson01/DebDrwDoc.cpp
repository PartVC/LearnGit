// DebDrwDoc.cpp : implementation of the CDebDoc class
//

#include "stdafx.h"
#include "DebDrw.h"

#include "DebDrwDoc.h"
#include "DebDrwDoc.h"
#include "DebDrwView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDebDoc

IMPLEMENT_DYNCREATE(CDebDoc, CDocument)

BEGIN_MESSAGE_MAP(CDebDoc, CDocument)
	ON_COMMAND(ID_FILE_REREAD, &CDebDoc::OnFileReread)
END_MESSAGE_MAP()


// CDebDoc construction/destruction

CDebDoc::CDebDoc():m_iNewDocument(-1)
{
	m_strFileName.Empty();

}

CDebDoc::~CDebDoc()
{
	ClearConts();
}

BOOL CDebDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CDebDoc serialization

void CDebDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CDebDoc diagnostics

#ifdef _DEBUG
void CDebDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDebDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDebDoc commands

BOOL CDebDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_strFileName = lpszPathName;
	ReadFromFile(lpszPathName);
	m_iNewDocument = 0;

	POSITION pos = GetFirstViewPosition();
	CViewDeb *pView = (CViewDeb *)GetNextView(pos);
	UpdateAllViews(pView);

	return TRUE;
}

void CDebDoc::OnFileReread()
{
	ReadFromFile(m_strFileName);
	POSITION pos = GetFirstViewPosition();
	CViewDeb *pView = (CViewDeb *)GetNextView(pos);
	pView->RedrawWindow();
//	UpdateAllViews(pView);
}

void CDebDoc::ClearConts()
{
	if (m_arPtrCont.GetSize() > 0)
	{
		for (INT_PTR i = 0; i < m_arPtrCont.GetSize(); i++)
		{
			m_arPtrCont[i]->RemoveAll();
			delete m_arPtrCont[i];
		}
		m_arPtrCont.RemoveAll();
	}
	m_arRect.RemoveAll();
}

void CDebDoc::ReadFromFile(LPCTSTR lpszPathName)
{
	CFile oFile(lpszPathName, CFile::modeRead);
	
	INT_PTR ui, uiErr;
	INT_PTR iCount, iIndex;
	Pointd oPoi;
	
	ClearConts();

	//m_arCont.RemoveAll();
	//m_arDet.RemoveAll();

	while(1)
	{
	uiErr = oFile.Read(&ui, sizeof(INT_PTR));	// code
	if (uiErr <= 0) break;
	if (ui == 1)	// контур
	{
		oFile.Read(&iCount, sizeof(INT_PTR));

		Contour *pCont = new Contour();
		for (iIndex = 0; iIndex < iCount; iIndex++)
		{
			oFile.Read(&oPoi.x, sizeof(double));
			oFile.Read(&oPoi.y, sizeof(double));
			pCont->Add(oPoi);
		}
		m_arPtrCont.Add(pCont);
	}
	else
	{
		m_arRect.RemoveAll();

		oFile.Read(&iCount, sizeof(INT_PTR));
		Rectd oRc;
		for (iIndex = 0; iIndex < iCount; iIndex++)
		{
			oFile.Read(&oRc.left, sizeof(double));
			oFile.Read(&oRc.right, sizeof(double));
			oFile.Read(&oRc.top, sizeof(double));
			oFile.Read(&oRc.bottom, sizeof(double));
			oFile.Read(&oRc.rightf, sizeof(double));
			oFile.Read(&oRc.topf, sizeof(double));
			m_arRect.Add(oRc);
		}
	}
	}

	oFile.Close();
}