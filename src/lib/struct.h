struct __attribute((__packed__)) h_def{
    //File header
    char signature[2];                                                      //"BM"
    uint32_t fsize;                                                         //File size
    uint16_t reserved[2];                                                   //Reserved
    uint32_t offset;                                                        //Offset of pixel data
    //DIB header
    uint32_t hsize;                                                         //Size of *DIB* header
     int32_t width;                                                         //bitmap width
     int32_t height;                                                        //bitmap height
    uint16_t planes;                                                        //number of color planes, must be 1
    uint16_t bpp;                                                           //Bits per pixel
    uint32_t compr;                                                         //Compression method and flags
#define BI_RGB 0                                                            //  No compression
#define BI_BITFIELDS 3                                                      //  Bitfields providd for colors
    uint32_t isize;                                                         //Bitmap data size
     int32_t hres;                                                          //Horizontal resolution in pixel per meter
     int32_t vres;                                                          //Vertical resolution in pixel per meter
    uint32_t ncol;                                                          //Number of colors in palette (0 to default)
    uint32_t icnm;                                                          //Number of important colors, 0 to ignore
    uint32_t rmsk;                                                          //Bit mask for red
    uint32_t gmsk;                                                          //Bit mask for gree
    uint32_t bmsk;                                                          //Bit mask for blue
    uint32_t amsk;                                                          //Bit mask for alpha
};

struct __attribute((__packed__)) c_def{                                     //List of colors found
    union{
        struct __attribute((__packed__)){
        uint8_t red;
        uint8_t blue;
        uint8_t green;
        uint8_t alpha;
        };
        uint32_t color;
    };
    uint32_t times;
    struct c_def *next;
};

struct __attribute((__packed__)) s_def{
    uint16_t mag   :  6;                                                    //Struct for typing out file sizes
    uint16_t value : 10;
};

const char helpstr[]={                                                      //Help text
    "This tool can convert a bitmap image in a text file based format\n\
    which can then be used as a seed to generate other images\n\n\
        usage: deconv input_file [output_file]\n\n\
    The input file must be a (small) bitmap image file having at most\n\
    95 colors and at least a BITMAPINFOHEADER, if more colors are\n\
    present they will be united by similarity to reduce them\n\n\
    The output file will be a simple text delimited by \'#\' where\n\
    each character identifies one pixel\'s color and a palette\n\
    will then be provided below in RGBA format\n\
    This is meant to be used in making small retro icons etc\n\n"
};
