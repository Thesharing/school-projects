using System.Drawing.Imaging;

namespace ImageFunctions
{
    public class FilterHandler
    {
        ImageHandler imageHandler;

        public FilterHandler(ImageHandler imageHandler)
        {
            this.imageHandler = imageHandler;
        }

        public void SetColorFilter(ColorFilterTypes colorFilterType)
        {
            imageHandler.RestorePrevious();
            ColorMatrix cMatrix = new ColorMatrix(CurrentColorMatrix.Array);
            switch (colorFilterType)
            {
                case ColorFilterTypes.Red:
                    cMatrix.Matrix11 = 0;
                    cMatrix.Matrix22 = 0;
                    break;
                case ColorFilterTypes.Green:
                    cMatrix.Matrix00 = 0;
                    cMatrix.Matrix22 = 0;
                    break;
                case ColorFilterTypes.Blue:
                    cMatrix.Matrix00 = 0;
                    cMatrix.Matrix11 = 0;
                    break;
            }
            imageHandler.ProcessBitmap(cMatrix);
        }

        public void SetAlphaFilter(int alphaValue)
        {
            imageHandler.RestorePrevious();
            if (alphaValue < -255) alphaValue = -255;
            if (alphaValue > 255) alphaValue = 255;
            ColorMatrix cMatrix = new ColorMatrix(CurrentColorMatrix.Array);
            cMatrix.Matrix33 = alphaValue / 255.0F;
            imageHandler.ProcessBitmap(cMatrix);
        }

        
    }
}
