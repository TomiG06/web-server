FLAGS = -o ws
SOURCE_F = main.cc server.cc server.h

ws: main.cc server.cc server.h
	$(CXX) $(FLAGS) $(SOURCE_F)
