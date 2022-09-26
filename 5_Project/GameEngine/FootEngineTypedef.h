#pragma once

// ���� ����ϴ� ������ϵ��� �����ص� ��� ����
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

// ���� ��Ʈ���� int�� ��Ī�� �����ش�
typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

// ���� ������Ʈ ������ ���� ��Ʈ���� ������ֱ� ����..
#include <xstring>

// ������ ���� Math Ŭ������ ����Ѵ�.
#include "../HeraclesMath/HeraclesMath.h"
using namespace HeraclesMath;

#pragma comment(lib, "../MathLibrary/HeraclesMath.lib")
#pragma comment(lib, "XInput.lib")

#if defined(UNICODE)
using tstring = std::wstring;
#else
using tstring = std::string;
#endif