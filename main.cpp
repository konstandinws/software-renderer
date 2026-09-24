#include <cmath>
#include <random>
#include <ctime>
#include <tuple>

#include "tgaimage.h"
#include "model.h"

constexpr TGAColor white   = {255, 255, 255, 255}; // BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    bool steep = std::abs(ax-bx) < std::abs(ay-by);
    if (steep) { // if the line is steep, we transpose the image
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax>bx) { // make it left−to−right
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int y = ay;
    int ierror = 0;
    for (int x=ax; x<=bx; x++) {
        if (steep) // if transposed, de−transpose
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
        ierror += 2 * std::abs(by-ay);
        y += (by > ay ? 1 : -1) * (ierror > bx - ax);
        ierror -= 2 * (bx-ax)   * (ierror > bx - ax);
    }
}

TGAColor random_color_V1() {
    return {
        static_cast<unsigned char>(rand()%255),
        static_cast<unsigned char>(rand()%255),
        static_cast<unsigned char>(rand()%255),
        static_cast<unsigned char>(rand()%255),
    };
}

TGAColor random_color_V2(std::uniform_int_distribution<> &random_number_generator, std::mt19937 &gen) {
    return {
        static_cast<unsigned char>(random_number_generator(gen)),
        static_cast<unsigned char>(random_number_generator(gen)),
        static_cast<unsigned char>(random_number_generator(gen)),
        static_cast<unsigned char>(random_number_generator(gen))
    };
}

void draw_model(TGAImage &framebuffer, const Model &model, const int w, const int h) {

    for(int i = 0; i < model.faces.size() ; ++i) {
        int vertice_index_0 = std::get<0>(model.faces[i]) - 1;
        int vertice_index_1 = std::get<1>(model.faces[i]) - 1;
        int vertice_index_2 = std::get<2>(model.faces[i]) - 1;

        // 1 - 2
        line((std::get<0>(model.vertices[vertice_index_0]) + 1.0 ) * w / 2.0,
             (std::get<1>(model.vertices[vertice_index_0]) + 1.0 ) * h / 2.0,
             (std::get<0>(model.vertices[vertice_index_1]) + 1.0 ) * w / 2.0,
             (std::get<1>(model.vertices[vertice_index_1]) + 1.0 ) * h / 2.0,
             framebuffer, red);

        // 2 - 3
        line((std::get<0>(model.vertices[vertice_index_1]) + 1.0 ) * w / 2.0,
             (std::get<1>(model.vertices[vertice_index_1]) + 1.0 ) * h / 2.0,
             (std::get<0>(model.vertices[vertice_index_2]) + 1.0 ) * w / 2.0,
             (std::get<1>(model.vertices[vertice_index_2]) + 1.0 ) * h / 2.0,
             framebuffer, red);
        
        // 3 - 1
        line((std::get<0>(model.vertices[vertice_index_2]) + 1.0 ) * w / 2.0,
             (std::get<1>(model.vertices[vertice_index_2]) + 1.0 ) * h / 2.0,
             (std::get<0>(model.vertices[vertice_index_0]) + 1.0 ) * w / 2.0,
             (std::get<1>(model.vertices[vertice_index_0]) + 1.0 ) * h / 2.0,
             framebuffer, red);
    }
}

int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    Model model("test.txt");

    draw_model(framebuffer, model, width, height);

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}