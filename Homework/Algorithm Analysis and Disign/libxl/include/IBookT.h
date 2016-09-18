#ifndef IBOOKT_H
#define IBOOKT_H

#include <stddef.h>
#include "setup.h"
#include "enum.h"

namespace libxl {

    template<class TCHAR> struct ISheetT;
    template<class TCHAR> struct IFormatT;
    template<class TCHAR> struct IFontT;

    template<class TCHAR>
    struct IBookT
    {
        virtual             bool XLAPIENTRY load(const TCHAR* filename) = 0;
        virtual             bool XLAPIENTRY save(const TCHAR* filename) = 0;

        virtual             bool XLAPIENTRY loadRaw(const char* data, unsigned size) = 0;
        virtual             bool XLAPIENTRY saveRaw(const char** data, unsigned* size) = 0;

        virtual  ISheetT<TCHAR>* XLAPIENTRY addSheet(const TCHAR* name, ISheetT<TCHAR>* initSheet = 0) = 0;
        virtual  ISheetT<TCHAR>* XLAPIENTRY getSheet(int index) const = 0;
        virtual             bool XLAPIENTRY delSheet(int index) = 0;
        virtual              int XLAPIENTRY sheetCount() const = 0;

        virtual IFormatT<TCHAR>* XLAPIENTRY addFormat(IFormatT<TCHAR>* initFormat = 0) = 0;
        virtual   IFontT<TCHAR>* XLAPIENTRY addFont(IFontT<TCHAR>* initFont = 0) = 0;
        virtual              int XLAPIENTRY addCustomNumFormat(const TCHAR* customNumFormat) = 0;
        virtual     const TCHAR* XLAPIENTRY customNumFormat(int fmt) = 0;

        virtual IFormatT<TCHAR>* XLAPIENTRY format(int index) = 0;
        virtual              int XLAPIENTRY formatSize() = 0;

        virtual   IFontT<TCHAR>* XLAPIENTRY font(int index) = 0;
        virtual              int XLAPIENTRY fontSize() = 0;

        virtual           double XLAPIENTRY datePack(int year, int month, int day, int hour = 0, int min = 0, int sec = 0, int msec = 0) = 0;
        virtual             bool XLAPIENTRY dateUnpack(double value, int* year, int* month, int* day, int* hour = 0, int* min = 0, int* sec = 0, int* msec = 0) = 0;

        virtual            Color XLAPIENTRY colorPack(int red, int green, int blue) = 0;
        virtual             void XLAPIENTRY colorUnpack(Color color, int* red, int* green, int* blue) = 0;

        virtual              int XLAPIENTRY activeSheet() const = 0;
        virtual             void XLAPIENTRY setActiveSheet(int index) = 0;

        virtual              int XLAPIENTRY addPicture(const TCHAR* filename) = 0;
        virtual              int XLAPIENTRY addPicture2(const char* data, unsigned size) = 0;

        virtual     const TCHAR* XLAPIENTRY defaultFont(int* fontSize) = 0;
        virtual             void XLAPIENTRY setDefaultFont(const TCHAR* fontName, int fontSize) = 0;

        virtual             void XLAPIENTRY setKey(const wchar_t* name, const wchar_t* key) = 0;

        virtual             bool XLAPIENTRY rgbMode() = 0;
        virtual             void XLAPIENTRY setRgbMode(bool rgbMode = true) = 0;

        virtual             void XLAPIENTRY setLocale(const char* locale) = 0;
        virtual      const char* XLAPIENTRY errorMessage() const = 0;

        virtual             void XLAPIENTRY release() = 0;
    };

}

extern "C" XLAPI libxl::IBookT<char>* XLAPIENTRY xlCreateBookA();
extern "C" XLAPI libxl::IBookT<wchar_t>* XLAPIENTRY xlCreateBookW();

extern "C" XLAPI libxl::IBookT<char>* XLAPIENTRY xlCreateXMLBookA();
extern "C" XLAPI libxl::IBookT<wchar_t>* XLAPIENTRY xlCreateXMLBookW();

#endif
