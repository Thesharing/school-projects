using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Photostore {
    /// <summary>
    /// ResizePanel.xaml 的交互逻辑
    /// </summary>
    public partial class ResizePanel : UserControl {

        public int picWidth;
        public int picHeight;
        public MainWindow mainWindow;
        public PictureProcessor processor;

        private bool isInit = false;

        public ResizePanel() {
            InitializeComponent();
            picWidth = 1;
            picHeight = 1;
        }

        public void SetResizePanel(MainWindow mainWindow, PictureProcessor processor) {
            this.processor = processor;
            isInit = true;
            picHeight = processor.CurrentBitmap.Height;
            this.HeightTextBox.Text = picHeight.ToString();
            picWidth = processor.CurrentBitmap.Width;
            this.WidthTextBox.Text = picWidth.ToString();
            this.mainWindow = mainWindow;
            isInit = false;
        }

        private void OkButton_Click(object sender, RoutedEventArgs e) {
            try {
                int.TryParse(HeightTextBox.Text, out this.picHeight);
                int.TryParse(WidthTextBox.Text, out this.picWidth);
                processor.Resize(picWidth, picHeight);
                mainWindow.PaintPicture();
                mainWindow.SetUndoMenuItem();
            }
            catch (FormatException){
                MessageBox.Show("输入错误。");
            }
            catch (NullReferenceException) {
                MessageBox.Show("未加载图片。");
            }
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e) {
            mainWindow.HideResizePanel();
        }

        private void HeightTextBox_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e) {
            if (LockToggleButton.IsChecked == true && !isInit) {
                try {
                    int height = 0;
                    int.TryParse(HeightTextBox.Text, out height);
                    this.WidthTextBox.Text = ((int)((float)height / (float)picHeight * (float)picWidth)).ToString();
                }
                catch (FormatException) {
                    this.WidthTextBox.Text = picWidth.ToString();
                }
                catch (NullReferenceException) {
                    this.WidthTextBox.Text = picWidth.ToString();
                }
            }
        }

        private void WidthTextBox_LostKeyboardFocus(object sender, KeyboardFocusChangedEventArgs e) {
            if (LockToggleButton.IsChecked == true && !isInit) {
                try {
                    int width = 0;
                    int.TryParse(HeightTextBox.Text, out width);
                    this.HeightTextBox.Text = ((int)((float)width / (float)picWidth * (float)picHeight)).ToString();
                }
                catch (FormatException) {
                    this.HeightTextBox.Text = picHeight.ToString();
                }
                catch (NullReferenceException) {
                    this.HeightTextBox.Text = picHeight.ToString();
                }
            }
        }
    }
}
