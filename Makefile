build:
	g++ -Wfatal-errors \
	-std=c++0x \
	src/*.cpp \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-o main.exe

clean :
	rm *.exe