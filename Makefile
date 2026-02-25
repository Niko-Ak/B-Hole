run: main
			./main
main: main.cpp
			g++ main.cpp -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

