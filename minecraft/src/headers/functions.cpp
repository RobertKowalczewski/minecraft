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
    return x + y * (Constants::CHUNK_SIZE_X+2) + z * (Constants::CHUNK_XY + 2 * Constants::CHUNK_SIZE_Y);
}
//unsigned int vectorHash(glm::ivec2 const& v)
//{
//	return std::hash<int>{}(v.x) ^ (std::hash<int>{}(v.y) << 1);
//}

inline std::size_t pair_hash::operator()(const glm::ivec2& p) const
{
    auto h1 = std::hash<int>{}(p.x);
    auto h2 = std::hash<int>{}(p.y);

    // Mainly for demonstration purposes, i.e. works but is overly simple
    // In the real world, use sth. like boost.hash_combine
    return h1 ^ (h2<<1);
}