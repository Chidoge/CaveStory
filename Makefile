build:
	g++ -Wfatal-errors \
	-std=c++0x \
	src/*.cpp \
	-I"C:\libsdl\include" \
	-L"C:\libsdl\lib" \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-o main.exe