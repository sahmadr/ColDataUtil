CC = g++
CFLAGS = -std=c++17 -Wpedantic -Wall -Wextra -O3
SRC = output.cpp cmdArgs.cpp colData.cpp main.cpp libfftw3.a  
EXE = coldatautil

cppNSSolver3D:
	@$(CC) $(CFLAGS) $(SRC) -o $(EXE)
	@echo "ColDataUtil compiled successfully!"

clean:
	@rm $(EXE)
	@echo "ColDataUtil project cleaned!"
