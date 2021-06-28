Uint32 clock_add(Uint32 interval, void* param)
{
	ValueData* value = (ValueData*)param;
	//Return 0 when 1 day has past
	value->time = ((value->time) + 1) % (60 * 60 * 24);
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

void controlRender(SDL_Renderer* renderer, const WindowData& fullViewport, const bool windowChange,const Mouse& mouse, char& pause,const ValueData &value, ImageData control_pic[], ImageData build_pic[], ImageData flag_pic) {
	int width = fullViewport.w, height=fullViewport.h;
	static bool select = 0;
	
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
	if (value.money < 0) {
		textRender(renderer, textlose, Top, width / 2, height * 1 / 9 + 0.1 * textmenu.height, no, 255);
		select = 1;
	}


	//top
	boxColor(renderer, 0, 0, width, height / 12, 0xFF073480);  //color aabbggrr
	
	//clock
	filledCircleColor(renderer, height / 15, height / 24, height / 30, 0xFFFFFFFF);
	thickLineColor(renderer, height / 15, height / 24, height / 15 + height / 45 * sin(value.time / 60. / 60 / 12 * 2 * M_PI), height / 24 - height / 45 * cos(value.time / 60. / 60 / 12. * 2 * M_PI), height / 300 + 1, 0xFF008888);
	thickLineColor(renderer, height / 15, height / 24, height / 15 + height / 36 * sin(value.time/ 60. / 60 * 2 * M_PI), height / 24 - height / 36 * cos(value.time / 60. / 60 * 2 * M_PI), height / 600 + 1, 0xFF008888);
	if ((mouse.X - height / 15) * (mouse.X - height / 15) + (mouse.Y - height / 24) * (mouse.Y - height / 24) <= height / 30 * height / 30)
		boxColor(renderer, height / 15-height/30, height / 24 - height / 30, height / 15 + height / 30, height / 24 + height / 30, 0x22FFFFFF);
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

void incident(SDL_Renderer* renderer, const WindowData& fullViewport, bool windowChange, int incident_type, int alpha) {

	if (!(alpha) || (!(incident_type)))
		return;
	//Use alpha to control the interval of words present
	if (alpha > 255)
		alpha = 255;
	int width = fullViewport.w, height = fullViewport.h;
	//Change the words here to choose different incident
	char incident_name[12][50] = {"There is no road!", "There is a building!", "There is no building!", "There is no car!", "No enough money!", "Fire!", "Car Accident!", "Road Closure!", "Delivery!", "Thief!", "Lightening!"};
	char inci_pic_path[50] = "../images/incident.png";
	static ImageData inci_pic = loadImgTexture(renderer, inci_pic_path, 1, 1, 1);
	static TextData incident_text= loadTextTexture(renderer, incident_name[incident_type - 1], "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 10, 10, 10, BLENDED);
	
	//free up resource when closing
	if (renderer == NULL)
		SDL_DestroyTexture(inci_pic.texture);

	//Saving Resources(Only change texture when window changes)
	static int fromer_type;
	if ((fromer_type != incident_type || windowChange)&&(incident_text.texture)) {
		SDL_DestroyTexture(incident_text.texture);
		fromer_type = incident_type;
		incident_text=loadTextTexture(renderer, incident_name[fromer_type-1], "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 27, 10, 10, 10, BLENDED);
	}

	
	//Incident words and background 
	roundedBoxColor(renderer, width - 1.3 * incident_text.width-height/12, height / 12 * 1.1, width-0.1*incident_text.width-height/12, height / 12 * 1.1 + incident_text.height * 1.2, height / 50, 0x88FFFFFF);
	textRender(renderer, incident_text, LeftTop, width - 1.2 * incident_text.width-height/12, height / 12 * 1.1 + 0.1 * incident_text.height, no, alpha);
	imgRender(renderer, inci_pic,  LeftTop, width - 1.2 * incident_text.width - 1.2 * incident_text.height-height/12, height / 12 * 1.1 , NULL, incident_text.height * 1.2, 1, NULL, NULL, 0, no, 255);
	
}

Uint32 incident_add(Uint32 interval, void* param)
{
	int* t = (int*)param;
	//Use alpha to control to the time of presentation
	if ((*t) > 180) {
		(*t) = (*t) -= ((450-(*t))/10+1);
		return interval;
	}
	else {
		(*t) = 0;
		return 0;
	}
}

bool menuRender(SDL_Renderer* renderer, const WindowData& fullViewport, const bool windowChange, const Mouse& mouse, ValueData& value) {
	int width = fullViewport.w, height = fullViewport.h;

	static TextData textlevel1 = loadTextTexture(renderer, "Level1", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	static TextData textlevel2 = loadTextTexture(renderer, "Level2", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	static TextData textlevel3 = loadTextTexture(renderer, "Level3", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	static TextData textexit = loadTextTexture(renderer, "Exit", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	if (renderer == NULL) {
		SDL_DestroyTexture(textlevel1.texture);
		SDL_DestroyTexture(textlevel2.texture);
		SDL_DestroyTexture(textlevel3.texture);
		SDL_DestroyTexture(textexit.texture);
	}
	else if (windowChange) {
		SDL_DestroyTexture(textlevel1.texture);
		textlevel1 = loadTextTexture(renderer, "Level1", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textlevel2.texture);
		textlevel2 = loadTextTexture(renderer, "Level2", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textlevel3.texture);
		textlevel3 = loadTextTexture(renderer, "Level3", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
		SDL_DestroyTexture(textexit.texture);
		textexit = loadTextTexture(renderer, "Exit", "../fonts/TaipeiSansTCBeta-Regular.ttf", height / 9, 255, 255, 255, BLENDED);
	}

	roundedBoxColor(renderer, width / 3, height * 1 / 6, width * 2 / 3, height * 1 / 6 + 1.2 * textexit.height, height / 50, 0xFF888888);
	roundedBoxColor(renderer, width / 3, height * 2 / 6, width * 2 / 3, height * 2 / 6 + 1.2 * textexit.height, height / 50, 0xFF888888);
	roundedBoxColor(renderer, width / 3, height * 3 / 6, width * 2 / 3, height * 3 / 6 + 1.2 * textexit.height, height / 50, 0xFF888888);
	roundedBoxColor(renderer, width / 3, height * 4 / 6, width * 2 / 3, height * 4 / 6 + 1.2 * textexit.height, height / 50, 0xFF888888);
	if (renderer) {
		textRender(renderer, textlevel1, Top, width / 2, height * 1 / 6 + 0.1 * textlevel1.height, no, 255);
		textRender(renderer, textlevel2, Top, width / 2, height * 2/ 6 + 0.1 * textlevel2.height, no, 255);
		textRender(renderer, textlevel3, Top, width / 2, height * 3 / 6 + 0.1 * textlevel3.height, no, 255);
		textRender(renderer, textexit, Top, width / 2, height * 4 / 6 + 0.1 * textexit.height, no, 255);
	}
	if (fabs(mouse.Y - height * 1 / 6 - 0.6 * textlevel1.height) <= 0.6 * textlevel1.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			value.level = 1;
		else
			roundedBoxColor(renderer, width / 3, height * 1 / 6, width * 2 / 3, height * 1 / 6 + 1.2 * textexit.height, height / 50, 0x22FFFFFF);
	if (fabs(mouse.Y - height * 2 / 6 - 0.6 * textlevel2.height) <= 0.6 * textlevel2.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			value.level = 2;
		else
			roundedBoxColor(renderer, width / 3, height * 2/ 6, width * 2 / 3, height * 2 / 6 + 1.2 * textexit.height, height / 50, 0x22FFFFFF);
	if (fabs(mouse.Y - height * 3 / 6 - 0.6 * textlevel3.height) <= 0.6 * textlevel3.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			value.level = 3;
		else
			roundedBoxColor(renderer, width / 3, height * 3 / 6, width * 2 / 3, height * 3 / 6 + 1.2 * textexit.height, height / 50, 0x22FFFFFF);
	if (fabs(mouse.Y - height * 2 / 3 - 0.6 * textexit.height) <= 0.6 * textexit.height && fabs(mouse.X - width / 2) <= width / 6)
		if (mouse.state == IN_LB_SC)
			return true;
		else
			roundedBoxColor(renderer, width / 3, height * 2 / 3, width * 2 / 3, height * 2 / 3 + 1.2 * textexit.height, height / 50, 0x22FFFFFF);
	return false;
}

