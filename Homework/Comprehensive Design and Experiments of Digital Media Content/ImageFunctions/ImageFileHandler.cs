using System.Drawing;

namespace ImageFunctions
{
    public class ImageFileHandler
    {
        ImageHandler imageHandler;

        public ImageFileHandler(ImageHandler imageHandler)
        {
            this.imageHandler = imageHandler;
        }

        public void Load(string bitmapPath)
        {
            imageHandler.CurrentBitmap = (Bitmap)Bitmap.FromFile(bitmapPath);
            imageHandler.CurrentBitmapPath = bitmapPath;
        }

        public void Save(string filePath)
        {
            imageHandler.CurrentBitmapPath = filePath;
            if (System.IO.File.Exists(filePath))
                System.IO.File.Delete(filePath);
            imageHandler.CurrentBitmap.Save(filePath);
        }
    }
}
