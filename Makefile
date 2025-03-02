01:
	@gcc -lSDL2 01-open-window.c -o 01-open-window && ./01-open-window
02:
	@gcc -lSDL2 02-close-window.c -o 02-close-window && ./02-close-window
03:
	@gcc -lSDL2 -lSDL2_image 03-background.c -o 03-background && ./03-background
04:
	@gcc -lSDL2 -lSDL2_image 04-colors.c -o 04-colors && ./04-colors
05:
	@gcc -lSDL2 -lSDL2_image -lSDL2_ttf 05-create-text.c -o 05-create-text && ./05-create-text
06:
	@gcc -lSDL2 -lSDL2_image -lSDL2_ttf 06-moving-text.c -o 06-moving-text && ./06-moving-text
07:
	@gcc -lSDL2 -lSDL2_image -lSDL2_ttf 07-player-sprite.c -o 07-player-sprite && ./07-player-sprite
08:
	@gcc -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer 08-sound-effects.c -o 08-sound-effects && ./08-sound-effects

clean:
	@find . -type f -perm -a=x -maxdepth 1 -delete
