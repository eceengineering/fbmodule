#ifndef FB_INTERFACE_H_
#define FB_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

struct fbmodule_arguments 
{
	int screenwidth;
  int screenheight;
	int camwidth;
	int camheight;
	char *device_name;
};

/* These functions return ERROR value as an integer */
int fbmodule_init 	(struct fbmodule_arguments *arg);
int fbmodule_close	(void);
int fbmodule_render	(char *data);

#ifdef __cplusplus
}
#endif

#endif /* FB_INTERFACE_H_ */
