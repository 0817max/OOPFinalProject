void createEvent(EventData& e, WindowData& fullViewport, ImageData event_pic[], Building**& build, ValueData* value, CarData car[]) {
	e.level = value->level, e.season = value->season;
	e.car = car;
	e.value = value;
	int sx, sy;
	int p, q;
	int* type = new int[6];
	do {
		switch (e.level) {
		case 1:
			p = rand() % 10000;
			p *= 2;
			p = p % 10000;
			if (p < 100)
				e.exist = true;
			break;
		case 2:
			p = rand() % 10000;
			p *= 4;
			p = p % 10000;
			if (p < 250)
				e.exist = true;
			break;
		case 3:
			p = rand() % 10000;
			p *= 8;
			p = p % 10000;
			if (p < 500)
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
			if (rand() % 1000 < 25 * 4 * (e.level))
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
	} while (!((e.exist == true) && (e.type != 6)) && !((e.exist == false) && (e.type == 6)));
	delete[]type;
	//e.exist = true;
	//e.type = rand()%6;
	e.time = 0;
	e.img = event_pic;
	int hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	if ((e.type == 0) || (e.type == 3) || (e.type == 4) || (e.type == 5)) {
		do {
			sx = rand() % wnum;
			sy = rand() % hnum;
		} while (build[sy][sx].type == Empty);
		if (e.exist = true) {
			e.x = (sx + 1);
			e.y = (sy + 1);
		}
	}
	else if ((e.type == 1) || (e.type == 2)) {
		e.h = rand() % 2;
		if (e.h == 1) {
			do {
				sx = rand() % (wnum - 1);
				sy = rand() % hnum;
			} while (((road[sy][sx] >> 8) % 2) * (road[sy][sx + 1] % 2)==0);
			if (e.exist = true) {
				e.x =  (sx + 1);
				e.y =  (sy + 0.5);
			}
			road[sy][sx] += 0x0100;
			road[sy][sx + 1] += 0x0001;
		}
		else if (e.h == 0) {
			do {
				sx = rand() % wnum;
				sy = rand() % (hnum - 1);
			} while (((road[sy][sx] >> 4) % 2) * ((road[sy + 1][sx] >> 12) % 2)==0);
			if (e.exist = true) {
				e.x = (sx + 0.5);
				e.y = (sy + 1);
			}
			road[sy][sx] += 0x0010;
			road[sy + 1][sx] += 0x1000;

		}
	}
}
void Fire(EventData& e, ValueData& value) {
	if (e.time <= 16 - (e.level)) {
		value.love += 25 + e.level * e.level * 5;
		e.time = -510;
	}
	else {
		value.love -= 25 * e.level * e.level;
		value.population -= 4 * e.level;
		value.money -= 100 + 5 * e.level * e.level;
		e.time = -255;
	}

}
void FireRender(SDL_Renderer* renderer, WindowData& fullViewport, EventData& e, ImageData cloud_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	if (e.time < -255)
		imgRender(renderer, cloud_pic[0], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[0].width / cloud_pic[0].height), NULL, 1, NULL, NULL, 0, no, -e.time - 255);
	else
		imgRender(renderer, cloud_pic[1], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[1].width / cloud_pic[1].height), NULL, 1, NULL, NULL, 0, no, -e.time);

}
void CarAccident(EventData& e, ValueData& value) {
	int sx, sy;
	if (e.time <= 18 - (e.level)) {
		value.love += 20 + e.level * e.level * 5;
		e.time = -510;
	}
	else {
		value.love -= 25 * e.level * e.level;
		value.population -= 2 * e.level;
		value.money -= 100 + 5 * e.level * e.level;
		e.time = -255;
	}
	if (e.h == 1) {
		sx = (int)(e.x - 1);
		sy = (int)(e.y - 0.5);
		road[sy][sx] -= 0x0100;
		road[sy][sx + 1] -= 0x0001;
	}
	else if (e.h == 0) {
		sx = (int)(e.x - 0.5);
		sy = (int)(e.y - 1);
		road[sy][sx] -= 0x0010;
		road[sy + 1][sx] -= 0x1000;
	}

}
void CarAccidentRender(SDL_Renderer* renderer, WindowData& fullViewport, EventData& e, ImageData cloud_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	if (e.time < -255)
		imgRender(renderer, cloud_pic[0], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[0].width / cloud_pic[0].height), NULL, 1, NULL, NULL, 0, no, -e.time - 255);
	else
		imgRender(renderer, cloud_pic[1], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[1].width / cloud_pic[1].height), NULL, 1, NULL, NULL, 0, no, -e.time);

}

void RoadClosure(EventData& e, ValueData& value) {
	int sx, sy;
	if (e.time <= 18 - (e.level)) {
		value.love += 20 + e.level * e.level * 5;
		e.time = -510;
	}
	else {
		value.love -= 25 * e.level * e.level;
		value.money -= 100 + 5 * e.level * e.level;
		e.time = -255;
	}
	if (e.h == 1) {
		sx = (int)(e.x - 1);
		sy = (int)(e.y - 0.5);
		road[sy][sx] -= 0x0100;
		road[sy][sx + 1] -= 0x0001;
	}
	else if (e.h == 0) {
		sx = (int)(e.x - 0.5);
		sy = (int)(e.y - 1);
		road[sy][sx] -= 0x0010;
		road[sy + 1][sx] -= 0x1000;
	}

}
void RoadClosureRender(SDL_Renderer* renderer, WindowData& fullViewport, EventData& e, ImageData cloud_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	if (e.time < -255)
		imgRender(renderer, cloud_pic[0], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[0].width / cloud_pic[0].height), NULL, 1, NULL, NULL, 0, no, -e.time - 255);
	else
		imgRender(renderer, cloud_pic[1], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[1].width / cloud_pic[1].height), NULL, 1, NULL, NULL, 0, no, -e.time);

}
void Delivery(EventData& e, ValueData& value) {
	if (e.time <= 25 - (e.level)) {
		value.love += 20 + e.level * e.level * 5;
		value.money += 10 * e.level * e.level;
		e.time = -510;
	}
	else {
		value.love -= 25 * e.level * e.level;
		value.money -= 150 + 5 * e.level * e.level;
		e.time = -255;
	}
}
void DeliveryRender(SDL_Renderer* renderer, WindowData& fullViewport, EventData& e, ImageData cloud_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	if (e.time < -255)
		imgRender(renderer, cloud_pic[0], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[0].width / cloud_pic[0].height), NULL, 1, NULL, NULL, 0, no, -e.time - 255);
	else
		imgRender(renderer, cloud_pic[1], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[1].width / cloud_pic[1].height), NULL, 1, NULL, NULL, 0, no, -e.time);

}
void Thief(EventData& e, ValueData& value) {
	if (e.time <= 16 - (e.level)) {
		value.love += 20 + e.level * e.level * 5;
		e.time = -510;
	}
	else {
		value.love -= 25 * e.level * e.level;
		value.money -= 150 + 5 * e.level * e.level;
		e.time = -255;
	}
}
void ThiefRender(SDL_Renderer* renderer, WindowData& fullViewport, EventData& e, ImageData cloud_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	if (e.time < -255)
		imgRender(renderer, cloud_pic[0], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[0].width / cloud_pic[0].height), NULL, 1, NULL, NULL, 0, no, -e.time - 255);
	else
		imgRender(renderer, cloud_pic[1], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[1].width / cloud_pic[1].height), NULL, 1, NULL, NULL, 0, no, -e.time);

}

void Lightening(EventData& e, ValueData& value) {
	if (e.time <= 16 - (e.level)) {
		value.love += 20 + e.level * e.level * 5;
		e.time = -510;
	}
	else {
		value.love -= 25 * e.level * e.level;
		value.population -= e.level;
		value.money -= 50 + 5 * e.level * e.level;
		e.time = -255;
	}
}
void LighteningRender(SDL_Renderer* renderer, WindowData& fullViewport, EventData& e, ImageData cloud_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	if (e.time<-255)
		imgRender(renderer, cloud_pic[0], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[0].width / cloud_pic[0].height), NULL, 1, NULL, NULL, 0, no, -e.time-255);
	else
		imgRender(renderer, cloud_pic[1], Middle, e.x* (width - height / 12) / (wnum + 1), e.y* (height - height / 12) / (hnum + 1)+height/12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[1].width / cloud_pic[1].height), NULL, 1, NULL, NULL, 0, no, -e.time);
}

Uint32 event_change(Uint32 interval, void* param)
{
	EventData* t = (EventData*)param;
	if (t->exist == false) {
		if (t->time < 0)
			t->time += 40;
		if (t->time >= -295&&t->time<-255)
			t->time = 0;
		return interval;
	}
	else {
		t->time++;
		switch (t->type) {//Àu¥ý¯Å: 0 -> 4 -> 1 -> 5 -> 3 -> 2
		case 0:
			if (t->value->money >= 0)
				t->value->money -= 5 * (t->time / 15) * t->level;
			else {
				if ((t->value->population > 0) && (t->value->population >= t->time))
					t->value->population -= t->time;
			}
			if (t->value->love >= 0)
				t->value->love -= t->time / 20;
			else {
				if ((t->value->population > 0) && (t->value->population >= t->time))
					t->value->population -= t->time;
			}
			if ((t->value->population > 0) && (t->value->population >= t->time / 30))
				t->value->population -= t->time / 30;
			break;
		case 1:
			if (t->value->money >= 0)
				t->value->money -= 5 * (t->time / 20) * t->level;
			else {
				if ((t->value->population > 0) && (t->value->population >= t->time))
					t->value->population -= t->time;
			}
			if (t->value->love >= 0)
				t->value->love -= t->time / 20;
			else {
				if ((t->value->population > 0) && (t->value->population >= t->time))
					t->value->population -= t->time;
			}
			if ((t->value->population > 0) && (t->value->population >= t->time / 35))
				t->value->population -= t->time / 35;
			break;
		case 2:
			if (t->value->money >= 0)
				t->value->money -= 5 * (t->time / 50) * t->level;
			else {
				if ((t->value->population > 0) && (t->value->population >= t->time))
					t->value->population -= t->time;
			}
			if (t->value->love >= 0)
				t->value->love -= t->time / 40;
			else {
				if ((t->value->population > 0) && (t->value->population >= t->time))
					t->value->population -= t->time;
			}
			break;
		case 3:
			if (t->value->money >= 0)
				t->value->money -= 5 * (t->time / 15) * t->level;
			else {
				if ((t->value->population > 0) && (t->value->population >= t->time))
					t->value->population -= t->time;
			}
			if (t->value->love >= 0)
				t->value->love -= t->time / 30;
			else {
				if ((t->value->population > 0) && (t->value->population >= t->time))
					t->value->population -= t->time;
			}
			break;
		case 4:
			if (t->value->money >= 0)
				t->value->money -= 5 * (t->time / 15) * t->level;
			else {
				if ((t->value->population > 0) && (t->value->population >= t->time))
					t->value->population -= t->time;
			}
			if (t->value->love >= 0)
				t->value->love -= t->time / 20;
			else {
				if ((t->value->population > 0) && (t->value->population >= t->time))
					t->value->population -= t->time;
			}
			if ((t->value->population > 0) && (t->value->population >= t->time / 100))
				t->value->population -= t->time / 100;
			break;
		case 5:
			if (t->value->money >= 0)
				t->value->money -= 5 * (t->time / 20) * t->level;
			else {
				if ((t->value->population > 0) && (t->value->population >= t->time))
					t->value->population -= t->time;
			}
			if (t->value->love >= 0)
				t->value->love -= t->time / 35;
			else {
				if ((t->value->population > 0) && (t->value->population >= t->time))
					t->value->population -= t->time;
			}
			if ((t->value->population > 0) && (t->value->population >= t->time / 50))
				t->value->population -= t->time / 50;
			break;
		default:
			break;
		}

		return interval;
	}
}

int eventRender(SDL_Renderer* renderer, WindowData& fullViewport, EventData& e, CarData car[], ImageData cloud_pic[], Building **build, ValueData &value) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum, wnum = fullViewport.wnum;
	void (*f1[6])(SDL_Renderer *,WindowData&, EventData&, ImageData*) = { FireRender,CarAccidentRender,RoadClosureRender,DeliveryRender,ThiefRender,LighteningRender};
	void (*f2[6])(EventData&, ValueData&) = { Fire,CarAccident,RoadClosure,Delivery,Thief,Lightening };
	if (e.exist == false) {
		if(e.time<0)
			(*f1[e.type])(renderer, fullViewport, e, cloud_pic);
		else {
			createEvent(e, fullViewport, e.img, build, &value, car);
			if(e.exist)
				return e.type + 6;
		}
	}
	else {
		for (int i = 0; i < CARNUM; i++) {
			if (car[i].velocity < -1 && ((fabs(car[i].home_y - e.y) + fabs(car[i].home_x - e.x)) < 1e-1)) {
				car[i].velocity = -1;
				//0:fire	1 : car accident	2 : road closure	3 : delivery	4 : thief	5 : lightening
				switch (e.type) {
				case 0:
					if (build[(int)(e.y - 1)][(int)(e.x - 1)].type&& build[(int)(e.y - 1)][(int)(e.x - 1)].type<=7) {
						if (car[i].type != 2)
							continue;
					}
					else {
						if (car[i].type != 1)
							continue;
					}
					break;
				case 1:
				case 2:
				case 4:
					if (car[i].type != 4)
						continue;
					break;
				case 3:
					if (car[i].type != 3)
						continue;
					break;
				case 5:
					if (car[i].type != 1)
						continue;
					break;
				}
				e.exist = false;
				(*f2[e.type])(e, value);
				break;
			}
		}
		if (e.img != NULL)
			if(e.type==1||e.type==2)
				imgRender(renderer, e.img[e.type], Middle, e.x * (width - height / 12) / wnum, e.y * (height - height / 12) / hnum + height / 12, fmin((width - height / 12) / wnum / 4, (height - height / 12) / hnum /2 * e.img[e.type].width / e.img[e.type].height), NULL, 1, NULL, NULL, 0, no, 255);
			else
				imgRender(renderer, e.img[e.type], Middle, e.x * (width - height / 12) / wnum, e.y * (height - height / 12) / hnum + height / 12, fmin((width - height / 12) / wnum * 4 / 5, (height - height / 12) / hnum * 2/ 3 * e.img[e.type].width / e.img[e.type].height), NULL, 1, NULL, NULL, 0, no, 255);
	}
	return 0;
}