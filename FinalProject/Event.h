struct EventData {
	int season;	//	0:spring	1:summer	2:fall	3:winter
	int level;
	double x, y;
	bool exist;
	int type;	//	0:fire	1:traffic jam	2:car accident	3:road closure	4:delivery	5:thief	6:no event
	WindowData* window;
	ImageData* img;
	int time; //time start from the event created
};
void createEvent(EventData& e, WindowData& fullViewport) {
	int p, q;
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
			if (rand() % 1000 < 25 * 4 * (e.level))
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
			if (type[0] + type[1] + type[2] + type[3] + type[4] + type[5] == 0)
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
			if (type[0] + type[1] + type[2] + type[3] + type[4] + type[5] == 0)
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
			if (type[0] + type[1] + type[2] + type[3] + type[4] + type[5] == 0)
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
			if (rand() % 1000 < 25 * (e.level))
				type[1] = 1;
			else
				type[1] = 0;
			if (rand() % 1000 < 25 * 4 * (e.level))
				type[2] = 1;
			else
				type[2] = 0;
			if (rand() % 1000 < 25 * 8 * (e.level))
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
	delete[]type;
	e.time = 0;
}
void Fire(SDL_Renderer* renderer, WindowData fullViewport, EventData e) {
	//create random point where the event happen
	//imgRender();
}
void TrafficJam(SDL_Renderer* renderer, WindowData fullViewport, EventData e) {
	//create random point where the event happen
	//imgRender();
}
void CarAccident(SDL_Renderer* renderer, WindowData fullViewport, EventData e) {
	//create random point where the event happen
	//imgRender();
}
void RoadClosure(SDL_Renderer* renderer, WindowData fullViewport, EventData e) {
	//create random point where the event happen
	//imgRender();
}
void Delivery(SDL_Renderer* renderer, WindowData fullViewport, EventData e) {
	//create random point where the event happen
	//imgRender();
}
void Thief(SDL_Renderer* renderer, WindowData fullViewport, EventData e) {
	//create random point where the event happen
	//imgRender();
}
Uint32 event_change(Uint32 interval, void* param)
{
	EventData* t = (EventData*)param;

	if (t->exist == false)
		return interval;
	else {
		t->time++;
		return interval;
	}
}
void eventRender(SDL_Renderer* renderer, WindowData fullViewport, EventData& e) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum, wnum = fullViewport.wnum;
	void (*f[6]) (SDL_Renderer*, WindowData, EventData) = { Fire,TrafficJam,CarAccident,RoadClosure,Delivery,Thief };
	if (e.exist == false)
		return;
	else
		(*f[e.type])(renderer, fullViewport, e);
}