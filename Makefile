all:
	g++ -I src/include -L src/lib -o main main.cpp Bullet.cpp Entity.cpp -lmingw32 -lSDL2main -lSDL2