/*
10.	Смоделировать работу светофора пешеходного перехода через дорогу.
Светофор должен разрешать переход, если число пешеходов, желающих пересечь дорогу, не менее трех человек. 
После разрешения перехода все пешеходы переходят дорогу, движение транспорта возобновляется через 20 секунд и не может быть приостановлено повторно ранее, чем через минуту. 
Количество пешеходов моделируется по случайному закону: каждые 10 секунд к светофору подходят от 0 до 3 человек. Можно предусмотреть кнопку на форме, 
добавляющую внеочередного пешехода к количеству ожидающих на переходе. Если светофор зеленый, то пешеходы не накапливаются: «переходят» сразу.
*/

#include "PedestrianGenerator.h"

Pedestrian NSTU;
#define ID_TIMER1 1010
#define IDC_STATIC -1
#define IDC_STATUS 1001
#define IDC_BUTTON 1002
#define IDC_TIME 1003
#define IDC_PEDESTRIANS 1004

void UpdateTraffic(HWND hwnd)
{
    SetWindowTextA(GetDlgItem(hwnd, IDC_PEDESTRIANS), (std::to_string(NSTU.checkPeople())).c_str());
    SetWindowTextA(GetDlgItem(hwnd, IDC_STATUS), (NSTU.checkStatus()).c_str());
    SetWindowTextA(GetDlgItem(hwnd, IDC_TIME), (std::to_string(NSTU.checkTime())).c_str());
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateWindow(L"STATIC", L"Pedestrians:", WS_VISIBLE | WS_CHILD, 10, 10, 120, 20, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 170, 10, 70, 20, hWnd, (HMENU)IDC_PEDESTRIANS, NULL, NULL);
        CreateWindow(L"STATIC", L"Status:", WS_VISIBLE | WS_CHILD, 10, 40, 120, 20, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 170, 40, 70, 20, hWnd, (HMENU)IDC_STATUS, NULL, NULL);
        CreateWindow(L"STATIC", L"Time:", WS_VISIBLE | WS_CHILD, 10, 70, 120, 20, hWnd, (HMENU)IDC_STATIC, NULL, NULL);
        CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 170, 70, 70, 20, hWnd, (HMENU)IDC_TIME, NULL, NULL);
        CreateWindow(L"BUTTON", L"Add Pedestrian", WS_VISIBLE | WS_CHILD, 10, 100, 120, 30, hWnd, (HMENU)IDC_BUTTON, NULL, NULL);

        NSTU.startWork();

        SetTimer(hWnd, 1, 1000, NULL);

        break;

    case WM_COMMAND:
        switch(LOWORD(wParam))
        {
        case IDC_BUTTON:
            NSTU.addPeople();
        }
        break;

    case WM_TIMER:
       
            UpdateTraffic(hWnd);

        break;

    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hWnd;
    MSG msg;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"MainWindowClass";
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassEx(&wcex);

    hWnd = CreateWindowEx(0, L"MainWindowClass", L"Pedestrian Traffic", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 300, 180, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)
    {
        return false;
    }

    //отображение и обновление главного окна
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    //основной цикл сообщений
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}