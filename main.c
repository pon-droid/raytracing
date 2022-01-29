#define STB_IMAGE_IMPLEMENTATION
#include "./stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb/stb_image_write.h"

#define IW 256
#define IH 256
#define CH 3

int main(int argc, char *argv[]){

	if(argc != 2) { printf("Wrong\n"); return -1;}

	unsigned char *img = malloc(IW * IH * CH);

	unsigned char *p = img;
    double y, x;
	for( y = IH - 1; y >= 0; y--){
		for( x = 0; x < IW; x++){
			int align = (y * IW + x) * CH;
			
			double r = x / (IW);
			double g = y / (IH);
			double b = 0.2;

			*(p + align) = r * 255.999;
			*(p + align + 1) = g * 255.999;
			*(p + align + 2) = b * 255.999;
		}
	}
    printf("Wrote %s\n",argv[1]);
	stbi_write_png(argv[1],IW,IH,CH,img, IW * CH);

	free(img);
	return 0;
}
