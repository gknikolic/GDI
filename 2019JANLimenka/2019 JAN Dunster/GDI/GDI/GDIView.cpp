
// GDIView.cpp : implementation of the CGDIView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GDI.h"
#endif

#include "GDIDoc.h"
#include "GDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "DImage.h"

#define PI 3.14
#define ToRad PI/180

// CGDIView

IMPLEMENT_DYNCREATE(CGDIView, CView)

BEGIN_MESSAGE_MAP(CGDIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CGDIView construction/destruction

CGDIView::CGDIView() noexcept
{
	// TODO: add construction code here

}

CGDIView::~CGDIView()
{
}

void CGDIView::Translate(CDC* pDC, float dx, float dy, bool right)
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

void CGDIView::Scale(CDC* pDC, float sx, float sy, bool right)
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


void CGDIView::Shear(CDC* pDC, float sx, float sy, bool right)
{
	XFORM* lpxform = new XFORM();
	lpxform->eDx = 0;
	lpxform->eDy = 0;
	lpxform->eM11 = 1.0;
	lpxform->eM12 = sx;
	lpxform->eM21 = sy;
	lpxform->eM22 = 1.0;
	pDC->ModifyWorldTransform(lpxform, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CGDIView::DrawCanTop(CDC* pDC, DImage* img, float cX, float cY, float a, float b, int n)
{
	//CPoint* points = new CPoint[n];
	//int c = 0;
	//float h = img->Height();
	//float w = img->Width();
	//for (int i = 0; i < 360; i = i + 360 / n)
	//{
	//	points[c].x = h / 4 * cos(i * ToRad) + w / 2;
	//	points[c].y = h / 4 * sin(i * ToRad) + h / 4;
	//	c++;
	//}

	//CDC* pomDC = new CDC();
	//pomDC->CreateCompatibleDC(pDC);
	//img->Draw(pomDC, CRect(0, 0, w, h), CRect(0, 0, w, h));
	//CRgn polyRgn;
	//polyRgn.CreatePolygonRgn(points, n, WINDING);
	////HRGN hrgn = CreatePolygonRgn(points, c, 0);

	//pomDC->SelectClipRgn(&polyRgn,RGN_COPY);

	//delete[] points;

	CPen pen1(0, 2, RGB(0, 0, 0));
	CBrush brush(0, RGB(0, 0, 0));
	pDC->SelectObject(&pen1);
	pDC->SelectObject(&brush);

	pDC->BeginPath();

	for (int i = 0; i < 360; i = i + 360 / n)
	{
		float x = a * cos(i * ToRad) + cX;
		float y = b * sin(i * ToRad) + cY;
		if (i == 0)
			pDC->MoveTo(x, y);
		else
			pDC->LineTo(x, y);
	}

	pDC->EndPath();
	pDC->StrokeAndFillPath();

}

void CGDIView::DrawCanWrapper(CDC* pDC, DImage* img, float cX, float cY, float a, float b, int n, float h)
{
	CPen pen1(0, 2, RGB(0, 0, 0));
	CBrush brush( RGB(255, 255, 255));
	pDC->SelectObject(&pen1);
	pDC->SelectObject(&brush);


	for (int i = 0; i < 180; i = i + 360 / n)
	{
		float x = a * cos(i * ToRad) + cX;
		float y = b * sin(i * ToRad) + cY;

		float x1 = a * cos((i + 360 / n) * ToRad) + cX;
		float y1 = b * sin((i + 360 / n) * ToRad) + cY;

		pDC->BeginPath();
		pDC->MoveTo(x, y);
		pDC->LineTo(x, y + h);
		pDC->LineTo(x1, y1 + h);
		pDC->LineTo(x1, y1);

		pDC->EndPath();
		pDC->StrokeAndFillPath();

	}

}

void CGDIView::DrawCan(CDC* pDC, float cX, float cY, float a, float b, int n, float h)
{
	DImage img;

	img.Load(CString("paja.bmp"));

	this->DrawCanTop(pDC, &img, cX, cY, a, b, n);
	this->DrawCanWrapper(pDC, &img, cX, cY, a, b, n, h);
}

BOOL CGDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGDIView drawing

void CGDIView::OnDraw(CDC* pDC)
{
	CGDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)
		return;

	SetGraphicsMode(*pDC, GM_ADVANCED);

	XFORM* lpxform = new XFORM();
	lpxform->eDx = 0;
	lpxform->eDy = 0;
	lpxform->eM11 = 1.0;
	lpxform->eM12 = 0.0;
	lpxform->eM21 = 0.0;
	lpxform->eM22 = 1.0;

	pDC->SetWorldTransform(lpxform);

	this->DrawCan(pDC, 200, 100, 100, 50, 10, 300);


	// TODO: add draw code for native data here
}


// CGDIView printing

BOOL CGDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGDIView diagnostics

#ifdef _DEBUG
void CGDIView::AssertValid() const
{
	CView::AssertValid();
}

void CGDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGDIDoc* CGDIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGDIDoc)));
	return (CGDIDoc*)m_pDocument;
}
#endif //_DEBUG


// CGDIView message handlers
