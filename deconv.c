#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t *b;

FILE *input;
long insize;

int main(int argc,char*argv[]){
    if(argc==2){
        if((input=fopen(argv[1],"rb"))){                                    //Open input file
            fseek(input,0,SEEK_END);
            insize=ftell(input);                                            //Get file size
            fseek(input,0,SEEK_SET);
            b=malloc(insize);                                               //Allocate big enough buffer
            if(insize==fread(b,1,insize,input)){                            //Read file into buffer
                fclose(input);                                              //Close file

                free(b);                                                    //Free buffer
                return 0;                                                   //Exit - 0
            }else{fclose(input);printf("Can\'t read \'%s\'\n", argv[1]);};  //Close file and error out
            free(b);                                                        //Free buffer
        }else printf("Can\'t open \'%s\'\n", argv[1]);
    }else printf("Invalid arguments\n");

    printf("HELP (TODO)\n");                                                //Print help

    return 1;                                                               //Exit - 1
}
