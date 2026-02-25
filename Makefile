run: main
			./main
main: main.cpp
			g++ main.cpp \
-O3 -flto -funroll-loops \
-fno-math-errno -fno-trapping-math \
-Wall -Wextra \
-o main \
$(pkg-config --cflags --libs raylib)

