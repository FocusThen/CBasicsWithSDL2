01:
	@gcc -lSDL2 01-open-window.c -o 01-open-window && ./01-open-window
02:
	@gcc -lSDL2 02-close-window.c -o 02-close-window && ./02-close-window
03:
	@gcc -lSDL2 -lSDL2_image 03-background.c -o 03-background && ./03-background


clean:
	@find . -type f -perm -a=x -maxdepth 1 -delete
