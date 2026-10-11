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
#include <cstdint>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

mat<4,4> ModelView, Viewport, Perspective;

void lookat(const vec3 eye, const vec3 center, const vec3 up) {
    vec3 n = normalized(eye-center);
    vec3 l = normalized(cross(up,n));
    vec3 m = normalized(cross(n, l));
    ModelView = mat<4,4>{{{l.x,l.y,l.z,0}, {m.x,m.y,m.z,0}, {n.x,n.y,n.z,0}, {0,0,0,1}}} *
                mat<4,4>{{{1,0,0,-center.x}, {0,1,0,-center.y}, {0,0,1,-center.z}, {0,0,0,1}}};
}

void perspective(const double f) {
    Perspective = {{{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0, -1/f,1}}};
}

void viewport(const int x, const int y, const int w, const int h) {
    Viewport = {{{w/2., 0, 0, x+w/2.}, {0, h/2., 0, y+h/2.}, {0,0,1,0}, {0,0,0,1}}};
}

void rasterize(const vec4 clip[3], std::vector<double> &zbuffer, TGAImage &framebuffer, const TGAColor color) {
    vec4 ndc[3]    = { clip[0]/clip[0].w, clip[1]/clip[1].w, clip[2]/clip[2].w };                // normalized device coordinates
    vec2 screen[3] = { (Viewport*ndc[0]).xy(), (Viewport*ndc[1]).xy(), (Viewport*ndc[2]).xy() }; // screen coordinates

    mat<3,3> ABC = {{ {screen[0].x, screen[0].y, 1.}, {screen[1].x, screen[1].y, 1.}, {screen[2].x, screen[2].y, 1.} }};
    if (ABC.det()<1) return; // backface culling + discarding triangles that cover less than a pixel
    const mat<3,3> ABC_it = ABC.invert_transpose(); // the same for every pixel of the triangle: compute it once

    auto [bbminx,bbmaxx] = std::minmax({screen[0].x, screen[1].x, screen[2].x}); // bounding box for the triangle
    auto [bbminy,bbmaxy] = std::minmax({screen[0].y, screen[1].y, screen[2].y}); // defined by its top left and bottom right corners
#pragma omp parallel for
    for (int x=std::max<int>(bbminx, 0); x<=std::min<int>(bbmaxx, framebuffer.width()-1); x++) { // clip the bounding box by the screen
        for (int y=std::max<int>(bbminy, 0); y<=std::min<int>(bbmaxy, framebuffer.height()-1); y++) {
            vec3 bc = ABC_it * vec3{static_cast<double>(x), static_cast<double>(y), 1.}; // barycentric coordinates of {x,y} w.r.t the triangle
            if (bc.x<0 || bc.y<0 || bc.z<0) continue;                                                    // negative barycentric coordinate => the pixel is outside the triangle
            double z = bc * vec3{ ndc[0].z, ndc[1].z, ndc[2].z };
            if (z <= zbuffer[x+y*framebuffer.width()]) continue;
            zbuffer[x+y*framebuffer.width()] = z;
            framebuffer.set(x, y, color);
        }
    }
}

// Coupe le triangle par le plan proche (w = near) avant de le rasteriser. w vaut profondeur/focal devant la caméra :
// un sommet avec w <= 0 est derrière elle, et la division par w donnerait un triangle retourné ou immense.
// Un triangle coupé par un plan donne un polygone de 3 ou 4 sommets, qu'on redécoupe en 1 ou 2 triangles.
void clip_and_rasterize(const vec4 clip[3], std::vector<double> &zbuffer, TGAImage &framebuffer, const TGAColor color) {
    constexpr double near = 0.05;
    vec4 poly[4];
    int n = 0;
    for (int i=0; i<3; i++) {
        const vec4 &a = clip[i], &b = clip[(i+1)%3];
        if (a.w >= near) poly[n++] = a;           // a est devant le plan : on le garde
        if ((a.w >= near) != (b.w >= near)) {     // l'arête [a,b] traverse le plan : on ajoute le point d'intersection
            double t = (near - a.w) / (b.w - a.w);
            poly[n++] = a + (b - a)*t;
        }
    }
    for (int i=1; i+1<n; i++) {                   // découpage en éventail, l'ordre des sommets (et donc le backface culling) est conservé
        const vec4 tri[3] = { poly[0], poly[i], poly[i+1] };
        rasterize(tri, zbuffer, framebuffer, color);
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
        return 1;
    }

    constexpr int width  = 800;      // window size
    constexpr int height = 800;
    constexpr vec3 up{0,1,0};        // camera up vector
    constexpr double focal = 1.732;  // distance eye–center : règle le champ de vision, 2*atan(1/focal) ≈ 60°

    std::vector<Model> models;       // chargés une seule fois
    for (int m=1; m<argc; m++) models.emplace_back(argv[m]);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("tinyRenderer", width, height, 0, &window, &renderer)) {
        std::cerr << "SDL_CreateWindowAndRenderer: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_SetRenderVSync(renderer, 1);
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
    std::vector<std::uint32_t> pixels(width*height); // copie du framebuffer au format de la texture

    vec3 eye{0,0,3};                 // position de la caméra dans le monde
    double yaw = 0, pitch = 0;       // direction du regard : yaw = gauche/droite, pitch = haut/bas (0,0 = vers -z)
    std::uint64_t last = SDL_GetTicks(), fps_timer = last;
    int fps_frames = 0;
    bool running = true;

    while (running) {
        // 1. Entrées
        //    souris (clic gauche maintenu) ou flèches = regarder
        //    ZQSD (WASD en QWERTY) = avancer/gauche/reculer/droite, Espace = monter, Ctrl = descendre, Shift = plus vite
        //    Échap = quitter
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = false;
            if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_ESCAPE) running = false;
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN && e.button.button == SDL_BUTTON_LEFT) SDL_SetWindowRelativeMouseMode(window, true);  // cache et bloque le curseur
            if (e.type == SDL_EVENT_MOUSE_BUTTON_UP   && e.button.button == SDL_BUTTON_LEFT) SDL_SetWindowRelativeMouseMode(window, false);
            if (e.type == SDL_EVENT_MOUSE_MOTION && (e.motion.state & SDL_BUTTON_LMASK)) {
                yaw   += e.motion.xrel * 0.003;
                pitch -= e.motion.yrel * 0.003;
            }
        }

        std::uint64_t now = SDL_GetTicks();
        double dt = (now - last) / 1000.; // secondes écoulées depuis l'image précédente
        last = now;
        const bool* keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_LEFT])  yaw   -= 1.5*dt;
        if (keys[SDL_SCANCODE_RIGHT]) yaw   += 1.5*dt;
        if (keys[SDL_SCANCODE_UP])    pitch += 1.5*dt;
        if (keys[SDL_SCANCODE_DOWN])  pitch -= 1.5*dt;
        pitch = std::clamp(pitch, -1.5, 1.5); // jamais à la verticale : up serait parallèle à la direction du regard

        // Les scancodes désignent la position physique de la touche : SDL_SCANCODE_W est la touche Z d'un clavier AZERTY
        vec3 forward = {std::cos(pitch)*std::sin(yaw), std::sin(pitch), -std::cos(pitch)*std::cos(yaw)}; // direction du regard
        vec3 right   = {std::cos(yaw), 0, std::sin(yaw)};                                                // vers la droite, à l'horizontale
        double step  = (keys[SDL_SCANCODE_LSHIFT] ? 6. : 2.) * dt;                                        // vitesse en unités par seconde
        if (keys[SDL_SCANCODE_W])     eye = eye + forward*step;
        if (keys[SDL_SCANCODE_S])     eye = eye - forward*step;
        if (keys[SDL_SCANCODE_A])     eye = eye - right*step;
        if (keys[SDL_SCANCODE_D])     eye = eye + right*step;
        if (keys[SDL_SCANCODE_SPACE]) eye.y += step;
        if (keys[SDL_SCANCODE_LCTRL]) eye.y -= step;

        // 2. Caméra : on regarde un point placé à la distance focal devant soi
        lookat(eye, eye + forward*focal, up);
        perspective(focal);
        viewport(0, 0, width, height);

        // 3. Rendu, comme pour une image TGA
        TGAImage framebuffer(width, height, TGAImage::RGB);
        std::vector<double> zbuffer(width*height, -std::numeric_limits<double>::max());
        std::srand(0); // mêmes couleurs à chaque image, sinon ça clignote
        for (const Model& model : models) {
            for (int i=0; i<model.nfaces(); i++) {
                vec4 clip[3];
                for (int d : {0,1,2}) {
                    vec3 v = model.vert(i, d);
                    clip[d] = Perspective * ModelView * vec4{v.x, v.y, v.z, 1.};
                }
                TGAColor rnd;
                for (int c=0; c<3; c++) rnd[c] = std::rand()%255;
                clip_and_rasterize(clip, zbuffer, framebuffer, rnd);
            }
        }

        // 4. Affichage : TGAImage (BGR, origine en bas) -> texture SDL (ARGB, origine en haut)
        for (int y=0; y<height; y++)
            for (int x=0; x<width; x++) {
                TGAColor c = framebuffer.get(x, y);
                pixels[(height-1-y)*width + x] = 0xFF000000u | c[2]<<16 | c[1]<<8 | c[0];
            }
        SDL_UpdateTexture(texture, nullptr, pixels.data(), width*sizeof(std::uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        // FPS dans le titre de la fenêtre
        fps_frames++;
        if (now - fps_timer >= 1000) {
            std::string title = "tinyRenderer - " + std::to_string(fps_frames) + " fps";
            SDL_SetWindowTitle(window, title.c_str());
            fps_frames = 0;
            fps_timer  = now;
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
