#pragma once

//#ifndef VC_EXTRALEAN //(会造成RichEdit无法编译)
//#define VC_EXTRALEAN
//#endif

#ifndef WIN32_LEAN_AND_MEAN 
#define WIN32_LEAN_AND_MEAN	
#endif


#ifndef WINVER				
#define WINVER 0x0500	
#endif

#ifndef _WIN32_WINNT		
#define _WIN32_WINNT 0x0500
#endif						

#ifndef _WIN32_WINDOWS	
#define _WIN32_WINDOWS 0x0500 
#endif

#ifndef _WIN32_IE			
#define _WIN32_IE 0x0500	
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#pragma warning (disable : 4018)
#pragma warning (disable : 4244)
#pragma warning (disable : 4267)
#pragma warning (disable : 4311)

#include <afxwin.h>    
#include <afxext.h>     

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         
#include <afxodlgs.h>       
#include <afxdisp.h>      
#endif

//宏定义
#define lengthof(x) (sizeof(x)/sizeof(*x))
#define MAX max
#define MIN min
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))

#ifdef UNICODE
#define _tcssprintf wsprintf
#define tcsplitpath _wsplitpath
#else
#define _tcssprintf sprintf
#define tcsplitpath _splitpath
#endif

//引入文件
#include <windowsx.h>
#include "Core/XUnzip.h"
#include "FGuilibHead.h"
