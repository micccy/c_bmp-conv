#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t *buffer;

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
} *header;

uint8_t *pix;

FILE *input;
uint32_t insize;

int main(int argc,char*argv[]){
    if(argc==2){
        if((input=fopen(argv[1],"rb"))){                                                                    //Open input file
            buffer=malloc(6);
            fread(buffer,1,6,input);
            header=(struct h_def*)buffer;
            insize=header->fsize;                                                                           //Get file size
            if((header->signature[0]=='B')&&(header->signature[1]=='M')){
                free(buffer);
                fseek(input,0,SEEK_SET);
                buffer=malloc(insize);                                                                      //Allocate big enough buffer
                if(insize==fread(buffer,1,insize,input)){                                                   //Read file into buffer
                    fclose(input);                                                                          //Close file
                    header=(struct h_def*)buffer;                                                           //Set header pointer

                    if((header->height > 0)&&(header->planes==1)&&
                    (((header->compr==BI_RGB)&&(header->hsize>=24))||((header->compr==BI_BITFIELDS)&&
                    (header->hsize>=52)))){                                                                 //Check if a sane file has been opened

                        printf("Header size: %u %ix%i %08X:%08X:%08X:%08X (RGBA)\n", header->hsize,header->width,header->height,header->rmsk,header->gmsk,header->bmsk,header->amsk);

                        free(buffer);                                               //Free buffer
                        return 0;                                                   //Exit - 0
                    };
                }else{fclose(input);printf("Can\'t read \'%s\'\n", argv[1]);};                              //Close file and error out
                free(buffer);                                                                               //Free buffer
            }else{fclose(input);free(buffer);printf("\'%s\' is not a bitmap file\n", argv[1]);};            //Close file, free buffer, error out
        }else printf("Can\'t open \'%s\'\n", argv[1]);
    }else printf("Invalid arguments\n");

    printf("HELP (TODO)\n");                                                                                //Print help

    return 1;                                                                                               //Exit - 1
}
