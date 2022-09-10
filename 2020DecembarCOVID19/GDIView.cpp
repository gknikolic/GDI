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
    ModifyWorldTransform(pDC->m_hDC, &m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
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
    ModifyWorldTransform(pDC->m_hDC, &m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void GDIView::Mirror(CDC* pDC, bool x, bool y, bool rightMultiply) {
    XFORM m;
    m.eM11 = y ? -1 : 1;
    m.eM12 = 0;
    m.eM21 = 0;
    m.eM22 = x ? -1 : 1;
    m.eDx = 0;
    m.eDy = 0;
    ModifyWorldTransform(pDC->m_hDC, &m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
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
    ModifyWorldTransform(pDC->m_hDC, &m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
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

#pragma region 2020DecembarCOVID19 
#pragma region code
void GDIView::SaveDC(CDC* pDC, CRect rcDC, CRect rcBmp) {
    CDC* saveDC = new CDC();
    saveDC->CreateCompatibleDC(NULL);

    CBitmap bmp;
    bmp.CreateCompatibleBitmap(pDC, rcBmp.Width(), rcBmp.Height());
    saveDC->SelectObject(&bmp);

    pDC->Ellipse(-5, -5, 5, 5);

    saveDC->BitBlt(0, 0, rcDC.Width(), rcDC.Height(), pDC, -rcDC.Width()/2, -rcDC.Height()/2, SRCCOPY);

    DImage img(bmp);
    img.Save(CString("Test.png"));

    saveDC->DeleteDC();
    delete saveDC;
    bmp.DeleteObject();
}
void GDIView::DrawCycle(CDC* pDC, int r, int n, int d) {
    float angle = 360 / n;
    float alpha = angle / 2;
    float stranica = 2 * (sin(alpha * toRad) * r);
    float h = sqrt(pow(r, 2) - pow(stranica / 2, 2));

    pDC->Rectangle(-stranica / 2, 0, stranica / 2, -8 * r);
    Translate(pDC, 0, -8 * r - h, false);

    CPoint* points = new CPoint[n];
    for (int i = 0; i < n; i++) {
        float x = r * cos(i * angle * toRad);
        float y = r * sin(i * angle * toRad);
        points[i] = CPoint(x, y);
    }

    pDC->Polygon(points, n);

    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen* oldPen = pDC->SelectObject(&pen);
    for (int i = 0; i < n; i++) {
        Translate(pDC, points[i].x, points[i].y, false);
        Rotate(pDC, i * angle, false);
        pDC->LineTo(2 * r, 0);
        pDC->MoveTo(0, 0);
        Rotate(pDC, -i * angle, false);
        Translate(pDC, -points[i].x, -points[i].y, false);
    }
    Translate(pDC, 0, -(-8 * r - h), false);
    pDC->SelectObject(oldPen);

    pen.DeleteObject();
    delete[] points;
}
void GDIView::DrawVirus(CDC* pDC, CPoint ptCenter, int rad, int m, int r, int n, int d, int alpha, int size, CString strText, COLORREF color) {
    float veceR = rad;
    float manjeR = 0.8 * rad;

    Translate(pDC, ptCenter.x, ptCenter.y, false);

    CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
    CPen* oldPen = pDC->SelectObject(&pen);

    CBrush brush(color);
    CBrush* oldBrush = pDC->SelectObject(&brush);

    pDC->Ellipse(-veceR, -veceR, veceR, veceR);

    pDC->SelectObject(oldBrush);
    pDC->Ellipse(-manjeR, -manjeR, manjeR, manjeR);

    float angle = 360 / m;

    for (int i = 0; i < m; i++) {
        Rotate(pDC, i * angle, false);
        Translate(pDC, 0, -veceR, false);
        DrawCycle(pDC, r, n, d);
        Translate(pDC, 0, veceR, false);
        Rotate(pDC, -i * angle, false);
    }

    CFont font;
    font.CreateFontW(size, size, 10 * alpha, 10 * alpha, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));

    int oldBkMode = pDC->SetBkMode(TRANSPARENT);
    int oldTextAlign = pDC->SetTextAlign(TA_CENTER);

    pDC->SelectObject(brush);


    pDC->SelectObject(oldPen);
    pDC->BeginPath();
    CFont* oldFont = pDC->SelectObject(&font);
    pDC->TextOutW(0, -size / 2, strText);
    pDC->EndPath();
    pDC->StrokeAndFillPath();

    pDC->SelectObject(oldBrush);
    Translate(pDC, -ptCenter.x, -ptCenter.y, false);

}
void GDIView::Draw(CDC* pDC) {
    CPoint p(150, 150);
    float radius = 90;
    float r = 10;
    float cycle = r * 11;
    DrawVirus(pDC, p, radius, 6, r, 6, 2, 45, 12, CString("covid-19"), RGB(128, 5, 200));
    //DrawCycle(pDC, 20, 6,2);

    Translate(pDC, p.x, p.y, false);
    //Nebitne su koordinate, bitno je samo rastojanje izmedju njih, jer se u SaveDC koristi samo Width(), Height()
    //SaveDC(pDC, CRect(- radius - cycle, - radius - cycle, radius + cycle, radius + cycle), CRect(-radius - cycle, -radius - cycle, radius + cycle, radius + cycle));
    SaveDC(pDC, CRect(0,0, 2*(radius+cycle), 2 * (radius + cycle)), CRect(0, 0, 2 * (radius + cycle), 2 * (radius + cycle)));
    Translate(pDC, -p.x, -p.y, false);
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

    Translate(memDC, 100, 60, false);
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
