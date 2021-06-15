#include<limits.h>


int carMenu(SDL_Renderer* renderer, const WindowData& fullViewport, MouseState mouseState, int mousex, int mousey, int& choose, ImageData car_pic[]);
void bestRoute(const WindowData& fullViewport, int** distance, int y, int x, int direction);
bool existDirection(int hnum, int wnum, int y, int x, int direction);
char* createPath(const WindowData& fullViewport, int start_xnum, int start_ynum, bool verticle, int house_hnum, int house_wnum, int& length);
void fillIntersect(char* intersect, int direction1, int direction2, int num);
bool equalIntersect(char* intersect, int direction1, int direction2, int num);

void createCar(CarData& car, WindowData& fullViewport, int start_ynum, int start_xnum, int house_hnum, int house_wnum, int type, ImageData car_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, wnum = fullViewport.wnum, hnum = fullViewport.hnum, oldw=fullViewport.oldw, oldh=fullViewport.oldh;

	char* path;
	car.length = INT_MAX;
	int choose, length;

	//Run for four times to choose the shortest path
	if ((checkRoad(start_ynum, start_xnum) >> 3) % 2) {		//top road
		path=createPath(fullViewport, start_ynum, start_xnum, false, house_hnum, house_wnum, length);
		if (car.length > length) {
			car.length = length;
			car.path = path;
			choose = 3;
		}
		else
			delete[]path;
	}
	if ((checkRoad(start_ynum, start_xnum) >> 2) % 2) {		//right road
		path = createPath(fullViewport, start_ynum, start_xnum+1, true, house_hnum, house_wnum, length);
		if (car.length > length) {
			car.length = length;
			car.path = path;
			choose = 2;
		}
		else
			delete[]path;
	}
	if ((checkRoad(start_ynum, start_xnum) >> 1) % 2) {		//bottom road
		path = createPath(fullViewport, start_ynum+1, start_xnum, false, house_hnum, house_wnum, length);
		if (car.length > length) {
			car.length = length;
			car.path = path;
			choose = 1;
		}
		else
			delete[]path;
	}
	if ((checkRoad(start_ynum, start_xnum)) % 2) {		//left road
		path = createPath(fullViewport, start_ynum, start_xnum, true, house_hnum, house_wnum, length);
		if (car.length > length) {
			car.length = length;
			car.path = path;
			choose = 0;
		}
		else
			delete[]path;
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
	car.img = &(car_pic[type]);
	car.i = 0;
	car.angle = (3-car.path[0])*90;
	car.velocity = 0.01;
	car.intersect = false;
}

int addCar(SDL_Renderer* renderer, WindowData& fullViewport, MouseState mouseState, int mousex, int mousey, Building** build , CarData car[], ImageData car_pic[]) {
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
				else if (build[ynum][xnum].type == House||Factory) {
					CarData t;
					car[choose-1].length = INT_MAX;
					if (!car[choose - 1].path)
						delete[](car[choose - 1].path);
					if (choose == 1 || choose == 2) {
						for (int i = 0; i < hnum-1; i++)
							for (int j = 0; j < wnum-1; j++)
								if (build[i][j].type == FireSta) {
									createCar(t, fullViewport, i, j, ynum, xnum, choose + 2, car_pic);
									if (t.length < car[choose - 1].length)
										car[choose - 1] = t;
									choose = 0;
								}
					}
					else if (choose == 3) {
						for (int i = 0; i < hnum-1; i++)
							for (int j = 0; j < wnum-1; j++)
								if (build[i][j].type == Logistics) {
									createCar(t, fullViewport, i, j, ynum, xnum, 5, car_pic);
									if (t.length < car[choose - 1].length)
										car[choose - 1] = t;
									choose = 0;
								}
					}
					else {
						for (int i = 0; i < hnum-1; i++)
							for (int j = 0; j < wnum-1; j++)
								if (build[i][j].type == PoliceOff) {
									createCar(t, fullViewport, i, j, ynum, xnum, 7, car_pic);
									if (t.length < car[choose - 1].length)
										car[choose - 1] = t;
									choose = 0;
								}
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
			imgRender(renderer, car_pic[3], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 230);
			break;
		case 2:
			imgRender(renderer, car_pic[4], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 230);
			break;
		case 3:
			imgRender(renderer, car_pic[5], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 230);
			break;
		case 4:
			imgRender(renderer, car_pic[7], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 230);
			break;
		}
	}

	return 0;
}

int carMenu(SDL_Renderer* renderer, const WindowData& fullViewport, MouseState mouseState, int mousex, int mousey, int& choose, ImageData car_pic[]) {
	int width = fullViewport.w, height = fullViewport.h;
	boxColor(renderer, 0, height / 12, width - height/ 12, height, 0xBB000000);
	imgRender(renderer, car_pic[3], RightBottom, width / 2 - 10, height / 2 - 10, NULL, height / 3, 1, NULL, NULL, 0, no, 250);
	imgRender(renderer, car_pic[4], LeftBottom, width / 2 + 10, height / 2 - 10, NULL, height / 3, 1, NULL, NULL, 0, no, 250);
	imgRender(renderer, car_pic[5], RightTop, width / 2 - 10, height / 2 + 10, NULL, height / 3, 1, NULL, NULL, 0, no, 250);
	imgRender(renderer, car_pic[7], LeftTop, width / 2 + 10, height / 2 + 10, NULL, height / 3, 1, NULL, NULL, 0, no, 250);

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

void carRender(SDL_Renderer* renderer, const WindowData& fullViewport, CarData car[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum, wnum = fullViewport.wnum;
	for (int i = 0; i < 4 + RANDCARNUM; i++) {
		if (car[i].velocity == -1) {
			if (!car[i].path) {
				delete[]car[i].path;
				car[i].path = NULL;
			}
			continue;
		}
		imgRender(renderer, *(car[i].img), Left, car[i].x * (width - height / 12) / wnum, car[i].y * (height * 11 / 12) / hnum + (height / 12), fmin((width - height / 12) / wnum / 10., height * 11 / 12 / hnum / 6.), NULL, 1, NULL, NULL, car[i].angle, no, 255);
	}
}

Uint32 car_move(Uint32 interval, void* param)
{
	CarData *t = (CarData*)param;
	int width = t[4].window->w, height = t[4].window->h, wnum = t[4].window->wnum, hnum = t[4].window->hnum;
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

	for (int i = 0; i < RANDCARNUM + 4; i++) {
		if (t[i].x<0.45 || t[i].x>wnum - 0.45 || t[i].y<0.45 || t[i].y>hnum - 0.45) {
			t[i].velocity = -1;
			for (int j = 0; j < hnum; j++)
				for (int k = 0; k < wnum; k++)
					for (int l = 0; l < 4; l++)
						if (CarIntersect[j][k][l] == i + 1)
							CarIntersect[j][k][l] = 0;
		}
		//printf("%d\n", i);
		//If velocity =-1, car disappear
		if (t[i].velocity == -1) {
			if(i<4)
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
						createRandomCar(t[i], *(t[i].window), start, *(t[i].img));
					}
					if(t[i].path)
						fillIntersect(CarIntersect[start.y][start.x], t[i].path[0], t[i].path[0], i + 1);
				}
			}
		}

		
		//stop when there is a car in front of mycar(by direction) 
		bool stop = false;
		for (int k = 0; k < RANDCARNUM + 4; k++) {
			t[i].angle %= 360;
			if (i == k || t[k].velocity < 0)
				continue;
			int length;
			if (k == 2||i==2) //truck
				length = 3;
			else
				length = 2;
			if (t[i].angle == t[k].angle&&(t[i].angle%90==0)) {
				if (t[i].x == t[k].x && t[i].y == t[k].y)
					if (i < k)
						stop = false;
					else
						stop = true;
				if (((width - height / 12) / wnum / 10.) < (height * 11 / 12 / hnum / 6.)) {
					double xlength=1 / 10. * ((width - height / 12) / wnum) / (height * 11 / 12 / hnum);
					switch (t[i].angle) {
						case 0:
							if (t[k].x == t[i].x && (t[i].y - t[k].y) > 0 && (t[i].y - t[k].y) < length / 6.)
								stop = true;
							break;
						case 90:
							if (t[k].y == t[i].y &&(t[k].x - t[i].x) > 0 && (t[k].x - t[i].x) < length*xlength)
								stop = true;
							break;
						case 180:
							if (t[k].x == t[i].x && (t[k].y - t[i].y )> 0 &&( t[k].y - t[i].y) < length / 6.)
								stop = true;
							break;
						case 270:
							if (t[k].y == t[k].y && (t[i].x - t[k].x) > 0 && (t[i].x - t[k].x) < length*xlength)
								stop = true;
							break;

					}
				}
				else {
					
					double ylength = 1 / 6. / ((width - height / 12) / wnum) * (height * 11 / 12 / hnum);
					switch (t[i].angle) {
						case 0:
							if (t[k].x == t[i].x && (t[i].y - t[k].y) > 0 && (t[i].y - t[k].y) < length*ylength)
								stop = true;
							break;
						case 90:
							if (t[k].y == t[i].y && (t[k].x - t[i].x) > 0 && (t[k].x - t[i].x) < length/10.)
								stop = true;
							break;
						case 180:
							if (t[k].x == t[i].x && (t[k].y - t[i].y) > 0 && (t[k].y - t[i].y) < length / 6.)
								stop = true;
							break;
						case 270:
							if (t[k].y == t[k].y && (t[i].x - t[k].x) > 0 && (t[i].x - t[k].x) < length*ylength)
								stop = true;
							break;
					}
				}
			}
		}

		if (stop)
			continue;

		if (!t[i].path)
			continue;

		t[i].angle %= 360;
	//	printf("%lf\n", t[i].velocity);
		//If not into the intersect or the angle has meet the next direction(If not go straight)
		if (!checkIntersect(*(t[i].window), road, t[i].y, t[i].x)||((t[i].i < t[i].length) && ((t[i].path[t[i].i] != t[i].path[t[i].i + 1] && (t[i].angle == (3 - t[i].path[t[i].i + 1]) * 90))))) {
			if (t[i].intersect) {
				(t[i].i)++;
			}
			t[i].intersect = false;
	//		printf("1");
			//Leave the interscetion, free the intersection
			if (i == 2&& (fabs(t[i].x - (int)t[i].x - 0.5) > 2 / 8. || fabs(t[i].y - (int)t[i].y - 0.5) >= 1 / 2.2)) {	//for truck
				for (int j = 0; j < hnum; j++)
					for (int k = 0; k < wnum; k++)
						for (int l = 0; l < 4; l++)
							if (CarIntersect[j][k][l] == i + 1)
								CarIntersect[j][k][l] = 0;
			}
			else{
				if((fabs(t[i].x - (int)t[i].x - 0.5) > 1 / 8. || fabs(t[i].y - (int)t[i].y - 0.5) > 1 / 3.))
					for (int j = 0; j < hnum; j++)
						for (int k = 0; k < wnum; k++)
							for (int l = 0; l < 4; l++)
								if (CarIntersect[j][k][l] == i + 1)
									CarIntersect[j][k][l] = 0;
			}

			//close to intersection, stop to check if there is a car
			switch (t[i].angle % 360) {
				case 0:
					if (t[i].i != t[i].length&&!equalIntersect(CarIntersect[(int)(t[i].y - 0.5)][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i+1], 0) && (t[i].y + 0.5-(int)(t[i].y + 0.5)) < 1 / 4.) {
						continue;
					}
					else {
						t[i].y -= (t[i].velocity);
						t[i].x = (int)t[i].x + 0.5;
					}
					break;
				case 90:
					if (t[i].i != t[i].length&&!equalIntersect(CarIntersect[(int)(t[i].y)][(int)(t[i].x+0.5)], t[i].path[t[i].i], t[i].path[t[i].i + 1], 0)&& (t[i].x + 0.5 - (int)(t[i].x + 0.5) )>  8/ 9.) {
						continue;
					}
					else {
						t[i].x += (t[i].velocity);
						t[i].y = (int)t[i].y + 0.5;
					}
					break;
				case 180:
					if (t[i].i != t[i].length&&!equalIntersect(CarIntersect[(int)(t[i].y + 0.5)][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], 0) && (t[i].y + 0.5 - (int)(t[i].y + 0.5)) > 3 / 4.) {
						continue;
					}
					else {
						t[i].y += (t[i].velocity);
						t[i].x = (int)t[i].x + 0.5;
					}
					break;
				case 270:
					if (t[i].i != t[i].length&&!equalIntersect(CarIntersect[(int)(t[i].y)][(int)(t[i].x-0.5)], t[i].path[t[i].i], t[i].path[t[i].i + 1], 0) && (t[i].x + 0.5 - (int)(t[i].x + 0.5)) < 1 / 9.) {
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
						if (i < 4)
							t[i].velocity = -1;
						else
							t[i].velocity = -1;
				}
				else if (t[i].angle == 0 || t[i].angle == 180) {
					if (fabs(t[i].y - (int)t[i].y) < 2 * t[i].velocity)
						if (i < 4)
							t[i].velocity = -1;
						else
							t[i].velocity = -1;
				}
			}
			continue;
		}
		else if (t[i].path[t[i].i] == t[i].path[t[i].i + 1]) {		//go straight
	//		printf("3");
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
			if (!equalIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], (i + 1))) {
				if (equalIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], 0))
					fillIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], (i + 1));
				else
					continue;
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
			continue;
		}
		else
		{
	//		printf("4");
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
			if (!equalIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], (i + 1))) {
				if (equalIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], 0))
					fillIntersect(CarIntersect[(int)t[i].y][(int)t[i].x], t[i].path[t[i].i], t[i].path[t[i].i + 1], (i + 1));
				else
					continue;
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
			//		printf("%d ", t[i].angle);
			//		printf("%lf %lf\n", t[i].x, t[i].y);
			continue;
		}
	}
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

char* createPath(const WindowData& fullViewport, int start_ynum,int start_xnum, bool verticle, int house_hnum, int house_wnum, int &length) {
	int wnum = fullViewport.wnum, hnum = fullViewport.hnum;
	
	//create diatance map
	int** distance = new int* [hnum];
	int min_dis[2] = { INT_MAX, 0 };
	for (int i = 0; i < hnum; i++) {
		distance[i] = new int[wnum];
		for (int j = 0; j < wnum; j++)
			distance[i][j] = INT_MAX;
	}

	//start on the verticle road
	if (verticle) {
		if (existDirection(hnum, wnum, start_ynum, start_xnum, 1)) {
			distance[start_ynum][start_xnum] = 1;
			distance[start_ynum + 1][start_xnum] = 1;
			bestRoute(fullViewport, distance, start_ynum, start_xnum, 3);
			bestRoute(fullViewport, distance, start_ynum + 1, start_xnum, 1);
		}
	}
	//start on the horizontal road
	else {
		if (existDirection(hnum, wnum, start_ynum, start_xnum, 2)) {
			distance[start_ynum][start_xnum] = 1;
			distance[start_ynum][start_xnum + 1] = 1;
			bestRoute(fullViewport, distance, start_ynum, start_xnum, 0);
			bestRoute(fullViewport, distance, start_ynum, start_xnum + 1, 2);
		}
	}
	//find the shortest corner to the destination
	//top road
	if ((checkRoad(house_hnum, house_wnum) >> 3) % 2) {		
		if (min_dis[0] > distance[house_hnum][house_wnum]) {
			min_dis[0] = distance[house_hnum][house_wnum];
			min_dis[1] = 3;
		}
		if (min_dis[0] > distance[house_hnum][house_wnum + 1]) {
			min_dis[0] = distance[house_hnum][house_wnum+1];
			min_dis[1] = 3;
		}
	}
	//right road
	if ((checkRoad(house_hnum, house_wnum) >> 2)%2) {		
		if (min_dis[0] > distance[house_hnum][house_wnum+1]) {
			min_dis[0] = distance[house_hnum][house_wnum+1];
			min_dis[1] = 2;
		}
		if (min_dis[0] > distance[house_hnum+1][house_wnum + 1]) {
			min_dis[0] = distance[house_hnum+1][house_wnum + 1];
			min_dis[1] = 2;
		}
	}
	//bottom road
	if ((checkRoad(house_hnum, house_wnum) >> 1)%2) {		
		if (min_dis[0] > distance[house_hnum+1][house_wnum]) {
			min_dis[0] = distance[house_hnum+1][house_wnum];
			min_dis[1] = 1;
		}
		if (min_dis[0] > distance[house_hnum+1][house_wnum + 1]) {
			min_dis[0] = distance[house_hnum+1][house_wnum + 1];
			min_dis[1] = 1;
		}
	}
	//left road
	if (checkRoad(house_hnum, house_wnum)%2) {				
		if (min_dis[0] > distance[house_hnum][house_wnum]) {
			min_dis[0] = distance[house_hnum][house_wnum];
			min_dis[1] = 0;
		}
		if (min_dis[0] > distance[house_hnum+1][house_wnum]) {
			min_dis[0] = distance[house_hnum+1][house_wnum];
			min_dis[1] = 0;
		}
	}

	//retrace the shortest path(from destination to the starting point)
	length = min_dis[0];
	char* path=new char [min_dis[0]+1];
	int y, x;
	//find the direction from destination point to house and initialize the starting point of retracing
	switch (min_dis[1]) {
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
	if (verticle) {
		if (x == start_xnum && y == start_ynum)
			path[0] = 3;
		else
			path[0] = 1;
	}
	else {
		if (x == start_xnum && y == start_ynum)
			path[0] = 0;
		else
			path[0] = 2;
	}
	for (int i = 0; i < hnum; i++)
		delete[]distance[i];
	delete[]distance;
	return path;
}

void bestRoute(const WindowData& fullViewport, int **distance, int y, int x, int direction) {
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