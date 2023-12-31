// NYR.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "NYR.h"

#include "Mmsystem.h"

#include <string>
#include <algorithm>
#include <vector>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HHOOK hKeyboardHook;

static std::string global_buffer = "";

static std::vector<std::string> xd = { "arse",
"arsehead",
"arsehole",
"ass",
"asshole",
"bastard",
"bitch",
"bloody",
"bollocks",
"brotherfucker",
"bugger",
"bullshit",
"child-fucker",
"Christ on a bike",
"Christ on a cracker",
"cock",
"cocksucker",
"crap",
"cunt",
"cyka blyat",
"damn",
"damn it",
"dick",
"dickhead",
"dyke",
"fatherfucker",
"frigger",
"fuck",
"goddamn",
"godsdamn",
"hell",
"holy shit",
"horseshit",
"in shit",
"Jesus fuck",
"Jesus wept",
"kike",
"motherfucker",
"nigga",
"nigra",
"pigfucker",
"piss",
"prick",
"pussy",
"shit",
"shit ass",
"shite",
"sisterfucker",
"slut",
"son of a whore",
"son of a bitch",
"spastic",
"sweet Jesus",
"turd",
"twat",
"wanker" };

static void ShowWarning() {
    HWND desktop = GetDesktopWindow();
    PAINTSTRUCT ps{};
    HDC hdc = GetDC(desktop);
    HFONT hFont = CreateFont(700, 60, 0, 0, FW_DONTCARE,
        FALSE, FALSE, FALSE, ANSI_CHARSET,
        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH,
        TEXT("Arial Black")/*"SYSTEM_FIXED_FONT"*/);

    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont); // <-- add this

    static constexpr wchar_t lol[] = L"DONT BE TOXIC YOU RETARD";

    SetTextColor(hdc, RGB(255, 0, 0));
    TextOut(hdc, 100, 100, lol, sizeof(lol)/sizeof(lol[0]));
    SelectObject(hdc, hOldFont); // <-- add this
    DeleteObject(hFont);  // <-- add this
    // TODO: Add any drawing code that uses hdc here...
    EndPaint(desktop, &ps);

    PlaySound(L"X:\\Downloads\\DX-Ball\\DX-Ball\\Orchestr.wav", NULL, SND_ASYNC | SND_FILENAME);
}

static void CheckForToxicity() {
    std::transform(global_buffer.begin(), global_buffer.end(), global_buffer.begin(), [](char ch) -> char {
        return std::tolower(ch);
        });

    for (auto& lol : xd) {
        std::transform(lol.begin(), lol.end(), lol.begin(), [](char ch) -> char {
            return std::tolower(ch);
            });
        if (global_buffer.find(lol, 0) != global_buffer.npos) {
            ShowWarning();
            global_buffer.clear();
        }
    }

    if (global_buffer.size() > 2000) {
        global_buffer.erase(global_buffer.begin());
    }
}

__declspec(dllexport) LRESULT CALLBACK KeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
    if ((nCode == HC_ACTION) && ((wParam == WM_SYSKEYDOWN) || (wParam == WM_KEYDOWN)))
    {
        KBDLLHOOKSTRUCT hooked_key = *((KBDLLHOOKSTRUCT*)lParam);
        DWORD dwMsg = 1;
        dwMsg += hooked_key.scanCode << 16;
        dwMsg += hooked_key.flags << 24;

        int key = hooked_key.vkCode;
        // char xd[2] = { (char)key, '\0' };

        if (key == VK_BACK && global_buffer.empty() == false) {
            global_buffer.pop_back();
        }
        else
            global_buffer += static_cast<char>(key);

        CheckForToxicity();
    }

    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

// Forward declarations of functions included in this code module:
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

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_NYR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NYR));

    MSG msg;

    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardEvent, hInstance, NULL);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NYR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_NYR);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      10, 10, 150, 100, nullptr, nullptr, hInstance, nullptr);

   SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

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
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            RECT r{ 10,10,40,40 };
            DrawText(hdc, L"TOXICITY CHECKER", 14, &r, DT_NOCLIP);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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
