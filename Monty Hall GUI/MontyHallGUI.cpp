// MontyHallGUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MontyHallGUI.h"
#include "MontyHall class.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
wchar_t print_out_string_buffer[128];
HWND door_1 = 0;
HWND door_2 = 0;
HWND door_3 = 0;
HWND text_box = 0;
MontyHallDoor door;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void AI_choose_door(MontyHallDoor* this_door);
void AI_choose_door_to_open(MontyHallDoor* door_game);
void result(void);
void user_win_message(int door);
void assign_door( int door_number);
void user_lose_message(void);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	srand((int)time(0));
	
	door.clear_door_array(door.remaining_door_to_choose);

	ZeroMemory(print_out_string_buffer, sizeof(wchar_t) * 128);

	wsprintf(print_out_string_buffer, L"This is a brain tester\nThere are 3 doors and one of it has a prize\nYour job is to choose which door has the price\nContinue??");

	int click_result = MessageBox(NULL, print_out_string_buffer, L"Welcome", MB_YESNO);

	if (click_result == IDNO)
	{
		return 0;
	}

	AI_choose_door(&door);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MONTYHALLGUI, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MONTYHALLGUI));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MONTYHALLGUI));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MONTYHALLGUI);
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
      CW_USEDEFAULT, 0, 460, 460, nullptr, nullptr, hInstance, nullptr);

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
//  PURPOSE:  Processes messages for the main window.
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
	case WM_CREATE:
	{
		door_1 = CreateWindow(L"Button", L"DOOR 1",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 10, 100, 200,
			hWnd, (HMENU)ID_DOOR_1_BUTTON, NULL, NULL);

		door_2 = CreateWindow(L"Button", L"DOOR 2",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			150, 10, 100, 200,
			hWnd, (HMENU)ID_DOOR_2_BUTTON, NULL, NULL);

		door_3 = CreateWindow(L"Button", L"DOOR 3",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			290, 10, 100, 200,
			hWnd, (HMENU)ID_DOOR_3_BUTTON, NULL, NULL);

		ZeroMemory(print_out_string_buffer, sizeof(wchar_t) * 128);

		wsprintf(print_out_string_buffer, L"Choose a door you think the prize is in there");

		text_box = CreateWindow(L"Static", print_out_string_buffer,
			WS_CHILD | WS_VISIBLE | SS_CENTER,
			10, 220, 380, 50,
			hWnd, NULL, NULL, NULL);
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

			if (wmId == ID_DOOR_1_BUTTON || wmId == ID_DOOR_2_BUTTON || wmId == ID_DOOR_3_BUTTON)
			{
				wsprintf(print_out_string_buffer, L"The computer will open one of the door");

				SetWindowText(text_box, print_out_string_buffer);

			}

            // Parse the menu selections:
            switch (wmId)
            {
			case ID_DOOR_1_BUTTON:
			{
				if (door.clicked_time == 0)
				{
					assign_door(0);
					
					Sleep(500);

					AI_choose_door_to_open(&door);

					wsprintf(print_out_string_buffer, L"Now you have a chance to change the door, click on the door you think the price is in");

					SetWindowText(text_box, print_out_string_buffer);
				}
				else
				{
					assign_door(0);

					result();
				}

			}
			break;
			case ID_DOOR_2_BUTTON:
			{
				if (door.clicked_time == 0)
				{
					assign_door(1);

					Sleep(500);

					AI_choose_door_to_open(&door);

					wsprintf(print_out_string_buffer, L"Now you have a chance to change the door, click on the door you think the price is in");
					SetWindowText(text_box, print_out_string_buffer);
				}
				else
				{
					assign_door(1);

					result();
				}
			}
			break;
			case ID_DOOR_3_BUTTON:
			{
				if (door.clicked_time == 0)
				{
					assign_door(2);

					Sleep(500);

					AI_choose_door_to_open(&door);

					wsprintf(print_out_string_buffer, L"Now you have a chance to change the door, click on the door you think the price is in");
					SetWindowText(text_box, print_out_string_buffer);
				}
				else
				{
					assign_door(2);

					result();
				}
			}
			break;
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
			UNREFERENCED_PARAMETER(hdc);
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

void AI_choose_door(MontyHallDoor* this_door)
{
	this_door->prize_door = rand() % 3;

	this_door->remaining_door_to_choose[this_door->prize_door] = AI_CHOOSE_DOOR;
}

void AI_choose_door_to_open(MontyHallDoor* door_game)
{
	int door_to_open = -1;

	// Use while loop to let computer randomly pick one of the door that is not prize door or door user choose
	while (door_to_open == -1)
	{
		int door_number = rand() % 3;

		if (door_game->remaining_door_to_choose[door_number] != AI_CHOOSE_DOOR && door_game->remaining_door_to_choose[door_number] != USER_CHOOSE_DOOR && door_game->remaining_door_to_choose[door_number] != USER_AI_CHOOSE_SAME_DOOR)
		{
			door_to_open = door_number + 1;

			door.remaining_door_to_choose[door_number] = DOOR_OPENED;
		}
	}

	switch (door_to_open)
	{
	case 1:

		SetWindowText(door_1, L"X");

		break;

	case 2:

		SetWindowText(door_2, L"X");

		break;
	case 3:

		SetWindowText(door_3, L"X");

		break;
	default:
		break;
	}
}

//
//  FUNCTION:	result(void)
//
//  PURPOSE:  Decide if user choose the prize door or not
//				If user chooose the prize door then user win message function will be called
//				If user did not choose the prize door then user_lose_message function will be called
void result(void)
{
	int user_win_message_func_called = false;

	for (int door_number = 0; door_number < NUMBER_DOOR; ++door_number)
	{
		if (door.remaining_door_to_choose[door_number] == USER_AI_CHOOSE_SAME_DOOR)
		{
			user_win_message_func_called = true;

			user_win_message(door_number + 1);
		}
	}

	if (user_win_message_func_called == false)
	{
		user_lose_message();
	}
}

void user_win_message(int win_door)
{
	switch (win_door)
	{
	case 1:
		SetWindowText(door_1,L"$");
		SetWindowText(door_2, L"X");
		SetWindowText(door_3, L"X");

		SetWindowText(text_box, L"Congratulation you chose the right price door	");
		
		break;
	case 2:
		SetWindowText(door_2, L"$");
		SetWindowText(door_3, L"X");
		SetWindowText(door_1, L"X");

		SetWindowText(text_box, L"Congratulation you chose the right price door	");
		
		break;
	case 3:
		SetWindowText(door_3, L"$");
		SetWindowText(door_2, L"X");
		SetWindowText(door_1, L"X");

		SetWindowText(text_box, L"Congratulation you chose the right price door	");

		break;
	default:
		break;
	}
}

void user_lose_message(void)
{
	switch ((door.prize_door) + 1)
	{
	case 1:

		SetWindowText(door_1, L"$");
		SetWindowText(door_2, L"X");
		SetWindowText(door_3, L"X");

		SetWindowText(text_box, L"Unlucky the win door was number 1");
		
		break;	
	case 2:

		SetWindowText(door_2, L"$");
		SetWindowText(door_1, L"X");
		SetWindowText(door_3, L"X");
		
		SetWindowText(text_box, L"Unlucky the win door was number 2");

		break;
	case 3:

		SetWindowText(door_3, L"$");
		SetWindowText(door_1, L"X");
		SetWindowText(door_2, L"X");

		SetWindowText(text_box, L"Unlucky the win door was number 3");
		
		break;
	default:
		break;
	}
}

void assign_door( int door_number)
{
	++door.clicked_time;
	
	if (door.remaining_door_to_choose[door_number] == AI_CHOOSE_DOOR)
	{
		door.remaining_door_to_choose[door_number] = USER_AI_CHOOSE_SAME_DOOR;
	}
	else
	{
		door.remaining_door_to_choose[door_number] = USER_CHOOSE_DOOR;
	}
}