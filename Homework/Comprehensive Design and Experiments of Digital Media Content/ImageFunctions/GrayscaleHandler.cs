using System.Drawing.Imaging;

namespace ImageFunctions
{

    public class GrayscaleHandler
    {
        ImageHandler imageHandler;

        public GrayscaleHandler(ImageHandler imageHandler)
        {
            this.imageHandler = imageHandler;
        }

        public void SetGrayscale()
        {
            imageHandler.RestorePrevious();
            ColorMatrix cMatrix = new ColorMatrix(CurrentColorMatrix.Array);
            cMatrix.Matrix00 = cMatrix.Matrix01 = cMatrix.Matrix02 = 0.299F;
            cMatrix.Matrix10 = cMatrix.Matrix11 = cMatrix.Matrix12 = 0.587F;
            cMatrix.Matrix20 = cMatrix.Matrix21 = cMatrix.Matrix22 = 0.114F;
            imageHandler.ProcessBitmap(cMatrix);
        }
    }
}
