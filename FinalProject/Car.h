#include<limits.h>
struct CarData
{
	double x, y;	//x:0.5~(wnum-0.5), y:0.5~(hnum-0.5)
	WindowData* window;
	ImageData* img;
	int i;	//start from 0 to length-1
	char* path;  //up: 3, right: 2, down: 1, left: 0
	int length;
	int angle;
	double velocity;  //-1:no car
	bool intersect;
};

int carMenu(SDL_Renderer* renderer, WindowData fullViewport, MouseState mouseState, int mousex, int mousey, int& choose, ImageData car_pic[]);
void bestRoute(WindowData& fullViewport, int** distance, int y, int x, int direction);
bool existDirection(int hnum, int wnum, int y, int x, int direction);
char* createPath(WindowData& fullViewport, int start_xnum, int start_ynum, bool verticle, int house_hnum, int house_wnum, int& length);

void createCar(CarData& car, WindowData& fullViewport, bool windowChange, int start_ynum, int start_xnum, int house_hnum, int house_wnum, int type, ImageData car_pic[]) {
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

int addCar(SDL_Renderer* renderer, WindowData fullViewport, MouseState mouseState, int mousex, int mousey, Building** build , CarData& car, ImageData car_pic[]) {
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
					car.length = INT_MAX;
					if (!car.path)
						delete[](car.path);
					if (choose == 1 || choose == 2) {
						for (int i = 0; i < hnum-1; i++)
							for (int j = 0; j < wnum-1; j++)
								if (build[i][j].type == FireSta) {
									createCar(t, fullViewport, false, i, j, ynum, xnum, choose + 2, car_pic);
									if (t.length < car.length)
										car = t;
									choose = 0;
								}
					}
					else if (choose == 3) {
						for (int i = 0; i < hnum-1; i++)
							for (int j = 0; j < wnum-1; j++)
								if (build[i][j].type == Logistics) {
									createCar(t, fullViewport, false, i, j, ynum, xnum, 5, car_pic);
									if (t.length < car.length)
										car = t;
									choose = 0;
								}
					}
					else {
						for (int i = 0; i < hnum-1; i++)
							for (int j = 0; j < wnum-1; j++)
								if (build[i][j].type == PoliceOff) {
									createCar(t, fullViewport, false, i, j, ynum, xnum, 7, car_pic);
									if (t.length < car.length)
										car = t;
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

	if (car.length == 0)
		printf("!");
	return 0;
}

int carMenu(SDL_Renderer* renderer, WindowData fullViewport, MouseState mouseState, int mousex, int mousey, int& choose, ImageData car_pic[]) {
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

void carRender(SDL_Renderer* renderer, WindowData fullViewport, CarData& car) {
	int width = fullViewport.w, height = fullViewport.h, hnum=fullViewport.hnum, wnum=fullViewport.wnum;
	if (car.velocity == -1||car.velocity==0)
		return;
	if (car.x<0.5 || car.x>wnum - 0.5 || car.y<0.5 || car.y>hnum - 0.5) {
		car.velocity = -1;
		return;
	}
	imgRender(renderer, *(car.img), Left, car.x*(width-height/12)/wnum, car.y*(height*11/12)/hnum+(height/12), fmin((width-height/12)/wnum/10., height*11/12/hnum/6.), NULL, 1, NULL, NULL, car.angle, no, 255);
}

Uint32 car_move(Uint32 interval, void* param)
{
	CarData* t = (CarData*)param;
	
	//If velocity =-1, car disappear
	if (t->velocity == -1)
		return interval;


	int width = t->window->w, height = t->window->h, wnum=t->window->wnum, hnum=t->window->hnum;
	t->angle %= 360;

	//If not into the intersect or the angle has meet the next direction(If not go straight)
	if (!checkIntersect(*(t->window), road, t->y, t->x)|| ((t->path[t->i] != t->path[t->i + 1] && (t->angle == (3 - t->path[t->i + 1]) * 90)))) {
		if (t->intersect) {
			(t->i)++;
			t->intersect = false;
		}
		t->intersect = false;
		switch (t->angle%360) {
			case 0:
				t->y -= (t->velocity);
				t->x = (int)t->x + 0.5;
				break;
			case 90:
				t->x += (t->velocity);
				t->y = (int)t->y + 0.5;
				break;
			case 180:
				t->y += (t->velocity);
				t->x = (int)t->x + 0.5;
				break;
			case 270:
				t->x -= (t->velocity);
				t->y = (int)t->y + 0.5;
				break;
		}

		//the end of the path, stop at the middle of the road
		if (t->i == t->length)
			if (t->angle == 90 || t->angle == 270) {
				if (fabs(t->x-(int)t->x)<2*t->velocity)
					t->velocity = 0;
			}
			else if (t->angle == 0 || t->angle == 180) {
				if (fabs(t->y - (int)t->y) < 2*t->velocity)
					t->velocity = 0;
			}
			
		return interval;
	}

	else if (t->path[t->i] == t->path[t->i+1]) {		//go straight

		t->intersect = true;
		switch (t->angle % 360) {
			case 0:
				t->y -= (t->velocity);
				break;
			case 90:
				t->x += (t->velocity);
			break;
			case 180:
				t->y += (t->velocity);
				break;
			case 270:
				t->x -= (t->velocity);
				break;
		}
		return interval;
	}
	else
	{
		if (t->path[t->i+1] < 0) {		//special stop
			t->velocity = -1;
		}

		t->intersect =true;
		t->x = (int)t->x+0.5;
		t->y = (int)t->y+0.5;
		if ((t->path[t->i] + 3) % 4 == t->path[t->i + 1]) {   //turn right
			(t->angle) = (t->angle + 5 +360)% 360;
			switch (t->path[t->i]) {
				case 0:			//270-360 sin<0, cos>0
					t->x -= (1. / 10) * sin(t->angle * M_PI / 180);
					t->y -= (1. / 6) * cos(t->angle * M_PI / 180);
					break;
				case 1:			//180-270 sin<0 cos<0
					t->x += (1. / 10) * sin(t->angle * M_PI / 180);
					t->y += (1. / 6) * cos(t->angle * M_PI / 180);
					break;
				case 2:			//90-180 sin>0 cos<0
					t->x -= (1. / 10) * sin(t->angle * M_PI / 180);
					t->y -= (1. / 6) * cos(t->angle * M_PI / 180);
					break;
				case 3:			//0-90 sin>0 cos>0
					t->x += (1. / 10) * sin(t->angle * M_PI / 180);
					t->y += (1. / 6) * cos(t->angle * M_PI / 180);
					break;
			}
			
		}
		else if ((t->path[t->i] + 1) % 4 == t->path[t->i + 1]) {  //turn left
			(t->angle) = (t->angle - 5 + 360) % 360;
			switch (t->path[t->i]) {
				case 0:			//270-180 sin<0, cos<0
					t->x -= (1. / 10) * sin(t->angle * M_PI / 180);
					t->y -= (1. / 6) * cos(t->angle * M_PI / 180);
					break;
				case 1:			//180-90 sin>0 cos<0
					t->x += (1. / 10) * sin(t->angle * M_PI / 180);
					t->y += (1. / 6) * cos(t->angle * M_PI / 180);
					break;
				case 2:			//90-0 sin>0 cos>0
					t->x -= (1. / 10) * sin(t->angle * M_PI / 180);
					t->y -= (1. / 6) * cos(t->angle * M_PI / 180);
					break;
				case 3:			//0-270 sin<0 cos>0
					t->x += (1. / 10) * sin(t->angle * M_PI / 180);
					t->y += (1. / 6) * cos(t->angle * M_PI / 180);
					break;
			}
		}
//		printf("%d ", t->angle);
//		printf("%lf %lf\n", t->x, t->y);
		return interval;
	}
} 

char* createPath(WindowData& fullViewport, int start_ynum,int start_xnum, bool verticle, int house_hnum, int house_wnum, int &length) {
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
		else
			printf("!");
	}
	//start on the horizontal road
	else {
		if (existDirection(hnum, wnum, start_ynum, start_xnum, 2)) {
			distance[start_ynum][start_xnum] = 1;
			distance[start_ynum][start_xnum + 1] = 1;
			bestRoute(fullViewport, distance, start_ynum, start_xnum, 0);
			bestRoute(fullViewport, distance, start_ynum, start_xnum + 1, 2);
		}
		else
			printf("?");
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
			if (distance[y][x - 1] == distance[y][x] - 1) {
				x -= 1;
				path[i] = 2;
				continue;
			}
		}
		if(y<hnum-1){
			if (distance[y + 1][x] == distance[y][x] - 1) {
				y += 1;
				path[i] = 3;
				continue;
			}
		}
		if(x<wnum-1){
			if (distance[y][x + 1] == distance[y][x] - 1) {
				x += 1;
				path[i] = 0;
				continue;
			}
		}
		if(y>0){
			if (distance[y - 1][x] == distance[y][x] - 1) {
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

void bestRoute(WindowData& fullViewport, int **distance, int y, int x, int direction) {
	int wnum = fullViewport.wnum, hnum = fullViewport.hnum;
	
	if (direction != 1 && y > 0 && existDirection(hnum, wnum, y, x, 3)) {		//go up
		if (distance[y - 1][x] > distance[y][x]) {
			distance[y - 1][x] = distance[y][x] + 1;
			bestRoute(fullViewport, distance, y - 1, x, 3);
		}
	}
	if (direction != 3 && y < hnum - 1 && existDirection(hnum, wnum, y, x, 1)) {	//go down
		if (distance[y + 1][x] > distance[y][x]) {
			distance[y + 1][x] = distance[y][x] + 1;
			bestRoute(fullViewport, distance, y + 1, x, 1);
		}
	}
	if (direction != 2 && x > 0 && existDirection(hnum, wnum, y, x, 0)) {  //go left
		if (distance[y][x - 1] > distance[y][x]) {
			distance[y][x - 1] = distance[y][x] + 1;
			bestRoute(fullViewport, distance, y, x - 1, 0);
		}
	}
	if (direction != 0 && x < wnum - 1 && existDirection(hnum, wnum, y, x, 2)) {  //go right
		if (distance[y][x + 1] > distance[y][x]) {
			distance[y][x + 1] = distance[y][x] + 1;
			bestRoute(fullViewport, distance, y, x + 1, 2);
		}
	}
}

bool existDirection(int hnum, int wnum, int y, int x, int direction) {
	if (y == 0 && direction == 3) //go up
		return false;
	if (x == wnum - 1 && direction == 2)	//go right
		return false;
	if (y == hnum - 1 && direction == 1)  //go down
		return false;
	if (x == 0 && direction == 0)	//go left
		return false;
	if ((road[y][x] >> (direction * 4)) % 2)
		return true;
	return false;
}