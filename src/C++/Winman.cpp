/*
 * Java-Direct3D Wrapper
 * Copyright (c) 2006 Matthew Ross
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
 * OR OTHER DEALINGS IN THE SOFTWARE.
 */


#define _WIN32_WINNT 0x0500
#define WINVER 0x0500
#define STRICT
#define _CRT_SECURE_NO_DEPRECATE


#include <windows.h>
#include <tchar.h>
#include <psapi.h>
#include <jawt_md.h>
#include <map>
#include <assert.h>
#include <stdio.h>
#include "jdw_graphics_WindowManager.h"
#include "jdw_graphics_NativeCanvas.h"
using namespace std;


// Macros //////////////////////////////////////////////////////////////////

typedef void (*RENDERPROC)(HWND, HDC);
typedef void (*RENDERPROC2)(JNIEnv*, jobject, JAWT_Win32DrawingSurfaceInfo*);
typedef jboolean (*AWTPROC)(JNIEnv*, JAWT*);

#define START_J2AWT(env, canvas, dsi_win) { \
    jboolean result; \
    jint lock; \
    awt.version = JAWT_VERSION_1_4; \
    AWTPROC getAwtProc = FindGetAWTProc(); \
    assert(getAwtProc != NULL); \
    result = getAwtProc((env), &awt); \
    assert(result != JNI_FALSE); \
    ds = awt.GetDrawingSurface((env), (canvas)); \
    assert(ds != NULL); \
    lock = ds->Lock(ds); \
    assert((lock & JAWT_LOCK_ERROR) == 0); \
    dsi = ds->GetDrawingSurfaceInfo(ds); \
    (dsi_win) = static_cast<JAWT_Win32DrawingSurfaceInfo*>(dsi->platformInfo); \
} \

#define END_J2AWT() { \
    ds->FreeDrawingSurfaceInfo(dsi); \
    ds->Unlock(ds); \
    awt.FreeDrawingSurface(ds); \
} \

typedef struct _NATIVE_CANVAS {
    LPSTR libName;
    HMODULE PaintModule;
    RENDERPROC rp;
    RENDERPROC2 rp2;
} NATIVE_CANVAS;

// Global variables ////////////////////////////////////////////////////////
map<jobject,NATIVE_CANVAS*> g_canvasMap;
CRITICAL_SECTION g_critsec;
LONG_PTR g_origWndProc;

// Function prototypes /////////////////////////////////////////////////////
void InvokePaintPlugin(NATIVE_CANVAS*, JNIEnv*, jobject, 
                        JAWT_Win32DrawingSurfaceInfo*, bool);
char* GetLibName(JNIEnv* env, jobject canvas);
LRESULT CALLBACK SubclassProc(HWND hwnd, UINT uMsg,
                            WPARAM wParam, LPARAM lParam);
void ThrowJavaException(JNIEnv* env, char* message);
void CleanUp();
AWTPROC FindGetAWTProc();
void GetJawtPath(TCHAR pathRef[]);
////////////////////////////////////////////////////////////////////////////

/*
 * Class:     WindowManager
 * Method:    setDraggable
 * Signature: (Ljava/awt/Window;Z)V
 */
JNIEXPORT void JNICALL 
Java_jdw_graphics_WindowManager_setDraggable(JNIEnv* env, jclass, 
    jobject win, jboolean drag)
{
    JAWT awt;
    JAWT_DrawingSurface* ds;
    JAWT_DrawingSurfaceInfo* dsi;
    JAWT_Win32DrawingSurfaceInfo* dsi_win;
    START_J2AWT(env, win, dsi_win);
    HWND hwnd = dsi_win->hwnd;

    if (JNI_TRUE == drag && NULL == g_origWndProc)
    {
        g_origWndProc = SetWindowLongPtr(hwnd, GWLP_WNDPROC, 
            reinterpret_cast<LONG_PTR>(&SubclassProc));
    }
    else if (JNI_FALSE == drag && g_origWndProc != NULL)
    {
        SetWindowLongPtr(hwnd, GWLP_WNDPROC, g_origWndProc);
        g_origWndProc = NULL;
    }
    END_J2AWT();
}

/*
 * Class:     WindowManager
 * Method:    setOpacity
 * Signature: (Ljava/awt/Window;I)V
 */
JNIEXPORT void JNICALL 
Java_jdw_graphics_WindowManager_setOpacity(JNIEnv* env, jclass, jobject win, jint opacity)
{
    JAWT awt;
    JAWT_DrawingSurface* ds;
    JAWT_DrawingSurfaceInfo* dsi;
    JAWT_Win32DrawingSurfaceInfo* dsi_win;
    START_J2AWT(env, win, dsi_win);
    HWND hwnd = dsi_win->hwnd;

    if (opacity > 255)
        opacity = 255;

    if (opacity >= 0)
    {
        // make window transparent
        LONG_PTR l = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
        l |= WS_EX_LAYERED;
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, l);

        // set opacity
        SetLayeredWindowAttributes(hwnd, 0, (BYTE)opacity, LWA_ALPHA);
    }
    else // opacity < 0
    {
        // turn off window transparency
        LONG_PTR l = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
        l &= ~WS_EX_LAYERED;
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, l);
    }
    END_J2AWT();
}

/*
 * Class:     WindowManager
 * Method:    getHWND
 * Signature: (Ljava/awt/Component;)I
 */
JNIEXPORT jint JNICALL 
Java_jdw_graphics_WindowManager_getHWND(JNIEnv* env, jclass, jobject comp)
{
    JAWT awt;
    JAWT_DrawingSurface* ds;
    JAWT_DrawingSurfaceInfo* dsi;
    JAWT_Win32DrawingSurfaceInfo* dsi_win;
    START_J2AWT(env, comp, dsi_win);

    jint hwnd = reinterpret_cast<jint>(dsi_win->hwnd);

    END_J2AWT();
    return hwnd;
}

/*
 * Class:     WindowManager
 * Method:    fadeIn
 * Signature: (Ljava/awt/Window;)V
 */
JNIEXPORT void JNICALL 
Java_jdw_graphics_WindowManager_fadeIn(JNIEnv* env, jclass, jobject win)
{
    JAWT awt;
    JAWT_DrawingSurface* ds;
    JAWT_DrawingSurfaceInfo* dsi;
    JAWT_Win32DrawingSurfaceInfo* dsi_win;
    START_J2AWT(env, win, dsi_win);

    HWND hwnd = dsi_win->hwnd;
    AnimateWindow(hwnd, 4000, AW_BLEND);

    END_J2AWT();
}

/*
 * Class:     WindowManager
 * Method:    fadeOut
 * Signature: (Ljava/awt/Window;)V
 */
JNIEXPORT void JNICALL 
Java_jdw_graphics_WindowManager_fadeOut(JNIEnv* env, jclass, jobject win)
{
    JAWT awt;
    JAWT_DrawingSurface* ds;
    JAWT_DrawingSurfaceInfo* dsi;
    JAWT_Win32DrawingSurfaceInfo* dsi_win;
    START_J2AWT(env, win, dsi_win);

    HWND hwnd = dsi_win->hwnd;
    AnimateWindow(hwnd, 4000, AW_BLEND|AW_HIDE);

    END_J2AWT();
}

/*
 * Class:     gfx_NativeCanvas
 * Method:    getNativePtr
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_jdw_graphics_NativeCanvas_getNativePtr
  (JNIEnv *, jclass)
{
    return 0;
}

/*
 * Class:     NativeCanvas
 * Method:    paint
 * Signature: (Ljava/awt/Graphics;)V
 */
JNIEXPORT void JNICALL
Java_jdw_graphics_NativeCanvas_paint(JNIEnv* env, jobject canvas, jobject /*graphics*/)
{
    try
    {
        EnterCriticalSection(&g_critsec);
        // Get NATIVE_CANVAS* from global map
        NATIVE_CANVAS* currCanvas = NULL;
        bool isNew = false;
        map<jobject,NATIVE_CANVAS*>::iterator i = g_canvasMap.find(canvas);
        if (i == g_canvasMap.end())
        {//create new
            currCanvas = new NATIVE_CANVAS;
            assert(currCanvas != NULL);
            ZeroMemory(currCanvas, sizeof(NATIVE_CANVAS));
            g_canvasMap[canvas] = currCanvas;
            isNew = true;
        }
        else
        {
            currCanvas = i->second;
        }
        
        JAWT awt;
        JAWT_DrawingSurface* ds;
        JAWT_DrawingSurfaceInfo* dsi;
        JAWT_Win32DrawingSurfaceInfo* dsi_win;
        START_J2AWT(env, canvas, dsi_win);
    
        if (currCanvas->libName == 0)
            currCanvas->libName = GetLibName(env, canvas);
        InvokePaintPlugin(currCanvas, env, canvas, dsi_win, isNew);

        END_J2AWT();
        //LeaveCriticalSection(&g_critsec); // no!
    }
    catch(...)
    {
    }
    LeaveCriticalSection(&g_critsec);
}

////////////////////////////////////////////////////////////////////////////
// Private functions
////////////////////////////////////////////////////////////////////////////

void InvokePaintPlugin(NATIVE_CANVAS* currCanvas, JNIEnv* env, jobject canvas, 
	JAWT_Win32DrawingSurfaceInfo* drawInfo, bool allowLoad)
{
    if (NULL == currCanvas->PaintModule 
        && currCanvas->libName != NULL && allowLoad)
    {
        // Paint DLL not loaded yet, so load it
        currCanvas->PaintModule = LoadLibraryA(currCanvas->libName);
        if (currCanvas->PaintModule != NULL)
        {
            // try to find Render2 function first
            currCanvas->rp2 = NULL;
            currCanvas->rp = NULL;
            void* proc = GetProcAddress(currCanvas->PaintModule, "Render2");
            if (proc != NULL)
            {
                currCanvas->rp2 = static_cast<RENDERPROC2>(proc);
            }

            if (NULL == currCanvas->rp2)
            {
                // try to find Render function
                
                proc = GetProcAddress(currCanvas->PaintModule, "Render");

                if (proc != NULL)
                {
                    currCanvas->rp = static_cast<RENDERPROC>(proc);
                }
                else
                {
                    // throw Java exception
                    ThrowJavaException(env, "Render[2] method not found");
                }
            }
        }
        else
        {
            // throw Java exception
            ThrowJavaException(env, "Library not found");
        }
    }
    if (currCanvas->rp2 != NULL)
    {
        currCanvas->rp2(env, canvas, drawInfo);
    }
    else if (currCanvas->rp != NULL)
    {
        currCanvas->rp(drawInfo->hwnd, drawInfo->hdc);
    }
}

char* GetLibName(JNIEnv* env, jobject canvas)
{
    assert(env != NULL);
    assert(canvas != NULL);

    char* libName = NULL;
    jclass cls = env->GetObjectClass(canvas);
    jfieldID fid = env->GetFieldID(cls, "libName",
        "Ljava/lang/String;");
    if (NULL == fid)
    {
        return NULL;
    }
    jstring jstr = static_cast<jstring>(env->GetObjectField(canvas, fid));
    const char* str = env->GetStringUTFChars(jstr, NULL);
    if (NULL == str)
    {
        return NULL;
    }
    libName = _strdup(str);
    env->ReleaseStringUTFChars(jstr, str);
    return libName;
}


LRESULT CALLBACK SubclassProc(HWND hwnd, UINT uMsg,
                            WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_NCHITTEST:
        LRESULT uHitTest = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
        //printf("%d ", uHitTest);
        if (HTCLIENT == uHitTest)
            return HTCAPTION;
        else
            return uHitTest;
    }

    return CallWindowProc(reinterpret_cast<WNDPROC>(g_origWndProc), 
                          hwnd, uMsg, wParam, lParam);
}

void CleanUp()
{
    map<jobject,NATIVE_CANVAS*>::iterator i = g_canvasMap.begin();

    while (i != g_canvasMap.end())
    {
        NATIVE_CANVAS* c = i->second;
        if (c)
        {
            if (c->libName)
            {
                free(c->libName);
            }
            if (c->PaintModule)
            {
                FreeLibrary(c->PaintModule);
            }
            delete c;
        }
        i++;
    }
}

void ThrowJavaException(JNIEnv* env, char* message)
{
    jclass exClass = env->FindClass("java/lang/Exception");
    env->ThrowNew(exClass, message);
}

AWTPROC FindGetAWTProc()
{
    static bool previousCall = false;
    static AWTPROC awtProc = NULL;
    
    if (previousCall)   // Only need to load jawt.dll once
    {
        return awtProc;
    }
    
    previousCall = true;
    
    //
    // Need to know where jawt.dll actually is
    //
    TCHAR jawtPath[MAX_PATH];
    ZeroMemory(jawtPath, MAX_PATH);
    GetJawtPath(jawtPath);
    
    //
    // Load jawt.dll
    //
    HMODULE modJawt = LoadLibrary(jawtPath);
    if (modJawt)
    {
#if defined _WIN64
        FARPROC procTemp = GetProcAddress(modJawt, "JAWT_GetAWT");
#else
        FARPROC procTemp = GetProcAddress(modJawt, "_JAWT_GetAWT@8");
#endif
        if (procTemp)
        {
            awtProc = reinterpret_cast<AWTPROC>(procTemp);
        }
        else
        {
#if defined _DEBUG
            printf("JAWT_GetAWT not found\n");
#endif
        }
    }
    else
    {
#if defined _DEBUG
        printf("jawt.dll not loaded\n");
#endif
    }
    
    return awtProc;
}

void GetJawtPath(TCHAR pathRef[])
{
    //
    // Use PSAPI to find the full path of the loaded jvm.dll
    //
    
    HMODULE hMods[1024];
    DWORD cbNeeded;

    HANDLE hProcess = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
        FALSE, GetCurrentProcessId());
    
    if (NULL == hProcess)
    {
        return;
    }
    
    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR szModName[MAX_PATH];
            ZeroMemory(szModName, MAX_PATH);

            // Get the full path to the module's file
            if (GetModuleFileNameEx(hProcess, hMods[i], szModName,
                                      sizeof(szModName)/sizeof(TCHAR)))
            {
                // If module name ends with jvm.dll guess that it's
                // the Java Virtual Machine
                if (_tcslen(szModName) >= 7 
                    && 0 == _tcsicmp(&szModName[_tcslen(szModName)-7], 
                        _T("jvm.dll")))
                {
                    // jawt.dll is in the directory above
                    _tcsncpy(pathRef, szModName, _tcslen(szModName)-7);
                    _tcscat(pathRef, _T("..\\jawt.dll"));
                    break;
                }
            }
        }
    }

    CloseHandle(hProcess);
}

BOOL APIENTRY DllMain(HANDLE /*hModule*/, DWORD dwReason, LPVOID /*reserved*/)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        InitializeCriticalSection(&g_critsec);
        g_origWndProc = NULL;
        break;
    case DLL_PROCESS_DETACH:
        CleanUp();
        DeleteCriticalSection(&g_critsec);
        break;
    }

    return TRUE;
}

