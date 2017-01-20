using System.Drawing;

namespace ImageFunctions
{
    public class CropHandler
    {
        ImageHandler imageHandler;
        private Bitmap _bitmapPrevCropArea;

        public CropHandler(ImageHandler imageHandler)
        {
            this.imageHandler = imageHandler;
        }

        public void Crop(int xPosition, int yPosition, int width, int height)
        {
            Bitmap temp = (Bitmap)imageHandler.CurrentBitmap;
            Bitmap bmap = (Bitmap)temp.Clone();
            if (xPosition + width > imageHandler.CurrentBitmap.Width)
                width = imageHandler.CurrentBitmap.Width - xPosition;
            if (yPosition + height > imageHandler.CurrentBitmap.Height)
                height = imageHandler.CurrentBitmap.Height - yPosition;
            Rectangle rect = new Rectangle(xPosition, yPosition, width, height);
            imageHandler.CurrentBitmap = (Bitmap)bmap.Clone(rect, bmap.PixelFormat);
        }

        public void DrawOutCropArea(int xPosition, int yPosition, int width, int height)
        {
            imageHandler.RestorePrevious();
            _bitmapPrevCropArea = (Bitmap)imageHandler.CurrentBitmap;
            Bitmap bmap = (Bitmap)_bitmapPrevCropArea.Clone();
            Graphics gr = Graphics.FromImage(bmap);
            Brush cBrush = new Pen(Color.FromArgb(150, Color.White)).Brush;
            Rectangle rect1 = new Rectangle(0, 0, imageHandler.CurrentBitmap.Width, yPosition);
            Rectangle rect2 = new Rectangle(0, yPosition, xPosition, height);
            Rectangle rect3 = new Rectangle(0, (yPosition + height), imageHandler.CurrentBitmap.Width, imageHandler.CurrentBitmap.Height);
            Rectangle rect4 = new Rectangle((xPosition + width), yPosition, (imageHandler.CurrentBitmap.Width - xPosition - width), height);
            gr.FillRectangle(cBrush, rect1);
            gr.FillRectangle(cBrush, rect2);
            gr.FillRectangle(cBrush, rect3);
            gr.FillRectangle(cBrush, rect4);
            imageHandler.CurrentBitmap = (Bitmap)bmap.Clone();
        }

        public void RemoveCropAreaDraw()
        {
            imageHandler.CurrentBitmap = (Bitmap)_bitmapPrevCropArea.Clone();
        }
    }
}
