// WindowsProject1.cpp : Definiuje punkt wejścia dla aplikacji.
//

#include "framework.h"
#include "WindowsProject1.h"
#include "cmath"
#include <fstream>
#include "string"

#define MAX_LOADSTRING 100
#define DLUGOSC  1600
#define ODLEGLOSC 40
#define WYSOKOSC 280
#define MARGINES 30
#define DLUGOSC_PRZEDZIALKI 5
#define DYSTANS_PRZEDZIALKA_Y 20
#define DYSTANS_PRZEDZIALKA_X 50
#define ILOSC_PRZEDZIALEK_Y 13

#define WYSOKOSC_PRZYCISKU 20
#define SZEROKOSC_PRZYCISKU 100

#define SINUSOIDA_PRZYCISK 1
#define KWADRATOWA_PRZYCISK 2
#define TROJKATNA_PRZYCISK 3
#define WCZYTAJ_DANE1 4
#define WCZYTAJ_DANE2 5
#define WCZYTAJ_DANE3 6
#define WCZYTAJ_DANE4 7
#define WCZYTAJ_DANE5 8
#define WCZYTAJ_DANE6 9

#define N 2 // rząd systemu
#define h 0.005 // krok obliczeń
#define T 20.0 // całkowity czas symulacji ? przedział [0 , T]
#define l 4 // liczba okresów sygnału sinus w przedziale T
#define M 80.0 // amplituda sygnału sinus
#define PI 3.14159265 // liczba PI

typedef struct { double n[N]; } Vect;
typedef struct { double n[N][N]; } Matr;
// pomocniczy typ ? kolejne bajty danej ?double?
typedef union { char c[sizeof(double)]; double d; } Box;
// zmienne globalne w programie
double us[(int)(1.0 * T / h) + 1]; // sygnał wejściowy sinus
double uf[(int)(1.0 * T / h) + 1]; // sygnał wejściowy fala prostokątna
double y[(int)(1.0 * T / h) + 1]; // sygnał wyjściowy
double y2[(int)(1.0 * T / h) + 1]; // sygnał wyjściowy


Box z; // zmienna: pojedyncza wartość sygnału (u lub y)
// dodawanie wektorów - operator przeciążony
Vect operator+(const Vect& U, const Vect& V)
{
    Vect W;
    int i;
    for (i = 0; i < N; i++)
        W.n[i] = U.n[i] + V.n[i];
    return W;
}
// mnożenie: wektor razy skalar - operator przeciążony
Vect operator*(const Vect& U, const double& d)
{
    Vect W;
    int i;
    for (i = 0; i < N; i++)
        W.n[i] = U.n[i] * d;
    return W;
}
// mnożenie: macierz razy wektor - operator przeciążony
Vect operator*(const Matr& A, const Vect& V)
{
    Vect W;
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0, W.n[i] = 0; j < N; j++)
            W.n[i] += A.n[i][j] * V.n[j];
    return W;
}
// mnożenie skalarne: wektor razy wektor - operator przeciążony
double operator*(const Vect& U, const Vect& V)
{
    int i;
    double s;
    for (i = 0, s = 0; i < N; i++)
        s += U.n[i] * V.n[i];
    return s;
}

// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego
INT wybor_funkcji;

INT tpobudzenia = 200;
INT ttlumienia = 999;

FLOAT A;
FLOAT m=100;
FLOAT r=1;
FLOAT k=10;
FLOAT b=10;

HWND button[3];


HWND Textboxskip;
HWND Textboxfile;
HWND TextboxMasa;
HWND TextboxAmplituda;
HWND TextboxPromien;
HWND TextboxSprezystosc;
HWND TextboxTlumik;
HWND TextboxtPobudzenia;
HWND Textboxttlumienia;

// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: W tym miejscu umieść kod.

    // Inicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Wykonaj inicjowanie aplikacji:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

    MSG msg;

    // Główna pętla komunikatów:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNKCJA: MyRegisterClass()
//
//  PRZEZNACZENIE: Rejestruje klasę okna.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   PRZEZNACZENIE: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    

    button[0] = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Sinusoida",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        1650,         // x position 
        20,         // y position 
        SZEROKOSC_PRZYCISKU,        // Button width
        WYSOKOSC_PRZYCISKU,        // Button height
        hWnd,     // Parent window
        (HMENU)SINUSOIDA_PRZYCISK,        // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    button[1] = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Kwadratowa",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        1650,         // x position 
        40,         // y position 
        SZEROKOSC_PRZYCISKU,        // Button width
        WYSOKOSC_PRZYCISKU,        // Button height
        hWnd,     // Parent window
        (HMENU)KWADRATOWA_PRZYCISK,        // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    button[2] = CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Trojkatna",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        1650,         // x position 
        60,         // y position 
        SZEROKOSC_PRZYCISKU,        // Button width
        WYSOKOSC_PRZYCISKU,        // Button height
        hWnd,     // Parent window
        (HMENU)TROJKATNA_PRZYCISK,        // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"amplituda",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        1650,         // x position 
        100,        // y position 
        100,        // Button width
        20,        // Button height
        hWnd,     // Parent window
        (HMENU)WCZYTAJ_DANE1,     // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    TextboxMasa = CreateWindowEx(
        0,
        L"EDIT",
        NULL, 
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        1750,     // x position
        100,     // y position 
        100,    // Button width
        20,     // Button height
        hWnd,
        NULL /*(HMENU)TEXTBOX*/,
        hInstance, NULL);

    CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"masa",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        1650,         // x position 
        120,        // y position 
        100,        // Button width
        20,        // Button height
        hWnd,     // Parent window
        (HMENU)WCZYTAJ_DANE2,     // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    TextboxAmplituda = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        1750,     // x position
        120,     // y position 
        100,    // Button width
        20,     // Button height
        hWnd,
        NULL /*(HMENU)TEXTBOX*/,
        hInstance, NULL);


    CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Promien",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        1650,         // x position 
        140,        // y position 
        100,        // Button width
        20,        // Button height
        hWnd,     // Parent window
        (HMENU)WCZYTAJ_DANE3,     // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    TextboxPromien = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        1750,     // x position
        140,     // y position 
        100,    // Button width
        20,     // Button height
        hWnd,
        NULL /*(HMENU)TEXTBOX*/,
        hInstance, NULL);

    CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"Sprezystosc",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        1650,         // x position 
        160,        // y position 
        100,        // Button width
        20,        // Button height
        hWnd,     // Parent window
        (HMENU)WCZYTAJ_DANE3,     // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    TextboxSprezystosc = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        1750,     // x position
        160,     // y position 
        100,    // Button width
        20,     // Button height
        hWnd,
        NULL /*(HMENU)TEXTBOX*/,
        hInstance, NULL);

    CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"b Tlumika",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        1650,         // x position 
        180,        // y position 
        100,        // Button width
        20,        // Button height
        hWnd,     // Parent window
        (HMENU)WCZYTAJ_DANE3,     // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    TextboxTlumik = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        1750,     // x position
        180,     // y position 
        100,    // Button width
        20,     // Button height
        hWnd,
        NULL /*(HMENU)TEXTBOX*/,
        hInstance, NULL);


    TextboxtPobudzenia = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        1750,     // x position
        200,     // y position 
        100,    // Button width
        20,     // Button height
        hWnd,
        NULL /*(HMENU)TEXTBOX*/,
        hInstance, NULL);

    CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"t pobudzenia",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        1650,         // x position 
        200,        // y position 
        100,        // Button width
        20,        // Button height
        hWnd,     // Parent window
        (HMENU)WCZYTAJ_DANE5,     // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.

    Textboxttlumienia = CreateWindowEx(
        0,
        L"EDIT",
        NULL,
        WS_CHILD | WS_VISIBLE | WS_BORDER,
        1750,     // x position
        220,     // y position 
        100,    // Button width
        20,     // Button height
        hWnd,
        NULL /*(HMENU)TEXTBOX*/,
        hInstance, NULL);

    CreateWindow(
        L"BUTTON",  // Predefined class; Unicode assumed 
        L"t tlumienia",      // Button text 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
        1650,         // x position 
        220,        // y position 
        100,        // Button width
        20,        // Button height
        hWnd,     // Parent window
        (HMENU)WCZYTAJ_DANE6,     // No menu.
        (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
        NULL);      // Pointer not needed.



    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PRZEZNACZENIE: Przetwarza komunikaty dla okna głównego.
//
//  WM_COMMAND  - przetwarzaj menu aplikacji
//  WM_PAINT    - Maluj okno główne
//  WM_DESTROY  - opublikuj komunikat o wyjściu i wróć
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Analizuj zaznaczenia menu:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case SINUSOIDA_PRZYCISK:
            wybor_funkcji = 1;
            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            break;
        case KWADRATOWA_PRZYCISK:
            wybor_funkcji = 2;
            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            break;
        case TROJKATNA_PRZYCISK:
            wybor_funkcji = 3;
            RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {

        int i, j, total;
        double a3, a2, a1, a0, b3, b2, b1, b0, w;
        static Matr A;
        static Vect B, C, C2, Ax, Bu, xi, xi_1;
        static double D, Cx, C2x, Du;

        for (int i = 0; i < (1.0 * T / h) + 1; i++)
        {
            y[i] = 0;
            y2[i] = 0;

        }


        A.n[0][0] = 0; A.n[0][1] = 1; 
        A.n[1][0] = -0.1; A.n[1][1] = -0.2; 
        
        B.n[0] = 0; B.n[1] = 1/m;
        C.n[0] = 1; C.n[1] = 0;
        C2.n[0] = 0; C2.n[1] = 1;
        D = 0;
        total = 4000;// rozmiar wektorów danych
        w = 2.0 * PI * l / T; // częstotliwość sinusoidy
        for (i = 0; i < total; i++) // obliczenie pobudzenia ? sinus lub fala prostokątna
        {
            us[i] =  sin(w * i * h); // sygnał wejściowy sinus: u=M*sin(w*t) , t=i*h
            uf[i] = ((us[i] > 0) ? M : -M); // sygnał wejściowy fala prostokątna
        }




        float funkcja[4000][4];

        if (wybor_funkcji == 1)
        {
            for (float i = 0; i < T / h; i++)
            {
                funkcja[int(i)][0] = us[int(i)];
                //funkcja[int(i)][1] = y[int(i)] * 50;
                //funkcja[int(i)][2] = y2[int(i)] * 50;

            }
        }
        else if (wybor_funkcji == 2)
        {
            for (float i = 0; i < T / h; i++)
            {
                if (i<ttlumienia && i>tpobudzenia)
                {
                    funkcja[int(i)][0] = 1;
                }
                else
                {
                    funkcja[int(i)][0] = 0;
                }
                
                //funkcja[int(i)][1] = 0;
                //funkcja[int(i)][2] = 0;

            }
            for (int i = tpobudzenia; i < ttlumienia; i++)
            {
                funkcja[int(i)][0] = 1;
                //funkcja[int(i)][1] = 67;
                //funkcja[int(i)][2] = 78;

            }
        }
        else if (wybor_funkcji == 3)
        {
            for (float i = 0; i < T / h; i++)
            {
                funkcja[int(i)][0] = (sin(i / 90)) ;
                //funkcja[int(i)][1] = (sin(i / 90)) * 180;
                //funkcja[int(i)][2] = (sin(i / 90)) * 130;

            }
        }
        else
        {
            for (float i = 0; i < T / h; i++)
            {
                funkcja[int(i)][0] = 0;
                //funkcja[int(i)][1] = (sin(i / 90)) * 180;
                //funkcja[int(i)][2] = (sin(i / 90)) * 130;

            }
        }




        // zerowe warunki początkowe
        xi_1.n[0] = 0; xi_1.n[1] = 0; xi_1.n[2] = 0; xi_1.n[3] = 0;
        // główna pętla obliczeń - zamiast pobudzenia sinus (us) można wstawić falę (uf)
        for (i = 0; i < total; i++)
        {
            Ax = A * xi_1; Bu = B * funkcja[i][0]; Cx = C * xi_1; Du = D * funkcja[i][0];
            C2x = C2 * xi_1; 
            xi = Ax + Bu; xi = xi * h; xi = xi_1 + xi; xi_1 = xi; y[i] = Cx + Du;
            y2[i] = C2x + Du;
        }




        

        
            for (float i = 0; i < T / h; i++)
            {
                //funkcja[int(i)][0] = us[int(i)];
                funkcja[int(i)][1] = y[int(i)] * 50;
                funkcja[int(i)][2] = y2[int(i)] * 50;

            }
       



        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Tutaj dodaj kod rysujący używający elementu hdc...
        HPEN hPenOld;
        HPEN bialy_prostokatPen;
        COLORREF bialy_prostokatKolor;
        bialy_prostokatKolor = RGB(255, 255, 255);
        bialy_prostokatPen = CreatePen(PS_SOLID, 1920, bialy_prostokatKolor);
        hPenOld = (HPEN)SelectObject(hdc, bialy_prostokatPen);
        MoveToEx(hdc, 0, WYSOKOSC, NULL);
        LineTo(hdc, 16000, WYSOKOSC);

        HPEN pen1;
        COLORREF qosieColor;
        qosieColor = RGB(0, 0, 0);
        pen1 = CreatePen(PS_SOLID, 2, qosieColor);
        hPenOld = (HPEN)SelectObject(hdc, pen1);

        for (int i = 0; i < 3; i++)
        {
            MoveToEx(hdc, MARGINES, ODLEGLOSC * (i + 1) + i * WYSOKOSC, NULL);
            LineTo(hdc, MARGINES, ODLEGLOSC * (i + 1) + (i + 1) * WYSOKOSC);
            MoveToEx(hdc, MARGINES, ODLEGLOSC * (i + 1) + (i + 1) * WYSOKOSC - WYSOKOSC / 2, NULL);
            LineTo(hdc, MARGINES + DLUGOSC, ODLEGLOSC * (i + 1) + (i + 1) * WYSOKOSC - WYSOKOSC / 2);
            
            for (size_t j = 0; j < WYSOKOSC / DYSTANS_PRZEDZIALKA_Y + 1; j++)
            {
                MoveToEx(hdc, MARGINES, ODLEGLOSC * (i + 1) + i * WYSOKOSC + j * DYSTANS_PRZEDZIALKA_Y, NULL);
                LineTo(hdc, MARGINES - DLUGOSC_PRZEDZIALKI, ODLEGLOSC * (i + 1) + i * WYSOKOSC + j * DYSTANS_PRZEDZIALKA_Y);
            }
            for (size_t j = 0; j < DLUGOSC / DYSTANS_PRZEDZIALKA_X; j++)
            {
                MoveToEx(hdc, MARGINES + (j + 1) * DYSTANS_PRZEDZIALKA_X, ODLEGLOSC * (i + 1) + (i + 1) * WYSOKOSC - WYSOKOSC / 2, NULL);
                LineTo(hdc, MARGINES + (j + 1) * DYSTANS_PRZEDZIALKA_X, ODLEGLOSC * (i + 1) + (i + 1) * WYSOKOSC + DLUGOSC_PRZEDZIALKI - WYSOKOSC / 2);
            }

        }
        HPEN pen2;
        
        qosieColor = RGB(255, 0, 0);
        pen2 = CreatePen(PS_SOLID, 2, qosieColor);
        hPenOld = (HPEN)SelectObject(hdc, pen2);

       




        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < total - 1; j++)
            {
                //if (funkcja[j][i]<WYSOKOSC/2 && funkcja[j+1][i] < WYSOKOSC / 2 && funkcja[j][i]>-1*WYSOKOSC/2 && funkcja[j+1][i] >-1* WYSOKOSC / 2)
                {
                    MoveToEx(hdc, MARGINES + (j) * DLUGOSC/T*h, ODLEGLOSC* (i + 1) + WYSOKOSC * i + WYSOKOSC / 2 - funkcja[j][i]*M, NULL);
                    LineTo(hdc, MARGINES + (j + 1) * DLUGOSC/T*h, ODLEGLOSC* (i + 1) + WYSOKOSC * i + WYSOKOSC / 2 - funkcja[j + 1][i]*M);
                }
                
            }
        }


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

// Procedura obsługi komunikatów dla okna informacji o programie.
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
