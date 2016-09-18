#ifndef SETUP_CPP_H
#define SETUP_CPP_H

#ifdef _MSC_VER

  #ifdef libxl_EXPORTS
      #define XLAPI __declspec(dllexport)
  #else
      #define XLAPI __declspec(dllimport)
  #endif

  #define XLAPIENTRY __cdecl

#else

  #define XLAPI
  #define XLAPIENTRY

#endif

#endif
