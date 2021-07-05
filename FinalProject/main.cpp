#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include "SDL_image.h"  // Using SDL2 image extension library 
#include "SDL_ttf.h"
#include "SDL_mixer.h" // Using SDL2_mixer
#include "SDLusage.h"
#include "Interface.h"
#include "Map.h"
#include "randomCar.h"
#include "Car.h"
#include "House.h"
#include "Event.h"
#include "AI.h"


void InitialLevel(int level, WindowData& fullViewport, ValueData& value, CarData** car, EventData* event, ImageData event_pic[], InciData& inci, Mix_Music*& music, char tone_Path[3][50], SDL_TimerID timer[]);
void CloseLevel(int level, WindowData& fullViewport, ValueData& value, CarData** car, EventData* event, ImageData event_pic[], InciData& inci, Mix_Music*& music, Mouse& mouse, ImageData build_pic[], ImageData& flag_pic, ImageData season_pic[], char& pause, SDL_TimerID timer[]);

SDL_Window* window = NULL;
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
	char season_pic_path[4][50] = { "../images/spring.png", "../images/summer.png","../images/autumn.png","../images/snow.png" };
	ImageData season_pic[4];
	for (int i = 0; i < 4; i++)
		season_pic[i] = loadImgTexture(renderer, season_pic_path[i], 1, 1, 1);
	char control_pic_path[3][100] = { "../images/pop_value.png", "../images/money_value.png","../images/love_value.png" };
	ImageData control_pic[3];
	for(int i=0; i<3; i++)
		control_pic[i] = loadImgTexture(renderer, control_pic_path[i], 1, 1, 1);
	ImageData road_pic = loadImgTexture(renderer, (char*)"../images/road.png", 1, 1, 1);
	ImageData road_pic1 = loadImgTexture(renderer, (char*)"../images/road_1.png", 1, 1, 1);
	char build_pic_path[10][50] = { "../images/normal_building.png","../images/normal_building_2.png",  "../images/special_building_1.png", "../images/special_building_2.png",
	"../images/FireStation.png","../images/logistics.png", "../images/Police.png", "../images/shopping.png", "../images/school.png", "../images/hospital.png"};
	ImageData build_pic[10];
	for(int i=0; i<10; i++)
		build_pic[i] = loadImgTexture(renderer, build_pic_path[i], 1, 1, 1);
	ImageData flag_pic = loadImgTexture(renderer, (char*)"../images/flag.png", 1, 1, 1);
	char car_pic_path[7][50] = {"../images/fire_truck.png",
	"../images/bubble_truck.png", "../images/truck.png", "../images/police_car.png", "../images/car.png","../images/car1.png",  "../images/car2.png"};
	ImageData car_pic[7];
	for (int i = 0; i < 7; i++)
		car_pic[i] = loadImgTexture(renderer, car_pic_path[i], 1, 1, 1);
	ImageData cloud_pic[2];
	cloud_pic[0] = loadImgTexture(renderer, (char*)"../images/white_cloud.png", 1, 1, 1);
	cloud_pic[1] = loadImgTexture(renderer, (char*)"../images/dark_cloud.png", 1, 1, 1);
	char event_pic_path[6][50] = { "../images/fire.png", "../images/accident.png","../images/closed_road.png" ,"../images/delivery.png" ,"../images/stolen.png", "../images/flash.png" };
	ImageData event_pic[6];
	for (int i = 0; i < 6; i++)
		event_pic[i] = loadImgTexture(renderer, event_pic_path[i], 1, 1, 1);
	static ImageData inci_pic = loadImgTexture(renderer, (char*)"../images/incident.png", 1, 1, 1);


	//Initialize path of tone wave files
	char tone_Path[3][50] = { "../music/instruction.mp3", "../music/menu.mp3", "../music/game.mp3"};

	CarData *car=NULL;
	InciData inci;
	EventData event[3];
	Mix_Music* music = NULL;
	
	SDL_TimerID timer[4]; 
	InitialLevel(value.level, fullViewport, value, &car, event, event_pic, inci, music, tone_Path, timer);
	if (!music) {
		music = Mix_LoadMUS(tone_Path[0]);
		Mix_PlayMusic(music, -1);
	}
	//While application is running
	while (!quit)
	{
		SDL_GL_GetDrawableSize(window, &(fullViewport.w), &(fullViewport.h));
		if ((fullViewport.oldw != fullViewport.w || fullViewport.oldh != fullViewport.h)&&!windowChange) {
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
		if (value.level>0) {
			flag = true;
			//draw screen
			mapRender(renderer, fullViewport, road_pic, road_pic1);
			buildRender(renderer, fullViewport, build, build_pic);
			eventRender(renderer, fullViewport, event, car, cloud_pic, build, value, inci, event_pic);

			carRender(renderer, fullViewport, car, car_pic, cloud_pic);
			incident(renderer, fullViewport, windowChange, inci, inci_pic);
			controlRender(renderer, fullViewport, windowChange, mouse, pause, value, control_pic, build_pic, flag_pic, season_pic);
			//add things
			addCar(renderer, fullViewport, windowChange, mouse, pause, event, build, car, car_pic, inci);
			addBuild(renderer, fullViewport, windowChange, mouse, value, build, build_pic, car, inci);
			switch (pause) {
			case 1:
				pause = 2;
				for (int i = 0; i < 4; i++)
					SDL_RemoveTimer(timer[i]);
				break;
			case 3:
				pause = 0;
				timer[0] = SDL_AddTimer(10/value.speed+1, clock_add, &value);
				timer[1] = SDL_AddTimer(60, incident_add, &inci);
				timer[2] = SDL_AddTimer(10/value.speed+1, car_move, car);
				timer[3] = SDL_AddTimer(1000/value.speed+1, event_change, &event);
				break;
			case 4:
				CloseLevel(1, fullViewport, value, &car, event, event_pic, inci, music, mouse, build_pic, flag_pic, season_pic, pause, timer);
				quit = true;
				break;
			case 5:
				value.level = 0;
				pause = 0;
				CloseLevel(1, fullViewport, value, &car, event, event_pic, inci, music, mouse, build_pic, flag_pic, season_pic, pause, timer);
				InitialLevel(value.level, fullViewport, value, &car, event, event_pic, inci, music, tone_Path, timer);
				break;
			case 6:
				value.level++;
				pause = 0;
				CloseLevel(1, fullViewport, value, &car, event, event_pic, inci, music, mouse, build_pic, flag_pic, season_pic, pause, timer);
				InitialLevel(value.level, fullViewport, value, &car, event, event_pic, inci, music, tone_Path, timer);
			}
		}
		else if(value.level==-1){
		
			instRender(renderer, fullViewport, windowChange, mouse, value.level);
			if (value.level!=-1) {
				CloseLevel(-1, fullViewport, value, &car, event, event_pic, inci, music, mouse, build_pic, flag_pic, season_pic, pause, timer);
				InitialLevel(value.level, fullViewport, value, &car, event, event_pic, inci, music, tone_Path, timer);
				pause = 0;
			}
		}
		else {
			//draw screen
			mapRender1(renderer, fullViewport, road_pic, road_pic1);
			buildRender1(renderer, fullViewport, build, build_pic);
			carRender1(renderer, fullViewport, car, car_pic, cloud_pic);
			
			if(value.time%80==0)
				createBuilding1(build, fullViewport, road, fullViewport.hnum * fullViewport.wnum / 3, value.buildnum);
			if (value.time % 240 == 0) {
				value.time++;
				CloseLevel(0, fullViewport, value, &car, event, event_pic, inci, music, mouse, build_pic, flag_pic, season_pic, pause, timer);
				InitialLevel(value.level, fullViewport, value, &car, event, event_pic, inci, music, tone_Path, timer);
			}
			if (menuRender(renderer, fullViewport, windowChange, mouse, value)) {
				quit = true;
			}
			if (value.level!=0) {
				CloseLevel(0, fullViewport, value, &car, event, event_pic, inci, music, mouse, build_pic, flag_pic,season_pic,pause, timer);
				InitialLevel(value.level, fullViewport, value, &car, event, event_pic, inci, music, tone_Path, timer);
			}
		}
		musicRender(renderer, fullViewport, windowChange, value, mouse, music, tone_Path);
		
		// Update screen
		SDL_RenderPresent(renderer);
	}	

	if (Mix_PlayingMusic()) {
		while (!Mix_FadeOutMusic(250) && Mix_PlayingMusic()) {
			// wait for any fades to complete
			SDL_Delay(10);
		}
		Mix_FreeMusic(music);
	}

	//free up resource
	menuRender(NULL, fullViewport, windowChange, mouse, value);
	mapRender(NULL, fullViewport, road_pic, road_pic1);
	buildRender(NULL, fullViewport, build, build_pic);

	if (flag) {
		incident(NULL, fullViewport, windowChange, inci, inci_pic);
		controlRender(NULL, fullViewport, windowChange, mouse, pause, value, control_pic, build_pic, flag_pic, season_pic);
	}
	closeSDL(window, renderer);

	return 0;
}

void InitialLevel(int level, WindowData& fullViewport, ValueData& value, CarData** car, EventData* event, ImageData event_pic[], InciData& inci, Mix_Music*& music, char tone_Path[3][50], SDL_TimerID timer[]) {
	fullViewport.oldh = fullViewport.oldw = 1;
	value.level = level;
	switch (level) {
	case -1:
		break;
	case 0:
		fullViewport.hnum = 12, fullViewport.wnum = 14;
		fullViewport.carnum = fullViewport.hnum * fullViewport.wnum / 4;
		//initial value
		value.time = 21, value.speed=1;
		createRandomMap(fullViewport, road, fullViewport.hnum * fullViewport.wnum * 0.85);
		destroyRoad(road, fullViewport.hnum, fullViewport.wnum);
		createBuilding1(build, fullViewport, road, fullViewport.hnum * fullViewport.wnum / 3, value.buildnum);
		while (initCar(&fullViewport, car, build, fullViewport.hnum * fullViewport.wnum / 4)) {
			createRandomMap(fullViewport, road, 25 + 20 * value.level);
			destroyRoad(road, fullViewport.hnum, fullViewport.hnum * fullViewport.wnum * 0.85);
			createBuilding1(build, fullViewport, road, fullViewport.hnum * fullViewport.wnum / 3, value.buildnum);
		}
		timer[0] = SDL_AddTimer(600, clock_add, &value);
		timer[2] = SDL_AddTimer(2, car_move, *car);
		break;
	case 1:
	case 2:
	case 3:
		fullViewport.hnum = 3 + 3 * level, fullViewport.wnum = 5 + 3 * level;
		fullViewport.carnum = fullViewport.hnum * fullViewport.wnum *2/3;
		//initial value
		value.time = 0, value.season = rand() % 4, value.population = 35 * value.level, value.love = 0, value.money = 0, value.speed = 1;
		if (value.level == 2) {
			value.money = 100;
		}
		else if (value.level == 3) {
			value.money = 300;
		}
		createRandomMap(fullViewport, road, fullViewport.hnum*fullViewport.wnum*0.85);
		destroyRoad(road, fullViewport.hnum, fullViewport.wnum);
		createBuilding(build, fullViewport, road, fullViewport.hnum * fullViewport.wnum /4, value.buildnum);
		while (initCar(&fullViewport, car, build, fullViewport.hnum*fullViewport.wnum/3)) {
			createRandomMap(fullViewport, road, 25 + 20 * value.level);
			destroyRoad(road, fullViewport.hnum, fullViewport.hnum * fullViewport.wnum * 0.85);
			createBuilding(build, fullViewport, road, fullViewport.hnum * fullViewport.wnum/4, value.buildnum);
		}
		value.population = (value.buildnum[7]+value.buildnum[8])* value.level;
		inci.addhouse = inci.addcar = inci.car1 = inci.car2 = inci.car3 = 0;
		for (int i = 0; i < 5; i++)
			inci.alpha[i] = 0;
		for (int i = 0; i < value.level; i++) {
			createEvent(event[i], fullViewport, build, &value, *car, inci, i);
		}
		timer[0] = SDL_AddTimer(10/value.speed+1, clock_add, &value);
		timer[1] = SDL_AddTimer(60, incident_add, &inci);
		timer[2] = SDL_AddTimer(10/value.speed+1, car_move, *car);
		timer[3] = SDL_AddTimer(1000/value.speed+1, event_change, event);
		break;
	}
}
void CloseLevel(int level, WindowData& fullViewport, ValueData& value, CarData** car, EventData* event, ImageData event_pic[], InciData& inci, Mix_Music*& music, Mouse&mouse, ImageData build_pic[], ImageData& flag_pic, ImageData season_pic[], char &pause, SDL_TimerID timer[]) {
	if (level > 0) {
		car_move(NULL, *car);
		*car = NULL;
		addBuild(NULL, fullViewport, windowChange,mouse, value, build, build_pic, *car, inci);
		addCar(NULL, fullViewport, windowChange, mouse, pause, event, build, NULL, NULL, inci);
		controlRender(renderer, fullViewport, windowChange, mouse, pause, value, NULL, NULL, flag_pic, season_pic);
		createBuilding(build, fullViewport, road, 0, value.buildnum);
		createRandomMap(fullViewport, road, 0);
		for (int i = 0; i < 4; i++)
			SDL_RemoveTimer(timer[i]);
	}
	else if(level==0){
		car_move(NULL, *car);
		*car = NULL;
		createBuilding(build, fullViewport, road, 0, value.buildnum);
		createRandomMap(fullViewport, road, 0);
		SDL_RemoveTimer(timer[0]);
		SDL_RemoveTimer(timer[2]);
	}
	pause = 0;
}