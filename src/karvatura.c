#include <stdio.h>

#include <SDL3/SDL.h>


// defines

#define SCREEN_W 			1000
#define SCREEN_H 			1000
#define WIN_TITLE 		"Karvaturan Banaani Peli!"


// asset paths

// PNGs
#define PNG_ANNATKO		"res/image/annatko.png"
#define PNG_VOITIT		"res/image/voitit.png"
#define PNG_HAVISIT 	"res/image/havisit.png"

// WAVs
#define WAV_ANNATKO		"res/audio/annatko.wav"
#define WAV_VOITIT		"res/audio/voitit.wav"


int main() {	
	// Init SDL
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) == false) {
		printf("SDL_Init: Failed to init SDL\n");
		printf("SDL_Init: %s\n", SDL_GetError());
		
		return -1;

	}
	
	// Create window and renderer
	SDL_Window* window 			= NULL;
	SDL_Renderer* renderer 	= NULL;

	if (!SDL_CreateWindowAndRenderer(WIN_TITLE, SCREEN_W, SCREEN_H, SDL_WINDOW_INPUT_FOCUS, &window, &renderer)) {	
		printf("SDL_CreateWindowAndRenderer: Error creating window and/or renderer\n");
		printf("SDL_CreateWindowAndRenderer: %s\n", SDL_GetError());
		
		SDL_Quit();
		return -1;

	}
	
	// Load PNGs
	SDL_Surface* png_Annatko 	= SDL_LoadPNG(PNG_ANNATKO);	
	SDL_Surface* png_Voitit 	= SDL_LoadPNG(PNG_VOITIT);
	SDL_Surface* png_Havisit 	= SDL_LoadPNG(PNG_HAVISIT);
	
	if (png_Annatko == NULL || png_Voitit == NULL || png_Havisit == NULL) {
		printf("SDL_LoadPNG: Error loading PNG image\n");
		printf("SDL_LoadPNG: %s\n", SDL_GetError());

		SDL_DestroySurface(png_Annatko);
		SDL_DestroySurface(png_Voitit);
		SDL_DestroySurface(png_Havisit);
	
		SDL_Quit();
		return -1;
	
	}

	// Create Textures
	SDL_Texture* tex_Annatko = SDL_CreateTextureFromSurface(renderer, png_Annatko);
	SDL_Texture* tex_Voitit 	= SDL_CreateTextureFromSurface(renderer, png_Voitit);
	SDL_Texture* tex_Havisit	= SDL_CreateTextureFromSurface(renderer, png_Havisit);
	
	if(tex_Annatko == NULL || tex_Voitit == NULL || tex_Havisit == NULL) {
		printf("SDL_CreateTextureFromSurface: Error creating texture from surface\n");
		printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
		
		SDL_DestroyTexture(tex_Annatko);
		SDL_DestroyTexture(tex_Voitit);
		SDL_DestroyTexture(tex_Havisit);

		SDL_DestroySurface(png_Annatko);
		SDL_DestroySurface(png_Voitit);
		SDL_DestroySurface(png_Havisit);
	
		SDL_Quit();
		return -1;
		
	}

	// Main loop
	bool running = true;
	while(running) {
		// Input
		SDL_Event e;
		SDL_PollEvent(&e);

		// Update
		if (e.type == SDL_EVENT_QUIT) {
			running = false;
		}
		
		// Render
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);
		SDL_RenderTexture(renderer, tex_Annatko, NULL, NULL);
		SDL_RenderPresent(renderer);

	}

	// Deinit SDL
	SDL_DestroyTexture(tex_Annatko);
	SDL_DestroyTexture(tex_Voitit);
	SDL_DestroyTexture(tex_Havisit);

	SDL_DestroySurface(png_Annatko);
	SDL_DestroySurface(png_Voitit);
	SDL_DestroySurface(png_Havisit);
	
	SDL_Quit();
	return 0;

}
