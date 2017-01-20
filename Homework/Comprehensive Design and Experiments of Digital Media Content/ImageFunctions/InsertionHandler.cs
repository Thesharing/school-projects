using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;

namespace ImageFunctions
{
    public class InsertionHandler
    {
        protected Font GetFont(string fontName, float fontSize, string fontStyle)
        {
            return new Font(fontName, fontSize, GetFontStyle(fontStyle));
        }

        protected FontStyle GetFontStyle(string fontStyle)
        {
            FontStyle fStyle = FontStyle.Regular;
            if (!string.IsNullOrEmpty(fontStyle))
            {
                switch (fontStyle.ToLower())
                {
                    case "bold":
                        fStyle = FontStyle.Bold;
                        break;
                    case "italic":
                        fStyle = FontStyle.Italic;
                        break;
                    case "underline":
                        fStyle = FontStyle.Underline;
                        break;
                    case "strikeout":
                        fStyle = FontStyle.Strikeout;
                        break;

                }
            }
            return fStyle;
        }

        protected Color GetColor(string colorName)
        {
            Color c = Color.Black;
            if (colorName != string.Empty)
                c = Color.FromName(colorName);
            return c;
        }

        protected LinearGradientMode GetGradientStyle(string gradientStyle)
        {
            LinearGradientMode lgMode = LinearGradientMode.Vertical;
            if (!string.IsNullOrEmpty(gradientStyle))
            {
                switch (gradientStyle.ToLower())
                {
                    case "horizontal":
                        lgMode = LinearGradientMode.Horizontal;
                        break;
                    case "vertical":
                        lgMode = LinearGradientMode.Vertical;
                        break;
                    case "backwarddiagonal":
                        lgMode = LinearGradientMode.BackwardDiagonal;
                        break;
                    case "forwarddiagonal":
                        lgMode = LinearGradientMode.ForwardDiagonal;
                        break;

                }
            }
            return lgMode;
        }

        protected Bitmap Rotate(Bitmap bitmap, float angle)
        {
            PointF[] rotationPoints = { new PointF(0, 0),
                                        new PointF(bitmap.Width, 0),
                                        new PointF(0, bitmap.Height),
                                        new PointF(bitmap.Width, bitmap.Height)};
            PointMath.RotatePoints(rotationPoints, new PointF(bitmap.Width / 2.0f, bitmap.Height / 2.0f), angle);
            Rectangle bounds = PointMath.GetBounds(rotationPoints);
            Bitmap bmap = new Bitmap(bounds.Width, bounds.Height);
            using (Graphics g = Graphics.FromImage(bmap))
            {
                g.SmoothingMode = SmoothingMode.HighQuality;
                g.InterpolationMode = InterpolationMode.HighQualityBicubic;
                Matrix m = new Matrix();
                m.RotateAt((float)angle, new PointF(bitmap.Width / 2.0f, bitmap.Height / 2.0f));
                m.Translate(-bounds.Left, -bounds.Top, MatrixOrder.Append);
                g.Transform = m;
                g.DrawImage(bitmap, 0, 0);
            }
            return (Bitmap)bmap.Clone();
        }
    }
}
