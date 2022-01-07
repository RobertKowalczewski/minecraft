#include <glm/glm.hpp>
#include <functional>

#include "functions.h"
#include"constants.h"

int getIndex3D(int x, int y, int z){
 return x + y * Constants::CHUNK_SIZE_X + z * Constants::CHUNK_XY;
}
int getIndex3D(unsigned int x, unsigned int y, unsigned int z) {
	return x + y * Constants::CHUNK_SIZE_X + z * Constants::CHUNK_XY;
}
int getIndex3DNoise(unsigned int x, unsigned int y, unsigned int z) {
    return x + y * (Constants::CHUNK_SIZE_X + 2) + z * (Constants::CHUNK_XY + 2 * Constants::CHUNK_SIZE_Y);
}