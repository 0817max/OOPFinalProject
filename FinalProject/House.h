void createBuilding(Building **&build, const WindowData& fullViewport, int** road, int num) {
	int width = fullViewport.w, height = fullViewport.h;
	int hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	int i, j;

	//If there are buildings, clear all
	if (num==0) {
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
	} while (c < num);
}

int addBuild(SDL_Renderer* renderer, const WindowData& fullViewport, const Mouse& mouse, int& money, Building** build, ImageData build_pic[], CarData car[]) {
	int width = fullViewport.w, height = fullViewport.h, wnum=fullViewport.wnum, hnum=fullViewport.hnum, xnum, ynum;
	static int choose = 0;
	static SDL_TimerID timerID_clock;
	
	//Left Click
	if (mouse.state == IN_LB_SC) {

		//If choose, put down the building
		if (choose) {
			if (mouse.X > (width-height/12) / wnum / 2 && mouse.X<(width -height/12- (width - height / 12) / wnum / 2) && mouse.Y >(height / 12 + ((height - height / 12) / hnum) / 2) && mouse.Y < (height - ((height - height / 12) / hnum) / 2)){
				xnum = (mouse.X- (width - height / 12) / wnum / 2) / ((width-height/12) / wnum);
				ynum = (mouse.Y- height / 12 - (height - height / 12) / hnum / 2) / ((height - height / 12 ) / hnum);
				
				//Check for Empty building and having road
				if (build[ynum][xnum].type == Empty) {
					if (checkRoad(ynum, xnum)) {
						if (choose && choose <= 7) {
							if (money < 200)
								return 5;
							else
								money -= 200;
						}
						else {
							if (money < 150)
								return 5;
							else
								money -= 150;
						}
						build[ynum][xnum].type = (BuildType)choose;
						//adding more special car
						if (choose == FireSta) {
							int c = 0, i=0;
							while (c < 2&&i<CARNUM) {
								if (car[i].type > 4 || car[i].type == 0) {
									car[i].velocity = -3;
									car[i].type = c + 1;
									car[i].home_x = xnum+1;
									car[i].home_y = ynum+1;
									car[i].x = car[i].y = 0;
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
								car[i].home_x = xnum+1;
								car[i].home_y = ynum+1;
								car[i].x = car[i].y = 0;
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
								car[i].home_x = xnum+1;
								car[i].home_y = ynum+1;
								car[i].x = car[i].y = 0;
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
		else if (mouse.X >= (width-height/12) && mouse.Y >= (height/ 12)&&mouse.Y<=(height*10/12)) {
			choose = mouse.Y / (height / 12);
			if (choose == 10)
				choose = 0;
		}
	}
	
	//Right Click for releasing building
	else if (mouse.state == IN_RB_SC) {
		choose = 0;
	}

	//Fixed chosen building on mouse
	else {
		if (mouse.X >= (width -height / 12) && mouse.Y >= (height / 12.) && mouse.Y < (height * 10. / 12)) {
			boxColor(renderer, width - height / 12+ height / 1000 + 1, (mouse.Y / (height / 12)) * height / 12+ height / 1000 + 1, width, (mouse.Y / (height / 12) + 1) * height / 12- height / 1000 - 1, 0x22FFFFFF);
		}
		else if (choose) {
			if (mouse.X > (width - height / 12) / wnum / 2 && mouse.X<(width - height / 12 - (width - height / 12) / wnum / 2) && mouse.Y >(height / 12 + ((height - height / 12) / hnum) / 2) && mouse.Y < (height - ((height - height / 12) / hnum) / 2)) {
				xnum = (mouse.X - (width - height / 12) / wnum / 2) / ((width - height / 12) / wnum);
				ynum = (mouse.Y - height / 12 - (height - height / 12) / hnum / 2) / ((height - height / 12) / hnum);
				boxColor(renderer, (double)(width - height / 12) / wnum*(0.5+xnum+1/10.), height / 12+ ((double)(height - height / 12) / hnum)*(ynum+0.5+1/6.), (double)(width - height / 12) / wnum * (1.5 + xnum-1/10.), height / 12 + ((double)(height - height / 12) / hnum) * (ynum + 1.5-1/6.),0x22FFFFFF);
			}
			switch (choose) {
			case 1:
				imgRender(renderer, build_pic[4], Middle, mouse.X, mouse.Y, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;
			case 2:
				imgRender(renderer, build_pic[5], Middle, mouse.X, mouse.Y, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;
			case 3:
				imgRender(renderer, build_pic[6], Middle, mouse.X, mouse.Y, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;
			case 4:
				imgRender(renderer, build_pic[3], Middle, mouse.X, mouse.Y, height / 12, NULL, 1, NULL, NULL, 0, no, 100);
				break;
			case 5:
				imgRender(renderer, build_pic[7], Middle, mouse.X, mouse.Y, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;
			case 6:
				imgRender(renderer, build_pic[8], Middle, mouse.X, mouse.Y, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;
			case 7:
				imgRender(renderer, build_pic[9], Middle, mouse.X, mouse.Y, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;
			case 8:
				imgRender(renderer, build_pic[0], Middle, mouse.X, mouse.Y, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;
			case 9:
				imgRender(renderer, build_pic[1], Middle, mouse.X, mouse.Y, NULL, height / 12, 1, NULL, NULL, 0, no, 100);
				break;

			}
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