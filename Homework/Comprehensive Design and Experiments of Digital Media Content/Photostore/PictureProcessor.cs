using ImageFunctions;

namespace Photostore {
    public class PictureProcessor : ImageHandler{
        public BrightnessHandler BrightnessHandler { get; set; }
        public ContrastHandler ContrastHandler { get; set; }
        public GrayscaleHandler GrayscaleHandler { get; set; }
        public ImageFileHandler FileHandler { get; set; }
        public RotationHandler RotationHandler { get; set; }
        public PictureProcessor() : base() {
            BrightnessHandler = new BrightnessHandler(this);
            ContrastHandler = new ContrastHandler(this);
            GrayscaleHandler = new GrayscaleHandler(this);
            FileHandler = new ImageFileHandler(this);
            RotationHandler = new RotationHandler(this);
        }
    }
}
