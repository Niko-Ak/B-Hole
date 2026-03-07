ifeq ($(OS),Windows_NT)
    TARGET   = main.exe
    LIBS     = -lraylib -lopengl32 -lgdi32 -lwinmm
    RM       = del /f
else
    TARGET   = main
    LIBS     = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    RM       = rm -f
endif

CXXFLAGS = -O3 -flto -funroll-loops -fno-math-errno -fno-trapping-math -Wall -Wextra

.PHONY: run clean
.DEFAULT_GOAL := run

run: $(TARGET)
	./$(TARGET)

$(TARGET): main.cpp
	g++ main.cpp $(CXXFLAGS) -o $(TARGET) $(LIBS)

clean:
	$(RM) $(TARGET)