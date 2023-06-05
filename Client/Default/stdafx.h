// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once


#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <d3d9.h>
#include <d3dx9.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <process.h>
#include <fstream>

#include "Define.h"
#include "Engine_Include.h"
#include "Export_Function.h"

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;
extern _uint		g_iExplosion;

using namespace std;
