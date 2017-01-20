using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;

namespace ImageFunctions
{
    public class ImageInsertionHandler : InsertionHandler
    {
        ImageHandler imageHandler;

        public ImageInsertionHandler(ImageHandler imageHandler)
        {
            this.imageHandler = imageHandler;
        }

        public void Insert(string path, int xPosition, int yPosition, int width, int height, float angle, int opacity)
        {
            imageHandler.RestorePrevious();
            Bitmap bmap = (Bitmap)imageHandler.CurrentBitmap.Clone();
            Graphics gr = Graphics.FromImage(bmap);

            if (!string.IsNullOrEmpty(path))
            {
                Bitmap i_bitmap = (Bitmap)Bitmap.FromFile(path);
                if (opacity < -255) opacity = -255;
                if (opacity > 255) opacity = 255;
                ColorMatrix cMatrix = new ColorMatrix(CurrentColorMatrix.Array);
                cMatrix.Matrix33 = opacity / 255.0F;
                Bitmap bmap2 = new Bitmap(i_bitmap.Width, i_bitmap.Height);
                ImageAttributes imgAttributes = new ImageAttributes();
                imgAttributes.SetColorMatrix(cMatrix);
                Graphics g = Graphics.FromImage(bmap2);
                g.InterpolationMode = InterpolationMode.NearestNeighbor;
                g.DrawImage(i_bitmap, new Rectangle(0, 0, i_bitmap.Width, i_bitmap.Height), 0, 0, i_bitmap.Width, i_bitmap.Height, GraphicsUnit.Pixel, imgAttributes);
                i_bitmap = (Bitmap)bmap2.Clone();
                i_bitmap = Rotate(i_bitmap, angle);
                gr.DrawImage(i_bitmap, xPosition, yPosition, width, height);
            }
            imageHandler.CurrentBitmap = (Bitmap)bmap.Clone();
        }
    }
}
