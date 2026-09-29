#include <cmath>
#include <random>
#include <ctime>
#include <tuple>
#include <array>

#include "tgaimage.h"
#include "model.h"

constexpr int width  = 600;
constexpr int height = 600;

constexpr TGAColor white   = {255, 255, 255, 255}; // BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, const TGAColor &color) {
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

std::tuple<int,int,int> project(vec3 v) { // First of all, (x,y) is an orthogonal projection of the vector (x,y,z).
    return { (v.x + 1.) *  width/2,   // Second, since the input models are scaled to have fit in the [-1,1]^3 world coordinates,
             (v.y + 1.) * height/2,
             (v.z + 1.) *   255./2 }; // we want to shift the vector (x,y) and then scale it to span the entire screen.
}

struct Triangle {

    int ax,ay;
    int bx,by;
    int cx,cy;
};

void sort(Triangle &t) {
    if(t.ay > t.by) {
        std::swap(t.ax, t.bx);
        std::swap(t.ay, t.by);
    }
    if(t.ay > t.cy) {
        std::swap(t.ax, t.cx);
        std::swap(t.ay, t.cy);
    }
    if(t.by > t.cy) {
        std::swap(t.bx, t.cx);
        std::swap(t.by, t.cy);
    }
}

void triangle(Triangle &t, TGAImage &framebuffer, TGAColor color) {
    sort(t);
    int height = t.cy - t.ay;

    if (t.ay != t.by) { // if the bottom half is not degenerate
        int segment_height = t.by - t.ay;
        for (int y=t.ay; y<=t.by; y++) { // sweep the horizontal line from ay to by
            int x1 = t.ax + ((t.cx - t.ax)*(y - t.ay)) / height;
            int x2 = t.ax + ((t.bx - t.ax)*(y - t.ay)) / segment_height;
            line(x1, y, x2, y, framebuffer, color);
        }
    }

    if (t.by != t.cy) {
        int segment_height = t.cy - t.by;
        for (int y = t.by; y <= t.cy; y++) {
            int x1 = t.ax + ((t.cx - t.ax)*(y - t.ay)) / height;
            int x2 = t.bx + ((t.cx - t.bx)*(y - t.by)) / segment_height;
            line(x1, y, x2, y, framebuffer, color);
        }
    }
}

double triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return 0.5*((by-ay)*(bx+ax) + (cy-by)*(cx+bx) + (ay-cy)*(ax+cx));
}

void triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &zbuffer, TGAImage &framebuffer, TGAColor color) {
    int bbminx = std::min(std::min(ax, bx), cx); // bounding box for the triangle
    int bbminy = std::min(std::min(ay, by), cy); // defined by its top left and bottom right corners
    int bbmaxx = std::max(std::max(ax, bx), cx);
    int bbmaxy = std::max(std::max(ay, by), cy);

    double total_area = triangle_area(ax, ay, bx, by, cx, cy);
    if (total_area<1) return;

#pragma omp parallel for
    for (int x=bbminx; x<=bbmaxx; x++) {
        for (int y=bbminy; y<=bbmaxy; y++) {
            double alpha = triangle_area(x, y, bx, by, cx, cy) / total_area;
            double beta  = triangle_area(x, y, cx, cy, ax, ay) / total_area;
            double gamma = triangle_area(x, y, ax, ay, bx, by) / total_area;
            if (alpha<0 || beta<0 || gamma<0) continue;
            // float threshold = 0.1;
            // if (alpha > threshold && beta > threshold && gamma > threshold) continue;
            unsigned char z = static_cast<unsigned char>(alpha * az + beta * bz + gamma * cz);
            // TGAColor color{static_cast<uint8_t>(alpha * 255), static_cast<uint8_t>(beta * 255), static_cast<uint8_t>(gamma * 255), 255};
            if (z <= zbuffer.get(x, y)[0]) continue;
            zbuffer.set(x, y, {z});
            framebuffer.set(x, y, color);
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
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
        return 1;
    }

    Model model(argv[1]);
    TGAImage framebuffer(width, height, TGAImage::RGB);
    TGAImage     zbuffer(width, height, TGAImage::GRAYSCALE);

    for (int i=0; i<model.nfaces(); i++) { // iterate through all triangles
        auto [ax, ay, az] = project(model.vert(i, 0));
        auto [bx, by, bz] = project(model.vert(i, 1));
        auto [cx, cy, cz] = project(model.vert(i, 2));
        TGAColor rnd;
        for (int c=0; c<3; c++) rnd[c] = std::rand()%255;
        triangle(ax, ay, az, bx, by, bz, cx, cy, cz, zbuffer, framebuffer, rnd);
    }

    framebuffer.write_tga_file("framebuffer.tga");
    zbuffer.write_tga_file("zbuffer.tga");
    return 0;
}