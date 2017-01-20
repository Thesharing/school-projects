using System.Drawing.Imaging;

namespace ImageFunctions
{
    public class InversionHandler
    {
        ImageHandler imageHandler;

        public InversionHandler(ImageHandler imageHandler)
        {
            this.imageHandler = imageHandler;
        }

        public void SetInversion()
        {
            imageHandler.RestorePrevious();
            ColorMatrix cMatrix = new ColorMatrix(CurrentColorMatrix.Array);
            cMatrix.Matrix00 = -1;
            cMatrix.Matrix11 = -1;
            cMatrix.Matrix22 = -1;
            imageHandler.ProcessBitmap(cMatrix);
        }
    }
}
