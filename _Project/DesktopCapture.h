#pragma once

#include <windows.h>
#include "CaptureData.h"
#include "CaptureBlock.h"

class DesktopCapture {
public:
	DesktopCapture(int targetWidth, int targetHeight, int bitCount);

	void Init();
	void CaptureScreen(ULONG quality);

private:

	// methods
	void AssignTargetDimensions(int targetWidth, int targetHeight);
	int GetScreenValues();
	void AssignHeaders();
	int GetSizeDIB();
	void SwapCaptures();

	// variables
	HDC _hdcScreen;

	BITMAPFILEHEADER _bmpHeader;
	BITMAPINFO _bmpInfo;

	int _targetWidth, _targetHeight;
	int _screenWidth, _screenHeight;

	int _bitCount;

	int _fullDIBSize;

	CaptureData _captureOne, _captureTwo;
	CaptureData *_curCapture;
	CaptureData *_prevCapture;

	CaptureBlock captureBlockOne;
};