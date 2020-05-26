default:
	g++ -Wfatal-errors \
	-std=c++0x \
	source/src/*.cpp \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-lSDL2_image \
	-o game/main.exe

run:
	g++ -Wfatal-errors \
	-std=c++0x \
	source/src/*.cpp \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-lSDL2_image \
	-o game/main.exe
	game/main.exe

clean :
	rm game/*.exe