using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;

namespace ImageFunctions
{
    public enum ColorFilterTypes
    {
        Red,
        Green,
        Blue,
    };

    internal class CurrentColorMatrix
    {
        public static float[][] Array = {
                                    new float[]{1,0,0,0,0},
                                    new float[]{0,1,0,0,0},
                                    new float[]{0,0,1,0,0},
                                    new float[]{0,0,0,1,0},
                                    new float[]{0,0,0,0,1},
                                };
    }


    public class ImageHandler
    {
        private Bitmap _currentBitmap;
        private Bitmap _bitmapbeforeProcessing;

        public Bitmap CurrentBitmap
        {
            get
            {
                if (_currentBitmap == null)
                {
                    _currentBitmap = new Bitmap(1, 1);
                }
                return _currentBitmap;
            }
            set { _currentBitmap = value; }
        }

        public string CurrentBitmapPath { get; set; }

        public void ResetBitmap()
        {
            if (_currentBitmap != null && _bitmapbeforeProcessing != null)
            {
                Bitmap temp = (Bitmap)_currentBitmap.Clone();
                _currentBitmap = (Bitmap)_bitmapbeforeProcessing.Clone();
                _bitmapbeforeProcessing = (Bitmap)temp.Clone();
            }
        }

        public void ClearImage()
        {
            RestorePrevious();
            _currentBitmap = new Bitmap(1, 1);
        }

        public Bitmap ThumbnailView(int thumbWidth, int thumbHeight)
        {
            return (Bitmap)_currentBitmap.GetThumbnailImage(thumbWidth, thumbHeight, new Image.GetThumbnailImageAbort(abort), System.IntPtr.Zero);
        }

        public void Resize(int newWidth, int newHeight)
        {
            if (newWidth != 0 && newHeight != 0)
            {
                RestorePrevious();
                Bitmap temp = (Bitmap)_currentBitmap;
                Bitmap bmap = new Bitmap(newWidth, newHeight, temp.PixelFormat);
                Graphics g = Graphics.FromImage(bmap);
                g.InterpolationMode = InterpolationMode.HighQualityBicubic;
                Rectangle srcRect = new Rectangle(0, 0, _currentBitmap.Width, _currentBitmap.Height);
                Rectangle destRect = new Rectangle(0, 0, newWidth, newHeight);
                g.DrawImage(_currentBitmap, destRect, srcRect, GraphicsUnit.Pixel);
                _currentBitmap = (Bitmap)bmap.Clone();
            }
        }

        private bool abort()
        {
            return true;
        }

        internal void RestorePrevious()
        {
            _bitmapbeforeProcessing = _currentBitmap;
        }

        internal void ProcessBitmap(ColorMatrix colorMatrix)
        {
            Bitmap bmap = new Bitmap(_currentBitmap.Width, _currentBitmap.Height);
            ImageAttributes imgAttributes = new ImageAttributes();
            imgAttributes.SetColorMatrix(colorMatrix);
            Graphics g = Graphics.FromImage(bmap);
            g.InterpolationMode = InterpolationMode.NearestNeighbor;
            g.DrawImage(_currentBitmap, new Rectangle(0, 0, _currentBitmap.Width, _currentBitmap.Height), 0, 0, _currentBitmap.Width, _currentBitmap.Height, GraphicsUnit.Pixel, imgAttributes);
            _currentBitmap = (Bitmap)bmap.Clone();
        }
    }
}
