# tinyRenderer

Un moteur de rendu 3D logiciel écrit en C++ à partir de zéro, en suivant le cours
[tinyrenderer](https://github.com/ssloy/tinyrenderer) de Dmitry V. Sokolov.

Aucune API graphique (OpenGL, Vulkan, DirectX…) n'est utilisée : chaque pixel est calculé
par le CPU. Le but est de comprendre ce qui se passe réellement dans une carte graphique.

## Fonctionnalités

- Chargement de modèles `.obj` (sommets et faces)
- Tracé de lignes (Bresenham)
- Rastérisation de triangles par coordonnées barycentriques
- Z-buffer (gestion de la profondeur)
- Backface culling
- Pipeline de transformations : `ModelView`, `Perspective`, `Viewport`
- Bibliothèque de vecteurs et matrices génériques (`geometry.h`)

## ⚠️ Branche `sdl` : version de test

Cette branche est **expérimentale**. Au lieu d'écrire une image `.tga` sur le disque
(comme sur `master`), le rendu est affiché **en temps réel** dans une fenêtre, avec une
caméra qu'on peut faire tourner autour du modèle.

L'affichage passe par **SDL3** : **il faut avoir SDL3 installé pour que ça compile et
fonctionne**. SDL sert uniquement à ouvrir la fenêtre et à afficher le framebuffer ;
tout le rendu reste fait à la main.

## Prérequis

- Un compilateur C++20 (`g++` testé)
- **SDL3** (attention : SDL2 ne marche pas)

| Système | Installation de SDL3 |
|---|---|
| Windows (MSYS2, terminal MINGW64) | `pacman -S mingw-w64-x86_64-sdl3` |
| macOS | `brew install sdl3` |
| Arch Linux | `sudo pacman -S sdl3` |
| Debian / Ubuntu (versions récentes) | `sudo apt install libsdl3-dev` |

## Compilation

```sh
g++ -std=c++20 -O2 main.cpp model.cpp tgaimage.cpp -o prog -lSDL3
```

Optionnel : ajouter `-fopenmp` pour paralléliser la rastérisation.

## Utilisation

```sh
./prog <modele.obj> [autre_modele.obj ...]
```

Exemple :

```sh
./prog african_head.obj
```

Les fichiers `.obj` ne sont pas inclus dans le dépôt. On peut récupérer ceux du cours
(`african_head.obj`, `diablo3_pose.obj`…) dans le dossier
[`obj/`](https://github.com/ssloy/tinyrenderer/tree/master/obj) du dépôt tinyrenderer.

## Contrôles

| Action | Effet |
|---|---|
| Clic gauche + glisser | Tourner autour du modèle |
| Flèches du clavier | Tourner autour du modèle |
| Molette | Zoomer / dézoomer |
| `Échap` | Quitter |

Le nombre d'images par seconde est affiché dans le titre de la fenêtre.

## Structure du projet

| Fichier | Rôle |
|---|---|
| `main.cpp` | Pipeline de rendu, boucle SDL, gestion de la caméra |
| `geometry.h` | Vecteurs et matrices génériques |
| `model.h` / `model.cpp` | Lecture des fichiers `.obj` |
| `tgaimage.h` / `tgaimage.cpp` | Image en mémoire et écriture au format TGA |

## Limites actuelles

- Chaque triangle a une couleur aléatoire : pas encore d'éclairage ni de textures.
- Pas de clipping : la distance de la caméra est bornée pour ne jamais entrer dans le modèle.
