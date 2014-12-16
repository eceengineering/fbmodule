/* ============================================================================
 * @File: 	 framebuffer.c
 * @Author: 	 Ozgur Eralp [ozgur.eralp@outlook.com]
 * @Description: Frame Buffer Kernel Driver Interface.
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
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <asm/errno.h>
#include "framebuffer.h"

/* ============================================================================
 * @Function: init_framebuffer
 * @Description: Initialize kernel frame buffer driver for data displaying.
 * Kernel bufferers are memory mapped so this buffer can be passed to
 * DSP directly since kernel buffers are contiguous.
 * ============================================================================
 */
int init_framebuffer(struct framebuffer_info *fbinfo)
{
	struct fb_var_screeninfo varInfo;
  struct fb_fix_screeninfo fixInfo;
  int width = fbinfo->width;
  int height = fbinfo->height;
  int err;

  /* Open video display device */
  fbinfo->fd = open(fbinfo->device_name, O_RDWR);
  if (fbinfo->fd == -1) {
        	printf("$$ Unable to open fb device %s\n", fbinfo->device_name);
        	return -ENODEV;
  }

  if (ioctl(fbinfo->fd, FBIOGET_FSCREENINFO, &fixInfo) == -1) {
       	 	printf("$$ Unable to get FBIOGET_FSCREENINFO\n");
       	 	err = EINVAL;
        	goto cleanup_devnode;
  }

  if (ioctl(fbinfo->fd, FBIOGET_VSCREENINFO, &varInfo) == -1) {
        	printf("$$ Unable to get FBIOGET_VSCREENINFO\n");
        	err = EINVAL;
        	goto cleanup_devnode;
  }

	varInfo.xres 	= width;
	varInfo.yres	= height;
	varInfo.xres_virtual = width;
	varInfo.yres_virtual = height;
	varInfo.activate |= FB_ACTIVATE_NOW | FB_ACTIVATE_FORCE;

	/* Set video display format */
	if (ioctl(fbinfo->fd, FBIOPUT_VSCREENINFO, &varInfo) == -1) {
        	printf("$$ Unable to set activate VSCREENINFO\n");
  }

  /* Determine the size of the display buffers inside the device driver */
  //fbinfo->disp_size = fixInfo.line_length * varInfo.yres;
	fbinfo->disp_size = varInfo.yres * varInfo.yres * varInfo.bits_per_pixel / 8;
	printf(".Display Size %d - %d %d %d\n", fbinfo->disp_size,
					varInfo.xres,
					varInfo.yres,
					varInfo.bits_per_pixel);

  fbinfo->userptr = (char *) mmap (NULL,
                		fbinfo->disp_size,
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED,
                    fbinfo->fd, 0);

  if (fbinfo->userptr == MAP_FAILED) {
        	printf("$$ Failed mmap framebuffer memory\n");
        	err = ENOMEM;
        	goto cleanup_devnode;
  }

  return 0;

cleanup_devnode:
	close(fbinfo->fd);
	return -err;
}

/* ============================================================================
 * @Function: reset_framebuffer
 * @Description: Reset the frame buffer display format.
 * ============================================================================
 */
int reset_framebuffer(struct framebuffer_info *fbinfo)
{
	//struct fb_var_screeninfo varInfo;

	munmap(fbinfo->userptr, fbinfo->disp_size);
	close(fbinfo->fd);

	return 0;
}
