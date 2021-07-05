void assignCar(SDL_Renderer* renderer, WindowData& fullViewport, EventData e, CarData c, Building**& build, CarData car[], ImageData car_pic[], InciData& inci) {
	int width = fullViewport.w, height = fullViewport.h, wnum = fullViewport.wnum, hnum = fullViewport.hnum, xnum, ynum, carnum = fullViewport.carnum;
	static int select = 0;
	static SDL_TimerID timerID_clock;
	double x, y;
	if (c.type) {
		x = e.x;
		y = e.y;
		int special = 0;
		if (c.type == 4)
			if (e.type == 1 || e.type == 2)
				special = 1;
		if (special) {
			CarData t, min;
			int min_num = -1;
			min.length = INT_MAX;
			//search for stopping special car
			for (int i = 0; i < carnum; i++)
				if (car[i].type == c.type && car[i].velocity < 0) {
					if (!car[i].path)
						delete[](car[i].path);
					t = car[i];
					createCar(t, fullViewport, e.y, e.x, true, c.type, car_pic);
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
				c.type = 0;
			}
		}
		if (c.type != 4) {
			x = e.x;
			y = e.y;
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
					if (car[i].type == c.type && car[i].velocity < 0) {
						if (!car[i].path)
							delete[](car[i].path);
						t = car[i];
						createCar(t, fullViewport, y, x, false, c.type, car_pic);
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
					c.type = 0;
				}
			}
		}
	}
}
void AI(SDL_Renderer* renderer, WindowData& fullViewport, EventData* e, CarData car[], Building** build, ImageData car_pic[], ValueData value, InciData& inci) {
	int p[3];
	double d[3];
	CarData t;
	int min = 0;
	for (int i = 0; i < value.level; i++) {
		if (e[i].assign != true) {
			switch (e[i].type) {
			case 0:
				p[i] = 1;
				if (build[(int)(e[i].y - 1)][(int)(e[i].x - 1)].type && build[(int)(e[i].y - 1)][(int)(e[i].x - 1)].type <= 7)
					createCar(t, fullViewport, e[i].y, e[i].x, false, 2, car_pic);
				else
					createCar(t, fullViewport, e[i].y, e[i].x, false, 1, car_pic);
				d[i] = t.length;
				break;
			case 1:
				p[i] = 3;
				createCar(t, fullViewport, e[i].y, e[i].x, false, 4, car_pic);
				d[i] = t.length;
				break;
			case 2:
				p[i] = 6;
				createCar(t, fullViewport, e[i].y, e[i].x, false, 4, car_pic);
				d[i] = t.length;
				break;
			case 3:
				p[i] = 5;
				createCar(t, fullViewport, e[i].y, e[i].x, false, 3, car_pic);
				d[i] = t.length;
				break;
			case 4:
				p[i] = 2;
				createCar(t, fullViewport, e[i].y, e[i].x, false, 4, car_pic);
				d[i] = t.length;
				break;
			case 5:
				p[i] = 4;
				createCar(t, fullViewport, e[i].y, e[i].x, false, 1, car_pic);
				d[i] = t.length;
				break;
			default:
				break;
			}
		}
		if (p[i] * d[i] < p[min] * d[min])
			min = i;
	}
	e[min].assign = true;
	assignCar(renderer, fullViewport, e[min], t, build, car, car_pic, inci);
}

