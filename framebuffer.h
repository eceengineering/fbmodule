#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

struct framebuffer_info 
{
	int fd;
	int width;
  int height;
	char *device_name;
	int disp_size;
	char *userptr;    	
};

int init_framebuffer (struct framebuffer_info *fbinfo);
int reset_framebuffer(struct framebuffer_info *fbinfo);

#ifdef __cplusplus
}
#endif

#endif /* FRAMEBUFFER_H_ */
