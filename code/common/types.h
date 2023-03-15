#pragma once

#include <limits>
#include <cstdint>
#include "../pbbslib/utilities.h"

#ifndef NDEBUG
#define debug(_body) \
  _body;
#else
#define debug(_body)
#endif

// define VERTEXLONG if there are more than MAX_UINT vertices
#if defined(VERTEXLONG)
typedef long intV;
typedef unsigned long uintV;
#define INT_V_MAX LONG_MAX
#define UINT_V_MAX ULONG_MAX
#else
typedef int intV;
typedef unsigned int uintV;
#define INT_V_MAX INT_MAX
#define UINT_V_MAX UINT_MAX
#endif

// define EDGELONG if there are more than MAX_UINT edges
#if defined(EDGELONG)
typedef long intE;
typedef unsigned long uintE;
#define INT_E_MAX LONG_MAX
#define UINT_E_MAX ULONG_MAX
#else
typedef int intE;
typedef unsigned int uintE;
#define INT_E_MAX INT_MAX
#define UINT_E_MAX UINT_MAX
#endif

// Defining data types for Key and Value

// define KEYLONG
#if defined(KEYLONG)
typedef long intK;
typedef unsigned long uintK;
#define INT_K_MAX LONG_MAX
#define UINT_K_MAX ULONG_MAX
#else
typedef int intK;
typedef unsigned int uintK;
#define INT_K_MAX INT_MAX
#define UINT_K_MAX UINT_MAX
#endif

// String Values
typedef char* strV;

typedef unsigned int timestamp;
typedef unsigned int ref_count;

constexpr size_t TOP_BIT = ((size_t)std::numeric_limits<long>::max()) + 1;
