#include<stdio.h>
#include<SDL2/SDL.h>
#include<math.h>
#include<stdlib.h> // For rand()

#define WIDTH 1280
#define HEIGHT 720
#define COLOR_BLUE 0xFF0056F3
#define COLOR_BLACK 0x000000
#define COLOR_GRAY 0x88000000
#define COLOR_WHITE 0xffffff
#define URANIUM_RADIUS 30
#define NEUTRON_RADIUS 6
#define NEUTRON_VX 1
#define NEUTRON_VY 0
#define MAX_NEUTRONS 6

//DRAW URANIUM
#define U_IPX 320
#define U_IPY 200
#define U_SPACE 100
#define ROW 4
#define COLUMN 4

struct Circle{
	double x;
	double y;
	double r;
	double v_x;
	double v_y;
};


//Drawing a circle
void FillCircle(SDL_Surface* surface, struct Circle body, Uint32 color){
	        double high_x,low_x,high_y,low_y;
			high_x = body.x + body.r;
			low_x = body.x - body.r;
			high_y = body.y + body.r;
			low_y = body.y - body.r;
			double radius_squared = pow(body.r,2);
			for(double x=low_x;x<high_x;x++){
				for(double y=low_y;y<high_y;y++){
					double distance_squared = pow(x-body.x,2) + pow(y-body.y,2);
						if(distance_squared<radius_squared){
							SDL_Rect pixel = (SDL_Rect){x,y,1,1};
							SDL_FillRect(surface, &pixel, color);
						}
				}
			}
}

void step(struct Circle* body){
	body->x+=body->v_x;
	body->y+=body->v_y;
}

int check_collision(struct Circle neutron, struct Circle uranium){
	int d_s = pow(neutron.x-uranium.x,2)+pow(neutron.y-uranium.y,2); //Distance
	return d_s<=pow(neutron.r+uranium.r,2);
} 

// Generate random directions for velocity
void generate_random_directions(double* v_x, double* v_y) {
	double angle = ((double)rand() / RAND_MAX) * 2 * M_PI; // Random angle in radians
	*v_x = cos(angle);
	*v_y = sin(angle);
}

//MAIN FUNCTION

int main(int argc, char* argv[]){
	SDL_Init(SDL_INIT_EVERYTHING);
	srand(SDL_GetTicks()); // Seed random number generator with current time
	SDL_Window* window = SDL_CreateWindow("Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Surface* surface = SDL_GetWindowSurface(window);
	SDL_Rect background = (SDL_Rect){0,0,WIDTH,HEIGHT};
	SDL_FillRect(surface, &background, 0xffffff);
	// struct Circle uranium = {WIDTH/2,HEIGHT/2,URANIUM_RADIUS,0,0};
    struct Circle uranium[2] = {{WIDTH/2,HEIGHT/2,URANIUM_RADIUS,0,0},{3*WIDTH/4,HEIGHT/2,URANIUM_RADIUS,0,0}};
	struct Circle neutron = {10,HEIGHT/2,NEUTRON_RADIUS,NEUTRON_VX,NEUTRON_VY};
	struct Circle split_neutrons[MAX_NEUTRONS];
	SDL_Rect erase_rect = (SDL_Rect){0,0,WIDTH,HEIGHT};
	SDL_Event event;
	int split=0;
	int simulation_running=1;
    int u_split = 0;

	while(simulation_running){
		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT){
				simulation_running = 0;
			}
		}
		SDL_FillRect(surface,&erase_rect,COLOR_WHITE);
        for(int k=0;k<2;k++){
            if(!split && check_collision(neutron,uranium[k])){
                split=1;
                u_split = k;
                for(int i=0; i<3; i++){
                    split_neutrons[i].x = uranium[k].x;
                    split_neutrons[i].y = uranium[k].y;
                    split_neutrons[i].r = NEUTRON_RADIUS;
                    generate_random_directions(&split_neutrons[i].v_x, &split_neutrons[i].v_y);
                }
            }
        }
        for(int k=0;k<2;k++){
            if(k==u_split && split==1){
                FillCircle(surface,uranium[u_split],COLOR_GRAY);
            }else{
                FillCircle(surface,uranium[k], COLOR_BLUE);   
            }
        }

		if(split==0){
			FillCircle(surface,neutron,COLOR_BLACK);
			step(&neutron);
		}else{
			for(int i=0; i<3; i++){
				FillCircle(surface, split_neutrons[i], COLOR_BLACK);
				step(&split_neutrons[i]);
			}
		}

		SDL_UpdateWindowSurface(window);
		SDL_Delay(5);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
