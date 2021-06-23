#include<limits.h>


int carMenu(SDL_Renderer* renderer, const WindowData& fullViewport, MouseState mouseState, int mousex, int mousey, int& choose, ImageData car_pic[]);
void bestRoute(const WindowData& fullViewport, double** distance, int y, int x, int direction);
bool existDirection(int hnum, int wnum, int y, int x, int direction);
char* createPath(const WindowData& fullViewport, double start_xnum, double start_y, bool verticle, bool intersect, int house_hnum, int house_wnum, int& length);
void fillIntersect(char* intersect, int direction1, int direction2, int num);
bool equalIntersect(char* intersect, int direction1, int direction2, int num);

void createCar(CarData& car, WindowData& fullViewport, int start_ynum, int start_xnum, int house_hnum, int house_wnum, int type, ImageData car_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, wnum = fullViewport.wnum, hnum = fullViewport.hnum, oldw=fullViewport.oldw, oldh=fullViewport.oldh;

	char* path;
	car.length = INT_MAX;
	car.path = NULL;
	int choose, length;
	//Run for four times to choose the shortest path
	if ((checkRoad(start_ynum, start_xnum) >> 3) % 2) {		//top road
		if (start_xnum == house_wnum && (start_ynum == house_hnum + 1|| start_ynum == house_hnum)) {
			car.length = 0;
			choose = 3;
			car.velocity = -2;
			car.path = new char[1]{2};
		}
		else {
			path = createPath(fullViewport, start_ynum+0.5, start_xnum+1, false, false, house_hnum, house_wnum, length);
			if (car.length > length) {
				car.length = length;
				if (car.path) {
					delete[]car.path;
					car.path = NULL;
				}
				car.path = path;
				choose = 3;
			}
			else
				delete[]path;
		}
	}
	if ((checkRoad(start_ynum, start_xnum) >> 2) % 2&&car.length) {		//right road
		if ((start_xnum == house_wnum-1|| start_xnum == house_wnum) && start_ynum == house_hnum) {
			car.length = 0;
			choose = 2;
			car.velocity = -2;
			car.path = new char[1]{ 1 };
		}
		else {
			path = createPath(fullViewport, start_ynum+1, start_xnum + 1.5, true, false, house_hnum, house_wnum, length);
			if (car.length > length) {
				car.length = length;
				if (car.path) {
					delete[]car.path;
					car.path = NULL;
				}
				car.path = path;
				choose = 2;
			}
			else
				delete[]path;
		}
	}
	if ((checkRoad(start_ynum, start_xnum) >> 1) % 2&&car.length) {		//bottom road
		if (start_xnum == house_wnum && (start_ynum == house_hnum - 1|| start_ynum == house_hnum)) {
			car.length = 0;
			choose = 1;
			car.velocity = -2;
			car.path = new char[1]{ 0 };
		}
		else {
			path = createPath(fullViewport, start_ynum + 1.5, start_xnum+1, false, false, house_hnum, house_wnum, length);
			if (car.length > length) {
				car.length = length;
				if (car.path) {
					delete[]car.path;
					car.path = NULL;
				}
				car.path = path;
				choose = 1;
			}
			else
				delete[]path;
		}
	}
	if ((checkRoad(start_ynum, start_xnum)) % 2&&car.length) {		//left road
		if ((start_xnum == house_wnum+1|| start_xnum == house_wnum) && start_ynum == house_hnum) {
			car.length = 0;
			choose = 0;
			car.velocity = -2;
			car.path = new char[1]{ 3 };
		}
		else {
			path = createPath(fullViewport, start_ynum+1, start_xnum+0.5, true, false, house_hnum, house_wnum, length);
			if (car.length > length) {
				car.length = length;
				if (car.path) {
					delete[]car.path;
					car.path = NULL;
				}
				car.path = path;
				choose = 0;
			}
			else
				delete[]path;
		}
	}

	//Intialize the starting point
	switch (choose) {
		case 3:
			car.x = (start_xnum + 1.);
			car.y = (start_ynum + 0.5);
			break;
		case 2:
			car.x = (start_xnum + 1.5);
			car.y = (start_ynum + 1);
			break;
		case 1:
			car.x = (start_xnum + 1.);
			car.y = (start_ynum + 1.5);
			break;
		case 0:
			car.x = (start_xnum + 0.5);
			car.y = (start_ynum + 1);
			break;
	}

	//Initialize other information
	car.window = &fullViewport;
	car.type = type;
	car.i = 0;
	car.angle = (3-car.path[0])*90;
	car.home_ynum = house_hnum;
	car.home_xnum = house_wnum;
	if(car.length)
		car.velocity = 0.01;
	car.intersect = false;
}

int addCar(SDL_Renderer* renderer, WindowData& fullViewport, MouseState mouseState, int mousex, int mousey, Building**& build , CarData car[], ImageData car_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, wnum = fullViewport.wnum, hnum = fullViewport.hnum, xnum, ynum;
	static int choose = 0, select=0;
	static SDL_TimerID timerID_clock;
	
	//Cliking the flag
	if (select) {
		if(!carMenu(renderer, fullViewport, mouseState, mousex, mousey,choose, car_pic))	//false for quit
			select = 0;
	}
	
	//Right Click for releaing choose
	else if (mouseState == IN_RB_SC) {
		choose = 0;
		select = 0;
	}
	
	//Left Click
	else if (mouseState == IN_LB_SC) {
		
		//If choose, create car from special building to determined point
		if (choose) {
			if (mousex > (width - height / 12) / wnum / 2 && mousex<(width - height / 12 - (width - height / 12) / wnum / 2) && mousey >(height / 12 + ((height - height / 12) / hnum) / 2) && mousey < (height - ((height - height / 12) / hnum) / 2)) {
				xnum = (mousex - (width - height / 12) / wnum / 2) / ((width - height / 12) / wnum);
				ynum = (mousey - height / 12 - (height - height / 12) / hnum / 2) / ((height - height / 12) / hnum);
				if (build[ynum][xnum].type == Empty) {
					return 3;
				}
				else{
					CarData t, min;
					t.path=min.path = NULL;
					int length = INT_MAX, min_num=-1;
					//search for stopping special car
					for (int i = 0; i < CARNUM; i++)
						if (car[i].type == choose && car[i].velocity < 0) {
							if (!car[i].path)
								delete[](car[i].path);
							createCar(t, fullViewport, car[i].home_ynum, car[i].home_xnum, ynum, xnum, choose, car_pic);
							//find the closet car
							if (t.length < length) {
								min = t;
								min_num = i;
							}
						}
					//there is no car
					if (min_num == -1) {
						return 4;
					}
					else {
						car[min_num] = min;
						choose = 0;
					}
				}
			}
		}
		
		//If not choose, left click to enter select menu
		else if (mousex >= (width - height / 12) && mousey >= (height * 9 / 12) && mousey <= (height * 11 / 12)) {
			select = 1;
		}
	}
	
	//Fixed chosen car on mouse
	else {
		switch (choose) {
		case 1:
			imgRender(renderer, car_pic[0], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 230);
			break;
		case 2:
			imgRender(renderer, car_pic[1], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 230);
			break;
		case 3:
			imgRender(renderer, car_pic[2], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 230);
			break;
		case 4:
			imgRender(renderer, car_pic[3], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 230);
			break;
		}
	}

	return 0;
}

int carMenu(SDL_Renderer* renderer, const WindowData& fullViewport, MouseState mouseState, int mousex, int mousey, int& choose, ImageData car_pic[]) {
	int width = fullViewport.w, height = fullViewport.h;
	boxColor(renderer, 0, height / 12, width - height/ 12, height, 0xBB000000);
	imgRender(renderer, car_pic[0], RightBottom, width / 2 - 10, height / 2 - 10, NULL, height / 3, 1, NULL, NULL, 0, no, 250);
	imgRender(renderer, car_pic[1], LeftBottom, width / 2 + 10, height / 2 - 10, NULL, height / 3, 1, NULL, NULL, 0, no, 250);
	imgRender(renderer, car_pic[2], RightTop, width / 2 - 10, height / 2 + 10, NULL, height / 3, 1, NULL, NULL, 0, no, 250);
	imgRender(renderer, car_pic[3], LeftTop, width / 2 + 10, height / 2 + 10, NULL, height / 3, 1, NULL, NULL, 0, no, 250);

	//select which car
	if (mouseState == IN_LB_SC) {
		if (fabs(mousex - width / 2) < height / 6 && fabs(mousey - height / 2) < height / 3) {
			if ((mousey - height / 2) < 0) {
				if ((mousex - width / 2) < 0)
					choose = 1;
				else
					choose = 2;
			}
			else {
				if ((mousex - width / 2) < 0)
					choose = 3;
				else
					choose = 4;
			}
			//exit menu
			return 0;
		}

		//keep in menu
		else
			return 1;
	}

	//Right Click for exit menu
	if (mouseState == IN_RB_SC) {
		choose = 0;
		return 0;
	}

	//keep in menu
	return 1;
}

void carRender(SDL_Renderer* renderer, const WindowData& fullViewport, CarData car[], ImageData car_pic[], ImageData cloud_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum, wnum = fullViewport.wnum;
	for (int i = 0; i < CARNUM; i++) {

		//cloud for car
		if (car[i].velocity < -1) {
			//verticle
			if(car[i].angle%180)
				imgRender(renderer, cloud_pic[0], Middle, car[i].x * (width - height / 12) / wnum, car[i].y * (height * 11 / 12) / hnum + (height / 12),NULL, (height - height / 12) / hnum / 3., 1, NULL, NULL, 0, no, (int)(255 * (-1 - car[i].velocity)) % 255);
			else
				imgRender(renderer, cloud_pic[0], Middle, car[i].x * (width - height / 12) / wnum, car[i].y * (height * 11 / 12) / hnum + (height / 12), (width - height / 12) / wnum / 4., NULL, 1, NULL, NULL, 0, no, (int)(255*(-1-car[i].velocity))%255);
		}
		if (car[i].velocity < 0) {
			if (!car[i].path) {
				car[i].velocity = -1;
				delete[]car[i].path;
				car[i].path = NULL;
			}
			continue;
		}
		imgRender(renderer, car_pic[car[i].type-1], Left, car[i].x * (width - height / 12) / wnum, car[i].y * (height * 11 / 12) / hnum + (height / 12), fmin((width - height / 12) / wnum / 10., height * 11 / 12 / hnum / 6.), NULL, 1, NULL, NULL, car[i].angle, no, 255);
	}
}

Uint32 car_move(Uint32 interval, void* param)
{
	CarData *t = (CarData*)param;
	int width = t[4].window->w, height = t[4].window->h, wnum = t[4].window->wnum, hnum = t[4].window->hnum;
	
	//create the intersect for car (divide into four piece: 0:lefttop 1:righttop 2:rightbottom 3:leftbottom)
	static char*** CarIntersect = NULL;
	if (CarIntersect == NULL) {
		CarIntersect = new char** [hnum];
		for (int i = 0; i < hnum; i++) {
			CarIntersect[i] = new char* [wnum];
			for (int j = 0; j < wnum; j++) {
				CarIntersect[i][j] = new char[4];
				for (int k = 0; k < 4; k++)
					CarIntersect[i][j][k] = 0;
			}
		}
	}
	for (int i = 0; i < CARNUM; i++) {
		//car drives over the region
		if (t[i].x<0.45 || t[i].x>wnum - 0.45 || t[i].y<0.45 || t[i].y>hnum - 0.45) {
			t[i].velocity = -1;
			for (int j = 0; j < hnum; j++)
				for (int k = 0; k < wnum; k++)
					for (int l = 0; l < 4; l++)
						if (CarIntersect[j][k][l] == i + 1)
							CarIntersect[j][k][l] = 0;
		}
		//special car tansformed from random car
		if (t[i].velocity == -3) {
			t[i].velocity = -1;
			for (int j = 0; j < hnum; j++)
				for (int k = 0; k < wnum; k++)
					for (int l = 0; l < 4; l++)
						if (CarIntersect[j][k][l] == i + 1)
							CarIntersect[j][k][l] = 0;
		}
		//disappearing special car
		if (t[i].velocity < -1) {
			t[i].velocity += 0.004;
			continue;
		}
		//printf("%d\n", i);
		//If velocity =-1, car disappear
		if (t[i].velocity < 0) {
			if(t[i].type<5)
				continue;
			//Create new car and avoid generate in intersect with car
			else {
				while (t[i].path) {
					delete t[i].path;
					t[i].path = NULL;
				}
				while (t[i].path == NULL) {
					point start = randomstart(*(t[i].window), road);
					while (!equalIntersect(CarIntersect[start.y][start.x], 0, 2, 0)) {
						start = randomstart(*(t[i].window), road);
					}
					while (t[i].path == NULL) {
						createRandomCar(t[i], *(t[i].window), start);
					}
					if(t[i].path)
						fillIntersect(CarIntersect[start.y][start.x], t[i].path[0], t[i].path[0], i + 1);
				}
			}
		}

		
		//stop when there is a car in front of mycar(by direction) 
		bool stop = false;
		if(t[i].angle % 90 == 0)
			for (int k = 0; k < CARNUM; k++) {
				t[i].angle %= 360;
				if (i == k || t[k].velocity < 0)
					continue;
				int length;
				if (k == 2 || i == 2) //truck
					length = 3;
				else
					length = 2;
				if (t[i].angle == t[k].angle) {
					if (t[i].x == t[k].x && t[i].y == t[k].y)
						if (i > k)
							stop = true;
						else
							stop = false;
					switch (t[i].angle) {
						case 0:
							if (t[k].x == t[i].x && (t[i].y - t[k].y) > 0 && (t[i].y - t[k].y) < length / 4.5) {
								stop = true;
	//							printf("%d ", k);
							}
							break;
						case 90:
							if (t[k].y == t[i].y && (t[k].x - t[i].x) > 0 && (t[k].x - t[i].x) < length / 9.) {
								stop = true;
	//							printf("%d ", k);
							}
							break;
						case 180:
							if (t[k].x == t[i].x && (t[k].y - t[i].y) > 0 && (t[k].y - t[i].y) < length / 4.5) {
								stop = true;
	//							printf("%d ", k);
							}
							break;
						case 270:
							if (t[k].y == t[i].y && (t[i].x - t[k].x) > 0 && (t[i].x - t[k].x) < length / 9.) {
								stop = true;
	//							printf("%d ", k);
							}
							break;
					}
				}
			}
			
		//can't stop in intersection
		if (stop) {
	//		printf("%d %lf %lf %d --\n", i, t[i].x, t[i].y, t[i].angle);
			continue;
		}

		if (!t[i].path)
			continue;

		t[i].angle %= 360;
	//	printf("%lf\n", t[i].velocity);
		//If not into the intersect or the angle has meet the next direction(If not go straight)
		if (!checkIntersect(*(t[i].window), road, t[i].y, t[i].x)||((t[i].i < t[i].length) && ((t[i].path[t[i].i] != t[i].path[t[i].i + 1] && (t[i].angle == (3 - t[i].path[t[i].i + 1]) * 90))))) {
			if (t[i].intersect) {
				fillIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], 0);
				(t[i].i)++;
				CarIntersect[(int)t[i].y][(int)t[i].x][(t[i].path[t[i].i] + 3) % 4] = i + 1;
			}
			t[i].intersect = false;
	//		printf("1");
			//Leave the interscetion, free the intersection
			if (i == 2&& (fabs(t[i].x - (int)t[i].x - 0.5) > 1 / 3 || fabs(t[i].y - (int)t[i].y - 0.5) >= 1 / 2.01)) {	//for truck
				for (int j = 0; j < 2; j++)
					for (int k = 0; k < 2; k++)
						for (int l = 0; l < 4; l++)
							if (CarIntersect[(int)(t[i].y - 1 + j)][(int)(t[i].x-1+k)][l] == i + 1)
								CarIntersect[(int)(t[i].y - 1 + j)][(int)(t[i].x - 1 + k)][l] = 0;
			}
			else{
				if((fabs(t[i].x - (int)t[i].x - 0.5) > 1 / 4.2 || fabs(t[i].y - (int)t[i].y - 0.5) > 1 / 2.24))
					for (int j = 0; j < 2; j++)
						for (int k = 0; k < 2; k++)
							for (int l = 0; l < 4; l++)
								if (CarIntersect[(int)(t[i].y - 1 + j)][(int)(t[i].x - 1 + k)][l] == i + 1)
									CarIntersect[(int)(t[i].y - 1 + j)][(int)(t[i].x - 1 + k)][l] = 0;
			}

			//close to intersection, stop to check if there is a car
			switch (t[i].angle % 360) {
				case 0:
					if (t[i].i != t[i].length&&!equalIntersect(CarIntersect[(int)(t[i].y)][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i+1], 0) && (t[i].y + 0.5-(int)(t[i].y + 0.5)) < 1 / 3.) {
	//					printf("%d %d %lf %lf %d 01\n", CarIntersect[(int)(t[i].y)][(int)t[i].x][(t[i].path[t[i].i] + 2) % 4]-1, i, t[i].x, t[i].y, t[i].angle);
						continue;
					}
					else {
						t[i].y -= (t[i].velocity);
						t[i].x = (int)t[i].x + 0.5;
					}
					break;
				case 90:
					if (t[i].i != t[i].length&&!equalIntersect(CarIntersect[(int)(t[i].y)][(int)(t[i].x)], t[i].path[t[i].i], t[i].path[t[i].i + 1], 0)&& (t[i].x + 0.5 - (int)(t[i].x + 0.5) )>  7/ 9.) {
	//					printf("%d %d %lf %lf %d 02\n", CarIntersect[(int)(t[i].y)][(int)t[i].x][(t[i].path[t[i].i] + 2) % 4]-1, i, t[i].x, t[i].y, t[i].angle);
						continue;
					}
					else {
						t[i].x += (t[i].velocity);
						t[i].y = (int)t[i].y + 0.5;
					}
					break;
				case 180:
					if (t[i].i != t[i].length&&!equalIntersect(CarIntersect[(int)(t[i].y)][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], 0) && (t[i].y + 0.5 - (int)(t[i].y + 0.5)) > 2 / 3.) {
	//					printf("%d %d %lf %lf %d 03\n", CarIntersect[(int)(t[i].y)][(int)t[i].x][(t[i].path[t[i].i]+2) % 4]-1, i, t[i].x, t[i].y, t[i].angle);
						continue;
					}
					else {
						t[i].y += (t[i].velocity);
						t[i].x = (int)t[i].x + 0.5;
					}
					break;
				case 270:
					if (t[i].i != t[i].length&&!equalIntersect(CarIntersect[(int)(t[i].y)][(int)(t[i].x)], t[i].path[t[i].i], t[i].path[t[i].i + 1], 0) && (t[i].x + 0.5 - (int)(t[i].x + 0.5)) < 2 / 9.) {
	//					printf("%d %d %lf %lf %d 04\n", CarIntersect[(int)(t[i].y)][(int)t[i].x][(t[i].path[t[i].i] + 2) % 4]-1, i, t[i].x, t[i].y, t[i].angle);
						continue;
					}
					else {
						t[i].x -= (t[i].velocity);
						t[i].y = (int)t[i].y + 0.5;
					}
					break;
			}

			//the end of the path, stop at the middle of the road
			if (t[i].i == t[i].length) {
				if (t[i].angle == 90 || t[i].angle == 270) {
					if (fabs(t[i].x - (int)t[i].x) < 2 * t[i].velocity)
						if (t[i].type < 5)
							t[i].velocity = -2;
						else 
							t[i].velocity = -1;
				}
				else if (t[i].angle == 0 || t[i].angle == 180) {
					if (fabs(t[i].y - (int)t[i].y) < 2 * t[i].velocity)
						if (t[i].type < 5)
							t[i].velocity = -2;
						else
							t[i].velocity = -1;
				}
				for (int j = 0; j < hnum; j++)
					for (int k = 0; k < wnum; k++)
						for (int l = 0; l < 4; l++)
							if (CarIntersect[j][k][l] == i + 1)
								CarIntersect[j][k][l] = 0;

			}
	//		printf("%d %lf %lf %d 05\n", i, t[i].x, t[i].y, t[i].angle);
			continue;
		}
		else if (t[i].path[t[i].i] == t[i].path[t[i].i + 1]) {		//go straight
			if (t[i].path[t[i].i + 1] < 0) {		//special stop
				t[i].velocity = -1;
				for (int j = 0; j < hnum; j++)
					for (int k = 0; k < wnum; k++)
						for(int l=0; l<4; l++)
							if (CarIntersect[j][k][l] == i + 1)
								CarIntersect[j][k][l] = 0;
				continue;
			}

			t[i].intersect = true;
			//check if there is car in the intersect
			if (!equalIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], (i + 1))) {
				if (equalIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], 0))
					fillIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], (i + 1));
				else {
	//				printf("%d %d %lf %lf %d 11\n", CarIntersect[(int)(t[i].y)][(int)t[i].x][(t[i].path[t[i].i] + 2) % 4] - 1, i, t[i].x, t[i].y, t[i].angle);
					continue;
				}
			}
			
			switch (t[i].angle % 360) {
				case 0:
					t[i].y -= (t[i].velocity);
					break;
				case 90:
					t[i].x += (t[i].velocity);
					break;
				case 180:
					t[i].y += (t[i].velocity);
					break;
				case 270:
					t[i].x -= (t[i].velocity);
					break;
			}
	//		printf("%d %lf %lf %d 12\n", i, t[i].x, t[i].y, t[i].angle);
			continue;
		}
		else
		{
			if (t[i].path[t[i].i + 1] < 0) {		//special stop
				t[i].velocity = -1;
				for (int j = 0; j < hnum; j++)
					for (int k = 0; k < wnum; k++)
						for (int l = 0; l < 4; l++)
							if (CarIntersect[j][k][l] == i + 1)
								CarIntersect[j][k][l] = 0;
				continue;
			}

			t[i].intersect = true;
			//check if there is car in the intersect
			if (!equalIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], (i + 1))) {
				if (equalIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], 0))
					fillIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], (i + 1));
				else {
	//				printf("%d %d %lf %lf %d 21\n", CarIntersect[(int)(t[i].y)][(int)t[i].x][(t[i].path[t[i].i] + 2) % 4] - 1, i, t[i].x, t[i].y, t[i].angle);
					continue;
				}
			}

			t[i].x = (int)t[i].x + 0.5;
			t[i].y = (int)t[i].y + 0.5;
			if ((t[i].path[t[i].i] + 3) % 4 == t[i].path[t[i].i + 1]) {   //turn right
				(t[i].angle) = (int)(t[i].angle + 5*t[i].velocity/0.05+ 360) % 360;
				switch (t[i].path[t[i].i]) {
					case 0:			//270-360 sin<0, cos>0
						t[i].x -= (1. / 10) * sin(t[i].angle * M_PI / 180);
						t[i].y -= (1. / 6) * cos(t[i].angle * M_PI / 180);
						break;
					case 1:			//180-270 sin<0 cos<0
						t[i].x += (1. / 10) * sin(t[i].angle * M_PI / 180);
						t[i].y += (1. / 6) * cos(t[i].angle * M_PI / 180);
						break;
					case 2:			//90-180 sin>0 cos<0
						t[i].x -= (1. / 10) * sin(t[i].angle * M_PI / 180);
						t[i].y -= (1. / 6) * cos(t[i].angle * M_PI / 180);
						break;
					case 3:			//0-90 sin>0 cos>0
						t[i].x += (1. / 10) * sin(t[i].angle * M_PI / 180);
						t[i].y += (1. / 6) * cos(t[i].angle * M_PI / 180);
						break;
				}

			}
			else if ((t[i].path[t[i].i] + 1) % 4 == t[i].path[t[i].i + 1]) {  //turn left
				(t[i].angle) = (int)(t[i].angle - 5 * t[i].velocity / 0.05 + 360) % 360;
				switch (t[i].path[t[i].i]) {
					case 0:			//270-180 sin<0, cos<0
						t[i].x -= (1. / 10) * sin(t[i].angle * M_PI / 180);
						t[i].y -= (1. / 6) * cos(t[i].angle * M_PI / 180);
						break;
					case 1:			//180-90 sin>0 cos<0
						t[i].x += (1. / 10) * sin(t[i].angle * M_PI / 180);
						t[i].y += (1. / 6) * cos(t[i].angle * M_PI / 180);
						break;
					case 2:			//90-0 sin>0 cos>0
						t[i].x -= (1. / 10) * sin(t[i].angle * M_PI / 180);
						t[i].y -= (1. / 6) * cos(t[i].angle * M_PI / 180);
						break;
					case 3:			//0-270 sin<0 cos>0
						t[i].x += (1. / 10) * sin(t[i].angle * M_PI / 180);
						t[i].y += (1. / 6) * cos(t[i].angle * M_PI / 180);
						break;
				}
			}
	//		printf("%d %lf %lf %d 22\n", i, t[i].x, t[i].y, t[i].angle);
			continue;
		}
	}
	//printf("==========\n\n");
	/*
	for (int j = 0; j < hnum; j++) {
		for (int k = 0; k < wnum; k++) {
			for (int l = 0; l < 4; l++) {
				printf("%3d ", CarIntersect[j][k][l]);
				int temp;
				if (temp = CarIntersect[j][k][l])
					printf("\n%d %d %d\n", t[temp - 1].angle, t[temp - 1].path[t[temp - 1].i], t[temp - 1].path[t[temp - 1].i+1]);
			}
		}
		printf("\n-------\n");
	}
	printf("\n-------------\n");
	*/
	return interval;
} 

char* createPath(const WindowData& fullViewport, double start_y, double start_x, bool verticle, bool intersect, int house_hnum, int house_wnum, int &length) {
	int wnum = fullViewport.wnum, hnum = fullViewport.hnum;
	
	//create diatance map
	double** distance = new double* [hnum];
	double min_dis = (double)INT_MAX;
	int min_disp=0;
	for (int i = 0; i < hnum; i++) {
		distance[i] = new double[wnum];
		for (int j = 0; j < wnum; j++)
			distance[i][j] = 1e8;
	}
	
	//start on the verticle road
	if (intersect) {
		distance[(int)(start_y)][(int)start_x] = 0;
		bestRoute(fullViewport, distance, (int)(start_y), (int)(start_x), 4);
	}
	else if (verticle) {
		if (existDirection(hnum, wnum, (int)(start_y-0.5), (int)start_x, 1)) {
			distance[(int)(start_y - 0.5)][(int)start_x] = start_y-0.5-(int)(start_y-0.5);
			distance[(int)(start_y + 0.5)][(int)start_x] = (int)(start_y + 0.5)-start_y + 0.5;
			bestRoute(fullViewport, distance, (int)(start_y - 0.5), (int)start_x, 3);
			bestRoute(fullViewport, distance, (int)(start_y + 0.5), (int)start_x, 1);
		}
	}
	//start on the horizontal road
	else {
		if (existDirection(hnum, wnum, (int)(start_y), (int)(start_x-0.5), 2)) {
			distance[(int)(start_y)][(int)(start_x-0.5)] = start_x - 0.5 - (int)(start_x - 0.5);
			distance[(int)(start_y)][(int)(start_x+0.5)] = (int)(start_x + 0.5) - start_x + 0.5;
			bestRoute(fullViewport, distance, (int)(start_y), (int)(start_x - 0.5), 0);
			bestRoute(fullViewport, distance, (int)(start_y), (int)(start_x + 0.5), 2);
		}
	}
	//find the shortest corner to the destination
	//top road
	if ((checkRoad(house_hnum, house_wnum) >> 3) % 2) {		
		if (min_dis > distance[house_hnum][house_wnum]) {
			min_dis = distance[house_hnum][house_wnum];
			min_disp = 3;
		}
		if (min_dis > distance[house_hnum][house_wnum + 1]) {
			min_dis = distance[house_hnum][house_wnum+1];
			min_disp = 3;
		}
	}
	//right road
	if ((checkRoad(house_hnum, house_wnum) >> 2)%2) {		
		if (min_dis > distance[house_hnum][house_wnum+1]) {
			min_dis = distance[house_hnum][house_wnum+1];
			min_disp = 2;
		}
		if (min_dis > distance[house_hnum+1][house_wnum + 1]) {
			min_dis = distance[house_hnum+1][house_wnum + 1];
			min_disp = 2;
		}
	}
	//bottom road
	if ((checkRoad(house_hnum, house_wnum) >> 1)%2) {		
		if (min_dis > distance[house_hnum+1][house_wnum]) {
			min_dis = distance[house_hnum+1][house_wnum];
			min_disp = 1;
		}
		if (min_dis > distance[house_hnum+1][house_wnum + 1]) {
			min_dis = distance[house_hnum+1][house_wnum + 1];
			min_disp = 1;
		}
	}
	//left road
	if (checkRoad(house_hnum, house_wnum)%2) {				
		if (min_dis > distance[house_hnum][house_wnum]) {
			min_dis = distance[house_hnum][house_wnum];
			min_disp = 0;
		}
		if (min_dis > distance[house_hnum+1][house_wnum]) {
			min_dis = distance[house_hnum+1][house_wnum];
			min_disp = 0;
		}
	}

	//retrace the shortest path(from destination to the starting point)
	length = (int)(min_dis+1);
	char* path=new char [length+1];
	int y, x;
	//find the direction from destination point to house and initialize the starting point of retracing
	switch (min_disp) {
		case 3:
			y = house_hnum;
			if (distance[house_hnum][house_wnum] < distance[house_hnum][house_wnum + 1]) {
				x = house_wnum;
				path[length] = 2;
			}
			else {
				x = house_wnum + 1;
				path[length] = 0;
			}
			break;
		case 2:
			x = house_wnum+1;
			if (distance[house_hnum][house_wnum+1] < distance[house_hnum+1][house_wnum + 1]) {
				y = house_hnum;
				path[length] = 1;
			}
			else {
				y = house_hnum + 1;
				path[length] = 3;
			}
			break;
		case 1:
			y = house_hnum+1;
			if (distance[house_hnum+1][house_wnum] < distance[house_hnum+1][house_wnum + 1]) {
				x = house_wnum;
				path[length] = 2;
			}
			else {
				x = house_wnum + 1;
				path[length] = 0;
			}
			break;
		case 0:
			x = house_wnum;
			if (distance[house_hnum][house_wnum] < distance[house_hnum + 1][house_wnum]) {
				y = house_hnum;
				path[length] = 1;
			}
			else {
				y = house_hnum + 1;
				path[length] = 3;
			}
			break;
	}
	for (int i = length - 1; i > 0; i--) {
		if (x > 0) {
			if ((distance[y][x - 1] == distance[y][x] - 1) && existDirection(hnum, wnum, y, x-1, 2)) {
				x -= 1;
				path[i] = 2;
				continue;
			}
		}
		if(y<hnum-1){
			if ((distance[y + 1][x] == distance[y][x] - 1)&&existDirection(hnum, wnum, y+1, x, 3)) {
				y += 1;
				path[i] = 3;
				continue;
			}
		}
		if(x<wnum-1){
			if ((distance[y][x + 1] == distance[y][x] - 1)&& existDirection(hnum, wnum, y, x+1, 0)) {
				x += 1;
				path[i] = 0;
				continue;
			}
		}
		if(y>0){
			if ((distance[y - 1][x] == distance[y][x] - 1)&& existDirection(hnum, wnum, y-1, x, 1)) {
				y -= 1;
				path[i] = 1;
				continue;
			}
		}
	}
	//determine the direction from house to the closet corner 
	if (intersect) {
		path[0] = path[1];
	}
	else if (verticle) {
		if (x == (int)(start_x) && y == (int)(start_y-0.5))
			path[0] = 3;
		else
			path[0] = 1;
	}
	else {
		if (x == (int)(start_x-0.5) && y == (int)(start_y))
			path[0] = 0;
		else
			path[0] = 2;
	}
	/*for (int i = 0; i < hnum; i++) {
		for (int j = 0; j < wnum; j++)
			printf("%lg ", distance[i][j]);
		printf("\n");
	}
	printf("\n");*/
	for (int i = 0; i < hnum; i++)
		delete[]distance[i];
	delete[]distance;
	return path;
}

void bestRoute(const WindowData& fullViewport, double **distance, int y, int x, int direction) {
	int wnum = fullViewport.wnum, hnum = fullViewport.hnum;
	if (direction != 1 && existDirection(hnum, wnum, y, x, 3)) {		//go up
		if (distance[y - 1][x] > distance[y][x]) {
			distance[y - 1][x] = distance[y][x] + 1;
			bestRoute(fullViewport, distance, y - 1, x, 3);
		}
	}
	if (direction != 3 && existDirection(hnum, wnum, y, x, 1)) {	//go down
		if (distance[y + 1][x] > distance[y][x]) {
			distance[y + 1][x] = distance[y][x] + 1;
			bestRoute(fullViewport, distance, y + 1, x, 1);
		}
	}
	if (direction != 2 && existDirection(hnum, wnum, y, x, 0)) {  //go left
		if (distance[y][x - 1] > distance[y][x]) {
			distance[y][x - 1] = distance[y][x] + 1;
			bestRoute(fullViewport, distance, y, x - 1, 0);
		}
	}
	if (direction != 0 && existDirection(hnum, wnum, y, x, 2)) {  //go right
		if (distance[y][x + 1] > distance[y][x]) {
			distance[y][x + 1] = distance[y][x] + 1;
			bestRoute(fullViewport, distance, y, x + 1, 2);
		}
	}
}

bool existDirection(int hnum, int wnum, int y, int x, int direction) {
	if (y == 0 && direction == 3) //go up
		return false;
	if (x == wnum && direction == 2)	//go right
		return false;
	if (y == hnum && direction == 1)  //go down
		return false;
	if (x == 0 && direction == 0)	//go left
		return false;
	if ((road[y][x] >> (direction * 4)) % 2)
		return true;
	return false;
}

void fillIntersect(char* intersect, int direction1, int direction2, int num) {
//	printf("%d", num);
	if (direction1 == direction2) {
//		printf("??\n");
		intersect[(direction1 + 3) % 4] = num;
		intersect[(direction1 + 2) % 4] = num;
	}
	else if ((direction1 + 3) % 4 == direction2) {
//		printf("!!\n");
		intersect[(direction1 + 2) % 4] = num;
	}
	else {
//		printf(">>\n");
		intersect[direction1] = num;
		intersect[(direction1 + 3) % 4] = num;
		intersect[(direction1 + 2) % 4] = num;
	}
}

bool equalIntersect(char* intersect, int direction1, int direction2, int num) {
	if (direction1 < 0 || direction2 < 0)
		return true;
	if (direction1 == direction2)
		if ((intersect[(direction1 + 3) % 4]!=num || intersect[(direction1 + 2) % 4]!=num))
			return false;
		else
			return true;
	else if ((direction1 + 3) % 4 == direction2)
		if (intersect[(direction1 + 2) % 4]!=num)
			return false;
		else
			return true;
	else if ((direction1 + 1) % 4 == direction2)
		if (intersect[direction1]!=num || intersect[(direction1 + 3) % 4]!=num || intersect[(direction1 + 2) % 4]!=num)
			return false;
		else
			return true;
	else
		if (intersect[direction1]!=num || intersect[(direction1 + 3) % 4]!=num || intersect[(direction1 + 2) % 4]!=num || intersect[(direction1 + 1) % 4]!=num)
			return false;
		else
			return true;
}