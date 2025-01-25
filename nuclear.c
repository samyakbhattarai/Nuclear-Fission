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
#define MAX_NEUTRONS 4

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
    int active;
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

int check_collision(int nx, int ny, int ux, int uy){
	int d_s = pow(nx-ux,2)+pow(ny-uy,2); //Distance
	return d_s<=pow(NEUTRON_RADIUS+URANIUM_RADIUS,2);
} 

// Generate random directions for velocity
// void generate_random_directions(double* v_x, double* v_y) {
// 	double angle = ((double)rand() / RAND_MAX) * 2 * M_PI; // Random angle in radians
// 	*v_x = cos(angle);
// 	*v_y = sin(angle);
// }

void generate_random_directions(double* v_x, double* v_y, int direction) {
    *v_x=1;
    if(direction==0){
        *v_y = 0;
    }else if(direction==1){
        *v_y = 1;
    }else{
        *v_y = -1;
    }
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
    struct Circle uranium[2] = {{WIDTH/2,HEIGHT/2,URANIUM_RADIUS,0,0,1},{3*WIDTH/4,HEIGHT/2,URANIUM_RADIUS,0,0,1}};
    struct Circle neutron[MAX_NEUTRONS+1];
	// neutron[0] = {10,HEIGHT/2,NEUTRON_RADIUS,NEUTRON_VX,NEUTRON_VY};
    neutron[0].x = 10;
    neutron[0].y = HEIGHT/2;
    neutron[0].r = NEUTRON_RADIUS;
    neutron[0].v_x = NEUTRON_VX;
    neutron[0].v_y = NEUTRON_VY;
    neutron[0].active = 1;
	// struct Circle split_neutrons[MAX_NEUTRONS];
	SDL_Rect erase_rect = (SDL_Rect){0,0,WIDTH,HEIGHT};
	SDL_Event event;
	int split=0;
	int simulation_running=1;
    int index_n = -1;
    int direction = 0; //TEST
    int count=0;
    int t_n = 1;
    int help_me = 0; //HELP ME 
	while(simulation_running){
		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT){
				simulation_running = 0;
			}
		}
		SDL_FillRect(surface,&erase_rect,COLOR_WHITE);
        for(int k=0;k<2;k++){
            if(uranium[k].active){
            for(int p=0;p<t_n;p++){
                printf("CHECKED FOR %d\t THE T_N: %d\n", p, t_n);
                if(neutron[p].active && check_collision(neutron[p].x,neutron[p].y,uranium[k].x,uranium[k].y)){
                        printf("THOKYOOO %.1f\n", uranium[k].r);
                        uranium[k].active = 0;
                        split=1;
                        count=1;
                        neutron[p].active = 0;
                        printf("P: %d\t", p);
                        for(int i=0;i<3;i++){
                            neutron[i+t_n].x = uranium[k].x;
                            neutron[i+t_n].y = uranium[k].y;
                            neutron[i+t_n].r = NEUTRON_RADIUS;
                            neutron[i+t_n].active = 1;
                            generate_random_directions(&neutron[i+t_n].v_x, &neutron[i+t_n].v_y,direction);
                            direction++;    
                        }
                        help_me+=2;
                    
            }
        }
        }}

        for(int p=0;p<t_n;p++){
            if(!split && neutron[p].active){
                FillCircle(surface,neutron[p],COLOR_BLACK);
                step(&neutron[p]);
            }else{  
                for(int i=0; i<3; i++){
                    FillCircle(surface, neutron[t_n+i], COLOR_BLACK);
                    step(&neutron[t_n+i]);
                }
                            
                split=0;
            }}

        for(int k=0;k<2;k++){
            if(uranium[k].active){
                 FillCircle(surface,uranium[k], COLOR_BLUE); 
            }else{
                FillCircle(surface,uranium[k], COLOR_GRAY); 
            }
        }

        if(count==1){
            printf("The count!");
            count=0;
            t_n=help_me;
            direction=0;
        }
        
		SDL_UpdateWindowSurface(window);
		SDL_Delay(1);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
