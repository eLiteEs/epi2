#include <iostream>
#include <string>
#include <windows.h>
#include <commctrl.h>
#include <lmcons.h>  // Para `UNLEN`
#include <filesystem>
#include <urlmon.h>
#include <fstream>

#define ID_BUTTON 1
#define ID_BUTTON2 2
#define ID_EDITBOX 3
#define ID_LABEL 4
#define ID_LISTBOX 5
#define ID_ICON 6
#define ID_COMBOBOX 7
#define ID_PROGRESS 1001
#define ID_STATUS 1002
#define ID_BUTTON_FINISH 1003
#define ID_LABEL_STATUS 1004

HBRUSH hbrBkgnd; // Pincel para el fondo de la ventana
HFONT hFontLabel; // Fuente para el label
HICON hIcon; // Imagen

// Procedimiento de la ventana principal
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Procedimiento de la ventana secundaria
LRESULT CALLBACK StatusWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hwndProgress;
    static HWND hwndLabelStatus;
    static HWND hwndButtonFinish;

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CREATE:
            hwndProgress = CreateWindowEx(
                0,
                PROGRESS_CLASS,
                NULL,
                WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
                20, 70, 360, 30,
                hwnd,
                (HMENU)ID_PROGRESS,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );

            CreateWindow(
                "STATIC",
                "Installing...",
                WS_CHILD | WS_VISIBLE,
                20, 20, 360, 20,
                hwnd,
                (HMENU)ID_STATUS,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );

            hwndLabelStatus = CreateWindow(
                "STATIC",
                "Initializing installation...",
                WS_CHILD | WS_VISIBLE,
                20, 45, 360, 20,
                hwnd,
                (HMENU)ID_LABEL_STATUS,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
                NULL
            );

            hwndButtonFinish = CreateWindow(
                "BUTTON", "Close", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                400, 70, 100, 30,
                hwnd, (HMENU)ID_BUTTON_FINISH, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL
            );

            EnableWindow(hwndButtonFinish, FALSE); // Deshabilitar el botón inicialmente

            SendMessage(hwndProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
            SendMessage(hwndProgress, PBM_SETSTEP, (WPARAM)1, 0);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BUTTON_FINISH) {
                PostQuitMessage(0); // Terminar el programa cuando se pulse el botón Finish
            }
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Crear una ventana secundaria
HWND CreateStatusWindow(HINSTANCE hInstance) {
    const char CLASS_NAME[] = "Status Window Class";

    WNDCLASS wc = {};

    wc.lpfnWndProc = StatusWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hIcon = (HICON)LoadImage(
        NULL,
        "favicon.ico",
        IMAGE_ICON,
        0, 0,
        LR_LOADFROMFILE |
        LR_DEFAULTSIZE |
        LR_SHARED
    );
    RegisterClass(&wc);

    HWND hwndStatus = CreateWindowEx(
        0,
        CLASS_NAME,
        "Installation Status",
        WS_OVERLAPPEDWINDOW,
        GetSystemMetrics(SM_CXSCREEN) / 2 - 300, GetSystemMetrics(SM_CYSCREEN) / 2 - 150 / 2, 600, 150,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwndStatus == NULL) {
        return NULL;
    }

    ShowWindow(hwndStatus, SW_SHOW);
    UpdateWindow(hwndStatus);
    return hwndStatus;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Inicializar los controles comunes
    INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(icex);
    icex.dwICC = ICC_PROGRESS_CLASS;
    InitCommonControlsEx(&icex);

    const char CLASS_NAME[] = "epi2 Installer";

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hIcon = (HICON)LoadImage(
        NULL,
        "favicon.ico",
        IMAGE_ICON,
        0, 0,
        LR_LOADFROMFILE |
        LR_DEFAULTSIZE |
        LR_SHARED
    );

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "epi2 Installer",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        GetSystemMetrics(SM_CXSCREEN) / 2 - 300, GetSystemMetrics(SM_CYSCREEN) / 2 - 750 / 2, 600, 750,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    CreateWindow(
        "STATIC", "epi2 Installer:", WS_VISIBLE | WS_CHILD,
        50, 50, 150, 30,
        hwnd, (HMENU)ID_LABEL, hInstance, NULL);

    CreateWindow(
        "STATIC", "Select epi2 version to install", WS_VISIBLE | WS_CHILD,
        50, 68, 200, 30,
        hwnd, (HMENU)ID_LABEL, hInstance, NULL);

    HWND hwndComboBox = CreateWindow(
        "COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
        50, 86, 150, 200,
        hwnd, (HMENU)ID_COMBOBOX, hInstance, NULL);

    SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"epi2-v_0.1");
    //SendMessage(hwndComboBox, CB_ADDSTRING, 0, (LPARAM)"epi2-v_0.2");

    hIcon = (HICON)LoadImage(
        NULL,
        "favicon.ico",
        IMAGE_ICON,
        0, 0,
        LR_LOADFROMFILE |
        LR_DEFAULTSIZE |
        LR_SHARED
    );

    HWND hwndIcon = CreateWindow(
        "STATIC", NULL, WS_VISIBLE | WS_CHILD | SS_ICON,
        10, 50, 64, 64,
        hwnd, (HMENU)ID_ICON, hInstance, NULL);

    SendMessage(hwndIcon, STM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hIcon);

    CreateWindow(
        "BUTTON", "Cancel", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        50, 116, 100, 30,
        hwnd, (HMENU)ID_BUTTON, hInstance, NULL);

    CreateWindow(
        "BUTTON", "Install epi2", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        156, 116, 100, 30,
        hwnd, (HMENU)ID_BUTTON2, hInstance, NULL);

    hbrBkgnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(hbrBkgnd);
    return 0;
}


std::string GetCurrentUsername() {
    char username[UNLEN + 1];
    DWORD username_len = sizeof(username);

    if (GetUserNameA(username, &username_len)) {
        return std::string(username);
    } else {
        throw std::runtime_error("Error al obtener el nombre de usuario.");
    }
}
void UpdatePathVariable(const std::string& newPath) {
    // Obtener la variable PATH actual
    const char* currentPath = std::getenv("PATH");
    if (currentPath == nullptr) {
        std::cerr << "Error al obtener la variable PATH actual." << std::endl;
        return;
    }

    // Crear el nuevo valor de PATH
    std::string newPathVar = std::string(currentPath) + ";" + newPath;

    // Crear un archivo por lotes para actualizar PATH
    std::ofstream batchFile("update_path.bat");
    if (!batchFile) {
        std::cerr << "Error al crear el archivo por lotes." << std::endl;
        return;
    }

    batchFile << "@echo off\n";
    batchFile << "set PATH=" << newPathVar << "\n";
    batchFile << "setx PATH \"" << newPathVar << "\"\n";
    batchFile.close();

    // Ejecutar el archivo por lotes
    if (system("update_path.bat") == 0) {
        std::cout << "Variable PATH actualizada exitosamente." << std::endl;
    } else {
        std::cerr << "Error al ejecutar el archivo por lotes." << std::endl;
    }

    // Eliminar el archivo por lotes
    std::remove("update_path.bat");
}
void SetFileAssociation(const std::wstring& extension, const std::wstring& progId, const std::wstring& application, const std::wstring& args, const std::wstring& description) {
    HKEY hKey;
    LONG result;

    // Set the default value for the file extension key
    std::wstring extKey = L"Software\\Classes\\" + extension;
    result = RegCreateKeyExW(HKEY_CURRENT_USER, extKey.c_str(), 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL);
    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Error creating key for extension: " << result << std::endl;
        return;
    }

    result = RegSetValueExW(hKey, NULL, 0, REG_SZ, (const BYTE*)progId.c_str(), (progId.size() + 1) * sizeof(wchar_t));
    RegCloseKey(hKey);
    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Error setting ProgID for extension: " << result << std::endl;
        return;
    }

    // Set the default value for the ProgID key
    std::wstring progIdKey = L"Software\\Classes\\" + progId;
    result = RegCreateKeyExW(HKEY_CURRENT_USER, progIdKey.c_str(), 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL);
    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Error creating key for ProgID: " << result << std::endl;
        return;
    }

    // Set the description for the ProgID
    result = RegSetValueExW(hKey, NULL, 0, REG_SZ, (const BYTE*)description.c_str(), (description.size() + 1) * sizeof(wchar_t));
    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Error setting description for ProgID: " << result << std::endl;
        RegCloseKey(hKey);
        return;
    }

    // Set the shell command for the ProgID
    std::wstring shellKey = progIdKey + L"\\shell\\open\\command";
    result = RegCreateKeyExW(HKEY_CURRENT_USER, shellKey.c_str(), 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL);
    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Error creating shell command key: " << result << std::endl;
        return;
    }

    // Set the command line for the ProgID
    std::wstring command = L"\"" + application + L"\" " + args + L" \"%1\"";
    result = RegSetValueExW(hKey, NULL, 0, REG_SZ, (const BYTE*)command.c_str(), (command.size() + 1) * sizeof(wchar_t));
    RegCloseKey(hKey);
    if (result != ERROR_SUCCESS) {
        std::wcerr << L"Error setting command for ProgID: " << result << std::endl;
        return;
    }

    // Also set the description for the file type (just in case)
    std::wstring fileTypeKey = L"Software\\Classes\\" + extension + L"\\OpenWithProgids";
    result = RegCreateKeyExW(HKEY_CURRENT_USER, fileTypeKey.c_str(), 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL);
    if (result == ERROR_SUCCESS) {
        result = RegSetValueExW(hKey, progId.c_str(), 0, REG_NONE, NULL, 0);
        RegCloseKey(hKey);
        if (result != ERROR_SUCCESS) {
            std::wcerr << L"Error setting OpenWithProgids for extension: " << result << std::endl;
        }
    } else {
        std::wcerr << L"Error creating OpenWithProgids key: " << result << std::endl;
    }
}

void createfolder(std::string name) {
    try {
        if (std::filesystem::create_directories(name)) {
            } else {
            }
    } catch (const std::filesystem::filesystem_error& e) {
        }
}
void DownloadFile(const std::wstring& url, const std::wstring& filename) {
    HRESULT result = URLDownloadToFileW(NULL, url.c_str(), filename.c_str(), 0, NULL);
    if (SUCCEEDED(result)) {
        } else {
        }
}
std::wstring stringToWstring(const std::string& str) {
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
    return wstr;
}
bool exists(std::string filename) {
    std::fstream f(filename);
    bool r = f.good();
    f.close();
    return r;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND hwndStatus;

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetBkColor(hdcStatic, GetSysColor(COLOR_WINDOW));
            return (INT_PTR)hbrBkgnd;
        }

        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case ID_BUTTON:
                    DestroyWindow(hwnd);
                    break;
                case ID_BUTTON2: {
                    HWND hwndComboBox = GetDlgItem(hwnd, ID_COMBOBOX);
                    int selectedIndex = SendMessage(hwndComboBox, CB_GETCURSEL, 0, 0);
                    if (selectedIndex != CB_ERR) {
                        char selectedText[256];
                        SendMessage(hwndComboBox, CB_GETLBTEXT, selectedIndex, (LPARAM)selectedText);

                        // Crear y mostrar la ventana secundaria
                        hwndStatus = CreateStatusWindow((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE));
                        int p = 0, c = 100 / 5;

                        // Crear carpeta
                        std::string username = GetCurrentUsername();
                        SetWindowText(GetDlgItem(hwndStatus, ID_LABEL_STATUS), "Deleting previous epi2 instalations...");
                        p += c;
                        SendMessage(GetDlgItem(hwndStatus, ID_PROGRESS), PBM_SETPOS, p, 0);
                        std::filesystem::path directoryToDelete = "C:\\Users\\" + username + "\\epi2\\";

                        try {
                            if (std::filesystem::exists(directoryToDelete)) {
                                std::filesystem::remove_all(directoryToDelete);
                            } else {
                            }
                        } catch (const std::filesystem::filesystem_error& e) {
                        }


                        SetWindowText(GetDlgItem(hwndStatus, ID_LABEL_STATUS), "Creating epi2 folder...");
                        p += c;
                        SendMessage(GetDlgItem(hwndStatus, ID_PROGRESS), PBM_SETPOS, p, 0);
                        createfolder("C:\\Users\\" + username + "\\epi2\\");

                        // Descargar archivos
                        
                        SetWindowText(GetDlgItem(hwndStatus, ID_LABEL_STATUS), "Downloading epi2...");
                        p += c;
                        SendMessage(GetDlgItem(hwndStatus, ID_PROGRESS), PBM_SETPOS, p, 0);
                        std::string s(selectedText);
                        DownloadFile(stringToWstring("https://elitees.github.io/epi2-downloads/" + s + "/epi.exe" ), stringToWstring("C:\\Users\\" + username + "\\epi2\\epi.exe"));

                        // Configurar PATH
                        SetWindowText(GetDlgItem(hwndStatus, ID_LABEL_STATUS), "Updating Registry variable...");
                        p += c;
                        SendMessage(GetDlgItem(hwndStatus, ID_PROGRESS), PBM_SETPOS, p, 0);
                        SetFileAssociation(L".epi2", L"epi.exe", stringToWstring("C:\\Users\\" + username + "\\epi2\\epi.exe"), L"--from-explorer", L"epi2 Source file");

                        // Finalizar
                        SetWindowText(GetDlgItem(hwndStatus, ID_LABEL_STATUS), "Checking if epi2 instalation was succesfull...");
                        p += c;
                        SendMessage(GetDlgItem(hwndStatus, ID_PROGRESS), PBM_SETPOS, p, 0);

                        if(exists("C:\\Users\\" + username + "\\epi2\\epi.exe")) {
                            EnableWindow(GetDlgItem(hwndStatus, ID_BUTTON_FINISH), TRUE);
                            SetWindowText(GetDlgItem(hwndStatus, ID_LABEL_STATUS), "Instalation succesfull!");
                        } else {
                            EnableWindow(GetDlgItem(hwndStatus, ID_BUTTON_FINISH), TRUE);
                            SetWindowText(GetDlgItem(hwndStatus, ID_LABEL_STATUS), "Instalation failed, try downloading epi2 on the web");    
                        }
                    }
                    break;
                }
            }
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
