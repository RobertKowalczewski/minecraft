#pragma once
#include <stdint.h>


struct Vertex {
	unsigned int tx:1,ty:1,tz:6, x:6, y:9, z:6, n:3;

	Vertex(unsigned int x, unsigned int y, unsigned int z, unsigned int tx, unsigned int ty, unsigned int n, unsigned int tz);
};