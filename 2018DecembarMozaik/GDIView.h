
// MFCKol1_2018View.h : interface of the CMFCKol12018View class
//

#pragma once
#include "DImage.h"

class GDIView : public CView
{
public:

#pragma region Attributers
    XFORM ident;
    CDC* memDC = new CDC();
    CRect cRect;
    CPen bgPen;
    CBrush bgBrush;
    CBitmap bitmap;

    double pi = atan(1) * 4;
    double toRad = pi / 180;
    double toDeg = 180 / pi;
#pragma endregion

    void Draw(CDC* pDC);


#pragma region Transformations
    void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
    void Rotate(CDC* pDC, float angle, bool rightMultiply);
    void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
    void Shear(CDC*, float, float, bool);
#pragma region ShearTest
//#pragma region code
//    void TestShear(int x, int y, int x1, int y1, int x2, int y2);
//    void ShearCompleteTest();
//#pragma endregion
#pragma endregion
#pragma endregion

#pragma region Useful functions
    void initializeXFORM();
    void Begin_onDraw(CDC* pDC);
    void End_onDraw(CDC* pDC);
    void writeCordsBottomRight();   
#pragma endregion


#pragma region 2018DecembarMozaik RADI!
#pragma region code
DImage* Pozadina;
void DrawEndText(CDC* pDC, CRect rcPuzzle, CString sText, int fnSize, COLORREF clrText);
void DrawPuzzle(CDC* pDC, CRect rcPuzzle, int nRows, int nCols, float ratio, CString sPicture, bool** matOpen, COLORREF** matColors);
void DrawTrap(CDC* pDC, float a, float b, float x, float h, COLORREF color);
void DrawSideTrap(CDC* pDC, float a, float b, float x, float h, COLORREF color);
void DrawRow(CDC* pDC, float a, float b, float x, float h, int nCols,int row, bool** matOpen, COLORREF** matColors);
void SavePuzzle(CDC* pDC, CRect rcPuzzle, CString sFile);
#pragma endregion
#pragma endregion





#pragma region ProjectDefault
protected: // create from serialization only
    GDIView() noexcept;
    DECLARE_DYNCREATE(GDIView)

public:
    GDIDoc* GetDocument() const;

// Overrides
public:
    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~GDIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    COLORREF GetColorOfFirstPixel(DImage* img);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

#pragma endregion
};

#pragma region ProjectDefault2

#ifndef _DEBUG  // debug version in MFCKol1_2018View.cpp
inline CMFCKol12018Doc* CMFCKol12018View::GetDocument() const
   { return reinterpret_cast<CMFCKol12018Doc*>(m_pDocument); }
#endif

#pragma endregion
