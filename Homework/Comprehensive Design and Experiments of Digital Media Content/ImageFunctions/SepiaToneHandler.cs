using System.Drawing.Imaging;

namespace ImageFunctions
{
    public class SepiaToneHandler
    {
        ImageHandler imageHandler;

        public SepiaToneHandler(ImageHandler imageHandler)
        {
            this.imageHandler = imageHandler;
        }

        public void SetSepiaTone()
        {
            imageHandler.RestorePrevious();
            ColorMatrix cMatrix = new ColorMatrix(CurrentColorMatrix.Array);
            cMatrix.Matrix00 = 0.393F;
            cMatrix.Matrix01 = 0.349F;
            cMatrix.Matrix02 = 0.299F;
            cMatrix.Matrix10 = 0.769F;
            cMatrix.Matrix11 = 0.686F;
            cMatrix.Matrix12 = 0.534F;
            cMatrix.Matrix20 = 0.189F;
            cMatrix.Matrix21 = 0.168F;
            cMatrix.Matrix22 = 0.131F;
            imageHandler.ProcessBitmap(cMatrix);
        }
    }
}
