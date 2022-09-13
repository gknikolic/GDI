
// MFCApplication1View.cpp : implementation of the CMFCApplication1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCApplication1.h"
#endif

#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication1View

IMPLEMENT_DYNCREATE(CMFCApplication1View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFCApplication1View construction/destruction

CMFCApplication1View::CMFCApplication1View() noexcept
{
	// TODO: add construction code here

}

CMFCApplication1View::~CMFCApplication1View()
{
}

BOOL CMFCApplication1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCApplication1View drawing

void CMFCApplication1View::OnDraw(CDC* pDC)
{
	CMFCApplication1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);

	XFORM* lpxform = new XFORM();
	lpxform->eDx = 0;
	lpxform->eDy = 0;
	lpxform->eM11 = 1.0;
	lpxform->eM12 = 0.0;
	lpxform->eM21 = 0.0;
	lpxform->eM22 = 1.0;

	pDC->SetWorldTransform(lpxform);

	this->DrawFigure(pDC, 500, 300, 100, 50, 250, 10,10);


	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CMFCApplication1View::Translate(CDC* pDC, float dx, float dy, bool right)
{
	XFORM* lpxform = new XFORM();
	lpxform->eDx = dx;
	lpxform->eDy = dy;
	lpxform->eM11 = 1.0;
	lpxform->eM12 = 0.0;
	lpxform->eM21 = 0.0;
	lpxform->eM22 = 1.0;
	pDC->ModifyWorldTransform(lpxform, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMFCApplication1View::Scale(CDC* pDC, float sx, float sy, bool right)
{
	XFORM* lpxform = new XFORM();
	lpxform->eDx = 0;
	lpxform->eDy = 0;
	lpxform->eM11 = sx;
	lpxform->eM12 = 0.0;
	lpxform->eM21 = 0.0;
	lpxform->eM22 = sy;
	pDC->ModifyWorldTransform(lpxform, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMFCApplication1View::Rotate(CDC* pDC, float alpha, bool right)
{
	XFORM* lpxform = new XFORM();
	float rad = alpha * (3.14 / 180);
	lpxform->eDx = 0;
	lpxform->eDy = 0;
	lpxform->eM11 = cos(rad);
	lpxform->eM12 = sin(rad);
	lpxform->eM21 = -sin(rad);
	lpxform->eM22 = cos(rad);
	pDC->ModifyWorldTransform(lpxform, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CMFCApplication1View::DrawConeBottom(CDC* pDC, float cx, float cy, float a, float b, float h, int n)
{
	float increment = 360 / n;

	Translate(pDC, cx, cy, false);
	pDC->MoveTo(0, 0); // vrh kupe

	// omotac
	for (int i = ceil(n / 2.0); i < n; i++) { // samo prednja strana omotaca
		pDC->BeginPath();
		float x = a * cos(increment * i * toRad);
		float y = b * sin(increment * i * toRad) + h;

		pDC->LineTo(x, y);

		x = a * cos(increment * (i + 1) * toRad);
		y = b * sin(increment * (i + 1) * toRad) + h;

		pDC->LineTo(x, y);

		pDC->LineTo(0, 0);

		pDC->EndPath();

		COLORREF black = 0x000000;
		COLORREF blakk = (0x000000 + (n-i-1) * 0xffffff / ceil(n / 2));

		CBrush brush(blakk);

		CPen pen(0, 2.5, black);
		pDC->SelectObject(&pen);

		pDC->SelectObject(&brush);
		pDC->StrokeAndFillPath();
	}

	// omotac
	float x = a * cos(0);
	float y = b * sin(0) + h;
	pDC->MoveTo(x, y);

	pDC->BeginPath();

	for (int i = 1; i < n; i++) {
		float x = a * cos(increment * i * toRad);
		float y = b * sin(increment * i * toRad) + h;

		pDC->LineTo(x, y);
	}

	pDC->LineTo(x, y);
	pDC->EndPath();

	COLORREF white = 0xffffff;
	COLORREF blakk = 0x000000;

	CBrush brush(white);

	CPen pen(0, 2.5, blakk);
	pDC->SelectObject(&pen);

	pDC->SelectObject(&brush);
	pDC->StrokeAndFillPath();

	Translate(pDC, -cx, -cy, false);
}

void CMFCApplication1View::DrawConeTop(CDC* pDC, float cx, float cy, float a, float b, float h, int n)
{
	float increment = 360 / n;

	Translate(pDC, cx, cy, false);
	pDC->MoveTo(0, 0); //vrh kupe


	for (int i = 0; i < ceil( n / 2.0); i++) {
		pDC->BeginPath();
		float x = a * cos(increment * i * toRad) ;
		float y = b * sin(increment * i * toRad) + h;

		pDC->LineTo(x, y);

		x = a * cos(increment * (i + 1) * toRad);
		y = b * sin(increment * (i + 1) * toRad) + h;

		pDC->LineTo(x, y);

		pDC->LineTo(0, 0);

		pDC->EndPath();

		COLORREF black = 0x000000;
		COLORREF blakk = (0x000000 + i * 0xffffff/ceil(n/2));

		CBrush brush(blakk);

		CPen pen(0, 2.5, black);
		pDC->SelectObject(&pen);

		pDC->SelectObject(&brush);
		pDC->StrokeAndFillPath();
	}

	Translate(pDC, -cx, -cy, false);
}

void CMFCApplication1View::DrawFigure(CDC* pDC, float cx, float cy, float a, float b, float h, int n, float r)
{

	Translate(pDC, cx, cy, false);

	Rotate(pDC, 45, false);
	DrawConeTop(pDC, 0, r, a, b, h, n);
	Rotate(pDC, 90, false);
	DrawConeTop(pDC, 0, r, a, b, h, n);

	Rotate(pDC, 90, false);
	DrawConeBottom(pDC, 0, r, a, b, h, n);
	Rotate(pDC, 90, false);
	DrawConeBottom(pDC, 0, r, a, b, h, n);
}



// CMFCApplication1View printing

BOOL CMFCApplication1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCApplication1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCApplication1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMFCApplication1View diagnostics

#ifdef _DEBUG
void CMFCApplication1View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication1Doc* CMFCApplication1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication1Doc)));
	return (CMFCApplication1Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFCApplication1View message handlers
