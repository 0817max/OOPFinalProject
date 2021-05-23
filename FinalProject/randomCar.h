struct point {
	int x, y;
	bool vertical;
	bool horizontal;
	bool exist;
	int l, r, u, d;
};
point randomstart(WindowData& fullViewport, int road[6][8]) {
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
	for (int j = 0; j < wnum; j++) //vertical road
		for (int i = 0; i < hnum - 1; i++) {
			if (((road[i][j] >> 4) % 2) * ((road[i + 1][j] >> 12) % 2))
				rv[i][j] = 1;
			else
				rv[i][j] = 0;
		}
	point p;
	p.exist = false;
	while (!p.exist) {
		p.x = rand() % wnum;
		p.y = rand() % hnum;
		p.vertical = 0;
		p.horizontal = 0;
		p.exist = false;
		if (p.x >= 1 && p.x < wnum - 1) {
			if (rh[p.y][p.x - 1] == 1) {
				p.horizontal = 1;
				p.exist = 1;
			}
			if (rh[p.y][p.x] == 1) {
				p.horizontal = 1;
				p.exist = 1;
			}
		}
		else if (p.x == 0 || p.x == wnum - 1) {
			if (p.x == 0) {
				if (rh[p.y][p.x] == 1) {
					p.horizontal = 1;
					p.exist = 1;
				}
			}
			else {
				if (rh[p.y][p.x - 1] == 1) {
					p.horizontal = 1;
					p.exist = 1;
				}
			}
		}
		if (p.y >= 1 && p.y < hnum - 1) {
			if (rv[p.y - 1][p.x] == 1) {
				p.vertical = 1;
				p.exist = 1;
			}
			if (rv[p.y][p.x] == 1) {
				p.vertical = 1;
				p.exist = 1;
			}
		}
		else if (p.y == 0 || p.y == hnum - 1) {
			if (p.y == 0) {
				if (rv[p.y][p.x] == 1) {
					p.vertical = 1;
					p.exist = 1;
				}
			}
			else {
				if (rv[p.y - 1][p.x] == 1) {
					p.vertical = 1;
					p.exist = 1;
				}
			}
		}
	}
	for (int i = 0; i < hnum; i++)
		delete rh[i];
	for (int i = 0; i < hnum - 1; i++)
		delete rv[i];
	delete[]rh;
	delete[]rv;
	return p;
}
char* randpath(WindowData& fullViewport, point startp, int& n) {
	n = 1 + rand() % 15;
	int stop = NULL;
	char* path = new char[n];
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
	for (int i = 0; i < n; i++) {
		if (p.x<0 || p.x>wnum || p.y<0 || p.y>hnum) {
			path[i] = -1;
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
		if (i == 0) {

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
			} while ((i >= 1) && ((path[i] != path[i - 1]) && ((path[i] + path[i - 1]) % 2 == 0)));
			printf("x=%d y=%d path=%d\n", p.x, p.y, path[i]);
			if (stop != NULL) {
				n = stop;
				break;
			}
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
	for (int i = 0; i < hnum; i++)
		delete rh[i];
	for (int i = 0; i < hnum - 1; i++)
		delete rv[i];
	delete[]rh;
	delete[]rv;
	return path;
}
void createRandomCar(CarData& car, WindowData& fullViewport, bool windowChange, point start, int house_hnum, int house_wnum, int type, ImageData car_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, wnum = fullViewport.wnum, hnum = fullViewport.hnum, oldw = fullViewport.oldw, oldh = fullViewport.oldh;
	if (windowChange && car.velocity >= 0) {
		if (!car.intersect) {
			if (car.path[car.i] % 2) {
				car.x = (int)car.x + 0.5;
			}
			else {
				car.y = (int)car.y + 0.5;
			}
		}
		else {
			if ((car.path[car.i] + 3) % 4 == car.path[car.i + 1]) {   //turn right
				car.x = (int)car.x + 0.5;
				car.y = (int)car.y + 0.5;
				(car.angle) = (car.angle + 5 + 360) % 360;
				switch (car.path[car.i]) {
				case 0:			//270-360 sin<0, cos>0
					car.x -= (1. / 10) * sin(car.angle * M_PI / 180);
					car.y -= (1. / 6) * cos(car.angle * M_PI / 180);
					break;
				case 1:			//180-270 sin<0 cos<0
					car.x += (1. / 10) * sin(car.angle * M_PI / 180);
					car.y += (1. / 6) * cos(car.angle * M_PI / 180);
					break;
				case 2:			//90-180 sin>0 cos<0
					car.x -= (1. / 10) * sin(car.angle * M_PI / 180);
					car.y -= (1. / 6) * cos(car.angle * M_PI / 180);
					break;
				case 3:			//0-90 sin>0 cos>0
					car.x += (1. / 10) * sin(car.angle * M_PI / 180);
					car.y += (1. / 6) * cos(car.angle * M_PI / 180);
					break;
				}

			}
			else if ((car.path[car.i] + 1) % 4 == car.path[car.i + 1]) {  //turn left
				car.x = (int)car.x + 0.5;
				car.y = (int)car.y + 0.5;
				(car.angle) = (car.angle - 5 + 360) % 360;
				switch (car.path[car.i]) {
				case 0:			//270-180 sin<0, cos<0
					car.x -= (1. / 10) * sin(car.angle * M_PI / 180);
					car.y -= (1. / 6) * cos(car.angle * M_PI / 180);
					break;
				case 1:			//180-90 sin>0 cos<0
					car.x += (1. / 10) * sin(car.angle * M_PI / 180);
					car.y += (1. / 6) * cos(car.angle * M_PI / 180);
					break;
				case 2:			//90-0 sin>0 cos>0
					car.x -= (1. / 10) * sin(car.angle * M_PI / 180);
					car.y -= (1. / 6) * cos(car.angle * M_PI / 180);
					break;
				case 3:			//0-270 sin<0 cos>0
					car.x += (1. / 10) * sin(car.angle * M_PI / 180);
					car.y += (1. / 6) * cos(car.angle * M_PI / 180);
					break;
				}
			}
			else {
				if (car.path[car.i] % 2) {
					car.x = (int)car.x + 0.5;
				}
				else {
					car.y = (int)car.y + 0.5;
				}
			}
		}
		car.velocity = sqrt(car.velocity * car.velocity * width * height / oldw / oldh);
		return;
	}
	int length;
	car.x = start.x;
	car.y = start.y;
	car.path = NULL;
	do {
		car.path = randpath(fullViewport, start, length);
	} while ((car.path == NULL) || (length < 1) || (car.path[0] == -1));
	for (int i = 0; i < length; i++)
		printf("path[%d]=%d\n", i, car.path[i]);
	car.length = length;
	car.window = &fullViewport;
	car.img = &(car_pic[type]);
	car.i = 0;
	car.angle = (3 - car.path[0]) * 90;
	car.velocity = 0.01;
	car.intersect = false;
}