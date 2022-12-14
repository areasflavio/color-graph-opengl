all:
	g++ main.cpp camera.cpp -o colorGraph -lGL -lGLU -lglut -lglfw

clean:
	rm colorGraph