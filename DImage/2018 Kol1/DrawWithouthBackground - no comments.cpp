void DImage::DrawWithouthBackground(CDC* pDC, COLORREF clrTopLeft)
{
    BITMAP bm;
    bm.bmWidth = this->Width();
    bm.bmHeight = this->Height();

    CDC* srcDC = new CDC();
    srcDC->CreateCompatibleDC(pDC);
    CBitmap bmpImage;
    bmpImage.CreateCompatibleBitmap(pDC, bm.bmWidth, bm.bmHeight);
    CBitmap* oldSrcBmp = srcDC->SelectObject(&bmpImage);

    CDC* dstDC = new CDC();
    dstDC->CreateCompatibleDC(pDC);
    CBitmap mask;
    mask.CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
    CBitmap* oldDstBmp = dstDC->SelectObject(&mask);

    this->Draw(srcDC, CRect(0, 0, bm.bmWidth, bm.bmHeight), CRect(0, 0, bm.bmWidth, bm.bmHeight));

    //COLORREF clrTopLeft = srcDC->GetPixel(0, 0);
    COLORREF clrSaveBk = srcDC->SetBkColor(clrTopLeft);

    dstDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, srcDC, 0, 0, SRCCOPY);
    COLORREF clrSaveDstText = srcDC->SetTextColor(RGB(255, 255, 255)); //foreground

    srcDC->SetBkColor(RGB(0, 0, 0));
    srcDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, dstDC, 0, 0, SRCAND);

    dstDC->SetTextColor(clrSaveDstText);
    srcDC->SetBkColor(clrSaveBk);
    srcDC->SelectObject(oldSrcBmp);
    dstDC->SelectObject(oldDstBmp);

    srcDC->DeleteDC();
    delete srcDC;
    dstDC->DeleteDC();
    delete dstDC;

    CDC* MemDC = new CDC();
    MemDC->CreateCompatibleDC(pDC);
    CBitmap* oldMemBmp = MemDC->SelectObject(&mask);
    pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, MemDC, 0, 0, SRCAND);

    MemDC->SelectObject(bmpImage);
    pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, MemDC, 0, 0, SRCPAINT);

    MemDC->SelectObject(oldMemBmp);
    MemDC->DeleteDC();
    delete MemDC;
}