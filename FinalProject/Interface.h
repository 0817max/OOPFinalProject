Uint32 clock_add(Uint32 interval, void* param)
{
	ValueData* value = (ValueData*)param;
	//Return 0 when 1 day has past
	value->time = ((value->time) + 1) % (60 * 60 * 24);
	if (value->time % 7200 == 0) {
		value->season = (value->season + 1) % 4;
	}
	if (value->population != 0) {
		if (value->time % (66 + 36 * 3 * value->level * 15 / value->population) == 0) {
			value->money += 5 * value->level;
		}
		if ((value->time % (1000 + 600 * 3 * value->level * 15 / value->population) == 0) && (value->love <= 100 * value->level)) {
			value->love += 10 * value->level;
		}
	}
	if ((value->time % 3600 == 0) && (value->population < value->level * 35 * 6))
		value->population *= 2;

	return interval;
}

void controlRender(SDL_Renderer* renderer, const WindowData& fullViewport, const bool windowChange,const Mouse& mouse, char& pause,const ValueData &value, ImageData control_pic[], ImageData build_pic[], ImageData flag_pic, ImageData season_pic[]) {
	int width = fullViewport.w, height=fullViewport.h;
	static bool select = 0;
	if (control_pic==NULL) {
		select = 0;
		return;
	}
	if (mouse.state == IN_LB_SC) {
		if ((mouse.X - height / 15) * (mouse.X - height / 15) + (mouse.Y - height / 24) * (mouse.Y - height / 24) <= height / 30 * height / 30) {
			select = !select;
			if (!select){
				if (pause == 2)
					pause = 3;
			}
			
		}
	}
	if (mouse.state == IN_RB_SC) {
		if (select) {
			if (pause == 2)
				pause = 3;
			select = 0;
		}
	}
	static TextData textlose = loadTextTexture(renderer, "You lose!!", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	static TextData textmenu = loadTextTexture(renderer, "Menu", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	static TextData textexit = loadTextTexture(renderer, "Exit", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	if (renderer == NULL) {
		SDL_DestroyTexture(textlose.texture);
		SDL_DestroyTexture(textmenu.texture);
		SDL_DestroyTexture(textexit.texture);
	}
	else if (windowChange) {
		SDL_DestroyTexture(textlose.texture);
		textlose = loadTextTexture(renderer, "You lose!!", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 7, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textmenu.texture);
		textmenu = loadTextTexture(renderer, "Menu", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textexit.texture);
		textexit = loadTextTexture(renderer, "Exit", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	}

	if (select) {
		if (pause == 0)
			pause = 1;
		boxColor(renderer, 0, height / 12 + height / 1000 + 1, width-height/12- height / 1000 - 1, height, 0xBB000000);  //color aabbggrr
		roundedBoxColor(renderer, width / 3, height * 1 / 3, width * 2 / 3, height * 1 / 3 + 1.2 * textexit.height, height / 50, 0xFF888888);
		roundedBoxColor(renderer, width / 3, height * 2 / 3, width * 2 / 3, height * 2 / 3 + 1.2 * textexit.height, height / 50, 0xFF888888);
		if (renderer) {
			textRender(renderer, textmenu, Top, width / 2, height * 1 / 3 + 0.1 * textmenu.height, no, 255);
			textRender(renderer, textexit, Top, width / 2, height * 2 / 3 + 0.1 * textexit.height, no, 255);
		}
		if (fabs(mouse.Y - height * 1 / 3 - 0.6 * textexit.height) <= 0.6 * textexit.height && fabs(mouse.X - width / 2) <= width / 6)
			if (mouse.state == IN_LB_SC) {
				pause = 5;
				select = 0;
			}
			else
				roundedBoxColor(renderer, width / 3, height * 1 / 3, width * 2 / 3, height * 1 / 3 + 1.2 * textexit.height, height / 50, 0x22FFFFFF);

		if (fabs(mouse.Y - height * 2 / 3 - 0.6 * textexit.height) <= 0.6 * textexit.height && fabs(mouse.X - width / 2) <= width / 6)
			if (mouse.state == IN_LB_SC) {
				pause = 4;
				select = 0;
			}
			else
				roundedBoxColor(renderer, width / 3, height * 2 / 3, width * 2 / 3, height * 2 / 3 + 1.2 * textexit.height, height / 50, 0x22FFFFFF);
	}
	if (value.population <= 0) {
		if(renderer)
			textRender(renderer, textlose, Top, width / 2, height * 1 / 9 + 0.1 * textmenu.height, no, 255);
		select = 1;
	}


	//top
	boxColor(renderer, 0, 0, width, height / 12, 0xFF073480);  //color aabbggrr
	

	//clock
	/* filledCircleColor(renderer, height / 15, height / 24, height / 30, 0xFFFFFFFF);
	thickLineColor(renderer, height / 15, height / 24, height / 15 + height / 45 * sin(value.time / 60. / 60 / 12 * 2 * M_PI), height / 24 - height / 45 * cos(value.time / 60. / 60 / 12. * 2 * M_PI), height / 300 + 1, 0xFF008888);
	thickLineColor(renderer, height / 15, height / 24, height / 15 + height / 36 * sin(value.time / 60. / 60 * 2 * M_PI), height / 24 - height / 36 * cos(value.time / 60. / 60 * 2 * M_PI), height / 600 + 1, 0xFF008888);
	*/
	//season
	if (renderer)
		imgRender(renderer, season_pic[value.season], Middle, height / 12, height / 24, NULL, height / 13, 1, NULL, NULL, 0, no, 255);

	if ((mouse.X - height / 12) * (mouse.X - height / 12) + (mouse.Y - height / 24) * (mouse.Y - height / 24) <= height / 26 * height / 26)
		boxColor(renderer, height / 12-height/26, height / 24 - height / 26, height / 12 + height / 26, height / 24 + height / 26, 0x22FFFFFF);
	//value
	char p[10], m[10], l[10];
	sprintf_s(p, 10, "%d", value.population);
	sprintf_s(m, 10, "%d", value.money);
	sprintf_s(l, 10, "%d", value.love);

	static TextData textp = loadTextTexture(renderer, p, "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 2.25 / 12, 255, 255, 255, BLENDED);
	static TextData textm = loadTextTexture(renderer, m, "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 2.25 / 12, 255, 255, 255, BLENDED);
	static TextData textl = loadTextTexture(renderer, l, "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 2.25 / 12, 255, 255, 255, BLENDED);

	//free up resources
	if (renderer == NULL) {
		SDL_DestroyTexture(textp.texture);
		SDL_DestroyTexture(textm.texture);
		SDL_DestroyTexture(textl.texture);
		SDL_DestroyTexture(control_pic[0].texture);
		SDL_DestroyTexture(control_pic[1].texture);
		SDL_DestroyTexture(control_pic[2].texture);
		select = 0;
		return;
	}

	//Population word and background
	imgRender(renderer, control_pic[0],  RightTop, width / 6 - height / 2.25 / 12 * 0.2, height / 4 / 12 + 0.1 * height / 2.25 / 12 , NULL, height / 2.25 / 12, 1, NULL, NULL, 0, no, 255);
	if (windowChange) {
		SDL_DestroyTexture(textp.texture);
		textp = loadTextTexture(renderer, p, "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 2.25 / 12, 255, 255, 255, BLENDED);
	}
	roundedBoxColor(renderer, width / 6, height / 4 / 12, width * 1.8 / 6, height / 4 / 12 + 1.2 * textp.height, height / 50, 0xFF888888);
	textRender(renderer, textp, Right, width *1.8/ 6 - 0.2*textp.width, height / 4 / 12+0.6*textp.height, no, 255);

	//Money word and background
	imgRender(renderer, control_pic[1],  RightTop, width * 3 / 6 - height / 2.25 / 12 * 0.2, height / 4 / 12 + 0.1 * height / 2.25 / 12 , NULL, height / 2.25 / 12, 1, NULL, NULL, 0, no, 255);
	if(windowChange) {
		SDL_DestroyTexture(textm.texture);
		textm = loadTextTexture(renderer, m, "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 2.25 / 12, 255, 255, 255, BLENDED);
	}
	roundedBoxColor(renderer, width * 3 / 6, height / 4 / 12, width * 3.8 / 6, height / 4 / 12 + 1.2 * textm.height, height / 50, 0xFF888888);
	textRender(renderer, textm, Right, width * 3.8 / 6 - 0.2 * textm.width, height / 4 / 12 + 0.6 * textm.height, no, 255);
	
	//Love word and background
	imgRender(renderer, control_pic[2],  RightTop, width * 5 / 6 - height / 2.25 / 12 * 0.2, height / 4 / 12 + 0.1 * height / 2.25 / 12 , NULL, height / 2.25 / 12, 1, NULL, NULL, 0, no, 255);
	if (windowChange) {
		SDL_DestroyTexture(textl.texture);
		textl = loadTextTexture(renderer, l, "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 2.25 / 12, 255, 255, 255, BLENDED);
	}
	roundedBoxColor(renderer, width * 5 / 6, height / 4 / 12, width * 5.8 / 6, height / 4 / 12 + 1.2 * textl.height, height / 50, 0xFF888888);
	textRender(renderer, textl, Right, width * 5.8 / 6 - 0.2 * textl.width, height / 4 / 12 + 0.6 * textl.height, no, 255);
	


	//Right
	//Background
	boxColor(renderer, width-height/12, height/12, width, height, 0xFF073480);
	boxColor(renderer, width - height / 12, height  *10/12 , width, height, 0xFF022D72);
	
	//House Picture
	imgRender(renderer, build_pic[4], Middle, width-height/24,	height *1.5/12, NULL, height/14,1, NULL, NULL, 0, no, 255);
	imgRender(renderer, build_pic[5], Middle, width - height / 24, height * 2.5 / 12, NULL, height / 14, 1, NULL, NULL, 0, no, 255);
	imgRender(renderer, build_pic[6], Middle, width-height/24,	height  *3.5/ 12, NULL, height/14, 1, NULL, NULL, 0, no, 255);
	imgRender(renderer, build_pic[3], Middle, width-height/24,	height *4.5/12, height/14, NULL, 1, NULL, NULL, 0, no, 255);
	imgRender(renderer, build_pic[7], Middle, width-height/24,	height *5.5/12, NULL, height/14, 1, NULL, NULL, 0, no, 255);
	imgRender(renderer, build_pic[8], Middle, width-height/24,	height *6.5/12, NULL, height/14, 1, NULL, NULL, 0, no, 255);
	imgRender(renderer, build_pic[9], Middle, width-height/24,	height *7.5/12, NULL, height/14, 1, NULL, NULL, 0, no, 255);
	imgRender(renderer, build_pic[0], Middle, width - height / 24, height * 8.5 / 12, height / 14, NULL, 1, NULL, NULL, 0, no, 255);
	imgRender(renderer, build_pic[1], Middle, width - height / 24, height * 9.5 / 12, height / 14, NULL, 1, NULL, NULL, 0, no, 255);


	//Flag Picture
	imgRender(renderer, flag_pic, Right, width, height*11/12, NULL, height/12, 1, NULL, NULL, 0, no, 255);

	//divide line
	thickLineColor(renderer, width - height / 12, height * 10 / 12, width, height * 10 / 12, height / 1000 + 1, 0x88FFFFFF);
	thickLineColor(renderer, width - height / 12, height / 12, width - height / 12, height, height / 1000 + 1, 0xFF008888);
	thickLineColor(renderer, 0, height / 12, width, height / 12, height / 1000 + 1, 0xFF00AAAA);

	
	
}

void incident(SDL_Renderer* renderer, const WindowData& fullViewport, bool windowChange, InciData& inci, ImageData inci_pic) {

	int width = fullViewport.w, height = fullViewport.h;
	//Use alpha to control the interval of words present
	//Change the words here to choose different incident
	char incident_name[5][50] = { "There is no road!", "There is a building!", "No enough money!","There is no building!", "There is no car!" };
	char event_name[6][50]={"Fire!", "Car Accident!", "Road Closure!", "Delivery!", "Thief!", "Lightening!"};
	TextData incident_text[5];
	TextData event_text[6];
	for (int i = 0; i < 5; i++)
		incident_text[i] = loadTextTexture(renderer, incident_name[i], "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 10, 10, 10, BLENDED);
	for(int i=0; i<6; i++)
		event_text[i] = loadTextTexture(renderer, event_name[i], "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 10, 10, 10, BLENDED);
	int count = 0;
	//Incident words and background
	for (int i = 0; i < 5; i++) {
		if (inci.alpha[i]) {
			switch (i) {
			case 0:
				if (inci.addhouse!=0) {
					roundedBoxColor(renderer, width - 1.3 * incident_text[inci.addhouse-1].width - height / 12, height / 12 * (1.1+count*4/5.), width - 0.1 * incident_text[inci.addhouse - 1].width - height / 12, height / 12 * (1.1+count*4/5.) + incident_text[inci.addhouse - 1].height * (1.2+count*4/5.), height / 50, 0x88FFFFFF);
					textRender(renderer, incident_text[inci.addhouse - 1], LeftTop, width - 1.2 * incident_text[inci.addhouse - 1].width - height / 12, height / 12 * (1.1+count*4/5.) + 0.1 * incident_text[inci.addhouse - 1].height, no, (inci.alpha[i]>255)?255:inci.alpha[i]);
					imgRender(renderer, inci_pic, LeftTop, width - 1.2 * incident_text[inci.addhouse - 1].width - 1.2 * incident_text[inci.addhouse - 1].height - height / 12, height / 12 * 1.1, NULL, incident_text[inci.addhouse - 1].height * 1.2, 1, NULL, NULL, 0, no, 255);
					count++;
				}
				break;
			case 1:
				if (inci.addcar!=0) {
					roundedBoxColor(renderer, width - 1.3 * incident_text[inci.addcar+2].width - height / 12, height / 12 * (1.1 + count*4/5.), width - 0.1 * incident_text[inci.addcar + 2].width - height / 12, height / 12 * (1.1+count*4/5.) + incident_text[inci.addcar + 2].height * (1.2+count*4/5.), height / 50, 0x88FFFFFF);
					textRender(renderer, incident_text[inci.addcar + 2], LeftTop, width - 1.2 * incident_text[inci.addcar + 2].width - height / 12, height / 12 * (1.1+count*4/5.) + 0.1 * incident_text[inci.addcar + 2].height, no, (inci.alpha[i] > 255) ? 255 : inci.alpha[i]);
					imgRender(renderer, inci_pic, LeftTop, width - 1.2 * incident_text[inci.addcar + 2].width - 1.2 * incident_text[inci.addcar + 2].height - height / 12, height / 12 * (1.1+count*4/5.), NULL, incident_text[inci.addcar + 2].height * 1.2, 1, NULL, NULL, 0, no, 255);
					count++;
				}
				break;
			case 2:
				if (inci.car1!=0) {
					roundedBoxColor(renderer, width - 1.3 * event_text[inci.car1-1].width - height / 12, height / 12 * (1.1 + count*4/5.), width - 0.1 * event_text[inci.car1 - 1].width - height / 12, height / 12 * (1.1+count*4/5.) + event_text[inci.car1 - 1].height * (1.2), height / 50, 0x88FFFFFF);
					textRender(renderer, event_text[inci.car1 - 1], LeftTop, width - 1.2 * event_text[inci.car1 - 1].width - height / 12, height / 12 * (1.1 + count*4/5.) + 0.1 * event_text[inci.car1 - 1].height, no, (inci.alpha[i] > 255) ? 255 : inci.alpha[i]);
					imgRender(renderer, inci_pic, LeftTop, width - 1.2 * event_text[inci.car1 - 1].width - 1.2 * event_text[inci.car1 - 1].height - height / 12, height / 12 * (1.1 + count*4/5.), NULL, event_text[inci.car1 - 1].height * 1.2, 1, NULL, NULL, 0, no, 255);
					count++;
				}
				break;
			case 3:
				if (inci.car2!=0) {
					roundedBoxColor(renderer, width - 1.3 * event_text[inci.car2-1].width - height / 12, height / 12 * (1.1 + count*4/5.), width - 0.1 * event_text[inci.car2-1].width - height / 12, height / 12 * (1.1+count*4/5.) + event_text[inci.car2-1].height * (1.2), height / 50, 0x88FFFFFF);
					textRender(renderer, event_text[inci.car2-1], LeftTop, width - 1.2 * event_text[inci.car2-1].width - height / 12, height / 12 * (1.1 + count*4/5.) + 0.1 * event_text[inci.car2-1].height, no, (inci.alpha[i] > 255) ? 255 : inci.alpha[i]);
					imgRender(renderer, inci_pic, LeftTop, width - 1.2 * event_text[inci.car2-1].width - 1.2 * event_text[inci.car2-1].height - height / 12, height / 12 * (1.1 + count*4/5.), NULL, event_text[inci.car2-1].height * 1.2, 1, NULL, NULL, 0, no, 255);
					count++;
				}
				break;
			case 4:
				if (inci.car3!=0) {
					roundedBoxColor(renderer, width - 1.3 * event_text[inci.car3-1].width - height / 12, height / 12 * (1.1 + count*4/5.), width - 0.1 * event_text[inci.car3-1].width - height / 12, height / 12 * (1.1+count*4/5.) + event_text[inci.car3-1].height * (1.2), height / 50, 0x88FFFFFF);
					textRender(renderer, event_text[inci.car3-1], LeftTop, width - 1.2 * event_text[inci.car3-1].width - height / 12, height / 12 * (1.1 + count*4/5.) + 0.1 * event_text[inci.car3-1].height, no, (inci.alpha[i] > 255) ? 255 : inci.alpha[i]);
					imgRender(renderer, inci_pic, LeftTop, width - 1.2 * event_text[inci.car3-1].width - 1.2 * event_text[inci.car3-1].height - height / 12, height / 12 * (1.1 + count*4/5.), NULL, event_text[inci.car3-1].height * 1.2, 1, NULL, NULL, 0, no, 255);
					count++;
				}
				break;
			}
		}
	}
	
	for (int i = 0; i < 5; i++)
		SDL_DestroyTexture(incident_text[i].texture);
	for (int i = 0; i < 6; i++)
		SDL_DestroyTexture(event_text[i].texture);
	

}

Uint32 incident_add(Uint32 interval, void* param)
{
	InciData* inci = (InciData*)param;
	//Use alpha to control to the time of presentation
	for (int i = 0; i < 5; i++) {
		if ( inci->alpha[i]> 150)
			inci->alpha[i] = inci->alpha[i] - ((450 - (inci->alpha[i])) / 15 + 1);
		else {
			inci->alpha[i] = 0;
			switch (i) {
			case 0:
				inci->addhouse = 0;
				break;
			case 1:
				inci->addcar = 0;
				break;
			case 2:
				inci->car1 = 0;
				break;
			case 3:
				inci->car2 = 0;
				break;
			case 4:
				inci->car3 = 0;
				break;
			}
		}
	}
	return interval;
}

bool menuRender(SDL_Renderer* renderer, const WindowData& fullViewport, const bool windowChange, const Mouse& mouse, ValueData& value) {
	int width = fullViewport.w, height = fullViewport.h;
	static TextData texttitle = loadTextTexture(renderer, "City Housekeeper", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 8, 255, 255, 255, BLENDED);
	static TextData textlevel1 = loadTextTexture(renderer, "Level1", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 10, 255, 255, 255, BLENDED);
	static TextData textlevel2 = loadTextTexture(renderer, "Level2", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 10, 255, 255, 255, BLENDED);
	static TextData textlevel3 = loadTextTexture(renderer, "Level3", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 10, 255, 255, 255, BLENDED);
	static TextData textinst = loadTextTexture(renderer, "Instruction", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 10, 255, 255, 255, BLENDED);
	static TextData textexit = loadTextTexture(renderer, "Exit", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 10, 255, 255, 255, BLENDED);
	if (renderer == NULL) {
		SDL_DestroyTexture(texttitle.texture);
		SDL_DestroyTexture(textlevel1.texture);
		SDL_DestroyTexture(textlevel2.texture);
		SDL_DestroyTexture(textlevel3.texture);
		SDL_DestroyTexture(textinst.texture);
		SDL_DestroyTexture(textexit.texture);
	}
	else if (windowChange) {
		SDL_DestroyTexture(texttitle.texture);
		texttitle = loadTextTexture(renderer, "City Housekeeper", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 8, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textlevel1.texture);
		textlevel1 = loadTextTexture(renderer, "Level1", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 12, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textlevel2.texture);
		textlevel2 = loadTextTexture(renderer, "Level2", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 12, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textlevel3.texture);
		textlevel3 = loadTextTexture(renderer, "Level3", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 12, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textinst.texture);
		textinst = loadTextTexture(renderer, "Instruction", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 12, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textexit.texture);
		textexit = loadTextTexture(renderer, "Exit", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 12, 255, 255, 255, BLENDED);
	}

	roundedBoxColor(renderer, width / 3, height * 3 / 8, width * 2 / 3, height * 3 / 8 + 1.2 * textexit.height, height / 50, 0xFF888888);
	roundedBoxColor(renderer, width / 3, height * 4 / 8, width * 2 / 3, height * 4 / 8 + 1.2 * textexit.height, height / 50, 0xFF888888);
	roundedBoxColor(renderer, width / 3, height * 5 / 8, width * 2 / 3, height * 5 / 8 + 1.2 * textexit.height, height / 50, 0xFF888888);
	roundedBoxColor(renderer, width / 3, height * 6 / 8, width * 2 / 3, height * 6 / 8 + 1.2 * textexit.height, height / 50, 0xFF888888);
	roundedBoxColor(renderer, width / 3, height * 7 / 8, width * 2 / 3, height * 7 / 8 + 1.2 * textexit.height, height / 50, 0xFF888888);
	if (renderer) {
		textRender(renderer, texttitle, Top, width / 2, height * 1 / 8 + 0.1 * textlevel1.height, no, 255);
		textRender(renderer, textlevel1, Top, width / 2, height * 3 / 8 + 0.1 * textlevel1.height, no, 255);
		textRender(renderer, textlevel2, Top, width / 2, height * 4/ 8 + 0.1 * textlevel2.height, no, 255);
		textRender(renderer, textlevel3, Top, width / 2, height * 5 / 8 + 0.1 * textlevel3.height, no, 255);
		textRender(renderer, textinst, Top, width / 2, height * 6 / 8 + 0.1 * textlevel1.height, no, 255);
		textRender(renderer, textexit, Top, width / 2, height * 7 / 8 + 0.1 * textexit.height, no, 255);
	}
	if (fabs(mouse.Y - height * 3 / 8 - 0.6 * textlevel1.height) <= 0.6 * textlevel1.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			value.level = 1;
		else
			roundedBoxColor(renderer, width / 3, height * 3 / 8, width * 2 / 3, height * 3 / 8 + 1.2 * textexit.height, height / 50, 0x22FFFFFF);
	if (fabs(mouse.Y - height * 4 / 8 - 0.6 * textlevel2.height) <= 0.6 * textlevel2.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			value.level = 2;
		else
			roundedBoxColor(renderer, width / 3, height * 4/ 8, width * 2 / 3, height * 4 / 8+ 1.2 * textexit.height, height / 50, 0x22FFFFFF);
	if (fabs(mouse.Y - height * 5 / 8 - 0.6 * textlevel3.height) <= 0.6 * textlevel3.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			value.level = 3;
		else
			roundedBoxColor(renderer, width / 3, height * 5 / 8, width * 2 / 3, height * 5 / 8 + 1.2 * textexit.height, height / 50, 0x22FFFFFF);
	if (fabs(mouse.Y - height * 7 / 8 - 0.6 * textexit.height) <= 0.6 * textexit.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			return true;
		else
			roundedBoxColor(renderer, width / 3, height * 7 / 8, width * 2 / 3, height * 7 / 8 + 1.2 * textexit.height, height / 50, 0x22FFFFFF);
	return false;
}

