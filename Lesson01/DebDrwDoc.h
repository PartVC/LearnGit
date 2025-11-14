// DebDrwDoc.h : interface of the CDebDoc class
//


#pragma once
struct Pointd
{
	double x;
	double y;
};
struct Rectd
{
	double left;
	double right;
	double top;
	double bottom;
	double rightf;
	double topf;
};

typedef CArray<Pointd> Contour;


class CDebDoc : public CDocument
{
protected: // create from serialization only
	CDebDoc();
	DECLARE_DYNCREATE(CDebDoc)

private:
	void ReadFromFile(LPCTSTR lpszPathName);
	void ClearConts();

// Attributes
public:
	CTypedPtrArray <CPtrArray, Contour *> m_arPtrCont;
	//Contour m_arCont;
	Contour m_arDet;
	CArray<Rectd> m_arRect;
	CString m_strFileName;

	int m_iNewDocument;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CDebDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	afx_msg void OnFileReread();
};


