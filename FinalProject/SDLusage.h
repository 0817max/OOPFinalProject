struct ImageData
{
	SDL_Texture* texture;
	int width;
	int height;
	int num;
	int wn;
	int hn;
};


struct TextData
{
	SDL_Texture* texture;
	int width;
	int height;
};

struct WindowData {
	int w, h, wnum, hnum;
	int oldw, oldh;
};

enum MouseState
{
	NONE = 0,
	OUT = 1, // Mouse out
	IN_LB_SC = 2,  // Inside, Left Button, Single Click
	IN_RB_SC = 3,  // Inside, RIGHT Button, Single Click
	IN_LB_DC = 4,  // Inside, Left Button, Double Click
	IN_RB_DC = 5,  // Inside, RIGHT Button, Double Click
	IN_LB_PR = 6,  // Inside, Left Button, Press
	IN_RB_PR = 7,  // Inside, Left Button, Press
	IN_WU = 8,  // Inside, Wheel UP
	IN_WD = 9,  // Inside, Wheel DOWN
	HOVER = 10, // Mouse hover
	IN_LB_PR_HOVER = 11 // Inside, Left Button, Press, Hover
};

enum PosPoint {
	LeftTop ,Top, RightTop,
	Left, Middle, Right, 
	LeftBottom, Bottom, RightBottom
};

const int SOLID = 100;
const int SHADED = 101;
const int BLENDED = 102;

// Flip type	
SDL_RendererFlip no = SDL_FLIP_NONE;
SDL_RendererFlip ho = SDL_FLIP_HORIZONTAL;
SDL_RendererFlip ve = SDL_FLIP_VERTICAL;
SDL_RendererFlip hove = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);

int initSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
	// Initialize SDL	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		// Error Handling		
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}

	// Create window	
	// SDL_WINDOWPOS_UNDEFINED: Used to indicate that you don't care what the window position is.
	window = SDL_CreateWindow("City Housekeeper", 50, 50, 960, 480, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
	if (window == NULL)
	{
		printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 2;
	}

	// Initialize PNG loading	
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image failed: %s\n", IMG_GetError());
		return 3;
	}

	// Initialize SDL_ttf
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	if (TTF_Init() == -1)
	{
		// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return 4;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 5;
	}

	return 0;
}


void closeSDL(SDL_Window*& window, SDL_Renderer*& renderer)
{
	// Destroy renderer	
	// Destroy window	
	// Quit Image subsystem
	// Quit SDL subsystems
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// Shutdown and cleanup the truetype font API.
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	TTF_Quit();

	SDL_Quit();
}

ImageData loadImgTexture(SDL_Renderer* renderer, char* path, int num, int hn, int wn)
{
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);

	if (loadedSurface == NULL)
	{
		printf("IMG_Load failed: %s\n", IMG_GetError());
		return { NULL };
	}
	else
	{
		ImageData img;

		// Set the color key (transparent pixel) in a surface.		
		//SDL_SetColorKey(loadedSurface, ckEnable, SDL_MapRGB(loadedSurface->format, r, g, b));

		// Create texture from surface pixels
		img.texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (img.texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
		}

		//Get image dimensions and information
		img.width = loadedSurface->w;
		img.height = loadedSurface->h;
		img.num = num;
		img.wn = wn;
		img.hn = hn;

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);

		//return newTexture;
		return img;
	}
}

TextData loadTextTexture(SDL_Renderer* renderer, const char* str, const char* fontPath, int fontSize, Uint8 fr, Uint8 fg, Uint8 fb, int textType)
{
	// TTF Font sturct
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	TTF_Font* ttfFont = NULL;

	// Open the font
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	ttfFont = TTF_OpenFont(fontPath, fontSize); //("../fonts/lazy.ttf", 28);
	if (ttfFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}

	// A structure that represents a color.
	// https://wiki.libsdl.org/SDL_Color
	SDL_Color textFgColor = { fr, fg, fb }, textBgColor = { NULL, NULL, NULL };

	// Render text surface
	// The way SDL_ttf works is that you create a new image from a font and color. 
	// We're going to be loading our image from text rendered by SDL_ttf instead of a file.
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html#SEC42		

	SDL_Surface* textSurface = NULL;

	// Creates a solid/shaded/blended color surface from the font, text, and color given.
	// https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_35.html
	if (textType == SOLID)
	{
		// Quick and Dirty
		textSurface = TTF_RenderText_Solid(ttfFont, str, textFgColor);
	}
	else if (textType == SHADED)
	{
		// Slow and Nice, but with a Solid Box
		textSurface = TTF_RenderText_Shaded(ttfFont, str, textFgColor, textBgColor);
	}
	else if (textType == BLENDED)
	{
		// Slow Slow Slow, but Ultra Nice over another image
		textSurface = TTF_RenderText_Blended(ttfFont, str, textFgColor);
	}

	// Free font
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	TTF_CloseFont(ttfFont);

	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return { NULL };
	}
	else
	{
		TextData text;

		// Create texture from surface pixels
		text.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (text.texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
		}

		//Get text dimensions and information
		text.width = textSurface->w;
		text.height = textSurface->h;

		// Get rid of old loaded surface
		SDL_FreeSurface(textSurface);

		//return font data
		return text;
	}
}

int imgRender(SDL_Renderer* renderer, const ImageData& img, const PosPoint& Pos,  int posX, int posY, int w, int h, int frame,int cx, int cy, int angle, SDL_RendererFlip flip, int alpha)
{
	int wc = frame % img.wn;
	int hc = frame / img.wn;

	SDL_Rect src, dst;
	src.x = img.width / img.wn * wc;
	src.y = img.height / img.hn * hc;
	src.w = img.width / img.wn;
	src.h = img.height / img.hn;
	if(w==NULL)
		if (h == NULL) {
			dst.w = src.w;
			dst.h = src.h;
		}
		else {
			dst.w = h * src.w / src.h;
			dst.h = h;
		}
	else
		if (h == NULL) {
			dst.w = w;
			dst.h = w * src.h / src.w;
		}
		else {
			dst.w = w;
			dst.h = h;
		}
	if (Pos < 3)
		dst.y = posY;
	else if (Pos < 6)
		dst.y = posY - dst.h / 2;
	else
		dst.y = posY - dst.h;
	if ((Pos % 3) == 0)
		dst.x = posX;
	else if ((Pos % 3) == 1)
		dst.x = posX - dst.w / 2;
	else
		dst.x = posX - dst.w;
	// Set and enable standard alpha blending mode for a texture
	// https://wiki.libsdl.org/SDL_SetTextureBlendMode
	// https://wiki.libsdl.org/SDL_BlendMode
	if (SDL_SetTextureBlendMode(img.texture, SDL_BLENDMODE_BLEND) == -1)
	{
		printf("SDL_SetTextureBlendMode failed: %s\n", SDL_GetError());
		return -1;
	}

	// Modulate texture alpha
	// https://wiki.libsdl.org/SDL_SetTextureAlphaMod
	if (SDL_SetTextureAlphaMod(img.texture, alpha) == -1)
	{
		printf("SDL_SetTextureAlphaMod failed: %s\n", SDL_GetError());
		return -1;
	}

	//SDL_RenderCopy(renderer, img.texture, &src, &dst);
	// SDL_Point
	// A structure that defines a two dimensional point.
	// https://wiki.libsdl.org/SDL_Point
	SDL_Point center = {cx, cy};

	if (center.x == NULL)
		center.x = posX-dst.x;
	if (center.y == NULL)
		center.y= posY-dst.y;
	// Copy a portion of the texture to the current rendering target, 
	// optionally rotating it by angle around the given center and also flipping it top-bottom and/or left-right.
	// if center is NULL, rotation will be done around pos point
	// https://wiki.libsdl.org/SDL_RenderCopyEx
	SDL_RenderCopyEx(renderer, img.texture, NULL, &dst, angle, &center, flip);

	return 1;
}

int textRender(SDL_Renderer* renderer, const TextData& text, const PosPoint& Pos, int posX, int posY, SDL_RendererFlip flip, int alpha)
{
	SDL_Rect r;
	r.w = text.width;
	r.h = text.height;
	if (Pos < 3)
		r.y = posY;
	else if (Pos < 6)
		r.y = posY - r.h / 2;
	else
		r.y = posY - r.h;
	if ((Pos % 3) == 0)
		r.x = posX;
	else if ((Pos % 3) == 1)
		r.x = posX - r.w / 2;
	else
		r.x = posX - r.w;

	// Set and enable standard alpha blending mode for a texture	
	if (SDL_SetTextureBlendMode(text.texture, SDL_BLENDMODE_BLEND) == -1)
	{
		printf("SDL_SetTextureBlendMode failed: %s\n", SDL_GetError());
		return -1;
	}

	// Modulate texture alpha	
	if (SDL_SetTextureAlphaMod(text.texture, alpha) == -1)
	{
		printf("SDL_SetTextureAlphaMod failed: %s\n", SDL_GetError());
		return -1;
	}

	SDL_Point center = { 0, 0 };
	int angle = 0;
	SDL_RenderCopyEx(renderer, text.texture, NULL, &r, angle, &center, flip);
	return 1;
}

void keyboardHandleEvent(SDL_Event& e, bool& quit)
{
	/* The method for "Debunce" */

	// If a key was pressed
	// repeat: non-zero if this is a key repeat
	// https://wiki.libsdl.org/SDL_KeyboardEvent
	//if (e.type == SDL_KEYDOWN)
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:  break;
		case SDLK_DOWN:  break;
		case SDLK_LEFT:  break;
		case SDLK_RIGHT:  break;
		case SDLK_ESCAPE:
			quit = true; break; //press esc for the end of program
		}
	}
	//If a key was released
	//else if (e.type == SDL_KEYUP)
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:  break;
		case SDLK_DOWN:  break;
		case SDLK_LEFT:  break;
		case SDLK_RIGHT: break;
		}
	}
}

void mouseHandleEvent(SDL_Event* e, MouseState* mouseState, int* x, int* y)
{
	// If mouse event happened
	// https://wiki.libsdl.org/SDL_Event
	// https://wiki.libsdl.org/SDL_MouseMotionEvent
	// https://wiki.libsdl.org/SDL_MouseButtonEvent
	// https://wiki.libsdl.org/SDL_MouseWheelEvent
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEWHEEL)
	{
		// Get mouse position
		// https://wiki.libsdl.org/SDL_GetMouseState		
		SDL_GetMouseState(x, y);
		//printf("(%d, %d)\n", *x, *y);

		// Mouse is at left/right/above/below of the button. Mouse is outside the button

		// Mouse is inside button
		{
			static int lasttime = SDL_GetTicks();
			static int curtime = SDL_GetTicks();
			int timediv;

			lasttime = curtime;
			curtime = SDL_GetTicks();
			timediv = curtime - lasttime;

			switch (e->type)
			{
				case SDL_MOUSEBUTTONDOWN:
					if (e->button.state == SDL_PRESSED)
					{
						*mouseState = IN_LB_PR;
					}
					if (e->button.button == SDL_BUTTON_LEFT && timediv > 800)
					{
						*mouseState = IN_LB_PR;
					}
					else if (e->button.button == SDL_BUTTON_RIGHT && timediv > 800)
					{
						*mouseState = IN_RB_PR;
					}
					else if (e->button.button == SDL_BUTTON_LEFT && e->button.clicks == 2 && timediv < 100)
					{
						*mouseState = IN_LB_DC;
					}
					else if (e->button.button == SDL_BUTTON_RIGHT && e->button.clicks == 2 && timediv < 100)
					{
						*mouseState = IN_RB_DC;
					}
					else if (e->button.button == SDL_BUTTON_LEFT && e->button.clicks == 1 && timediv < 800 && timediv > 50)
					{
						*mouseState = IN_LB_SC;
					}
					else if (e->button.button == SDL_BUTTON_RIGHT && e->button.clicks == 1 && timediv < 800 && timediv > 50)
					{
						*mouseState = IN_RB_SC;
					}
					break;

						
				case SDL_MOUSEBUTTONUP:
					*mouseState = NONE;

				
					if (e->button.button == SDL_BUTTON_LEFT && timediv > 800)
					{
						*mouseState = IN_LB_PR;
					}
					else if (e->button.button == SDL_BUTTON_RIGHT && timediv > 800)
					{
						*mouseState = IN_RB_PR;
					}
					else if (e->button.button == SDL_BUTTON_LEFT && e->button.clicks == 2 && timediv < 100)
					{
						*mouseState = IN_LB_DC;
					}
					else if (e->button.button == SDL_BUTTON_RIGHT && e->button.clicks == 2 && timediv < 100)
					{
						*mouseState = IN_RB_DC;
					}
					break;

				case SDL_MOUSEWHEEL:
					if (e->wheel.y > 0) // scroll up
					{
						// Put code for handling "scroll up" here!
						*y = e->wheel.y;
						*mouseState = IN_WU;
					}
					else if (e->wheel.y < 0) // scroll down
					{
						// Put code for handling "scroll down" here!
						*y = e->wheel.y;
						*mouseState = IN_WD;
					}
					break;

				case SDL_MOUSEMOTION:
					*mouseState = HOVER;
					if (e->button.button == SDL_BUTTON_LEFT)
					{
						*mouseState = IN_LB_PR_HOVER;
					}
					break;
			}
		}
	}
}
