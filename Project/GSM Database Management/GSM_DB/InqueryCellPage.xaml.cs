using System;
using System.Collections.Generic;
using System.Data;
using System.Data.SqlClient;
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

namespace GSM_DB
{
    /// <summary>
    /// InqueryCellPage.xaml 的交互逻辑
    /// </summary>
    public partial class InqueryCellPage : Window
    {
        public InqueryCellPage() {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e) {
            string connStr = "Data Source=" + DatabaseInfo.dataSource + ";UID=" + DatabaseInfo.uid + ";PWD=" + DatabaseInfo.pwd + ";Initial Catalog=GSM_DB;Integrated Security=True";
            using (SqlConnection connection = new SqlConnection(connStr)) {
                SqlCommand command = new SqlCommand("getCellIDList", connection);
                command.CommandType = System.Data.CommandType.StoredProcedure;
                connection.Open();
                SqlDataReader reader = command.ExecuteReader();
                while (reader.Read()) {
                    comboxBoxCellID.Items.Add((int)reader[0]);
                }
                reader.Close();
                comboxBoxCellID.SelectedIndex = 0;
            }
        }

        private void buttonOk_Click(object sender, RoutedEventArgs e) {
            string connStr = "Data Source=" + DatabaseInfo.dataSource + ";UID=" + DatabaseInfo.uid + ";PWD=" + DatabaseInfo.pwd + ";Initial Catalog=GSM_DB;Integrated Security=True";
            using (SqlConnection connection = new SqlConnection(connStr)) {
                SqlCommand command = new SqlCommand("getCellInfo", connection);
                command.CommandType = System.Data.CommandType.StoredProcedure;
                SqlParameter paramCellID = new SqlParameter("@CELLID", SqlDbType.Int);
                paramCellID.Value = (int)comboxBoxCellID.SelectedItem;
                command.Parameters.Add(paramCellID);
                connection.Open();
                SqlDataReader reader = command.ExecuteReader();
                while (reader.Read()) {
                    btsNameTextBlock.Text = reader[0].ToString();
                    areaNameTextBlock.Text = reader[1].ToString();
                    lacTextBlock.Text = reader[2].ToString();
                    lontitudeTextBlock.Text = reader[3].ToString();
                    latitudeTextBlock.Text = reader[4].ToString();
                    directionTextBlock.Text = reader[5].ToString();
                    radiousTextBlock.Text = reader[6].ToString();
                    bcchTextBlock.Text = reader[7].ToString();
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
