// #include <cmath>
// #include <random>
// #include <ctime>
// #include <tuple>
// #include <array>
// #include <numbers>

// #include "tgaimage.h"
// #include "model.h"

// constexpr int width  = 600;
// constexpr int height = 600;

// constexpr TGAColor white   = {255, 255, 255, 255}; // BGRA order
// constexpr TGAColor green   = {  0, 255,   0, 255};
// constexpr TGAColor red     = {  0,   0, 255, 255};
// constexpr TGAColor blue    = {255, 128,  64, 255};
// constexpr TGAColor yellow  = {  0, 200, 255, 255};

// void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, const TGAColor &color) {
//     bool steep = std::abs(ax-bx) < std::abs(ay-by);
//     if (steep) { // if the line is steep, we transpose the image
//         std::swap(ax, ay);
//         std::swap(bx, by);
//     }
//     if (ax>bx) { // make it left−to−right
//         std::swap(ax, bx);
//         std::swap(ay, by);
//     }
//     int y = ay;
//     int ierror = 0;
//     for (int x=ax; x<=bx; x++) {
//         if (steep) // if transposed, de−transpose
//             framebuffer.set(y, x, color);
//         else
//             framebuffer.set(x, y, color);
//         ierror += 2 * std::abs(by-ay);
//         y += (by > ay ? 1 : -1) * (ierror > bx - ax);
//         ierror -= 2 * (bx-ax)   * (ierror > bx - ax);
//     }
// }

// std::tuple<int,int,int> project(vec<3> v) { // First of all, (x,y) is an orthogonal projection of the vector (x,y,z).
//     return { (v[0] + 1.) *  width/2,   // Second, since the input models are scaled to have fit in the [-1,1]^3 world coordinates,
//              (v[1] + 1.) * height/2,
//              (v[2] + 1.) *   255./2 }; // we want to shift the vector (x,y) and then scale it to span the entire screen.
// }

// struct Triangle {

//     int ax,ay;
//     int bx,by;
//     int cx,cy;
// };

// void sort(Triangle &t) {
//     if(t.ay > t.by) {
//         std::swap(t.ax, t.bx);
//         std::swap(t.ay, t.by);
//     }
//     if(t.ay > t.cy) {
//         std::swap(t.ax, t.cx);
//         std::swap(t.ay, t.cy);
//     }
//     if(t.by > t.cy) {
//         std::swap(t.bx, t.cx);
//         std::swap(t.by, t.cy);
//     }
// }

// void triangle(Triangle &t, TGAImage &framebuffer, TGAColor color) {
//     sort(t);
//     int height = t.cy - t.ay;

//     if (t.ay != t.by) { // if the bottom half is not degenerate
//         int segment_height = t.by - t.ay;
//         for (int y=t.ay; y<=t.by; y++) { // sweep the horizontal line from ay to by
//             int x1 = t.ax + ((t.cx - t.ax)*(y - t.ay)) / height;
//             int x2 = t.ax + ((t.bx - t.ax)*(y - t.ay)) / segment_height;
//             line(x1, y, x2, y, framebuffer, color);
//         }
//     }

//     if (t.by != t.cy) {
//         int segment_height = t.cy - t.by;
//         for (int y = t.by; y <= t.cy; y++) {
//             int x1 = t.ax + ((t.cx - t.ax)*(y - t.ay)) / height;
//             int x2 = t.bx + ((t.cx - t.bx)*(y - t.by)) / segment_height;
//             line(x1, y, x2, y, framebuffer, color);
//         }
//     }
// }

// double triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
//     return 0.5*((by-ay)*(bx+ax) + (cy-by)*(cx+bx) + (ay-cy)*(ax+cx));
// }

// // void triangle(int ax, int ay, int az, int bx, int by, int bz, int cx, int cy, int cz, TGAImage &zbuffer, TGAImage &framebuffer, TGAColor color) {
// //     int bbminx = std::min(std::min(ax, bx), cx); // bounding box for the triangle
// //     int bbminy = std::min(std::min(ay, by), cy); // defined by its top left and bottom right corners
// //     int bbmaxx = std::max(std::max(ax, bx), cx);
// //     int bbmaxy = std::max(std::max(ay, by), cy);

// //     double total_area = triangle_area(ax, ay, bx, by, cx, cy);
// //     if (total_area<1) return;

// // #pragma omp parallel for
// //     for (int x=bbminx; x<=bbmaxx; x++) {
// //         for (int y=bbminy; y<=bbmaxy; y++) {
// //             double alpha = triangle_area(x, y, bx, by, cx, cy) / total_area;
// //             double beta  = triangle_area(x, y, cx, cy, ax, ay) / total_area;
// //             double gamma = triangle_area(x, y, ax, ay, bx, by) / total_area;
// //             if (alpha<0 || beta<0 || gamma<0) continue;
// //             // float threshold = 0.1;
// //             // if (alpha > threshold && beta > threshold && gamma > threshold) continue;
// //             unsigned char z = static_cast<unsigned char>(alpha * az + beta * bz + gamma * cz);
// //             // TGAColor color{static_cast<uint8_t>(alpha * 255), static_cast<uint8_t>(beta * 255), static_cast<uint8_t>(gamma * 255), 255};
// //             if (z <= zbuffer.get(x, y)[0]) continue;
// //             zbuffer.set(x, y, {z});
// //             framebuffer.set(x, y, color);
// //         }
// //     }
// // }


// // Fonction barycentrique compacte retournant votre struct vec<3>
// constexpr vec<3> barycentric(const vec<3>& a, const vec<3>& b, const vec<3>& c, double px, double py) noexcept {
//     double denom = (b[1] - c[1]) * (a[0] - c[0]) + (c[0] - b[0]) * (a[1] - c[1]);
//     if (std::abs(denom) < 1e-6) return {-1.0, 1.0, 1.0}; // Triangle dégénéré

//     double alpha = ((b[1] - c[1]) * (px - c[0]) + (c[0] - b[0]) * (py - c[1])) / denom;
//     double beta  = ((c[1] - a[1]) * (px - c[0]) + (a[0] - c[0]) * (py - c[1])) / denom;
//     double gamma = 1.0 - alpha - beta;

//     return {alpha, beta, gamma};
// }

// void triangle(const vec<3>& a, const vec<3>& b, const vec<3>& c,
//               std::vector<double>& zbuffer,
//               TGAImage& framebuffer,
//               const TGAColor& color) 
// {
//     int width  = framebuffer.width();
//     int height = framebuffer.height();

//     // Bounding box bornée aux limites réelles de l'écran (anti-crash)
//     int bbminx = std::max(0, static_cast<int>(std::floor(std::min({a[0], b[0], c[0]}))));
//     int bbminy = std::max(0, static_cast<int>(std::floor(std::min({a[1], b[1], c[1]}))));
//     int bbmaxx = std::min(width  - 1, static_cast<int>(std::ceil(std::max({a[0], b[0], c[0]}))));
//     int bbmaxy = std::min(height - 1, static_cast<int>(std::ceil(std::max({a[1], b[1], c[1]}))));

//     if (bbminx > bbmaxx || bbminy > bbmaxy) return;

//     // Parcours mémoire optimal : Y à l'extérieur, X à l'intérieur
//     for (int y = bbminy; y <= bbmaxy; ++y) {
//         for (int x = bbminx; x <= bbmaxx; ++x) {
//             vec<3> bc = barycentric(a, b, c, x + 0.5, y + 0.5);

//             // Pixel en dehors du triangle
//             if (bc[0] < 0.0 || bc[1] < 0.0 || bc[2] < 0.0) continue;

//             // Interpolation du Z en double précision (plus aucun overflow 8-bit !)
//             double z = bc[0] * a[2] + bc[1] * b[2] + bc[2] * c[2];

//             int idx = x + y * width;
//             if (z > zbuffer[idx]) {
//                 zbuffer[idx] = z;
//                 framebuffer.set(x, y, color);
//             }
//         }
//     }
// }

// TGAColor random_color_V1() {
//     return {
//         static_cast<unsigned char>(rand()%255),
//         static_cast<unsigned char>(rand()%255),
//         static_cast<unsigned char>(rand()%255),
//         static_cast<unsigned char>(rand()%255),
//     };
// }

// TGAColor random_color_V2(std::uniform_int_distribution<> &random_number_generator, std::mt19937 &gen) {
//     return {
//         static_cast<unsigned char>(random_number_generator(gen)),
//         static_cast<unsigned char>(random_number_generator(gen)),
//         static_cast<unsigned char>(random_number_generator(gen)),
//         static_cast<unsigned char>(random_number_generator(gen))
//     };
// }

// constexpr double degree2rad(const double angle) {
//     return angle * std::numbers::pi / 180.0;
// }

// vec<3> rot(vec<3> v, double angle) {
//     const double a = degree2rad(angle);
//     mat<3,3> Ry = {{{std::cos(a), 0, std::sin(a)}, {0,1,0}, {-std::sin(a), 0, std::cos(a)}}};
//     return Ry*v;
// }

// vec<3> persp(vec<3> v) {
//     constexpr double c = 3.;
//     return v / (1-v[2]/c);
// }


// // int main(int argc, char** argv) {
// //     if (argc != 2) {
// //         std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
// //         return 1;
// //     }

// //     Model model(argv[1]);
// //     TGAImage framebuffer(width, height, TGAImage::RGB);
// //     TGAImage     zbuffer(width, height, TGAImage::GRAYSCALE);

// //     for (int i=0; i<model.nfaces(); i++) { // iterate through all triangles
// //         auto [ax, ay, az] = project(persp(rot(model.vert(i, 0))));
// //         auto [bx, by, bz] = project(persp(rot(model.vert(i, 1))));
// //         auto [cx, cy, cz] = project(persp(rot(model.vert(i, 2))));
// //         TGAColor rnd;
// //         for (int c=0; c<3; c++) rnd[c] = std::rand()%255;
// //         triangle(ax, ay, az, bx, by, bz, cx, cy, cz, zbuffer, framebuffer, rnd);
// //     }

// //     framebuffer.write_tga_file("framebuffer.tga");
// //     zbuffer.write_tga_file("zbuffer.tga");
// //     return 0;
// // }


// void save_zbuffer_image(const std::vector<double>& zbuffer, int width, int height, const char* filename) {
//     TGAImage img(width, height, TGAImage::GRAYSCALE);

//     double min_z =  std::numeric_limits<double>::infinity();
//     double max_z = -std::numeric_limits<double>::infinity();

//     for (double z : zbuffer) {
//         if (z > -std::numeric_limits<double>::infinity() / 2.0) {
//             min_z = std::min(min_z, z);
//             max_z = std::max(max_z, z);
//         }
//     }

//     for (int y = 0; y < height; ++y) {
//         for (int x = 0; x < width; ++x) {
//             double z = zbuffer[x + y * width];
//             if (z > -std::numeric_limits<double>::infinity() / 2.0 && max_z > min_z) {
//                 auto val = static_cast<std::uint8_t>(255.0 * (z - min_z) / (max_z - min_z));
//                 img.set(x, y, TGAColor{val});
//             } else {
//                 img.set(x, y, TGAColor{0}); // Fond noir
//             }
//         }
//     }

//     img.write_tga_file(filename);
// }

// int main(int argc, char** argv) {
//     if (argc != 2) {
//         std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
//         return 1;
//     }

//     Model model(argv[1]);
//     constexpr int width  = 800;
//     constexpr int height = 800;

//     TGAImage framebuffer(width, height, TGAImage::RGB);

//     // Initialisation du Z-buffer avec -infini
//     std::vector<double> zbuffer(width * height, -std::numeric_limits<double>::infinity());

//     // 1. Matrice de rotation Y (45 degrés par exemple)

//     // 2. Boucle sur les triangles du modèle
//     for (int i = 0; i < model.nfaces(); ++i) {
//         vec<3> screen_pts[3];

//         for (int j = 0; j < 3; ++j) {
//             vec<3> world_pt = model.vert(i, j); // Sommet 3D du modèle dans [-1, 1]^3

//             // A. Rotation
//             vec<3> rotated = rot(world_pt, 30);

//             // B. Perspective (votre fonction)
//             vec<3> projected = persp(rotated);

//             // C. Transformation vers l'espace écran (Viewport)
//             screen_pts[j] = {
//                 (projected[0] + 1.0) * 0.5 * (width  - 1),
//                 (projected[1] + 1.0) * 0.5 * (height - 1),
//                 projected[2] // Profondeur flottante conservée intacte
//             };
//         }

//         // D. Rasterization avec zbuffer flottant
//         TGAColor color = TGAColor{static_cast<unsigned char>(rand() % 255), static_cast<unsigned char>(rand() % 255), static_cast<unsigned char>(rand() % 255), static_cast<unsigned char>(255)};
//         triangle(screen_pts[0], screen_pts[1], screen_pts[2], zbuffer, framebuffer, color);
//     }

//     framebuffer.write_tga_file("framebuffer.tga");

//     // Optionnel : Sauvegarder une image du Z-buffer normalisé pour le débogage
//     save_zbuffer_image(zbuffer, width, height, "zbuffer.tga");

//     return 0;
// }

#include <limits>
#include <algorithm>
#include "geometry.h"
#include "model.h"
#include "our_gl.h"

extern mat<4,4> ModelView, Perspective; // "OpenGL" state matrices and
extern std::vector<double> zbuffer;     // the depth buffer

struct RandomShader : IShader {
    const Model &model;
    TGAColor color = {};
    vec3 tri[3];  // triangle in eye coordinates

    RandomShader(const Model &m) : model(m) {
    }

    virtual vec4 vertex(const int face, const int vert) {
        vec3 v = model.vert(face, vert);                          // current vertex in object coordinates
        vec4 gl_Position = ModelView * vec4{v.x, v.y, v.z, 1.};
        tri[vert] = gl_Position.xyz();                            // in eye coordinates
        return Perspective * gl_Position;                         // in clip coordinates
    }

    virtual std::pair<bool,TGAColor> fragment(const vec3 bar) const {
        return {false, color};                                    // do not discard the pixel
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
        return 1;
    }

    constexpr int width  = 800;      // output image size
    constexpr int height = 800;
    constexpr vec3    eye{ 0, 0, 2}; // camera position
    constexpr vec3 center{ 0, 0, 0}; // camera direction
    constexpr vec3     up{ 0, 1, 0}; // camera up vector

    lookat(eye, center, up);                                   // build the ModelView   matrix
    init_perspective(norm(eye-center));                        // build the Perspective matrix
    init_viewport(width/16, height/16, width*7/8, height*7/8); // build the Viewport    matrix
    init_zbuffer(width, height);
    TGAImage framebuffer(width, height, TGAImage::RGB, {0, 0, 0, 255});

    for (int m=1; m<argc; m++) {                    // iterate through all input objects
        Model model(argv[m]);                       // load the data
        RandomShader shader(model);
        for (int f=0; f<model.nfaces(); f++) {      // iterate through all facets
            shader.color = { static_cast<unsigned char>(std::rand()%255), static_cast<unsigned char>(std::rand()%255), static_cast<unsigned char>(std::rand()%255), 255 };
            Triangle clip = { shader.vertex(f, 0),  // assemble the primitive
                              shader.vertex(f, 1),
                              shader.vertex(f, 2) };
            rasterize(clip, shader, framebuffer);   // rasterize the primitive
        }
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
