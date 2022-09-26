#pragma once

// 자주 사용하는 헤더파일들을 정리해둔 헤더 파일
#include <Windows.h>
#include <memory>
#include <tchar.h>
#include <assert.h>
#include <functional>
#include <string>
#include <set>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <queue>

// 각종 비트단위 int의 별칭을 정해준다
typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

// 현재 프로젝트 설정에 따라 스트링을 사용해주기 위해..
#include <xstring>

// 지훈이 형의 Math 클래스를 사용한다.
#include "../HeraclesMath/HeraclesMath.h"
using namespace HeraclesMath;

#pragma comment(lib, "../MathLibrary/HeraclesMath.lib")
#pragma comment(lib, "XInput.lib")

#if defined(UNICODE)
using tstring = std::wstring;
#else
using tstring = std::string;
#endif