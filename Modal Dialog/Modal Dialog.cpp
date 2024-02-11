#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include "resource.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
HWND hStatic;
HINSTANCE hInst;
int LEFT, TOP, RIGHT, BOTTOM;
unsigned short staticNumber = 0;
vector <HWND> hStaticArray;
wstring titleStr;
TCHAR number[4];

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    hInst = hInstance;
    // создаём главное окно приложения на основе модального диалога
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wp, LPARAM lp)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return TRUE; // сообщение обработано

    case WM_CLOSE:
        EndDialog(hWnd, 0); // закрываем модальный диалог
        return TRUE;

    case WM_LBUTTONDOWN:
        LEFT = LOWORD(lp);
        TOP = HIWORD(lp);
        return TRUE;

    case WM_LBUTTONUP:
        RIGHT = LOWORD(lp);
        BOTTOM = HIWORD(lp);
        if (RIGHT - LEFT < 10 || BOTTOM - TOP < 10) {
            MessageBox(hWnd, L"Error. Static box must be at least 10x10 size", 0, 0);
        }
        else {
            HWND hStatic = CreateWindowEx(0, TEXT("STATIC"), 0,
                WS_CHILD | WS_VISIBLE | WS_BORDER | SS_CENTER | WS_EX_CLIENTEDGE,
                LEFT, TOP, RIGHT - LEFT, BOTTOM - TOP, hWnd, 0, hInst, 0);
            hStaticArray.push_back(hStatic);
            staticNumber++;
            SetWindowText(hStatic, to_wstring(staticNumber).c_str());
        }
        return TRUE;

    case WM_RBUTTONDOWN:
        for (int i = 0; i < hStaticArray.size(); i++)
        {
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            RECT rect;
            GetWindowRect(hStaticArray[i], &rect);
            if (cursorPos.x >= rect.left && cursorPos.x <= rect.right && cursorPos.y >= rect.top && cursorPos.y <= rect.bottom) {
                GetWindowText(hStaticArray[i], number, 4);
                titleStr = L" Static #" + to_wstring(stoi(number)) + L" Width: " + to_wstring(rect.right - rect.left) + L" Height: " + to_wstring(rect.bottom - rect.top) + L" X: " + to_wstring(rect.left) + L" Y: " + to_wstring(rect.top);
                SetWindowText(hWnd, titleStr.c_str());
            }
        }
        return TRUE;
    case WM_MBUTTONDOWN: //Для удобства сделал не по дабл клику, а по средней кнопке мыши.
        for (int i = hStaticArray.size()-1; i >=0; i--)
        {
            POINT cursorPos;
            GetCursorPos(&cursorPos);
            RECT rect;
            GetWindowRect(hStaticArray[i], &rect);
            if (cursorPos.x >= rect.left && cursorPos.x <= rect.right && cursorPos.y >= rect.top && cursorPos.y <= rect.bottom) {
                DestroyWindow(hStaticArray[i]);
                hStaticArray.erase(hStaticArray.begin() + i);
                return TRUE;
            }
        }
        return TRUE;
    }
    return FALSE;
}
