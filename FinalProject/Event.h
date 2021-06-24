struct EventData {
	int season;	//	0:spring	1:summer	2:fall	3:winter
	int level;
	double x, y;
	bool exist;
	int type;	//	0:fire	1:car accident	2:road closure	3:delivery	4:thief	5:lightening	6:no event
	ImageData* img;
	int time; //time start from the event created
	int* p;
	int* m;
	int* l;
};
void createEvent(EventData& e, WindowData& fullViewport, ImageData event_pic[], Building**& build, ValueData* value) {
	e.p = &(value->population), e.m = &(value->money), e.l =&(value->love);
	e.level = value->level, e.season = value->season;
	/*int p, q;
	int* type = new int[6];
	do {
		switch (e.level) {
		case 1:
			p = rand() % 10000;
			p *= 2;
			p = p % 10000;
			if (p > 1000)
				e.exist = true;
			break;
		case 2:
			p = rand() % 10000;
			p *= 4;
			p = p % 10000;
			if (p > 2500)
				e.exist = true;
			break;
		case 3:
			p = rand() % 10000;
			p *= 8;
			p = p % 10000;
			if (p > 5000)
				e.exist = true;
			break;
		}
		switch (e.season) {
		case 0:
			if (rand() % 1000 < 13 * (e.level))
				type[0] = 1;
			else
				type[0] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[1] = 1;
			else
				type[1] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[2] = 1;
			else
				type[2] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[3] = 1;
			else
				type[3] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[4] = 1;
			else
				type[4] = 0;
			if (rand() % 1000 < 25 * 8 * (e.level))
				type[5] = 1;
			else
				type[5] = 0;
			if (type[0] + type[1] + type[2] + type[3] + type[4] + type[5]  == 0)
				e.type = 6;
			else {
				do {
					q = rand() % 6;
				} while (type[q] != 0);
				e.type = q;
			}
			break;
		case 1:
			if (rand() % 1000 < 25 * 8 * (e.level))
				type[0] = 1;
			else
				type[0] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[1] = 1;
			else
				type[1] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[2] = 1;
			else
				type[2] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[3] = 1;
			else
				type[3] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[4] = 1;
			else
				type[4] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[5] = 1;
			else
				type[5] = 0;
			if (type[0] + type[1] + type[2] + type[3] + type[4] + type[5]  == 0)
				e.type = 6;
			else {
				do {
					q = rand() % 6;
				} while (type[q] != 0);
				e.type = q;
			}
			break;
		case 2:
			if (rand() % 1000 < 25 * (e.level))
				type[0] = 1;
			else
				type[0] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[1] = 1;
			else
				type[1] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[2] = 1;
			else
				type[2] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[3] = 1;
			else
				type[3] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[4] = 1;
			else
				type[4] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[5] = 1;
			else
				type[5] = 0;
			if (type[0] + type[1] + type[2] + type[3] + type[4] + type[5]  == 0)
				e.type = 6;
			else {
				do {
					q = rand() % 6;
				} while (type[q] != 0);
				e.type = q;
			}
			break;
		case 3:
			if (rand() % 1000 < 12 * (e.level))
				type[0] = 1;
			else
				type[0] = 0;
			if (rand() % 1000 < 25 *4* (e.level))
				type[1] = 1;
			else
				type[1] = 0;
			if (rand() % 1000 < 25 * 8 * (e.level))
				type[2] = 1;
			else
				type[2] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[3] = 1;
			else
				type[3] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[4] = 1;
			else
				type[4] = 0;
			if (rand() % 1000 < 25 * (e.level))
				type[5] = 1;
			else
				type[5] = 0;
			if (type[0] + type[1] + type[2] + type[3] + type[4] + type[5] == 0)
				e.type = 6;
			else {
				do {
					q = rand() % 6;
				} while (type[q] != 0);
				e.type = q;
			}
			break;
		}
	} while (!((e.exist == true) && (e.type == 6)) && !((e.exist == false) && (e.type != 6)));
	delete[]type;*/
	e.exist = true;
	e.type = 0;
	e.time = 0;
	if (e.type != 6)
		e.img = &event_pic[e.type];
	else
		e.img = NULL;
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	int x, y;
	do {
		x = rand() % wnum;
		y = rand() % hnum;
	} while (build[y][x].type == Empty);
	if (e.exist = true) {
		e.x = (double)(width - height / 12) / (wnum + 1) * (x + 1);
		e.y = height / 12 + (double)(height - height / 12) / (hnum + 1) * (y + 1);
	}
}
/*event ending result
void Fire(SDL_Renderer* renderer, WindowData fullViewport, EventData e) {

}
void CarAccident(SDL_Renderer* renderer, WindowData fullViewport, EventData e) {

}
void RoadClosure(SDL_Renderer* renderer, WindowData fullViewport, EventData e) {

}
void Delivery(SDL_Renderer* renderer, WindowData fullViewport, EventData e) {

}
void Thief(SDL_Renderer* renderer, WindowData fullViewport, EventData e) {

}
void Lightening(SDL_Renderer* renderer, WindowData fullViewport, EventData e) {

}
*/
Uint32 event_change(Uint32 interval, void* param)
{
	EventData* t = (EventData*)param;
	//void (*f[6])(SDL_Renderer*, WindowData, EventData) = {Fire,CarAccident,RoadClosure,Delivery,Thief,Lightening};
	if (t->exist == false) {
		return interval;
	}
	else {
		/*if () {
			t->exist = false;
			(*f[t->type])(renderer,fullViewport,e);
		}*/
		t->time++;
	//	printf("t=%d\n", t->time);
		return interval;
	}
}
void eventRender(SDL_Renderer* renderer, WindowData fullViewport, EventData& e) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	if (e.exist == false) {
		return;
	}
	else {
		if (e.img != NULL)
			imgRender(renderer, *e.img, Middle, e.x, e.y, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * (*e.img).width / (*e.img).height), (height - height / 12) / (hnum + 1) * 4 / 6, 1, NULL, NULL, 0, no, 255);
	}
}