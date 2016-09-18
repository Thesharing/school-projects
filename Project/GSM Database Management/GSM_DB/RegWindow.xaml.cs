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
using System.Windows.Shapes;
using System.Data.SqlClient;
using System.Security.Cryptography;

namespace GSM_DB
{
    /// <summary>
    /// RegWindow.xaml 的交互逻辑
    /// </summary>
    public partial class RegWindow : Window
    {
        public RegWindow() {
            InitializeComponent();
        }

        private void OkButton_Click(object sender, RoutedEventArgs e) {
            if(AccountTextBox.Text == null || PassWord.Password == null) {
                System.Windows.MessageBox.Show("请填写账号和密码。", "信息未填写",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
                return;
            }
            else if(AccountTextBox.Text.Length > 25) {
                System.Windows.MessageBox.Show("账号名称过长，最大为25位。", "填写错误",
                    MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
                return;
            }
            string connectionStr = "Data Source=" + DatabaseInfo.dataSource + ";UID=" + DatabaseInfo.uid + ";PWD=" + DatabaseInfo.pwd + ";Initial Catalog=AccountDB;Integrated Security=True";
            using (SqlConnection connection = new SqlConnection(connectionStr)) {
                string queryString = "SELECT * FROM dbo.Account WHERE Name = " + AccountTextBox.Text;
                SqlCommand command = new SqlCommand(queryString, connection);
                connection.Open();
                SqlDataReader reader = command.ExecuteReader();
                if (reader.Read()) {
                    System.Windows.MessageBox.Show("账号已注册，您可以直接登录。", "账号已注册", MessageBoxButton.OK, MessageBoxImage.Warning, MessageBoxResult.OK);
                    reader.Close();
                    return;         
                }
                else {
                    reader.Close();
                    string passwordMD5;
                    using (MD5 md5Hash = MD5.Create()) {
                        passwordMD5 = CalcMD5.GetMd5Hash(md5Hash, PassWord.Password);
                    }
                    string queryStringInsert = "insert into dbo.Account (Name, Password) values ('" + AccountTextBox.Text + "', '" + passwordMD5 + "')";
                    SqlCommand commandInsert = new SqlCommand(queryStringInsert, connection);
                    commandInsert.ExecuteNonQuery();
                    MessageBox.Show("注册成功。", "提示",MessageBoxButton.OK, MessageBoxImage.Information, MessageBoxResult.OK);
                    this.Close();
                }
            }
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e) {
            this.Close();
        }

        private void AccountTextBox_TextChanged(object sender, TextChangedEventArgs e) {
            if(AccountTextBox.Text=="" || PassWord.Password == "") {
                OkButton.IsEnabled = false;
            }
            else {
                OkButton.IsEnabled = true;
            }
        }

        private void PassWordBox_PasswordChanged(object sender, RoutedEventArgs e) {
            if (AccountTextBox.Text == "" || PassWord.Password == "") {
                OkButton.IsEnabled = false;
            }
            else {
                OkButton.IsEnabled = true;
            }
        }

        private void buttonClose_Click(object sender, RoutedEventArgs e) {
            this.Close();
        }

        private void buttonMinimize_Click(object sender, RoutedEventArgs e) {
            this.WindowState = WindowState.Minimized;
        }
    }
}
