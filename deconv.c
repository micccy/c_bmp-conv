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
uint32_t temp,t0,insize;

const char magind[][4]={"B\0\0", "KiB", "MiB", "GiB"};
struct __attribute((__packed__)) s_def{
    uint16_t mag   :  6;                                                    //Struct for typing out file sizes
    uint16_t value : 10;
} prsiz;

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
                        prsiz.mag=0;
                        temp=header->fsize;
                        while(temp>1023){temp/=1024;prsiz.mag++;};
                        prsiz.value=temp;
                        printf("\'%s\' %u%s, ",argv[1], prsiz.value, magind[prsiz.mag]);                    //Print name and size of opened file
                        printf("%ix%i, %ubpp, ",header->width, header->height, header->bpp);                 //Print dimensions and bits per pixel
                        if(header->compr){                                                                  //If bitfields are specified, print them with RGBAX syntax
                            temp=0;
                            t0=32;
                            for(int i=0;i<32;i++) temp+=((header->rmsk&(1<<i))?1:0);
                            printf("%u.", temp);
                            t0-=temp;
                            temp=0;
                            for(int i=0;i<32;i++) temp+=((header->gmsk&(1<<i))?1:0);
                            printf("%u.", temp);
                            t0-=temp;
                            temp=0;
                            for(int i=0;i<32;i++) temp+=((header->bmsk&(1<<i))?1:0);
                            printf("%u.", temp);
                            t0-=temp;
                            temp=0;
                            for(int i=0;i<32;i++) temp+=((header->amsk&(1<<i))?1:0);
                            t0-=temp;
                            printf("%u.%u, ", temp,t0);
                        }
                        if(header->ncol) printf("%ucol, ", header->ncol);                                   //If not inferred from the bpp, print the number of colors
                        if(header->hres!=header->vres) printf("%ix", (header->hres*127)/5000+
                        ((header->hres*127)%5000>2499?1:0));                                                //If not equal to vertical, printorizontal resolution
                        printf("%idpi\n", (header->vres*127)/5000+((header->vres*127)%5000>2499?1:0));      //Print vertical resolution


                        free(buffer);                                                                       //Free buffer
                        return 0;                                                                           //Exit - 0
                    };
                }else{fclose(input);printf("Can\'t read \'%s\'\n", argv[1]);};                              //Close file and error out
                free(buffer);                                                                               //Free buffer
            }else{fclose(input);free(buffer);printf("\'%s\' is not a bitmap file\n", argv[1]);};            //Close file, free buffer, error out
        }else printf("Can\'t open \'%s\'\n", argv[1]);
    }else printf("Invalid arguments\n");

    printf("HELP (TODO)\n");                                                                                //Print help

    return 1;                                                                                               //Exit - 1
}
