//////////////////////////////////////////////////////////////////////

#pragma once
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

//////////////////////////////////////////////////////////////////////

#include "targetver.h"

//////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <d3d.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////

#define null nullptr

typedef unsigned int		uint;

typedef unsigned __int8		uint8;
typedef unsigned __int16	uint16;
typedef unsigned __int32	uint32;
typedef unsigned __int64	uint64;

typedef __int8				int8;
typedef __int16				int16;
typedef __int32				int32;
typedef __int64				int64;

using std::wstring;
using std::string;
using std::vector;
using DirectX::XMMATRIX;

//////////////////////////////////////////////////////////////////////

#include "D3D.h"
#include "Util.h"
#include "Vertex.h"
#include "Texture.h"
#include "Font.h"
#include "Line.h"
#include "Quad.h"