
// GDIView.h : interface of the CGDIView class
//

#pragma once

#include "DImage.h"

class CGDIView : public CView
{
protected: // create from serialization only
	CGDIView() noexcept;
	DECLARE_DYNCREATE(CGDIView)

// Attributes
public:
	CGDIDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	void Translate(CDC*, float, float, bool);
	void Scale(CDC*, float, float, bool);
	void Shear(CDC*, float, float, bool);
	void DrawCanTop(CDC* pDC, DImage* img, float cX, float cY, float a, float b, int n);
	void DrawCanWrapper(CDC* pDC, DImage* img, float cX, float cY, float a, float b, int n, float h);
	void DrawCan(CDC* pDC, float cX, float cY, float a, float b, int n, float h);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GDIView.cpp
inline CGDIDoc* CGDIView::GetDocument() const
   { return reinterpret_cast<CGDIDoc*>(m_pDocument); }
#endif

