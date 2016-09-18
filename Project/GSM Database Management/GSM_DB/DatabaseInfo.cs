using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace GSM_DB
{
    class DatabaseInfo
    {
        public static string dataSource = "CYPRESTAR\\SQLEXPRESS";
        public static string uid = "cyprestar";
        public static string pwd = "HZL04291316wr";

        public static void getDatabaseInfo() {
            if (File.Exists(System.Windows.Forms.Application.StartupPath + "\\setting.ini")) {
                string settingFilePath = System.Windows.Forms.Application.StartupPath + "\\setting.ini";
                StreamReader settingFileSR = new StreamReader(settingFilePath, Encoding.UTF8);
                dataSource = settingFileSR.ReadLine();
                uid = settingFileSR.ReadLine();
                pwd = settingFileSR.ReadLine();
                settingFileSR.Close();
            }
        }
    }
}
