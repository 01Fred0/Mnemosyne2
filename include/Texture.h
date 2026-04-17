#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
public:
    unsigned int ID;
    std::string type;
    std::string path;

    Texture(const char* path, const std::string& directory);
    void Bind(unsigned int slot = 0) const;
};

#endif
