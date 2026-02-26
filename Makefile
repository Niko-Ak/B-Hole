main.exe: main.cpp
	g++ main.cpp \
	-O3 -flto -funroll-loops \
	-fno-math-errno -fno-trapping-math \
	-Wall -Wextra \
	-o main.exe \
	-lraylib -lopengl32 -lgdi32 -lwinmm \

run: main.exe
	./main.exe

clean:
	rm -f main.exe
