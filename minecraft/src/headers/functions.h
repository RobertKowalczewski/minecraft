#pragma once

int getIndex3D(int x, int y, int z);
int getIndex3D(unsigned int x, unsigned int y, unsigned int z);
int getIndex3DNoise(unsigned int x, unsigned int y, unsigned int z);

//unsigned int vectorHash(glm::ivec2 const& v);

struct pair_hash {
    std::size_t operator () (const glm::ivec2& p) const;
};

inline bool vectorEqual(const glm::ivec2& a, const glm::ivec2& b) {
    return a.x == b.x && a.y == b.y;
}
