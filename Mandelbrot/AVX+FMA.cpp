#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <x86intrin.h>
#include <xmmintrin.h>
#include <GLFW/glfw3.h>
#include "mandelbrot.h"

const __m256 REZ_COEFFS = _mm256_set_ps(7, 6, 5, 4, 3, 2, 1, 0);
const __m256 abs_max    =_mm256_set1_ps ( MAX_ABS );

void CalcFrac(unsigned char buffer[H][W][3], float Re_position, float Im_position, float Current_scale);

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
    
    float Re_position   = RE_START;
    float Im_position   = IM_START;
    float Current_scale = INITIAL_SCALE;

    unsigned char buffer[H][W][3] = {};

    float last_frame_time = glfwGetTime();
    float next_frame_time = 0.0f;
    float frame_rate      = 0.0f;

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
            Current_scale += MOVE_COEF * (Current_scale/INITIAL_SCALE);
        }
        if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
            Current_scale -= MOVE_COEF  * (Current_scale/INITIAL_SCALE);
        }

        
        CalcFrac(buffer, Re_position, Im_position, Current_scale);

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


void CalcFrac(unsigned char buffer[H][W][3], float Re_position, float Im_position, float Current_scale)
{   

    glClearColor( 0, 0, 0, 1 );

    glClear( GL_COLOR_BUFFER_BIT );

    float ImZ        = Im_position - IM_WIDTH * Current_scale; 
    float Im_step    = INITIAL_IM_STEP * Current_scale;
    float Re_step    = INITIAL_RE_STEP * Current_scale; 
    float x8_Re_step = Re_step * 8;

    for ( size_t pixel_y = 0; pixel_y < H; pixel_y++, ImZ += Im_step )
    {
        float ReZ    = Re_position - RE_WIDTH * Current_scale;
        __m256 ImZ_0 = _mm256_set1_ps ( ImZ );
        
        for ( size_t pixel_x = 0; pixel_x < W; pixel_x += 8, ReZ += x8_Re_step )
        {   
            __m256 ReZ_0   = _mm256_fmadd_ps (_mm256_set1_ps(Re_step), REZ_COEFFS, _mm256_set1_ps( ReZ ));
            __m256 ReZ_arr = ReZ_0;

            __m256 ImZ_arr =  ImZ_0;
            __m256i N      =  _mm256_setzero_si256();                
            
            int iteration = 0;

            do {
                __m256 ReZ_N_2     = _mm256_mul_ps ( ReZ_arr, ReZ_arr );
                __m256 ImZ_N_2     = _mm256_mul_ps ( ImZ_arr, ImZ_arr );
                __m256 ImZ_N_ReZ_N = _mm256_mul_ps ( ImZ_arr, ReZ_arr ); 
                
                // Doing formula z^2 = (ReZ^2 - ImZ^2) + 2 ImZ ReZ i

                __m256 abs_Z_N_2 = _mm256_add_ps (ReZ_N_2, ImZ_N_2); 

                __m256 abs_cmp_mask = _mm256_cmp_ps (abs_Z_N_2, abs_max, _CMP_LE_OS);

                if (!_mm256_movemask_ps (abs_cmp_mask) ) break;
                
                N = _mm256_sub_epi32(N, _mm256_castps_si256 (abs_cmp_mask));
                
                ReZ_arr = _mm256_add_ps (_mm256_sub_ps (ReZ_N_2, ImZ_N_2), ReZ_0);
                ImZ_arr = _mm256_add_ps (_mm256_add_ps (ImZ_N_ReZ_N, ImZ_N_ReZ_N), ImZ_0);

                iteration++;
            } while (iteration < N_MAX_VALUE);
            
            int32_t* N_iterations = (int32_t*)(&N);

            for (int pPixelIndex = 0; pPixelIndex < 8; pPixelIndex++)
            {
                buffer[pixel_y][pixel_x + pPixelIndex][0] = N_iterations[pPixelIndex] % 64  * 4;
                buffer[pixel_y][pixel_x + pPixelIndex][1] = N_iterations[pPixelIndex];
                buffer[pixel_y][pixel_x + pPixelIndex][2] = N_iterations[pPixelIndex] % 128 * 2;
            }
        }
    }
}