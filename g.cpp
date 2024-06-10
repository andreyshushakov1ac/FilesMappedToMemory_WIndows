#include <windows.h>
#include <iostream>
#include <fstream>
// g++ g.cpp -o g.exe -lgdi32
// Обработчик сообщений окна
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        // Обработка сообщения о закрытии окна
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        // Обработка сообщения о перерисовке окна
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Загрузка и отображение изображения
            HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, "imagefile.jpg", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            if (hBitmap != NULL)
            {
                HDC hdcMem = CreateCompatibleDC(hdc);
                HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

                BITMAP bitmap;
                GetObject(hBitmap, sizeof(BITMAP), &bitmap);

                BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

                SelectObject(hdcMem, hBitmapOld);
                DeleteDC(hdcMem);
                DeleteObject(hBitmap);
            }

            // Отображение текста из файла
            std::ifstream infile("textfile.txt");
            if (infile.is_open())
            {
                std::string text((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
                SetTextColor(hdc, RGB(255, 255, 255)); // Установка цвета текста на белый
                SetBkMode(hdc, TRANSPARENT); // Установка режима фона на прозрачный
                RECT rect = {10, 10, 300, 50}; // Позиция и размер текста
                DrawText(hdc, text.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            }

            EndPaint(hwnd, &ps);
            break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

// Функция для отображения изображения и текста
void DisplayImageAndText(const char* imageFileName, const char* textFileName)
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "ImageTextWindowClass";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, "Image and Text Display", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, wc.hInstance, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


// Главная функция программы
int main()
{
    DisplayImageAndText("imagefile.jpg", "textfile.txt");

    return 0;
}
