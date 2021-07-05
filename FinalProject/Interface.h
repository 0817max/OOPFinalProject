#include<string.h>
Uint32 clock_add(Uint32 interval, void* param)
{
	ValueData* value = (ValueData*)param;
	//Return 0 when 1 day has past
	value->time = ((value->time) + 1) % (60 * 60 * 24);
	if (value->time % 7200 == 0) {
		value->season = (value->season + 1) % 4;
	}
	if (value->population != 0) {
		if (value->time % (100 + 500 * value->level / value->population) == 0) {
			value->money += 5 * value->level;
		}

		if ((value->time % (1000 + 10000 * value->level / value->population) == 0) && (value->love <= 500 * value->level)) {
			value->love += 10 * value->level;
		}
	}
	if ((value->time % 3600 == 0) && (value->population <= value->level * 100 + 50 * value->level * value->level)) {
		if (value->population < 150)
			value->population *= 2;
		else if (value->population <= 300)
			value->population += 300;
		else
			value->population += 50;
	}
	if(value->time % 100==0){
		value->money +=value->buildnum[Factory-1]*value->level*5;
		value->love -= value->buildnum[Factory - 1];
	}
	if(value->time % 500==0)
		value->population += 5* value->buildnum[Hospital - 1] * value->level;

	if(value->time%1000==0)
		value->love += 2* value->buildnum[School - 1] * value->level;
	if(value->time % 200==0)
		value->money += value->buildnum[Shopping - 1] * value->level;
	return interval;
}

void controlRender(SDL_Renderer* renderer, const WindowData& fullViewport, const bool windowChange,const Mouse& mouse, char& pause, ValueData &value, ImageData control_pic[], ImageData build_pic[], ImageData flag_pic, ImageData season_pic[]) {
	int width = fullViewport.w, height=fullViewport.h;
	static int select = 0;
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
	
	static double speed=1;
	char sp[10];

	static TextData textlose = loadTextTexture(renderer, "You lose!!", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 7, 255, 255, 255, BLENDED);
	static TextData textwin = loadTextTexture(renderer, "You win!!", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 7, 255, 255, 255, BLENDED);
	static TextData textnext = loadTextTexture(renderer, "Next Level", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	static TextData textcont = loadTextTexture(renderer, "Continue", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	static TextData textmenu = loadTextTexture(renderer, "Menu", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	static TextData textexit = loadTextTexture(renderer, "Exit", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	static TextData textspeed = loadTextTexture(renderer, "1x", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 18, 255, 255, 255, BLENDED);
	
	if (renderer == NULL) {
		SDL_DestroyTexture(textlose.texture);
		SDL_DestroyTexture(textwin.texture);
		SDL_DestroyTexture(textnext.texture);
		SDL_DestroyTexture(textcont.texture);
		SDL_DestroyTexture(textmenu.texture);
		SDL_DestroyTexture(textexit.texture);
		SDL_DestroyTexture(textspeed.texture);
		return;
	}
	else if (windowChange) {
		SDL_DestroyTexture(textlose.texture);
		textlose = loadTextTexture(renderer, "You lose!!", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 7, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textwin.texture);
		textwin = loadTextTexture(renderer, "You win!!", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 7, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textnext.texture);
		textnext = loadTextTexture(renderer, "Next Level", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textcont.texture);
		textcont = loadTextTexture(renderer, "Continue", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textmenu.texture);
		textmenu = loadTextTexture(renderer, "Menu", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textexit.texture);
		textexit = loadTextTexture(renderer, "Exit", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	}

	if (windowChange || fabs(speed-value.speed)>1e-3) {
		SDL_DestroyTexture(textspeed.texture);
		sprintf_s(sp, 10, "%lgx", value.speed);
		speed = value.speed;
		textspeed = loadTextTexture(renderer, sp, "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 18, 255, 255, 255, BLENDED);
	}

	//lose condition
	if (value.population <= 0)
		select = 2;

	//win condition
	switch (value.level) {
	case 1:
		if (value.population >= 100 && value.money >= 1000 && value.love >= 1000)
			select = 3;
		break;
	case 2:
		if (value.population >= 300 && value.money >= 2000 && value.love >= 2000)
			select = 3;
		break;
	case 3:
		if (value.population >= 500 && value.money >= 4000 && value.love >= 3000)
			select = 3;
		break;
	}

	//for menu
	if (select&&renderer) {
		if (pause == 0)
			pause = 1;
		boxColor(renderer, 0, height / 12 + height / 1000 + 1, width - height / 12 - height / 1000 - 1, height, 0xBB000000);  //color aabbggrr
		//roundedBoxColor(renderer, width / 3, height * 2 / 5, width * 2 / 3, height*2/5+height/25, height / 50, 0xFF888888);

		//for lose
		if (select == 2) {
			textRender(renderer, textlose, Top, width / 2, height * 1 / 5 + 0.1 * textlose.height, no, 255);
		}
		//for win
		else if (select == 3) {
			textRender(renderer, textwin, Top, width / 2, height * 1 / 5 + 0.1 * textwin.height, no, 255);
			if (value.level != 3) {
				roundedBoxColor(renderer, width / 3, height * 2 / 5, width * 2 / 3, height * 2 / 5 + 1.2 * textnext.height, height / 50, 0xFF888888);
				textRender(renderer, textnext, Top, width / 2, height * 2 / 5 + 0.1 * textnext.height, no, 255);
				if (fabs(mouse.Y - height * 2 / 5 - 0.6 * textnext.height) <= 0.6 * textnext.height && fabs(mouse.X - width / 2) <= width / 6)
					if (mouse.state == IN_LB_SC) {
						pause = 6;
						select = 0;
					}
					else
						roundedBoxColor(renderer, width / 3, height * 2 / 5, width * 2 / 3, height * 2 / 5 + 1.2 * textnext.height, height / 50, 0x22FFFFFF);
			}
		}
		//for normal select
		else{
			roundedBoxColor(renderer, width / 5- height / 100, height * 1 / 5+height/ 50., width * 4 / 5+ height / 100, height * 1 / 5 + height*2/50., height / 100, 0xFF888888);
			int speedx;
			if (value.speed >= 1)
				speedx = width / 2 + width*3 / 10 * (value.speed - 1) / 9;
			else
				speedx = width / 2 - width*3 / 10 * (1 - value.speed)/0.9;
			filledCircleColor(renderer, speedx, height * 1 / 5 + height * 1.5 / 50., height / 35, 0xFF00AAAA);
			if((mouse.Y - height * 1 / 5 - height * 1.5 / 50.)*(mouse.Y - height * 1 / 5 - height * 1.5 / 50.)<height / 20* height / 20)
				if (mouse.state==IN_LB_PR_HOVER||mouse.state==IN_LB_SC) {
					speedx = mouse.X;
					if (speedx >= width * 4 / 5)
						speedx = width * 4 / 5;
					else if (speedx <= width / 5)
						speedx = width / 5;
					if (speedx >= width / 2)
						value.speed = 1 + (speedx - width / 2) / (width *3/ 10/36)*0.25;
					else
						value.speed = 1 - (width / 2-speedx) / (width *3/ 10/36) * 0.1/4;
					if (value.speed >= 10)
						value.speed = 10;
					else if (value.speed <= 0.1)
						value.speed = 0.1;
				}
				else
					filledCircleColor(renderer, speedx, height * 1 / 5 + height * 1.5 / 50., height / 35, 0x22FFFFFF);
			textRender(renderer, textspeed, Bottom, speedx, height * 1 / 5, no, 255);
			roundedBoxColor(renderer, width / 3, height * 2 / 5, width * 2 / 3, height * 2 / 5 + 1.2 * textcont.height, height / 50, 0xFF888888);
			textRender(renderer, textcont, Top, width / 2, height * 2 / 5 + 0.1 * textcont.height, no, 255);
			if (fabs(mouse.Y - height * 2 / 5 - 0.6 * textcont.height) <= 0.6 * textcont.height && fabs(mouse.X - width / 2) <= width / 6)
				if (mouse.state == IN_LB_SC) {
					pause = 3;
					select = 0;
				}
				else
					roundedBoxColor(renderer, width / 3, height * 2 / 5, width * 2 / 3, height * 2 / 5 + 1.2 * textcont.height, height / 50, 0x22FFFFFF);

		}

		roundedBoxColor(renderer, width / 3, height * 3 / 5, width * 2 / 3, height * 3 / 5 + 1.2 * textmenu.height, height / 50, 0xFF888888);
		roundedBoxColor(renderer, width / 3, height * 4 / 5, width * 2 / 3, height * 4 / 5 + 1.2 * textexit.height, height / 50, 0xFF888888);
		
		textRender(renderer, textmenu, Top, width / 2, height * 3 / 5 + 0.1 * textmenu.height, no, 255);
		textRender(renderer, textexit, Top, width / 2, height * 4 / 5 + 0.1 * textexit.height, no, 255);
		
		if (fabs(mouse.Y - height * 3 / 5 - 0.6 * textmenu.height) <= 0.6 * textmenu.height && fabs(mouse.X - width / 2) <= width / 6)
			if (mouse.state == IN_LB_SC) {
				pause = 5;
				select = 0;
			}
			else
				roundedBoxColor(renderer, width / 3, height * 3 / 5, width * 2 / 3, height * 3 / 5 + 1.2 * textmenu.height, height / 50, 0x22FFFFFF);

		if (fabs(mouse.Y - height * 4 / 5 - 0.6 * textexit.height) <= 0.6 * textexit.height && fabs(mouse.X - width / 2) <= width / 6)
			if (mouse.state == IN_LB_SC) {
				pause = 4;
				select = 0;
			}
			else
				roundedBoxColor(renderer, width / 3, height * 4 / 5, width * 2 / 3, height * 4 / 5 + 1.2 * textexit.height, height / 50, 0x22FFFFFF);
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
	static ValueData oldvalue = {0, 0, 0, 0, 0, 0};
	
	//free up resources
	if (renderer == NULL) {
		SDL_DestroyTexture(textp.texture);
		SDL_DestroyTexture(textm.texture);
		SDL_DestroyTexture(textl.texture);
		select = 0;
		return;
	}

	//Population word and background
	imgRender(renderer, control_pic[0],  RightTop, width / 6 - height / 2.25 / 12 * 0.2, height / 4 / 12 + 0.1 * height / 2.25 / 12 , NULL, height / 2.25 / 12, 1, NULL, NULL, 0, no, 255);
	if (windowChange||value.population!=oldvalue.population) {
		oldvalue.population = value.population;
		SDL_DestroyTexture(textp.texture);
		textp = loadTextTexture(renderer, p, "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 2.25 / 12, 255, 255, 255, BLENDED);
	}
	roundedBoxColor(renderer, width / 6, height / 4 / 12, width * 1.8 / 6, height / 4 / 12 + 1.2 * textp.height, height / 100, 0xFF888888);
	textRender(renderer, textp, Right, width *1.8/ 6 - 0.2*textp.width, height / 4 / 12+0.6*textp.height, no, 255);

	//Money word and background
	imgRender(renderer, control_pic[1],  RightTop, width * 3 / 6 - height / 2.25 / 12 * 0.2, height / 4 / 12 + 0.1 * height / 2.25 / 12 , NULL, height / 2.25 / 12, 1, NULL, NULL, 0, no, 255);
	if(windowChange|| value.money != oldvalue.money) {
		oldvalue.money = value.money;
		SDL_DestroyTexture(textm.texture);
		textm = loadTextTexture(renderer, m, "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 2.25 / 12, 255, 255, 255, BLENDED);
	}
	roundedBoxColor(renderer, width * 3 / 6, height / 4 / 12, width * 3.8 / 6, height / 4 / 12 + 1.2 * textm.height, height / 100, 0xFF888888);
	textRender(renderer, textm, Right, width * 3.8 / 6 - 0.2 * textm.width, height / 4 / 12 + 0.6 * textm.height, no, 255);
	
	//Love word and background
	imgRender(renderer, control_pic[2],  RightTop, width * 5 / 6 - height / 2.25 / 12 * 0.2, height / 4 / 12 + 0.1 * height / 2.25 / 12 , NULL, height / 2.25 / 12, 1, NULL, NULL, 0, no, 255);
	if (windowChange||value.love!=oldvalue.love) {
		oldvalue.love = value.love;
		SDL_DestroyTexture(textl.texture);
		textl = loadTextTexture(renderer, l, "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 2.25 / 12, 255, 255, 255, BLENDED);
	}
	roundedBoxColor(renderer, width * 5 / 6, height / 4 / 12, width * 5.8 / 6, height / 4 / 12 + 1.2 * textl.height, height / 100, 0xFF888888);
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
	char incident_name[7][50] = { "There is no road!", "There is a building!", "No enough money!", "No enough Happiness!","There is no building!", "There is no car!",  "There is no way!" };
	char event_name[6][50]={"Fire!", "Car Accident!", "Road Closure!", "Delivery!", "Thief!", "Lightening!"};
	static TextData incident_text[6] = {NULL, 0, 0};
	static TextData event_text[6]= { NULL, 0, 0 };
	if (renderer == NULL) {
		for (int i = 0; i < 7; i++)
			SDL_DestroyTexture(incident_text[i].texture);
		for (int i = 0; i < 6; i++)
			SDL_DestroyTexture(event_text[i].texture);
		return;
	}
	if (incident_text[0].texture == NULL) {
		for (int i = 0; i < 7; i++)
			incident_text[i] = loadTextTexture(renderer, incident_name[i], "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 10, 10, 10, BLENDED);
		for (int i = 0; i < 6; i++)
			event_text[i] = loadTextTexture(renderer, event_name[i], "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 10, 10, 10, BLENDED);
	}
	else if (windowChange) {
		for (int i = 0; i < 7; i++)
			SDL_DestroyTexture(incident_text[i].texture);
		for (int i = 0; i < 6; i++)
			SDL_DestroyTexture(event_text[i].texture);
		for (int i = 0; i < 7; i++)
			incident_text[i] = loadTextTexture(renderer, incident_name[i], "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 10, 10, 10, BLENDED);
		for (int i = 0; i < 6; i++)
			event_text[i] = loadTextTexture(renderer, event_name[i], "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 10, 10, 10, BLENDED);
	}
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
					roundedBoxColor(renderer, width - 1.3 * incident_text[inci.addcar+3].width - height / 12, height / 12 * (1.1 + count*4/5.), width - 0.1 * incident_text[inci.addcar+3].width - height / 12, height / 12 * (1.1+count*4/5.) + incident_text[inci.addcar+3].height * (1.2+count*4/5.), height / 50, 0x88FFFFFF);
					textRender(renderer, incident_text[inci.addcar+3], LeftTop, width - 1.2 * incident_text[inci.addcar+3].width - height / 12, height / 12 * (1.1+count*4/5.) + 0.1 * incident_text[inci.addcar+3].height, no, (inci.alpha[i] > 255) ? 255 : inci.alpha[i]);
					imgRender(renderer, inci_pic, LeftTop, width - 1.2 * incident_text[inci.addcar+3].width - 1.2 * incident_text[inci.addcar+3].height - height / 12, height / 12 * (1.1+count*4/5.), NULL, incident_text[inci.addcar+3].height * 1.2, 1, NULL, NULL, 0, no, 255);
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
	
	

}

Uint32 incident_add(Uint32 interval, void* param)
{
	InciData* inci = (InciData*)param;
	//Use alpha to control to the time of presentation
	for (int i = 0; i < 5; i++) {
		if ( inci->alpha[i]> 150)
			inci->alpha[i] = inci->alpha[i] - ((450 - (inci->alpha[i])) / 25 + 1);
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
		return false;
	}
	else if (windowChange) {
		SDL_DestroyTexture(texttitle.texture);
		texttitle = loadTextTexture(renderer, "City Housekeeper", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 8, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textlevel1.texture);
		textlevel1 = loadTextTexture(renderer, "Level 1", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 12, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textlevel2.texture);
		textlevel2 = loadTextTexture(renderer, "Level 2", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 12, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textlevel3.texture);
		textlevel3 = loadTextTexture(renderer, "Level 3", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 12, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textinst.texture);
		textinst = loadTextTexture(renderer, "Instruction", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 12, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textexit.texture);
		textexit = loadTextTexture(renderer, "Exit", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 12, 255, 255, 255, BLENDED);
	}
	
	boxColor(renderer, 0, 0, width, height, 0x300A0A0A);


	roundedBoxColor(renderer, width / 3, height * 2 / 8, width * 2 / 3, height * 2 / 8 + 1.2 * textexit.height, height / 50, 0xC0606060);
	roundedBoxColor(renderer, width / 3, height * 3 / 8, width * 2 / 3, height * 3 / 8 + 1.2 * textexit.height, height / 50, 0xC0606060);
	roundedBoxColor(renderer, width / 3, height * 4 / 8, width * 2 / 3, height * 4 / 8 + 1.2 * textexit.height, height / 50, 0xC0606060);
	roundedBoxColor(renderer, width / 3, height * 5 / 8, width * 2 / 3, height * 5 / 8 + 1.2 * textexit.height, height / 50, 0xC0606060);
	roundedBoxColor(renderer, width / 3, height * 6 / 8, width * 2 / 3, height * 6 / 8 + 1.2 * textexit.height, height / 50, 0xC0606060);
	
	textRender(renderer, texttitle, Top, width / 2, 0, no, 255);
	textRender(renderer, textlevel1, Top, width / 2, height * 2 / 8 + 0.1 * textlevel1.height, no, 255);
	textRender(renderer, textlevel2, Top, width / 2, height * 3/ 8 + 0.1 * textlevel2.height, no, 255);
	textRender(renderer, textlevel3, Top, width / 2, height * 4 / 8 + 0.1 * textlevel3.height, no, 255);
	textRender(renderer, textinst, Top, width / 2, height * 5 / 8 + 0.1 * textlevel1.height, no, 255);
	textRender(renderer, textexit, Top, width / 2, height * 6 / 8 + 0.1 * textexit.height, no, 255);
	
	if (fabs(mouse.Y - height * 2 / 8 - 0.6 * textlevel1.height) <= 0.6 * textlevel1.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			value.level = 1;
		else
			roundedBoxColor(renderer, width / 3, height * 2 / 8, width * 2 / 3, height * 2 / 8 + 1.2 * textexit.height, height / 50, 0x44FFFFFF);
	if (fabs(mouse.Y - height * 3 / 8 - 0.6 * textlevel2.height) <= 0.6 * textlevel2.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			value.level = 2;
		else
			roundedBoxColor(renderer, width / 3, height * 3/ 8, width * 2 / 3, height * 3 / 8+ 1.2 * textexit.height, height / 50, 0x44FFFFFF);
	if (fabs(mouse.Y - height * 4 / 8 - 0.6 * textlevel3.height) <= 0.6 * textlevel3.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			value.level = 3;
		else
			roundedBoxColor(renderer, width / 3, height * 4 / 8, width * 2 / 3, height * 4 / 8 + 1.2 * textexit.height, height / 50, 0x44FFFFFF);
	if (fabs(mouse.Y - height * 5 / 8 - 0.6 * textlevel3.height) <= 0.6 * textlevel3.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			value.level=-1;
		else
			roundedBoxColor(renderer, width / 3, height * 5 / 8, width * 2 / 3, height * 5 / 8 + 1.2 * textexit.height, height / 50, 0x44FFFFFF);

	if (fabs(mouse.Y - height * 6 / 8 - 0.6 * textexit.height) <= 0.6 * textexit.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			return true;
		else
			roundedBoxColor(renderer, width / 3, height * 6 / 8, width * 2 / 3, height * 6 / 8 + 1.2 * textexit.height, height / 50, 0x44FFFFFF);
	return false;
}

void instRender(SDL_Renderer* renderer, const WindowData& fullViewport, const bool windowChange, const Mouse& mouse, int& level) {
	int width = fullViewport.w, height = fullViewport.h, i;
	FILE* fp;
	static TextData textmenu = loadTextTexture(renderer, "Menu", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 20, 255, 255, 255, BLENDED);
	static TextData textplay = loadTextTexture(renderer, "Play", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 20, 255, 255, 255, BLENDED);
	static char words[120][150] = { "0" };
	static TextData inst_text[18] = { NULL, 0, 0 };
	static int length = 0, place=0;
	if (renderer == NULL&&strcmp(words[0], "0")) {
		for (int i = 0; i < 18; i++)
			if (inst_text[i].texture) {
				SDL_DestroyTexture(inst_text[i].texture);
				inst_text[i].texture = NULL;
			}
		SDL_DestroyTexture(textmenu.texture);
		SDL_DestroyTexture(textplay.texture);
	}
	if (inst_text[0].texture==NULL) {
		length = 0;
		place = 0;
		fp = fopen("./Background.txt", "r");
		while (length < 120&&!feof(fp)) {
			fgets(words[length], (62* width / height>120)?120: 62 * width / height, fp);
			int j = 0;
			while (words[length][j] != '\n'&&j<120) {
				j++;
			}
			if (words[length][j] == '\n') {
				if (j == 0) {
					strcpy(words[length], " ");
				}
				else
					words[length][j] = ' ';
			}
			else
				words[length][119] = '\0';
			length++;
		}
		fclose(fp);
		i = 0;
		while (i < 18 &&i < length) {
			if(words[i+place][0]>='A'&& words[i+place][0] <='Z')
				inst_text[i] = loadTextTexture(renderer, words[i], "../fonts/times.ttf", height / 21, 255, 255, 255, BLENDED);
			else if(words[i + place][2] >= 'A' && words[i + place][2] <= 'Z')
				inst_text[i] = loadTextTexture(renderer, words[i], "../fonts/times.ttf", height / 23, 255, 255, 255, BLENDED);
			else if (words[i + place][4] >= 'A' && words[i + place][4] <= 'Z')
				inst_text[i] = loadTextTexture(renderer, words[i], "../fonts/times.ttf", height / 25, 255, 255, 255, BLENDED);
			else
				inst_text[i] = loadTextTexture(renderer, words[i], "../fonts/times.ttf", height / 27, 255, 255, 255, BLENDED);
			i++;
		}
	}
	else if (windowChange) {
		length = 0;
		fp = fopen("./Background.txt", "r");
		while (length < 120 && !feof(fp)) {
			fgets(words[length], (62 * width / height > 120) ? 120 : 62 * width / height, fp);
			int j = 0;
			while (words[length][j] != '\n' && j < 120) {
				j++;
			}
			if (words[length][j] == '\n') {
				if (j == 0) {
					strcpy(words[length], " ");
				}
				else
					words[length][j] = ' ';
			}
			else
				words[length][119] = '\0';
			length++;
		}
		fclose(fp);
		i = 0;
		while (i + place < length&&i<18) {
			SDL_DestroyTexture(inst_text[i].texture);
			if (words[i + place][0] >= 'A' && words[i + place][0] <= 'Z')
				inst_text[i] = loadTextTexture(renderer, words[i], "../fonts/times.ttf", height / 21, 255, 255, 255, BLENDED);
			else if (words[i + place][2] >= 'A' && words[i + place][2] <= 'Z')
				inst_text[i] = loadTextTexture(renderer, words[i], "../fonts/times.ttf", height / 23, 255, 255, 255, BLENDED);
			else if (words[i + place][4] >= 'A' && words[i + place][4] <= 'Z')
				inst_text[i] = loadTextTexture(renderer, words[i], "../fonts/times.ttf", height / 25, 255, 255, 255, BLENDED);
			else
				inst_text[i] = loadTextTexture(renderer, words[i], "../fonts/times.ttf", height / 27, 255, 255, 255, BLENDED);
			i++;
		}
	}

	for(int i=0; i<18; i++)
		textRender(renderer, inst_text[i], LeftTop, textmenu.width * 0.4, height / 20 + i * height / 20, no, 255);

	if ((mouse.state == IN_WD || mouse.state == IN_WU)&&fabs(mouse.Y)<100) {
		i = 0;
		if (fabs(mouse.Y) > 20)
			place -= 20 * mouse.Y / (int)fabs(mouse.Y);
		else
			place -= mouse.Y;
		if (mouse.state == IN_WD) {
			if (18 + place >= length || place < 0)
				place = length - 18;
		}
		else if (mouse.state == IN_WU) {
			if (18 + place >= length || place < 0)
				place = 0;
		}
		while (i < 18 && i +place < length) {
			SDL_DestroyTexture(inst_text[i].texture);
			if (words[i + place][0] >= 'A' && words[i + place][0] <= 'Z')
				inst_text[i] = loadTextTexture(renderer, words[i+place], "../fonts/times.ttf", height / 21, 255, 255, 255, BLENDED);
			else if (words[i + place][2] >= 'A' && words[i + place][2] <= 'Z')
				inst_text[i] = loadTextTexture(renderer, words[i+place], "../fonts/times.ttf", height / 23, 255, 255, 255, BLENDED);
			else if (words[i + place][4] >= 'A' && words[i + place][4] <= 'Z')
				inst_text[i] = loadTextTexture(renderer, words[i+place], "../fonts/times.ttf", height / 25, 255, 255, 255, BLENDED);
			else
				inst_text[i] = loadTextTexture(renderer, words[i+place], "../fonts/times.ttf", height / 27, 255, 255, 255, BLENDED);
			i++;
		}
	}

	if (windowChange) {
		SDL_DestroyTexture(textmenu.texture);
		textmenu = loadTextTexture(renderer, "Menu", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 20, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textplay.texture);
		textplay = loadTextTexture(renderer, "Play", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 20, 255, 255, 255, BLENDED);
	}

	thickLineColor(renderer, height / 100, height / 2, 0.2 * textmenu.width, height / 2 - 0.2 * textmenu.width, height/300+1, 0xAAFFFFFF);
	thickLineColor(renderer, height / 100, height / 2, 0.2 * textmenu.width, height / 2 + 0.2 * textmenu.width, height / 300 + 1,0xAAFFFFFF);
	thickLineColor(renderer, width- height / 100, height / 2, width-0.2 * textmenu.width, height / 2 - 0.2 * textmenu.width, height / 300 + 1,0xAAFFFFFF);
	thickLineColor(renderer, width- height / 100, height / 2, width-0.2 * textmenu.width, height / 2 + 0.2 * textmenu.width, height / 300 + 1, 0xAAFFFFFF);

	if (fabs(mouse.Y - height / 2) <= 0.6 * textmenu.height && fabs(mouse.X - 0.7 * textmenu.width) <= 0.7 * textmenu.width)
		if (mouse.state == IN_LB_SC)
			level = 0;
		else {
			textRender(renderer, textmenu, Middle, 0.8 * textmenu.width, height / 2, no, 255);
		}
	if (fabs(mouse.Y - height / 2) <= 0.6 * textplay.height && fabs(mouse.X + 0.7 * textmenu.width - width) <= 0.7 * textplay.width)
		if (mouse.state == IN_LB_SC)
			level = 1;
		else {
			textRender(renderer, textplay, Middle, width - 0.8 * textplay.width, height / 2, no, 255);
		}
	if(level!=-1)
		for(int i=0; i<18; i++)
			if (inst_text[i].texture) {
				SDL_DestroyTexture(inst_text[i].texture);
				inst_text[i].texture = NULL;
			}
}

void musicRender(SDL_Renderer* renderer, const WindowData& fullViewport, bool windowChange, ValueData& value,const Mouse& mouse, Mix_Music*& music,char tone_Path[3][50]){
	int width = fullViewport.w, height = fullViewport.h;
	static TextData textmute = loadTextTexture(renderer, "Mute", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 20, 255, 255, 255, BLENDED);
	static TextData textplay = loadTextTexture(renderer, "Play", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 7, 255, 255, 255, BLENDED);
	static int level=0;
	if (renderer == NULL) {
		SDL_DestroyTexture(textmute.texture);
		SDL_DestroyTexture(textplay.texture);
		return;
	}
	else if (level != value.level) {
		level = value.level;
		if (Mix_PlayingMusic()) {
			while (!Mix_FadeOutMusic(250) && Mix_PlayingMusic()) {
				// wait for any fades to complete
				SDL_Delay(10);
			}
			Mix_FreeMusic(music);
			music = NULL;
			switch (value.level) {
			case -1:
				music = Mix_LoadMUS(tone_Path[1]);
				break;
			case 0:
				music = Mix_LoadMUS(tone_Path[0]);
				break;
			case 1:
			case 2:
			case 3:
				music = Mix_LoadMUS(tone_Path[2]);
				break;
			}
			Mix_PlayMusic(music, -1);
		}
	}
	else if (windowChange) {
		SDL_DestroyTexture(textmute.texture);
		textmute = loadTextTexture(renderer, "Mute", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 20, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textplay.texture);
		textplay = loadTextTexture(renderer, "Play", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 20, 255, 255, 255, BLENDED);
	}
	if (Mix_PlayingMusic()) {
		textRender(renderer, textmute, LeftBottom, 0, height, no, 200);
		//Mix_HaltMusic();
				//Mix_PausedMusic();
		if (mouse.X<textmute.width && mouse.Y>height - textmute.height) {
			textRender(renderer, textmute, LeftBottom, 0, height, no, 255);
			if (mouse.state == IN_LB_SC) {
				while (!Mix_FadeOutMusic(250) && Mix_PlayingMusic()) {
					// wait for any fades to complete
					SDL_Delay(10);
				}
				Mix_FreeMusic(music);
				music = NULL;
			}
		}
	}
	else {
		textRender(renderer, textplay, LeftBottom, 0, height, no, 200);
		if (mouse.X<textplay.width && mouse.Y>height - textplay.height) {
			textRender(renderer, textplay, LeftBottom, 0, height, no, 255);
			if (mouse.state == IN_LB_SC) {
				switch (value.level) {
				case -1:
					music = Mix_LoadMUS(tone_Path[1]);
					break;
				case 0:
					music = Mix_LoadMUS(tone_Path[0]);
					break;
				case 1:
				case 2:
				case 3:
					music = Mix_LoadMUS(tone_Path[2]);
					break;
				}
				Mix_PlayMusic(music, -1);
			}
		}
		
		
	}
}