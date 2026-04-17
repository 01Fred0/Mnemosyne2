#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <nlohmann/json.hpp>
#include "Room.h"
#include <vector>
#include <memory>
#include <string>

class Serialization {
public:
    static void SavePalace(const std::string& filename, const std::vector<std::unique_ptr<Room>>& rooms);
    static std::vector<std::unique_ptr<Room>> LoadPalace(const std::string& filename);
};

#endif
