#ifndef ISHEETT_H
#define ISHEETT_H

#include "setup.h"
#include "enum.h"

namespace libxl
{

    template<class TCHAR> struct IFormatT;

    template<class TCHAR>
    struct ISheetT
    {
        virtual         CellType XLAPIENTRY cellType(int row, int col) const = 0;
        virtual             bool XLAPIENTRY isFormula(int row, int col) const = 0;

        virtual IFormatT<TCHAR>* XLAPIENTRY cellFormat(int row, int col) const = 0;
        virtual             void XLAPIENTRY setCellFormat(int row, int col, IFormatT<TCHAR>* format) = 0;

        virtual     const TCHAR* XLAPIENTRY readStr(int row, int col, IFormatT<TCHAR>** format = 0) = 0;
        virtual             bool XLAPIENTRY writeStr(int row, int col, const TCHAR* value, IFormatT<TCHAR>* format = 0) = 0;

        virtual           double XLAPIENTRY readNum(int row, int col, IFormatT<TCHAR>** format = 0) const = 0;
        virtual             bool XLAPIENTRY writeNum(int row, int col, double value, IFormatT<TCHAR>* format = 0) = 0;

        virtual             bool XLAPIENTRY readBool(int row, int col, IFormatT<TCHAR>** format = 0) const = 0;
        virtual             bool XLAPIENTRY writeBool(int row, int col, bool value, IFormatT<TCHAR>* format = 0) = 0;

        virtual             bool XLAPIENTRY readBlank(int row, int col, IFormatT<TCHAR>** format) const = 0;
        virtual             bool XLAPIENTRY writeBlank(int row, int col, IFormatT<TCHAR>* format) = 0;

        virtual     const TCHAR* XLAPIENTRY readFormula(int row, int col, IFormatT<TCHAR>** format = 0) = 0;
        virtual             bool XLAPIENTRY writeFormula(int row, int col, const TCHAR* value, IFormatT<TCHAR>* format = 0) = 0;

        virtual     const TCHAR* XLAPIENTRY readComment(int row, int col) const = 0;
        virtual             void XLAPIENTRY writeComment(int row, int col, const TCHAR* value, const TCHAR* author = 0, int width = 129, int height = 75) = 0;

        virtual             bool XLAPIENTRY isDate(int row, int col) const = 0;
        virtual        ErrorType XLAPIENTRY readError(int row, int col) const = 0;

        virtual           double XLAPIENTRY colWidth(int col) const = 0;
        virtual           double XLAPIENTRY rowHeight(int row) const = 0;

        virtual             bool XLAPIENTRY setCol(int colFirst, int colLast, double width, IFormatT<TCHAR>* format = 0, bool hidden = false) = 0;
        virtual             bool XLAPIENTRY setRow(int row, double height, IFormatT<TCHAR>* format = 0, bool hidden = false) = 0;

        virtual             bool XLAPIENTRY rowHidden(int row) const = 0;
        virtual             bool XLAPIENTRY setRowHidden(int row, bool hidden) = 0;

        virtual             bool XLAPIENTRY colHidden(int col) const = 0;
        virtual             bool XLAPIENTRY setColHidden(int col, bool hidden) = 0;

        virtual             bool XLAPIENTRY getMerge(int row, int col, int* rowFirst, int* rowLast, int* colFirst, int* colLast) = 0;
        virtual             bool XLAPIENTRY setMerge(int rowFirst, int rowLast, int colFirst, int colLast) = 0;
        virtual             bool XLAPIENTRY delMerge(int row, int col) = 0;

        virtual             void XLAPIENTRY setPicture(int row, int col, int pictureId, double scale = 1.0, int offset_x = 0, int offset_y = 0) = 0;
        virtual             void XLAPIENTRY setPicture2(int row, int col, int pictureId, int width, int height, int offset_x = 0, int offset_y = 0) = 0;

        virtual             bool XLAPIENTRY setHorPageBreak(int row, bool pageBreak = true) = 0;
        virtual             bool XLAPIENTRY setVerPageBreak(int col, bool pageBreak = true) = 0;

        virtual             void XLAPIENTRY split(int row, int col) = 0;

        virtual             bool XLAPIENTRY groupRows(int rowFirst, int rowLast, bool collapsed = true) = 0;
        virtual             bool XLAPIENTRY groupCols(int colFirst, int colLast, bool collapsed = true) = 0;

        virtual             bool XLAPIENTRY groupSummaryBelow() const = 0;
        virtual             void XLAPIENTRY setGroupSummaryBelow(bool below) = 0;

        virtual             bool XLAPIENTRY groupSummaryRight() const = 0;
        virtual             void XLAPIENTRY setGroupSummaryRight(bool right) = 0;

        virtual             bool XLAPIENTRY clear(int rowFirst = 0, int rowLast = 65535, int colFirst = 0, int colLast = 255) = 0;

        virtual             bool XLAPIENTRY insertCol(int colFirst, int colLast) = 0;
        virtual             bool XLAPIENTRY insertRow(int rowFirst, int rowLast) = 0;
        virtual             bool XLAPIENTRY removeCol(int colFirst, int colLast) = 0;
        virtual             bool XLAPIENTRY removeRow(int rowFirst, int rowLast) = 0;

        virtual             bool XLAPIENTRY copyCell(int rowSrc, int colSrc, int rowDst, int colDst) = 0;

        virtual              int XLAPIENTRY firstRow() const = 0;
        virtual              int XLAPIENTRY lastRow() const = 0;
        virtual              int XLAPIENTRY firstCol() const = 0;
        virtual              int XLAPIENTRY lastCol() const = 0;

        virtual             bool XLAPIENTRY displayGridlines() const = 0;
        virtual             void XLAPIENTRY setDisplayGridlines(bool show = true) = 0;

        virtual             bool XLAPIENTRY printGridlines() const = 0;
        virtual             void XLAPIENTRY setPrintGridLines(bool print = true) = 0;

        virtual              int XLAPIENTRY zoom() const = 0;
        virtual             void XLAPIENTRY setZoom(int zoom) = 0;

        virtual              int XLAPIENTRY printZoom() const = 0;
        virtual             void XLAPIENTRY setPrintZoom(int zoom) = 0;

        virtual             bool XLAPIENTRY landscape() const = 0;
        virtual             void XLAPIENTRY setLandscape(bool landscape = true) = 0;

        virtual            Paper XLAPIENTRY paper() const = 0;
        virtual             void XLAPIENTRY setPaper(Paper paper = PAPER_DEFAULT) = 0;

        virtual     const TCHAR* XLAPIENTRY header() const = 0;
        virtual             bool XLAPIENTRY setHeader(const TCHAR* header, double margin = 0.5) = 0;
        virtual           double XLAPIENTRY headerMargin() const = 0;

        virtual     const TCHAR* XLAPIENTRY footer() const = 0;
        virtual             bool XLAPIENTRY setFooter(const TCHAR* footer, double margin = 0.5) = 0;
        virtual           double XLAPIENTRY footerMargin() const = 0;

        virtual             bool XLAPIENTRY hCenter() const = 0;
        virtual             void XLAPIENTRY setHCenter(bool hCenter = true) = 0;

        virtual             bool XLAPIENTRY vCenter() const = 0;
        virtual             void XLAPIENTRY setVCenter(bool vCenter = true) = 0;

        virtual           double XLAPIENTRY marginLeft() const = 0;
        virtual             void XLAPIENTRY setMarginLeft(double margin) = 0;

        virtual           double XLAPIENTRY marginRight() const = 0;
        virtual             void XLAPIENTRY setMarginRight(double margin) = 0;

        virtual           double XLAPIENTRY marginTop() const = 0;
        virtual             void XLAPIENTRY setMarginTop(double margin) = 0;

        virtual           double XLAPIENTRY marginBottom() const = 0;
        virtual             void XLAPIENTRY setMarginBottom(double margin) = 0;

        virtual             bool XLAPIENTRY printRowCol() const = 0;
        virtual             void XLAPIENTRY setPrintRowCol(bool print = true) = 0;

        virtual             void XLAPIENTRY setPrintRepeatRows(int rowFirst, int rowLast) = 0;
        virtual             void XLAPIENTRY setPrintRepeatCols(int colFirst, int colLast) = 0;
        virtual             void XLAPIENTRY setPrintArea(int rowFirst, int rowLast, int colFirst, int colLast) = 0;

        virtual             void XLAPIENTRY clearPrintRepeats() = 0;
        virtual             void XLAPIENTRY clearPrintArea() = 0;

        virtual             bool XLAPIENTRY setNamedRange(const TCHAR* name, int rowFirst, int rowLast, int colFirst, int colLast) = 0;
        virtual             bool XLAPIENTRY delNamedRange(const TCHAR* name) = 0;

        virtual     const TCHAR* XLAPIENTRY name() const = 0;
        virtual             void XLAPIENTRY setName(const TCHAR* name) = 0;

        virtual             bool XLAPIENTRY protect() const = 0;
     virtual             void XLAPIENTRY setProtect(bool protect = true) = 0;
    };

}

#endif
