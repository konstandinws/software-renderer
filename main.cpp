#include "tgaimage.h"
#include <cmath>
#include <random>
#include <ctime>

constexpr TGAColor white   = {255, 255, 255, 255}; // BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, const TGAColor& color = white) {
    bool steep = std::abs(ay - by) > std::abs(ax - bx);
    if(steep) {
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax > bx) {
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    int y = ay;
    int ierror = 0;
    for(int x = ax; x <= bx; ++x) {
        float t = (x - ax) / static_cast<float>(bx - ax);
        int y = std::round(ay + t * (by - ay));
        if(steep) 
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
        ierror += 2 * std::abs(by-ay);
        if(ierror > bx - ax) {
            y += by > ay ? 1 : -1;
            ierror -= 2 * (bx-ax);
        }
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


int main(int argc, char** argv) {
    constexpr int width  = 64;
    constexpr int height = 64;
    TGAImage framebuffer(width, height, TGAImage::RGB);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> height_distribution(0, height);
    std::uniform_int_distribution<> width_distribution(0, width);
    std::uniform_int_distribution<> random_color_number(0, 255);

    std::srand(std::time({}));

    for(int i = 0; i <(1<<24); ++i) {
        int ax = width_distribution(gen); int ay = height_distribution(gen);
        int bx = width_distribution(gen); int by = height_distribution(gen);

        line(ax, ay, bx, by, framebuffer, random_color_V2(random_color_number, gen));
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}