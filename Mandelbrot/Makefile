SetEnv:
	declare -x PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
	declare -x LD_LIBRARY_PATH=/home/tndnrd/Projects/lib/glfw-3.3.3/src

NoSSE:
	g++ $(pkg-config --cflags glfw3) -o NoSSE NoSSE.cpp $(pkg-config --static --libs glfw3 gl)
SSE:
	g++ $(pkg-config --cflags glfw3) -o SSE SSE.cpp $(pkg-config --static --libs glfw3 gl)