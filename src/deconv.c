#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lib/func.h"

uint8_t *buffer;															//Buffer to hold input data
char *dump;																	//Buffer to hold output data

struct h_def *header;														//Pointer to file header

uint32_t colnum;															//Number of colors found
struct c_def *head,**seek;													//List of colors

FILE *input,*output;														//IO stream pointers
uint32_t insize;															//File size

int main(int argc,char*argv[]){
	if((argc==2)||(argc==3)){
		if((input=fopen(argv[1],"rb"))){																	//Open input file
			buffer=malloc(6);
			fread(buffer,1,6,input);
			header=(struct h_def*)buffer;
			insize=header->fsize;																			//Get file size
			if((header->signature[0]=='B')&&(header->signature[1]=='M')){
				free(buffer);
				fseek(input,0,SEEK_SET);
				buffer=malloc(insize);																		//Allocate big enough buffer
				if(insize==fread(buffer,1,insize,input)){													//Read file into buffer
					fclose(input);																			//Close file
					header=(struct h_def*)buffer;															//Set header pointer
					if((header->height > 0)&&(header->planes==1)&&
					(((header->compr==BI_RGB)&&(header->hsize>=24))||((header->compr==BI_ALPHABITFIELDS)&&
					(header->hsize>=56))||((header->compr==BI_BITFIELDS)&&(header->hsize>=52)))&&
					(((header->compr==BI_RGB)&&((header->bpp==1)||(header->bpp==2)||(header->bpp==4)||
					(header->bpp==8)||(header->bpp==24)))||(header->bpp==16)||(header->bpp==32))){			//Check if a sane file has been opened
						printheader(header,argv[1]);														//Dump header

						union c_uni tcod;
						for(int i=header->height-1,j=0;i>=0;j++){
							if(j>=header->width){j=0;i--;};
							if(i>=0){ 
								tcod=coloread(header,i*header->width+j);
								if(header->bpp==1){
									if(!j) printf("\n");
									printf("%c", tcod.color?'X':'.');
								}else if(header->bpp==4){
									if(!j) printf("\n");
									printf("%X", tcod.color);
								}else{
									if(!(j%10)) printf("\n");
									printf("%03X=%02X.%02X.%02X.%02X ",i*header->width+j,tcod.red,tcod.green,tcod.blue,tcod.alpha);
								};
							};
						};
						printf("\n");
						//Parse file to find number of colors and create palette

						colnum=0;

						//Find out size of output buffer and allocate it

						if(argc==3) output=fopen(argv[2],"w");												//Open output file if it exists
						else output=stdout;
						if(output){

						//Write output file

							if(argc==3) fclose(output);														//Close output file
							free(buffer);																	//Free buffer
							free(dump);
							return 0;																		//Exit - 0
						}else printf("Can\'t open \'%s\'\n", argv[2]);
						free(dump);
					}else printf("Unrecognised data format\n");
				}else{fclose(input);printf("Can\'t read \'%s\'\n", argv[1]);};								//Close file and error out
				free(buffer);																				//Free buffer
			}else{fclose(input);free(buffer);printf("\'%s\' is not a bitmap file\n", argv[1]);};			//Close file, free buffer, error out
		}else printf("Can\'t open \'%s\'\n", argv[1]);
	}else printf("Invalid arguments\n");

	printhelp(argv[0]);																						//Print help

	return 1;																								//Exit - 1
};
