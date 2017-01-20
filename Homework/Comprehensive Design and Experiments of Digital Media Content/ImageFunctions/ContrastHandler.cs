using System.Drawing.Imaging;

namespace ImageFunctions
{
    public class ContrastHandler
    {
        ImageHandler imageHandler;

        public ContrastHandler(ImageHandler imageHandler)
        {
            this.imageHandler = imageHandler;
        }

        public void SetContrast(float contrast)
        {
            imageHandler.RestorePrevious();
            if (contrast < -100) contrast = -100;
            if (contrast > 100) contrast = 100;
            contrast = (100.0F + contrast) / 100.0F;
            contrast *= contrast;
            ColorMatrix cMatrix = new ColorMatrix(CurrentColorMatrix.Array);
            float tValue = (1.0F - contrast) / 2.0F;
            cMatrix.Matrix00 = cMatrix.Matrix11 = cMatrix.Matrix22 = contrast;
            cMatrix.Matrix40 = cMatrix.Matrix41 = cMatrix.Matrix42 = tValue;
            imageHandler.ProcessBitmap(cMatrix);
        }
    }
}
