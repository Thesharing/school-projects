using System;
using System.Windows;
using System.Windows.Media.Imaging;
using Microsoft.Win32;
using System.Drawing;
using Photostore.Common;

namespace Photostore {
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window {
        public BitmapImage sourceImage;
        public PictureProcessor processor;
        public Boolean edited;
        public string fileName;

        public MainWindow() {
            InitializeComponent();
            processor = new PictureProcessor();
            edited = false;
        }

        public void PaintPicture() {
            System.IO.MemoryStream stream = new System.IO.MemoryStream();
            processor.CurrentBitmap.Save(stream, System.Drawing.Imaging.ImageFormat.Bmp);
            stream.Position = 0;
            byte[] data = new byte[stream.Length];
            stream.Read(data, 0, Convert.ToInt32(stream.Length));
            BitmapImage bmapImage = new BitmapImage();
            bmapImage.BeginInit();
            bmapImage.StreamSource = stream;
            bmapImage.EndInit();
            MainImage.Source = bmapImage;
        }

        public void OpenFile() {
            if (edited) {
                MessageBoxResult result = MessageBox.Show("您的修改还未保存，是否保存？", "PhotoStore", MessageBoxButton.YesNoCancel, MessageBoxImage.Question, MessageBoxResult.Cancel);
                if (result == MessageBoxResult.Yes) {
                    if (SaveFile() == true)
                        this.Close();
                    else {
                        return;
                    }
                }
                else if (result == MessageBoxResult.Cancel) {
                    return;
                }
            }
            OpenFileDialog dialog = new OpenFileDialog();
            dialog.Title = "打开图片";
            dialog.Filter = "BMP 文件(*.bmp)|*.bmp|JPEG 文件(*.jpg)|*.jpg|PNG 文件(*.png)|*.png";
            try {
                if (dialog.ShowDialog() == true) {
                    fileName = dialog.FileName;
                    processor.FileHandler.Load(dialog.FileName);
                    PaintPicture();
                    EnableButtons();
                    OpenFileButton.Visibility = Visibility.Collapsed;
                }
            }
            catch (Exception ex) {
                MessageBox.Show("错误：" + ex.Message, "PhotoStore", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            edited = false;
        }

        private void EnableButtons() {
            SaveMenuItem.IsEnabled = true;
            CopyMenuItem.IsEnabled = true;
            BrightnessAddButton.IsEnabled = true;
            BrightnessMinusButton.IsEnabled = true;
            ContrastAddButton.IsEnabled = true;
            ContrastMinusButton.IsEnabled = true;
            GrayScaleButton.IsEnabled = true;
            XFlipButton.IsEnabled = true;
            YFlipButton.IsEnabled = true;
            Rotate90Button.IsEnabled = true;
            Rotate270Button.IsEnabled = true;
            ResizeButton.IsEnabled = true;
        }

        public void SetUndoMenuItem() {
            UndoMenuItem.Header = "撤销(_U)";
            UndoMenuItem.IsEnabled = true;
            UndoAllMenuItem.IsEnabled = true;
        }

        private bool SaveFile() {
            SaveFileDialog dialog = new SaveFileDialog();
            dialog.Title = "保存图片";
            dialog.Filter = "BMP 文件(*.bmp)|*.bmp|JPEG 文件(*.jpg)|*.jpg|PNG 文件(*.png)|*.png";
            try {
                if (dialog.ShowDialog() == true) {
                    processor.FileHandler.Save(dialog.FileName);
                    return true;
                }
            }
            catch (Exception ex) {
                MessageBox.Show("错误：" + ex.Message, "PhotoStore", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            return false;
        }

        public void HideResizePanel() {
            this.resizePanel.Visibility = Visibility.Collapsed;
        }

        private void OnClose() {
            if (edited) {
                MessageBoxResult result = MessageBox.Show("您的修改还未保存，是否保存？", "PhotoStore", MessageBoxButton.YesNoCancel, MessageBoxImage.Question, MessageBoxResult.Cancel);
                if (result == MessageBoxResult.Yes) {
                    if (SaveFile() == true)
                        this.Close();
                }
                else if (result == MessageBoxResult.No) {
                    this.Close();
                }
            }
            else {
                this.Close();
            }
        }

        #region Menu
        private void OpenMenuItem_Click(object sender, RoutedEventArgs e) {
            OpenFile();
        }

        private void SaveMenuItem_Click(object sender, RoutedEventArgs e) {
            if (SaveFile() == true) {
                edited = false;
                UndoMenuItem.IsEnabled = false;
                UndoAllMenuItem.IsEnabled = false;
                UndoMenuItem.Header = "撤销(_U)";
            }
        }

        private void ExitMenuItem_Click(object sender, RoutedEventArgs e) {
            OnClose();
        }

        private void UndoMenuItem_Click(object sender, RoutedEventArgs e) {
            processor.ResetBitmap();
            PaintPicture();
            if((string)UndoMenuItem.Header == "撤销(_U)") {
                UndoMenuItem.Header = "重做(_R)";
            }
            else {
                UndoMenuItem.Header = "撤销(_U)";
            }
        }

        private void UndoAllMenuItem_Click(object sender, RoutedEventArgs e) {
            if(fileName != null){
                processor.FileHandler.Load(fileName);
                PaintPicture();
            }
            UndoMenuItem.IsEnabled = false;
            UndoAllMenuItem.IsEnabled = false;
            UndoMenuItem.Header = "撤销(_U)";
        }

        private void CopyMenuItem_Click(object sender, RoutedEventArgs e) {
            Clipboard.SetImage(Converter.ConvertBitmapToBitmapSource(processor.CurrentBitmap));
        }

        private void HelpMenuItem_Click(object sender, RoutedEventArgs e) {
            MessageBox.Show("感谢您的使用。");
        }

        private void AboutMenuItem_Click(object sender, RoutedEventArgs e) {
            AboutWindow window = new AboutWindow();
            window.ShowDialog();
        }
        #endregion

        #region Button

        private void BrightnessAddButton_Click(object sender, RoutedEventArgs e) {
            edited = true;
            SetUndoMenuItem();
            try {
                processor.BrightnessHandler.SetBrightness(10);
                PaintPicture();
            }
            catch (Exception ex) {
                MessageBox.Show("错误：" + ex.Message, "PhotoStore", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void BrightnessMinusButton_Click(object sender, RoutedEventArgs e) {
            edited = true;
            SetUndoMenuItem();
            try {
                processor.BrightnessHandler.SetBrightness(-10);
                PaintPicture();
            }
            catch (Exception ex) {
                MessageBox.Show("错误：" + ex.Message, "PhotoStore", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void ContrastAddButton_Click(object sender, RoutedEventArgs e) {
            edited = true;
            SetUndoMenuItem();
            try {
                processor.ContrastHandler.SetContrast(10);
                PaintPicture();
            }
            catch (Exception ex) {
                MessageBox.Show("错误：" + ex.Message, "PhotoStore", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void ContrastMinusButton_Click(object sender, RoutedEventArgs e) {
            edited = true;
            SetUndoMenuItem();
            try {
                processor.ContrastHandler.SetContrast(-10);
                PaintPicture();
            }
            catch (Exception ex) {
                MessageBox.Show("错误：" + ex.Message, "PhotoStore", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void GrayScaleButton_Click(object sender, RoutedEventArgs e) {
            edited = true;
            SetUndoMenuItem();
            try {
                processor.GrayscaleHandler.SetGrayscale();
                PaintPicture();
            }
            catch (Exception ex) {
                MessageBox.Show("错误：" + ex.Message, "PhotoStore", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void YFlipButton_Click(object sender, RoutedEventArgs e) {
            edited = true;
            SetUndoMenuItem();
            try {
                processor.RotationHandler.Flip(RotateFlipType.RotateNoneFlipY);
                PaintPicture();
            }
            catch(Exception ex) {
                MessageBox.Show("错误：" + ex.Message, "PhotoStore", MessageBoxButton.OK, MessageBoxImage.Error);
            }
        }

        private void XFlipButton_Click(object sender, RoutedEventArgs e) {
            edited = true;
            SetUndoMenuItem();
            try {
                processor.RotationHandler.Flip(RotateFlipType.RotateNoneFlipX);
                PaintPicture();
            }
            catch(Exception ex) {
                MessageBox.Show("错误：" + ex.Message, "PhotoStore", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            HideResizePanel();
        }

        private void Rotate90Button_Click(object sender, RoutedEventArgs e) {
            edited = true;
            SetUndoMenuItem();
            try {
                processor.RotationHandler.Flip(RotateFlipType.Rotate90FlipNone);
                PaintPicture();
            }
            catch (Exception ex) {
                MessageBox.Show("错误：" + ex.Message, "PhotoStore", MessageBoxButton.OK, MessageBoxImage.Error);
            }
            HideResizePanel();
        }

        private void Rotate270Button_Click(object sender, RoutedEventArgs e) {
            edited = true;
            SetUndoMenuItem();
            try {
                processor.RotationHandler.Flip(RotateFlipType.Rotate270FlipNone);
                PaintPicture();
            }
            catch(Exception ex) {
                MessageBox.Show("错误：" + ex.Message, "PhotoStore", MessageBoxButton.OK, MessageBoxImage.Error);

            }
            HideResizePanel();
        }

        private void ResizeButton_Click(object sender, RoutedEventArgs e) {
            edited = true;
            if (resizePanel.Visibility == Visibility.Visible) {
                HideResizePanel();
            }
            else {
                this.resizePanel.SetResizePanel(this, processor);
                this.resizePanel.Visibility = Visibility.Visible;
            }
        }

        private void OpenFileButton_Click(object sender, RoutedEventArgs e) {
            OpenFile();
        }

        #endregion


    }
}
