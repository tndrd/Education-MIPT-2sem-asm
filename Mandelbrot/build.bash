declare -x PKG_CONFIG_PATH=/usr/local/lib/pkgconfig
declare -x LD_LIBRARY_PATH=/home/tndnrd/Projects/lib/glfw-3.3.3/src

mkdir NoSSE
g++     $(pkg-config --cflags glfw3) -o NoSSE/NoSSE_0 NoSSE.cpp $(pkg-config --static --libs glfw3 gl)
g++ -O1 $(pkg-config --cflags glfw3) -o NoSSE/NoSSE_1 NoSSE.cpp $(pkg-config --static --libs glfw3 gl)
g++ -O2 $(pkg-config --cflags glfw3) -o NoSSE/NoSSE_2 NoSSE.cpp $(pkg-config --static --libs glfw3 gl)
g++ -O3 $(pkg-config --cflags glfw3) -o NoSSE/NoSSE_3 NoSSE.cpp $(pkg-config --static --libs glfw3 gl)

mkdir SSE
g++     $(pkg-config --cflags glfw3) -o SSE/SSE_0 SSE.cpp $(pkg-config --static --libs glfw3 gl)
g++ -O1 $(pkg-config --cflags glfw3) -o SSE/SSE_1 SSE.cpp $(pkg-config --static --libs glfw3 gl)
g++ -O2 $(pkg-config --cflags glfw3) -o SSE/SSE_2 SSE.cpp $(pkg-config --static --libs glfw3 gl)
g++ -O3 $(pkg-config --cflags glfw3) -o SSE/SSE_3 SSE.cpp $(pkg-config --static --libs glfw3 gl)

mkdir AVX
g++     $(pkg-config --cflags glfw3) -o AVX/AVX_0 AVX.cpp -mavx -mavx2 $(pkg-config --static --libs glfw3 gl)
g++ -O1 $(pkg-config --cflags glfw3) -o AVX/AVX_1 AVX.cpp -mavx -mavx2 $(pkg-config --static --libs glfw3 gl)
g++ -O2 $(pkg-config --cflags glfw3) -o AVX/AVX_2 AVX.cpp -mavx -mavx2 $(pkg-config --static --libs glfw3 gl)
g++ -O3 $(pkg-config --cflags glfw3) -o AVX/AVX_3 AVX.cpp -mavx -mavx2 $(pkg-config --static --libs glfw3 gl)

mkdir AVX+FMA
g++     $(pkg-config --cflags glfw3) -o AVX+FMA/AVX+FMA_0 AVX+FMA.cpp -mavx -mavx2 -mfma $(pkg-config --static --libs glfw3 gl)
g++ -O1 $(pkg-config --cflags glfw3) -o AVX+FMA/AVX+FMA_1 AVX+FMA.cpp -mavx -mavx2 -mfma $(pkg-config --static --libs glfw3 gl)
g++ -O2 $(pkg-config --cflags glfw3) -o AVX+FMA/AVX+FMA_2 AVX+FMA.cpp -mavx -mavx2 -mfma $(pkg-config --static --libs glfw3 gl)
g++ -O3 $(pkg-config --cflags glfw3) -o AVX+FMA/AVX+FMA_3 AVX+FMA.cpp -mavx -mavx2 -mfma $(pkg-config --static --libs glfw3 gl)