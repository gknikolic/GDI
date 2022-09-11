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
    grafika->Load(CString("Grafika.png"));
    initializeXFORM();
}

GDIView::~GDIView()
{
    //DeleteEnhMetaFile(Viljuska);
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

void GDIView::DrawFlag(CDC* pDC, DImage* img, float cy, float cx, float a, int n, float h, float w) {

        Translate(pDC, cx, cy, false);
        float pomerajX = w / n; //pomeraj po x osi
        float korakUglaPoX = 360 / w;

        float y, sx, sy = 0;
        float staroY = 0;
        float x;
    
        //pDC->Ellipse(-5, -5, 5, 5);
        //Shear(pDC, 0, -0.1, false); //NE TREBA ZA SLIKU POD B!
        //float stranica = 1.0 / n;
        for (int i = 0; i < n; i++) {
            x = (i + 1) * pomerajX;
            y = -a * sin(korakUglaPoX * x * toRad); //cos(Pi/6)->cos(30)!!! mora toDeg // posto y raste na dole, ovde imamo - da bi sunus prvo poceo da raste
            sx = (y - staroY) / pomerajX;

            Shear(pDC, sx, 0, false);
            pDC->Rectangle(0, 0, pomerajX, h);
            grafika->Draw(pDC, CRect(i * grafika->Width() / n, 0, (i + 1) * grafika->Width() / n, grafika->Height()), CRect(0, 0, pomerajX, h));
            //grafika->Draw(pDC, CRect(i*stranica*grafika->Width(), 0,(i*stranica+stranica)*grafika->Width(), grafika->Height()), CRect(0, 0, pomerajX, h));
            Translate(pDC, pomerajX, 0, false);
            Shear(pDC, -sx, 0, false); // ponistava se transformacija
            staroY = y;
        }
        //Shear(pDC, 0, 0.1, false);

       /* COLORREF color = GetColorOfFirstPixel(grafika);
        grafika->DrawWithouthBackground(pDC, color);*/

        Translate(pDC, -w, 0, false);
        pDC->Ellipse(-5, -5, 5, 5);
        Translate(pDC, -cx, -cy, false);

}

void GDIView::DrawFlags(CDC* pDC, DImage* img, float cy, float cx, int m, float a, int n, float h, float w) {
    float angle = 360.0 / m;
    float naspramna = h / 2;
    float hipotenuza = naspramna / sin(toRad * angle / 2);//hipotenuza je r
    float nalegla = sqrt(pow(hipotenuza, 2) - pow(naspramna, 2));
    CPoint *points=new CPoint[m];

    for (int i = 0; i < m; i++) {
        points[i].x = hipotenuza * cos(i * angle * toRad);
        points[i].y= hipotenuza * sin(i * angle * toRad);
    }

    //for (int i = 0; i < m; i++) {
    //    pDC->MoveTo(points[i]);
    //    pDC->LineTo(points[(i + 1) % m]);
    //}

    pDC->MoveTo(0,0);
    for (int i = 0; i < m; i++) {
        // levo mnozenje znaci da pomeramo lokalni koordinatni sistem, a transformacije se izvrsavaju u redosledu u kom se navode
        Rotate(pDC, (i*angle), false);
        Translate(pDC, nalegla,-h/2, false);
        //pDC->Ellipse(-10, -10, 10, 10);
        DrawFlag(pDC, img, 0, 0, a, n, h, w);
        //DrawFlag(pDC, img, -h/2, nalegla, a, n, h, w);
        Translate(pDC, -nalegla, h/2, false);
        Rotate(pDC, -((i * angle)), false);
    }

    pDC->MoveTo(0, 0);
}


void GDIView::Draw(CDC* pDC) {
    Scale(pDC, 0.5, 0.5, false);
    DrawFlags(pDC, grafika, 200, 200,6, 20, 8, 100, 200);
    Scale(pDC, 2, 2, false);

    // vezbanje
    //pDC->Ellipse(-10, -10, 10, 10);
    //Rotate(pDC, 45, false); Translate(pDC, 50, 0, false);
    //pDC->MoveTo(0, 0);
    //pDC->LineTo(320, 0);
    //Rotate(pDC, -45, false); Translate(pDC, -50, 0, false);
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

    Translate(memDC, 200, 200, false);
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