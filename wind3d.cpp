#include "wind3d.h"

const wchar_t CLASS_NAME[] = L"__WC_D3DLAB__";
const int STYLE_BITS = WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_VISIBLE | WS_SYSMENU;


LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

HWND WinD3DCreate(const wchar_t * title, int width, int height)
{
    HINSTANCE hInstance = GetModuleHandleW(nullptr);

    WNDCLASSW wc;

    wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassW(&wc)) {        
        return NULL;
    }

    RECT rc {0,0, width, height};

    AdjustWindowRect(&rc,STYLE_BITS,FALSE);

    int cli_width = rc.right - rc.left;
    int cli_height = rc.bottom - rc.top;

    HWND hWnd = CreateWindowW(CLASS_NAME,title,STYLE_BITS, CW_USEDEFAULT,CW_USEDEFAULT,
    cli_width,cli_height,NULL,NULL,hInstance,nullptr);
    if (hWnd != NULL) {
        ShowWindow(hWnd,SW_SHOW);
        UpdateWindow(hWnd);
    }

    return hWnd;

}


LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch(Msg) {
        case WM_DESTROY:
        PostQuitMessage(0);
        break;
        default: return DefWindowProcW(hWnd,Msg,wParam,lParam);
    }

    return 0L;
}