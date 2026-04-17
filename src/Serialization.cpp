#include "Serialization.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void Serialization::SavePalace(const std::string& filename, const std::vector<std::unique_ptr<Room>>& rooms) {
    json j;
    j["rooms"] = json::array();

    for (const auto& room : rooms) {
        json r;
        r["position"] = {room->Position.x, room->Position.y, room->Position.z};
        r["size"] = {room->Size.x, room->Size.y, room->Size.z};

        if (dynamic_cast<SquareRoom*>(room.get())) r["type"] = "square";
        else if (dynamic_cast<RoundRoom*>(room.get())) r["type"] = "round";
        else if (dynamic_cast<HexRoom*>(room.get())) r["type"] = "hex";

        r["objects"] = json::array();
        for (const auto& obj : room->Objects) {
            json o;
            o["name"] = obj->Name;
            o["position"] = {obj->Position.x, obj->Position.y, obj->Position.z};
            o["scale"] = {obj->Scale.x, obj->Scale.y, obj->Scale.z};
            o["type"] = "file";
            r["objects"].push_back(o);
        }

        j["rooms"].push_back(r);
    }

    std::ofstream file(filename);
    file << j.dump(4);
}

std::vector<std::unique_ptr<Room>> Serialization::LoadPalace(const std::string& filename) {
    std::vector<std::unique_ptr<Room>> rooms;
    std::ifstream file(filename);
    if (!file.is_open()) return rooms;

    json j;
    try {
        file >> j;
        if (!j.contains("rooms")) return rooms;

        for (const auto& r : j["rooms"]) {
            if (!r.contains("position") || !r.contains("size") || !r.contains("type")) continue;

            glm::vec3 pos(r["position"][0], r["position"][1], r["position"][2]);
            glm::vec3 size(r["size"][0], r["size"][1], r["size"][2]);
            std::string type = r["type"];

            std::unique_ptr<Room> room;
            if (type == "square") room = std::make_unique<SquareRoom>(pos, size);
            else if (type == "round") room = std::make_unique<RoundRoom>(pos, size);
            else if (type == "hex") room = std::make_unique<HexRoom>(pos, size);

            if (room && r.contains("objects")) {
                for (const auto& o : r["objects"]) {
                    if (!o.contains("position") || !o.contains("name")) continue;

                    glm::vec3 opos(o["position"][0], o["position"][1], o["position"][2]);
                    auto obj = std::make_unique<FileObject>(o["name"], opos);

                    if (o.contains("scale")) {
                        obj->Scale = glm::vec3(o["scale"][0], o["scale"][1], o["scale"][2]);
                    }

                    room->AddObject(std::move(obj));
                }
                rooms.push_back(std::move(room));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading palace: " << e.what() << std::endl;
    }

    return rooms;
}
