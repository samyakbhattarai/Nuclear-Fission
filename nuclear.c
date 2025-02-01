#include<stdio.h>
#include<SDL2/SDL.h>
#include<math.h>
#include<stdlib.h>

#define WIDTH 1280
#define HEIGHT 720
#define COLOR_BLUE 0xFF0056F3
#define COLOR_BLACK 0x000000
#define COLOR_GRAY 0x88000000
#define COLOR_WHITE 0xffffff
#define URANIUM_RADIUS 20
#define NEUTRON_RADIUS 6
#define NEUTRON_SPEED 6
#define MAX_NEUTRONS 1000
#define GRID_ROWS 8
#define GRID_COLUMNS 8
#define GRID_SPACING 50

struct Circle {
    double x;
    double y;
    double r;
    double v_x;
    double v_y;
    int active;
};

// Drawing the circle
void FillCircle(SDL_Surface* surface, struct Circle body, Uint32 color) {
    double radius_squared = pow(body.r, 2);
    for (int x = body.x - body.r; x <= body.x + body.r; x++) {
        for (int y = body.y - body.r; y <= body.y + body.r; y++) {
            double distance_squared = pow(x - body.x, 2) + pow(y - body.y, 2);
            if (distance_squared <= radius_squared) {
                SDL_Rect pixel = {x, y, 1, 1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

// Moving the neutron
void step(struct Circle* body) {
    body->x += body->v_x;
    body->y += body->v_y;
}

// Check if neutron and uranium collide
int check_collision(double nx, double ny, double ux, double uy) {
    double distance_squared = pow(nx - ux, 2) + pow(ny - uy, 2);
    return distance_squared <= pow(NEUTRON_RADIUS + URANIUM_RADIUS, 2);
}



void generate_random_directions(double *v_x, double *v_y) {
    double angle = ((double)rand() / RAND_MAX) * 2 * M_PI; // Random angle in radians
    double speed = NEUTRON_SPEED; // Desired speed of the neutron
    *v_x = speed * cos(angle);
	*v_y = speed * sin(angle);
}


int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Neutron Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    SDL_Event event;

    // Create a 4x4 grid of uranium
    struct Circle uranium[GRID_ROWS * GRID_COLUMNS];
    int uranium_count = 0;
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLUMNS; col++) {
            uranium[uranium_count++] = (struct Circle){
                .x = 300 + col * (2 * URANIUM_RADIUS + GRID_SPACING),
                .y = GRID_SPACING + row * (2 * URANIUM_RADIUS + GRID_SPACING),
                .r = URANIUM_RADIUS,
                .v_x = 0,
                .v_y = 0,
                .active = 1
            };
        }
    }

    // Set the neutron to target the first uranium atom
    struct Circle neutron[MAX_NEUTRONS];
    int t_n = 1; // Total active neutrons
    int direction = 0; // Direction index for splitting neutrons
    int help_me = 0; // Helper for tracking new neutrons
    double target_x = uranium[0].x;
    double target_y = uranium[0].y;
    double angle = atan2(target_y - HEIGHT / 2, target_x - 10);
    neutron[0] = (struct Circle){
        .x = 10,
        .y = HEIGHT / 2,
        .r = NEUTRON_RADIUS,
        .v_x = NEUTRON_SPEED * cos(angle),
        .v_y = NEUTRON_SPEED * sin(angle),
        .active = 1
    };

    int simulation_running = 1;
    while (simulation_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                simulation_running = 0;
            }
        }

        SDL_FillRect(surface, NULL, COLOR_WHITE);

        // Handle collisions and splitting
        for (int i = 0; i < t_n; i++) {
            if (neutron[i].active) {
                for (int j = 0; j < uranium_count; j++) {
                    if (uranium[j].active && check_collision(neutron[i].x, neutron[i].y, uranium[j].x, uranium[j].y)) {
                        uranium[j].active = 0; // Deactivate uranium
                        neutron[i].active = 0; // Deactivate current neutron

                        // Generate 3 new neutrons
                        for (int k = 0; k < 3; k++) {
                            int new_index = t_n + k;
                            if (new_index < MAX_NEUTRONS) {
                                neutron[new_index] = (struct Circle){
                                    uranium[j].x, uranium[j].y, NEUTRON_RADIUS, 0, 0, 1
                                };
                                generate_random_directions(&neutron[new_index].v_x, &neutron[new_index].v_y);
                             }
                        }
                        help_me += 3;
                    }
                }
            }
        }

        // Update neutron
        t_n += help_me;
        help_me = 0;

        // Move and draw active neutrons
        for (int i = 0; i < t_n; i++) {
            if (neutron[i].active) {
                step(&neutron[i]);
                FillCircle(surface, neutron[i], COLOR_BLACK);
            }
        }

        // Draw uranium
        for (int j = 0; j < uranium_count; j++) {
            if (uranium[j].active) {
                FillCircle(surface, uranium[j], COLOR_BLUE);
            } else {
                FillCircle(surface, uranium[j], COLOR_GRAY);
            }
        }

        SDL_UpdateWindowSurface(window);
        SDL_Delay(1);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
