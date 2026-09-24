#include <vector>
#include <tuple>
#include <string>
#include <fstream>
#include <iostream>

class Model {
    
public:
    std::vector<std::tuple<float, float, float>> vertices;
    // the index of the vertices that form a face
    std::vector<std::tuple<int, int, int>> faces;
    
    Model(std::string filename);

    void draw();

    int number_vertices = vertices.size();
    int number_faces = faces.size();

    void show_vertices();
    void show_faces();
};