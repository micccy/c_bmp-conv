#include "def.h"

void phelp(char *filename){													//Help text
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

void pheader(struct h_def *h,char *filename){								//Dump info about the file
	struct s_def prsiz;														//Printable file size
	const char magind[][4]={"B\0\0", "KiB", "MiB", "GiB"};					//Text to display for each magnitude
	uint32_t t0,t1,i,j,rb;													//tmp
	uint8_t *b;
	prsiz.mag=0;
	t0=h->fsize;
	while(t0>1023){t0/=1024;prsiz.mag++;};
	prsiz.value=t0;
	printf("\'%s\' %u%s, ",filename, prsiz.value, magind[prsiz.mag]);		//Print name and size of opened file
	printf("%ix%i, %ubpp, ",h->width, h->height, h->bpp);					//Print dimensions and bits per pixel
	if(h->compr){															//If bitfields are specified, print them with RGBAX syntax
		for(j=0,b=(uint8_t*)&h->rmsk,t0=0,t1=32;j<4;j++,b+=4){
			rb=*b+(*(b+1)<<8)+(*(b+2)<<16)+(*(b+3)<<24);					//read like this to protect against misalignments
			for(i=0;i<32;i++) t0+=((rb&(1<<i))?1:0);
			printf("%u.", t0);
			t1-=t0;
			t0=0;
		};
		printf("%u, ", t1);													//Print number of unused bits
	};
	if(h->ncol) printf("%ucol, ", h->ncol);									//If not inferred from the bpp, print the number of colors
	if(h->hres!=h->vres) printf("%ix", (h->hres*127)/5000+
	((h->hres*127)%5000>2499?1:0));											//If not equal to vertical, printorizontal resolution
	printf("%idpi\n", (h->vres*127)/5000+((h->vres*127)%5000>2499?1:0));	//Print vertical resolution
};

union c_uni rpixel(struct h_def *h,uint32_t pn){							//Read the value of a pixel
	uint8_t *p=((uint8_t*)h)+h->offset,o;									//Pointer to start of pixel array, in-byte offset
	union c_uni code;														//Value to return
	uint64_t rowsize=((uint64_t)h->width*h->bpp);							//Size of row from header
	if(rowsize%32) rowsize+=32;												//Rows always start on a 4 byte boundary
	rowsize/=32;															//Number of dwords per row
	rowsize*=4;																//Number of bytes per row
	p+=rowsize*(pn/h->width);												//Start of current row
	p+=((pn%h->width)*h->bpp)/8;											//Set pointer to first byte
	o=((pn%h->width)*h->bpp)%8;												//Set offset in byte
	code.color=*((uint32_t*)p);												//Read 4 bytes at pointed offset
	if(h->bpp <8) code.color>>=(8-(h->bpp)-o);								//Pixels are stored from the most significant bit
	code.color&=(((uint64_t)1<<(h->bpp))-1);								//Mask out other stuff
	return code;
};

union c_uni rpalette(struct h_def *h,uint32_t n){							//Convert a pixel value in a color
	union c_uni val;
	val.color=0;
	return val;
};
