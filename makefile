FLAGS = -o ws -pthread -Wall -Wextra -pedantic
SOURCE_F = main.cc server.cc

ws: main.cc server.cc server.h
	$(CXX) $(FLAGS) $(SOURCE_F)
