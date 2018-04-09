// DesktopCapture.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "DesktopCapture.h"

#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>



#include <fstream>
#include <iterator>
#include <string>
#include <vector>


int captureCount = 0;
int buffer = 0;

DesktopCapture::DesktopCapture(int targetWidth, int targetHeight, int bitCount)
{
	_bitCount = bitCount;
	AssignTargetDimensions(targetWidth, targetHeight);
	Init();

}

void DesktopCapture::AssignTargetDimensions(int targetWidth, int targetHeight)
{
	_targetWidth = targetWidth;
	_targetHeight = targetHeight;
}

void DesktopCapture::Init()
{
	GetScreenValues();

	_fullDIBSize = GetSizeDIB();

	AssignHeaders();

	_captureOne.Initialize(_hdcScreen, &_bmpInfo, _targetWidth, _targetHeight);
	_captureTwo.Initialize(_hdcScreen, &_bmpInfo, _targetWidth, _targetHeight);

	_curCapture = &_captureOne;
	_prevCapture = &_captureTwo;
}

void DesktopCapture::SwapCaptures()
{
	if (_curCapture == &_captureOne)
	{
		buffer = 0;
		_curCapture = &_captureTwo;
		_prevCapture = &_captureOne;
		return;
	}

	buffer = 1;
	_curCapture = &_captureOne;
	_prevCapture = &_captureTwo;
}

int DesktopCapture::GetScreenValues()
{
	_hdcScreen = CreateDCW(L"DISPLAY", L"\\\\.\\DISPLAY1", NULL, NULL);

	_screenWidth = GetDeviceCaps(_hdcScreen, HORZRES);
	_screenHeight = GetDeviceCaps(_hdcScreen, VERTRES);

	return 1;
}

int DesktopCapture::GetSizeDIB()
{
	return ((((_targetWidth * _bitCount) + 31) & ~31) >> 3) * _targetHeight;
}

void DesktopCapture::AssignHeaders()
{
	// BMP FILE HEADER
	memset(&_bmpHeader, 0, sizeof(_bmpHeader));
	_bmpHeader.bfType = 0x4D42;
	_bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	// BMP INFO
	memset(&_bmpInfo, 0, sizeof(_bmpInfo));
	_bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	_bmpInfo.bmiHeader.biWidth = _targetWidth;
	_bmpInfo.bmiHeader.biHeight = _targetHeight;
	_bmpInfo.bmiHeader.biPlanes = 1;
	_bmpInfo.bmiHeader.biBitCount = _bitCount;
	_bmpInfo.bmiHeader.biCompression = BI_RGB;
	_bmpInfo.bmiHeader.biSizeImage = _fullDIBSize;
}

void DesktopCapture::CaptureScreen(ULONG quality)
{

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	SwapCaptures();
	captureCount++;

	std::cout << "CaptureCount: " << captureCount << ", Capturing screen on buffer " << buffer << ", ScreenRes(" << _targetWidth << ", " << _targetHeight << ")" << ", Quality: " << quality << std::endl;

	// Get Desktop and Scale
	_curCapture->GrabDesktop(_hdcScreen, _targetWidth, _targetHeight, _screenWidth/2, _screenHeight/2, 0, 0);

	// Compare
	std::cout << "CaptureCount: " << captureCount << ", Comparing buffer " << buffer << " with previous buffer" << std::endl;
	if (_curCapture->CompareBuffer(_prevCapture, _fullDIBSize)) {
		std::cout << "CaptureCount: " << captureCount << ", The two buffers are the same " << std::endl;
		//return;
	}
	else
	{
		std::cout << "CaptureCount: " << captureCount << ", The two buffers have changed " << std::endl;
	}

	_curCapture->EncodeJpeg(quality);

	std::cout << "CaptureCount: " << captureCount << ", Non-Compression size: " << _fullDIBSize << std::endl;
	std::cout << "CaptureCount: " << captureCount << ", Jpeg size is: " << _curCapture->_jpegCaptureSize << std::endl;

	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

	std::cout << "CaptureCount: " << captureCount << ", Time taken to capture screen: " << time_span.count() << std::endl;
	std::cout << std::endl;

}