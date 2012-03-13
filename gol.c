#include <windows.h>

const char g_szClassName[] = "myWindowClass";
const int MOVE_DELTA = 2;
const int CELL_SIZE = 5;
const int ID_TIMER = 1;

typedef struct _CELL {
  int x;
  int y;

  int dx;
  int dy;
}CELL;

CELL g_cell;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
void UpdateCell(RECT* rect);
void DrawCell(HDC hdc);


void UpdateCell(RECT* rect) {
  g_cell.x += g_cell.dx;
  g_cell.y += g_cell.dy;

  if(g_cell.x < 0) {
    g_cell.x = 0;
    g_cell.dx = MOVE_DELTA;
  } else if (g_cell.x + CELL_SIZE > rect->right) {
    g_cell.x = rect->right - CELL_SIZE;
    g_cell.dx = -MOVE_DELTA;
  }

  if(g_cell.y < 0) {
    g_cell.y = 0;
    g_cell.dy = MOVE_DELTA;
  } else if (g_cell.y + CELL_SIZE > rect->bottom) {
    g_cell.y = rect->bottom - CELL_SIZE;
    g_cell.dy = -MOVE_DELTA;
  }
}
void DrawCell(HDC hdc) {
  RECT rect;
  rect.left = g_cell.x;
  rect.top = g_cell.y;
  rect.right = g_cell.x + CELL_SIZE;
  rect.bottom = g_cell.y + CELL_SIZE;

  FillRect(hdc, &rect, CreateSolidBrush(RGB(197,0,11)));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  WNDCLASSEX wc;
  HWND hwnd;
  MSG Msg;

  wc.cbSize        = sizeof(WNDCLASSEX);
  wc.style         = 0;
  wc.lpfnWndProc   = WndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hInstance;
  wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wc.lpszMenuName  = NULL;
  wc.lpszClassName = g_szClassName;
  wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

  if(!RegisterClassEx(&wc)) {
    MessageBox(NULL, "Window Registration Failed!", "Error!",
        MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  hwnd = CreateWindowEx(
      WS_EX_CLIENTEDGE,
      g_szClassName,
      "Conway's Game of Life",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 800, 640,
      NULL, NULL, hInstance, NULL);

  if(hwnd == NULL) {
    MessageBox(NULL, "Window Creation Failed!", "Error!",
        MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  while(GetMessage(&Msg, NULL, 0, 0) > 0) {
    TranslateMessage(&Msg);
    DispatchMessage(&Msg);
  }
  return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch(msg) {
    case WM_CREATE: 
      {
			  UINT ret;

        g_cell.dx = MOVE_DELTA;
        g_cell.dy = MOVE_DELTA;

			  ret = SetTimer(hwnd, ID_TIMER, 50, NULL);
			  if(ret == 0)
				  MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);
      }
    case WM_TIMER:
      {
        HDC hdc;
        RECT rcClient;
        hdc = GetDC(hwnd);

        GetClientRect(hwnd, &rcClient);
        UpdateCell(&rcClient);
        DrawCell(hdc);

        // Clear the screen so there is no trace of the last iteration
        InvalidateRect(hwnd,NULL,TRUE);

        ReleaseDC(hwnd, hdc);
        break;
      }
    case WM_PAINT:
      {
        HDC hdc;
        PAINTSTRUCT ps;
        RECT rcClient;
        hdc = BeginPaint(hwnd, &ps);

        GetClientRect(hwnd, &rcClient);
        DrawCell(hdc);

        EndPaint(hwnd, &ps);
        break;
      }
    case WM_CLOSE:
      DestroyWindow(hwnd);
      break;
    case WM_DESTROY:
			KillTimer(hwnd, ID_TIMER);
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}
