#pragma region ProjectDefault
// MFCKol1_2018View.cpp : implementation of the GDIView class
//

#include "stdafx.h"
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

// GDIView

IMPLEMENT_DYNCREATE(GDIView, CView)

BEGIN_MESSAGE_MAP(GDIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
    ON_WM_KEYDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// GDIView diagnostics

#ifdef _DEBUG
void GDIView::AssertValid() const
{
    CView::AssertValid();
}

void GDIView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}



GDIDoc* GDIView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(GDIDoc)));
    return (GDIDoc*)m_pDocument;
}
#endif //_DEBUG


// GDIView printing

BOOL GDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
    // default preparation
    return DoPreparePrinting(pInfo);
}

void GDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add extra initialization before printing
}

void GDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    // TODO: add cleanup after printing
}

BOOL GDIView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CView::PreCreateWindow(cs);
}



#pragma endregion

#include "math.h"
#include <string> 
using namespace std;

GDIView::GDIView() noexcept
{
    bgPen.CreatePen(PS_COSMETIC, 1, RGB(255, 255, 255));
    bgBrush.CreateSolidBrush(RGB(255, 255, 255));

    initializeXFORM();

    Bager.Load(L"bager.png");
    Arm1.Load(L"arm1.png");
    Arm2.Load(L"arm2.png");
    Pozadina.Load(L"pozadina.png");
    Viljuska = GetEnhMetaFile(L"viljuska.emf");
    GetEnhMetaFileHeader(Viljuska, sizeof(ENHMETAHEADER), &ViljuskaHeader);
    VWidth = ViljuskaHeader.rclBounds.right - ViljuskaHeader.rclBounds.left;
    VHeight = ViljuskaHeader.rclBounds.bottom - ViljuskaHeader.rclBounds.top;

}

GDIView::~GDIView()
{
    DeleteEnhMetaFile(Viljuska);
    delete memDC;
    bgPen.DeleteObject();
    bgBrush.DeleteObject();
}

#pragma region Useful functions
void GDIView::initializeXFORM() {
    ident.eM11 = 1;
    ident.eM12 = 0;
    ident.eM21 = 0;
    ident.eM22 = 1;
    ident.eDx = 0;
    ident.eDy = 0;
}
void GDIView::Begin_onDraw(CDC* pDC)
{
    //1
    GetClientRect(&cRect);
    //2
    memDC->CreateCompatibleDC(pDC);
    bitmap.CreateCompatibleBitmap(pDC, cRect.Width(), cRect.Height());
    memDC->SelectObject(bitmap);
    //3
    memDC->SetGraphicsMode(GM_ADVANCED);
    //4
    CPen* pen = memDC->SelectObject(&bgPen);
    memDC->SelectObject(&bgBrush);
    memDC->Rectangle(0, 0, cRect.Width(), cRect.Height());
    memDC->SelectObject(pen);
}
void GDIView::End_onDraw(CDC* pDC)
{
    //1
    memDC->SetWorldTransform(&ident);
    //2
    pDC->BitBlt(0, 0, cRect.Width(), cRect.Height(), memDC, 0, 0, SRCCOPY);
    //3
    memDC->DeleteDC();
    bitmap.DeleteObject();
}
void GDIView::writeCordsBottomRight() {
    GetClientRect(cRect);
    std::string text = "(";
    text += std::to_string((int)(cRect.right * 0.99));
    text += ",";
    text += std::to_string((int)(cRect.bottom * 0.99));
    text += ")";
    char const* cords = text.c_str();

    Translate(memDC, cRect.right * 0.975, cRect.bottom * 0.96, false);

    memDC->SetTextColor(RGB(0, 0, 0));
    memDC->TextOutW(-40, 0, CString(cords));

    Translate(memDC, -cRect.right * 0.975, -cRect.bottom * 0.96, false);
}
#pragma endregion

#pragma region Transformations
void GDIView::Shear(CDC* pDC, float sx, float sy, bool right)
{
    //(x,y)->(x+sy*y,y)
    //(x,y)->(x,sx*x+y)
    //DA BI SE VRSILO POMERANJE PO X OSI, PROSLEDJUJE SE VREDNOST ZA Y
    //Posluzice za razumevanje, ali u primeru y raste na gore: https://www.youtube.com/watch?v=Z2bR0Mb1Jj0&t=1s&ab_channel=JoelSperanzaMath

    XFORM* lpxform = new XFORM();
    lpxform->eDx = 0;
    lpxform->eDy = 0;
    lpxform->eM11 = 1.0;
    lpxform->eM12 = sx;
    lpxform->eM21 = sy;
    lpxform->eM22 = 1.0;
    pDC->ModifyWorldTransform(lpxform, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}
#pragma region ShearTest
//#pragma region code
//void GDIView::TestShear(int x, int y, int x1, int y1, int x2, int y2) {
//    CString str;
//    str.Format(L"x=%d, y=%d", x, y);
//    Translate(memDC, 150, 0, false);
//    memDC->TextOutW(-70, 0, str);
//    memDC->Ellipse(-5, -5, 5, 5);
//    Shear(memDC, x, y, false);
//    memDC->Rectangle(x1, y1, x2, y2);
//    Shear(memDC, -x, -y, false);
//}
//void GDIView::ShearCompleteTest() {
//    Translate(memDC, 100, 50, false);
//
//    //PRVI RED
//    memDC->Ellipse(-5, -5, 5, 5);
//    memDC->Rectangle(0, 0, 50, 50);
//
//    TestShear(1, 0, 0, 0, 50, 50);
//    TestShear(-1, 0, 0, 0, 50, 50);
//    TestShear(0, -1, 0, 0, 50, 50);
//    TestShear(0, 1, 0, 0, 50, 50);
//
//    Translate(memDC, -600, 120, false);
//
//    //DRUGI RED
//    memDC->Ellipse(-5, -5, 5, 5);
//    memDC->Rectangle(0, 0, -50, 50);
//
//    TestShear(1, 0, 0, 0, -50, 50);
//    TestShear(-1, 0, 0, 0, -50, 50);
//    TestShear(0, -1, 0, 0, -50, 50);
//    TestShear(0, 1, 0, 0, -50, 50);
//
//    Translate(memDC, -600, 120, false);
//
//    //TRECI RED
//    memDC->Ellipse(-5, -5, 5, 5);
//    memDC->Rectangle(0, 0, -50, -50);
//
//    TestShear(1, 0, 0, 0, -50, -50);
//    TestShear(-1, 0, 0, 0, -50, -50);
//    TestShear(0, -1, 0, 0, -50, -50);
//    TestShear(0, 1, 0, 0, -50, -50);
//
//    Translate(memDC, -600, 120, false);
//
//    //CETVRTI RED
//
//    memDC->Ellipse(-5, -5, 5, 5);
//    memDC->Rectangle(0, 0, 50, -50);
//
//    TestShear(1, 0, 0, 0, 50, -50);
//    TestShear(-1, 0, 0, 0, 50, -50);
//    TestShear(0, -1, 0, 0, 50, -50);
//    TestShear(0, 1, 0, 0, 50, -50);
//
//    Translate(memDC, -600, 120, false);
//}
//#pragma endregion
#pragma endregion
void GDIView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM m;
	m.eM11 = 1;
	m.eM12 = 0;
	m.eM21 = 0;
	m.eM22 = 1;
	m.eDx = dX;
	m.eDy = dY;
	ModifyWorldTransform(pDC->m_hDC, &m, MWT_LEFTMULTIPLY + rightMultiply);
}
void GDIView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM m;
	m.eM11 = cos(angle * toRad);
	m.eM12 = sin(angle * toRad);
	m.eM21 = -sin(angle * toRad);
	m.eM22 = cos(angle * toRad);
	m.eDx = 0;
	m.eDy = 0;
	ModifyWorldTransform(pDC->m_hDC, &m, MWT_LEFTMULTIPLY + rightMultiply);
}
#pragma region Mirror
//Ako se za X i Y u Scale koriste negativne vrednosti, dobija se mirror 
//Da bi se ponistio mirror, ponoviti transformaciju sa istim vrednostima za x i y
//(pDc, 1, -1, false) - mirror u odnosu na x osu 
//(pDc,  -1, 1, false) - mirror u odnosu na y osu
//(pDc, -1, -1, false) - mirror u odnosu na x i y osu
#pragma endregion
void GDIView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM m;
	m.eM11 = sX;
	m.eM12 = 0;
	m.eM21 = 0;
	m.eM22 = sY;
	m.eDx = 0;
	m.eDy = 0;
	ModifyWorldTransform(pDC->m_hDC, &m, MWT_LEFTMULTIPLY + rightMultiply);
}
#pragma endregion

#pragma region 2018KOL1Bager
#pragma region code
void GDIView::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
    //unsigned char* DBits = pImage->GetDIBBits();
    //COLORREF color = RGB(DBits[2], DBits[1], DBits[0]);

    COLORREF color = GetColorOfFirstPixel(pImage);
    pImage->DrawWithouthBackground(pDC, color);
}
void GDIView::DrawBackground(CDC* pDC)
{
    //OLD WAY - primer sa blanketa
    //int centerX = cRect.Width() / 2;
    //int offset = Pozadina.Width() / 2;
    //int leftX = centerX - offset;
    //int rightX = centerX + offset;

    //CRect rcImg(0, 0, Pozadina.Width(), Pozadina.Height());
    //CRect rcDC(leftX, cRect.bottom - Pozadina.Height(), rightX, cRect.bottom);
    //this->Pozadina.Draw(pDC, rcImg, rcDC);

    this->Pozadina.Draw(pDC, CRect(0, 0, Pozadina.Width(), Pozadina.Height()), CRect(0, 0, cRect.Width(), cRect.Height()));
}
void GDIView::DrawBody(CDC* pDC)
{
    int w = Bager.Width(), h = Bager.Height();
    Translate(pDC, -w, -h, false);
    DrawImgTransparent(pDC, &Bager);
    Translate(pDC, (int)(0.17 * w), (int)(0.57 * h), false);
}

void GDIView::DrawArm1(CDC* pDC)
{
    int w = Arm1.Width(), h = Arm1.Height();
    Rotate(pDC, rot1, false);
    Translate(pDC, -58, -61, false);
    DrawImgTransparent(pDC, &Arm1);
    Translate(pDC, 309, 61, false);
}

void GDIView::DrawArm2(CDC* pDC)
{
    int w = Arm2.Width(), h = Arm1.Height();
    Rotate(pDC, rot2, false);
    Translate(pDC, -36, -40, false);
    DrawImgTransparent(pDC, &Arm2);
    Translate(pDC, 272, 40, false);
}

void GDIView::DrawFork(CDC* pDC)
{
    Rotate(pDC, rot3, false);
    Scale(pDC, 2.5, 2.5, false);
    Translate(pDC, -14, -20, false);
    PlayEnhMetaFile(pDC->m_hDC, Viljuska, CRect(0, 0, VWidth, VHeight));
}

void GDIView::DrawExcavator(CDC* pDC)
{
    Translate(pDC, cRect.right * 0.99 + x, cRect.bottom * 0.99, false);
    DrawBody(memDC);
    DrawArm1(memDC);
    DrawArm2(memDC);
    DrawFork(memDC);
}


void GDIView::Draw(CDC* pDC) {
    DrawBackground(memDC);
    DrawExcavator(memDC);
}
#pragma endregion 
#pragma endregion

void GDIView::OnDraw(CDC* pDC)
{
	GDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

    Begin_onDraw(pDC);
    writeCordsBottomRight();

    //Translate(memDC, 200, 200, false);
    Draw(memDC);

 
    End_onDraw(pDC);
}

void GDIView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_TAB)
        exit(0);

   /* if (nChar == '1')
    {
        rot1 -= step;
    }
    else if(nChar == '2')
    {
        rot1 += step;
    }
    else if (nChar == '3')
    {
        rot2 -= step;
    }
    else if (nChar == '4')
    {
        rot2 += step;
    }
    else if (nChar == '5')
    {
        rot3 -= step;
    }
    else if (nChar == '6')
    {
        rot3 += step;
    }
    else if (nChar == VK_LEFT)
    {
        x -= step;
    }
    else if (nChar == VK_RIGHT)
    {
        x += step;
    }
    else
        return;*/
    Invalidate();
}

#pragma region other
COLORREF GDIView::GetColorOfFirstPixel(DImage* img)
{
    unsigned char* DBits = img->GetDIBBits();
    return RGB(DBits[2], DBits[1], DBits[0]);
}

void GDIView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    point.x, point.y;
    //sluzi da se pogledaju koordinate tacke na kojoj je bio dupli klik
}

BOOL GDIView::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;
}
#pragma endregion