enum BuildType { Empty=0, FireSta=1, Logistics=2, PoliceOff=3, Factory=4, Shopping=5, School=6, Hospital=7, House=8 };
struct Building {
	BuildType type;
	int x, y;
	PosPoint Pos;
};

void createBuilding(Building **&build, WindowData fullViewport,int road[6][8]) {
	int width = fullViewport.w, height = fullViewport.h;
	int hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	int i, j;
	
	if (build) {
		for (i = 0; i < hnum; i++)
			for (j = 0; j < wnum; j++) {
				build[i][j].x = (width-height/12) / (wnum + 1) * (j + 1);
				build[i][j].y = height / 12 + (height - height / 12) / (hnum + 1) * (i + 1);
			}
		return;
	}
	build = new Building * [hnum];
	for (i = 0; i < hnum; i++)
		build[i] = new Building[wnum]{ Empty, 0, 0 , Middle };
	for (i = 0; i < hnum; i++)
		for (j = 0; j < wnum; j++) {
			build[i][j].x = (width - height / 12) /(wnum+1)* (j + 1);
			build[i][j].y = height / 12 + (height - height / 12)/(hnum+1) * (i + 1);
			build[i][j].Pos = Middle;
		}
	int c = 1;
	do {
		i = rand() % hnum;
		j = rand() % wnum;
		if (build[i][j].type == Empty && checkRoad(i, j)) {
			if (c < 8)
				build[i][j].type = (BuildType)c;
			else if (c >= 8)
				build[i][j].type = House;
			c++;
		}
	} while (c < 20);
}

int addBuild(SDL_Renderer* renderer, WindowData fullViewport, MouseState mouseState, int mousex, int mousey, Building** build, ImageData build_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, wnum=fullViewport.wnum, hnum=fullViewport.hnum, xnum, ynum;
	static int choose = 0;
	static SDL_TimerID timerID_clock;
	if (mouseState == IN_LB_SC) {
		if (choose) {
			if (mousex > (width-height/12) / wnum / 2 && mousex<(width -height/12- (width - height / 12) / wnum / 2) && mousey >(height / 12 + ((height - height / 12) / hnum) / 2) && mousey < (height - ((height - height / 12) / hnum) / 2)){
				xnum = (mousex- (width - height / 12) / wnum / 2) / ((width-height/12) / wnum);
				ynum = (mousey - height / 12 - (height - height / 12) / hnum / 2) / ((height - height / 12 ) / hnum);
				if (build[ynum][xnum].type == Empty) {
					if (checkRoad(ynum, xnum)) {
						build[ynum][xnum].type = (BuildType)choose;
					}
					else 
						return 1;
				}
				else
					return 2;
			}
		}
		else if (mousex >= (width-height/12) && mousey >= (height/ 12)&&mousey<=(height*8/12)) {
			choose = mousey / (height / 12);
			if (choose == 8)
				choose = 0;
		}
	}
	else if (mouseState == IN_RB_SC) {
		choose = 0;
	}
	else {
		switch(choose) {
			case 1:
				imgRender(renderer, build_pic[4], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;
			case 2:
				imgRender(renderer, build_pic[5], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;
			case 3:
				imgRender(renderer, build_pic[6], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;
			case 4:
				imgRender(renderer, build_pic[3], Middle, mousex, mousey, height / 12, NULL, 1, NULL, NULL, 0, no, 100);
				break;																	
			case 5:																		
				imgRender(renderer, build_pic[7], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;																	
			case 6:																		
				imgRender(renderer, build_pic[8], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;																	
			case 7:																		
				imgRender(renderer, build_pic[9], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;
		}
	}
	return 0;
}

void buildRender(SDL_Renderer* renderer, WindowData fullViewport, Building** build, ImageData build_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum=fullViewport.hnum-1, wnum=fullViewport.wnum-1, t;

	//wnum==null, delete dynamic array
	if (wnum == NULL) {
		for (int i = 0; i < hnum; i++)
			delete[]build[i];
		delete[]build;
		return;
	}

	//free resources
	if (renderer == NULL) {
		for(int i=0; i<9; i++)
			SDL_DestroyTexture(build_pic[i].texture);
		for (int i = 0; i < hnum; i++)
			delete[]build[i];
		delete[]build;
		return;
	}

	for (int i = 0; i < hnum; i++) {
		for (int j = 0; j < wnum; j++) {
			if (build[i][j].type == 8) {
				t = (i + j) % 2;
				imgRender(renderer, build_pic[t], build[i][j].Pos, build[i][j].x, build[i][j].y, fmin((width-height/12)/ (wnum + 1)*8/10, (height - height / 12 ) / (hnum + 1) *4/6* build_pic[t].width / build_pic[t].height), (height - height / 12) / (hnum + 1) * 4 / 6, 1, NULL, NULL, 0, no, 255);
			}
			else {
				if (build[i][j].type) {
					t = (i + j) % 2 + 2;
					imgRender(renderer, build_pic[t], build[i][j].Pos, build[i][j].x, build[i][j].y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), (height - height / 12) / (hnum + 1) * 4 / 6, 1, NULL, NULL, 0, no, 255);
				}
				switch (build[i][j].type) {
					case 1:
						t = 4;
						imgRender(renderer, build_pic[t], build[i][j].Pos, build[i][j].x, build[i][j].y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height),NULL,1, NULL, NULL, 0, no, 255);
						break;
					case 2:
						t = 5;
						imgRender(renderer, build_pic[t], build[i][j].Pos, build[i][j].x, build[i][j].y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height),NULL,  1,NULL, NULL, 0, no, 255);
						break;
					case 3:
						t=6;
						imgRender(renderer, build_pic[t], build[i][j].Pos, build[i][j].x, build[i][j].y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), NULL,1,NULL, NULL, 0, no, 255);
						break;
					case 4:
						build[i][j].type = Factory;
						break;
					case 5:
						t = 7;
						imgRender(renderer, build_pic[t], build[i][j].Pos, build[i][j].x, build[i][j].y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), NULL, 1, NULL, NULL, 0, no, 255);
						break;
					case 6:
						t = 8;
						imgRender(renderer, build_pic[t], build[i][j].Pos, build[i][j].x, build[i][j].y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height),NULL,1, NULL, NULL, 0, no, 255);
						break;
					case 7:
						t = 9;
						imgRender(renderer, build_pic[t], build[i][j].Pos, build[i][j].x, build[i][j].y, fmin((width-height/12)/ (wnum + 1)*8/10, (height - height / 12 ) / (hnum + 1) *4/6* build_pic[t].width / build_pic[t].height), NULL,1, NULL,NULL, 0, no, 255);
						break;
				}
			}
		}
	}
}