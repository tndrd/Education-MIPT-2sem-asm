#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <x86intrin.h>
#include <GLFW/glfw3.h>
#include "assert.h"

const unsigned int W = 800;
const unsigned int H = 400;

const float Re_start = -1;
const float Im_start = 0;

const float Re_width = 2;
const float Im_width = 1;

const float Scale_koeff = 1.0f;
const float Re_scale = 2 * Re_width / W * Scale_koeff;
const float Im_scale = 2 * Im_width / H * Scale_koeff;

const size_t N_MAX_VALUE = 256;
const float MAX_ABS = 50;

const float MOVE_COEF = 0.05f;

struct SSE_vector
{
    private:
    float* aligned_mem = nullptr;

    public:
    __m128 SSE_container;

    SSE_vector():
        aligned_mem( (float*)_mm_malloc(sizeof(float)*4, 16) )
    {
        if (!aligned_mem)
        {
            printf("Failed to alloc memory\n");
        }

        WeHateMalloc();
    }

    SSE_vector(float filling_value):
        aligned_mem(nullptr)
    {
        SSE_container = _mm_set1_ps(filling_value);
    }

    ~SSE_vector()
    {
        _mm_free(aligned_mem);
    }

    SSE_vector(const SSE_vector& that) = delete;
    SSE_vector& operator= (const SSE_vector& that) = delete;

    __m128 load(float value_0, float value_1, float value_2, float value_3)
    {
        aligned_mem[0] = value_0;
        aligned_mem[1] = value_1;
        aligned_mem[2] = value_2;
        aligned_mem[3] = value_3;

        SSE_container = _mm_load_ps(aligned_mem);
        return SSE_container;
    }

    private:
    void WeHateMalloc() // aka CleanMemory
    {   
        printf("malloc hate\n");
        assert(aligned_mem);
        for (int n_component = 0; n_component < 4; n_component++) aligned_mem[n_component] = 0.0f;
    }    

};

struct SSEPoints
{
    SSE_vector ReZ_vector;
    SSE_vector ImZ_vector;
    SSE_vector N_vector;
    SSE_vector N_max;
    
    SSEPoints():
        N_max (N_MAX_VALUE)//,
        //ReZ_vector(),
        //ImZ_vector(),
        //N_vector()
        { }
    
};

void CalcFrac(unsigned char buffer[H][W][3], SSEPoints& points, float Re_position, float Im_position, float Current_scale);

int main()
{
    if (!glfwInit())
    {
        printf("glfwInit failed!\n");
        return -1;
    }
    
     GLFWwindow* window = glfwCreateWindow(W, H, "Hello World", NULL, NULL);
    
    if (!window)
    {
        printf("Failed to create window!\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    float Re_position   = Re_start;
    float Im_position   = Im_start;
    float Current_scale = Scale_koeff;

    unsigned char buffer[H][W][3] = {};

    float last_frame_time = glfwGetTime();
    float next_frame_time = 0.0f;
    float frame_rate      = 0.0f;

    printf("here\n");
    SSEPoints points;
    printf("here2\n");

    while (!glfwWindowShouldClose(window))
    {
        // Movement
        if (glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS){
            Im_position += MOVE_COEF * Current_scale;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS){
            Im_position -= MOVE_COEF * Current_scale;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
            Re_position += MOVE_COEF * Current_scale;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS){
            Re_position -= MOVE_COEF * Current_scale;
        }
        
        // Scale
        if (glfwGetKey(window, GLFW_KEY_Q ) == GLFW_PRESS){
            Current_scale += MOVE_COEF * (Current_scale/Scale_koeff);
        }
        if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
            Current_scale -= MOVE_COEF  * (Current_scale/Scale_koeff);
        }

        
        CalcFrac(buffer, points, Re_position, Im_position, Current_scale);

        glDrawPixels( W, H, GL_RGB, GL_UNSIGNED_BYTE, buffer );
        glfwSwapBuffers(window);
        glfwPollEvents();

        next_frame_time = glfwGetTime();
        frame_rate = 0.6f / (next_frame_time - last_frame_time);
        last_frame_time = next_frame_time;
        printf("%f\n", frame_rate);

        
    }

    glfwTerminate();
    return 0;
}

void Print__m128(__m128 vector)
{
    //vector = (float*)vector;
    for (int i = 0; i < 4; i++)
    {
        printf("%f ", vector[i]);
    }
    printf("\n");
}

void CalcFrac(unsigned char buffer[H][W][3], SSEPoints& points, float Re_position, float Im_position, float Current_scale)
{   

    glClearColor( 0, 0, 0, 1 );

        glClear( GL_COLOR_BUFFER_BIT );

        float ImZ = Im_position - Im_width * Current_scale; 

        float Im_step = Im_scale * Current_scale;

        float Re_step = Re_scale * Current_scale; 
        float x4_Re_step = Re_step * 4;

        __m128 N_max_vector = points.N_max.SSE_container;

        for ( size_t pixel_y = 0; pixel_y < H; pixel_y++, ImZ += Im_step )
        {
            float ReZ = Re_position - Re_width * Current_scale;

            for ( size_t pixel_x = 0; pixel_x < W; pixel_x+=4, ReZ += x4_Re_step )
            {   
                

                __m128 ReZ_arr = points.ReZ_vector.load(ReZ + Re_step * 0,
                                                        ReZ + Re_step * 1,
                                                        ReZ + Re_step * 2,
                                                        ReZ + Re_step * 3 );

                __m128 ImZ_arr = points.ImZ_vector.load(ImZ, ImZ, ImZ, ImZ);

                __m128 N       = points.N_vector.load(0, 0, 0, 0);                



                Print__m128(ReZ_arr);

                        
                        // Doing formula z^2 = (ReZ^2 - ImZ^2) + 2 ImZ ReZ i

                        __m128 ReZ_N_2     = _mm_mul_ps (ReZ_arr, ReZ_arr);
                        __m128 ImZ_N_2     = _mm_mul_ps (ImZ_arr, ImZ_arr);
                        __m128 ImZ_N_ReZ_N = _mm_mul_ps (ImZ_arr, ReZ_arr);
                        
                        __m128 abs_Z_N_2 = _mm_add_ps (ReZ_N_2, ImZ_N_2); 

                        
                        __m128 abs_cmp_mask = _mm_cmpge_ps (abs_Z_N_2, N_max_vector);
                        
                        /*
                        if (abs_Z_N_2 >= MAX_ABS)
                            break;
                        
                        ReZ_N = ReZ_N_2 - ImZ_N_2 + ReZ_arr[pPixelIndex];
                        ImZ_N = ImZ_N_ReZ_N + ImZ_N_ReZ_N  + ImZ;

                        //ImZ_N = 2 * ImZ_N * ReZ_N + ImZ; Works bad (draws dumb mandelbrot-like figure)
                        */       

                for (int pPixelIndex = 0; pPixelIndex < 4; pPixelIndex++)
                {
                    buffer[pixel_y][pixel_x + pPixelIndex][0] = 255; //N[pPixelIndex] % 64  * 4;
                    buffer[pixel_y][pixel_x + pPixelIndex][1] = 255; //N[pPixelIndex];
                    buffer[pixel_y][pixel_x + pPixelIndex][2] = 255; //N[pPixelIndex] % 128 * 2;
                }
            }
        }
}