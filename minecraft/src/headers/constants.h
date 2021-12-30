#pragma once

namespace Constants {
	const unsigned int CHUNK_SIZE_X = 32u;
	const unsigned int CHUNK_SIZE_Y = 128u;
	const unsigned int CHUNK_SIZE_Z = 32u;
	const unsigned int BLOCK_COUNT = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;
	const unsigned int CHUNK_XY = Constants::CHUNK_SIZE_X * Constants::CHUNK_SIZE_Y;
	const unsigned int RENDERDISTANCE = 40u;
	
	const unsigned int NOISE_ARRAY_SIZE = (Constants::CHUNK_SIZE_X + 2) * (Constants::CHUNK_SIZE_Z + 2);
}
namespace Debug {
	const bool DEBUG = false;
}
enum class BlockType : unsigned char{
	AIR,
	DIRT,
	WATER,
	STONE
};