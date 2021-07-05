void createEvent(EventData& e, const WindowData& fullViewport, Building**& build, ValueData* value, CarData car[], InciData& inci, int carnum) {
	
	e.level = &(value->level), e.season = &(value->season);
	e.car = car;
	e.value = value;
	e.assign=true;
	int sx, sy;
	int p, q;
	int* type = new int[6];
	do {
		switch (*(e.level)) {
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
		switch (*(e.season)) {
		case 0:
			if (rand() % 1000 < 13 * (*(e.level)))
				type[0] = 1;
			else
				type[0] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[1] = 1;
			else
				type[1] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[2] = 1;
			else
				type[2] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[3] = 1;
			else
				type[3] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[4] = 1;
			else
				type[4] = 0;
			if (rand() % 1000 < 25 * 8 * (*(e.level)))
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
			if (rand() % 1000 < 25 * 8 * (*(e.level)))
				type[0] = 1;
			else
				type[0] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[1] = 1;
			else
				type[1] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[2] = 1;
			else
				type[2] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[3] = 1;
			else
				type[3] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[4] = 1;
			else
				type[4] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
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
			if (rand() % 1000 < 25 * (*(e.level)))
				type[0] = 1;
			else
				type[0] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[1] = 1;
			else
				type[1] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[2] = 1;
			else
				type[2] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[3] = 1;
			else
				type[3] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[4] = 1;
			else
				type[4] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
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
			if (rand() % 1000 < 12 * (*(e.level)))
				type[0] = 1;
			else
				type[0] = 0;
			if (rand() % 1000 < 25 * 4 * (*(e.level)))
				type[1] = 1;
			else
				type[1] = 0;
			if (rand() % 1000 < 25 * 8 * (*(e.level)))
				type[2] = 1;
			else
				type[2] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[3] = 1;
			else
				type[3] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
				type[4] = 1;
			else
				type[4] = 0;
			if (rand() % 1000 < 25 * (*(e.level)))
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
			} while (((road[sy][sx] >> 8) % 2) * (road[sy][sx + 1] % 2) == 0);
			if (e.exist = true) {
				e.x = (sx + 1);
				e.y = (sy + 0.5);
			}
			road[sy][sx] += 0x0100;
			road[sy][sx + 1] += 0x0001;
		}
		else if (e.h == 0) {
			do {
				sx = rand() % wnum;
				sy = rand() % (hnum - 1);
			} while (((road[sy][sx] >> 4) % 2) * ((road[sy + 1][sx] >> 12) % 2) == 0);
			if (e.exist = true) {
				e.x = (sx + 0.5);
				e.y = (sy + 1);
			}
			road[sy][sx] += 0x0010;
			road[sy + 1][sx] += 0x1000;

		}
	}
	if(e.exist)
		switch (carnum) {
		case 0:
			inci.car1 = e.type + 1;
			inci.alpha[2] = 450;
			break;
		case 1:
			inci.car2 = e.type + 1;
			inci.alpha[3] = 450;
			break;
		case 2:
			inci.car3 = e.type + 1;
			inci.alpha[4] = 450;
			break;
		}
	
}
void Fire(EventData& e, ValueData& value) {
	int t;
	switch (*e.level) {
	case 1:
		t = 18;
		break;
	case 2:
		t = 25;
		break;
	case 3:
		t = 35;
		break;
	default:
		break;
	}
	if (e.time <= t) {
		value.love += 26 + *(e.level) * *(e.level) * 6 + 125 * *(e.level) / (e.time+1);
		value.money += *(e.level) * 10;
		e.time = -510;
	}
	else {
		value.love -= 25 * *(e.level) * *(e.level);
		value.population -= 4 * *(e.level);
		value.money -= 100 + 5 * *(e.level) * *(e.level);
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
	int t;
	switch (*e.level) {
	case 1:
		t = 20;
		break;
	case 2:
		t = 26;
		break;
	case 3:
		t = 36;
		break;
	default:
		break;
	}
	if (e.time <= t) {
		value.love += 25 + *(e.level) * *(e.level) * 6 + 100 * *(e.level) / (e.time+1);
		value.money += *(e.level) * 10;
		e.time = -510;
	}
	else {
		value.love -= 25 * *(e.level) * *(e.level);
		value.population -= 2 * *(e.level);
		value.money -= 100 + 5 * *(e.level) * *(e.level);
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
	int t;
	switch (*e.level) {
	case 1:
		t = 30;
		break;
	case 2:
		t = 35;
		break;
	case 3:
		t = 45;
		break;
	default:
		break;
	}
	if (e.time <= t) {
		value.love += 20 + *(e.level) * *(e.level) * 5 + 100 * *(e.level) / (e.time+1);
		value.money += *(e.level) * 10;
		e.time = -510;
	}
	else {
		value.love -= 25 * *(e.level) * *(e.level);
		value.money -= 100 + 5 * *(e.level) * *(e.level);
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
	int t;
	switch (*e.level) {
	case 1:
		t = 30;
		break;
	case 2:
		t = 35;
		break;
	case 3:
		t = 45;
		break;
	default:
		break;
	}
	if (e.time <= t) {
		value.love += 20 + *(e.level) * *(e.level) * 5 + 100 * *(e.level) / (e.time+1);
		value.money += 50 * *(e.level) * *(e.level) + 250 * *(e.level) / (e.time+1);
		e.time = -510;
	}
	else {
		value.love -= 25 * *(e.level) * *(e.level);
		value.money -= 150 + 5 * *(e.level) * *(e.level);
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
	int t;
	switch (*e.level) {
	case 1:
		t = 25;
		break;
	case 2:
		t = 30;
		break;
	case 3:
		t = 40;
		break;
	default:
		break;
	}
	if (e.time <= t) {
		value.love += 25 + *(e.level) * *(e.level) * 5 + 100 * *(e.level) / (e.time+1);
		value.money += *(e.level) * 10;
		e.time = -510;
	}
	else {
		value.love -= 25 * *(e.level) * *(e.level);
		value.money -= 150 + 5 * *(e.level) * *(e.level);
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
	int t;
	switch (*e.level) {
	case 1:
		t = 25;
		break;
	case 2:
		t = 30;
		break;
	case 3:
		t = 40;
		break;
	default:
		break;
	}
	if (e.time <= t) {
		value.love += 25 + *(e.level) * *(e.level) * 6 + 100 * *(e.level) / (e.time+1);
		value.money += *(e.level) * 10;
		e.time = -510;
	}
	else {
		value.love -= 25 * *(e.level) * *(e.level);
		value.population -= *(e.level);
		value.money -= 50 + 5 * *(e.level) * *(e.level);
		e.time = -255;
	}
}
void LighteningRender(SDL_Renderer* renderer, WindowData& fullViewport, EventData& e, ImageData cloud_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum - 1, wnum = fullViewport.wnum - 1;
	if (e.time < -255)
		imgRender(renderer, cloud_pic[0], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[0].width / cloud_pic[0].height), NULL, 1, NULL, NULL, 0, no, -e.time - 255);
	else
		imgRender(renderer, cloud_pic[1], Middle, e.x * (width - height / 12) / (wnum + 1), e.y * (height - height / 12) / (hnum + 1) + height / 12, fmin((width - height / 12) / (wnum + 1) * 8 / 10, (height - height / 12) / (hnum + 1) * 4 / 6 * cloud_pic[1].width / cloud_pic[1].height), NULL, 1, NULL, NULL, 0, no, -e.time);
}

Uint32 event_change(Uint32 interval, void* param)
{
	EventData* e = (EventData*)param;
	for (int i = 0; i < *(e[0].level); i++) {
		if (e[i].exist == false) {
			if (e[i].time < 0)
				e[i].time += 40;
			if (e[i].time >= -295 && e[i].time < -255)
				e[i].time = 0;
		}
		else {
			e[i].time++;
			if (e[i].time >= 5 * *(e[i].level) + *(e[i].level) * *(e[i].level)) {
				switch (e[i].type) {//Àu¥ý¯Å: 0 -> 1 -> 5 -> 4 -> 3 -> 2
				case 0:
					if (e[i].value->money >= 0)
						e[i].value->money -= 10 * *(e[i].level) * e[i].time / (5 * (4 - *(e[i].level)));
					else {
						if ((e[i].value->population > 0) && (e[i].value->population >= e[i].time) && (e[i].time % 5 == 0))
							e[i].value->population -= e[i].time;
					}
					if (e[i].value->love >= 0)
						e[i].value->love -= 5 * *(e[i].level) * e[i].time / (10 * (4 - *(e[i].level)));
					else {
						if ((e[i].value->population > 0) && (e[i].value->population >= e[i].time) && (e[i].time % 5 == 0))
							e[i].value->population -= e[i].time;
					}
					if ((e[i].value->population > 0) && (e[i].value->population >= *(e[i].level) * e[i].time / (10 * (4 - *(e[i].level)))))
						e[i].value->population -= *(e[i].level) * e[i].time / (10 * (4 - *(e[i].level)));
					break;
				case 1:
					if (e[i].value->money >= 0)
						e[i].value->money -= 6 * *(e[i].level) * e[i].time / (6 * (4 - *(e[i].level)));
					else {
						if ((e[i].value->population > 0) && (e[i].value->population >= e[i].time) && (e[i].time % 5 == 0))
							e[i].value->population -= e[i].time;
					}
					if (e[i].value->love >= 0)
						e[i].value->love -= 5 * *(e[i].level) * e[i].time / (12 * (4 - *(e[i].level)));
					else {
						if ((e[i].value->population > 0) && (e[i].value->population >= e[i].time) && (e[i].time % 5 == 0))
							e[i].value->population -= e[i].time;
					}
					if ((e[i].value->population > 0) && (e[i].value->population >= *(e[i].level) * e[i].time / (12 * (4 - *(e[i].level)))))
						e[i].value->population -= *(e[i].level) * e[i].time / (12 * (4 - *(e[i].level)));
					break;
				case 2:
					if (e[i].value->money >= 0)
						e[i].value->money -= 8 * *(e[i].level) * e[i].time / (10 * (4 - *(e[i].level)));
					else {
						if ((e[i].value->population > 0) && (e[i].value->population >= e[i].time) && (e[i].time % 10 == 0))
							e[i].value->population -= e[i].time;
					}
					if (e[i].value->love >= 0)
						e[i].value->love -= 2 * *(e[i].level) * e[i].time / (15 * (4 - *(e[i].level)));
					else {
						if ((e[i].value->population > 0) && (e[i].value->population >= e[i].time) && (e[i].time % 10 == 0))
							e[i].value->population -= e[i].time;
					}
					break;
				case 3:
					if (e[i].value->money >= 0)
						e[i].value->money -= 10 * *(e[i].level) * e[i].time / (10 * (4 - *(e[i].level)));
					else {
						if ((e[i].value->population > 0) && (e[i].value->population >= e[i].time) && (e[i].time % 10 == 0))
							e[i].value->population -= e[i].time;
					}
					if (e[i].value->love >= 0)
						e[i].value->love -= 2 * *(e[i].level) * e[i].time / (15 * (4 - *(e[i].level)));
					else {
						if ((e[i].value->population > 0) && (e[i].value->population >= e[i].time) && (e[i].time % 10 == 0))
							e[i].value->population -= e[i].time;
					}
					break;
				case 4:
					if (e[i].value->money >= 0)
						e[i].value->money -= 15 * *(e[i].level) * e[i].time / (10 * (4 - *(e[i].level)));
					else {
						if ((e[i].value->population > 0) && (e[i].value->population >= e[i].time) && (e[i].time % 10 == 0))
							e[i].value->population -= e[i].time;
					}
					if (e[i].value->love >= 0)
						e[i].value->love -= 3 * *(e[i].level) * e[i].time / (15 * (4 - *(e[i].level)));
					else {
						if ((e[i].value->population > 0) && (e[i].value->population >= e[i].time) && (e[i].time % 10 == 0))
							e[i].value->population -= e[i].time;
					}
					if ((e[i].value->population > 0) && (e[i].value->population >= *(e[i].level) * e[i].time / (15 * (4 - *(e[i].level)))))
						e[i].value->population -= *(e[i].level) * e[i].time / (15 * (4 - *(e[i].level)));
					break;
				case 5:
					if (e[i].value->money >= 0)
						e[i].value->money -= 8 * *(e[i].level) * e[i].time / (6 * (4 - *(e[i].level)));
					else {
						if ((e[i].value->population > 0) && (e[i].value->population >= e[i].time) && (e[i].time % 5 == 0))
							e[i].value->population -= e[i].time;
					}
					if (e[i].value->love >= 0)
						e[i].value->love -= 3 * *(e[i].level) * e[i].time / (12 * (4 - *(e[i].level)));
					else {
						if ((e[i].value->population > 0) && (e[i].value->population >= e[i].time) && (e[i].time % 5 == 0))
							e[i].value->population -= e[i].time;
					}
					if ((e[i].value->population > 0) && (e[i].value->population >= *(e[i].level) * e[i].time / (12 * (4 - *(e[i].level)))))
						e[i].value->population -= *(e[i].level) * e[i].time / (12 * (4 - *(e[i].level)));
					break;
				default:
					break;
				}
			}



		}
	}
	return interval;
}

void eventRender(SDL_Renderer* renderer, WindowData& fullViewport, EventData* e, CarData car[], ImageData cloud_pic[], Building **build, ValueData &value, InciData& inci, ImageData event_pic[]) {
	int width = fullViewport.w, height = fullViewport.h, hnum = fullViewport.hnum, wnum = fullViewport.wnum, carnum = fullViewport.carnum;
	void (*f1[6])(SDL_Renderer *,WindowData&, EventData&, ImageData*) = { FireRender,CarAccidentRender,RoadClosureRender,DeliveryRender,ThiefRender,LighteningRender};
	void (*f2[6])(EventData&, ValueData&) = { Fire,CarAccident,RoadClosure,Delivery,Thief,Lightening };
	for (int i = 0; i < value.level; i++) {
		if (e[i].exist == false) {
			if (e[i].time < 0)
				(*f1[e[i].type])(renderer, fullViewport, e[i], cloud_pic);
			else {
				createEvent(e[i], fullViewport, build, &value, car, inci, i);
			}
		}
		else {
			for (int j = 0; j < carnum; j++) {
				if (car[j].velocity < -1 && ((fabs(car[j].home_y - e[i].y) + fabs(car[j].home_x - e[i].x)) < 1e-1)) {
					car[j].velocity = -1;
					//0:fire	1 : car accident	2 : road closure	3 : delivery	4 : thief	5 : lightening
					switch (e[i].type) {
					case 0:
						if (build[(int)(e[i].y - 1)][(int)(e[i].x - 1)].type && build[(int)(e[i].y - 1)][(int)(e[i].x - 1)].type <= 7) {
							if (car[j].type != 2)
								continue;
						}
						else {
							if (car[j].type != 1)
								continue;
						}
						break;
					case 1:
					case 2:
					case 4:
						if (car[j].type != 4)
							continue;
						break;
					case 3:
						if (car[j].type != 3)
							continue;
						break;
					case 5:
						if (car[j].type != 1)
							continue;
						break;
					}
					e[i].exist = false;
					(*f2[e[i].type])(e[i], value);
					break;
				}
			}
			if (e[i].type == 1 || e[i].type == 2)
				imgRender(renderer, event_pic[e[i].type], Middle, e[i].x * (width - height / 12) / wnum, e[i].y * (height - height / 12) / hnum + height / 12, fmin((width - height / 12) / wnum / 4, (height - height / 12) / hnum / 2 * event_pic[e[i].type].width / event_pic[e[i].type].height), NULL, 1, NULL, NULL, 0, no, 255);
			else
				imgRender(renderer, event_pic[e[i].type], Middle, e[i].x * (width - height / 12) / wnum, e[i].y * (height - height / 12) / hnum + height / 12, fmin((width - height / 12) / wnum * 4 / 5, (height - height / 12) / hnum * 2 / 3 * event_pic[e[i].type].width / event_pic[e[i].type].height), NULL, 1, NULL, NULL, 0, no, 255);
		}
	}
}