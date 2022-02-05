#include<stdbool.h>
#include<math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb/stb_image_write.h"

#define IW 1920
#define IH 1080
#define OBJ 3
#define LIT 1
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

v3 scalar_prod(v3 v, double t){
	v.x *= t;
	v.y *= t;
	v.z *= t;

	return v;
}

typedef struct sphere {
	v3 centre;
	double radius;
} sphere;

bool ray_intersect_sphere(const sphere s,const v3 orig, const v3 dir, double *t0){
	v3 l = sub_v(s.centre,orig);

	double tca = dot_prod(l,dir);
	double d2 = dot_prod(l,l) - (tca * tca);

	if( d2 > s.radius * s.radius) { return false; }

	double thc = sqrt(s.radius * s.radius - d2);

	*t0 = tca - thc;
	double t1 = tca + thc;
	if(*t0 < 0) { *t0 = t1; };
	if(*t0 < 0) { return false; };

	return true;
}

void normalise(v3 *v){
	v->x /= sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->y /= sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
	v->z /= sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
}

typedef struct light_source {
	v3 pos;
	double amount;
} light_source;

double max(double x, double y){
	if(x > y){
		return x;
	} else {
		return y;
	}
}

v3 render_spheres(light_source light_arr[],sphere obj_arr[], const v3 orig, const v3 dir){
	int i;
	v3 P, N;
	double t0 = 0;
	bool hit = true;
	for( i = 0; i < OBJ; i++){
	
		if(ray_intersect_sphere(obj_arr[i],orig,dir,&t0)){
		    hit = true;
		    v3 tmp = scalar_prod(dir,t0);
		    P = add_v(orig,tmp);
		    N  = sub_v(P,obj_arr[i].centre);
		    normalise(&N);
		    break;
		} else {
			hit = false;
		}
	}

	if(!hit){ return (v3){.3,.3,.50};}
	
    double light_amount = 0;

    for(i = 0; i < LIT; i++){
    	v3 l_dir = sub_v(light_arr[i].pos,P);
    	normalise(&l_dir);

    	light_amount += max((light_arr[i].amount * dot_prod(l_dir,N)),0);
    }
	return scalar_prod((v3){.25,.50,.70},light_amount);
}

double min(double x, double y){
	if(x < y){
		return y;
	} else {
		return x;
	}
}


int main(int argc, char *argv[]){

	if(argc != 2) { printf("Wrong\n"); return -1;}

	sphere obj_arr[OBJ] =
	{
		//0
		{
			{-3,0,-16},
			1
		},
		//1
		{
			{1,2,-20},
			3
		},
	    //2
		{
			{1,2,-20},
			5
		}
	};

	light_source light_arr[LIT] =
	{
		//0
		{
			{1,-25,0},
			.34
		}
	};



	unsigned char *img = malloc(IW * IH * CH);

	unsigned char *p = img;
    double y, x;

    
	for( y = IH - 1; y >= 0; y--){
		for( x = 0; x < IW; x++){
			int align = (y * IW + x) * CH;
            //A formula that manages projection from the world to the screen
            //16 pixels = 2 * tan(fov/2)
			double nx = (2*(x + 0.5)/IW -1)*tan(FOV/2.0)*IW/IH; // "*IW/IH" to scale to aspect ratio
			double ny = (2*(y + 0.5)/IH -1)*tan(FOV/2.0);
			v3 dir = { .x = nx, .y = ny, .z = -1};
			//Convert to unit length
			normalise(&dir);
			
			v3 rgb = render_spheres(light_arr,obj_arr,(v3){0,0,0},dir);

			*(p + align) = rgb.x * 255;
			*(p + align + 1) = rgb.y * 255;
			*(p + align + 2) = rgb.z * 255;
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
