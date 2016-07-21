#ifndef IMAGE_READER_H
#define IMAGE_READER_H
typedef struct
{
    unsigned int    bfType;
    unsigned long   bfSize;
    unsigned int    bfReserved1;
    unsigned int    bfReserved2;
    unsigned long   bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
    unsigned int    biSize;
    int             biWidth;
    int             biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned int    biCompression;
    unsigned int    biSizeImage;
    int             biXPelsPerMeter;
    int             biYPelsPerMeter;
    unsigned int    biClrUsed;
    unsigned int    biClrImportant;
} BITMAPINFOHEADER;

typedef struct
{
    int   rgbBlue;
    int   rgbGreen;
    int   rgbRed;
    int   rgbReserved;
} RGBQUAD;

static unsigned short read_u16(FILE *fp);
static unsigned int   read_u32(FILE *fp);
static int            read_s32(FILE *fp);

static void write_u16(unsigned short input, FILE *fp);
static void write_u32(unsigned int input, FILE *fp);
static void write_s32(int input, FILE *fp);

static unsigned short read_u16(FILE *fp)
{
    unsigned char b0, b1;

    b0 = getc(fp);
    b1 = getc(fp);

    return ((b1 << 8) | b0);
}


static unsigned int read_u32(FILE *fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}


static int read_s32(FILE *fp)
{
    unsigned char b0, b1, b2, b3;

    b0 = getc(fp);
    b1 = getc(fp);
    b2 = getc(fp);
    b3 = getc(fp);

    return ((int)(((((b3 << 8) | b2) << 8) | b1) << 8) | b0);
}

static void write_u16(unsigned short input, FILE *fp)
{
    putc(input, fp);
    putc(input >> 8, fp);
}

static void write_u32(unsigned int input, FILE *fp)
{
    putc(input, fp);
    putc(input >> 8, fp);
    putc(input >> 16, fp);
    putc(input >> 24, fp);
}

static void write_s32(int input, FILE *fp)
{
    putc(input, fp);
    putc(input >> 8, fp);
    putc(input >> 16, fp);
    putc(input >> 24, fp);
}

#endif // IMAGE_READER_H
