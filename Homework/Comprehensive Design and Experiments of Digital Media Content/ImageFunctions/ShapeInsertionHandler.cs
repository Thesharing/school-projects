using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;

namespace ImageFunctions
{
    public class ShapeInsertionHandler : InsertionHandler
    {
        ImageHandler imageHandler;

        public ShapeInsertionHandler(ImageHandler imageHandler)
        {
            this.imageHandler = imageHandler;
        }

        public void Insert(string shape, int xPosition, int yPosition, int width, int height, float angle, int opacity, string color1, string color2, string gradientStyle)
        {
            imageHandler.RestorePrevious();
            Bitmap bmap = (Bitmap)imageHandler.CurrentBitmap.Clone();
            Graphics gr = Graphics.FromImage(bmap);
            Color col1 = GetColor(color1);
            Color col2 = new Color();
            if (string.IsNullOrEmpty(color2))
                col2 = col1;
            else
                col2 = GetColor(color2);
            Rectangle rect = new Rectangle(0, 0, width, height);
            LinearGradientBrush LGBrush = new LinearGradientBrush(rect, col1, col2, GetGradientStyle(gradientStyle));
            Bitmap i_bitmap = new Bitmap(width, height);
            Graphics g1 = Graphics.FromImage(i_bitmap);
            g1.FillRectangle(Brushes.Transparent, 0, 0, width, height);
            switch (shape.ToLower())
            {
                case "filledellipse":
                    g1.FillEllipse(LGBrush, 0, 0, width, height);
                    break;
                case "filledrectangle":
                    g1.FillRectangle(LGBrush, 0, 0, width, height);
                    break;
                case "ellipse":
                    g1.DrawEllipse(new Pen(LGBrush), 0, 0, width, height);
                    break;
                case "rectangle":
                default:
                    g1.DrawRectangle(new Pen(LGBrush), 0, 0, width, height);
                    break;
            }
            g1.Dispose();
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
