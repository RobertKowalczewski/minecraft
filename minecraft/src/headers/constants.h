#pragma once

namespace Constants {
	const unsigned int CHUNK_SIZE_X = 32u;
	const unsigned int CHUNK_SIZE_Y = 128u;
	const unsigned int CHUNK_SIZE_Z = 32u;
	const unsigned int BLOCK_COUNT = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z;
	const unsigned int CHUNK_XY = Constants::CHUNK_SIZE_X * Constants::CHUNK_SIZE_Y;
	const unsigned int RENDERDISTANCE = 40u;
}
namespace Debug {
	const bool DEBUG = false;
}