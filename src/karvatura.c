#include <SDL3/SDL.h>


// defines

#define SCREEN_W 			1000
#define SCREEN_H 			1000
#define WIN_TITLE 		"Karvaturan Banaani Peli!"


// asset paths

// PNGs
#define PNG_ANNATKO		"res/annatko.png"
#define PNG_VOITIT		"res/voitit.png"
#define PNG_HAVISIT 	"res/havisit.png"

// MP4s
#define MP4_SYO				"res/syo.mp4"

// WAVs
// TODO


int main() {
	if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) == false);
	{
		printf("SDL_Init: Failed to init SDL\n");
		printf("SDL_Init: %s\n", SDL_GetError());
		
		return -1;

	}
	
	SDL_Window* window = SDL_CreateWindow(WIN_TITLE, );
	if ()	
	
	SDL_Quit();
	return 0;

}
