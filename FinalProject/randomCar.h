#define CARNUM 32
#define INITAILlRANDOMCARNUM 28
struct point {
	int x, y;
	bool exist;
	int l, r, u, d;
};

point randomstart(const WindowData& fullViewport, int** road);
void createRandomCar(CarData& car, WindowData& fullViewport, const point& start);

int initCar(WindowData* fullViewport, CarData car[], Building **build) {
	point* start = new point[INITAILlRANDOMCARNUM];
	bool repeat = false;
	int count = 0;
	for (int i = 0; i < CARNUM; i++) {
		car[i].x = car[i].y = 0.5;
		car[i].window = fullViewport;
		car[i].path = NULL;
		car[i].i = car[i].length = car[i].angle = car[i].type = 0;
		car[i].home_xnum = car[i].home_ynum = 0;
		car[i].velocity = -1;
		car[i].intersect = false;
	}
	for (int x = 0; x < fullViewport->wnum - 1; x++)
		for (int y = 0; y < fullViewport->hnum - 1; y++)
			if (build[y][x].type == FireSta) {
				car[0].type = 1;
				car[1].type = 2;
				car[0].velocity = car[1].velocity = -1;
				car[0].home_xnum = car[1].home_xnum = x;
				car[0].home_ynum = car[1].home_ynum = y;
			}
			else if (build[y][x].type == Logistics) {
				car[2].type = 3;
				car[2].velocity = -1;
				car[2].home_xnum = x;
				car[2].home_ynum = y;
			}
			else if (build[y][x].type == PoliceOff) {
				car[3].type = 4;
				car[3].velocity = -1;
				car[3].home_xnum = x;
				car[3].home_ynum = y;
			}
	for (int i = 0; i < INITAILlRANDOMCARNUM; i++) {
		//printf("%d\n", i);
		count = 0;
		do {
			repeat = false;
			start[i] = randomstart(*fullViewport, road);
			//printf("%d %d\n", start[i].x, start[i].y);
			for (int j = 0; j < i; j++)
				if ((start[j].x == start[i].x) && (start[j].y == start[i].y)) {
					repeat = true;
					break;
				}
			count++;
			if (count >= 1e2)
				return count;
		} while (repeat);
	}
	for (int i = 0; i < INITAILlRANDOMCARNUM; i++)
		createRandomCar(car[i + 4], *fullViewport, start[i]);
	delete[]start;
	return 0;
}

point randomstart(const WindowData& fullViewport, int **road) {
	int wnum = fullViewport.wnum, hnum = fullViewport.hnum;

	point p;
	p.exist = false;
	while (!p.exist) {
		p.x = rand() % wnum;
		p.y = rand() % hnum;
		p.exist = false;
		if (road[p.y][p.x])
			p.exist = true;
	}
	return p;
}

char* randpath(const WindowData& fullViewport, const point& startp, int& n) {
	n = 3 + rand() % 13;
	//printf("%d\n", n);
	int stop = 0;
	char* path = new char[n + 1];
	int wnum = fullViewport.wnum, hnum = fullViewport.hnum;
	int** rh = new int* [hnum];
	for (int i = 0; i < hnum; i++)
		rh[i] = new int[wnum - 1];
	int** rv = new int* [hnum - 1];
	for (int i = 0; i < hnum - 1; i++)
		rv[i] = new int[wnum];
	for (int i = 0; i < hnum; i++) //horizontal road
		for (int j = 0; j < wnum - 1; j++) {
			if (((road[i][j] >> 8) % 2) * (road[i][j + 1] % 2))
				rh[i][j] = 1;
			else
				rh[i][j] = 0;
		}
	for (int i = 0; i < hnum - 1; i++)//vertical road
		for (int j = 0; j < wnum; j++) {
			if (((road[i][j] >> 4) % 2) * ((road[i + 1][j] >> 12) % 2))
				rv[i][j] = 1;
			else
				rv[i][j] = 0;
		}
	point p;
	p.x = startp.x;
	p.y = startp.y;
	p.l = 0, p.r = 0, p.u = 0, p.d = 0;
	for (int i = 1; i < n + 1; i++) {
		if (p.x<0 || p.x>wnum || p.y<0 || p.y>hnum) {
			path[i] = -1;
			n = i;
			break;
		}
		if (p.x >= 1 && p.x < wnum - 1) {
			if (rh[p.y][p.x - 1] == 1)
				p.l = 1;
			else
				p.l = 0;
			if (rh[p.y][p.x] == 1)
				p.r = 1;
			else
				p.r = 0;
		}
		else if (p.x == 0 || p.x == wnum - 1) {
			if (p.x == 0) {
				p.l == 0;
				if (rh[p.y][p.x] == 1)
					p.r = 1;
				else
					p.r = 0;
			}
			else {
				p.r = 0;
				if (rh[p.y][p.x - 1] == 1)
					p.l = 1;
				else
					p.l = 0;
			}
		}
		if (p.y >= 1 && p.y < hnum - 1) {
			if (rv[p.y - 1][p.x] == 1)
				p.u = 1;
			else
				p.u = 0;
			if (rv[p.y][p.x] == 1)
				p.d = 1;
			else
				p.d = 0;
		}
		else if (p.y == 0 || p.y == hnum - 1) {
			if (p.y == 0) {
				p.u = 0;
				if (rv[p.y][p.x] == 1)
					p.d = 1;
				else
					p.d = 0;
			}
			else {
				p.d = 0;
				if (rv[p.y - 1][p.x] == 1)
					p.u = 1;
				else
					p.u = 0;
			}
		}
		if (i == 1) {
			if (p.l + p.r + p.u + p.d == 1) {
				if (p.l == 1)
					path[i] = 0;
				else if (p.r == 1)
					path[i] = 2;
				else if (p.u == 1)
					path[i] = 3;
				else
					path[i] = 1;
			}
			else if (p.l + p.r + p.u + p.d == 2) {
				if ((p.l == 1) && (p.r == 1))
					path[i] = (rand() % 2 == 0) ? 0 : 2;
				else if ((p.l == 1) && (p.u == 1))
					path[i] = (rand() % 2 == 0) ? 0 : 3;
				else if ((p.l == 1) && (p.d == 1))
					path[i] = (rand() % 2 == 0) ? 0 : 1;
				else if ((p.r == 1) && (p.u == 1))
					path[i] = (rand() % 2 == 0) ? 2 : 3;
				else if ((p.r == 1) && (p.d == 1))
					path[i] = (rand() % 2 == 0) ? 2 : 1;
				else if ((p.u == 1) && (p.d == 1))
					path[i] = (rand() % 2 == 0) ? 3 : 1;
			}
			else if (p.l + p.r + p.u + p.d == 3) {
				if (p.l == 0)
					switch (rand() % 3) {
					case 0:
						path[i] = 2;
						break;
					case 1:
						path[i] = 3;
						break;
					case 2:
						path[i] = 1;
						break;
					}
				else if (p.r == 0)
					switch (rand() % 3) {
					case 0:
						path[i] = 0;
						break;
					case 1:
						path[i] = 3;
						break;
					case 2:
						path[i] = 1;
						break;
					}
				else if (p.u == 0)
					switch (rand() % 3) {
					case 0:
						path[i] = 0;
						break;
					case 1:
						path[i] = 2;
						break;
					case 2:
						path[i] = 1;
						break;
					}
				else if (p.d == 0)
					switch (rand() % 3) {
					case 0:
						path[i] = 0;
						break;
					case 1:
						path[i] = 2;
						break;
					case 2:
						path[i] = 3;
						break;
					}
			}
			else if (p.l + p.r + p.u + p.d == 4)
				switch (rand() % 4) {
				case 0:
					path[i] = 0;
					break;
				case 1:
					path[i] = 2;
					break;
				case 2:
					path[i] = 3;
					break;
				case 3:
					path[i] = 1;
					break;
				}
		}
		else
			do {
				if (p.l + p.r + p.u + p.d == 1) {
					stop = i;
					path[i] = -1;
					break;
				}
				else if (p.l + p.r + p.u + p.d == 2) {
					if ((p.l == 1) && (p.r == 1))
						path[i] = (rand() % 2 == 0) ? 0 : 2;
					else if ((p.l == 1) && (p.u == 1))
						path[i] = (rand() % 2 == 0) ? 0 : 3;
					else if ((p.l == 1) && (p.d == 1))
						path[i] = (rand() % 2 == 0) ? 0 : 1;
					else if ((p.r == 1) && (p.u == 1))
						path[i] = (rand() % 2 == 0) ? 2 : 3;
					else if ((p.r == 1) && (p.d == 1))
						path[i] = (rand() % 2 == 0) ? 2 : 1;
					else if ((p.u == 1) && (p.d == 1))
						path[i] = (rand() % 2 == 0) ? 3 : 1;
				}
				else if (p.l + p.r + p.u + p.d == 3) {
					if (p.l == 0)
						switch (rand() % 3) {
						case 0:
							path[i] = 2;
							break;
						case 1:
							path[i] = 3;
							break;
						case 2:
							path[i] = 1;
							break;
						}
					else if (p.r == 0)
						switch (rand() % 3) {
						case 0:
							path[i] = 0;
							break;
						case 1:
							path[i] = 3;
							break;
						case 2:
							path[i] = 1;
							break;
						}
					else if (p.u == 0)
						switch (rand() % 3) {
						case 0:
							path[i] = 0;
							break;
						case 1:
							path[i] = 2;
							break;
						case 2:
							path[i] = 1;
							break;
						}
					else if (p.d == 0)
						switch (rand() % 3) {
						case 0:
							path[i] = 0;
							break;
						case 1:
							path[i] = 2;
							break;
						case 2:
							path[i] = 3;
							break;
						}
				}
				else if (p.l + p.r + p.u + p.d == 4)
					switch (rand() % 4) {
					case 0:
						path[i] = 0;
						break;
					case 1:
						path[i] = 2;
						break;
					case 2:
						path[i] = 3;
						break;
					case 3:
						path[i] = 1;
						break;
					}
			} while ((i >= 2) && ((path[i] != path[i - 1]) && ((path[i] + path[i - 1]) % 2 == 0)));
			if (stop != NULL) {
				n = stop;
				break;
			}
			//printf("x=%d y=%d path=%d\n", p.x, p.y, path[i]);
			switch (path[i]) {
			case 0:
				p.x--;
				break;
			case 2:
				p.x++;
				break;
			case 3:
				p.y--;
				break;
			case 1:
				p.y++;
				break;
			}
	}
	path[0] = path[1];
	for (int i = 0; i < hnum; i++)
		delete rh[i];
	for (int i = 0; i < hnum - 1; i++)
		delete rv[i];
	delete[]rh;
	delete[]rv;
	while (path == NULL) {
		path=randpath(fullViewport, startp, n);
	}
	return path;
}

void createRandomCar(CarData& car, WindowData& fullViewport, const point& start) {
	int width = fullViewport.w, height = fullViewport.h, wnum = fullViewport.wnum, hnum = fullViewport.hnum;
	int length;
	car.x = 0.5 + start.x;
	car.y = 0.5 + start.y;
	car.path = NULL;
	do {
		car.path = randpath(fullViewport, start, length);
		} while ((car.path == NULL) || (length < 2) || (car.path[0] == -1));
	//for (int i = 0; i <= length; i++)
	//	printf("path[%d]=%d\n", i, car.path[i]);
	car.length = length;
	car.window = &fullViewport;
	car.i = 0;
	car.angle = (3 - car.path[0]) * 90;
	car.type = rand() % 3 + 5;
	car.velocity = 0.002*(rand()%10+5);
	car.intersect = true;
}