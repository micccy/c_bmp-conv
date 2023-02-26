struct __attribute((__packed__)) h_def{
	//File header
	char signature[2];						//"BM"
	uint32_t fsize;							//File size
	uint16_t reserved[2];					//Reserved
	uint32_t offset;						//Offset of pixel data
	//DIB header
	uint32_t hsize;							//Size of *DIB* header
	 int32_t width;							//bitmap width
	 int32_t height;						//bitmap height
	uint16_t planes;						//number of color planes, must be 1
	uint16_t bpp;							//Bits per pixel
	uint32_t compr;							//Compression method and flags
#define BI_RGB 0							// No compression
#define BI_BITFIELDS 3						// Bitfields providd for colors
#define BI_ALPHABITFIELDS 6					// Very rare alternative to previous value
	uint32_t isize;							//Bitmap data size
	 int32_t hres;							//Horizontal resolution in pixel per meter
	 int32_t vres;							//Vertical resolution in pixel per meter
	uint32_t ncol;							//Number of colors in palette (0 to default)
	uint32_t icnm;							//Number of important colors, 0 to ignore
	uint32_t rmsk;							//Bit mask for red
	uint32_t gmsk;							//Bit mask for gree
	uint32_t bmsk;							//Bit mask for blue
	uint32_t amsk;							//Bit mask for alpha
};

union c_uni{
	struct __attribute((__packed__)){
		uint8_t red;
		uint8_t blue;
		uint8_t green;
		uint8_t alpha;
	};
	uint32_t color;
};

struct __attribute((__packed__)) c_def{		//List of colors found
	union c_uni code;
	uint32_t times;
	struct c_def *next;
};

struct __attribute((__packed__)) s_def{
	uint16_t mag   :  6;					//Struct for typing out file sizes
	uint16_t value : 10;
};
