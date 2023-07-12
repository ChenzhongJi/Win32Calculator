#include <windows.h>
#include <windowsx.h>
#include <string>
#include <comutil.h>
#include <iostream>
#include "Shlwapi.h"
#pragma   comment(lib,"shlwapi.lib") 
#include "basewin.h"
#include "CalculateExpression.h"
#include "MyLog.h"
#include <tchar.h>

#define IDB_E		    3329
#define IDB_ZERO		3300
#define IDB_ONE		    3301
#define IDB_TWO		    3302
#define IDB_THREE		3303
#define IDB_FOUR		3304
#define IDB_FIVE		3305
#define IDB_SIX		    3306
#define IDB_SEVEN		3307
#define IDB_EIGHT		3308
#define IDB_NINE		3309
#define IDB_DOT		    3310
#define IDB_DEL		    3311
#define IDB_AC  		3312
#define IDB_ADD		    3313
#define IDB_SUB		    3314
#define IDB_MULTI       3315
#define IDB_DIV         3316
#define IDB_EQ          3317
#define IDB_RM          3318


//编辑框中的字符串
std::wstring EditStr = L"0";
//std::wstring ResultStr = L"=";
//当前输入的数字
std::wstring curNUM = L"0";

class MainWindow : public BaseWindow<MainWindow>
{
    HWND* pEdit;
    HANDLE hFile;

public:

    MainWindow() : pEdit(NULL)
    {
        TCHAR Buffer[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, Buffer);
        TCHAR LogFileName[] = TEXT("CalculatorLog.txt");
        PathAppend(Buffer, LogFileName);
        OutputDebugString(L"=====");
        OutputDebugString(Buffer);

        hFile = CreateFile(Buffer, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        SetFilePointer(hFile, NULL, NULL, FILE_END);
    }


    PCWSTR  ClassName() const { return L"MyCalculator"; } //窗口标题
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam); //消息处理
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    MainWindow win;

    if (!win.Create(L"Calculator", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, 400, 350))
    {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

//wstring类型转换为string类型
std::string wstringTostring(wstring wstr)
{
    string result;
    //获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
    int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
    char* buffer = new char[len + 1];
    //宽字节编码转换成多字节编码  
    WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
    buffer[len] = '\0';
    //删除缓冲区并返回值  
    result.append(buffer);
    delete[] buffer;
    return result;
}

std::wstring stringTowstring(const std::string& str)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    wchar_t* wide = new wchar_t[len + 1];
    memset(wide, '\0', sizeof(wchar_t) * (len + 1));
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, len);
    std::wstring w_str(wide);
    delete[] wide;
    return w_str;
}

//判断是否是数字（包含小数点和负号）
bool isNumber(std::string str)
{
    return str.find_first_not_of(".-0123456789") == string::npos;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    TCHAR tmpStr[100] = { NULL };

    switch (uMsg)
    {
    case WM_CREATE:
    {
        HINSTANCE hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
        //创建文本框和相关按钮
        CreateWindow(TEXT("Edit"), TEXT("0"), WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_MULTILINE | ES_WANTRETURN | ES_READONLY, 30, 20, 320, 40, m_hwnd, (HMENU)IDB_E, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("7"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 30, 80, 50, 40, m_hwnd, (HMENU)IDB_SEVEN, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("8"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 90, 80, 50, 40, m_hwnd, (HMENU)IDB_EIGHT, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("9"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 150, 80, 50, 40, m_hwnd, (HMENU)IDB_NINE, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("DEL"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 80, 50, 40, m_hwnd, (HMENU)IDB_DEL, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("AC"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 290, 80, 50, 40, m_hwnd, (HMENU)IDB_AC, hInstance, NULL);

        CreateWindow(TEXT("Button"), TEXT("4"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 30, 130, 50, 40, m_hwnd, (HMENU)IDB_FOUR, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("5"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 90, 130, 50, 40, m_hwnd, (HMENU)IDB_FIVE, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("6"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 150, 130, 50, 40, m_hwnd, (HMENU)IDB_SIX, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("+"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 130, 50, 40, m_hwnd, (HMENU)IDB_ADD, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("-"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 290, 130, 50, 40, m_hwnd, (HMENU)IDB_SUB, hInstance, NULL);

        CreateWindow(TEXT("Button"), TEXT("1"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 30, 180, 50, 40, m_hwnd, (HMENU)IDB_ONE, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("2"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 90, 180, 50, 40, m_hwnd, (HMENU)IDB_TWO, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("3"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 150, 180, 50, 40, m_hwnd, (HMENU)IDB_THREE, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("*"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 180, 50, 40, m_hwnd, (HMENU)IDB_MULTI, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("/"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 290, 180, 50, 40, m_hwnd, (HMENU)IDB_DIV, hInstance, NULL);

        CreateWindow(TEXT("Button"), TEXT("0"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 30, 230, 110, 40, m_hwnd, (HMENU)IDB_ZERO, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("."), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 150, 230, 50, 40, m_hwnd, (HMENU)IDB_DOT, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("="), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 230, 50, 40, m_hwnd, (HMENU)IDB_EQ, hInstance, NULL);
        CreateWindow(TEXT("Button"), TEXT("%"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 290, 230, 50, 40, m_hwnd, (HMENU)IDB_RM, hInstance, NULL);
    }
    break;

    case WM_DESTROY:
        CloseHandle(hFile);
        PostQuitMessage(0);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        // 使无效的客户区变的有效，并取得设备环境句柄
        hdc = BeginPaint(m_hwnd, &ps);
        HBRUSH hbrush = CreateSolidBrush(RGB(0, 255, 0));
        SelectObject(hdc, hbrush);
        EndPaint(m_hwnd, &ps);
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDB_ZERO:
            if (curNUM.length() == 1 && curNUM[0] == '0')
            {
                break;
            }
            EditStr.append(L"0");
            curNUM.append(L"0");
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_ONE:
            if (curNUM.length() == 1 && curNUM[0] == '0')
            {
                EditStr.pop_back();
                curNUM.pop_back();
            }
            EditStr.append(L"1");
            curNUM.append(L"1");
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_TWO:
            if (curNUM.length() == 1 && curNUM[0] == '0')
            {
                EditStr.pop_back();
                curNUM.pop_back();
            }
            EditStr.append(L"2");
            curNUM.append(L"2");
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_THREE:
            if (curNUM.length() == 1 && curNUM[0] == '0')
            {
                EditStr.pop_back();
                curNUM.pop_back();
            }
            EditStr.append(L"3");
            curNUM.append(L"3");
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_FOUR:
            if (curNUM.length() == 1 && curNUM[0] == '0')
            {
                EditStr.pop_back();
                curNUM.pop_back();
            }
            EditStr.append(L"4");
            curNUM.append(L"4");
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_FIVE:
            if (curNUM.length() == 1 && curNUM[0] == '0')
            {
                EditStr.pop_back();
                curNUM.pop_back();
            }
            EditStr.append(L"5");
            curNUM.append(L"5");
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_SIX:
            if (curNUM.length() == 1 && curNUM[0] == '0')
            {
                EditStr.pop_back();
                curNUM.pop_back();
            }
            EditStr.append(L"6");
            curNUM.append(L"6");
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_SEVEN:
            if (curNUM.length() == 1 && curNUM[0] == '0')
            {
                EditStr.pop_back();
                curNUM.pop_back();
            }
            EditStr.append(L"7");
            curNUM.append(L"7");
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_EIGHT:
            if (curNUM.length() == 1 && curNUM[0] == '0')
            {
                EditStr.pop_back();
                curNUM.pop_back();
            }
            EditStr.append(L"8");
            curNUM.append(L"8");
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_NINE:
            if (curNUM.length() == 1 && curNUM[0] == '0')
            {
                EditStr.pop_back();
                curNUM.pop_back();
            }
            EditStr.append(L"9");
            curNUM.append(L"9");
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_DOT:
            if (curNUM.empty())
            {
                EditStr.append(L"0");
                curNUM.append(L"0");
            }
            else if (curNUM.find(L".") != std::string::npos)
            {
                break;
            }
            EditStr.append(L".");
            curNUM.append(L".");
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_DEL:
            EditStr.pop_back();
            if (!curNUM.empty())
            {
                curNUM.pop_back();
            }

            if (EditStr.empty())
            {
                EditStr = L"0";
                curNUM = L"0";
            }
            else if (curNUM.empty() && EditStr.back() <= '9' && EditStr.back() >= '0')
            {
                int flagIndex = EditStr.length() - 1;
                while (flagIndex-- >= 0)
                {
                    if (flagIndex < 0 || !(EditStr[flagIndex] <= '9' && EditStr[flagIndex] >= '0'))
                    {
                        break;
                    }
                }
                flagIndex++;
                curNUM = EditStr.substr(flagIndex, EditStr.length() - flagIndex);
            }

            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_AC:
            EditStr = L"0";
            curNUM = L"0";
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_ADD:
            if (curNUM.empty())
            {
                break;
            }
            if (EditStr.back() == '.')
            {
                EditStr.pop_back();
            }
            EditStr.append(L"+");
            curNUM.clear();
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_SUB:
            if (curNUM.empty())
            {
                break;
            }
            if (EditStr.back() == '.')
            {
                EditStr.pop_back();
            }
            EditStr.append(L"-");
            curNUM.clear();
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_MULTI:
            if (curNUM.empty())
            {
                break;
            }
            if (EditStr.back() == '.')
            {
                EditStr.pop_back();
            }
            EditStr.append(L"*");
            curNUM.clear();
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_DIV:
            if (curNUM.empty())
            {
                break;
            }
            if (EditStr.back() == '.')
            {
                EditStr.pop_back();
            }
            EditStr.append(L"/");
            curNUM.clear();
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        case IDB_EQ:
        {
            //删除最后的运算符和小数点（+、-、*、/、.）
            if (EditStr.back() == '+' || EditStr.back() == '-' || EditStr.back() == '*' || EditStr.back() == '/' || EditStr.back() == '.')
            {
                EditStr.pop_back();
            }
            std::string result = solve(wstringTostring(EditStr.c_str()));
            std::string logStr = wstringTostring(EditStr);
            EditStr.append(TEXT("\r\n="));
            EditStr.append(stringTowstring(result));
            logStr.append("=");
            logStr.append(result);
            logStr.append("\n");
            getLogStr(logStr);
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            WriteFile(hFile, logStr.c_str(), strlen(logStr.c_str()), 0, NULL);
            if (isNumber(result))
            {
                EditStr = stringTowstring(result);
                curNUM = stringTowstring(result);
            }
            else
            {
                EditStr = L"0";
                curNUM = L"0";
            }
            break;
        }
        case IDB_RM:
            if (curNUM.empty())
            {
                break;
            }
            else if (curNUM.back() == '.')
            {
                EditStr.pop_back();
            }

            EditStr.append(L"%");
            curNUM.clear();
            SetDlgItemText(m_hwnd, IDB_E, EditStr.c_str());
            break;
        default:
            break;
        }

    case WM_SIZE:  //窗口大小改变
        //Resize();
        break;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam); //默认窗口行为
}