struct ImageData {
    unsigned char idLength;
    unsigned char colorMapType;
    unsigned char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    unsigned char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    unsigned char bitsPerPixel;
    unsigned char imageDescriptor;
};

struct Pixel {
    unsigned char BluePixel;
    unsigned char GreenPixel;
    unsigned char RedPixel;
};
