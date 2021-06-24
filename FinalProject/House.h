
void createBuilding(Building **&build, const WindowData& fullViewport, int** road) {
	int width = fullViewport.w, height = fullViewport.h;
	int hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	int i, j;

	//If there are buildings, clear all
	if (build) {
		for (i = 0; i < hnum; i++)
			delete[]build[i];
		delete[]build;
		build = NULL;
	}

	//create all building
	build = new Building * [hnum];
	for (i = 0; i < hnum; i++)
		build[i] = new Building[wnum]{ Empty, 0, 0 , Middle };
	for (i = 0; i < hnum; i++)
		for (j = 0; j < wnum; j++) {
			build[i][j].x = j;
			build[i][j].y = i;
			build[i][j].Pos = Middle;
		}

	//random create building
	int c = 1;
	do {
		i = rand() % hnum;
		j = rand() % wnum;
		if (build[i][j].type == Empty && checkRoad(i, j)) {
			if (c < 8)
				build[i][j].type = (BuildType)c;
			else if (c >= 8)
				build[i][j].type = (c%2)?House1:House2;
			c++;
		}
	} while (c < 20);
}

int addBuild(SDL_Renderer* renderer, const WindowData& fullViewport, const MouseState& mouseState, const int mousex, const int mousey, Building** build, ImageData build_pic[], CarData car[]) {
	int width = fullViewport.w, height = fullViewport.h, wnum=fullViewport.wnum, hnum=fullViewport.hnum, xnum, ynum;
	static int choose = 0;
	static SDL_TimerID timerID_clock;
	
	//Left Click
	if (mouseState == IN_LB_SC) {

		//If choose, put down the building
		if (choose) {
			if (mousex > (width-height/12) / wnum / 2 && mousex<(width -height/12- (width - height / 12) / wnum / 2) && mousey >(height / 12 + ((height - height / 12) / hnum) / 2) && mousey < (height - ((height - height / 12) / hnum) / 2)){
				xnum = (mousex- (width - height / 12) / wnum / 2) / ((width-height/12) / wnum);
				ynum = (mousey - height / 12 - (height - height / 12) / hnum / 2) / ((height - height / 12 ) / hnum);
				
				//Check for Empty building and having road
				if (build[ynum][xnum].type == Empty) {
					if (checkRoad(ynum, xnum)) {
						build[ynum][xnum].type = (BuildType)choose;
						//adding more special car
						if (choose == FireSta) {
							int c = 0, i=0;
							while (c < 2&&i<CARNUM) {
								if (car[i].type > 4 || car[i].type == 0) {
									car[i].velocity = -3;
									car[i].type = c + 1;
									car[i].home_xnum = xnum;
									car[i].home_ynum = ynum;
									c++;
								}
								i++;
							}
						}
						else if (choose == Logistics) {
							int i = 0;
							while (car[i].type <= 4 && car[i].type != 0 && i < CARNUM) {
								i++;
							}
							if (i < CARNUM) {
								car[i].velocity = -3;
								car[i].type = 3;
								car[i].home_xnum = xnum;
								car[i].home_ynum = ynum;
							}
						}
						else if (choose==PoliceOff){
							int i = 0;
							while (car[i].type <= 4 && car[i].type != 0 && i < CARNUM) {
								i++;
							}
							if (i < CARNUM) {
								car[i].velocity = -3;
								car[i].type = 4;
								car[i].home_xnum = xnum;
								car[i].home_ynum = ynum;
							}
						}
					}
					else 
						return 1;
				}
				else
					return 2;
			}
		}

		//Choosing which building would be built
		else if (mousex >= (width-height/12) && mousey >= (height/ 12)&&mousey<=(height*10/12)) {
			choose = mousey / (height / 12);
			if (choose == 10)
				choose = 0;
		}
	}
	
	//Right Click for releasing building
	else if (mouseState == IN_RB_SC) {
		choose = 0;
	}

	//Fixed chosen building on mouse
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
			case 8:
				imgRender(renderer, build_pic[0], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;
			case 9:
				imgRender(renderer, build_pic[1], Middle, mousex, mousey, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;

		}
	}
	return 0;
}

void buildRender(SDL_Renderer* renderer, const WindowData& fullViewport, Building** build, ImageData build_pic[]) {
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

	//Render the building
	for (int i = 0; i < hnum; i++) {
		for (int j = 0; j < wnum; j++) {
			int x, y;
			x = (double)(width - height / 12) / (wnum + 1) * (build[i][j].x + 1);
			y = height / 12 + (double)(height - height / 12) / (hnum + 1) * (build[i][j].y + 1);
			
			if (build[i][j].type >=8) {
				t = build[i][j].type - 8;
				imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width-height/12)/ (wnum + 1)*8/10, (height - height / 12 ) / (hnum + 1) *4/6* build_pic[t].width / build_pic[t].height), (height - height / 12) / (hnum + 1) * 4 / 6, 1, NULL, NULL, 0, no, 255);
			}
			else {
				//If there is a special building
				if (build[i][j].type) {
					t = (i + j) % 2 + 2;
					imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), (height - height / 12) / (hnum + 1) * 4 / 6, 1, NULL, NULL, 0, no, 255);
				}
				switch (build[i][j].type) {
					
					//Fire Station
					case 1:
						t = 4;
						imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height),NULL,1, NULL, NULL, 0, no, 255);
						break;
					
					//Logistics
					case 2:
						t = 5;
						imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height),NULL,  1,NULL, NULL, 0, no, 255);
						break;
					
					//Police Office
					case 3:
						t=6;
						imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), NULL,1,NULL, NULL, 0, no, 255);
						break;
					
					//Factory
					case 4:
						break;
					
					//Shopping Mall
					case 5:
						t = 7;
						imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), NULL, 1, NULL, NULL, 0, no, 255);
						break;
					
					//School
					case 6:
						t = 8;
						imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height),NULL,1, NULL, NULL, 0, no, 255);
						break;
					
					//Hospital
					case 7:
						t = 9;
						imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width-height/12)/ (wnum + 1)*8/10, (height - height / 12 ) / (hnum + 1) *4/6* build_pic[t].width / build_pic[t].height), NULL,1, NULL,NULL, 0, no, 255);
						break;
				}
			}
		}
	}
}