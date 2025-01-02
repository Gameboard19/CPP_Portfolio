// CalculatorApp.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "CalculatorApp.h"
#include <string>
#include <cmath>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
//global hwnds for my convenience
HWND hTextBox;                                  // where the answers and entered things show up
HWND h0, h1, h2, h3, h4, h5, h6, h7, h8, h9;    // number buttons
HWND hAdd, hSubtract, hMultiply, hDivide;       // operation buttons
HWND hEquals, hDecimal, hClear, hPar;           // equals and misc buttons
bool parMode = 1;                               // determines if it prints '(' or ')'

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
std::wstring             Calculate(LPCWSTR);
WCHAR*              Append(WCHAR[], WCHAR[]);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CALCULATORAPP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CALCULATORAPP));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CALCULATORAPP));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
      CW_USEDEFAULT, 0, 425, 459, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

std::wstring textBoxData = L"";

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //I'm throwing this here until I get this thing working
    switch (message)
    {
    case WM_COMMAND:
        //TODO: make these all else ifs in the future
        if ((HWND)lParam == h1) {
            textBoxData += L"1";
        }
        if ((HWND)lParam == h2) {
            textBoxData += L"2";
        }
        if ((HWND)lParam == h3) {
            textBoxData += L"3";
        }
        if ((HWND)lParam == h4) {
            textBoxData += L"4";
        }
        if ((HWND)lParam == h5) {
            textBoxData += L"5";
        }
        if ((HWND)lParam == h6) {
            textBoxData += L"6";
        }
        if ((HWND)lParam == h7) {
            textBoxData += L"7";
        }
        if ((HWND)lParam == h8) {
            textBoxData += L"8";
        }
        if ((HWND)lParam == h9) {
            textBoxData += L"9";
        }
        if ((HWND)lParam == h0) {
            textBoxData += L"0";
        }
        if ((HWND)lParam == hAdd) {
            textBoxData += L"+";
        }
        if ((HWND)lParam == hSubtract) {
            textBoxData += L"-";
        }
        if ((HWND)lParam == hMultiply) {
            textBoxData += L"*";
        }
        if ((HWND)lParam == hDivide) {
            textBoxData += L"/";
        }
        if ((HWND)lParam == hClear) {
            textBoxData = L"";
        }
        if ((HWND)lParam == hEquals) {
            textBoxData = Calculate(textBoxData.c_str());
        }
        if ((HWND)lParam == hPar) {
            if (parMode) {
                textBoxData += L"(";
                parMode = false;
            }
            else {
                textBoxData += L")";
                parMode = true;
            }
        }
        if ((HWND)lParam == hDecimal) {
            textBoxData += L".";
        }
        SetWindowTextW(hTextBox, textBoxData.c_str());
        break;
    case WM_CREATE:
        CreateWindowW(L"static", L"Calculator App", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 0, 409, 25, hWnd, nullptr, nullptr, nullptr);
        hTextBox = CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | SS_RIGHT | WS_BORDER, 0, 25, 409, 20, hWnd, nullptr, nullptr, nullptr);
        h7 = CreateWindowW(L"button", L"7", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 45, 102, 75, hWnd, nullptr, nullptr, nullptr);
        h8 = CreateWindowW(L"button", L"8", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 102, 45, 102, 75, hWnd, nullptr, nullptr, nullptr);
        h9 = CreateWindowW(L"button", L"9", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 204, 45, 102, 75, hWnd, nullptr, nullptr, nullptr);
        hAdd = CreateWindowW(L"button", L"+", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 306, 45, 103, 75, hWnd, nullptr, nullptr, nullptr);
        h4 = CreateWindowW(L"button", L"4", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 120, 102, 75, hWnd, nullptr, nullptr, nullptr);
        h5 = CreateWindowW(L"button", L"5", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 102, 120, 102, 75, hWnd, nullptr, nullptr, nullptr);
        h6 = CreateWindowW(L"button", L"6", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 204, 120, 102, 75, hWnd, nullptr, nullptr, nullptr);
        hSubtract = CreateWindowW(L"button", L"-", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 306, 120, 103, 75, hWnd, nullptr, nullptr, nullptr);
        h1 = CreateWindowW(L"button", L"1", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 195, 102, 75, hWnd, nullptr, nullptr, nullptr);
        h2 = CreateWindowW(L"button", L"2", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 102, 195, 102, 75, hWnd, nullptr, nullptr, nullptr);
        h3 = CreateWindowW(L"button", L"3", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 204, 195, 102, 75, hWnd, nullptr, nullptr, nullptr);
        hMultiply = CreateWindowW(L"button", L"*", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 306, 195, 103, 75, hWnd, nullptr, nullptr, nullptr);
        hPar = CreateWindowW(L"button", L"( )", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 270, 102, 75, hWnd, nullptr, nullptr, nullptr);
        h0 = CreateWindowW(L"button", L"0", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 102, 270, 102, 75, hWnd, nullptr, nullptr, nullptr);
        hDecimal = CreateWindowW(L"button", L".", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 204, 270, 102, 75, hWnd, nullptr, nullptr, nullptr);
        hDivide = CreateWindowW(L"button", L"/", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 306, 270, 103, 75, hWnd, nullptr, nullptr, nullptr);
        hClear = CreateWindowW(L"button", L"Clear", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, 345, 204, 75, hWnd, nullptr, nullptr, nullptr);
        hEquals = CreateWindowW(L"button", L"=", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 204, 345, 205, 75, hWnd, nullptr, nullptr, nullptr);
    case WM_PAINT:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
//this function exists obly to save me headaches
int FindDifference(double num1, double num2) { return abs(num1 - num2) + 1; }; //the + 1 is really ugly but it is what it is
enum PEMDAS {
    PARENTHESIS,
    EXPONENTS,
    MULTIPLY,
    DIVIDE,
    ADD,
    SUBTRACT
};
//this handles calculations
std::wstring Calculate(LPCWSTR inputWString) {
    std::wstring ws(inputWString);
    std::wstring numbersRef = L"1234567890.";
    int posPar1 = NULL;                         //tracks position of '('
    int posPar2 = NULL;                         //tracks position of ')'
    int posOperator = 0;                        //tracks the position of '+', '-', '*', or '/'
    int posNum1 = NULL;                         //tracks the position of the first digit of the left number
    int posNum2 = NULL;                         //tracks the position of the last digit of the right number
    double number1 = NULL;
    double number2 = NULL;
    bool changedSomething = false;
    for (int i = 0; i < 6; i++) {
        switch (i) {
        case PARENTHESIS:
            while (ws.find(L'(') != -1) {
                posPar1 = ws.find(L'(');
                if (posPar1 == -1) {
                    break;
                }
                else {
                    posPar2 = ws.find(L')');
                    if (posPar2 == -1) {
                        return L"SYNTAX ERROR: UNCLOSED PARENTHESES";
                    }
                    else {
                        while (true) {
                            int temp = ws.find(L'(', posPar1);
                            if (temp > posPar1 && temp < posPar2) {
                                posPar1 = temp;
                            }
                            else {
                                break;
                            }
                        }
                        ws.replace(posPar1, FindDifference(posPar1, posPar2), Calculate(ws.substr(posPar1 + 1, posPar2 - posPar1 - 1).c_str()));
                        break;
                    }
                }
            }
            break;
        case EXPONENTS:
            if (changedSomething) posOperator = 0;
            changedSomething = false;
            break;
        case MULTIPLY:
            while (ws.find(L'*', posOperator) != -1) {
                posOperator = ws.find(L'*');
                //find the left-side number
                for (int i = posOperator; i >= 0; i--) {
                    if (i == 0) {
                        posNum1 = i;
                        break;
                    }
                    if (numbersRef.find(ws[i - 1]) == -1) {
                        posNum1 = i;
                        break;
                    }
                }
                //find the right-side number
                for (int i = posOperator; i < ws.length(); i++) {
                    if (i == ws.length() - 1) {
                        posNum2 = i;
                        break;
                    }
                    if (numbersRef.find(ws[i + 1]) == -1) {
                        posNum2 = i;
                        break;
                    }
                }
                number1 = std::stod(ws.substr(posNum1, FindDifference(posNum1, posOperator)));
                number2 = std::stod(ws.substr(posOperator + 1, FindDifference(posOperator + 1, posNum2)));
                ws.replace(posNum1, FindDifference(posNum1, posNum2), std::to_wstring(number1 * number2));
                changedSomething = true;
            }
            if (changedSomething) posOperator = 0;
            changedSomething = false;
            break;
        case DIVIDE:
            while (ws.find(L'/', posOperator) != -1) {
                posOperator = ws.find(L'/');
                //find the left-side number
                for (int i = posOperator; i >= 0; i--) {
                    if (i == 0) {
                        posNum1 = i;
                        break;
                    }
                    if (numbersRef.find(ws[i - 1]) == -1) {
                        posNum1 = i;
                        break;
                    }
                }
                //find the right-side number
                for (int i = posOperator; i < ws.length(); i++) {
                    if (i == ws.length() - 1) {
                        posNum2 = i;
                        break;
                    }
                    if (numbersRef.find(ws[i + 1]) == -1) {
                        posNum2 = i;
                        break;
                    }
                }
                number1 = std::stod(ws.substr(posNum1, FindDifference(posNum1, posOperator)));
                number2 = std::stod(ws.substr(posOperator + 1, FindDifference(posOperator + 1, posNum2)));
                ws.replace(posNum1, FindDifference(posNum1, posNum2), std::to_wstring(number1 / number2));
                changedSomething = true;
            }
            if (changedSomething) posOperator = 0;
            changedSomething = false;
            break;
        case ADD:
            while (ws.find(L'+', posOperator) != -1) {
                posOperator = ws.find(L'+');
                //find the left-side number
                for (int i = posOperator; i >= 0; i--) {
                    if (i == 0) {
                        posNum1 = i;
                        break;
                    }
                    if (numbersRef.find(ws[i - 1]) == -1) {
                        posNum1 = i;
                        break;
                    }
                }
                //find the right-side number
                for (int i = posOperator; i < ws.length(); i++) {
                    if (i == ws.length() - 1) {
                        posNum2 = i;
                        break;
                    }
                    if (numbersRef.find(ws[i + 1]) == -1) {
                        posNum2 = i;
                        break;
                    }
                }
                number1 = std::stod(ws.substr(posNum1, FindDifference(posNum1, posOperator)));
                number2 = std::stod(ws.substr(posOperator + 1, FindDifference(posOperator + 1, posNum2)));
                ws.replace(posNum1, FindDifference(posNum1, posNum2), std::to_wstring(number1 + number2));
                changedSomething = true;
            }
            if (changedSomething) posOperator = 0;
            changedSomething = false;
            break;
        case SUBTRACT:
            while (ws.find(L'-', posOperator) != -1 && ws.find(L'-', 1) != -1) {
                posOperator = ws.find(L'-');
                //find the left-side number
                for (int i = posOperator; i >= 0; i--) {
                    if (i == 0) {
                        posNum1 = i;
                        break;
                    }
                    if (numbersRef.find(ws[i - 1]) == -1) {
                        posNum1 = i;
                        break;
                    }
                }
                //find the right-side number
                for (int i = posOperator; i < ws.length(); i++) {
                    if (i == ws.length() - 1) {
                        posNum2 = i;
                        break;
                    }
                    if (numbersRef.find(ws[i + 1]) == -1) {
                        posNum2 = 1;
                        break;
                    }
                }
                number1 = std::stod(ws.substr(posNum1, FindDifference(posNum1, posOperator)));
                number2 = std::stod(ws.substr(posOperator + 1, FindDifference(posOperator + 1, posNum2)));
                ws.replace(posNum1, FindDifference(posNum1, posNum2), std::to_wstring(number1 - number2));
                changedSomething = true;
            }
            if (changedSomething) posOperator = 0;
            changedSomething = false;
            break;
        }

    }
    return ws;
}