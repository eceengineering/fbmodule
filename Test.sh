#!/bin/bash -e
echo "-------------------------------------------------------------------"
echo " THIS IS A TEST APPLICATION IMPLEMENTED FOR CAMMODULE AND FBMODULE " 
echo " ASSUMPTIONS:"
echo "	- Camera able to capture YUV (UYVY) format,"
echo "	- Frame Buffer Module renders YUV data captured by camera driver."
echo "-------------------------------------------------------------------"

sudo ./armTestApp
