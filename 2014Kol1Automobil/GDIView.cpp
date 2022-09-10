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
    tocak = new DImage();
    tocak->Load(CString("Wheel.png"));
    
    automobil = GetEnhMetaFile(CString("clio.emf"));
}

GDIView::~GDIView()
{
    delete tocak;
    DeleteEnhMetaFile(automobil);
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
//void GDIView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
//{
//	XFORM m;
//	m.eM11 = 1;
//	m.eM12 = 0;
//	m.eM21 = 0;
//	m.eM22 = 1;
//	m.eDx = dX;
//	m.eDy = dY;
//	ModifyWorldTransform(pDC->m_hDC, &m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
//}
//void GDIView::Rotate(CDC* pDC, float angle, bool rightMultiply)
//{
//	XFORM m;
//	m.eM11 = cos(angle * toRad);
//	m.eM12 = sin(angle * toRad);
//	m.eM21 = -sin(angle * toRad);
//	m.eM22 = cos(angle * toRad);
//	m.eDx = 0;
//	m.eDy = 0;
//	ModifyWorldTransform(pDC->m_hDC, &m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
//}
//void GDIView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
//{
//    //Scale moze da se koristi za mirror, tako sto se za x i y prosledjuju negativne vrednosti
//    //Da bi se ponistio mirror, ponoviti transformaciju sa istim vrednostima za x i y
//    //(pDc, 1, -1, false) - mirror u odnosu na x osu: Tacka A(2,5)->A(2,-5) (preslikava se u odnosu na X osu)
//    //(pDc,  -1, 1, false) - mirror u odnosu na y osu
//    //(pDc, -1, -1, false) - mirror u odnosu na x i y osu
//
//	XFORM m;
//	m.eM11 = sX;
//	m.eM12 = 0;
//	m.eM21 = 0;
//	m.eM22 = sY;
//	m.eDx = 0;
//	m.eDy = 0;
//	ModifyWorldTransform(pDC->m_hDC, &m, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
//}
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
#pragma region 2014Kol1Automobil
#pragma region code
void GDIView::Draw(CDC* pDC) {
    CRect rect;
    GetClientRect(&rect);
    
    float carWidth = 450;
    float carHeight = 180;
    float wheelR = 38; //poluprecnik tocka

    DrawGround(pDC, angle);

    Translate(pDC, 0, rect.Height(), false);
    Rotate(pDC, -angle, false);
    
    Translate(pDC, carWidth / 2, -carHeight/2, false);
    Translate(pDC, 0, -wheelR / 2, false);
    Translate(pDC, carDistance, 0, false);
    
    float wheelAngle = toDeg * (carDistance * 360) / (2 * wheelR * pi);
    DrawWheel(pDC, -155, 70, wheelR, wheelAngle);
    DrawWheel(pDC, 135, 70, wheelR, wheelAngle);


    DrawCar(pDC, 0, 0, carWidth, carHeight);

}

void GDIView::DrawWheel(CDC* pDC, int x, int y, int r, float angle) {
    Translate(pDC, x, y, false);
    Rotate(pDC, angle, false);

    int dx = 52, dy = 15, w = 376;
    CRect srcRect(dx, dy, dx + w, dy + w);
    CRect dstRect(-r, -r, r, r);

    //promeni RGB(255,255,255) na ucitavanje prvog piksela iz slike
    tocak->DrawTransparent(pDC, srcRect, dstRect, RGB(255, 255, 255));

    Rotate(pDC, -angle, false);
    Translate(pDC, -x, -y, false);
}


void GDIView::DrawGround(CDC* pDC, float angle) {
    CRect rect;
    GetClientRect(&rect);

    CPoint points[3];
    points[0] = CPoint(0, rect.Height());
    points[1] = CPoint(rect.Width(), rect.Height());
    points[2] = CPoint(rect.Width(), rect.Height() - rect.Width() * tan(angle * toRad));

    CPen pen(PS_SOLID, 3, RGB(64, 0, 0));
    CPen* oldPen = pDC->SelectObject(&pen);
    CBrush brush(HS_FDIAGONAL, RGB(64, 0, 0));
    CBrush* oldBrush = pDC->SelectObject(&brush);
    COLORREF oldBkColor = pDC->SetBkColor(RGB(64, 192, 64));

    pDC->Polygon(points, 3);

    pDC->SetBkColor(oldBkColor);
    pDC->SelectObject(oldBrush);

    pDC->SelectObject(oldBrush);
    pDC->SelectObject(oldPen);
    brush.DeleteObject();
    pen.DeleteObject();
}
void GDIView::Translate(CDC* pDC, float dx, float dy, bool rightMultiply) {
    XFORM form;
    form.eM11 = 1;
    form.eM12 = 0;
    form.eM21 = 0;
    form.eM22 = 1;
    form.eDx = dx;
    form.eDy = dy;

    pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void GDIView::Rotate(CDC* pDC, float angle, bool rightMultiply) {
    XFORM form;
    form.eM11 = cos(angle * toRad);
    form.eM12 = sin(angle * toRad);
    form.eM21 = -sin(angle * toRad);
    form.eM22 = cos(angle * toRad);
    form.eDx = 0;
    form.eDy = 0;

    pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void GDIView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply) {
    XFORM form;
    form.eM11 = sX;
    form.eM12 = 0;
    form.eM21 = 0;
    form.eM22 = sY;
    form.eDx = 0;
    form.eDy = 0;
    pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}
void GDIView::DrawCar(CDC* pDC, int x, int y, int w, int h) {
    Scale(pDC, -1, 1, false);
    pDC->PlayMetaFile(automobil, CRect(x-w/2, y-h/2, x + w/2 , y + h/2));
    Scale(pDC, -1, 1, false);
}

#pragma endregion
#pragma endregion
void GDIView::OnDraw(CDC* pDC)
{
    CRect cRect;
    GetClientRect(&cRect);
   
    CDC* memDC = new CDC();
    memDC->CreateCompatibleDC(pDC);
    
    CBitmap bmp;
    bmp.CreateCompatibleBitmap(pDC, cRect.Width(), cRect.Height());
    memDC->SelectObject(bmp);

    int oldMode = memDC->SetGraphicsMode(GM_ADVANCED);
    XFORM oldForm;
    memDC->GetWorldTransform(&oldForm);

    CBrush brush(RGB(255, 255, 255)); //Boja pozadine
    CBrush* oldBrush = memDC->SelectObject(&brush);
    memDC->Rectangle(0, 0, cRect.Width(), cRect.Height());
    memDC->SelectObject(oldBrush);

    //----------------------------------------------
    
    //Translate(memDC, 100, 100, false);
    Draw(memDC);

    //----------------------------------------------
    
    memDC->SetWorldTransform(&oldForm);
    memDC->SetGraphicsMode(oldMode);

    pDC->BitBlt(0, 0, cRect.Width(), cRect.Height(), memDC, 0, 0, SRCCOPY);

    memDC->DeleteDC();
    delete memDC;
    bmp.DeleteObject();
    brush.DeleteObject();
}

void GDIView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_TAB)
        exit(0);

    if (nChar == VK_UP && angle < 80) {
        angle += 10;
        carDistance = 0;
    }

    if (nChar == VK_DOWN && angle > -10) {
        angle -= 10;
        carDistance = 0;
    }

    if (nChar == VK_LEFT) {
        carDistance -= 40;
        carDistance -= 5;
    }

    if (nChar == VK_RIGHT) {
        carDistance += 40;
        //carDistance += 5;
    }

    Invalidate();
   /* if (nChar == '1')
    {
        rot1 -= step;
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

