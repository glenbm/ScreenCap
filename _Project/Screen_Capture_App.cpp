// Screen_Capture_App.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "DesktopCapture.h"

int main()
{
	DesktopCapture capture(960, 540, 24);

	while (true)
	{
		capture.CaptureScreen(80);

		Sleep(300);
	}

	return 0;
}

