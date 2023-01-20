#include <windows.h>
#include <windowsx.h>
#include <string>
#include "resource.h"

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

HINSTANCE hInst;
HICON hIconX, hIconO;

int numXWins = 0;
int numOWins = 0;
int numDraw = 0;
bool draw = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static char board[3][3] = { { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' } };
    static bool player1Turn = true;
    static bool gameOver = false;


    switch (message)
    {
        case WM_CREATE:
        {
            RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
            AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
            SetWindowPos(hwnd, HWND_TOP, 100, 100, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);

            numXWins = GetPrivateProfileInt("Wins", "X", 0, ".\\save.ini");
            numOWins = GetPrivateProfileInt("Wins", "O", 0, ".\\save.ini");
            numDraw = GetPrivateProfileInt("Wins", "D", 0, ".\\save.ini");

            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // lentos piešimas
            HBRUSH brush = CreateSolidBrush(RGB(122, 122, 122));
            SelectObject(hdc, brush);
            Rectangle(hdc, 50, 50, 150, 150);
            Rectangle(hdc, 200, 50, 300, 150);
            Rectangle(hdc, 350, 50, 450, 150);
            Rectangle(hdc, 50, 200, 150, 300);
            Rectangle(hdc, 200, 200, 300, 300);
            Rectangle(hdc, 350, 200, 450, 300);
            Rectangle(hdc, 50, 350, 150, 450);
            Rectangle(hdc, 200, 350, 300, 450);
            Rectangle(hdc, 350, 350, 450, 450);
            DeleteObject(brush);

            std::string xWinsStr = "X wins: " + std::to_string(numXWins);
            TextOutA(hdc, 50, 10, xWinsStr.c_str(), xWinsStr.length());
            std::string oWinsStr = "O wins: " + std::to_string(numOWins);
            TextOutA(hdc, 200, 10, oWinsStr.c_str(), oWinsStr.length());
            std::string DrawStr = "Draws: " + std::to_string(numDraw);
            TextOutA(hdc, 350, 10, DrawStr.c_str(), DrawStr.length());

            EndPaint(hwnd, &ps);

            hdc = GetDC(hwnd);

            // simbolių piešimas
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (board[i][j] == 'X')
                    {
                        hIconX = (HICON)LoadIcon(hInst, MAKEINTRESOURCE(IDI_X_ICO));
                        DrawIconEx(hdc, 50 + i * 150, 50 + j * 150, hIconX, 100, 100, 0, NULL, DI_NORMAL);
                        DestroyIcon(hIconX);
                    }
                    else if (board[i][j] == 'O')
                    {
                        hIconO = (HICON)LoadIcon(hInst, MAKEINTRESOURCE(IDI_O_ICO));
                        DrawIconEx(hdc, 50 + i * 150, 50 + j * 150, hIconO, 100, 100, 0, NULL, DI_NORMAL);
                        DestroyIcon(hIconO);
                    }
                }
            }

            // žaidimo baigtis
            if (gameOver)
            {
                std::string message;
                if (player1Turn)
                {
                    message = "Player 1 wins!";
                    numXWins ++;

                }
                else if (draw)
                {
                    message = "Draw!";
                    numDraw ++;
                    draw = false;
                    UpdateWindow(hwnd);
                }
                else
                {
                    message = "Player 2 wins!";
                    numOWins++;
                    UpdateWindow(hwnd);
                }

                std::string xWinsStr = "X wins: " + std::to_string(numXWins);
                TextOutA(hdc, 50, 10, xWinsStr.c_str(), xWinsStr.length());
                std::string oWinsStr = "O wins: " + std::to_string(numOWins);
                TextOutA(hdc, 200, 10, oWinsStr.c_str(), oWinsStr.length());
                std::string DrawStr = "Draws: " + std::to_string(numDraw);
                TextOutA(hdc, 350, 10, DrawStr.c_str(), DrawStr.length());

                UpdateWindow(hwnd);
                TextOutA(hdc, 50, 460, message.c_str(), message.length());
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            if (gameOver)
            {
                break;
            }

            // pelės pozicijos gavimas
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            int i = xPos / 150;
            int j = yPos / 150;

            // simbolių dėjimas į lentos matricą
            if (board[i][j] == ' ')
            {
                board[i][j] = (player1Turn ? 'X' : 'O');

                // tikrinimas ar kas nors laimėjo
                if
                ((board[0][0] != ' ' && board[0][0] == board[1][1] && board[0][0] == board[2][2]) ||
                 (board[2][0] != ' ' && board[2][0] == board[1][1] && board[2][0] == board[0][2]) ||
                 (board[0][0] != ' ' && board[0][0] == board[0][1] && board[0][0] == board[0][2]) ||
                 (board[1][0] != ' ' && board[1][0] == board[1][1] && board[1][0] == board[1][2]) ||
                 (board[2][0] != ' ' && board[2][0] == board[2][1] && board[2][0] == board[2][2]) ||
                 (board[0][0] != ' ' && board[0][0] == board[1][0] && board[0][0] == board[2][0]) ||
                 (board[0][1] != ' ' && board[0][1] == board[1][1] && board[0][1] == board[2][1]) ||
                 (board[0][2] != ' ' && board[0][2] == board[1][2] && board[0][2] == board[2][2]))
                {
                    gameOver = true;
                }
                else
                {
                    player1Turn = !player1Turn;
                }
            }

            // lygiųjų tikrinimas
            int counter = 0;

            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (board[i][j] != ' ')
                    {
                        counter++;
                    }
                }
            }

            if (counter == 9)
            {
                draw = true;
                gameOver = true;
            }

            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            switch(wmId)
            {
                // New Game implementacija
                case IDM_NG:
                {
                    player1Turn = true;
                    gameOver = false;
                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            board[i][j] = ' ';
                        }
                    }
                    InvalidateRect(hwnd, NULL, TRUE);
                    UpdateWindow(hwnd);
                    break;
                }
                // Save Game implementacija
                case IDM_SG:
                {
                    char buffer[64];
                    printf("%d %d", numXWins, numOWins);
                    sprintf_s(buffer, "%d", numXWins);
                    WritePrivateProfileStringA("Wins", "X", buffer, ".\\save.ini");
                    sprintf_s(buffer, "%d", numOWins);
                    WritePrivateProfileStringA("Wins", "O", buffer, ".\\save.ini");
                    sprintf_s(buffer, "%d", numDraw);
                    WritePrivateProfileStringA("Wins", "D", buffer, ".\\save.ini");
                }
                // Reset Wins implementacija
                case IDM_RW:
                {
                    numXWins = 0;
                    numOWins = 0;
                    numDraw = 0;
                    UpdateWindow(hwnd);
                }
            }
        }
        default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // Register the window class
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = "TicTacToe";
    wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MENU);
    RegisterClassEx(&wcex);

    hInst = hInstance;
    // Create the main window
    HWND hwnd = CreateWindowEx(0, "TicTacToe", "Tic Tac Toe", WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               NULL, NULL, hInstance, NULL);

    if (!hwnd)
    {
        return 1;
    }

    // Show the window and run the message loop
    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
