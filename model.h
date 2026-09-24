#include <vector>
#include <tuple>
#include <string>
#include <fstream>
#include <iostream>

class Model {
    std::vector<std::tuple<float, float, float>> vertices;
    // the index of the vertices that form a face
    std::vector<std::tuple<int, int, int>> faces;

public:

    Model(std::string filename);

    void show_vertices();
    void show_faces();
};