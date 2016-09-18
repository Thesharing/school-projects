using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
using System.Windows.Forms;
using System.IO;
using System.ComponentModel;

namespace GSM_DB
{
    /// <summary>
    /// ExportPage.xaml 的交互逻辑
    /// </summary>
    public partial class ExportPage : Window
    {
        private BackgroundWorker bw = new BackgroundWorker();
        private string filePathText;
        private int selectedIndex = 0;
        string[] infoType = { "MS", "MSC", "BSC", "BTS", "CELL", "CELLFREQ", "ANTENNA", "PHONECALL", "RTEST", "IDLEINFO" };
        public ExportPage() {
            InitializeComponent();
            for(int i = 0; i < infoType.Length; i++) {
                comboBox.Items.Add(infoType[i]);
            }
            bw.WorkerReportsProgress = true;
            bw.WorkerSupportsCancellation = true;
            bw.DoWork += new DoWorkEventHandler(LoginBW_DoWork);
            bw.ProgressChanged += new ProgressChangedEventHandler(LoginBW_ProgressChanged);
            bw.RunWorkerCompleted += new RunWorkerCompletedEventHandler(LoginBW_RunWorkerCompleted);
        }

        private void explorerButton_Click(object sender, RoutedEventArgs e) {
            if (comboBox.SelectedIndex == 0) {
                FolderBrowserDialog folderBrowseDialog = new FolderBrowserDialog();
                DialogResult result = folderBrowseDialog.ShowDialog();
                if(result == System.Windows.Forms.DialogResult.OK) {
                    filePathTextBox.Text = folderBrowseDialog.SelectedPath;
                }
            }
            else {
                Microsoft.Win32.SaveFileDialog saveDileDialog = new Microsoft.Win32.SaveFileDialog();
                saveDileDialog.FileName = infoType[comboBox.SelectedIndex - 1];
                saveDileDialog.DefaultExt = ".xls";
                saveDileDialog.Filter = "Excel File|*.xls";
                Nullable<bool> result = saveDileDialog.ShowDialog();
                if (result == true) {
                    filePathTextBox.Text = saveDileDialog.FileName;
                }
            }
            
        }

        private void okButton_Click(object sender, RoutedEventArgs e) {
            if(filePathTextBox.Text == "") {
                System.Windows.MessageBox.Show("请选定文件。", "文件未选定",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
                return;
            }
            selectedIndex = comboBox.SelectedIndex;
            filePathText = filePathTextBox.Text;
            if (selectedIndex != 0 && Directory.Exists(filePathText)) {
                System.Windows.MessageBox.Show("请选定文件而不是文件夹。", "文件未选定",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
            }
            grid.Visibility = Visibility.Visible;
            bw.RunWorkerAsync();
        }
        private void buttonClose_Click(object sender, RoutedEventArgs e) {
            this.Close();
        }

        private void buttonMinimize_Click(object sender, RoutedEventArgs e) {
            this.WindowState = WindowState.Minimized;
        }

        private void Image_MouseMove(object sender, System.Windows.Input.MouseEventArgs e) {
            if (e.LeftButton == System.Windows.Input.MouseButtonState.Pressed) {
                this.DragMove();
            }
        }

        private void LoginBW_DoWork(Object sender, DoWorkEventArgs e) {
            BackgroundWorker backgroundWorker = sender as BackgroundWorker;
            if (selectedIndex == 0) {
                for (int i = 0; i < infoType.Length; i++) {
                    ExcelSupport.ExportToTable(infoType[i], filePathText + "\\" + infoType[i]);
                }
            }
            else {
                ExcelSupport.ExportToTable(infoType[selectedIndex - 1], filePathText);
            }
            backgroundWorker.ReportProgress(100);
        }

        private void LoginBW_ProgressChanged(object sender, ProgressChangedEventArgs e) {
            return;
        }

        private void LoginBW_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e) {
            grid.Visibility = Visibility.Collapsed;
            grid.IsEnabled = false;
        }
    }
}
