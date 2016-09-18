using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Win32;
using System.IO;
using System.ComponentModel;

namespace GSM_DB
{
    /// <summary>
    /// ImportPage.xaml 的交互逻辑
    /// </summary>
    public partial class ImportPage : Window
    {
        private BackgroundWorker bw = new BackgroundWorker();
        private string filePathText;
        public ImportPage() {
            InitializeComponent();
            bw.WorkerReportsProgress = true;
            bw.WorkerSupportsCancellation = true;
            bw.DoWork += new DoWorkEventHandler(LoginBW_DoWork);
            bw.ProgressChanged += new ProgressChangedEventHandler(LoginBW_ProgressChanged);
            bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(LoginBW_RunWorkerCompleted);
        }

        private void explorerButton_Click(object sender, RoutedEventArgs e) {
            OpenFileDialog openDileDialog = new OpenFileDialog();
            openDileDialog.FileName = "v1";
            openDileDialog.DefaultExt = ".xls";
            openDileDialog.Filter = "Excel File|*.xls";
            openDileDialog.Multiselect = false;
            Nullable<bool> result = openDileDialog.ShowDialog();
            if (result == true) {
                filePathTextBox.Text = openDileDialog.FileName;
            }
        }

        private void okButton_Click(object sender, RoutedEventArgs e) {
            if(filePathTextBox.Text == "") {
                System.Windows.MessageBox.Show("请选定文件。", "文件未选定",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
                return;
            }
            if (File.Exists(filePathTextBox.Text)) {
                textBlockProgress.Text = "导入中...";
                grid.Visibility = Visibility.Visible;
                filePathText = filePathTextBox.Text;
                bw.RunWorkerAsync();
            }
            else {
                System.Windows.MessageBox.Show("文件不存在。", "请检查",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
            }
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

        private void LoginBW_DoWork(Object sender, DoWorkEventArgs e) {
            BackgroundWorker backgroundWorker = sender as BackgroundWorker;
            ExcelSupport.ImportFromExcel(filePathText);
            backgroundWorker.ReportProgress(100);
        }

        private void LoginBW_ProgressChanged(object sender, ProgressChangedEventArgs e) {
            return;
        }

        private void LoginBW_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) {
            grid.Visibility = Visibility.Collapsed;
            grid.IsEnabled = false;
            if (e.Cancelled == false && e.Error == null) {
                textBlockProgress.Text = "导入完成";
            }
        }
    }
}
