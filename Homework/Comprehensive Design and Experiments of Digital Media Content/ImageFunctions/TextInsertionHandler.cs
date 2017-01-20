using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;

namespace ImageFunctions
{
    public class TextInsertionHandler : InsertionHandler
    {
        ImageHandler imageHandler;

        public TextInsertionHandler(ImageHandler imageHandler)
        {
            this.imageHandler = imageHandler;
        }

        public void Insert(string text, int xPosition, int yPosition, string fontName, float fontSize, string fontStyle, float angle, int opacity, string color1, string color2, string gradientStyle)
        {
            imageHandler.RestorePrevious();
            Bitmap bmap = (Bitmap)imageHandler.CurrentBitmap.Clone();
            Graphics gr = Graphics.FromImage(bmap);
            Font font = GetFont(fontName, fontSize, fontStyle);
            SizeF  sF = gr.MeasureString(text, font, int.MaxValue);
            Rectangle rect = new Rectangle(0, 0, (int)sF.Width, (int)sF.Height);
            Color col1 = GetColor(color1);
            Color col2= new Color();
            if (string.IsNullOrEmpty(color2))
                col2 = col1;
            else
                col2 = GetColor(color2);
            LinearGradientBrush LGBrush = new LinearGradientBrush(rect, col1, col2, GetGradientStyle(gradientStyle));

            Bitmap i_bitmap = new Bitmap((int)sF.Width, (int)sF.Height);
            Graphics g1 = Graphics.FromImage(i_bitmap);
            g1.FillRectangle(Brushes.Transparent, 0, 0, (int)sF.Width, (int)sF.Height);
            g1.DrawString(text, font, LGBrush, 0, 0);
            if (opacity < -255) opacity = -255;
            if (opacity > 255) opacity = 255;
            ColorMatrix cMatrix = new ColorMatrix(CurrentColorMatrix.Array);
            cMatrix.Matrix33 = opacity / 255.0F;
            Bitmap bmap2 = new Bitmap(i_bitmap.Width, i_bitmap.Height);
            ImageAttributes imgAttributes = new ImageAttributes();
            imgAttributes.SetColorMatrix(cMatrix);
            Graphics g2 = Graphics.FromImage(bmap2);
            g2.InterpolationMode = InterpolationMode.NearestNeighbor;
            g2.DrawImage(i_bitmap, new Rectangle(0, 0, i_bitmap.Width, i_bitmap.Height), 0, 0, i_bitmap.Width, i_bitmap.Height, GraphicsUnit.Pixel, imgAttributes);
            i_bitmap = (Bitmap)bmap2.Clone();
            i_bitmap = Rotate(i_bitmap, angle);
            gr.DrawImage(i_bitmap, xPosition, yPosition);
            imageHandler.CurrentBitmap = (Bitmap)bmap.Clone();
        }
    }
}
