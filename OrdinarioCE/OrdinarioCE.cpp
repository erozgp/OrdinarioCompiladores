// OrdinarioCE.cpp : Define el punto de entrada de la aplicación.
//

#include "framework.h"
#include "OrdinarioCE.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "varG.h"
#include "Ordinyc.h"

#pragma comment(lib, "comctl32.lib")

#define MAX_LOADSTRING 100

int fila = 1, error = 0;
float res = 0;
char* MuestraOpe;


//HWND hWndList;

// Variables globales:
HINSTANCE hInst;                                // instancia actual
WCHAR szTitle[MAX_LOADSTRING];                  // Texto de la barra de título
WCHAR szWindowClass[MAX_LOADSTRING];            // nombre de clase de la ventana principal
HWND hWndToolbar;								/*Variable para la barra de herramientas*/
const int NUMBUTTONS = 6;						/*Numero de botones de la Barra*/
HWND  hLista;									/* Variable para la lista de mensajes de errores */
static int iVecesCompilado = 1;

// Declaraciones de funciones adelantadas incluidas en este módulo de código:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
long PopFileLength(FILE* file);
void saveFile(char nombre[], HWND hWndEdt, HWND hWndVentana);
void compilar(HWND hWnd);
UINT saveFileAs(HWND hWndEdt, HWND hWndVentana);
void actualizar( HWND hWnd);
void detectObstaculo(int tipo, HWND hWnd, HWND hLista);

CHOOSEFONT Seleccionar_Fuente(HWND hwnd, HFONT* hFont, COLORREF rgb);

//void ColorearCodigo(HWND hWndEdit, unsigned char*, int, COLORREF);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Colocar código aquí.

    // Inicializar cadenas globales
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ORDINARIOCE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Realizar la inicialización de la aplicación:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ORDINARIOCE));

    MSG msg;

    // Bucle principal de mensajes:
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
//  FUNCIÓN: MyRegisterClass()
//
//  PROPÓSITO: Registra la clase de ventana.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ORDINARIOCE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ORDINARIOCE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCIÓN: InitInstance(HINSTANCE, int)
//
//   PROPÓSITO: Guarda el identificador de instancia y crea la ventana principal
//
//   COMENTARIOS:
//
//        En esta función, se guarda el identificador de instancia en una variable común y
//        se crea y muestra la ventana principal del programa.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Almacenar identificador de instancia en una variable global

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   /*Código para crear la barra de herramientas*/
   INITCOMMONCONTROLSEX InitCtrlEx;
   InitCtrlEx.dwSize = sizeof(INITCOMMONCONTROLSEX);
   InitCtrlEx.dwICC = ICC_BAR_CLASSES;
   InitCommonControlsEx(&InitCtrlEx);
   TBBUTTON tbrButtons[6];
   tbrButtons[0].iBitmap = 0;
   tbrButtons[0].idCommand = ID_BTNNUEVO;
   tbrButtons[0].fsState = TBSTATE_ENABLED;
   tbrButtons[0].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[0].dwData = 0L;
   tbrButtons[0].iString = (INT_PTR)L"Nuevo";
   tbrButtons[1].iBitmap = 1;
   tbrButtons[1].idCommand = ID_BTNABRIR;
   tbrButtons[1].fsState = TBSTATE_ENABLED;
   tbrButtons[1].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[1].dwData = 0L;
   tbrButtons[1].iString = (INT_PTR)L"Abrir";
   tbrButtons[2].iBitmap = 2;
   tbrButtons[2].idCommand = ID_BTNGUARDAR;
   tbrButtons[2].fsState = TBSTATE_ENABLED;
   tbrButtons[2].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[2].dwData = 0L;
   tbrButtons[2].iString = (INT_PTR)L"Guardar";
   tbrButtons[3].iBitmap = 3;
   tbrButtons[3].idCommand = ID_BTNCOMPILAR;
   tbrButtons[3].fsState = TBSTATE_ENABLED;
   tbrButtons[3].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[3].dwData = 0L;
   tbrButtons[3].iString = (INT_PTR)L"Compilar";
   tbrButtons[4].iBitmap = 4;
   tbrButtons[4].idCommand = ID_BTNEJECUTAR;
   tbrButtons[4].fsState = TBSTATE_ENABLED;
   tbrButtons[4].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[4].dwData = 0L;
   tbrButtons[4].iString = (INT_PTR)L"Ejecutar";
   tbrButtons[5].iBitmap = 5;
   tbrButtons[5].idCommand = ID_BTNACERCADE;
   tbrButtons[5].fsState = TBSTATE_ENABLED;
   tbrButtons[5].fsStyle = TBSTYLE_BUTTON;
   tbrButtons[5].dwData = 0L;
   tbrButtons[5].iString = (INT_PTR)L"Acerca de";
   hWndToolbar = CreateToolbarEx(hWnd,
       WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS
       | CCS_TOP | CCS_NODIVIDER | TBSTYLE_FLAT,
       IDB_TOOLBAR,
       NUMBUTTONS,
       hInstance,
       IDB_TOOLBAR,
       tbrButtons,
       NUMBUTTONS,
       0, 0, 0, 0,
       sizeof(TBBUTTON));
   /*Se establece el tamaño de los iconos para cada botón*/
   SendMessage(hWndToolbar, TB_SETBITMAPSIZE, 0, MAKELPARAM(50, 32));
   /*Se reajusta el tamaño de la barra de herramientas*/
   SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0L);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCIÓN: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PROPÓSITO: Procesa mensajes de la ventana principal.
//
//  WM_COMMAND  - procesar el menú de aplicaciones
//  WM_PAINT    - Pintar la ventana principal
//  WM_DESTROY  - publicar un mensaje de salida y volver
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND hWndEdit; //Manejador de ventana}
    CHARFORMAT2 cf; //Formato del texto
    DWORD dwEVM; //Evento de captura
    HFONT hFont; //Manejador de fuente
    static TCHAR szItemLista[256];
    static HWND hStatus;
    static int iMitad, iArriba;
    static CHARFORMAT2 colorTexto;
    //Parte pen y brush
    PAINTSTRUCT ps;
    HDC hdc;
    static HPEN hpenNEGRO;
    static HBRUSH hbrA;	//un manejador para brocha

    static float tx;
    //static int compilar, resul;

    HDC hdcMemList[9];
    static HBITMAP bmpImagenes[9], bmpAnts[9];
    BITMAP bmS[9];

    UINT optionSelected;

    TCHAR txt[1024];

    switch (message)
    {
    case WM_CREATE:
    {
        hpenNEGRO = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
        hbrA = CreateSolidBrush(RGB(255, 255, 0));
        
        t = NULL;

        CrearCola(&cola);

        GetClientRect(hWnd, &rect);

        for (int i = 0; i < 18; i++)
        {
            for (int j = 0; j < 8; j++) {
                matriz[i][j] = 0;
            
            }
        }

        bmpImagenes[0] = LoadBitmap(GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDB_BROCHE_UP));

        if (bmpImagenes[0] == NULL)
            MessageBox(hWnd, L"No se pudo cargar la imagen BROCHE_UP.bmp", L"Error",
                MB_OK | MB_ICONEXCLAMATION);

        bmpImagenes[1] = LoadBitmap(GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDB_BROCHE_DER));
        if (bmpImagenes[1] == NULL)
            MessageBox(hWnd, L"No se pudo cargar la imagen BROCHE_DER.bmp", L"Error",
                MB_OK | MB_ICONEXCLAMATION);

        bmpImagenes[2] = LoadBitmap(GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDB_BROCHE_DOWN));
        if (bmpImagenes[2] == NULL)
            MessageBox(hWnd, L"No se pudo cargar la imagen BROCHE_DOWN.bmp", L"Error",
                MB_OK | MB_ICONEXCLAMATION);

        bmpImagenes[3] = LoadBitmap(GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDB_BROCHE_LEFT));
        if (bmpImagenes[3] == NULL)
            MessageBox(hWnd, L"No se pudo cargar la imagen BROCHE_LEFT.bmp", L"Error",
                MB_OK | MB_ICONEXCLAMATION);

        bmpImagenes[4] = LoadBitmap(GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDB_GE));
        if (bmpImagenes[4] == NULL)
            MessageBox(hWnd, L"No se pudo cargar la imagen GE.bmp", L"Error",
                MB_OK | MB_ICONEXCLAMATION);

        bmpImagenes[5] = LoadBitmap(GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDB_FLECHA));
        if (bmpImagenes[5] == NULL)
            MessageBox(hWnd, L"No se pudo cargar la imagen FELCHA.bmp", L"Error",
                MB_OK | MB_ICONEXCLAMATION);
        bmpImagenes[6] = LoadBitmap(GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDB_ARBUSTO));
        if (bmpImagenes[6] == NULL)
            MessageBox(hWnd, L"No se pudo cargar la imagen ARBUSTO.bmp", L"Error",
                MB_OK | MB_ICONEXCLAMATION);
        bmpImagenes[7] = LoadBitmap(GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDB_VALLA));
        if (bmpImagenes[7] == NULL)
            MessageBox(hWnd, L"No se pudo cargar la imagen VALLA.bmp", L"Error",
                MB_OK | MB_ICONEXCLAMATION);
        bmpImagenes[8] = LoadBitmap(GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDB_BARRERA));
        if (bmpImagenes[8] == NULL)
            MessageBox(hWnd, L"No se pudo cargar la imagen BARRERA.bmp", L"Error",
                MB_OK | MB_ICONEXCLAMATION);

        // Deshabilitamos el botón nuevo ya que al inicio está vacío.
        EnableMenuItem(GetMenu(hWnd), ID_BTNNUEVO, MF_DISABLED);
        
        LoadLibrary(L"riched20.dll");
        hWndEdit = CreateWindowEx(WS_EX_CLIENTEDGE, RICHEDIT_CLASS, L"", WS_CHILD
            | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL
            | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
            0, 0, 0, 0,
            hWnd, (HMENU)ID_EDITRICH, hInst, NULL);
        hFont = CreateFont(18, 0, 0, 0, 0, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, L"Eras Light ITC");
        SendMessage(hWndEdit, WM_SETFONT, (WPARAM)hFont, 0);
        hLista = CreateWindow(L"listbox", NULL, WS_CHILDWINDOW | WS_VISIBLE
            | WS_VSCROLL | WS_HSCROLL | LBS_NOTIFY,
            0, 0, 0, 0, hWnd, (HMENU)ID_LISTA, hInst, NULL);

        hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL,
            WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0,
            hWnd, (HMENU)IDB_STATUS, hInst, NULL);

        dwEVM = SendMessage(hWndEdit, EM_GETEVENTMASK, 0, 0);
        dwEVM |= ENM_UPDATE | ENM_CHANGE | ENM_SELCHANGE;
        SendMessage(hWndEdit, EM_SETEVENTMASK, 0, dwEVM);


        memset(&colorTexto, 0, sizeof colorTexto);
        colorTexto.cbSize = sizeof(CHARFORMAT2);
        colorTexto.dwMask = CFM_COLOR;
        colorTexto.crTextColor = RGB(0, 0, 0);

        SendMessage(hWndEdit, WM_SETFONT, (WPARAM)hFont, 0);
        SendMessage(hWndEdit, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&colorTexto);
        SendMessage(hWndEdit, EM_SETMODIFY, 0, 0);

        dwEVM = SendMessage(hWndEdit, EM_GETEVENTMASK, 0, 0);
        dwEVM |= ENM_UPDATE | ENM_CHANGE | ENM_SELCHANGE;
        SendMessage(hWndEdit, EM_SETEVENTMASK, 0, dwEVM);


        SetFocus(hWndEdit);
        break;
    }
    case WM_SIZE:
    {

        MoveWindow(hLista, 0, HIWORD(lParam) - 74, LOWORD(lParam), 60, TRUE);
        MoveWindow(hWndEdit, 0, 50, LOWORD(lParam) / 3, HIWORD(lParam) - 190, TRUE);
        SendDlgItemMessage(hWnd, IDB_STATUS, WM_SIZE, 0, 0);
        iMitad = LOWORD(lParam) / 3;
        iArriba = 50;
        break;
    }
    case WM_NOTIFY:
    {
        NMHDR* pHdr = (NMHDR*)lParam;
        TCHAR szPosCur[MAX_LOADSTRING];
        if (pHdr->hwndFrom == hWndEdit)
        {
            if (pHdr->code == EN_SELCHANGE)
            {
                wsprintf(szPosCur, L"Linea: %d", 1 + (WORD)SendMessage(hWndEdit, EM_LINEFROMCHAR, -1, 0L));
                SendDlgItemMessage(hWnd, IDB_STATUS, SB_SETTEXT, 0, (LPARAM)szPosCur);
            }
        }
    }
    break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            int wmEvent = HIWORD(wParam);
            static TCHAR szItemLista[64];
            static TCHAR sFileAbout[250], sFileHelp[250];
            // Analizar las selecciones de menú:
            switch (wmId)
            {
            case IDM_FUENTE:
            {
                CHOOSEFONT cf = Seleccionar_Fuente(hWnd, &hFont, colorTexto.crTextColor);
                colorTexto.crTextColor = cf.rgbColors;
                SendMessage(hWndEdit, WM_SETFONT, (WPARAM)hFont, 0);
                SendMessage(hWndEdit, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&colorTexto);
                SendMessage(hWndEdit, EM_SETMODIFY, 0, 0);									 //   Indica que se ha modificado el control edit (para las acciones)
            }
            break;
            case ID_EDITRICH:
                switch (wmEvent)
                {
                case EN_CHANGE:
                    DWORD iLength;
                    DWORD iIni = 0, iFin = 0;
                    PSTR pstrBuffer;
                    TCHAR* ptchBuffer = NULL;
                    POINT posScroll;
                    RECT rect;
                    iLength = GetWindowTextLength(hWndEdit);
                    SendMessage(hWndEdit, EM_GETSEL, (WPARAM)&iIni, (LPARAM)&iFin);					//		Posición actual del cursor
                    SendMessage(hWndEdit, EM_GETSCROLLPOS, 0L, (LPARAM)&posScroll);					//		Poscicion del scroll
                    if (NULL == (pstrBuffer = (PSTR)malloc(sizeof(char) * (iLength + 1))) ||
                        NULL == (ptchBuffer = (TCHAR*)malloc(sizeof(TCHAR) * (iLength + 1)))) {
                        MessageBox(hWnd, L"Error al reservar memoria", L"Error", MB_OK | MB_ICONERROR);
                    }
                    else {
                        GetWindowText(hWndEdit, ptchBuffer, iLength + 1);
                        wcstombs(pstrBuffer, ptchBuffer, iLength + 1);

                        dwEVM = SendMessage(hWndEdit, EM_GETEVENTMASK, 0, 0);
                        SendMessage(hWndEdit, WM_SETREDRAW, FALSE, 0);

                        SendMessage(hWndEdit, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&colorTexto);                             //   Color de texto de edit
                        //ColorearCodigo(hWndEdit, (unsigned char*)pstrBuffer, iLength, colorTexto.crTextColor);												 //  Inicia el coloreo del código

                        SendMessage(hWndEdit, WM_SETREDRAW, TRUE, 0);
                        GetClientRect(hWndEdit, &rect);
                        InvalidateRect(hWndEdit, &rect, TRUE);

                        SendMessage(hWndEdit, EM_SETSEL, (WPARAM)iIni, (LPARAM)iFin);				 //		Reposicionar el cursor
                        SendMessage(hWndEdit, EM_SETSCROLLPOS, 0L, (LPARAM)&posScroll);

                        SendMessage(hWndEdit, EM_SETEVENTMASK, 0, dwEVM);
                        free(pstrBuffer);
                        free(ptchBuffer);
                    }
                    //MessageBeep(100);
                    //MessageBox(hWnd, L"hola", L"Hola", MB_OK);
                    //InvalidateRect(hWnd, NULL, TRUE);

                    break;
                }
                break;
            case ID_LISTA:
            {
                switch (wmEvent)
                {
                case LBN_DBLCLK:
                {
                    int i = SendMessage(hLista, LB_GETCURSEL, 0, 0);
                    SendMessage(hLista, LB_GETTEXT, i, (LPARAM)szItemLista);
                    InvalidateRect(hWnd, NULL, TRUE);

                    PSTR pstrBuffer;
                    TCHAR* ptchBuffer = NULL;
                    long iLength = 256;
                    int inicio = 0;
                    int saltos = 0;
                    int numero_linea;

                    if (NULL != (pstrBuffer = (PSTR)malloc(sizeof(char) * (iLength + 1))))
                        wcstombs(pstrBuffer, szItemLista, iLength + 1);

                    sscanf(pstrBuffer, "Error en la Linea %d", &numero_linea);	//	Copiamos el numero que esta en el mensaje de error
                    free(pstrBuffer);

                    iLength = GetWindowTextLength(hWndEdit);
                    if (NULL == (pstrBuffer = (PSTR)malloc(sizeof(char) * (iLength + 1))) ||
                        NULL == (ptchBuffer = (TCHAR*)malloc(sizeof(TCHAR) * (iLength + 2))))
                    {
                        MessageBox(hWnd, L"Error al reservar memoria",
                            L"Error", MB_OK | MB_ICONERROR);
                    }

                    GetWindowText(hWndEdit, ptchBuffer, iLength + 1);
                    wcstombs(pstrBuffer, ptchBuffer, iLength + 1);
                    pstrBuffer[iLength] = '\n';

                    i = 0;
                    while (saltos < numero_linea - 1)
                    {
                        while (pstrBuffer[i] != '\n')
                            i++;
                        i++;
                        saltos++;
                    }
                    inicio = i - saltos;
                    i++;
                    while (pstrBuffer[i] != '\r')
                        i++;
                    SendMessage(hWndEdit, EM_SETSEL, inicio, i - saltos);

                    free(pstrBuffer);
                    free(ptchBuffer);
                    SetFocus(hWndEdit);
                    break;
                }
                break;
                }
            }break;
            case ID_BTNNUEVO:

                if (isSaved == FALSE) {

                    optionSelected = MessageBox(hWnd, L"¿Guardar los cambios?", L"Guardar archivo", MB_YESNOCANCEL | MB_ICONQUESTION);

                    if (optionSelected == IDYES) {
                        saveFileAs(hWndEdit, hWnd);

                        SetWindowText(hWndEdit, L"");
                        //SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)L"Guardado!");

                        //SetWindowText(hWnd, L"JoJo´s Golden Wind");

                        EnableMenuItem(GetMenu(hWnd), ID_BTNNUEVO, MF_DISABLED);
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNNUEVO, MF_DISABLED);

                        strcpy(cFile, " ");
                        isSaved = FALSE;
                    }
                    else if (optionSelected == IDCANCEL) {
                        
                    }
                    else {
                        SetWindowText(hWndEdit, L"");

                        //SetWindowText(hWnd, L"JoJo´s Golden Wind");

                        EnableMenuItem(GetMenu(hWnd), ID_BTNNUEVO, MF_DISABLED);
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNNUEVO, MF_DISABLED);
                        strcpy(cFile, " ");
                        isSaved = FALSE;
                    }
                }
                else {
                    isSaved = FALSE;
                    strcpy(cFile, " ");
                    //SetWindowText(hWnd, L"JoJo´s Golden Wind");
                    SetWindowText(hWndEdit, L"");

                    EnableMenuItem(GetMenu(hWnd), ID_BTNNUEVO, MF_DISABLED);
                    SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNNUEVO, MF_DISABLED);
                }

                break;
            case ID_BTNABRIR:
            {
                TCHAR szFile[MAX_PATH], szCaption[64 + _MAX_FNAME + _MAX_EXT];
                ZeroMemory(szFile, MAX_PATH);
                OPENFILENAME ofn;
                ZeroMemory(&ofn, sizeof(OPENFILENAME));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
                    OFN_HIDEREADONLY | OFN_CREATEPROMPT;
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = _T("Tipos de formatos soportados(*.txt)\0 * .txt\0Texto(*.txt)\0\0");
                ofn.lpstrTitle = _T("Abrir archivo de texto");
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = MAX_PATH;
                if (IDOK == GetOpenFileName(&ofn)) {
                    wsprintf(szCaption, _T("%s - %s"), szTitle, szFile[0] ? szFile :
                        _T("Sin archivo abierto"));
                    SetWindowText(hWnd, szCaption);
                    FILE* file;
                    int iLength;
                    PSTR pstrBuffer;
                    char cFile[MAX_PATH];
                    TCHAR* ptchBuffer;
                    wcstombs(cFile, szFile, MAX_PATH);
                    if (NULL == (file = fopen(cFile, "rb"))) {
                        MessageBox(hWnd, L"Error al leer el archivo", L"Error",
                            MB_OK | MB_ICONERROR);
                    }
                    else {
                        iLength = PopFileLength(file);
                        if (NULL == (pstrBuffer = (PSTR)malloc
                        (sizeof(char) * (iLength + 1))) ||
                            NULL == (ptchBuffer = (TCHAR*)malloc
                            (sizeof(TCHAR) * (iLength + 1)))) {
                            fclose(file);
                            MessageBox(hWnd, L"Error al reservar memoria",
                                L"Error", MB_OK | MB_ICONERROR);
                        }
                        else {
                            fread(pstrBuffer, 1, iLength, file);
                            fclose(file);
                            pstrBuffer[iLength] = '\0';
                            mbstowcs(ptchBuffer, pstrBuffer, iLength + 1);
                            SetWindowText(hWndEdit, ptchBuffer);
                            free(pstrBuffer);
                            free(ptchBuffer);
                        }
                    }
                }
                //El siguiente segmento de código da formato reemplazando texto
                //normal por texto con formato
                memset(&cf, 0, sizeof cf); //Se limpia la estructura del formato
                cf.cbSize = sizeof(CHARFORMAT2); //Se fija el tamaño de la estructura
                                                 // Se establece la mascara para que sea posible aplicar color al texto
                cf.dwMask = CFM_COLOR; //| CFM_BACKCOLOR ;
                //cf.crTextColor = RGB(255, 0, 0); //Se establece el color del texto
                                                 //cf.crBackColor = RGB(0,0,255);
                                                 //Se establece un rango de texto a seleccionar
                //SendMessage(hWndEdit, EM_SETSEL, (WPARAM)5, (LPARAM)9);
                //Se aplica el formato al rango seleccionado
                //SendMessage(hWndEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
                //Se reemplaza el rango seleccionado con el nuevo texto y formato
            }
            break;
            case ID_BTNGUARDAR:
            {
                TCHAR szFile[MAX_PATH];
                ZeroMemory(szFile, MAX_PATH);
                OPENFILENAME ofn;
                ZeroMemory(&ofn, sizeof(OPENFILENAME));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST |
                    OFN_OVERWRITEPROMPT;
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = _T("Tipos de formatos soportados(*.txt)\0 * .txt\0Texto(*.txt)\0\0");
                ofn.lpstrTitle = _T("Guardar archivo de texto");
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = MAX_PATH;
                if (IDOK == GetSaveFileName(&ofn)) {
                    FILE* file;
                    int iLength;
                    PSTR pstrBuffer;
                    char cFile[MAX_PATH];
                    TCHAR* ptchBuffer = NULL;
                    wcstombs(cFile, szFile, MAX_PATH);
                    if (NULL == (file = fopen(cFile, "wb"))) {
                        MessageBox(hWnd, L"Error al crear el archivo", L"Error",
                            MB_OK | MB_ICONERROR);
                    }
                    else {
                        iLength = GetWindowTextLength(hWndEdit);
                        if (NULL == (pstrBuffer = (PSTR)malloc(sizeof(char) *
                            (iLength + 1))) ||
                            NULL == (ptchBuffer = (TCHAR*)malloc(sizeof(TCHAR) *
                            (iLength + 1))))
                        {
                            MessageBox(hWnd, L"Error al reservar memoria",
                                L"Error", MB_OK | MB_ICONERROR);
                            fclose(file);
                        }
                        GetWindowText(hWndEdit, ptchBuffer, iLength + 1);
                        wcstombs(pstrBuffer, ptchBuffer, iLength + 1);
                        fwrite(pstrBuffer, 1, iLength + 1, file);
                        fclose(file);
                        free(pstrBuffer);
                        free(ptchBuffer);
                        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"Guardado bro uwu");
                    }
                }
            }
            break;
            case ID_BTNCOMPILAR: {


                bEjecutar = FALSE;
                bCompilado = TRUE;

                VaciarCola(&cola);
                while (eliminarCabeza(&t));
                for (int i = 0; i < 18; i++)
                {
                    for (int j = 0; j < 8; j++) {
                        matriz[i][j] = 0;

                    }
                }

                SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, MF_DISABLED);
                SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, MF_DISABLED);


                
                if (!isSaved && (strcmp(cFile, " ") == 0))
                { 
                    if (saveFileAs(hWndEdit, hWnd) == IDOK)
                    {
                        isSaved = TRUE;
                        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"Guardado bro uwu");

                        compilar(hWnd);
                        iVecesCompilado++;


                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, TBSTATE_ENABLED);
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, TBSTATE_ENABLED);

                    } 
                } 
                else
                {
                    saveFile(cFile, hWndEdit, hWnd);
                    SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"Guardado bro uwu");
                    

                    isSaved = TRUE;

                    if (iVecesCompilado == 1)
                    {
                        compilar(hWnd);
                        iVecesCompilado++;

                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, TBSTATE_ENABLED);
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, TBSTATE_ENABLED);

                    }
                    else
                    {
                        compilar(hWnd);

                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, TBSTATE_ENABLED);
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, TBSTATE_ENABLED);

                    }

                

                }


               
                InvalidateRect(hWnd, NULL, TRUE);
            }
            break;

            case ID_BTNEJECUTAR: {
                bEjecutar = TRUE;

                SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, MF_DISABLED);
                SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, MF_DISABLED);

                if (isSaved)
                {
                    if (bCompilado)
                    {
                        SetTimer(hWnd, IDT_TIMER, 1000, NULL);
                        InvalidateRect(hWnd, &rect, TRUE);
                    }
                    else
                    {
                        VaciarCola(&cola);
                        while (eliminarCabeza(&t));

                        optionSelected = MessageBox(hWnd, L"No ha compilado.\n¿Desea compilar ahora?", L"Compilar", MB_YESNOCANCEL | MB_ICONQUESTION);

                        if (optionSelected == IDYES) {

                            if (iVecesCompilado == 1)
                            {
                                compilar(hWnd);
                                bCompilado = TRUE;
                                iVecesCompilado++;

                                SetTimer(hWnd, IDT_TIMER, 1000, NULL);
                                InvalidateRect(hWnd, &rect, TRUE);

                            }
                            else
                            {
                                compilar(hWnd);
                                
                                bCompilado = TRUE;
                                SetTimer(hWnd, IDT_TIMER, 1000, NULL);
                                InvalidateRect(hWnd, &rect, TRUE);


                            }
                        }
                        else if (optionSelected == IDCANCEL) {
                            
                            SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, TBSTATE_ENABLED);
                            SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, TBSTATE_ENABLED);
                        }
                        else {
                            
                            SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, TBSTATE_ENABLED);
                            SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, TBSTATE_ENABLED);
                        }
                    }

                }
                else
                {
                    if (!isSaved && (strcmp(cFile, "") == 0))
                    {
                        if (saveFileAs(hWndEdit, hWnd) == IDOK)
                        {
                            isSaved = TRUE;
                            bCompilado = TRUE;
                            
                            compilar(hWnd);
                            iVecesCompilado++;

                            SetTimer(hWnd, IDT_TIMER, 1000, NULL);
                            InvalidateRect(hWnd, &rect, TRUE);

                            EnableMenuItem(GetMenu(hWnd), ID_BTNNUEVO, MF_ENABLED | MF_BYCOMMAND);
                            SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNNUEVO, TBSTATE_ENABLED);
                        }
                    }
                    else
                    {
                        VaciarCola(&cola);
                        while (eliminarCabeza(&t));

                        optionSelected = MessageBox(hWnd, L"No ha compilado,\n¿Desea compilar ahora?", L"Compilar", MB_YESNOCANCEL | MB_ICONQUESTION);

                        if (optionSelected == IDYES) {

                            if (iVecesCompilado == 1)
                            {
                                saveFileAs(hWndEdit, hWnd);
                                isSaved = TRUE;
                                bCompilado = TRUE;

                                compilar(hWnd);
                                iVecesCompilado++;

                                SetTimer(hWnd, IDT_TIMER, 1000, NULL);
                                InvalidateRect(hWnd, &rect, TRUE);

                            }
                            else
                            {
                                saveFileAs(hWndEdit, hWnd);
                                isSaved = TRUE;
                                bCompilado = TRUE;

                                compilar(hWnd);
                                

                                SetTimer(hWnd, IDT_TIMER, 1000, NULL);
                                InvalidateRect(hWnd, &rect, TRUE);

                            }
                        }
                        else if (optionSelected == IDCANCEL) {
                            
                        }
                        else {
                            
                        }

                    }
                }
    
            }
            break;
            case ID_BTNACERCADE: {
                MessageBox(hWnd, L"Alumnos: Carlos Eugenio Cabrera Guerra, Erick Oswaldo Gallegos Perez\nProfesor: J. Jesus Arellano Pimentel\nMateria:Compiladores\nCompilador basado en LOGO.\n12/02/2020", L"Acerca de...", MB_ICONINFORMATION);
            }break;
            case IDM_ABOUT:
                
                MessageBox(hWnd, L"Alumnos: Carlos Eugenio Cabrera Guerra, Erick Oswaldo Gallegos Perez\nProfesor: J. Jesus Arellano Pimentel\nMateria:Compiladores\nCompilador basado en LOGO.\n12/02/2020", L"Acerca de...", MB_ICONINFORMATION);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_TIMER:

        if (bCompilacionCorrecta && bEjecutar)
        {
            
            actualizar(hWnd);
            InvalidateRect(hWnd, &rect, TRUE);
        }
        else {
            SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, TBSTATE_ENABLED);
            SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, TBSTATE_ENABLED);
            KillTimer(hWnd, IDT_TIMER);
            InvalidateRect(hWnd, &rect, TRUE);
            bCompilado = FALSE;
        }

        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            simbolo* temp = t;
            

            // TODO: Agregar cualquier código de dibujo que use hDC aquí...
            
            SelectObject(hdc, hpenNEGRO);

            TextOut(hdc, iMitad + 10, iArriba + 10, L"Rectángulos", wcslen(L"Rectángulos"));
            Rectangle(hdc, iMitad + 10, iArriba + 10, iMitad + 910, iArriba + 510);
            int arriba = iArriba + 10;
            int mitad = iMitad + 10;
            int x0 = mitad;
            int y0 = arriba;
            for (int i = 1; i <= 9; i++)
            {
                SelectObject(hdc, hpenNEGRO);
                MoveToEx(hdc, iMitad + 10, arriba + 50 * i, NULL); LineTo(hdc, iMitad + 910, arriba + 50 * i);
            }
            for (int i = 1; i <= 17; i++)
            {
                SelectObject(hdc, hpenNEGRO);
                MoveToEx(hdc, mitad + 50 * i, arriba, NULL); LineTo(hdc, mitad + 50 * i, arriba + 500);
            }
            

            for (int i = 0; i < 9; i++)
            {
                hdcMemList[i] = CreateCompatibleDC(hdc);
            }

            for (int i = 0; i < 9; i++)
            {
                bmpAnts[i] = (HBITMAP)SelectObject(hdcMemList[i], bmpImagenes[i]);
                GetObject(bmpImagenes[i], sizeof(bmS[i]), &bmS[i]);
            }

            while (temp)
            {
                switch (temp->tvar)
                {
                case BROCH:
                    if (temp->direccion == RIGHT || temp->direccion == DIRNULL)
                    {
                        BitBlt(hdc, x0 + temp->x * bmS[1].bmWidth,
                            y0 + temp->y * bmS[1].bmWidth,
                            bmS[1].bmWidth, bmS[1].bmHeight,
                            hdcMemList[1], 0, 0, SRCCOPY);
                    }
                    if (temp->direccion == LEFT)
                    {
                        BitBlt(hdc, x0 + temp->x * bmS[3].bmWidth,
                            y0 + temp->y * bmS[3].bmWidth,
                            bmS[3].bmWidth, bmS[3].bmHeight,
                            hdcMemList[3], 0, 0, SRCCOPY);
                    }
                    if (temp->direccion == UP)
                    {
                        BitBlt(hdc, x0 + temp->x * bmS[0].bmWidth,
                            y0 + temp->y * bmS[0].bmWidth,
                            bmS[0].bmWidth, bmS[0].bmHeight,
                            hdcMemList[0], 0, 0, SRCCOPY);
                    }
                    if (temp->direccion == DOWN)
                    {
                        BitBlt(hdc, x0 + temp->x * bmS[2].bmWidth,
                            y0 + temp->y * bmS[2].bmWidth,
                            bmS[2].bmWidth, bmS[2].bmHeight,
                            hdcMemList[2], 0, 0, SRCCOPY);
                    }
                    break;
                case ARROW:
                    BitBlt(hdc, x0 + temp->x * bmS[5].bmWidth,
                        y0 + temp->y * bmS[5].bmWidth,
                        bmS[5].bmWidth, bmS[5].bmHeight,
                        hdcMemList[5], 0, 0, SRCCOPY);
                    matriz[temp->x][temp->y] = ARROW;
                    break;
                case GOLD:
                    BitBlt(hdc, x0 + temp->x * bmS[4].bmWidth,
                        y0 + temp->y * bmS[4].bmWidth,
                        bmS[4].bmWidth, bmS[4].bmHeight,
                        hdcMemList[4], 0, 0, SRCCOPY);
                    matriz[temp->x][temp->y] = GOLD;
                    break;
                case BUSH:
                    BitBlt(hdc, x0 + temp->x * bmS[6].bmWidth,
                        y0 + temp->y * bmS[6].bmWidth,
                        bmS[6].bmWidth, bmS[6].bmHeight,
                        hdcMemList[6], 0, 0, SRCCOPY);
                    matriz[temp->x][temp->y] = BUSH;
                    matriz[temp->x][temp->y + 1] = BUSH;
                    matriz[temp->x][temp->y + 2] = BUSH;
                    break;
                case FENCE:
                    BitBlt(hdc, x0 + temp->x * bmS[7].bmWidth,
                        y0 + temp->y * bmS[7].bmWidth,
                        bmS[7].bmWidth, bmS[7].bmHeight,
                        hdcMemList[7], 0, 0, SRCCOPY);
                    matriz[temp->x][temp->y] = FENCE;
                    matriz[temp->x][temp->y + 1] = FENCE;
                    matriz[temp->x][temp->y + 2] = FENCE;
                    break;
                case WALL:
                    BitBlt(hdc, x0 + temp->x * bmS[8].bmWidth,
                        y0 + temp->y * bmS[8].bmWidth,
                        bmS[8].bmWidth, bmS[8].bmHeight,
                        hdcMemList[8], 0, 0, SRCCOPY);
                    matriz[temp->x][temp->y] = WALL;
                    matriz[temp->x][temp->y + 1] = WALL;
                    matriz[temp->x][temp->y + 2] = WALL;
                    break;
                
                }
                temp = temp->sig;
            }

            for (int i = 0; i < 9; i++)
            {
                SelectObject(hdcMemList[i], bmpAnts[i]);
                DeleteDC(hdcMemList[i]);
            }


            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
    {
        int i = 0;
        while (eliminarCabeza(&t));
        VaciarCola(&cola);

        DeleteObject(hpenNEGRO);
        

        for (i = 0; i < 9; i++)
        {
            DeleteObject(bmpImagenes[i]);
        }
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Controlador de mensajes del cuadro Acerca de.
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

long PopFileLength(FILE* file)
{
    int iCurrentPos, iFileLength;
    iCurrentPos = ftell(file);
    fseek(file, 0, SEEK_END);
    iFileLength = ftell(file);
    fseek(file, iCurrentPos, SEEK_SET);
    return iFileLength;
}

void  saveFile(char nombre[], HWND hWndEdt, HWND hWndVentana)
{
    FILE* file;
    int  iLength;
    PSTR pstrBuffer;
    TCHAR* ptchBuffer = NULL;
    TCHAR szFileF[MAX_PATH], szCaption[64 + _MAX_FNAME + _MAX_EXT];

    file = fopen(nombre, "w");
    iLength = GetWindowTextLength(hWndEdt);
    if (NULL == (pstrBuffer = (PSTR)malloc(sizeof(char) * (iLength + 1))) || NULL == (ptchBuffer = (TCHAR*)malloc(sizeof(TCHAR) * (iLength + 1)))) {

        MessageBox(hWndVentana, L"Error al reservar memoria", L"Error", MB_OK | MB_ICONERROR);
        fclose(file);

    }
    GetWindowText(hWndEdt, ptchBuffer, iLength + 1);
    wcstombs(pstrBuffer, ptchBuffer, iLength + 1);
    fwrite(pstrBuffer, 1, iLength + 1, file);
    fclose(file);
    free(pstrBuffer);
    free(ptchBuffer);

    mbstowcs(szFileF, cFile, strlen(cFile) + 1);
    wsprintf(szCaption, _T("%s - %s"), szTitle, szFileF[0] ? szFileF : _T("Sin archivo abierto"));
    SetWindowText(hWndVentana, szCaption);
}

UINT  saveFileAs(HWND hWndEdt, HWND hWndVentana) {

    TCHAR szFileF[MAX_PATH], szCaption[64 + _MAX_FNAME + _MAX_EXT];
    ZeroMemory(szFileF, MAX_PATH);
    OPENFILENAME ofno;
    ZeroMemory(&ofno, sizeof(OPENFILENAME));
    ofno.lStructSize = sizeof(OPENFILENAME);
    ofno.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
    ofno.hwndOwner = hWndVentana;
    ofno.lpstrFilter = _T("Tipos de formatos soportados(*.txt)\0 * .txt\0Texto(*.txt)\0\0");
    ofno.lpstrTitle = _T("Guardar archivo de texto");
    ofno.lpstrFile = szFileF;
    ofno.nMaxFile = MAX_PATH;

    if (IDOK == GetSaveFileName(&ofno)) {
        
        wsprintf(szCaption, _T("%s - %s"), szTitle, szFileF[0] ? szFileF : _T("Sin archivo abierto"));
        SetWindowText(hWndVentana, szCaption);

        FILE* file;
        int  iLength;
        PSTR pstrBuffer;
        TCHAR* ptchBuffer = NULL;
        wcstombs(cFile, szFileF, MAX_PATH);

        if (NULL == (file = fopen(cFile, "w"))) {  
            MessageBox(hWndVentana, L"Error al crear el archivo", L"Error", MB_OK | MB_ICONERROR);

        }
        else {
            iLength = GetWindowTextLength(hWndEdt);
            if (NULL == (pstrBuffer = (PSTR)malloc(sizeof(char) * (iLength + 1))) || NULL == (ptchBuffer = (TCHAR*)malloc(sizeof(TCHAR) * (iLength + 1)))) {

                MessageBox(hWndVentana, L"Error al reservar memoria", L"Error", MB_OK | MB_ICONERROR);
                fclose(file);

            }
            GetWindowText(hWndEdt, ptchBuffer, iLength + 1);
            wcstombs(pstrBuffer, ptchBuffer, iLength + 1);
            fwrite(pstrBuffer, 1, iLength + 1, file);
            fclose(file);
            free(pstrBuffer);
            free(ptchBuffer);

            return IDOK;
        }
    }
}


void compilar(HWND hWnd) {

    numlineas = 1;
    yyin = fopen(cFile, "r");

    if (yyin == NULL) {
        MessageBox(hWnd, L"No es posible cargar yyin", L"Error", MB_OK | MB_ICONERROR);
    }
    else {
        yyout = fopen("empty.txt", "w");
        if (yyout == NULL) {
            MessageBox(hWnd, L"No es posible crear yyout", L"Error", MB_OK | MB_ICONERROR);
        }
        else {

            if (!yyparse())
            {
                yyparse();
                bCompilacionCorrecta = TRUE;
                //limpiarListBox();
                SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"Compila bro uwu");
                fclose(yyout);
                fclose(yyin);

            }
            else
            {
                bCompilacionCorrecta = FALSE;
                
                SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"No compila bro uwu");
                fclose(yyout);
                fclose(yyin);
            }

        }
    }

}

CHOOSEFONT Seleccionar_Fuente(HWND hwnd, HFONT* hFont, COLORREF rgb)
{
    CHOOSEFONT cf = { sizeof(CHOOSEFONT) };
    LOGFONT lf;

    GetObject(*hFont, sizeof(LOGFONT), &lf);

    cf.Flags = CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
    cf.hwndOwner = hwnd;
    cf.lpLogFont = &lf;
    cf.rgbColors = rgb;

    if (ChooseFont(&cf))
    {
        HFONT hf = CreateFontIndirect(&lf);
        if (hf)
            *hFont = hf;
        else
            MessageBox(hwnd, L"Font creation failed!", L"Error", MB_OK | MB_ICONEXCLAMATION);
    }
    return cf;
}

void actualizar(HWND hWnd)
{
    
    simbolo* aux;
    char bus[] = " ";
    aux = buscar(t, bus, 1);
    NODO* actual;
    actual = cola.delante;

    if (aux)
    {
        //MessageBox(NULL, L"ANTES DE VERIFICAR LA COLA", L"Advertencia", MB_OK);
        if (!ColaStaVacia(cola)) {
            //MessageBox(NULL, L"DESPUES DE VERIFICAR LA COLA", L"Advertencia", MB_OK);
            if (actual->Accion == MOVE) {
                 
                if (aux->direccion == RIGHT && (actual->nveces) > 0)
                {
                    if (aux->x < 17) {
                        
                        if (matriz[aux->x+1][aux->y] == 0) {
                            matriz[aux->x][aux->y] = 0;
                            aux->x = aux->x + 1;
                            matriz[aux->x][aux->y] = 1;
                            cola.delante->nveces = cola.delante->nveces - 1;
                        }
                        else {
                            int tipo = matriz[aux->x + 1][aux->y];
                            detectObstaculo(tipo, hWnd, hLista);
                            
                        }

                    }
                    else {
                        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"No se puede ir mas alla bro uwu");
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, TBSTATE_ENABLED);
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, TBSTATE_ENABLED);
                        KillTimer(hWnd, IDT_TIMER);
                    }

                    

                }
                else if (aux->direccion == DIRNULL && (actual->nveces) > 0)
                {
                    aux->x = aux->x + 1;
                    cola.delante->nveces = cola.delante->nveces - 1;


                }
                else if (aux->direccion == LEFT && (actual->nveces) > 0)
                {
                    
                    if (aux->x > 0) {

                        if (matriz[aux->x - 1][aux->y] == 0) {
                            matriz[aux->x][aux->y] = 0;
                            aux->x = aux->x - 1;
                            matriz[aux->x][aux->y] = 1;
                            cola.delante->nveces = cola.delante->nveces - 1;
                        }
                        else {
                            int tipo = matriz[aux->x - 1][aux->y];
                            detectObstaculo(tipo, hWnd, hLista);
                            
                        }

                    }
                    else {
                        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"No se puede ir mas alla bro uwu");
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, TBSTATE_ENABLED);
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, TBSTATE_ENABLED);
                        KillTimer(hWnd, IDT_TIMER);
                    }

                    
                }
                else if (aux->direccion == UP && (actual->nveces) > 0)
                {
                    if (aux->y > 0) {

                        if (matriz[aux->x][aux->y-1] == 0) {
                            matriz[aux->x][aux->y] = 0;
                            aux->y = aux->y - 1;
                            matriz[aux->x][aux->y] = 1;
                            cola.delante->nveces = cola.delante->nveces - 1;
                        }
                        else {
                            int tipo = matriz[aux->x][aux->y-1];
                            detectObstaculo(tipo, hWnd, hLista);
                           
                        }

                    }
                    else {
                        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"No se puede ir mas alla bro uwu");
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, TBSTATE_ENABLED);
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, TBSTATE_ENABLED);
                        KillTimer(hWnd, IDT_TIMER);
                    }
                    
                }
                else if (aux->direccion == DOWN && (actual->nveces) > 0)
                {
                    if (aux->y < 9) {

                        if (matriz[aux->x][aux->y + 1] == 0) {
                            matriz[aux->x][aux->y] = 0;
                            aux->y = aux->y + 1;
                            matriz[aux->x][aux->y] = 1;
                            cola.delante->nveces = cola.delante->nveces - 1;
                        }
                        else {
                            int tipo = matriz[aux->x][aux->y + 1];
                            detectObstaculo(tipo, hWnd, hLista);
 
                        }
                    }
                    else {
                        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"No se puede ir mas alla bro uwu");
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, TBSTATE_ENABLED);
                        SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, TBSTATE_ENABLED);
                        KillTimer(hWnd, IDT_TIMER);
                    }
                   

                }
                else {
                    
                    cola.delante = cola.delante->sig;
                    free(actual);
                }

            }

            if (actual->Accion == TURN)
            {

                if (actual->dir == LEFT)
                {
                    if (aux->direccion == RIGHT)
                    {
                        aux->direccion = UP;
                        cola.delante = cola.delante->sig;
                        free(actual);
                    }
                    else if (aux->direccion == LEFT)
                    {
                        aux->direccion = DOWN;
                        cola.delante = cola.delante->sig;
                        free(actual);
                    }
                    else if (aux->direccion == UP)
                    {
                        aux->direccion = LEFT;
                        cola.delante = cola.delante->sig;
                        free(actual);
                    }
                    else if (aux->direccion == DOWN)
                    {
                        aux->direccion = RIGHT;
                        cola.delante = cola.delante->sig;
                        free(actual);
                    }
                }
                if (actual->dir == RIGHT)
                {
                    if (aux->direccion == RIGHT)
                    {
                        aux->direccion = DOWN;
                        cola.delante = cola.delante->sig;
                        free(actual);
                    }
                    else if (aux->direccion == LEFT)
                    {
                        aux->direccion = UP;
                        cola.delante = cola.delante->sig;
                        free(actual);
                    }
                    else if (aux->direccion == UP)
                    {
                        aux->direccion = RIGHT;
                        cola.delante = cola.delante->sig;
                        free(actual);
                    }
                    else if (aux->direccion == DOWN)
                    {
                        aux->direccion = LEFT;
                        cola.delante = cola.delante->sig;
                        free(actual);
                    }
                }

            }
        }
        else {
            
            bEjecutar = FALSE;
        }
        
    }
    else
    {
        MessageBox(NULL, L"aux es nulo", L"Debug", MB_OK);
        bEjecutar = FALSE;
    }
}

void detectObstaculo(int tipo, HWND hWnd, HWND hLista) {

    switch (tipo)
    {
    case GOLD:
        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"Golden Experience encontrado.");

        break;
    case ARROW:
        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"Flecha encontrada.");
        break;
    case BUSH:
        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"Obstaculo arbusto encontrado.");
        break;
    case FENCE:
        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"Obstaculo valla encontrada.");
        break;
    case WALL:
        SendMessage(hLista, LB_ADDSTRING, 0, (LPARAM)L"Obstaculo barrera encontrada.");
        break;
    default:
        break;
    }

    SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNEJECUTAR, TBSTATE_ENABLED);
    SendMessage(hWndToolbar, TB_SETSTATE, ID_BTNCOMPILAR, TBSTATE_ENABLED);
    KillTimer(hWnd, IDT_TIMER);
}

/*
void ColorearCodigo(HWND hWndEdit, unsigned char* Buffer, int tam, COLORREF rgb)
{
    CHARFORMAT2 cf;				//Formato del texto

    // El siguiente segmento de código da formato reemplazando texto 
    // normal por texto con formato
    memset(&cf, 0, sizeof cf);						// Se limpia la estructura del formato
    cf.cbSize = sizeof(CHARFORMAT2);					// Se fija el tamaño de la estructura
    cf.dwMask = CFM_COLOR; //| CFM_BACKCOLOR ;			// Se establece la mascara para que sea posible aplicar color al texto

    int i = 0, inicioo = 0, finn = 0;
    int token;
    char lexema[512];
    TCHAR tchLexema[512];
    /*
    while ((token = OT2(lexema, Buffer, &i, &inicioo, &finn, tam)) != FINCADENA)
    {
        switch (token)
        {
        case NUMERO:  cf.crTextColor = RGB(0, 0, 250); break;
        case MAS: case MENOS:  cf.crTextColor = RGB(255, 0, 255); break;
        case PROD: case DIV: case MOD: case EXP: cf.crTextColor = RGB(127, 50, 255); break;
        case PIZQ: case PDER: case CIZQ: case CDER: cf.crTextColor = RGB(127, 127, 127); break;
        case TOKENERROR: cf.dwMask = CFM_BACKCOLOR; cf.crBackColor = RGB(255, 0, 0); break;
        }

        mbstowcs(tchLexema, lexema, sizeof(lexema));									//Empatar(lexema);
        SendMessage(hWndEdit, EM_SETSEL, (WPARAM)inicioo, (LPARAM)finn);			//Se establece un rango de texto a seleccionar
        SendMessage(hWndEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);  //Se aplica el formato al rango seleccionado
        SendMessage(hWndEdit, EM_REPLACESEL, FALSE, (LPARAM)tchLexema);          //Se reemplaza el rango seleccionado con el nuevo texto y formato

        cf.dwMask = CFM_COLOR | CFM_BACKCOLOR;
        cf.crTextColor = rgb;
        cf.crBackColor = RGB(255, 255, 255);
        SendMessage(hWndEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
    }

    //mbstowcs(tchLexema, lexema, sizeof(lexema));									//Empatar(lexema);
    SendMessage(hWndEdit, EM_SETSEL, (WPARAM)inicioo, (LPARAM)finn);			//Se establece un rango de texto a seleccionar
    SendMessage(hWndEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);  //Se aplica el formato al rango seleccionado
    SendMessage(hWndEdit, EM_REPLACESEL, FALSE, (LPARAM)tchLexema);          //Se reemplaza el rango seleccionado con el nuevo texto y formato

    cf.dwMask = CFM_COLOR | CFM_BACKCOLOR;
    cf.crTextColor = rgb;
    cf.crBackColor = RGB(255, 255, 255);
    SendMessage(hWndEdit, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}*/