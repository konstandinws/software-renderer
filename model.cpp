#include "model.h"
#include <array>

Model::Model(std::string filename) {
    std::ifstream file(filename.c_str(), std::ios_base::in);
    if(file) {
        std::string text;
        while(file >> text) {
            if (text == "v") {
                std::array<float, 3> pos;
                std::string next;
                for(int i = 0; i < 3; ++i) {
                    file >> next;
                    pos[i] = std::stof(next);
                }
                std::tuple<float, float, float> new_vertice = { pos[0], pos[1], pos[2] };
                vertices.push_back(new_vertice);
            } else if (text == "f") {
                std::array<int, 3> indexes;
                std::string next;
                for(int i = 0; i < 3; ++i) {
                    file >> next;
                    indexes[i] = next[0] - '0';
                }
                std::tuple<float, float, float> new_face = { indexes[0], indexes[1], indexes[2] };
                faces.push_back(new_face);
            }
        }
        file.close();
    } else {
        std::cerr << "Erreur de l'ouverture du fichier" << std::endl; 
    }
}

void Model::show_vertices() {
    for(auto el : vertices) {
        std::cout << "( " << std::get<0>(el) << ", " << std::get<1>(el) << ", " << std::get<2>(el) << ")" << std::endl;
    }
}

void Model::show_faces() {
    for(auto el : faces) {
        std::cout << "( " << std::get<0>(el) << ", " << std::get<1>(el) << ", " << std::get<2>(el) << ")" << std::endl;
    }
}