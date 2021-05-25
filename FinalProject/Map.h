//0x: up right bottom left
int road[6][8] = {
	{0x0000,0x0010,0x0000,0x0010,0x0000,0x0000,0x0110,0x0111},
	{0x0000,0x1110,0x0101,0x1011,0x0100,0x0101,0x1011,0x0000},
	{0x0000,0x1000,0x0000,0x1110,0x0101,0x0101,0x1001,0x0000},
	{0x0100,0x0111,0x0101,0x1101,0x0101,0x0111,0x0101,0x0011},
	{0x0000,0x1010,0x0000,0x0100,0x0111,0x1001,0x0000,0x1010},
	{0x0000,0x1000,0x0100,0x0101,0x1001,0x0100,0x0101,0x1001}
};

void mapRender(SDL_Renderer* renderer, WindowData fullViewport, ImageData road_pic, ImageData road_pic1) {
	int width = fullViewport.w, height=fullViewport.h, wnum=fullViewport.wnum, hnum=fullViewport.hnum;
	
	//free up resource
	if (renderer == NULL) {
		SDL_DestroyTexture(road_pic.texture);
		SDL_DestroyTexture(road_pic1.texture);
		return;
	}

	//background color
	boxColor(renderer, 0, height/12, width-height/12, height, 0xFF003333);
	
	//draw horizontal road
	for (int i = 0; i < hnum; i++) {
		for (int j = 0; j < wnum - 1; j++) {
			if (((road[i][j] >> 8) % 2) * (road[i][j + 1] % 2))
				imgRender(renderer, road_pic,  Middle, (double)(width-height/12)/ wnum * (j + 1), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5) , (width - height / 12) / wnum, (height - height / 12) / hnum/3.,1,NULL, NULL, 0, no, 255);
		}
	}

	//draw vertical road
	for (int j = 0; j < wnum; j++) {
		for (int i = 0; i < hnum - 1; i++) {
			if (((road[i][j] >> 4) % 2) * ((road[i + 1][j] >> 12) % 2))
				imgRender(renderer, road_pic1,  Middle, (double)(width - height / 12) / wnum * (j + 0.5), height / 12 + (double)(height - height / 12) / hnum * (i + 1) , (width - height / 12) / wnum /5., (height - height / 12) / hnum, 1,NULL, NULL, 0, no, 255);
		}
	}

	//intersect for more than two road connection
	for (int i = 0; i < hnum; i++)
		for (int j = 0; j < wnum; j++)
			//Top
			if (i == 0)
				//Left
				if (j == 0) {
					if (((road[i][j] >> 8) % 2) & ((road[i][j] >> 4) % 2))
						boxColor(renderer, (double)(width - height / 12) / wnum * (j + 0.5-1./10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5-1./6) , (double)(width - height / 12) / wnum * (j + 0.5+1./10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5+1./6), 0xFF4C4C4C);
				}
				//Right
				else if (j == wnum - 1) {
					if (((road[i][j] >> 4) % 2) & (road[i][j] % 2))
						boxColor(renderer, (double)(width - height / 12) / wnum * (j + 0.5 - 1. / 10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5 - 1. / 6), (double)(width - height / 12) / wnum * (j + 0.5 + 1. / 10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5 + 1. / 6), 0xFF4C4C4C);
				}
				else {
					if ((((road[i][j] >> 8) % 2) + ((road[i][j] >> 4) % 2) + (road[i][j] % 2)) >= 2)	
						boxColor(renderer, (double)(width - height / 12) / wnum * (j + 0.5 - 1. / 10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5 - 1. / 6), (double)(width - height / 12) / wnum * (j + 0.5 + 1. / 10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5 + 1. / 6), 0xFF4C4C4C);
				}
			//bottom
			else if (i == hnum - 1)
				//Left
				if (j == 0) {
					if (((road[i][j] >> 12) % 2) & ((road[i][j] >> 8) % 2))
						boxColor(renderer, (double)(width - height / 12) / wnum * (j + 0.5 - 1. / 10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5 - 1. / 6), (double)(width - height / 12) / wnum * (j + 0.5 + 1. / 10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5 + 1. / 6), 0xFF4C4C4C);
				}
				//Right
				else if (j == wnum - 1) {
					if (((road[i][j] >> 12) % 2) & (road[i][j] % 2))
						boxColor(renderer, (double)(width - height / 12) / wnum * (j + 0.5 - 1. / 10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5 - 1. / 6), (double)(width - height / 12) / wnum * (j + 0.5 + 1. / 10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5 + 1. / 6), 0xFF4C4C4C);
				}
				else {
					if ((((road[i][j] >> 12) % 2) + ((road[i][j] >> 8) % 2) + (road[i][j] % 2)) >= 2)
						boxColor(renderer, (double)(width - height / 12) / wnum * (j + 0.5 - 1. / 10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5 - 1. / 6), (double)(width - height / 12) / wnum * (j + 0.5 + 1. / 10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5 + 1. / 6), 0xFF4C4C4C);
				}
			//Other place
			else {
				if ((((road[i][j] >> 12) % 2) + ((road[i][j] >> 8) % 2) + ((road[i][j] >> 4) % 2) + (road[i][j] % 2)) >= 2)
					boxColor(renderer, (double)(width - height / 12) / wnum * (j + 0.5 - 1. / 10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5 - 1. / 6), (double)(width - height / 12) / wnum * (j + 0.5 + 1. / 10), height / 12 + (double)(height - height / 12) / hnum * (i + 0.5 + 1. / 6), 0xFF4C4C4C);
			}
}

int checkRoad(int y_num, int x_num) {
	int t = 0;
	if (((road[y_num][x_num] >> 8) % 2) * ((road[y_num][x_num + 1]) % 2))
		t += 8; //top horizontal
	if (((road[y_num][x_num + 1] >> 4) % 2) * ((road[y_num + 1][x_num + 1] >> 12) % 2))
		t += 4; //right verticle
	if (((road[y_num + 1][x_num] >> 8) % 2) * ((road[y_num + 1][x_num + 1]) % 2))
		t += 2; //bottom horizontal
	if (((road[y_num][x_num] >> 4) % 2) * ((road[y_num + 1][x_num] >> 12) % 2))
		t += 1; //left verticle
	return t;
}

void destroyRoad(int road[6][8], int hnum, int wnum) {
	for (int i = 0; i < hnum-1 ; i++)
		for (int j = 0; j < wnum-1 ; j++) {
			//Top Road
			if ((road[i][j] >> 8) % 2)
				if (!(road[i][j + 1] % 2))
					road[i][j] -= (1 << 8);
			if (road[i][j + 1] % 2)
				if (!((road[i][j] >> 8) % 2))
					road[i][j + 1] -= 1;

			//Left Road
			if ((road[i][j] >> 4) % 2)
				if (!((road[i + 1][j] >> 12) % 2))
					road[i][j] -= (1 << 4);
			if ((road[i + 1][j] >> 12) % 2)
				if (!((road[i][j] >> 4) % 2))
					road[i + 1][j] -= (1 << 12);
			
			//Bottom Road
			if ((road[i + 1][j] >> 8) % 2)
				if (!(road[i + 1][j + 1] % 2))
					road[i + 1][j] -= (1 << 8);
			if (road[i + 1][j + 1] % 2)
				if (!((road[i + 1][j] >> 8) % 2))
					road[i + 1][j + 1] -= 1;

			//Right Road
			if ((road[i][j + 1] >> 4) % 2)
				if (!((road[i + 1][j + 1] >> 12) % 2))
					road[i][j + 1] -= (1 << 4);
			if ((road[i + 1][j + 1] >> 12) % 2)
				if (!((road[i][j + 1] >> 4) % 2))
					road[i + 1][j + 1] -= (1 << 12);
		}
}

int checkIntersect(WindowData fullViewport, int road[6][8], double y, double x) {
	int width = fullViewport.w, height = fullViewport.h, wnum = fullViewport.wnum, hnum = fullViewport.hnum;
	if (x < 0.5 || y < 0.5||x>wnum-0.5||y>hnum-0.5)
		return 1;
	else {
		double x_remain, y_remain;
		x_remain = x-(int)x-0.5;
		y_remain = y-(int)y-0.5;

		//In the middle(Road intersection)
		if (fabs(x_remain) < (1. / 10) && fabs(y_remain) < (1. / 6))
			return 1;
		else
			return 0;
	}
}