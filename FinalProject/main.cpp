#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include "SDL_image.h"  // Using SDL2 image extension library 
#include "SDL_ttf.h"
#include "SDLusage.h"
#include "Interface.h"
#include "Map.h"
#include "randomCar.h"
#include "Car.h"
#include "House.h"
#include "Event.h"

void InitialLevel(int, WindowData&, ValueData&, CarData*, EventData&, ImageData*);

SDL_Window* window = NULL; // The window we'll be rendering to
SDL_Renderer* renderer = NULL; // The window rendererint initSDL()

bool windowChange = false;


WindowData fullViewport;

ValueData value = { 0, 0, 0, 0, 0, 0 };


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
	bool quit = false, flag=false;
	char pause = 0;

	//Event handler
	SDL_Event e;
	Mouse mouse;

	srand(time(NULL));

	
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
	char car_pic_path[7][100] = {"../images/fire_truck.png",
	"../images/bubble_truck.png", "../images/truck.png", "../images/police_car.png", "../images/car.png","../images/car1.png",  "../images/car2.png"};
	ImageData car_pic[7];
	for (int i = 0; i < 7; i++)
		car_pic[i] = loadImgTexture(renderer, car_pic_path[i], 1, 1, 1);
	ImageData cloud_pic[2];
	cloud_pic[0] = loadImgTexture(renderer, (char*)"../images/white_cloud.png", 1, 1, 1);
	cloud_pic[1] = loadImgTexture(renderer, (char*)"../images/dark_cloud.png", 1, 1, 1);
	char event_pic_path[6][100] = { "../images/fire.png", "../images/accident.png","../images/closed_road.png" ,"../images/delivery.png" ,"../images/stolen.png", "../images/flash.png" };
	ImageData event_pic[6];
	for (int i = 0; i < 6; i++)
		event_pic[i] = loadImgTexture(renderer, event_pic_path[i], 1, 1, 1);

	CarData car[CARNUM];
	int next_inci, inci=0, inci_alpha=0, path_length;
	EventData event;

	
	SDL_TimerID timerID_clock;
	SDL_TimerID timerID_incident;
	SDL_TimerID timerID_car;
	SDL_TimerID timerID_event;

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

		mouse.state = NONE; //Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			keyboardHandleEvent(e, quit);
			mouseHandleEvent(&e, &mouse.state, &mouse.X, &mouse.Y);
		}

		// Clear screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderClear(renderer);

		if (value.level) {
			flag = true;

			if (next_inci || inci) {
				if (next_inci) {
					inci = next_inci;
					inci_alpha = 450;
					timerID_incident = SDL_AddTimer(50, incident_add, &inci_alpha);
				}
				else if (inci && (!inci_alpha))
					inci = 0;
			}

			//draw screen
			mapRender(renderer, fullViewport, road_pic, road_pic1);
			buildRender(renderer, fullViewport, build, build_pic);
			next_inci=eventRender(renderer, fullViewport, event, car, cloud_pic, build, value);
			carRender(renderer, fullViewport, car, car_pic, cloud_pic);
			incident(renderer, fullViewport, windowChange, inci, inci_alpha);
			controlRender(renderer, fullViewport, windowChange, mouse, pause, value, control_pic, build_pic, flag_pic);

			//add things
			next_inci += addCar(renderer, fullViewport, mouse, pause, event, build, car, car_pic);
			next_inci += addBuild(renderer, fullViewport, mouse, value.money, build, build_pic, car);
			
			switch (pause) {
			case 1:
				pause = 2;
				SDL_RemoveTimer(timerID_clock);
				SDL_RemoveTimer(timerID_car);
				SDL_RemoveTimer(timerID_event);
				break;
			case 3:
				pause = 0;
				timerID_clock = SDL_AddTimer(10, clock_add, &value);
				timerID_car = SDL_AddTimer(10, car_move, car);
				timerID_event = SDL_AddTimer(1000, event_change, &event);
				break;
			case 4:
				quit = true;
				break;
			case 5:
				value.level = 0;
				pause = 0;
				SDL_RemoveTimer(timerID_clock);
				SDL_RemoveTimer(timerID_car);
				SDL_RemoveTimer(timerID_event);
				car_move(NULL, (void*)car);
				createBuilding(build, fullViewport, road, 0);
				createRandomMap(fullViewport, road, 0);
				InitialLevel(value.level, fullViewport, value, car, event, event_pic);
				break;
			}
		}
		else {
			if (menuRender(renderer, fullViewport, windowChange, mouse, value)) {
				quit = true;
			}
			if (value.level) {
				inci = 0, inci_alpha = 0;
				InitialLevel(value.level, fullViewport, value, car, event, event_pic);
				next_inci = event.type + 6;
				timerID_clock = SDL_AddTimer(10, clock_add, &value);
				timerID_incident;
				timerID_car = SDL_AddTimer(10, car_move, car);
				timerID_event = SDL_AddTimer(1000, event_change, &event);
			}
		}
		
		// Update screen
		SDL_RenderPresent(renderer);
	}	

	if (flag) {
		incident(NULL, fullViewport, windowChange, inci, inci_alpha);
		mapRender(NULL, fullViewport, road_pic, road_pic1);
		controlRender(NULL, fullViewport, windowChange, mouse, pause, value, control_pic, build_pic, flag_pic);
		buildRender(NULL, fullViewport, build, build_pic);
	}
	closeSDL(window, renderer);

	return 0;
}

void InitialLevel(int level, WindowData& fullViewport, ValueData& value, CarData car[], EventData& event, ImageData event_pic[]) {
	fullViewport.oldh = fullViewport.oldw = 1;
	value.level = level;
	switch (level) {
	case 0:
		fullViewport.hnum = fullViewport.wnum = 1;
		break;
	case 1:
	case 2:
	case 3:
		fullViewport.hnum = 3 + 3 * level, fullViewport.wnum = 5 + 3 * level;
		//initial value
		value.time = 0, value.season = rand() % 4, value.population = 35 * value.level, value.money = 0;
		createRandomMap(fullViewport, road, fullViewport.hnum*fullViewport.wnum*0.85);
		destroyRoad(road, fullViewport.hnum, fullViewport.wnum);
		createBuilding(build, fullViewport, road, fullViewport.hnum * fullViewport.wnum /4);
		while (initCar(&fullViewport, car, build, fullViewport.hnum*fullViewport.wnum/3)) {
			createRandomMap(fullViewport, road, 25 + 20 * value.level);
			destroyRoad(road, fullViewport.hnum, fullViewport.hnum * fullViewport.wnum * 0.85);
			createBuilding(build, fullViewport, road, fullViewport.hnum * fullViewport.wnum/3);
		}
		createEvent(event, fullViewport, event_pic, build, &value, car);
		break;
	}
}