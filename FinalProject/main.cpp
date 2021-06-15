#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include "SDL_image.h"  // Using SDL2 image extension library 
#include "SDL_ttf.h"
#include "SDLusage.h"
#include "Interface.h"
#include "Map.h"
#include "House.h"
#include "randomCar.h"
#include "Car.h"
#include "Event.h"


SDL_Window* window = NULL; // The window we'll be rendering to
SDL_Renderer* renderer = NULL; // The window rendererint initSDL()   int w:1;

int population, love, money;
int t = 0;
bool windowChange = false;


WindowData fullViewport = { 0, 0, 8, 6, 1, 1};

Building** build=NULL;


// When using SDL, you have to use "int main(int argc, char* args[])"
// int main() and void main() are not allowed
int main(int argc, char* args[])
{
	// Start up SDL and create window
	if (initSDL(window,renderer))
	{
		printf("Failed to initialize SDL!\n");
		return -1;
	}	
	SDL_GL_GetDrawableSize(window, &(fullViewport.w), &(fullViewport.h));
	
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;
	MouseState mouseState;
	int mouseX, mouseY;

	srand(time(NULL));

	SDL_TimerID timerID_clock = SDL_AddTimer(10, clock_add, &t);
	SDL_TimerID timerID_incident;
	SDL_TimerID timerID_car;
	SDL_TimerID timerID_event;

	//load images
	char control_pic_path[3][100] = { "../images/pop_value.png", "../images/money_value.png","../images/love_value.png" };
	ImageData control_pic[3];
	for(int i=0; i<3; i++)
		control_pic[i] = loadImgTexture(renderer, control_pic_path[i], 1, 1, 1);
	ImageData road_pic = loadImgTexture(renderer, (char*)"../images/road.png", 1, 1, 1);
	ImageData road_pic1 = loadImgTexture(renderer, (char*)"../images/road_1.png", 1, 1, 1);
	char build_pic_path[10][100] = { "../images/normal_building.png","../images/normal_building_2.png",  "../images/special_building_1.png", "../images/special_building_2.png",
	"../images/FireStation.png","../images/logistics.png", "../images/Police.png", "../images/shopping.png", "../images/school.png", "../images/hospital.png"};
	ImageData build_pic[10];
	for(int i=0; i<10; i++)
		build_pic[i] = loadImgTexture(renderer, build_pic_path[i], 1, 1, 1);
	ImageData flag_pic = loadImgTexture(renderer, (char*)"../images/flag.png", 1, 1, 1);
	char car_pic_path[8][100] = { "../images/car.png","../images/car1.png",  "../images/car2.png", "../images/fire_truck.png",
	"../images/bubble_truck.png","../images/truck.png", "../images/truck1.png", "../images/police_car.png"};
	ImageData car_pic[8];
	for (int i = 0; i < 8; i++)
		car_pic[i] = loadImgTexture(renderer, car_pic_path[i], 1, 1, 1);
	ImageData fire = loadImgTexture(renderer, (char*)"../images/fire.png", 1, 1, 1);

	createRandomMap(fullViewport, road, RoadLength);
	destroyRoad(road, fullViewport.hnum, fullViewport.wnum);
	createBuilding(build, fullViewport, road);

	int next_inci, inci=0, inci_alpha=0, path_length;

	CarData car[RANDCARNUM+4];
	point *start=new point [RANDCARNUM];
	bool repeat = false;
	for (int i = 0; i < RANDCARNUM + 4; i++) {
		car[i].velocity = -1;
	}
	for (int i = 0; i < RANDCARNUM; i++) {
		//printf("%d\n", i);
		do {
			repeat = false;
			start[i] = randomstart(fullViewport, road);
			//printf("%d %d\n", start[i].x, start[i].y);
			for (int j = 0; j <i; j++)
				if ((start[j].x == start[i].x)&& (start[j].y == start[i].y)) {
					repeat = true;
					break;
				}
		} while (repeat);
	}
	for (int i = 4; i < RANDCARNUM + 4; i++)
		createRandomCar(car[i], fullViewport, start[i-4], car_pic[rand() % 3]);
	delete[]start;

	//EventData event;
	//event.level = 1, event.season=0;
	//createEvent(event, fullViewport);


	timerID_car = SDL_AddTimer(15, car_move, car);
	//timerID_event = SDL_AddTimer(100, event_change, &event);

	
	//While application is running
	while (!quit)
	{
		SDL_GL_GetDrawableSize(window, &(fullViewport.w), &(fullViewport.h));
		if (fullViewport.oldw != fullViewport.w || fullViewport.oldh != fullViewport.h) {
			windowChange = true;
		}
		else {
			fullViewport.oldw = fullViewport.w;
			fullViewport.oldh = fullViewport.h;
			windowChange = false;
		}
		
		mouseState = NONE; //Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			keyboardHandleEvent(e, quit);
			mouseHandleEvent(&e, &mouseState, &mouseX, &mouseY);
		}

		// Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderClear(renderer);

		//draw screen
		mapRender(renderer, fullViewport, road_pic, road_pic1); 
		controlRender(renderer, fullViewport, windowChange, t, population, money, love, control_pic, build_pic, flag_pic);
		buildRender(renderer, fullViewport, build, build_pic);
		carRender(renderer, fullViewport, car);
		incident(renderer, fullViewport, windowChange, inci, inci_alpha);
		next_inci = addCar(renderer, fullViewport, mouseState, mouseX, mouseY, build, car, car_pic);
		next_inci += addBuild(renderer, fullViewport, mouseState, mouseX, mouseY, build, build_pic);
		if (next_inci||inci) {
			if (next_inci) {
				inci = next_inci;
				inci_alpha = 450;
				timerID_incident = SDL_AddTimer(50, incident_add, &inci_alpha);
			}
			else if (inci&&(!inci_alpha))
				inci = 0;
		}
		//eventRender(renderer, fullViewport, event);
		
		// Update screen
		SDL_RenderPresent(renderer);
	}	

	//Free resources and close SDL
	SDL_RemoveTimer(timerID_clock);
	SDL_RemoveTimer(timerID_car);
	//SDL_RemoveTimer(timerID_event);
	incident(NULL, fullViewport, windowChange, inci, inci_alpha);
	mapRender(NULL, fullViewport, road_pic, road_pic1);
	controlRender(NULL, fullViewport, windowChange, t, population, money, love, control_pic, build_pic, flag_pic);
	buildRender(NULL, fullViewport, build, build_pic);
	closeSDL(window,renderer);

	return 0;
}