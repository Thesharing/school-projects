using System.Drawing.Imaging;

namespace ImageFunctions
{
    public class BrightnessHandler
    {
        ImageHandler imageHandler;

        public BrightnessHandler(ImageHandler imageHandler)
        {
            this.imageHandler = imageHandler;
        }

        public void SetBrightness(int brightness)
        {
            imageHandler.RestorePrevious();
            if (brightness < -255) brightness = -255;
            if (brightness > 255) brightness = 255;
            ColorMatrix cMatrix = new ColorMatrix(CurrentColorMatrix.Array);
            cMatrix.Matrix40 = cMatrix.Matrix41 = cMatrix.Matrix42 = brightness / 255.0F;
            imageHandler.ProcessBitmap(cMatrix);
        }
    }
}
