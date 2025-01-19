// Client.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Client.h"
#include <stdio.h>
#include <iostream>

#include "Include.h"

#include "Logger.h"

#include "allocator.h"
#include "StackMemoryPool.h"

#include "MemoryInterface.h"

#include "EngineKernel.h"

#include "EASTL/list.h"

#pragma comment(lib, "../Binaries/DelightEngine.lib")

#define MAX_LOADSTRING 100

// 전역 변수:
HWND gHwnd;
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	_stprintf_s(szTitle, sizeof(szTitle) / sizeof(WCHAR), TEXT("Delight Engine v0.0"));

    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;

    CDelightEngineKernel Engine;
    
    Engine.Initialize(gHwnd);


	GLogger->ToggleConsole();

	GLogger->SetIgnoreLogLevel(5);

	GLogger->LogW(LOG_NORMAL, 10, TEXT("This is Normal"));
	GLogger->LogW(LOG_NORMAL, 3, TEXT("Lower LogLevel"));
	GLogger->LogW(LOG_ERROR, 10, TEXT("This is Error"));
	GLogger->LogW(LOG_INFO, 10, TEXT("This is Error"));
	GLogger->LogW(LOG_WARN, 10, TEXT("This is Warning"));

	//FOR_RANGE(index, 0, 100)
	//{
	//	GLogger.LogW(LOG_NORMAL, 15, TEXT("Loop Test : %d"), index);
	//	Sleep(100);
	//}

	//Delight::CStackMemoryPool Stack(256);

	//Delight::CAllocator allocator;

	//allocator.bind_memory_pool(&Stack);

	//int* array = (int*)allocator.allocate(4 * 12);


	//for (int i = 0; i < 12; ++i)
	//{
	//	array[i] = i;
	//}

	//for (int i = 0; i < 12; ++i)
	//{
	//	std::cout << array[i] << std::endl;
	//}

 //   int* testarray = new int[30];

 //   for (int i = 0; i < 30; ++i)
 //   {
 //       testarray[i] = i;
 //   }

	//for (int i = 0; i < 30; ++i)
	//{
 //       std::cout << testarray[i] << std::endl;
	//}

	eastl::list<int> test;

	for (int i = 0; i < 30; ++i)
	{
		test.push_back(i);
	}

	for (eastl::list<int>::iterator Iter(test.begin()); Iter != test.end(); ++Iter)
	{
        GLogger->LogW(LOG_WARN, 10, TEXT("%d"), *Iter);
	}

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = 0;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = 0;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = 0;

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   gHwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1280, 720, nullptr, nullptr, hInstance, nullptr);

   if (!gHwnd)
   {
      return FALSE;
   }

   ShowWindow(gHwnd, nCmdShow);
   UpdateWindow(gHwnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
