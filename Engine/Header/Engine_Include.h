#ifndef Engine_Include_h__
#define Engine_Include_h__

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
#include <cstdint>
#include <thread>

#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#include "Engine_Typedef.h"
#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Template.h"

#pragma warning(disable : 4251)

#include <io.h>

#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;
using namespace Engine;

#endif // Engine_Include_h__










