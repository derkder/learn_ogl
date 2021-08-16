#!/bin/bash

CC=g++
CPPFLAGS=`pkg-config --cflags glew ImageMagick++`
CPPFLAGS="$CPPFLAGS -I../Include"
LDFLAGS=`pkg-config --libs glew ImageMagick++ assimp`
LDFLAGS="$LDFLAGS -lglut -lX11"

$CC tutorial18.cpp ../Common/ogldev_util.cpp  ../Common/math_3d.cpp ../Common/ogldev_texture.cpp ../Common/3rdparty/stb_image.cpp ../Common/ogldev_world_transform.cpp camera.cpp ../Common/ogldev_basic_mesh.cpp $CPPFLAGS $LDFLAGS -o tutorial18
