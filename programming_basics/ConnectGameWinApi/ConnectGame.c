///////////////////////////////////////////////////////////////////////////////////////
// Author: Amin Allam

///////////////////////////////////////////////////////////////////////////////////////
// From CodeBlocks
// File -> New -> Project -> Win32 GUI project
// Replace the code of main.cpp with the following code

///////////////////////////////////////////////////////////////////////////////////////
// From command line (with mingw installed):
// add these libraries (-l):
// gdi32 user32 kernel32

///////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>

///////////////////////////////////////////////////////////////////////////////////////

#define EMPTY_CHAR '-'
#define PLAY1_CHAR 'x'
#define PLAY2_CHAR 'o'
#define BORDER_CHAR '#'

typedef struct
{
	char**	board;
	int		n, m, r;
	char	cur_player;

	int		num;
	int*	num_col;
	int		last_play_col;
}
ConnectGame;

///////////////////////////////////////////////////////////////////////////////////////

void InitializeGame(ConnectGame* g, int cn, int cm, int cr)
{
	int i,j;
	g->n=cn; g->m=cm; g->r=cr;

	g->board=(char**)malloc((g->n+2)*sizeof(char*));
	for(i=0;i<g->n+2;i++) g->board[i]=(char*)malloc((g->m+2)*sizeof(char));

	for(i=0;i<g->n+2;i++) for(j=0;j<g->m+2;j++) g->board[i][j]=BORDER_CHAR;
	for(i=1;i<=g->n;i++) for(j=1;j<=g->m;j++) g->board[i][j]=EMPTY_CHAR;

	g->num=0;
	g->num_col=(int*)malloc((g->m)*sizeof(int));
	for(i=0;i<g->m;i++) g->num_col[i]=0;

	g->last_play_col=-1;
	g->cur_player=PLAY1_CHAR;
}

///////////////////////////////////////////////////////////////////////////////////////

void DestroyGame(ConnectGame* g)
{
	int i;
	for(i=0;i<g->n+2;i++) free(g->board[i]);
	free(g->board);
	free(g->num_col);
}

///////////////////////////////////////////////////////////////////////////////////////

BOOL IsFull(ConnectGame* g)
{
	return g->num==g->n*g->m;
}

///////////////////////////////////////////////////////////////////////////////////////

BOOL PlayTurn(ConnectGame* g, int col)
{
	if(col<0 || col>=g->m || g->num_col[col]==g->n)
		return FALSE;
	g->board[g->num_col[col]+1][col+1]=g->cur_player;
	g->num_col[col]++;
	g->num++;
	g->last_play_col=col;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////

char IsWinning(ConnectGame* g)
{
	int k,p,q;
	if(g->last_play_col<0) return EMPTY_CHAR;

	int di[]={0,1,1,-1};
	int dj[]={1,0,1,1};

	int sti=g->num_col[g->last_play_col];
	int stj=g->last_play_col+1;
	char c=g->board[sti][stj];

	for(p=0;p<4;p++)
	{
		int cnt=1;

		for(q=1;q>=-1;q-=2)
		{
			int ci=sti, cj=stj;
			for(k=1;k<g->r;k++)
			{
				ci+=q*di[p]; cj+=q*dj[p];
				if(g->board[ci][cj]!=c) break;
				cnt++;
			}
		}

		if(cnt>=g->r) return c;
	}

	return EMPTY_CHAR;
}

///////////////////////////////////////////////////////////////////////////////////////

void PrintCell(HDC hdc, ConnectGame* g, int stx, int sty, int totw, int toth, int i, int j)
{
	int h=toth/g->n;
	int w=totw/g->m;

	int y=sty+(g->n-i)*h;
	int x=stx+(j-1)*w;

	int mx=2, my=2;

	HBRUSH hBrush;
	if(g->board[i][j]==EMPTY_CHAR) hBrush = CreateSolidBrush (RGB (255, 255, 255));
	else if(g->board[i][j]==PLAY1_CHAR) hBrush = CreateSolidBrush (RGB (255, 255, 0));
	else if(g->board[i][j]==PLAY2_CHAR) hBrush = CreateSolidBrush (RGB (255, 0, 0));

	SelectObject (hdc, hBrush);
	Ellipse(hdc, x+mx, y+my, x+w-2*mx, y+h-2*my) ;
	DeleteObject (hBrush);
}

///////////////////////////////////////////////////////////////////////////////////////

void PrintGame(HDC hdc, ConnectGame* g, int stx, int sty, int totw, int toth)
{
	int i,j;

	int h=toth/g->n;
	int w=totw/g->m;

	HBRUSH hBrushBlue = CreateSolidBrush (RGB (0, 0, 255)) ;
	HBRUSH hBrushWhite = CreateSolidBrush (RGB (255, 255, 255)) ;
	HBRUSH hBrushYellow = CreateSolidBrush (RGB (255, 255, 0)) ;
	HBRUSH hBrushRed = CreateSolidBrush (RGB (255, 0, 0)) ;

	int mx=2, my=2;

	for(i=1;i<=g->n;i++)
	{
		for(j=1;j<=g->m;j++)
		{
			int y=sty+(g->n-i)*h;
			int x=stx+(j-1)*w;

			SelectObject(hdc, hBrushBlue);
			Rectangle(hdc, x, y, x+w, y+h) ;

			if(g->board[i][j]==EMPTY_CHAR) SelectObject (hdc, hBrushWhite);
			else if(g->board[i][j]==PLAY1_CHAR) SelectObject (hdc, hBrushYellow);
			else if(g->board[i][j]==PLAY2_CHAR) SelectObject (hdc, hBrushRed);

			Ellipse(hdc, x+mx, y+my, x+w-2*mx, y+h-2*my) ;
		}
	}

	DeleteObject (hBrushBlue) ;
	DeleteObject (hBrushWhite) ;
	DeleteObject (hBrushYellow) ;
	DeleteObject (hBrushRed) ;
}

///////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[]=TEXT("ConnectGame");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if(!RegisterClass(&wndclass))
	{
		MessageBox (NULL, TEXT("This program requires Windows NT!"),
		szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(
				szAppName,
				TEXT("Connect Game"),
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				CW_USEDEFAULT,
				NULL,
				NULL,
				hInstance,
				NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

///////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	int n,m,r;
}
DialogBoxData;

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static ConnectGame game;

	static int cxClient, cyClient;
	static HINSTANCE hInstance;
	static int cxChar, cyChar;
	static DialogBoxData dbd;

	int stx=10, sty=20;
	int xdlg=26, ydlg=12; // in chars

	char a[sizeof(DLGTEMPLATE)+3*sizeof(WORD)]={0};
	DLGTEMPLATE* t=(DLGTEMPLATE*)a;

	HDC hdc;
	HWND hdlg;
	PAINTSTRUCT ps;
	TEXTMETRIC tm;

	int x, y, cur_col;

	switch(message)
	{
	case WM_CREATE:
		hInstance = ((LPCREATESTRUCT) lParam)->hInstance ;

		hdc = GetDC (hwnd) ;
		SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;
		GetTextMetrics (hdc, &tm) ;
		cxChar = tm.tmAveCharWidth ;
		cyChar = tm.tmHeight + tm.tmExternalLeading ;
		ReleaseDC (hwnd, hdc) ;

		t->cdit=0;
		t->style=DS_MODALFRAME|DS_SETFOREGROUND;
		t->x=0;
		t->y=0;
		t->cx=xdlg*4;
		t->cy=ydlg*8;
		//DialogBoxIndirect(hInstance, t, hwnd, DlgProc);
		DialogBoxIndirectParam(hInstance, t, hwnd, DlgProc, (LPARAM)&dbd);

		InitializeGame(&game, dbd.n, dbd.m, dbd.r);
		//InitializeGame(&game, 6, 7, 4);
		return 0;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;

	case WM_TIMER:
		return 0;

	case WM_RBUTTONUP:
		return 0;

	case WM_LBUTTONUP:
		x = LOWORD (lParam);
		y = HIWORD (lParam);
		//cur_col=x/(cxClient/game.m);
		if(x<stx) cur_col=-1;
		else cur_col=((x-stx)*game.m)/(cxClient-2*stx);

		if(PlayTurn(&game, cur_col))
		{
			MessageBeep(MB_ICONHAND);

			//InvalidateRect(hwnd, NULL, FALSE);

			hdc = GetDC (hwnd) ;
			PrintCell(hdc, &game, stx, sty, cxClient-2*stx, cyClient-2*sty, game.num_col[game.last_play_col], game.last_play_col+1);
			ReleaseDC (hwnd, hdc) ;

			if(IsWinning(&game)==game.cur_player)
			{
				if(game.cur_player==PLAY1_CHAR) MessageBox(hwnd, "Player 1 Wins!", "Connect Game", MB_OK);
				else MessageBox(hwnd, "Player 2 Wins!", "Connect Game", MB_OK);
				SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
			else if(IsFull(&game))
			{
				MessageBox(hwnd, "Game Draw!", "Connect Game", MB_OK);
				SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
			else
			{
				if(game.cur_player==PLAY1_CHAR) game.cur_player=PLAY2_CHAR;
				else game.cur_player=PLAY1_CHAR;
			}
		}
		else
		{
			MessageBox(hwnd, "Invalid Position!", "Connect Game", MB_OK);
		}
		return 0;

	case WM_PAINT:
		hdc=BeginPaint(hwnd, &ps);
		PrintGame(hdc, &game, stx, sty, cxClient-2*stx, cyClient-2*sty);
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		DestroyGame(&game);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////////////

int StrToNum(LPTSTR s)
{
	int n=0;
	while(*s)
	{
		TCHAR c=*s;
		if(c<TEXT('0') || c>TEXT('9')) return -1;
		n=n*10+c-TEXT('0');
		s++;
	}
	return n;
}

///////////////////////////////////////////////////////////////////////////////////////


BOOL CALLBACK DlgProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxClient, cyClient;
	static HINSTANCE hInstance;
	static int cxChar, cyChar;
	static HWND hstN, hstM, hstR;
	static HWND hedN, hedM, hedR;
	static HWND hbutOk;
	static DialogBoxData* pdbd;

	int n,m,r;
	TCHAR buf[101];

	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;

	// Dialog units: 102, 		   122
	// Pixels:       102*4/cxChar, 122*8/cyChar

	// There is no WM_CREATE message sent to DlgProc

	switch(message)
	{
	case WM_INITDIALOG:
		hInstance=(HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		pdbd=(DialogBoxData*)lParam;

		hdc = GetDC (hwnd) ;
		SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;
		GetTextMetrics (hdc, &tm) ;
		cxChar = tm.tmAveCharWidth ;
		cyChar = tm.tmHeight + tm.tmExternalLeading ;
		ReleaseDC (hwnd, hdc) ;

		SetWindowText(hwnd, TEXT("Game Parameters"));

		hstN = CreateWindow (TEXT ("static"), TEXT ("Num Rows"),
		WS_CHILD | WS_VISIBLE | WS_GROUP | SS_LEFT,
		2*cxChar, 2*cyChar, 15*cxChar, 6+cyChar,
		hwnd, NULL, hInstance, NULL);

		hedN = CreateWindow (TEXT ("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | WS_BORDER,
		15*cxChar, 2*cyChar, 6*cxChar, 6+cyChar,
		hwnd, NULL, hInstance, NULL);

		hstM = CreateWindow (TEXT ("static"), TEXT ("Num Columns"),
		WS_CHILD | WS_VISIBLE | WS_GROUP | SS_LEFT,
		2*cxChar, 4*cyChar, 15*cxChar, 6+cyChar,
		hwnd, NULL, hInstance, NULL);

		hedM = CreateWindow (TEXT ("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | WS_BORDER,
		15*cxChar, 4*cyChar, 6*cxChar, 6+cyChar,
		hwnd, NULL, hInstance, NULL);

		hstR = CreateWindow (TEXT ("static"), TEXT ("Num Adjacent"),
		WS_CHILD | WS_VISIBLE | WS_GROUP | SS_LEFT,
		2*cxChar, 6*cyChar, 15*cxChar, 6+cyChar,
		hwnd, NULL, hInstance, NULL);

		hedR = CreateWindow (TEXT ("edit"), NULL,
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | WS_BORDER,
		15*cxChar, 6*cyChar, 6*cxChar, 6+cyChar,
		hwnd, NULL, hInstance, NULL);

		hbutOk = CreateWindow (TEXT ("button"), TEXT ("OK"),
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
		8*cxChar, 17*cyChar/2, 10*cxChar, 6+cyChar,
		hwnd, (HMENU)IDOK, hInstance, NULL);

		return FALSE;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			GetWindowText(hedN, buf, 100);
			n=StrToNum(buf);
			GetWindowText(hedM, buf, 100);
			m=StrToNum(buf);
			GetWindowText(hedR, buf, 100);
			r=StrToNum(buf);

			if(n>0 && n<100 && m>0 && m<100 && r>0 && r<100)
			{
				pdbd->n=n; pdbd->m=m; pdbd->r=r;
				EndDialog(hwnd, TRUE);
			}
			return TRUE ;
		}
		break;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////
