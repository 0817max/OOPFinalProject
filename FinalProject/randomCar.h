struct point {
	int x, y;
	bool exist;
	int l, r, u, d;
};

point randomstart(const WindowData& fullViewport, int** road);
void createRandomCar(CarData& car, WindowData& fullViewport, const point& start);

int initCar(WindowData* fullViewport, CarData** car, Building **build, int randomcarnum) {
	point* start = new point[randomcarnum];
	int carnum = fullViewport->carnum;
	bool repeat = false;
	int count = 0, i;
	if (*car) {
		delete[](*car);
		(*car) = NULL;
	}
	*car = new CarData[carnum];		
	for (i = 0; i < carnum; i++) {
		(*car)[i].x = (*car)[i].y = 0;
		(*car)[i].window = fullViewport;
		(*car)[i].path = NULL;
		(*car)[i].i = (*car)[i].length = (*car)[i].angle = (*car)[i].type = 0;
		(*car)[i].home_x = (*car)[i].home_y = 0;
		(*car)[i].velocity = -1;
		(*car)[i].intersect = false;
	}
	i = 0;
	for (int x = 0; x < fullViewport->wnum - 1; x++)
		for (int y = 0; y < fullViewport->hnum - 1; y++)
			if (build[y][x].type == FireSta) {
				while ((*car)[i].type)
					i++;
				(*car)[i].type = 1;
				(*car)[i].velocity = -1;
				(*car)[i].home_x = x+1;
				(*car)[i].home_y = y+1;
				while ((*car)[i].type)
					i++;
				(*car)[i].type = 2;
				(*car)[i].velocity = -1;
				(*car)[i].home_x = x + 1;
				(*car)[i].home_y = y + 1;

			}
			else if (build[y][x].type == Logistics) {
				while ((*car)[i].type)
					i++;
				(*car)[i].type = 3;
				(*car)[i].velocity = -1;
				(*car)[i].home_x = x + 1;
				(*car)[i].home_y = y + 1;
			}
			else if (build[y][x].type == PoliceOff) {
				while ((*car)[i].type)
					i++;
				(*car)[i].type = 4;
				(*car)[i].velocity = -1;
				(*car)[i].home_x = x + 1;
				(*car)[i].home_y = y + 1;
			}
	for (int i = 0; i < randomcarnum; i++) {
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
	for (int i = 0; i < randomcarnum; i++)
		createRandomCar((*car)[carnum- randomcarnum +i], *fullViewport, start[i]);
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
			if (road[p.y][p.x]%2)
				p.l = 1;
			else
				p.l = 0;
			if ((road[p.y][p.x]>>8) % 2)
				p.r = 1;
			else
				p.r = 0;
		}
		else if (p.x == 0 || p.x == wnum - 1) {
			if (p.x == 0) {
				p.l == 0;
				if ((road[p.y][p.x] >> 8) % 2)
					p.r = 1;
				else
					p.r = 0;
			}
			else {
				p.r = 0;
				if (road[p.y][p.x]% 2)
					p.l = 1;
				else
					p.l = 0;
			}
		}
		if (p.y >= 1 && p.y < hnum - 1) {
			if ((road[p.y][p.x] >> 12) % 2)
				p.u = 1;
			else
				p.u = 0;
			if ((road[p.y][p.x] >> 4) % 2)
				p.d = 1;
			else
				p.d = 0;
		}
		else if (p.y == 0 || p.y == hnum - 1) {
			if (p.y == 0) {
				p.u = 0;
				if ((road[p.y][p.x] >> 4) % 2)
					p.d = 1;
				else
					p.d = 0;
			}
			else {
				p.d = 0;
				if ((road[p.y][p.x] >> 12) % 2)
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
	//for (int i = 0; i <= length; i++)
	//	printf("path[%d]=%d\n", i, car.path[i]);
		car.length = length;
		car.window = &fullViewport;
		car.i = 0;
	} while ((car.path == NULL) || (length < 2) || (car.path[0] == -1));
	car.angle = (3 - car.path[0]) * 90;
	car.type = rand() % 3 + 5;
	car.velocity = 0.002*(rand()%10+5);
	car.intersect = true;
}