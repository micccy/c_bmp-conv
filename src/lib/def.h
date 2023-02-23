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

void printhelp(char *filename){                                                      //Help text
    printf("\n\
    This tool can convert a bitmap image in a text file based format\n\
    which can then be used as a seed to generate other images\n\n\
        usage: %s input_file [output_file]\n\n\
    The input file must be a (small) bitmap image file having at most\n\
    95 colors and at least a BITMAPINFOHEADER, if more colors are\n\
    present they will be united by similarity to reduce them\n\n\
    The output file will be a simple text delimited by \'#\' where\n\
    each character identifies one pixel\'s color and a palette\n\
    will then be provided below in RGBA format\n\
    This is meant to be used in making small retro icons etc\n\n", filename);
};

void printheader(struct h_def *h,char *filename){
    struct s_def prsiz;                                                                 //Printable file size
    const char magind[][4]={"B\0\0", "KiB", "MiB", "GiB"};                              //Text to display for each magnitude
    uint32_t t0,t1;                                                                     //tmp

    prsiz.mag=0;
    t0=h->fsize;
    while(t0>1023){t0/=1024;prsiz.mag++;};
    prsiz.value=t0;
    printf("\'%s\' %u%s, ",filename, prsiz.value, magind[prsiz.mag]);                    //Print name and size of opened file
    printf("%ix%i, %ubpp, ",h->width, h->height, h->bpp);                //Print dimensions and bits per pixel
    if(h->compr){                                                                  //If bitfields are specified, print them with RGBAX syntax
        t0=0;
        t1=32;
        for(int i=0;i<32;i++) t0+=((h->rmsk&(1<<i))?1:0);
        printf("%u.", t0);
        t1-=t0;
        t0=0;
        for(int i=0;i<32;i++) t0+=((h->gmsk&(1<<i))?1:0);
        printf("%u.", t0);
        t1-=t0;
        t0=0;
        for(int i=0;i<32;i++) t0+=((h->bmsk&(1<<i))?1:0);
        printf("%u.", t0);
        t1-=t0;
        t0=0;
        for(int i=0;i<32;i++) t0+=((h->amsk&(1<<i))?1:0);
        t1-=t0;
        printf("%u.%u, ", t0,t1);
    };
    if(h->ncol) printf("%ucol, ", h->ncol);                                   //If not inferred from the bpp, print the number of colors
    if(h->hres!=h->vres) printf("%ix", (h->hres*127)/5000+
    ((h->hres*127)%5000>2499?1:0));                                                //If not equal to vertical, printorizontal resolution
    printf("%idpi\n", (h->vres*127)/5000+((h->vres*127)%5000>2499?1:0));      //Print vertical resolution
}