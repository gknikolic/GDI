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

}

GDIView::~GDIView()
{
    //DeleteEnhMetaFile(Viljuska);
}

#pragma region Transformations
void GDIView::LoadIdentity(CDC* pDC) {
    XFORM m;
    m.eM11 = 1;
    m.eM12 = 0;
    m.eM21 = 0;
    m.eM22 = 1;
    m.eDx = 0;
    m.eDy = 0;
    pDC->SetWorldTransform(&m);
}
void GDIView::Shear(CDC* pDC, float sx, float sy, bool rightMultiply)
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
    pDC->ModifyWorldTransform(lpxform, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}
void GDIView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
    XFORM m;
    m.eM11 = 1;
    m.eM12 = 0;
    m.eM21 = 0;
    m.eM22 = 1;
    m.eDx = dX;
    m.eDy = dY;
    pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
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
    pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void GDIView::Mirror(CDC* pDC, bool x, bool y, bool rightMultiply) {
    XFORM m;
    m.eM11 = y ? -1 : 1;
    m.eM12 = 0;
    m.eM21 = 0;
    m.eM22 = x ? -1 : 1;
    m.eDx = 0;
    m.eDy = 0;
    pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void GDIView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
    //Scale moze da se koristi za mirror, tako sto se za x i y prosledjuju negativne vrednosti
    //Da bi se ponistio mirror, ponoviti transformaciju sa istim vrednostima za x i y
    //(pDc, 1, -1, false) - mirror u odnosu na x osu: Tacka A(2,5)->A(2,-5) (preslikava se u odnosu na X osu)
    //(pDc,  -1, 1, false) - mirror u odnosu na y osu
    //(pDc, -1, -1, false) - mirror u odnosu na x i y osu

    XFORM m;
    m.eM11 = sX;
    m.eM12 = 0;
    m.eM21 = 0;
    m.eM22 = sY;
    m.eDx = 0;
    m.eDy = 0;
    pDC->ModifyWorldTransform(&m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
#pragma endregion

#pragma region INT TO CSTRING
#pragma region
//CString str;
//    for (int i = 0; i < 12; i++) {
//        str.Format(L"%d", (i+2)%12+1);
//        memDC->TextOutW(points[i].x, points[i].y, str);
//    }
#pragma endregion
#pragma endregion

#pragma region imeBlanketa
#pragma region code
void GDIView::DrawCycle(CDC* pDC, int r, int n, int d) {
    CPen pen(PS_SOLID, d, RGB(0, 0, 255));
    CPen* oldPen = pDC->SelectObject(&pen);

    pDC->Ellipse(-r, -r, r, r);

    CPen pen2(PS_SOLID, 1, RGB(0, 0, 200));
    CBrush brush(RGB(0, 0, 200));
    pDC->SelectObject(&pen2);
    CBrush* oldBrush = pDC->SelectObject(&brush);

    pDC->Ellipse(-2 * d, -2 * d, 2 * d, 2 * d);

    float angle = 360. / n;
    for (int i = 0; i < n; i++) {
        pDC->LineTo(r * cos(angle * i * toRad), r * sin(angle * i * toRad));
        pDC->MoveTo(0, 0);
    }

    pDC->SelectObject(oldBrush);
    pDC->SelectObject(oldPen);
    brush.DeleteObject();
    pen.DeleteObject();
}
void GDIView::DrawBicycle(CDC* pDC, int r, int n, int d, int alpha, int beta) {

    float rVelikogTocka = r;
    float rMalogTocka = r / 4;

    float rVelikogRama = 1.4 * r;
    float rMalogRama = rVelikogTocka - rMalogTocka;

    DrawCycle(pDC, r, n, d);

    CRect rectVelikogRama(-rVelikogRama, -rVelikogRama, rVelikogRama, rVelikogRama);
    CPen penRam(PS_SOLID, d, RGB(0, 0, 240));
    CPen* oldPen = pDC->SelectObject(&penRam);

    pDC->Arc(rectVelikogRama, CPoint(rVelikogRama, 0), CPoint(0, -rVelikogRama));

    Translate(pDC, rVelikogRama + rMalogRama, rMalogRama, false);
    DrawCycle(pDC, r / 4, n, d);
    Translate(pDC, -(rVelikogRama + rMalogRama), -rMalogRama, false);

    CRect rectMalogRama(rVelikogRama, -rMalogRama, rVelikogRama + 2 * rMalogRama, rMalogRama);
    pDC->Arc(rectMalogRama, CPoint(rVelikogRama, 0), CPoint(rVelikogRama + rMalogRama, rMalogRama));

    CPen penOsovine(PS_SOLID, d, RGB(0, 0, 128));
    pDC->SelectObject(&penOsovine);

    pDC->MoveTo(rVelikogRama, 0);
    pDC->LineTo(rVelikogRama + 0.2 * (float)r, 0);
    pDC->MoveTo(0, 0);

    pDC->LineTo(0, -1.6 * r); //Pise 1.4*r, ali to nije dobra razmera
    pDC->LineTo(0.2 * r, -1.6 * r);
    pDC->MoveTo(0, 0);

    float xPedale = sin(alpha * toRad) * 0.4 * r;
    float yPedale = cos(alpha * toRad) * 0.4 * r;

    pDC->LineTo(-xPedale, -yPedale);
    pDC->MoveTo(-xPedale - 0.1 * r, -yPedale);
    pDC->LineTo(-xPedale + 0.1 * r, -yPedale);
    pDC->MoveTo(0, 0);

    pDC->LineTo(xPedale, yPedale);
    pDC->MoveTo(xPedale - 0.1 * r, yPedale);
    pDC->LineTo(xPedale + 0.1 * r, yPedale);
    pDC->MoveTo(0, 0);

    float x1Sedista = sin(beta * toRad) * rVelikogRama;
    float y1Sedista = cos(beta * toRad) * rVelikogRama;
    float x2Sedista = sin(beta * toRad) * (rVelikogRama + 0.2 * r);
    float y2Sedista = cos(beta * toRad) * (rVelikogRama + 0.2 * r);

    pDC->MoveTo(x1Sedista, -y1Sedista);
    pDC->LineTo(x2Sedista, -y2Sedista);

    pDC->MoveTo(x2Sedista - 0.15 * r, -y2Sedista);
    pDC->LineTo(x2Sedista + 0.15 * r, -y2Sedista);

    pDC->MoveTo(0, 0);

    pDC->SelectObject(oldPen);
    penRam.DeleteObject();
    penOsovine.DeleteObject();

    Translate(pDC, rVelikogRama + rMalogRama + rMalogTocka, rVelikogTocka, false);
    DrawText(pDC, 15, CString("penny-farthing"), RGB(0, 0, 200));
    Translate(pDC, -(rVelikogRama + rMalogRama + rMalogTocka), -rVelikogTocka, false);

    CRect rectSlike(-rVelikogTocka - d, -1.6 * r - d, rVelikogRama + rMalogRama + rMalogTocka + 15, rVelikogTocka + d);
    Translate(pDC, -rVelikogTocka - d, -1.6 * r - d, false);
    SaveDC(pDC, rectSlike, rectSlike);
    Translate(pDC, rVelikogTocka + d, 1.6 * r + d, false);

}
void GDIView::DrawText(CDC* pDC, int size, CString strText, COLORREF clrText) {
    CFont font;
    font.CreateFontW(size, size, 900, 900, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));
    CFont* oldFont = pDC->SelectObject(&font);

    int oldBkMode = pDC->SetBkMode(TRANSPARENT);
    CBrush brush(clrText);
    CBrush* oldBrush = pDC->SelectObject(&brush);

    pDC->BeginPath();
    pDC->TextOutW(0, 0, strText);
    pDC->EndPath();
    pDC->StrokeAndFillPath();

    pDC->SetBkMode(oldBkMode);
    pDC->SelectObject(oldBrush);
    pDC->SelectObject(oldFont);

    font.DeleteObject();
    brush.DeleteObject();
}
void GDIView::SaveDC(CDC* pDC, CRect rcDC, CRect rcBmp) {
    CDC* saveDC = new CDC();
    saveDC->CreateCompatibleDC(NULL);

    CBitmap bmp;
    bmp.CreateCompatibleBitmap(pDC, rcBmp.Width(), rcBmp.Height());

    saveDC->SelectObject(&bmp);
    saveDC->BitBlt(0, 0, rcDC.Width(), rcDC.Height(), pDC, 0, 0, SRCCOPY);

    DImage img(bmp);
    img.Save(CString("Bicikla.png"));

    bmp.DeleteObject();
    saveDC->DeleteDC();
    delete saveDC;
}
void GDIView::Draw(CDC* pDC) {
    DrawBicycle(pDC, 100, 18, 4, 45, 30);
}

#pragma endregion
#pragma endregion

void GDIView::OnDraw(CDC* pDC)
{
    CRect cRect;
    GetClientRect(&cRect);

    CDC* memDC = new CDC();
    memDC->CreateCompatibleDC(pDC);

    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, cRect.Width(), cRect.Height());
    memDC->SelectObject(bitmap);

    int oldMode = memDC->SetGraphicsMode(GM_ADVANCED);
    XFORM oldForm;
    memDC->GetWorldTransform(&oldForm);

    CBrush brush(RGB(255, 255, 255)); //Boja pozadine
    CBrush* oldBrush = memDC->SelectObject(&brush);
    memDC->Rectangle(0, 0, cRect.Width(), cRect.Height());
    memDC->SelectObject(oldBrush);

    //----------------------------------------------

    Translate(memDC, 200, 300, false);
    Draw(memDC);

    //----------------------------------------------

    memDC->SetWorldTransform(&oldForm);
    memDC->SetGraphicsMode(oldMode);

    pDC->BitBlt(0, 0, cRect.Width(), cRect.Height(), memDC, 0, 0, SRCCOPY);

    memDC->DeleteDC();
    delete memDC;
    bitmap.DeleteObject();
    brush.DeleteObject();
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
