int houserequire(ValueData& value, int type);
void createBuilding(Building **&build, const WindowData& fullViewport, int** road, int num, int buildnum[]) {
	int width = fullViewport.w, height = fullViewport.h;
	int hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	int i, j;

	for (int i = 0; i < 9; i++)
		buildnum[i] = 0;
	//If there are buildings, clear all
	if (num==0) {
		for (i = 0; i < hnum; i++)
			delete[]build[i];
		delete[]build;
		build = NULL;
		return;
	}

	//create all building
	if (!build) {
		build = new Building * [hnum];
		for (i = 0; i < hnum; i++)
			build[i] = new Building[wnum]{ Empty, 0, 0 , Middle };
	}
	for (i = 0; i < hnum; i++)
		for (j = 0; j < wnum; j++) {
			build[i][j].type = Empty;
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
			if (c < 4) {
				build[i][j].type = (BuildType)c;
				buildnum[c-1]++;
			}
			else if (c >= 4) {
				if (c % 2) {
					build[i][j].type = House1;
					buildnum[7]++;
				}
				else {
					build[i][j].type = House2;
					buildnum[8]++;
				}
			}
			c++;
		}
	} while (c < num);
}

void createBuilding1(Building**& build, const WindowData& fullViewport, int** road, int num, int buildnum[]) {
	int width = fullViewport.w, height = fullViewport.h;
	int hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	int i, j;

	for (int i = 0; i < 9; i++)
		buildnum[i] = 0;
	//If there are buildings, clear all
	if (num == 0) {
		for (i = 0; i < hnum; i++)
			delete[]build[i];
		delete[]build;
		build = NULL;
		return;
	}

	//create all building
	if (!build) {
		build = new Building * [hnum];
		for (i = 0; i < hnum; i++)
			build[i] = new Building[wnum]{ Empty, 0, 0 , Middle };
	}
	for (i = 0; i < hnum; i++)
		for (j = 0; j < wnum; j++) {
			build[i][j].type = Empty;
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
			build[i][j].type = (BuildType)(rand() % 9 + 1);
			c++;
		}
	} while (c < num);
}

void addBuild(SDL_Renderer* renderer, const WindowData& fullViewport,const bool windowChange, const Mouse& mouse, ValueData& value, Building** build, ImageData build_pic[], CarData car[], InciData& inci) {
	int width = fullViewport.w, height = fullViewport.h, wnum=fullViewport.wnum, hnum=fullViewport.hnum, xnum, ynum, carnum = fullViewport.carnum;
	static int choose = 0;
	static TextData textmoney = loadTextTexture(renderer, "Money: ", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 2.25 / 12, 0, 0, 0, BLENDED);
	static TextData textlove = loadTextTexture(renderer, "Happiness: ", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
	static TextData text25 = loadTextTexture(renderer, "25", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
	static TextData text100 = loadTextTexture(renderer, "100", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
	static TextData text150 = loadTextTexture(renderer, "150", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
	static TextData text200 = loadTextTexture(renderer, "200", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
	static TextData text300 = loadTextTexture(renderer, "300", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
	static TextData text350 = loadTextTexture(renderer, "350", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
	static TextData text400 = loadTextTexture(renderer, "400", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
	
	//free up resources
	if (renderer == NULL) {
		SDL_DestroyTexture(textmoney.texture);
		SDL_DestroyTexture(textlove.texture);
		SDL_DestroyTexture(text25.texture);
		SDL_DestroyTexture(text100.texture);
		SDL_DestroyTexture(text150.texture);
		SDL_DestroyTexture(text200.texture);
		SDL_DestroyTexture(text300.texture);
		SDL_DestroyTexture(text350.texture);
		SDL_DestroyTexture(text400.texture);
		choose = 0;
		return;
	}
	else if (windowChange) {
		SDL_DestroyTexture(textmoney.texture);
		SDL_DestroyTexture(textlove.texture);
		SDL_DestroyTexture(text25.texture);
		SDL_DestroyTexture(text100.texture);
		SDL_DestroyTexture(text150.texture);
		SDL_DestroyTexture(text200.texture);
		SDL_DestroyTexture(text300.texture);
		SDL_DestroyTexture(text350.texture);
		SDL_DestroyTexture(text400.texture);
		textmoney = loadTextTexture(renderer, "Money: ", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 2.25 / 12, 0, 0, 0, BLENDED);
		textlove = loadTextTexture(renderer, "Happiness: ", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
		text25 = loadTextTexture(renderer, "25", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
		text100 = loadTextTexture(renderer, "100", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
		text150 = loadTextTexture(renderer, "150", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
		text200 = loadTextTexture(renderer, "200", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
		text300 = loadTextTexture(renderer, "300", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
		text350 = loadTextTexture(renderer, "350", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
		text400 = loadTextTexture(renderer, "400", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 0, 0, 0, BLENDED);
	}

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
						if (houserequire(value, choose)) {
							inci.addhouse = houserequire(value, choose);
							inci.alpha[0] = 450;
						}
						else {
							switch (choose) {
							case 1:
							case 2:
							case 3:
								value.money -= 200;
								break;
							case 4:
								value.money -= 150;
								value.love -= 100;
								break;
							case 5:
								value.money -= 300;
								break;
							case 6:
								value.money -= 350;
								break;
							case 7:
								value.money -= 400;
								value.love -= 25;
								break;
							default:
									value.money -= 150;
							}
						}
						build[ynum][xnum].type = (BuildType)choose;
						value.buildnum[choose - 1]++;
						//adding more special car
						if (choose == FireSta) {
							int c = 0, i=0;
							while (c < 2&&i<carnum) {
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
							while (car[i].type <= 4 && car[i].type != 0 && i < carnum) {
								i++;
							}
							if (i < carnum) {
								car[i].velocity = -3;
								car[i].type = 3;
								car[i].home_x = xnum+1;
								car[i].home_y = ynum+1;
								car[i].x = car[i].y = 0;
							}
						}
						else if (choose==PoliceOff){
							int i = 0;
							while (car[i].type <= 4 && car[i].type != 0 && i < carnum) {
								i++;
							}
							if (i < carnum) {
								car[i].velocity = -3;
								car[i].type = 4;
								car[i].home_x = xnum+1;
								car[i].home_y = ynum+1;
								car[i].x = car[i].y = 0;
							}
						}
					}
					else {
						inci.addhouse = 1;
						inci.alpha[0] = 450;
						return;
					}
						
				}
				else {
					inci.addhouse = 2;
					inci.alpha[0] = 450;
					return;
				}
			}
		}

		//Choosing which building would be built
		else if (mouse.X >= (width-height/12) && mouse.Y >= (height/ 12)&&mouse.Y<=(height*10/12)) {
			choose = mouse.Y / (height / 12);
			if (choose == 10)
				choose = 0;
			if (houserequire(value, choose)&&choose) {
				inci.addhouse = houserequire(value, choose);
				inci.alpha[0] = 450;
				choose = 0;
			}
		}
	}
	
	//Right Click for releasing building
	else if (mouse.state == IN_RB_SC) {
		choose = 0;
	}

	//Fixed chosen building on mouse
	else {
		if (mouse.X >= (width -height / 12) && mouse.Y >= (height / 12.) && mouse.Y < (height * 10 / 12)) {
			boxColor(renderer, width - height / 12+ height / 1000 + 1, (int)(mouse.Y / (height / 12.)) * height / 12+ height / 1000 + 1, width, (int)(mouse.Y / (height / 12.) + 1) * height / 12- height / 1000 - 1, 0x22FFFFFF);
			if (!choose) {
				ynum= mouse.Y / (height / 12);
				if (ynum == 10)
					ynum = 0;
				if (ynum) {
					roundedBoxColor(renderer, width - height / 12 - 1.2 * (textlove.width + text400.width), height * ynum / 12, width - height / 12 - height / 200, height * (ynum + 1) / 12, height / 100, 0xAAFFFFFF);
					switch (ynum) {
					case 1:
					case 2:
					case 3:
						textRender(renderer, textmoney, Left, width - height / 12 - 1.1 * (textlove.width + text400.width), height * (ynum + 0.5) / 12, no, 255);
						textRender(renderer, text200, Left, width - height / 12 - 1.1 * (textlove.width + text400.width) + textmoney.width, height * (ynum + 0.5) / 12, no, 255);
						break;
					case 4:
						textRender(renderer, textmoney, LeftBottom, width - height / 12 - 1.1 * (textlove.width + text400.width), height * (ynum + 0.48) / 12, no, 255);
						textRender(renderer, text150, LeftBottom, width - height / 12 - 1.1 * (textlove.width + text400.width) + textmoney.width, height * (ynum + 0.48) / 12, no, 255);
						textRender(renderer, textlove, LeftTop, width - height / 12 - 1.1 * (textlove.width + text400.width), height * (ynum + 0.52) / 12, no, 255);
						textRender(renderer, text100, LeftTop, width - height / 12 - 1.1 * (textlove.width + text400.width) + textlove.width, height * (ynum + 0.52) / 12, no, 255);
						break;
					case 5:
						textRender(renderer, textmoney, Left, width - height / 12 - 1.1 * (textlove.width + text400.width), height * (ynum + 0.5) / 12, no, 255);
						textRender(renderer, text300, Left, width - height / 12 - 1.1 * (textlove.width + text400.width) + textmoney.width, height * (ynum + 0.5) / 12, no, 255);
						break;
					case 6:
						textRender(renderer, textmoney, Left, width - height / 12 - 1.1 * (textlove.width + text400.width), height * (ynum + 0.5) / 12, no, 255);
						textRender(renderer, text350, Left, width - height / 12 - 1.1 * (textlove.width + text400.width) + textmoney.width, height * (ynum + 0.5) / 12, no, 255);
						break;
					case 7:
						textRender(renderer, textmoney, LeftBottom, width - height / 12 - 1.1 * (textlove.width + text400.width), height * (ynum + 0.48) / 12, no, 255);
						textRender(renderer, text400, LeftBottom, width - height / 12 - 1.1 * (textlove.width + text400.width) + textmoney.width, height * (ynum + 0.48) / 12, no, 255);
						textRender(renderer, textlove, LeftTop, width - height / 12 - 1.1 * (textlove.width + text400.width), height * (ynum + 0.52) / 12, no, 255);
						textRender(renderer, text25, LeftTop, width - height / 12 - 1.1 * (textlove.width + text400.width) + textlove.width, height * (ynum + 0.52) / 12, no, 255);
						break;
					case 8:
					case 9:
						textRender(renderer, textmoney, Left, width - height / 12 - 1.1 * (textlove.width + text400.width), height * (ynum + 0.5) / 12, no, 255);
						textRender(renderer, text150, Left, width - height / 12 - 1.1 * (textlove.width + text400.width) + textmoney.width, height * (ynum + 0.5) / 12, no, 255);
						break;
					}
				}
			}
		}
		else if (choose){
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
		for (int i = 1; i <= 9; i++)
			if(houserequire(value, i))
				boxColor(renderer, width - height / 12 + height / 1000 + 1, i * height / 12 + height / 1000 + 1, width, (i+ 1) * height / 12, 0x22FFFFFF);
				
	}
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
void buildRender1(SDL_Renderer* renderer, const WindowData& fullViewport, Building** build, ImageData build_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1, t;

	//wnum==null, delete dynamic array
	if (wnum == NULL) {
		for (int i = 0; i < hnum; i++)
			delete[]build[i];
		delete[]build;
		return;
	}

	//free resources
	if (renderer == NULL) {
		for (int i = 0; i < 9; i++)
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
			x = (double)(width) / (wnum + 1) * (build[i][j].x + 1);
			y = (double)(height*6/8) / (hnum + 1) * (build[i][j].y + 1)+height/8;

			if (build[i][j].type >= 8) {
				t = build[i][j].type - 8;
				imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width) / (wnum + 1) * 8 / 10, (height*6/8) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), (height*6/8) / (hnum + 1) * 4 / 6, 1, NULL, NULL, 0, no, 255);
			}
			else {
				//If there is a special building
				if (build[i][j].type) {
					t = (i + j) % 2 + 2;
					imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width) / (wnum + 1) * 8 / 10, (height*6/8) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), (height*6/8) / (hnum + 1) * 4 / 6, 1, NULL, NULL, 0, no, 255);
				}
				switch (build[i][j].type) {

					//Fire Station
				case 1:
					t = 4;
					imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width) / (wnum + 1) * 8 / 10, (height*6/8) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), NULL, 1, NULL, NULL, 0, no, 255);
					break;

					//Logistics
				case 2:
					t = 5;
					imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width) / (wnum + 1) * 8 / 10, (height*6/8) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), NULL, 1, NULL, NULL, 0, no, 255);
					break;

					//Police Office
				case 3:
					t = 6;
					imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width) / (wnum + 1) * 8 / 10, (height*6/8) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), NULL, 1, NULL, NULL, 0, no, 255);
					break;

					//Factory
				case 4:
					break;

					//Shopping Mall
				case 5:
					t = 7;
					imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width) / (wnum + 1) * 8 / 10, (height*6/8) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), NULL, 1, NULL, NULL, 0, no, 255);
					break;

					//School
				case 6:
					t = 8;
					imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width) / (wnum + 1) * 8 / 10, (height*6/8) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), NULL, 1, NULL, NULL, 0, no, 255);
					break;

					//Hospital
				case 7:
					t = 9;
					imgRender(renderer, build_pic[t], build[i][j].Pos, x, y, fmin((width) / (wnum + 1) * 8 / 10, (height*6/8) / (hnum + 1) * 4 / 6 * build_pic[t].width / build_pic[t].height), NULL, 1, NULL, NULL, 0, no, 255);
					break;
				}
			}
		}
	}
}

int houserequire(ValueData& value, int type) {
	switch (type) {
	case 1:
	case 2:
	case 3:
		if (value.money < 200) {
			return 3;
		}
		else{
			return 0;
		}
		break;
	case 4:
		if (value.money < 150) {
			return 3;
		}
		else if (value.love < 100) {
			return 4;
		}
		else {
			return 0;
		}
		break;
	case 5:
		if (value.money < 300) {
			return 3;
		}
		else
			value.money -= 300;
		break;
	case 6:
		if (value.money < 350) {
			return 3;
		}
		else
			return 0;
		break;
	case 7:
		if (value.money < 400) {
			return 3;
		}
		else if (value.love < 25) {
			return 4;
		}
		else {
			return 0;
		}
		break;
	default:
		if (value.money < 150) {
			return 3;
		}
		else {
			return 0;
		}
	}

}