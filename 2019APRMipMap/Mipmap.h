#pragma once
#include "DImage.h"
#include <afxwin.h>

class CMipmap
{
public:
	CDC* drawDC;
	DImage img;

	CMipmap(CDC* pDC, CString nazivSlike);
	void DrawMipmap(CDC* pDC, int x, int y, int size);
};