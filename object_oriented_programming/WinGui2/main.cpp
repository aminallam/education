#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include "shape.h"

ShapeGame game;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("MovingShapes");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG msg;                 /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    //wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Moving Shapes"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&msg, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&msg);
        /* Send message to WindowProcedure */
        DispatchMessage(&msg);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return msg.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int cxClient, cyClient;
    int x, y; double a;
    char arr[10];
    HDC hdc;
    RECT rect;
    PAINTSTRUCT ps;

    switch (message)                  /* handle the messages */
    {
        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            break;
        case WM_PAINT:
            hdc = BeginPaint (hwnd, &ps) ;
            //GetClientRect (hwnd, &rect) ;
            game.DrawShapes(hdc);
            EndPaint (hwnd, &ps) ;
            break;
        case WM_RBUTTONUP:
            hdc = GetDC (hwnd) ;
            x = LOWORD (lParam);
            y = HIWORD (lParam);
            game.AddDrawShape(hdc, x, y);
            ReleaseDC (hwnd, hdc) ;
            break;
        case WM_LBUTTONDOWN:
            x = LOWORD (lParam);
            y = HIWORD (lParam);
            game.StartMove(x, y);
            break;
        case WM_LBUTTONUP:
            x = LOWORD (lParam);
            y = HIWORD (lParam);
            game.EndMove(x, y);
            InvalidateRect (hwnd, NULL, TRUE) ;
            break;
        case WM_KEYDOWN:
            switch(wParam)
            {
            case VK_DELETE:
                game.DestroyShapes();
                InvalidateRect (hwnd, NULL, TRUE) ;
                break;
            case VK_RETURN:
                a=game.GetArea();
                sprintf(arr, "%lf", a);
                MessageBox(hwnd, arr, "Area", MB_OK);
                break;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
