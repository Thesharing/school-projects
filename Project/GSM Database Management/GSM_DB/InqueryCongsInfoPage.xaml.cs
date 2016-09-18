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
using System.Timers;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;

namespace GSM_DB
{
    /// <summary>
    /// InqueryCongsInfoPage.xaml 的交互逻辑
    /// </summary>
    public partial class InqueryCongsInfoPage : System.Windows.Window
    {
        System.Timers.Timer renameTimer = new System.Timers.Timer();
        private delegate void DispatcherDelegateTimer();
        public InqueryCongsInfoPage() {
            InitializeComponent();
            dataPicker.SelectedDate = new DateTime(2007, 10, 1);
            dataPicker.DisplayDateStart = new DateTime(2007, 9, 1);
            dataPicker.DisplayDateEnd = new DateTime(2007, 11, 1);
            timePickerPrev.AllowTextInput = false;
            timePickerPrev.Value = new DateTime(2007, 11, 1, 0, 0, 0);
            timePickerNext.AllowTextInput = false;
            timePickerNext.Value = new DateTime(2007, 11, 1, 0, 0, 0);
            renameTimer.Interval = 2000;
            renameTimer.AutoReset = false;
            renameTimer.Elapsed += renameTimer_Elapsed;
        }

        private void renameTimer_Elapsed(object sender, ElapsedEventArgs e) {
            this.Dispatcher.Invoke(DispatcherPriority.Normal, new DispatcherDelegateTimer(changeHeader));
        }

        private void buttonOk_Click(object sender, RoutedEventArgs e) {
            if(textBoxCongsRate.Text == "") {
                System.Windows.MessageBox.Show("请填写拥塞率。", "拥塞率填写错误",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
                return;
            }
            double congsRate;
            try {
                if(!double.TryParse(textBoxCongsRate.Text, out congsRate)) {
                    System.Windows.MessageBox.Show("拥塞率填写格式错误，请检查", "拥塞率填写错误",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
                    return;
                }
            }
            catch {
                System.Windows.MessageBox.Show("拥塞率填写格式错误，请检查", "拥塞率填写错误",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
                return;
            }
            if(congsRate <= 0 || congsRate >= 1) {
                System.Windows.MessageBox.Show("拥塞率必须大于0且小于1", "拥塞率填写错误",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
                return;
            }
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
                SqlCommand command = new SqlCommand("setCongsDetail", connection);
                command.CommandType = System.Data.CommandType.StoredProcedure;
                SqlParameter paramCellID = new SqlParameter("@ACONGSRATE", SqlDbType.Real);
                paramCellID.Value = congsRate;
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
                SqlCommand command = new SqlCommand("getCongsDetail", connection);
                command.CommandType = System.Data.CommandType.StoredProcedure;
                connection.Open();
                SqlDataAdapter adapter = new SqlDataAdapter();
                adapter.SelectCommand = command;
                System.Data.DataTable table = new System.Data.DataTable("QuarterPhoneCallDetail");
                adapter.Fill(table);
                dataGrid.DataContext = table;
                grid.Visibility = Visibility.Visible;

                string Current = Directory.GetCurrentDirectory();
                string saveFileName = Current + "\\" + "CONGS";
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




                renameTimer.Start();
            }
        }

        private void hideButton_Click(object sender, RoutedEventArgs e) {
            grid.Visibility = Visibility.Hidden;
        }

        private void changeHeader() {
            dataGrid.Columns[0].Header = "时间段";
            dataGrid.Columns[1].Header = "小区ID";
            dataGrid.Columns[2].Header = "平均话务量";
            dataGrid.Columns[3].Header = "每线话务量";
            dataGrid.Columns[4].Header = "拥塞率";
            dataGrid.Columns[5].Header = "半速率话务比例";
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
