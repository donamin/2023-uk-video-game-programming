// Useful #define and #typedef definitions

#pragma once

#include <SFML/Graphics.hpp>

#define for_loop(i, n) for(int i = 0; i < n; i++)
#define for_loop_reverse(i, n) for(int i = n - 1; i >= 0; i--)
#define printf_pair_i(x, y) printf("%d, %d\n", x, y)
#define printf_pair_f(x, y) printf("%.2f, %.2f\n", x, y)

typedef sf::Vector2i Vec2i;
typedef sf::Vector2f Vec2f;
typedef sf::Vector3i Vec3i;
typedef sf::Vector3f Vec3f;