using System;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace ImageFunctions
{
    public class RotationHandler
    {
        ImageHandler imageHandler;
        public RotationHandler(ImageHandler imageHandler)
        {
            this.imageHandler = imageHandler;
        }

        public void Flip(RotateFlipType rotateFlipType)
        {
            imageHandler.RestorePrevious();
            Bitmap bmap = (Bitmap)imageHandler.CurrentBitmap.Clone();
            bmap.RotateFlip(rotateFlipType);
            imageHandler.CurrentBitmap = (Bitmap)bmap.Clone();
        }

        public void Rotate(float angle)
        {
            imageHandler.RestorePrevious();
            PointF[] rotationPoints = { new PointF(0, 0),
                                        new PointF(imageHandler.CurrentBitmap.Width, 0),
                                        new PointF(0, imageHandler.CurrentBitmap.Height),
                                        new PointF(imageHandler.CurrentBitmap.Width, imageHandler.CurrentBitmap.Height)};
            PointMath.RotatePoints(rotationPoints, new PointF(imageHandler.CurrentBitmap.Width / 2.0f, imageHandler.CurrentBitmap.Height / 2.0f), angle);
            Rectangle bounds = PointMath.GetBounds(rotationPoints);
            Bitmap bmap = new Bitmap(bounds.Width, bounds.Height);
            using (Graphics g = Graphics.FromImage(bmap))
            {
                g.SmoothingMode = SmoothingMode.HighQuality;
                g.InterpolationMode = InterpolationMode.HighQualityBicubic;
                Matrix m = new Matrix();
                m.RotateAt((float)angle, new PointF(imageHandler.CurrentBitmap.Width / 2.0f, imageHandler.CurrentBitmap.Height / 2.0f));
                m.Translate(-bounds.Left, -bounds.Top, MatrixOrder.Append);
                g.Transform = m;
                g.DrawImage(imageHandler.CurrentBitmap, 0, 0);
            }
            imageHandler.CurrentBitmap = (Bitmap)bmap.Clone();
        }
    }
}
