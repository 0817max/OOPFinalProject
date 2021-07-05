#include<limits.h>


int carMenu(SDL_Renderer* renderer, const WindowData& fullViewport, bool windowChange, const Mouse& mouse, int& choose, ImageData car_pic[], CarData car[]);
void bestRoute(const WindowData& fullViewport, double** distance, int y, int x, int direction);
bool existDirection(int hnum, int wnum, int y, int x, int direction);
char* createPath(const WindowData& fullViewport, double start_y, double start_x, bool intersect, double end_ynum, double end_xnum, bool house_point, int& length);
void fillIntersect(char* intersect, int direction1, int direction2, int num);
bool equalIntersect(char* intersect, int direction1, int direction2, int num);
int carNum(CarData car[], int carnum, int type);

void createCar(CarData& car, WindowData& fullViewport, double house_hnum, double house_wnum, bool house_point, int type, ImageData car_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, wnum = fullViewport.wnum, hnum = fullViewport.hnum, oldw=fullViewport.oldw, oldh=fullViewport.oldh;
	char* path;
	car.length = INT_MAX;
	car.path = NULL;
	int choose=-1, length;
	//Run for four times to choose the shortest path, beginning
	if (car.x == 0 && car.y == 0) {
		if ((checkRoad((int)(car.home_y-1), (int)(car.home_x-1)) >> 3) % 2) {		//top road
			if (car.home_x == house_wnum && (car.home_y - house_hnum <= 1)&& (car.home_y - house_hnum >=0)) {
				car.length = 0;
				choose = 3;
				car.velocity = -2;
				car.path = new char[1]{ 2 };
			}
			else {
				//printf("3\n");
				path = createPath(fullViewport, car.home_y - 0.5, car.home_x, false, house_hnum, house_wnum,house_point, length);
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
		if ((checkRoad((int)(car.home_y-1), (int)(car.home_x-1)) >> 2) % 2 && car.length) {		//right road
			if ((house_wnum - car.home_x >=0) &&(house_wnum - car.home_x <=1) && car.home_y == house_hnum) {
				car.length = 0;
				choose = 2;
				car.velocity = -2;
				car.path = new char[1]{ 1 };
			}
			else {
				//printf("2\n");
				path = createPath(fullViewport, car.home_y, car.home_x + 0.5, false, house_hnum, house_wnum, house_point, length);
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
		if ((checkRoad((int)(car.home_y-1), (int)(car.home_x-1)) >> 1) % 2 && car.length) {		//bottom road
			if (car.home_x == house_wnum && (house_hnum - car.home_y <=1)&& (house_hnum - car.home_y >=0)) {
				car.length = 0;
				choose = 1;
				car.velocity = -2;
				car.path = new char[1]{ 0 };
			}
			else {
				//printf("1\n");
				path = createPath(fullViewport, car.home_y + 0.5, car.home_x, false, house_hnum, house_wnum, house_point, length);
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
		if ((checkRoad((int)(car.home_y-1), (int)(car.home_x-1))) % 2 && car.length) {		//left road
			if ((car.home_x - house_wnum <=1)&& (car.home_x - house_wnum <= 1) && car.home_y == house_hnum) {
				car.length = 0;
				choose = 0;
				car.velocity = -2;
				car.path = new char[1]{ 3 };
			}
			else {
				//printf("0\n");
				path = createPath(fullViewport, car.home_y, car.home_x - 0.5, false, house_hnum, house_wnum, house_point, length);
				if (car.length > length) {
					car.length = length;
					if (car.path) {
						delete[]car.path;
						car.path = NULL;
					}
					car.path = path;
					choose = 0;
				}
			}
		}
		//Intialize the starting point
		switch (choose) {
		case 3:
			car.x = (car.home_x);
			car.y = (car.home_y - 0.5);
			break;
		case 2:
			car.x = (car.home_x + 0.5);
			car.y = (car.home_y);
			break;
		case 1:
			car.x = (car.home_x);
			car.y = (car.home_y + 0.5);
			break;
		case 0:
			car.x = (car.home_x - 0.5);
			car.y = (car.home_y);
			break;
		}
	}
	else {
		if (checkIntersect(fullViewport, road, car.y, car.x)) {
			car.intersect = true;
		}
		else
			car.intersect = false;
		//start on the vertical road
		if ((car.x - 0.5) == (int)car.x) {
			if (fabs(car.x - house_wnum) <= 0.51 && house_hnum == (int)(car.y + 0.5)) {
				car.length = 0;
				car.velocity = -2;
				car.path = new char[1]{ 3 };
			}
			else {
				car.path = createPath(fullViewport, car.y, car.x, car.intersect, house_hnum, house_wnum, house_point, length);
				car.length = length;
			}
		}
		//start on the horizontal road
		else{
			if (fabs(car.y - house_hnum) <= 0.51 && house_wnum == (int)(car.x + 0.5)) {
				car.length = 0;
				car.velocity = -2;
				car.path = new char[1]{ 2 };
			}
			else {
				car.path = createPath(fullViewport, car.y, car.x, car.intersect, house_hnum, house_wnum, house_point, length);
				car.length = length;
			}
		}
	}


	//Initialize other information
	car.window = &fullViewport;
	car.type = type;
	car.i = 0;
	car.angle = (3-car.path[0])*90;
	car.home_y = house_hnum;
	car.home_x = house_wnum;
	if(car.length)
		car.velocity = 0.01;
	car.intersect = false;
}

void addCar(SDL_Renderer* renderer, WindowData& fullViewport,bool windowChange, const Mouse& mouse, char& pause, const EventData event[], Building**& build , CarData car[], ImageData car_pic[], InciData& inci) {
	int width = fullViewport.w, height = fullViewport.h, wnum = fullViewport.wnum, hnum = fullViewport.hnum, xnum, ynum, carnum = fullViewport.carnum;
	static int choose = 0, select=0;
	double x, y;
	static bool flag = false;
	if (renderer == NULL) {
		if(flag)
			carMenu(NULL, fullViewport, windowChange, mouse, choose, car_pic, car);
		flag = false;
		choose = select = 0;
		return;
	}

	if (select) {
		flag = true;
	}
		
	//Right Click for releaing choose
	if (mouse.state == IN_RB_SC) {
		choose = 0;
		select = 0;
		if (pause==2)
			pause = 3;
	}

	//Left Click
	else if (mouse.state == IN_LB_SC) {

		//If choose, create car from special building to determined point
		if (choose) {
			x = (double)mouse.X / ((width - height / 12) / wnum);
			y = (double)(mouse.Y - height / 12) / ((height - height / 12) / hnum);
		//	printf("%d %lf %lf %lf %lf\n", choose, x, y, event.x, event.y);
			int special = 0;
			if(choose==4)
				for (int i = 0; i < *(event[0].level); i++) {
					if((event[i].type == 1 || event[i].type == 2) && fabs(event[i].x - x) < 0.3 && fabs(event[i].y - y) < 0.5){
						special = i + 1;
						break;
					}
				}
			if (special) {
				CarData t, min;
				int min_num = -1;
				min.length = INT_MAX;
				//search for stopping special car
				bool flag = false;
				for (int i = 0; i < carnum; i++)
					if (car[i].type == choose && car[i].velocity < 0) {
						flag = true;
						if (!car[i].path)
							delete[](car[i].path);
						t = car[i];
						createCar(t, fullViewport, event[special-1].y, event[special-1].x, true, choose, car_pic);
						//find the closet car
						if (t.length < min.length) {
							min = t;
							min_num = i;
						}
						else
							delete t.path;
					}
				if (min_num == -1) {
					//there is no way
					if (flag)
						inci.addcar = 3;
					//there is no car
					else
						inci.addcar = 2;
					inci.alpha[1] = 450;
					return;
				}
				else {
					car[min_num] = min;
					choose = 0;
				}
			}
			else if (mouse.X > (width - height / 12) / wnum / 2 && mouse.X<(width - height / 12 - (width - height / 12) / wnum / 2) && mouse.Y >(height / 12 + ((height - height / 12) / hnum) / 2) && mouse.Y < (height - ((height - height / 12) / hnum) / 2)) {
				x = (mouse.X - (width - height / 12) / wnum / 2) / ((width - height / 12) / wnum) + 1;
				y = (mouse.Y - height / 12 - (height - height / 12) / hnum / 2) / ((height - height / 12) / hnum) + 1;
				if (build[(int)(y - 1)][(int)(x - 1)].type == Empty) {
					inci.addcar = 1;
					inci.alpha[1] = 450;
					return;
				}
				else {
					CarData t, min;
					int  min_num = -1;
					min.length = INT_MAX;
					//search for stopping special car
					for (int i = 0; i < carnum; i++)
						if (car[i].type == choose && car[i].velocity < 0) {
							if (!car[i].path)
								delete[](car[i].path);
							t = car[i];
							createCar(t, fullViewport, y, x, false, choose, car_pic);
							//find the closet car
							if (t.length < min.length) {
								min = t;
								min_num = i;
							}
							else
								delete t.path;
						}
					//there is no car
					if (min_num == -1) {
						inci.addcar = 2;
						inci.alpha[1] = 450;
						return;
					}
					else {
						car[min_num] = min;
						choose = 0;
					}
				}
			}
		}

		//If not choose, left click to enter select menu
		if (mouse.X >= (width - height / 12) && mouse.Y >= (height * 10 / 12) && mouse.Y <= (height * 12 / 12)) {
			select = !select;
			if (select) {
				if (pause == 0)
					pause = 1;
			}
			else {
				if (pause == 2)
					pause = 3;
			}
		}
	}
	else if (mouse.X > (width - height / 12) && mouse.Y > (height * 10 / 12) && mouse.Y <= (height * 12 / 12)) {
		boxColor(renderer, (width - height / 12) + height / 1000 + 1, (height * 10 / 12) + height / 1000 + 1, width, height, 0x22FFFFFF);
	}
	else if (choose) {
		int special = 0;
		x = (double)mouse.X / ((width - height / 12) / wnum);
		y = (double)(mouse.Y - height / 12) / ((height - height / 12) / hnum);
		if (choose == 4)
			for (int i = 0; i < *(event[0].level); i++) {
				if ((event[i].type == 1 || event[i].type == 2) && fabs(event[i].x - x) < 0.3 && fabs(event[i].y - y) < 0.5) {
					special = i + 1;
					break;
				}
			}

		if (special) {
			if (event[special-1].h)
				boxColor(renderer, (double)(width - height / 12) / wnum * (event[special - 1].x - 0.5 + 1 / 10.), height / 12 + (double)(height - height / 12) / hnum * event[special - 1].y - (height - height / 12) / hnum / 6., (double)(width - height / 12) / wnum * (event[special - 1].x + 0.5 - 1 / 10.), height / 12 + (double)(height - height / 12) / hnum * event[special - 1].y + (height - height / 12) / hnum / 6., 0x22FFFFFF);
			else
				boxColor(renderer, (double)(width - height / 12) / wnum * (event[special - 1].x - 1 / 10.), height / 12 + (double)(height - height / 12) / hnum * (event[special - 1].y - 0.5 + 1 / 6.), (double)(width - height / 12) / wnum * (event[special - 1].x + 1 / 10.), height / 12 + (double)(height - height / 12) / hnum * (event[special - 1].y + 0.5 - 1 / 6.), 0x22FFFFFF);
		}
		else if (mouse.X > (width - height / 12) / wnum / 2 && mouse.X<(width - height / 12 - (width - height / 12) / wnum / 2) && mouse.Y >(height / 12 + ((height - height / 12) / hnum) / 2) && mouse.Y < (height - ((height - height / 12) / hnum) / 2)) {
			xnum = (mouse.X - (width - height / 12) / wnum / 2) / ((width - height / 12) / wnum);
			ynum = (mouse.Y - height / 12 - (height - height / 12) / hnum / 2) / ((height - height / 12) / hnum);
			boxColor(renderer, (double)(width - height / 12) / wnum * (0.5 + xnum + 1 / 10.), height / 12 + ((double)(height - height / 12) / hnum) * (ynum + 0.5 + 1 / 6.), (double)(width - height / 12) / wnum * (1.5 + xnum - 1 / 10.), height / 12 + ((double)(height - height / 12) / hnum) * (ynum + 1.5 - 1 / 6.), 0x22FFFFFF);
		}
	}

	//Cliking the flag
	if (select) {
		if (pause == 0)
			pause = 1;
		if (!carMenu(renderer, fullViewport, windowChange,mouse, choose, car_pic, car)) {	//false for quit
			select = 0;
			if (pause == 2)
				pause = 3;
		}
	}

	//Fixed chosen car on mouse
	switch (choose) {
	case 1:
		imgRender(renderer, car_pic[0], Middle, mouse.X, mouse.Y, NULL, height / 12, 1, NULL, NULL, 0, no, 230);
		break;
	case 2:
		imgRender(renderer, car_pic[1], Middle, mouse.X, mouse.Y, NULL, height / 12, 1, NULL, NULL, 0, no, 230);
		break;
	case 3:
		imgRender(renderer, car_pic[2], Middle, mouse.X, mouse.Y, NULL, height / 12, 1, NULL, NULL, 0, no, 230);
		break;
	case 4:
		imgRender(renderer, car_pic[3], Middle, mouse.X, mouse.Y, NULL, height / 12, 1, NULL, NULL, 0, no, 230);
		break;
	}
	
}

int carMenu(SDL_Renderer* renderer, const WindowData& fullViewport, bool windowChange, const Mouse& mouse, int& choose, ImageData car_pic[], CarData car[]) {
	int width = fullViewport.w, height = fullViewport.h;
	static int oldcarnum[4] = { -1, -1, -1, -1 };
	int type_carnum;
	static TextData number_text[4];
	char num[4];
	if (renderer == NULL) {
		oldcarnum[0] = -1;
		for (int i = 0; i < 4; i++)
			SDL_DestroyTexture(number_text[i].texture);
		return 0;
	}
	if (oldcarnum[0] == -1) {
		for (int i = 0; i < 4; i++) {
			oldcarnum[i] = carNum(car, fullViewport.carnum, i + 1);
			sprintf_s(num, 4, "%d", oldcarnum[i]);
			number_text[i] = loadTextTexture(renderer, num, "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 12, 255, 255, 255, BLENDED);
		}
	}
	else if (windowChange) {
		for (int i = 0; i < 4; i++) {
			oldcarnum[i] = carNum(car, fullViewport.carnum, i + 1);
			SDL_DestroyTexture(number_text[i].texture);
			sprintf_s(num, 4, "%d", oldcarnum[i]);
			number_text[i] = loadTextTexture(renderer, num, "../fonts/TaipeiSansTCBeta-Regular.ttf", height/ 12, 255, 255, 255, BLENDED);
		}
	}
	else{
		for (int i = 0; i < 4; i++) {
			type_carnum = carNum(car, fullViewport.carnum, i + 1);
			if (type_carnum != oldcarnum[i]) {
				SDL_DestroyTexture(number_text[i].texture);
				sprintf_s(num, 4, "%d", type_carnum);
				number_text[i] = loadTextTexture(renderer, num, "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 12, 255, 255, 255, BLENDED);
				oldcarnum[i] = type_carnum;
			}
		}
	}

	boxColor(renderer, 0, height / 12 + height / 1000 + 1, width - height / 12 - height / 1000 - 1, height, 0xBB000000);
	imgRender(renderer, car_pic[0], RightBottom, width / 2 - 10, height / 2 - 10, NULL, height / 3, 1, NULL, NULL, 0, no, 250);
	imgRender(renderer, car_pic[1], LeftBottom, width / 2 + 10, height / 2 - 10, NULL, height / 3, 1, NULL, NULL, 0, no, 250);
	imgRender(renderer, car_pic[2], RightTop, width / 2 - 10, height / 2 + 10, NULL, height / 3, 1, NULL, NULL, 0, no, 250);
	imgRender(renderer, car_pic[3], LeftTop, width / 2 + 10, height / 2 + 10, NULL, height / 3, 1, NULL, NULL, 0, no, 250);

	textRender(renderer, number_text[0], Right, width / 2 - 10 - height / 3 * car_pic[0].width / car_pic[0].height, height / 2 - 10 - height / 6, no, 255);
	textRender(renderer, number_text[1], Left, width / 2 + 10 + height / 3 * car_pic[1].width / car_pic[1].height, height / 2 - 10 - height / 6, no, 255);
	textRender(renderer, number_text[2], Right, width / 2 - 10 - height / 3 * car_pic[2].width / car_pic[2].height, height / 2 + 10 + height / 6, no, 255);
	textRender(renderer, number_text[3], Left, width / 2 + 10 + height / 3 * car_pic[3].width / car_pic[3].height, height / 2 + 10 + height / 6, no, 255);

	//select which car
	if (mouse.state == IN_LB_SC) {
		if (fabs(mouse.X - width / 2) < height / 6 && fabs(mouse.Y - height / 2) < height / 3) {
			if ((mouse.Y - height / 2) < 0) {
				if ((mouse.X - width / 2) < 0)
					choose = 1;
				else
					choose = 2;
			}
			else {
				if ((mouse.X - width / 2) < 0)
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
	else if (mouse.state == IN_RB_SC) {
		choose = 0;
		return 0;
	}
	else {
		if (fabs(mouse.X - width / 2) < height / 6 && fabs(mouse.Y - height / 2) < height / 3) {
			if ((mouse.Y - height / 2) < 0) {
				if ((mouse.X - width / 2) < 0)
					boxColor(renderer, width / 2 - 10- height / 3*car_pic[0].width/car_pic[0].height, height / 2 - 10- height / 3,width / 2 - 10, height / 2 - 10, 0x22FFFFFF);
				else
					boxColor(renderer, width / 2 + 10 + height / 3 * car_pic[1].width / car_pic[1].height, height / 2 - 10 - height / 3, width / 2 + 10, height / 2 - 10, 0x22FFFFFF);
			}
			else {
				if ((mouse.X - width / 2) < 0)
					boxColor(renderer, width / 2 - 10 - height / 3 * car_pic[2].width / car_pic[2].height, height / 2 + 10 + height / 3, width / 2 - 10, height / 2 + 10, 0x22FFFFFF);
				else
					boxColor(renderer, width / 2 + 10 + height / 3 * car_pic[3].width / car_pic[3].height, height / 2 + 10 + height / 3, width / 2 + 10, height / 2 + 10, 0x22FFFFFF);
			}
		}
	}
	//keep in menu
	return 1;
}

void carRender(SDL_Renderer* renderer, const WindowData& fullViewport, CarData car[], ImageData car_pic[], ImageData cloud_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum, wnum = fullViewport.wnum, carnum = fullViewport.carnum;
	;
	for (int i = 0; i < carnum; i++) {

		if (car[i].type >= 5 && car[i].type <= 7) {
			if (car[i].angle % 180) {
				if ((road[(int)(car[i].y - 0.5)][(int)(car[i].x - 0.5)] >> 8) % 2 == 0)
					continue;
			}
			else {
				if ((road[(int)(car[i].y - 0.5)][(int)(car[i].x - 0.5)] >> 4) % 2 == 0)
					continue;
			}
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
void carRender1(SDL_Renderer* renderer, const WindowData& fullViewport, CarData car[], ImageData car_pic[], ImageData cloud_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum, wnum = fullViewport.wnum, carnum = fullViewport.carnum;
	;
	for (int i = 0; i < carnum; i++) {

		if (car[i].type >= 5 && car[i].type <= 7) {
			if (car[i].angle % 180) {
				if ((road[(int)(car[i].y - 0.5)][(int)(car[i].x - 0.5)] >> 8) % 2 == 0)
					continue;
			}
			else {
				if ((road[(int)(car[i].y - 0.5)][(int)(car[i].x - 0.5)] >> 4) % 2 == 0)
					continue;
			}
		}

		if (car[i].velocity < 0) {
			if (!car[i].path) {
				car[i].velocity = -1;
				delete[]car[i].path;
				car[i].path = NULL;
			}
			continue;
		}
		imgRender(renderer, car_pic[car[i].type - 1], Left, car[i].x * width / wnum, height/8+car[i].y *(height*6/8)/ hnum, fmin(width / wnum / 10., height*6/8 / hnum / 6.), NULL, 1, NULL, NULL, car[i].angle, no, 255);
	}
}


Uint32 car_move(Uint32 interval, void* param)
{
	CarData *t = (CarData*)param;
	int width = t[4].window->w, height = t[4].window->h, wnum = t[4].window->wnum, hnum = t[4].window->hnum, carnum=t[4].window->carnum;
	
	//create the intersect for car (divide into four piece: 0:lefttop 1:righttop 2:rightbottom 3:leftbottom)
	static char*** CarIntersect = NULL;
	if (interval == NULL&&CarIntersect) {
		for (int i = 0; i < hnum; i++) {
			for (int j = 0; j < wnum; j++)
				if (CarIntersect[i][j])
					delete []CarIntersect[i][j];
			if (CarIntersect[i])
				delete[]CarIntersect[i];
		}
		delete[]CarIntersect;
		CarIntersect = NULL;
		
		for (int i = 0; i < carnum; i++) {
			if (t[i].path) {
				delete t[i].path;
				t[i].path = NULL;
			}
		}
		delete[]t;
		return 0;
	}
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
	for (int i = 0; i < carnum; i++) {
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
		/*if (t[i].velocity < -1) {
			t[i].velocity += 0.004;
			continue;
		}*/
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
			for (int k = 0; k < carnum; k++) {
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
			//Leave the interscetion, free the intersection
			if (i == 2&& (fabs(t[i].x - (int)t[i].x - 0.5) > 1 / 3 || fabs(t[i].y - (int)t[i].y - 0.5) >= 1 / 2.01)) {	//for truck
				for (int j = 0; j < 2; j++) {
					if ((int)(t[i].y - 1 + j) < 0 || (int)(t[i].y - 1 + j) >= hnum)
						continue;
					for (int k = 0; k < 2; k++) {
						if ((int)(t[i].x - 1 + k) < 0 || (int)(t[i].x - 1 + k) >= wnum)
							continue;
						for (int l = 0; l < 4; l++)
							if (CarIntersect[(int)(t[i].y - 1 + j)][(int)(t[i].x - 1 + k)][l] == i + 1)
								CarIntersect[(int)(t[i].y - 1 + j)][(int)(t[i].x - 1 + k)][l] = 0;
					}
				}
			}
			else{
				if((fabs(t[i].x - (int)t[i].x - 0.5) > 1 / 4.2 || fabs(t[i].y - (int)t[i].y - 0.5) > 1 / 2.24))
					for (int j = 0; j < 2; j++) {
						if ((int)(t[i].y - 1 + j) < 0 || (int)(t[i].y - 1 + j) >= hnum)
							continue;
						for (int k = 0; k < 2; k++) {
							if ((int)(t[i].x - 1 + k) < 0 || (int)(t[i].x - 1 + k) >= wnum)
								continue;
							for (int l = 0; l < 4; l++)
								if (CarIntersect[(int)(t[i].y - 1 + j)][(int)(t[i].x - 1 + k)][l] == i + 1)
									CarIntersect[(int)(t[i].y - 1 + j)][(int)(t[i].x - 1 + k)][l] = 0;
						}
					}
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

char* createPath(const WindowData& fullViewport, double start_y, double start_x, bool intersect,  double end_y, double end_x, bool house_point, int &length) {
	int wnum = fullViewport.wnum, hnum = fullViewport.hnum;
	
	//create diatance map
	double** distance = new double* [hnum];
	double min_dis = (double)INT_MAX;
	int min_disp=0;
	char* path;
	for (int i = 0; i < hnum; i++) {
		distance[i] = new double[wnum];
		for (int j = 0; j < wnum; j++)
			distance[i][j] = 1e8;
	}
	
	if (intersect) {
		distance[(int)(start_y)][(int)start_x] = 0;
		bestRoute(fullViewport, distance, (int)(start_y), (int)(start_x), 4);
	}
	//start on the vertical road
	else if (fabs(start_x - 0.5 - (int)start_x) < 1e-3) {
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
	if (house_point) {
		if (checkIntersect(fullViewport, road, end_y, end_x)) {
			min_dis=distance[(int)(end_y-0.5)][(int)(end_x-0.5)];
			min_disp = 0;
		}
		//end on the vertical road
		else if (fabs(end_x - 0.5 - (int)end_x) < 1e-3) {
			if (min_dis > distance[(int)(end_y-0.5)][(int)(end_x-0.5)]) {
				min_dis = distance[(int)(end_y-0.5)][(int)(end_x-0.5)];
				min_disp = 0;
			}
			if (min_dis > distance[(int)(end_y+0.5)][(int)(end_x-0.5)]) {
				min_dis = distance[(int)(end_y+0.5)][(int)(end_x-0.5)];
				min_disp = 1;
			}
		}
		//end on the horizontal road
		else {
			if (min_dis > distance[(int)(end_y-0.5)][(int)(end_x-0.5)]) {
				min_dis = distance[(int)(end_y-0.5)][(int)(end_x-0.5)];
				min_disp = 0;
			}
			if (min_dis > distance[(int)(end_y-0.5)][(int)(end_x+0.5)]) {
				min_dis = distance[(int)(end_y-0.5)][(int)(end_x+0.5)];
				min_disp = 1;
			}
		}
		//retrace the shortest path(from destination to the starting point)
		if (min_dis == INT_MAX) {
			return NULL;
		}
		length = (int)(min_dis + 1);
		path = new char[length + 1];
		int y, x;
		//find the direction from destination point to house and initialize the starting point of retracing
		switch (min_disp) {
		case 1:
			//end on the vertical road
			if (fabs(end_x - 0.5 - (int)end_x) < 1e-3) {
				path[length] = 3;
				y = (int)(end_y+0.5), x = (int)(end_x-0.5);
			}
			//end on the horizontal road
			else {
				path[length] = 0;
				y = (int)(end_y-0.5), x = (int)(end_x+0.5);
			}
			break;
		case 0:
			if (checkIntersect(fullViewport, road, end_y, end_x)) {
				path[length] = -1;
				y = (int)(end_y - 0.5), x = (int)(end_x - 0.5);
			}
			//end on the vertical road
			else if (fabs(end_x - 0.5-(int)end_x)<1e-3) {
				path[length] = 1;
				y = (int)(end_y-0.5), x = (int)(end_x-0.5);
			}
			//end on the horizontal road
			else {
				path[length] = 2;
				y = (int)(end_y-0.5), x = (int)(end_x-0.5);
			
			}
			break;
		}
		for (int i = length - 1; i > 0; i--) {
			if (x > 0) {
				if (fabs(distance[y][x]- distance[y][x - 1] - 1)<1e-4 && existDirection(hnum, wnum, y, x - 1, 2)) {
					x -= 1;
					path[i] = 2;
					continue;
				}
			}
			if (y < hnum - 1) {
				if (fabs(distance[y][x] - distance[y+1][x] - 1) < 1e-4 && existDirection(hnum, wnum, y + 1, x, 3)) {
					y += 1;
					path[i] = 3;
					continue;
				}
			}
			if (x < wnum - 1) {
				if (fabs(distance[y][x] - distance[y][x + 1] - 1) < 1e-4 && existDirection(hnum, wnum, y, x + 1, 0)) {
					x += 1;
					path[i] = 0;
					continue;
				}
			}
			if (y > 0) {
				if (fabs(distance[y][x] - distance[y-1][x] - 1) < 1e-4 && existDirection(hnum, wnum, y - 1, x, 1)) {
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
		else if (fabs(start_x - 0.5 - (int)start_x) < 1e-3) {
			if (x == (int)(start_x) && fabs(y + 1 - (int)start_y) < 1e-3)
				path[0] = 3;
			else
				path[0] = 1;
		}
		else {
			if (fabs(x + 1 - (int)start_x) < 1e-3 && y == (int)(start_y))
				path[0] = 0;
			else
				path[0] = 2;
		}
	}
	else {
		int house_hnum = (int)end_y-1, house_wnum = (int)end_x-1;
		//top road
		if ((checkRoad(house_hnum, house_wnum) >> 3) % 2) {
			if (min_dis > distance[house_hnum][house_wnum]) {
				min_dis = distance[house_hnum][house_wnum];
				min_disp = 3;
			}
			if (min_dis > distance[house_hnum][house_wnum + 1]) {
				min_dis = distance[house_hnum][house_wnum + 1];
				min_disp = 3;
			}
		}
		//right road
		if ((checkRoad(house_hnum, house_wnum) >> 2) % 2) {
			if (min_dis > distance[house_hnum][house_wnum + 1]) {
				min_dis = distance[house_hnum][house_wnum + 1];
				min_disp = 2;
			}
			if (min_dis > distance[house_hnum + 1][house_wnum + 1]) {
				min_dis = distance[house_hnum + 1][house_wnum + 1];
				min_disp = 2;
			}
		}
		//bottom road
		if ((checkRoad(house_hnum, house_wnum) >> 1) % 2) {
			if (min_dis > distance[house_hnum + 1][house_wnum]) {
				min_dis = distance[house_hnum + 1][house_wnum];
				min_disp = 1;
			}
			if (min_dis > distance[house_hnum + 1][house_wnum + 1]) {
				min_dis = distance[house_hnum + 1][house_wnum + 1];
				min_disp = 1;
			}
		}
		//left road
		if (checkRoad(house_hnum, house_wnum) % 2) {
			if (min_dis > distance[house_hnum][house_wnum]) {
				min_dis = distance[house_hnum][house_wnum];
				min_disp = 0;
			}
			if (min_dis > distance[house_hnum + 1][house_wnum]) {
				min_dis = distance[house_hnum + 1][house_wnum];
				min_disp = 0;
			}
		}
		//retrace the shortest path(from destination to the starting point)
		length = (int)(min_dis + 1);
		path = new char[length + 1];
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
			x = house_wnum + 1;
			if (distance[house_hnum][house_wnum + 1] < distance[house_hnum + 1][house_wnum + 1]) {
				y = house_hnum;
				path[length] = 1;
			}
			else {
				y = house_hnum + 1;
				path[length] = 3;
			}
			break;
		case 1:
			y = house_hnum + 1;
			if (distance[house_hnum + 1][house_wnum] < distance[house_hnum + 1][house_wnum + 1]) {
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
				if ((fabs(distance[y][x] - distance[y][x - 1] - 1)<1e-4) && existDirection(hnum, wnum, y, x - 1, 2)) {
					x -= 1;
					path[i] = 2;
					continue;
				}
			}
			if (y < hnum - 1) {
				if ((fabs(distance[y][x] - distance[y+1][x] - 1) < 1e-4) && existDirection(hnum, wnum, y + 1, x, 3)) {
					y += 1;
					path[i] = 3;
					continue;
				}
			}
			if (x < wnum - 1) {
				if ((fabs(distance[y][x] - distance[y][x+1] - 1) < 1e-4) && existDirection(hnum, wnum, y, x + 1, 0)) {
					x += 1;
					path[i] = 0;
					continue;
				}
			}
			if (y > 0) {
				if ((fabs(distance[y][x] - distance[y - 1][x] - 1) < 1e-4) && existDirection(hnum, wnum, y - 1, x, 1)) {
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
		else if ((start_x - 0.5) == (int)start_x) {
			if (x == (int)(start_x) && y == (int)(start_y - 0.5))
				path[0] = 3;
			else
				path[0] = 1;
		}
		else {
			if (x == (int)(start_x - 0.5) && y == (int)(start_y))
				path[0] = 0;
			else
				path[0] = 2;
		}
	}
	/*for (int i = 0; i < hnum; i++) {
		for (int j = 0; j < wnum; j++)
			printf("%lg ", distance[i][j]);
		printf("\n");
	}
	printf("\n");*/
	/*for (int i = 0; i < length + 1; i++) {
		printf("%d ", path[i]);
	}
	printf("\n\n");*/
	//printf("%d ", length);
	for (int i = 0; i < hnum; i++)
		delete[]distance[i];
	delete[]distance;
	return path;
}

void bestRoute(const WindowData& fullViewport, double **distance, int y, int x, int direction) {
	int wnum = fullViewport.wnum, hnum = fullViewport.hnum;
	if (direction != 1 && existDirection(hnum, wnum, y, x, 3)) {		//go up
		if (distance[y - 1][x] > distance[y][x]+1) {
			distance[y - 1][x] = distance[y][x] + 1;
			bestRoute(fullViewport, distance, y - 1, x, 3);
		}
	}
	if (direction != 3 && existDirection(hnum, wnum, y, x, 1)) {	//go down
		if (distance[y + 1][x] >distance[y][x]+1) {
			distance[y + 1][x] = distance[y][x] + 1;
			bestRoute(fullViewport, distance, y + 1, x, 1);
		}
	}
	if (direction != 2 && existDirection(hnum, wnum, y, x, 0)) {  //go left
		if (distance[y][x - 1] > distance[y][x]+1) {
			distance[y][x - 1] = distance[y][x] + 1;
			bestRoute(fullViewport, distance, y, x - 1, 0);
		}
	}
	if (direction != 0 && existDirection(hnum, wnum, y, x, 2)) {  //go right
		if (distance[y][x + 1] > distance[y][x]+1) {
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

int carNum(CarData car[], int carnum, int type) {
	int num=0;
	for (int i = 0; i < carnum; i++)
		if (car[i].type == type && car[i].velocity < 0)
			num++;
	return num;
}