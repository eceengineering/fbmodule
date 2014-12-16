/* ============================================================================
 * @File: 	 fbmodule.c
 * @Author: 	 Ozgur Eralp [ozgur.eralp@outlook.com]
 * @Description: Frame Buffer Interface required to render video preview.
 *
 * ============================================================================
 *
 * Copyright 2014 Ozgur Eralp.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/fb.h>
#include <asm/errno.h>
#include "framebuffer.h"
#include "fbmodule.h"

struct framebuffer_info fbinfo;
int camerawidth;
int cameraheight;
int screenwidth;
int screenheight;

/* ============================================================================
 * @Function: fbmodule_init
 * @Description: Initialize framebuffer interface.
 * ============================================================================
 */
int fbmodule_init 	(struct fbmodule_arguments *arg)
{
  /* Initialize Framebuffer */
  fbinfo.width = arg->screenwidth;
  fbinfo.height = arg->screenheight;
	fbinfo.device_name = (char *)"/dev/fb0";
    	fbinfo.fd = -1;

	camerawidth = arg->camwidth;
	cameraheight= arg->camheight;
	screenwidth = arg->screenwidth;
	screenheight= arg->screenheight;	
    	
	if (init_framebuffer(&fbinfo) == 0)
	{
        printf("..framebuffer init'ed successfully \n");
    	 	return 0;
	}else{
      printf("$$ Framebuffer initialization error!\n");
      return 1;
  }
}

/* ============================================================================
 * @Function: 	 fbmodule_close
 * @Description: Initialize framebuffer interface.
 * ============================================================================
 */
int fbmodule_close	(void)
{
	printf("..framebuffer interface will be closed! \n");
	reset_framebuffer(&fbinfo);

	return 0;
}

/* Macro for clearing structures */
#define CLEAR(x) memset (&(x), 0, sizeof (x))

/* ============================================================================
 * @Description: The following three macros (CLAMP, YCBCR_TO_RGB and
 * PIXEL_RGB16) copied from Direct FB source codes.
 * ============================================================================
 */
#define CLAMP(x,min,max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))
#define YCBCR_TO_RGB( y, cb, cr, r, g, b )                            \
do {                                                                  \
     int _y  = (y)  -  16;                                            \
     int _cb = (cb) - 128;                                            \
     int _cr = (cr) - 128;                                            \
                                                                      \
     int _r = (298 * _y             + 409 * _cr + 128) >> 8;          \
     int _g = (298 * _y - 100 * _cb - 208 * _cr + 128) >> 8;          \
     int _b = (298 * _y + 516 * _cb             + 128) >> 8;          \
                                                                      \
     (r) = CLAMP( _r, 0, 255 );                                       \
     (g) = CLAMP( _g, 0, 255 );                                       \
     (b) = CLAMP( _b, 0, 255 );                                       \
} while (0)
#define PIXEL_RGB16(r,g,b)     ( (((r)&0xF8) << 8) | \
                                 (((g)&0xFC) << 3) | \
                                 (((b)&0xF8) >> 3) )

/* ============================================================================
 * @Function: 	 fbmodule_render
 * @Description: Render data on the screen via framebuffer interface.
 * ============================================================================
 */
int fbmodule_render	(char *data)
{
	char *dstPlane = (char *)fbinfo.userptr;
	
	int height = cameraheight;
	int width = camerawidth;

	int U, Y1, Y2, V, R, G, B;

  	for (int i = 0; i < (height); i++)
	{
		/*Arrange the index for data buffer and frame buffer*/
		int index_source = i*width*4;
		int index_destination = i*screenwidth*2;

		for (int j = 0; j < width; j++)
		{	
			/*UY1VY2*/
			U  = data[index_source + 4*j];
			Y1 = data[index_source + 4*j+1];
			V  = data[index_source + 4*j+2];
			Y2 = data[index_source + 4*j+3];

			/*Convert to RGB and render on the framebuffer*/
			YCBCR_TO_RGB(Y1, U, V, R, G, B);
			dstPlane[index_destination + 2*j] = PIXEL_RGB16(R, G, B);			
			
			YCBCR_TO_RGB(Y2, U, V, R, G, B);
			dstPlane[index_destination + 2*j + 1] = PIXEL_RGB16(R, G, B);
		}
	}

	return 0;
}
