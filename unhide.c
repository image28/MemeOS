#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <python2.7/Python.h>

int main(int argc, char *argv[])
{
	if ( argc < 2 )
	{
		printf("USAGE: %s [image file(s) to extract data from]\n",argv[0]);
		return(1);
	}else{
		find(argc,argv);
		return(0);
	}
}

int find(int count, char *files[])
{
	SDL_Surface *surface;
	unsigned long filesize=0;
	unsigned long pixel_count=0;
	int start=0;
	int d,e,l;
	int inbyte=0;
	unsigned char command[1024];
	int lc=0;
	Uint8 *pixel;
	int temp=0;
	long pos=0;
	int first=1;
	int png=0;

	for(d=2;d<count;d++)
	{
		if ( files[d][strlen(files[d])-3] == 'p' )
		{	
			png++;
			if ( files[d][strlen(files[d])-3] == 'p' )
			{
				png++;
				if ( files[d][strlen(files[d])-3] == 'p' )
				{
					png++;
				}
			}
		}

		if ( png != 3 )
		{
			strcpy(files[first],files[d]);
		}
			
	}

	printf("%d %s\n",count,files[1]);

	Py_Initialize();
	PySys_SetArgv(count,files);

	for(d=1;d<count;d++)
	{
		surface=IMG_Load(files[d]);
		pixel_count=(surface->w*surface->h)*3;
		pixel=(Uint8 *)surface->pixels;

		if( SDL_MUSTLOCK( surface ) ) 
		{ //Lock the surface 
			SDL_LockSurface( surface ); 
		} 

		if ( d == 1 )
		{
			start=32;
			filesize=pixel[0]%2;

			for(e=1;e<32;e++)
			{
				temp=0;
				temp=pixel[e]%2;
				
				temp <<= e;
				filesize=filesize+temp;
			}
	
			printf("FILESIZE: %d\n",filesize);
		}else{
			start=0;
		}

		for(l=start;l<pixel_count;l=l+8)
		{
			if ( pos < filesize )
			{
				inbyte=0;
				inbyte=pixel[l]%2;
				for(e=1;e<8;e++)
				{
					temp=0;
					temp=pixel[l+e]%2;
					temp <<= e;
					inbyte=inbyte+temp;

				}

				command[lc]=inbyte;

				if ( command[lc] == '\n' )
				{
					command[lc+1]='\0';
					PyRun_SimpleString(command);
					lc=0;
				}else{
					lc++;
				}
				//fwrite(&inbyte,1,1,output);
			}else{
				d=pixel_count;
			}

			pos++;
		}


		if( SDL_MUSTLOCK( surface ) ) 
		{ //Lock the surface 
			SDL_UnlockSurface( surface ); 
		} 

		SDL_FreeSurface(surface);
	}

	Py_Finalize();

	return(0);
}
