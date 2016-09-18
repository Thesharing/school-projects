using Microsoft.Office.Interop.Excel;
using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace GSM_DB
{
    /// <summary>
    /// InqueryQuarterTrafficInfoPage.xaml 的交互逻辑
    /// </summary>
    public partial class InqueryQuarterTrafficInfoPage : System.Windows.Window
    {
        public InqueryQuarterTrafficInfoPage() {
            InitializeComponent();
            dataPicker.SelectedDate = new DateTime(2007, 10, 1);
            dataPicker.DisplayDateStart = new DateTime(2007, 9, 1);
            dataPicker.DisplayDateEnd = new DateTime(2007, 11, 1);
            timePickerPrev.AllowTextInput = false;
            timePickerPrev.Value = new DateTime(2007, 11, 1, 0, 0, 0);
            timePickerNext.AllowTextInput = false;
            timePickerNext.Value = new DateTime(2007, 11, 1, 0, 0, 0);
            string connStr = "Data Source=" + DatabaseInfo.dataSource + ";UID=" + DatabaseInfo.uid + ";PWD=" + DatabaseInfo.pwd + ";Initial Catalog=GSM_DB;Integrated Security=True";
            using (SqlConnection connection = new SqlConnection(connStr)) {
                SqlCommand command = new SqlCommand("getCellIDList", connection);
                command.CommandType = System.Data.CommandType.StoredProcedure;
                connection.Open();
                SqlDataReader reader = command.ExecuteReader();
                while (reader.Read()) {
                    comboBoxCellID.Items.Add((int)reader[0]);
                }
                reader.Close();
                comboBoxCellID.SelectedIndex = 0;
            }
        }


        private void buttonOk_Click(object sender, RoutedEventArgs e) {
            DateTime prevTime = (DateTime)timePickerPrev.Value;
            DateTime nextTime = (DateTime)timePickerNext.Value;
            DateTime date = (DateTime)dataPicker.SelectedDate;
            DateTime startTime = new DateTime(date.Year, date.Month, date.Day, prevTime.Hour, prevTime.Minute, prevTime.Second);
            DateTime endTime = new DateTime(date.Year, date.Month, date.Day, nextTime.Hour, nextTime.Minute, nextTime.Second);
            if (startTime.CompareTo(endTime) >= 0) {
                System.Windows.MessageBox.Show("起始日期时间必须早于终止日期时间。", "日期填写错误",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
                return;
            }
            string connStr = "Data Source=" + DatabaseInfo.dataSource + ";UID=" + DatabaseInfo.uid + ";PWD=" + DatabaseInfo.pwd + ";Initial Catalog=GSM_DB;Integrated Security=True";
            using (SqlConnection connection = new SqlConnection(connStr)) {
                SqlCommand command = new SqlCommand("setQuarterPhonecallDetail", connection);
                command.CommandType = System.Data.CommandType.StoredProcedure;
                SqlParameter paramCellID = new SqlParameter("@ACELLID", SqlDbType.Int);
                paramCellID.Value = (int)comboBoxCellID.SelectedItem;
                command.Parameters.Add(paramCellID);
                SqlParameter paramDate = new SqlParameter("@ADATE", SqlDbType.Int);
                paramDate.Value = (date.Year - 2000) * 10000 + date.Month * 100 + date.Day;
                command.Parameters.Add(paramDate);
                SqlParameter paramSTime = new SqlParameter("@ASTIME", SqlDbType.Int);
                paramSTime.Value = startTime.Hour * 10000 + startTime.Minute * 100 + startTime.Second;
                command.Parameters.Add(paramSTime);
                SqlParameter paramETime = new SqlParameter("@AETIME", SqlDbType.Int);
                paramETime.Value = endTime.Hour * 10000 + endTime.Minute * 100 + endTime.Second;
                command.Parameters.Add(paramETime);
                connection.Open();
                command.ExecuteNonQuery();
            }
            using (SqlConnection connection = new SqlConnection(connStr)) {
                SqlCommand command = new SqlCommand("getQuarterPhonecallDetail", connection);
                command.CommandType = System.Data.CommandType.StoredProcedure;
                connection.Open();
                SqlDataAdapter adapter = new SqlDataAdapter();
                adapter.SelectCommand = command;
                System.Data.DataTable table = new System.Data.DataTable("QuarterPhoneCallDetail");
                adapter.Fill(table);
                series.IndependentValuePath = "QTime";
                switch (comboBoxType.SelectedIndex) {
                    case 0:
                        series.DependentValuePath = "QAvgTraff";
                        chart.Title = "15分钟级平均话务量";
                        break;
                    case 1:
                        series.DependentValuePath = "QTraffPerLine";
                        chart.Title = "15分钟级每线话务量";
                        break;
                    case 2:
                        series.DependentValuePath = "QAvgCongsnum";
                        chart.Title = "15分钟级拥塞率";
                        break;
                    case 3:
                        series.DependentValuePath = "QThTraffRate";
                        chart.Title = "15分钟级半速率话务比例";
                        break;
                }
                series.Title = comboBoxCellID.Text;
                series.DataContext = table.DefaultView;
                grid.Visibility = Visibility.Visible;

                string Current = Directory.GetCurrentDirectory();
                //string saveFileName = Current + "\\" + "MINUTE_" + d + "_" + p + "to" + n;
                string saveFileName = Current + "\\" + "QUARTER";
                Microsoft.Office.Interop.Excel.Application excelApp = new Microsoft.Office.Interop.Excel.Application();
                Workbooks mWorkbooks = excelApp.Workbooks;
                Workbook mWorkbook = mWorkbooks.Add(XlWBATemplate.xlWBATWorksheet);
                Worksheet mWorksheet = (Worksheet)mWorkbook.Worksheets[1];
                Range mRange;
                long totalRowCount = table.Rows.Count;
                long totalColCount = table.Columns.Count;
                for (int k = 0; k < totalColCount; k++)
                {
                    mWorksheet.Cells[1, k + 1] = table.Columns[k].ColumnName;
                    mRange = (Range)mWorksheet.Cells[1, k + 1];
                    mRange.Interior.ColorIndex = 15;
                    mRange.Font.Bold = true;
                }
                for (int i = 0; i < totalRowCount; i++)
                {
                    for (int j = 0; j < totalColCount; j++)
                    {
                        mWorksheet.Cells[i + 2, j + 1] = table.Rows[i][j].ToString();
                    }
                }
                mRange = mWorksheet.get_Range((object)mWorksheet.Cells[2, 1], (object)mWorksheet.Cells[table.Rows.Count + 1, table.Columns.Count]);
                if (totalRowCount > 0)
                {
                    mRange.Borders[XlBordersIndex.xlInsideHorizontal].ColorIndex = XlColorIndex.xlColorIndexAutomatic;
                    mRange.Borders[XlBordersIndex.xlInsideHorizontal].LineStyle = XlLineStyle.xlContinuous;
                    mRange.Borders[XlBordersIndex.xlInsideHorizontal].Weight = XlBorderWeight.xlThin;
                }
                if (totalColCount > 1)
                {
                    mRange.Borders[XlBordersIndex.xlInsideVertical].ColorIndex = XlColorIndex.xlColorIndexAutomatic;
                    mRange.Borders[XlBordersIndex.xlInsideVertical].LineStyle = XlLineStyle.xlContinuous;
                    mRange.Borders[XlBordersIndex.xlInsideVertical].Weight = XlBorderWeight.xlThin;
                }
                if (mRange != null)
                {
                    Marshal.ReleaseComObject(mRange);
                    mRange = null;
                }
                if (mWorksheet != null)
                {
                    Marshal.ReleaseComObject(mWorksheet);
                    mWorksheet = null;
                }

                try
                {
                    mWorkbook.Saved = true;
                    mWorkbook.SaveAs(saveFileName, XlFileFormat.xlWorkbookNormal, Missing.Value, Missing.Value, Missing.Value, Missing.Value, XlSaveAsAccessMode.xlNoChange, Missing.Value, Missing.Value, Missing.Value, Missing.Value, Missing.Value);
                    //MessageBox.Show("Export succeeded.", "Export", MessageBoxButtons.OK, MessageBoxIcon.Asterisk);
                }
                catch (Exception exc)
                {
                    //MessageBox.Show("Export failed.", "Export", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    string excStr = exc.StackTrace;
                }

                if (mWorkbook != null)
                {
                    Marshal.ReleaseComObject(mWorkbook);
                    mWorkbook = null;
                }
                if (mWorkbooks != null)
                {
                    Marshal.ReleaseComObject(mWorkbooks);
                    mWorkbooks = null;
                }
                excelApp.Application.Workbooks.Close();
                excelApp.Quit();
                GC.Collect();


            }
        }

        private void hideButton_Click(object sender, RoutedEventArgs e) {
            grid.Visibility = Visibility.Hidden;
        }

        private void buttonClose_Click(object sender, RoutedEventArgs e) {
            this.Close();
        }

        private void buttonMinimize_Click(object sender, RoutedEventArgs e) {
            this.WindowState = WindowState.Minimized;
        }

        private void Image_MouseMove(object sender, MouseEventArgs e) {
            if (e.LeftButton == System.Windows.Input.MouseButtonState.Pressed) {
                this.DragMove();
            }
        }
    }
}
