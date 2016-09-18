using Microsoft.Office.Interop.Excel;
using System;
using System.Data;
using System.Data.SqlClient;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Timers;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;

namespace GSM_DB
{
    /// <summary>
    /// Distance.xaml 的交互逻辑
    /// </summary>
    public partial class Distance : System.Windows.Window
    {
        System.Timers.Timer renameTimer = new System.Timers.Timer();
        private delegate void DispatcherDelegateTimer();
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            string connStr = "Data Source=" + DatabaseInfo.dataSource + ";UID=" + DatabaseInfo.uid + ";PWD=" + DatabaseInfo.pwd + ";Initial Catalog=GSM_DB;Integrated Security=True";
            using (SqlConnection connection = new SqlConnection(connStr)) {
                SqlCommand command = new SqlCommand("getCellIDList", connection);
                command.CommandType = System.Data.CommandType.StoredProcedure;
                connection.Open();
                SqlDataReader reader = command.ExecuteReader();
                while (reader.Read()) {
                    this.comboBox.Items.Add((int)reader[0]);
                }
                reader.Close();
                this.comboBox.SelectedIndex = 0;
            }
        }

        private void textBox1_TextChanged(object sender, TextChangedEventArgs e)
        {
            foreach (char ch in this.textBox1.Text)
            {
                if (!char.IsDigit(ch) && ch != '.')//是否为数字
                {
                    System.Windows.MessageBox.Show("请输入数字", "错误", MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
                    this.textBox1.Text = "";
                    break;
                }
            }


        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            string CID = comboBox.Text;
            string dis = textBox1.Text;
            if (CID == "" || dis == "")
            {
                System.Windows.MessageBox.Show("信息输入不完整，请检查。", "错误", MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
            }
            else 
            {
                double distance = 0;
                if(!double.TryParse(textBox1.Text, out distance)) {
                    System.Windows.MessageBox.Show("输入数字格式错误，请检查。", "错误", MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
                    return;
                }
                int Cid = Convert.ToInt16(CID);
                double Dis = Convert.ToDouble(dis);
                string connStr = "Data Source=" + DatabaseInfo.dataSource + ";UID=" + DatabaseInfo.uid + ";PWD=" + DatabaseInfo.pwd + ";Initial Catalog=GSM_DB;Integrated Security=True";
                using (SqlConnection connection = new SqlConnection(connStr)) {
                    SqlCommand command = new SqlCommand("setAdjDist", connection);
                    command.CommandType = System.Data.CommandType.StoredProcedure;
                    SqlParameter sqlParme;
                    sqlParme = command.Parameters.Add("@ACELLID", SqlDbType.Int);
                    sqlParme.Direction = ParameterDirection.Input;
                    sqlParme.Value = Cid;
                    sqlParme = command.Parameters.Add("@Threshold", SqlDbType.Real);
                    sqlParme.Direction = ParameterDirection.Input;
                    sqlParme.Value = Dis;
                    connection.Open();
                    command.ExecuteNonQuery();

                    SqlDataAdapter sda = new SqlDataAdapter();
                    SqlCommand command1 = new SqlCommand("getAdjDist", connection);
                    command1.CommandType = System.Data.CommandType.StoredProcedure;
                    sda.SelectCommand = command1;
                    DataSet ds = new DataSet();
                    sda.Fill(ds);
                    System.Data.DataTable dt = ds.Tables[0];
                    dataGrid1.ColumnWidth = 225;
                    dataGrid1.ItemsSource = dt.DefaultView;

                    string Current = Directory.GetCurrentDirectory();
                    string saveFileName = Current + "\\" + "ADJ_" + CID + "_" + dis;
                    Microsoft.Office.Interop.Excel.Application excelApp = new Microsoft.Office.Interop.Excel.Application();
                    Workbooks mWorkbooks = excelApp.Workbooks;
                    Workbook mWorkbook = mWorkbooks.Add(XlWBATemplate.xlWBATWorksheet);
                    Worksheet mWorksheet = (Worksheet)mWorkbook.Worksheets[1];
                    Range mRange;
                    long totalRowCount = dt.Rows.Count;
                    long totalColCount = dt.Columns.Count;
                    for (int k = 0; k < totalColCount; k++)
                    {
                        mWorksheet.Cells[1, k + 1] = dt.Columns[k].ColumnName;
                        mRange = (Range)mWorksheet.Cells[1, k + 1];
                        mRange.Interior.ColorIndex = 15;
                        mRange.Font.Bold = true;
                    }
                    for (int i = 0; i < totalRowCount; i++)
                    {
                        for (int j = 0; j < totalColCount; j++)
                        {
                            mWorksheet.Cells[i + 2, j + 1] = dt.Rows[i][j].ToString();
                        }
                    }
                    mRange = mWorksheet.get_Range((object)mWorksheet.Cells[2, 1], (object)mWorksheet.Cells[dt.Rows.Count + 1, dt.Columns.Count]);
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
                renameTimer.Start();
            }

        }

        public Distance()
        {
            InitializeComponent();
            renameTimer.Interval = 2000;
            renameTimer.AutoReset = false;
            renameTimer.Elapsed += renameTimer_Elapsed;
        }
        private void renameTimer_Elapsed(object sender, ElapsedEventArgs e) {
            this.Dispatcher.Invoke(DispatcherPriority.Normal, new DispatcherDelegateTimer(changeHeader));
        }
        private void changeHeader() {
            dataGrid1.Columns[0].Header = "邻区ID";
            dataGrid1.Columns[1].Header = "与ID为"+comboBox.Text+"的小区距离";
        }

        private void buttonClose_Click(object sender, RoutedEventArgs e) {
            this.Close();
        }

        private void buttonMinimize_Click(object sender, RoutedEventArgs e) {
            this.WindowState = WindowState.Minimized;
        }
    }
}
