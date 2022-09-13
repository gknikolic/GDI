#include "stdafx.h"
#include "Mipmap.h"
#include "DImage.h"

CMipmap::CMipmap(CDC* pDC, CString nazivSlike) {
    img.Load(CString("Grafika.png"));
    drawDC = new CDC();
    drawDC->CreateCompatibleDC(NULL);

    float totalWidth = 0;
    float width = img.Width();
    float height = img.Height();

    while (!(width == 1 && height == 1)) {
        totalWidth += width;

        if (width > 1)
            width /= 2;

        if (height > 1)
            height /= 2;
    }

    CBitmap bmp;
    bmp.CreateCompatibleBitmap(pDC, totalWidth, img.Height());
    drawDC->SelectObject(&bmp);

    width = img.Width();
    height = img.Height();
    float startWidth = 0;
    do {
        img.Draw(drawDC, CRect(0, 0, img.Width(), img.Height()), CRect(startWidth, 0, startWidth + width, height));
        startWidth += width;

        if (width > 1)
            width /= 2;

        if (height > 1)
            height /= 2;
    } while (!(width == 1 && height == 1));
}

void CMipmap::DrawMipmap(CDC* pDC, int x, int y, int size) {
    float prevWidth = img.Width();
    float prevHeight = img.Height();
    float width = img.Width();
    float height = img.Height();

    bool found = false;
    float startWidth = 0;

    while (!(width == 1 && height == 1) && !found) {
        if (width > 1)
            width /= 2;

        if (height > 1)
            height /= 2;

        if (abs(size - prevWidth * prevHeight) < abs(size - width * height)) {
            found = true;
        }
        else {
            startWidth += prevWidth;
            prevWidth = width;
            prevHeight = height;
        }
    }
    pDC->BitBlt(x, y, prevWidth, prevHeight, drawDC, startWidth, 0, SRCCOPY);
    //x, y - gde u odredistnom DC-u da se crta!!!
    //prevWidth, prevHeight - (velicina recta koji se crta)
    //startWidth, 0 - mesto od kog se uzima iz drawDC-a
}