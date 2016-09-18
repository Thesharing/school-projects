using System;
using System.Collections.Generic;
using System.Data;
using System.Data.OleDb;
using System.Data.SqlClient;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Microsoft.Office.Interop.Excel;

namespace GSM_DB
{
    class ExcelSupport
    {
        public static void ToDataTable(string filePath, string SheetName, string tableName) {

            string connStr = "";

            connStr = "Provider=Microsoft.Jet.OLEDB.4.0;" + "Data Source=" + filePath + ";" + ";Extended Properties=\"Excel 8.0;HDR=YES;IMEX=1\"";
            string sql_F = "Select * FROM [{0}]";

            OleDbConnection conn = null;
            OleDbDataAdapter da = null;
            DataSet ds = new DataSet();

            try {
                conn = new OleDbConnection(connStr);
                conn.Open();
                da = new OleDbDataAdapter();

                da.SelectCommand = new OleDbCommand(String.Format(sql_F, SheetName + "$"), conn);
                DataSet dsItem = new DataSet();

                da.Fill(dsItem);

                System.Data.DataTable newdt = dsItem.Tables[0].Clone();
                foreach (DataRow sdr in dsItem.Tables[0].Rows) {
                    if (sdr[0].ToString() != "" || sdr[1].ToString() != "") {
                        newdt.Rows.Add(sdr.ItemArray);
                    }
                }
                ds.Tables.Add(newdt);
            }
            catch (Exception ex) {
                Console.WriteLine(ex.Message);
                return;
            }

            connStr = "Data Source="+DatabaseInfo.dataSource+";UID="+DatabaseInfo.uid+";PWD="+DatabaseInfo.pwd+";Initial Catalog=GSM_DB;Integrated Security=True";
            try {
                SqlBulkCopyOptions sqlBcpOpts = SqlBulkCopyOptions.FireTriggers;
                //建立块拷贝
                using (SqlBulkCopy sqlBcp = new SqlBulkCopy(connStr, sqlBcpOpts)) {
                    sqlBcp.BatchSize = 50;//以50记录为单位
                    sqlBcp.NotifyAfter = 100;//设定通知事件前处理的数据行数
                    sqlBcp.DestinationTableName = tableName;//指定目标数据库表名
                    sqlBcp.WriteToServer(ds.Tables[0]);//导入
                }
            }
            catch(SqlException e) {
                System.Windows.MessageBox.Show("访问错误，请检查SQL连接。", "数据库访问失败",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
            }
        }

        public static void ExportToTable(string tablename, string saveFileName) {
            try {
                string connStr = "Data Source=" + DatabaseInfo.dataSource + ";UID=" + DatabaseInfo.uid + ";PWD=" + DatabaseInfo.pwd + ";Initial Catalog=GSM_DB;Integrated Security=True";
                string sqlState = "select * from " + tablename;//要执行的sql语句
                SqlConnection sqlConn = new SqlConnection(connStr);//链接数据库
                sqlConn.Open();//打开链接
                               //dataset与数据之间起桥梁的作用
                SqlDataAdapter sqlDataAdp = new SqlDataAdapter(sqlState, sqlConn);//创建SqlDataAdapter数据适配实例
                System.Data.DataSet dataSet = new DataSet();//创建dataset实例（内存中的数据库）
                sqlDataAdp.Fill(dataSet);
                sqlConn.Close();//关闭数据库

                //新建excel对象
                Microsoft.Office.Interop.Excel.Application excelApp = new Microsoft.Office.Interop.Excel.Application();
                Workbooks mWorkbooks = excelApp.Workbooks;
                Workbook mWorkbook = mWorkbooks.Add(XlWBATemplate.xlWBATWorksheet);
                Worksheet mWorksheet = (Worksheet)mWorkbook.Worksheets[1];//excel文件里面的第一个表
                Range mRange;//一个range对象代表一个单元格，一行，一列等。
                long totalRowCount = dataSet.Tables[0].Rows.Count;//行数
                long totalColCount = dataSet.Tables[0].Columns.Count;//列数

                if (totalRowCount == 0) {
                    Console.WriteLine("没有任何数据可以导出到Excel文件！");
                }

                //填充列名称，设置列的格式
                for (int k = 0; k < totalColCount; k++) {
                    mWorksheet.Cells[1, k + 1] = dataSet.Tables[0].Columns[k].ColumnName;
                    mRange = (Range)mWorksheet.Cells[1, k + 1];
                    mRange.Interior.ColorIndex = 15;//设置字体颜色
                    mRange.Font.Bold = true;//字体加粗
                }

                //填充数据
                for (int i = 0; i < totalRowCount; i++) {
                    for (int j = 0; j < totalColCount; j++) {
                        mWorksheet.Cells[i + 2, j + 1] = dataSet.Tables[0].Rows[i][j].ToString();
                    }
                }
                mRange = mWorksheet.get_Range((object)mWorksheet.Cells[2, 1], (object)mWorksheet.Cells[dataSet.Tables[0].Rows.Count + 1, dataSet.Tables[0].Columns.Count]);
                if (totalRowCount > 0) {
                    mRange.Borders[XlBordersIndex.xlInsideHorizontal].ColorIndex = XlColorIndex.xlColorIndexAutomatic;
                    mRange.Borders[XlBordersIndex.xlInsideHorizontal].LineStyle = XlLineStyle.xlContinuous;
                    mRange.Borders[XlBordersIndex.xlInsideHorizontal].Weight = XlBorderWeight.xlThin;
                }
                if (totalColCount > 1) {
                    mRange.Borders[XlBordersIndex.xlInsideVertical].ColorIndex = XlColorIndex.xlColorIndexAutomatic;
                    mRange.Borders[XlBordersIndex.xlInsideVertical].LineStyle = XlLineStyle.xlContinuous;
                    mRange.Borders[XlBordersIndex.xlInsideVertical].Weight = XlBorderWeight.xlThin;
                }

                //释放资源
                if (mRange != null) {
                    Marshal.ReleaseComObject(mRange);
                    mRange = null;
                }
                if (mWorksheet != null) {
                    Marshal.ReleaseComObject(mWorksheet);
                    mWorksheet = null;
                }
                if (saveFileName != "") {
                    try {
                        mWorkbook.Saved = true;
                        mWorkbook.SaveAs(saveFileName, XlFileFormat.xlWorkbookNormal, Missing.Value, Missing.Value, Missing.Value, Missing.Value, XlSaveAsAccessMode.xlNoChange, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value);
                        //MessageBox.Show("Export succeeded.", "Export", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                    }
                    catch (Exception exc) {
                        //MessageBox.Show("Export failed.", "Export", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                        string excStr = exc.StackTrace;
                    }
                }
                if (mWorkbook != null) {
                    Marshal.ReleaseComObject(mWorkbook);
                    mWorkbook = null;
                }
                if (mWorkbooks != null) {
                    Marshal.ReleaseComObject(mWorkbooks);
                    mWorkbooks = null;
                }
                excelApp.Application.Workbooks.Close();//关闭workbooks
                excelApp.Quit();
                GC.Collect();//强制进行垃圾回收
            }
            catch(SqlException e) {
                System.Windows.MessageBox.Show("访问错误，请检查SQL连接。", "数据库访问失败",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);

            }
        }

        public static void ImportFromExcel(string filePath) {
            ToDataTable(filePath, "MS信息", "MS");
            ToDataTable(filePath, "MSC信息", "MSC");
            ToDataTable(filePath, "BSC信息", "BSC");
            ToDataTable(filePath, "BTS信息", "BTS");
            ToDataTable(filePath, "小区基本信息", "CELL");
            ToDataTable(filePath, "小区频点表", "CELLFREQ");
            ToDataTable(filePath, "天线信息", "ANTENNA");
            ToDataTable(filePath, "邻区信息", "ADJACENCE");
            ToDataTable(filePath, "20个小区一周分钟级话务数据", "PHONECALL");
            ToDataTable(filePath, "路测信息", "RTEST");
        }
    }
}
