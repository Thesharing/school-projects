using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Data.SqlClient;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Data;

namespace GSM_DB
{
    /// <summary>
    /// InqueryBTSPage.xaml 的交互逻辑
    /// </summary>
    public partial class InqueryBTSPage : Window
    {
        public InqueryBTSPage() {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e) {
            string connStr = "Data Source=" + DatabaseInfo.dataSource + ";UID=" + DatabaseInfo.uid + ";PWD=" + DatabaseInfo.pwd + ";Initial Catalog=GSM_DB;Integrated Security=True";
            using (SqlConnection connection = new SqlConnection(connStr)) {
                SqlCommand command = new SqlCommand("getBTSList", connection);
                command.CommandType = System.Data.CommandType.StoredProcedure;
                connection.Open();
                SqlDataReader reader = command.ExecuteReader();
                while (reader.Read()) {
                    comboxBoxBTSName.Items.Add((string)reader[0]);
                }
                reader.Close();
                comboxBoxBTSName.SelectedIndex = 0;
            }
        }

        private void buttonOk_Click(object sender, RoutedEventArgs e) {
            string connStr = "Data Source=" + DatabaseInfo.dataSource + ";UID=" + DatabaseInfo.uid + ";PWD=" + DatabaseInfo.pwd + ";Initial Catalog=GSM_DB;Integrated Security=True";
            using (SqlConnection connection = new SqlConnection(connStr)) {
                SqlCommand command = new SqlCommand("getBTSInfo", connection);
                command.CommandType = System.Data.CommandType.StoredProcedure;
                SqlParameter paramBTSName = new SqlParameter("@BTSName", SqlDbType.VarChar);
                paramBTSName.Value = (string)comboxBoxBTSName.SelectedItem;
                command.Parameters.Add(paramBTSName);
                connection.Open();
                SqlDataReader reader = command.ExecuteReader();
                while (reader.Read()) {
                    bscIDTextBlock.Text = reader[0].ToString();
                    lontitudeTextBlock.Text = reader[1].ToString();
                    latitudeTextBlock.Text = reader[2].ToString();
                    altitudeTextBlock.Text = reader[3].ToString();
                    manufTextBlock.Text = reader[4].ToString();
                    powerTextBlock.Text = reader[5].ToString();
                }
                reader.Close();
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
    }
}
