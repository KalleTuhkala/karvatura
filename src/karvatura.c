#include <stdio.h>

#include <SDL3/SDL.h>


// defines

#define SCREEN_W 			1000
#define SCREEN_H 			1000
#define WIN_TITLE 		"Karvaturan Banaani Peli!"


// asset paths

// PNGs
#define PNG_ANNATKO		"res/image/annatko.png"
#define PNG_MAISTUU_1	"res/image/maistuu1.png"
#define PNG_MAISTUU_2	"res/image/maistuu2.png"
#define PNG_VOITIT		"res/image/voitit.png"
#define PNG_HAVISIT 	"res/image/havisit.png"

// WAVs
#define WAV_ANNATKO		"res/audio/annatko.wav"
#define WAV_MAISTUU		"res/audio/maistuu.wav"
#define WAV_VOITIT		"res/audio/voitit.wav"

// structs
typedef struct {
	int x;
	int y;

} IVec2;

typedef struct {
	IVec2 upLCorner;
	IVec2 downRCorner;

} Rect2D;

// enums
typedef enum {
	ANNATKO,
	MAISTUU,
	VOITIT,
	HAVISIT
	
} GameState;


// Main Function
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
	
	// Open Default Playback Device as a audio stream
	SDL_AudioStream* def_Stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL, NULL, NULL);	
	if(def_Stream == NULL) {
		printf("SDL_OpenAudioDeviceStream: Error opening default playback device as stream\n");
		printf("SDL_OpenAudioDeviceStream: %s\n", SDL_GetError());
		
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return -1;
	
	}
	// unpause def_Stream
	if (SDL_ResumeAudioStreamDevice(def_Stream) == false) {
		printf("SDL_ResumeAudioStreamDevice: Error resuming audio stream device\n");
		printf("SDL_ResumeAudioStreamDevice: %s\n", SDL_GetError());
		
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return -1;
		
	}

	// Load PNGs
	SDL_Surface* png_Annatko 		= SDL_LoadPNG(PNG_ANNATKO);
	SDL_Surface* png_Maistuu_1	= SDL_LoadPNG(PNG_MAISTUU_1);
	SDL_Surface* png_Maistuu_2	= SDL_LoadPNG(PNG_MAISTUU_2);	
	SDL_Surface* png_Voitit 		= SDL_LoadPNG(PNG_VOITIT);
	SDL_Surface* png_Havisit 		= SDL_LoadPNG(PNG_HAVISIT);
	
	if (png_Annatko == NULL || png_Maistuu_1 == NULL || png_Maistuu_2 == NULL || png_Voitit == NULL || png_Havisit == NULL) {
		printf("SDL_LoadPNG: Error loading PNG image\n");
		printf("SDL_LoadPNG: %s\n", SDL_GetError());

		SDL_DestroySurface(png_Annatko);
		SDL_DestroySurface(png_Maistuu_1);
		SDL_DestroySurface(png_Maistuu_2);
		SDL_DestroySurface(png_Voitit);
		SDL_DestroySurface(png_Havisit);
	
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return -1;
	
	}

	// Create Textures
	SDL_Texture* tex_Annatko 		= SDL_CreateTextureFromSurface(renderer, png_Annatko);
	SDL_Texture* tex_Maistuu_1	= SDL_CreateTextureFromSurface(renderer, png_Maistuu_1);
	SDL_Texture* tex_Maistuu_2	= SDL_CreateTextureFromSurface(renderer, png_Maistuu_2);
	SDL_Texture* tex_Voitit 		= SDL_CreateTextureFromSurface(renderer, png_Voitit);
	SDL_Texture* tex_Havisit		= SDL_CreateTextureFromSurface(renderer, png_Havisit);
	
	if(tex_Annatko == NULL || tex_Maistuu_1 == NULL || tex_Maistuu_2 == NULL || tex_Voitit == NULL || tex_Havisit == NULL) {
		printf("SDL_CreateTextureFromSurface: Error creating texture from surface\n");
		printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
		
		SDL_DestroyTexture(tex_Annatko);
		SDL_DestroyTexture(tex_Maistuu_1);
		SDL_DestroyTexture(tex_Maistuu_2);
		SDL_DestroyTexture(tex_Voitit);
		SDL_DestroyTexture(tex_Havisit);

		SDL_DestroySurface(png_Annatko);
		SDL_DestroySurface(png_Maistuu_1);
		SDL_DestroySurface(png_Maistuu_2);
		SDL_DestroySurface(png_Voitit);
		SDL_DestroySurface(png_Havisit);
		
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return -1;
		
	}

	// Create the Audio Specs
	// Audio Spec structs
	SDL_AudioSpec wav_Annatko 	= {.format = SDL_AUDIO_S16LE, .channels = 1, .freq = 44100};
	SDL_AudioSpec wav_Maistuu 	= {.format = SDL_AUDIO_S16LE, .channels = 1, .freq = 44100};
	SDL_AudioSpec wav_Voitit	 	= {.format = SDL_AUDIO_S16LE, .channels = 1, .freq = 44100};
	
	// Audio buffers
	Uint8* wav_Annatko_Buf 	= NULL;	
	Uint8* wav_Maistuu_Buf 	= NULL;	
	Uint8* wav_Voitit_Buf 	= NULL;	

	// Audio lenghts
	Uint32 wav_Annatko_Len 	= 0;
	Uint32 wav_Maistuu_Len 	= 0;
	Uint32 wav_Voitit_Len 	= 0;

	// Load WAV data.
	if (SDL_LoadWAV(WAV_ANNATKO, &wav_Annatko, &wav_Annatko_Buf, &wav_Annatko_Len) == false) {
		printf("SDL_LoadWAV: Error loading WAV data\n");
		printf("SDL_LoadWAV: %s\n", SDL_GetError());
		
		SDL_DestroyTexture(tex_Annatko);
		SDL_DestroyTexture(tex_Maistuu_1);
		SDL_DestroyTexture(tex_Maistuu_2);
		SDL_DestroyTexture(tex_Voitit);
		SDL_DestroyTexture(tex_Havisit);

		SDL_DestroySurface(png_Annatko);
		SDL_DestroySurface(png_Maistuu_1);
		SDL_DestroySurface(png_Maistuu_2);
		SDL_DestroySurface(png_Voitit);
		SDL_DestroySurface(png_Havisit);
	
		SDL_free(wav_Annatko_Buf);
		SDL_free(wav_Maistuu_Buf);
		SDL_free(wav_Voitit_Buf);
		
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return -1;
		
	}

	if (SDL_LoadWAV(WAV_MAISTUU, &wav_Maistuu, &wav_Maistuu_Buf, &wav_Maistuu_Len) == false) {
		printf("SDL_LoadWAV: Error loading WAV data\n");
		printf("SDL_LoadWAV: %s\n", SDL_GetError());
		
		SDL_DestroyTexture(tex_Annatko);
		SDL_DestroyTexture(tex_Maistuu_1);
		SDL_DestroyTexture(tex_Maistuu_2);
		SDL_DestroyTexture(tex_Voitit);
		SDL_DestroyTexture(tex_Havisit);

		SDL_DestroySurface(png_Annatko);
		SDL_DestroySurface(png_Maistuu_1);
		SDL_DestroySurface(png_Maistuu_2);
		SDL_DestroySurface(png_Voitit);
		SDL_DestroySurface(png_Havisit);
	
		SDL_free(wav_Annatko_Buf);
		SDL_free(wav_Maistuu_Buf);
		SDL_free(wav_Voitit_Buf);
		
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return -1;
	
	}

	if (SDL_LoadWAV(WAV_VOITIT, &wav_Voitit, &wav_Voitit_Buf, &wav_Voitit_Len) == false) {
		printf("SDL_LoadWAV: Error loading WAV data\n");
		printf("SDL_LoadWAV: %s\n", SDL_GetError());
		
		SDL_DestroyTexture(tex_Annatko);
		SDL_DestroyTexture(tex_Maistuu_1);
		SDL_DestroyTexture(tex_Maistuu_2);
		SDL_DestroyTexture(tex_Voitit);
		SDL_DestroyTexture(tex_Havisit);

		SDL_DestroySurface(png_Annatko);
		SDL_DestroySurface(png_Maistuu_1);
		SDL_DestroySurface(png_Maistuu_2);
		SDL_DestroySurface(png_Voitit);
		SDL_DestroySurface(png_Havisit);
	
		SDL_free(wav_Annatko_Buf);
		SDL_free(wav_Maistuu_Buf);
		SDL_free(wav_Voitit_Buf);

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		SDL_Quit();
		return -1;
	
	}

	const Rect2D ui_Col_Kylla 	= {{50, 610}, {370, 810}};
	const Rect2D ui_Col_Ei			= {{455, 700}, {710, 850}};

	// set Game State
	GameState gState = ANNATKO;

	// switch to avoid repeating instructions
	static bool x = false;
	
	// Main loop
	bool running = true;
	while(running) {
		// Input
		SDL_Event e;
		SDL_PollEvent(&e);
		
		float mouseX, mouseY = 0;
		SDL_MouseButtonFlags mouseButton = SDL_GetMouseState(&mouseX, &mouseY);

		if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			if (gState == ANNATKO) {
				// KYLLA
				if ((int)mouseX >= ui_Col_Kylla.upLCorner.x && (int)mouseX <= ui_Col_Kylla.downRCorner.x && (int)mouseY >= ui_Col_Kylla.upLCorner.y && (int)mouseY <= ui_Col_Kylla.downRCorner.y && mouseButton == SDL_BUTTON_LEFT) {
					gState = MAISTUU;
				
					x = false;
				
				}	
			
				// EI
				if ((int)mouseX >= ui_Col_Ei.upLCorner.x && (int)mouseX <= ui_Col_Ei.downRCorner.x && (int)mouseY >= ui_Col_Ei.upLCorner.y && (int)mouseY <= ui_Col_Ei.downRCorner.y && mouseButton == SDL_BUTTON_LEFT) {
					gState = HAVISIT;
				
					x = false;
				
				}	
			}
		}
		
		if (e.type == SDL_EVENT_QUIT) {
			running = false;
		}
		
		// Update
		if (gState == ANNATKO) {
			if (SDL_GetAudioStreamQueued(def_Stream) < (int)wav_Annatko_Len && x == false) {
				if (SDL_PutAudioStreamData(def_Stream, wav_Annatko_Buf, (int)wav_Annatko_Len) == false) {
					printf("SDL_PutAudioStreamData: Error putting data to default plauback stream\n");
					printf("SDL_PutAudioStreamData: %s\n", SDL_GetError());
		
					SDL_DestroyTexture(tex_Annatko);
					SDL_DestroyTexture(tex_Maistuu_1);
					SDL_DestroyTexture(tex_Maistuu_2);
					SDL_DestroyTexture(tex_Voitit);
					SDL_DestroyTexture(tex_Havisit);

					SDL_DestroySurface(png_Annatko);
					SDL_DestroySurface(png_Maistuu_1);
					SDL_DestroySurface(png_Maistuu_2);
					SDL_DestroySurface(png_Voitit);
					SDL_DestroySurface(png_Havisit);
	
					SDL_free(wav_Annatko_Buf);
					SDL_free(wav_Maistuu_Buf);
					SDL_free(wav_Voitit_Buf);

					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);

					SDL_Quit();
					return -1;
	
				}

				x = true;
				
			}
		
	
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(renderer);
			SDL_RenderTexture(renderer, tex_Annatko, NULL, NULL);		

		}	
		

		if (gState == MAISTUU) {
			if (SDL_GetAudioStreamQueued(def_Stream) < (int)wav_Maistuu_Len && x == false) {
				if (SDL_PutAudioStreamData(def_Stream, wav_Maistuu_Buf, (int)wav_Maistuu_Len) == false) {
					printf("SDL_PutAudioStreamData: Error putting data to default plauback stream\n");
					printf("SDL_PutAudioStreamData: %s\n", SDL_GetError());
		
					SDL_DestroyTexture(tex_Annatko);
					SDL_DestroyTexture(tex_Maistuu_1);
					SDL_DestroyTexture(tex_Maistuu_2);
					SDL_DestroyTexture(tex_Voitit);
					SDL_DestroyTexture(tex_Havisit);

					SDL_DestroySurface(png_Annatko);
					SDL_DestroySurface(png_Maistuu_1);
					SDL_DestroySurface(png_Maistuu_2);
					SDL_DestroySurface(png_Voitit);
					SDL_DestroySurface(png_Havisit);
	
					SDL_free(wav_Annatko_Buf);
					SDL_free(wav_Maistuu_Buf);
					SDL_free(wav_Voitit_Buf);

					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);

					SDL_Quit();
					return -1;
	
				}
			
				x = true;				

			}
			
			static bool frameSwitch = true;
			if (frameSwitch) {
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
				SDL_RenderClear(renderer);
				SDL_RenderTexture(renderer, tex_Maistuu_1, NULL, NULL);			
	
				frameSwitch = false;

			} else {
				SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
				SDL_RenderClear(renderer);
				SDL_RenderTexture(renderer, tex_Maistuu_2, NULL, NULL);	
			
				frameSwitch = true;

			}		
	
			if (SDL_GetAudioStreamQueued(def_Stream) <= 0) {
				gState = VOITIT;
				x = false;				

			}

		}
		
		if (gState == VOITIT) {
			if (SDL_GetAudioStreamQueued(def_Stream) < (int)wav_Voitit_Len && x == false) {
				if (SDL_PutAudioStreamData(def_Stream, wav_Voitit_Buf, (int)wav_Voitit_Len) == false) {
					printf("SDL_PutAudioStreamData: Error putting data to default plauback stream\n");
					printf("SDL_PutAudioStreamData: %s\n", SDL_GetError());
		
					SDL_DestroyTexture(tex_Annatko);
					SDL_DestroyTexture(tex_Maistuu_1);
					SDL_DestroyTexture(tex_Maistuu_2);
					SDL_DestroyTexture(tex_Voitit);
					SDL_DestroyTexture(tex_Havisit);

					SDL_DestroySurface(png_Annatko);
					SDL_DestroySurface(png_Maistuu_1);
					SDL_DestroySurface(png_Maistuu_2);
					SDL_DestroySurface(png_Voitit);
					SDL_DestroySurface(png_Havisit);
	
					SDL_free(wav_Annatko_Buf);
					SDL_free(wav_Maistuu_Buf);
					SDL_free(wav_Voitit_Buf);

					SDL_DestroyRenderer(renderer);
					SDL_DestroyWindow(window);

					SDL_Quit();
					return -1;
	
				}
				
			x = true;
				
			}
		
	
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(renderer);
			SDL_RenderTexture(renderer, tex_Voitit, NULL, NULL);
			
			if(SDL_GetAudioStreamQueued(def_Stream) <= 0) {
				gState = ANNATKO;
				x = false;

			}			
			
		}	
		
		if (gState == HAVISIT) {
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(renderer);
			SDL_RenderTexture(renderer, tex_Havisit, NULL, NULL);	

		}

		SDL_RenderPresent(renderer);
		
		if (gState == HAVISIT) {	
			SDL_Delay(3000);
			gState = ANNATKO;
			x = false;

		}

	}

	// Deinit SDL
	SDL_DestroyTexture(tex_Annatko);
	SDL_DestroyTexture(tex_Maistuu_1);
	SDL_DestroyTexture(tex_Maistuu_2);
	SDL_DestroyTexture(tex_Voitit);
	SDL_DestroyTexture(tex_Havisit);

	SDL_DestroySurface(png_Annatko);
	SDL_DestroySurface(png_Maistuu_1);
	SDL_DestroySurface(png_Maistuu_2);
	SDL_DestroySurface(png_Voitit);
	SDL_DestroySurface(png_Havisit);
	
	SDL_free(wav_Annatko_Buf);
	SDL_free(wav_Maistuu_Buf);
	SDL_free(wav_Voitit_Buf);
		
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;

}
