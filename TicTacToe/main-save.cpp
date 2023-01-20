#include <windows.h>
#include <windowsx.h>
#include <string>

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;


// Window procedure, handles messages for the main window
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static char board[3][3] = { { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' } };
    static bool player1Turn = true;
    static bool gameOver = false;

    // Handle different messages
    switch (message)
    {
        case WM_CREATE:
    {
        // Set the window's initial size and position
        RECT rect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
        SetWindowPos(hwnd, HWND_TOP, 100, 100, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Draw the Tic Tac Toe board
        HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
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

        // Draw the player's symbols on the board
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == 'X')
                {
                    brush = CreateSolidBrush(RGB(0, 0, 255));
                    SelectObject(hdc, brush);
                    Rectangle(hdc, 50 + i * 150, 50 + j * 150, 150 + i * 150, 150 + j * 150);
                    DeleteObject(brush);
                }
                else if (board[i][j] == 'O')
                {
                    brush = CreateSolidBrush(RGB(255, 0, 0));
                    SelectObject(hdc, brush);
                    Ellipse(hdc, 50 + i * 150, 50 + j * 150, 150 + i * 150, 150 + j * 150);
                    DeleteObject(brush);
                    Ellipse(hdc, 60 + i * 150, 60 + j * 150, 140 + i * 150, 140 + j * 150);

                }
            }
        }

        // Draw a message if the game is over
        if (gameOver)
        {
            std::string message;
            if (player1Turn)
            {
                message = "Player 2 wins!";
            }
            else
            {
                message = "Player 1 wins!";
            }

            TextOutA(hdc, 50, 400, message.c_str(), message.length());
        }

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        // Check if the game is over
        if (gameOver)
        {
            break;
        }

        // Get the mouse position and determine which square was clicked
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        int i = xPos / 150;
        int j = yPos / 150;

        // Place the player's symbol on the board if the square is empty
        if (board[i][j] == ' ')
        {
            board[i][j] = (player1Turn ? 'X' : 'O');

            // Check if the player has won
            if ((board[0][0] != ' ' && board[0][0] == board[1][1] && board[0][0] == board[2][2]) ||
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
            // Switch turns
                player1Turn = !player1Turn;
            }
        }

        // Check if the game is a draw
        bool draw = true;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == ' ')
                {
                    draw = false;
                    break;
                }
            }
        }

        if (draw)
        {
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
    RegisterClassEx(&wcex);

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
