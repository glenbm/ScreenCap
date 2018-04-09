#pragma once
#include "CaptureData.h"

class CaptureBlock {
public:
	void Init(HDC screen, BITMAPINFO *bmpInfo, int bbp, int captureWidth, int captureHeight, RECT captureRect) {
		_captureOne.Initialize(screen, bmpInfo, captureWidth, captureHeight);
		_captureTwo.Initialize(screen, bmpInfo, captureWidth, captureHeight);

		_curCapture = &_captureOne;
		_prevCapture = &_captureTwo;

		_captureRect = captureRect;
		_imgSize = bbp * (_captureRect.right - _captureRect.left) * (_captureRect.bottom - _captureRect.top);
		_screenHandle = screen;
	}

	bool AreCapturesDifferent() {
		return _curCapture->CompareBuffer(_prevCapture, _imgSize);
	}

	void CaptureDesktop(ULONG quality) {
		SwapCaptures();

		_curCapture->GrabDesktop(
			_screenHandle,
			_targetWidth, 
			_targetHeight, 
			(_captureRect.right - _captureRect.left),
			(_captureRect.bottom - _captureRect.top),
			_captureRect.left,
			_captureRect.top
		);

		if (AreCapturesDifferent()) {
			return;
		}

		_curCapture->EncodeJpeg(quality);
	}

	void SwapCaptures(){
		if (_curCapture == &_captureOne) {
			_curCapture = &_captureTwo;
			_prevCapture = &_captureOne;
			return;
		}

		_curCapture = &_captureOne;
		_prevCapture = &_captureTwo;
	}


	CaptureData _captureOne, _captureTwo;
	CaptureData *_curCapture, *_prevCapture;

	HDC _screenHandle;

	RECT _captureRect;
	int _targetWidth, _targetHeight;
	int _imgSize;

};