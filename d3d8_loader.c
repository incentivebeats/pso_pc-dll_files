#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef void* (WINAPI *Direct3DCreate8_t)(UINT SDKVersion);

static HMODULE g_self = NULL;
static HMODULE g_psopol = NULL;
static Direct3DCreate8_t g_psopol_Direct3DCreate8 = NULL;

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD reason, LPVOID reserved) {
    (void)reserved;

    if (reason == DLL_PROCESS_ATTACH) {
        g_self = (HMODULE)hinst;
        DisableThreadLibraryCalls(hinst);
    }

    return TRUE;
}

static void dirname_in_place(char* path) {
    char* last_slash = NULL;
    char* p = path;

    while (*p) {
        if (*p == '\\' || *p == '/') {
            last_slash = p;
        }
        p++;
    }

    if (last_slash) {
        last_slash[1] = 0;
    } else {
        path[0] = 0;
    }
}

static int load_psopol(void) {
    char self_path[MAX_PATH];
    char dll_path[MAX_PATH];

    if (g_psopol_Direct3DCreate8) {
        return 1;
    }

    self_path[0] = 0;
    dll_path[0] = 0;

    if (!g_self) {
        return 0;
    }

    if (GetModuleFileNameA(g_self, self_path, sizeof(self_path)) == 0) {
        return 0;
    }

    dirname_in_place(self_path);

    lstrcpynA(dll_path, self_path, sizeof(dll_path));
    lstrcatA(dll_path, "psopol.dll");

    g_psopol = LoadLibraryA(dll_path);
    if (!g_psopol) {
        g_psopol = LoadLibraryA("psopol.dll");
    }
    if (!g_psopol) {
        MessageBoxA(NULL, "Failed to load psopol.dll", "D3D8 loader", MB_ICONERROR | MB_OK);
        return 0;
    }

    g_psopol_Direct3DCreate8 = (Direct3DCreate8_t)GetProcAddress(g_psopol, "Direct3DCreate8");
    if (!g_psopol_Direct3DCreate8) {
        MessageBoxA(NULL, "psopol.dll does not export Direct3DCreate8", "D3D8 loader", MB_ICONERROR | MB_OK);
        return 0;
    }

    return 1;
}

__declspec(dllexport)
void* WINAPI Direct3DCreate8(UINT SDKVersion) {
    if (!load_psopol()) {
        return NULL;
    }

    return g_psopol_Direct3DCreate8(SDKVersion);
}
