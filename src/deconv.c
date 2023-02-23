#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lib/struct.h"

uint8_t *buffer;                                                            //Buffer to hold input data
char *dump;                                                                 //Buffer to hold output data

struct h_def *header;                                                       //Pointer to file header

uint8_t *pix;                                                               //Pointer to pixel table

uint32_t colnum;                                                            //Number of colors found
struct c_def *head,**seek;                                                  //List of colors

FILE *input,*output;                                                        //IO stream pointers
uint32_t insize;                                                            //File size
struct s_def prsiz;                                                         //Printable file size
const char magind[][4]={"B\0\0", "KiB", "MiB", "GiB"};                      //Text to display for each magnitude

uint32_t t0,t1;                                                             //tmp

int main(int argc,char*argv[]){
    if((argc==2)||(argc==3)){
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
                    (header->hsize>=52)))&&((header->bpp==1)||(header->bpp==2)||(header->bpp==4)||
                    (header->bpp==8)||(header->bpp==16)||(header->bpp==24)||(header->bpp==32))){            //Check if a sane file has been opened
                        prsiz.mag=0;
                        t0=header->fsize;
                        while(t0>1023){t0/=1024;prsiz.mag++;};
                        prsiz.value=t0;
                        printf("\'%s\' %u%s, ",argv[1], prsiz.value, magind[prsiz.mag]);                    //Print name and size of opened file
                        printf("%ix%i, %ubpp, ",header->width, header->height, header->bpp);                //Print dimensions and bits per pixel
                        if(header->compr){                                                                  //If bitfields are specified, print them with RGBAX syntax
                            t0=0;
                            t1=32;
                            for(int i=0;i<32;i++) t0+=((header->rmsk&(1<<i))?1:0);
                            printf("%u.", t0);
                            t1-=t0;
                            t0=0;
                            for(int i=0;i<32;i++) t0+=((header->gmsk&(1<<i))?1:0);
                            printf("%u.", t0);
                            t1-=t0;
                            t0=0;
                            for(int i=0;i<32;i++) t0+=((header->bmsk&(1<<i))?1:0);
                            printf("%u.", t0);
                            t1-=t0;
                            t0=0;
                            for(int i=0;i<32;i++) t0+=((header->amsk&(1<<i))?1:0);
                            t1-=t0;
                            printf("%u.%u, ", t0,t1);
                        }
                        if(header->ncol) printf("%ucol, ", header->ncol);                                   //If not inferred from the bpp, print the number of colors
                        if(header->hres!=header->vres) printf("%ix", (header->hres*127)/5000+
                        ((header->hres*127)%5000>2499?1:0));                                                //If not equal to vertical, printorizontal resolution
                        printf("%idpi\n", (header->vres*127)/5000+((header->vres*127)%5000>2499?1:0));      //Print vertical resolution

                        //Parse file to find number of colors and create palette
                        colnum=0;

                        //Find out size of output buffer and allocate it

                        if(argc==3) output=fopen(argv[2],"w");                                              //Open output file if it exists
                        else output=stdout;
                        if(output){

                            if(argc==3) fclose(output);                                                     //Close output file
                            free(buffer);                                                                   //Free buffer
                            free(dump);
                            return 0;                                                                       //Exit - 0
                        }else printf("Can\'t open \'%s\'\n", argv[2]);
                        free(dump);
                    }else printf("Unrecognised data format\n");
                }else{fclose(input);printf("Can\'t read \'%s\'\n", argv[1]);};                              //Close file and error out
                free(buffer);                                                                               //Free buffer
            }else{fclose(input);free(buffer);printf("\'%s\' is not a bitmap file\n", argv[1]);};            //Close file, free buffer, error out
        }else printf("Can\'t open \'%s\'\n", argv[1]);
    }else printf("Invalid arguments\n");

    printf("%s", helpstr);                                                                                  //Print help

    return 1;                                                                                               //Exit - 1
}
