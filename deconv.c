#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t *buffer;

struct __attribute((__packed__))__{
    char signature[2];                                                      //"BM"
    uint32_t size;                                                          //File size
    uint16_t reserved[2];                                                   //Reserved
    uint32_t offset;                                                        //Offset of pixel data
} *header;

uint8_t *pix;

FILE *input;
long insize;

int main(int argc,char*argv[]){
    if(argc==2){
        if((input=fopen(argv[1],"rb"))){                                    //Open input file
            fseek(input,0,SEEK_END);
            insize=ftell(input);                                            //Get file size
            fseek(input,0,SEEK_SET);
            buffer=malloc(insize);                                          //Allocate big enough buffer
            if(insize==fread(buffer,1,insize,input)){                       //Read file into buffer
                fclose(input);                                              //Close file



                free(buffer);                                               //Free buffer
                return 0;                                                   //Exit - 0
            }else{fclose(input);printf("Can\'t read \'%s\'\n", argv[1]);};  //Close file and error out
            free(buffer);                                                   //Free buffer
        }else printf("Can\'t open \'%s\'\n", argv[1]);
    }else printf("Invalid arguments\n");

    printf("HELP (TODO)\n");                                                //Print help

    return 1;                                                               //Exit - 1
}
