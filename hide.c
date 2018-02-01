#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <png.h>
#include <SDL/SDL_image.h>

int png_save_surface(char *filename, SDL_Surface *surf);
void png_user_error(png_structp ctx, png_const_charp str);
void png_user_warn(png_structp ctx, png_const_charp str);
static int png_colortype_from_surface(SDL_Surface *surface);

int main(int argc, char *argv[])
{
	if ( argc < 3 )
	{
		printf("USAGE: %s [file to hide] [image file(s) to hide file in]\n",argv[0]);
		return(1);
	}
	else if ( check(argc,argv) )
	{
		printf("Not Enough Space To Hide File\n");
		return(1);
	}
	else
	{
		printf("Hiding File\n");
		hide(argc,argv);
		return(0);
	}
}

int check(int count, char *files[])
{
	FILE *input;
	SDL_Surface *surface;
	unsigned long filesize=0;
	unsigned long pixel_count=0;
	int d=0;

	printf("%s %s\n%d\n", files[1], files[2],count);

	if ( ( input=fopen(files[1],"rb") ) == NULL ) exit(-1);
printf(".");
	fseek(input,0L,SEEK_END);
	filesize=ftell(input);
	fclose(input);

	for(d=2;d<count;d++)
	{
		surface=IMG_Load(files[d]);
		if ( surface == NULL ) exit(-1);
		printf(".");
		pixel_count=pixel_count+((surface->w*surface->h)*3);
		SDL_FreeSurface(surface);
	}

	if ( pixel_count > filesize*8 )
	{
		return(0);
	}else{
		return(1);
	}
}

int hide(int count, char *files[])
{
	FILE *input;
	SDL_Surface *surface;
	unsigned long filesize=0;
	unsigned long pixel_count=0;
	int start=0;
	int d,e,l;
	int inbyte=0;
	char outfile[32768];
	Uint8 *pixel;
	unsigned long temp=0;



	if ( ( input=fopen(files[1],"rb") ) == NULL ) exit(-1);
	fseek(input,0L,SEEK_END);
	filesize=ftell(input);
	fseek(input,0L,SEEK_SET);
	printf("FILESIZE: %d\n",filesize);

	for(d=2;d<count;d++)
	{
		strcpy(outfile,files[d]);
		outfile[strlen(outfile)-4]='\0';
		strcat(outfile,"-2.png");

		surface=IMG_Load(files[d]);
		pixel_count=(surface->w*surface->h)*3;
		pixel=(Uint8 *)surface->pixels;

		if( SDL_MUSTLOCK( surface ) ) 
		{ //Lock the surface 
			SDL_LockSurface( surface ); 
		} 

		if ( d == 2 )
		{
			start=32;

			for(e=0;e<32;e++)
			{
				temp=0;
				temp=filesize%2;
				if ( temp )
				{
					if ( pixel[e]%2 == 0 )
					{
						pixel[e]=pixel[e]+1;
					}
				}else{
					if ( pixel[e]%2 == 1 )
					{
						pixel[e]=pixel[e]-1;
					}
				}
				
				filesize >>= 1;
				
			}
		}else{
			start=0;
		}

		for(l=start;l<pixel_count;l=l+8)
		{
			if ( ! feof(input) )
			{
				inbyte=0;
				fread(&inbyte,1,1,input);
				
				for(e=0;e<8;e++)
				{
					temp=0;
					temp=inbyte%2;

					if ( temp )
					{
						if ( pixel[l+e]%2 == 0)
						{
							pixel[l+e]=pixel[l+e]+1;
						}
					}else{
						if ( pixel[l+e]%2 == 1)
						{
							pixel[l+e]=pixel[l+e]-1;
						}
					}
					inbyte >>= 1;
				}
			}else{
				l=pixel_count;
			}
		}

		if( SDL_MUSTLOCK( surface ) ) 
		{ //Lock the surface 
			SDL_UnlockSurface( surface ); 
		} 

		// write new image file
		if( png_save_surface(outfile, surface) < 0 ) exit(-1);
		SDL_FreeSurface(surface);
	}

	return(0);
}

static int png_colortype_from_surface(SDL_Surface *surface)
{
	int colortype = PNG_COLOR_MASK_COLOR; /* grayscale not supported */

	if (surface->format->palette)
		colortype |= PNG_COLOR_MASK_PALETTE;
	else if (surface->format->Amask)
		colortype |= PNG_COLOR_MASK_ALPHA;
		
	return colortype;
}


void png_user_warn(png_structp ctx, png_const_charp str)
{
	fprintf(stderr, "libpng: warning: %s\n", str);
}


void png_user_error(png_structp ctx, png_const_charp str)
{
	fprintf(stderr, "libpng: error: %s\n", str);
}


int png_save_surface(char *filename, SDL_Surface *surf)
{
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	int i, colortype;
	png_bytep *row_pointers;

	/* Opening output file */
	fp = fopen(filename, "wb");
	if (fp == NULL) {
		perror("fopen error");
		return -1;
	}

	/* Initializing png structures and callbacks */
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
		NULL, png_user_error, png_user_warn);
	if (png_ptr == NULL) {
		printf("png_create_write_struct error!\n");
		return -1;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		printf("png_create_info_struct error!\n");
		exit(-1);
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		exit(-1);
	}

	png_init_io(png_ptr, fp);

	colortype = png_colortype_from_surface(surf);
	png_set_IHDR(png_ptr, info_ptr, surf->w, surf->h, 8, colortype,	PNG_INTERLACE_NONE, 
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	/* Writing the image */
	png_write_info(png_ptr, info_ptr);
	png_set_packing(png_ptr);

	row_pointers = (png_bytep*) malloc(sizeof(png_bytep)*surf->h);
	for (i = 0; i < surf->h; i++)
		row_pointers[i] = (png_bytep)(Uint8 *)surf->pixels + i*surf->pitch;
	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, info_ptr);

	/* Cleaning out... */
	free(row_pointers);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);

	return 0;
}
