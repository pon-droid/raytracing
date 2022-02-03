#include<stdbool.h>
#include<math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb/stb_image_write.h"

#define IW 256.0
#define IH 256.0
#define FOV ((M_PI/180) * 60)
#define CH 3

typedef struct v3 {
	double x, y, z;
} v3;


double length(v3 l){
	return sqrt(l.x*l.x + l.y*l.y* + l.z*l.z);
}

v3 uni_vec(v3 v){
	v.x /= length(v);
	v.y /= length(v);
	v.z /= length(v);

	return v;
}

v3 divide_v(v3 v, v3 d){
	v.x /= d.x;
	v.y /= d.y;
	v.z /= d.z;

	return v;
}

v3 divide_d(v3 v, double t){
	v.x /= t;
	v.y /= t;
	v.z /= t;

	return v;
}

v3 multiply_d(v3 v, double t){
	v.x *= t;
	v.y *= t;
	v.z *= t;
	return v;
}

v3 add_v(v3 v, v3 t){
	v.x += t.x;
	v.y += t.y;
	v.z += t.z;

	return v;
}

v3 sub_v(v3 v, v3 t){
	v.x -= t.x;
	v.y -= t.y;
	v.z -= t.z;

	return v;
}

double dot_prod(v3 v, v3 t){
	return v.x * t.x + v.y * t.y + v.z * t.z;
}

typedef struct sphere {
	v3 centre;
	double radius;
} sphere;

bool ray_intersect_sphere(const sphere s,const v3 orig, const v3 dir){
	v3 l = sub_v(s.centre,orig);

	double tca = dot_prod(l,dir);
	double d2 = dot_prod(l,l) - (tca * tca);

	if( d2 > s.radius * s.radius) { return false; }

	double thc = sqrt(s.radius * s.radius - d2);

	double t0 = tca - thc;
	double t1 = tca + thc;
	if(t0 < 0) { t0 = t1; };
	if(t0 < 0) { return false; };

	return true;
}

void normalise(v3 *v){
	v->x /= sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->y /= sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->z /= sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

int main(int argc, char *argv[]){

	if(argc != 2) { printf("Wrong\n"); return -1;}

	
    sphere round = {{-3,0,-16},2};

	unsigned char *img = malloc(IW * IH * CH);

	unsigned char *p = img;
    double y, x;

    
	for( y = IH - 1; y >= 0; y--){
		for( x = 0; x < IW; x++){
			int align = (y * IW + x) * CH;

			double nx = (2*(x + 0.5)/IW -1)*tan(FOV/2.0)*IW/IH;
			double ny = (2*(y + 0.5)/IH -1)*tan(FOV/2.0);
			v3 dir = { .x = nx, .y = ny, .z = -1};
			normalise(&dir);
			v3 tdir = {0,0,0};

			if( ray_intersect_sphere(round,tdir,dir)){
			  *(p + align) = 255;
			  *(p + align + 1) = 255;
			  *(p + align + 2) = 255;	
			} else {
              *(p + align) = 0;
			  *(p + align + 1) = 0;
			  *(p + align + 2) = 0;
			}
			/*
			double r = x / (IW);
			double g = y / (IH);
			double b = 0.2;

			*(p + align) = r * 255.999;
			*(p + align + 1) = g * 255.999;
			*(p + align + 2) = b * 255.999;
			*/
		}
	}
    printf("Wrote %s\n",argv[1]);
	stbi_write_png(argv[1],IW,IH,CH,img, IW * CH);

	free(img);
	return 0;
}
