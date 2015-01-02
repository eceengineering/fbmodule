fbmodule
========

VIDEO FRAME RENDERER

Description:

Purpose of the video frame renderer module is to provide a lightweight frame buffer rendering functionality which consumes CPU at the lowest level. This module has been designed for embedded Linux devices. The module runs at the user space.

Prerequirements:

The module requires a pointer accessing frame buffer device so frame buffer needs to be enabled in the Kernel. 

Details:

This module provides the quickest way to render a frame on the screen by accessing the device directly to write pixel data.

Input: See fbmodule.h for the module interface details.

- Screen resolution
- Camera resolution
- Framebuffer device name

Output:

- N/A

Sequence: See the test application available in main.c file.
  
- Init() 		- To initialize the module with device name and resolution info
- Close() 	- To close the device
- Render()	- To render video frame on the screen

How to Build and Run: Makefile available in the repository is generated regarding to Arm Linux Gnueabihf Gcc.
	
    Programming Language:   C
    Compiler:               GCC
    Test Platform(s):       ARM Cortex-A8 h/w
                            ARM Cortex-A9 h/w
    Test OS:                Linux Ubuntu
                            Linux Debian	
	
	Build: 	$make
	Run: 	$./Test.sh
	
