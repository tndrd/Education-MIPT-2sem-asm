#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


const unsigned int W = 800;
const unsigned int H = 400;

const float Re_start = 0;
const float Im_start = 0;

const float Re_width = 2;
const float Im_width = 1;

const float Scale_koeff = 1.0f;
const float Re_scale = 2 * Re_width / W * Scale_koeff;
const float Im_scale = 2 * Im_width / H * Scale_koeff;

const size_t N_max = 256;
const float MAX_ABS = 100;

const float MOVE_COEF = 0.05f;

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

    while (!glfwWindowShouldClose(window))
    {
        // Movement
        if (glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS){
            Im_position += MOVE_COEF * Current_scale;
            printf("here\n");
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

        glClearColor( 0, 0, 0, 1 );

        glClear( GL_COLOR_BUFFER_BIT );

        float ImZ = Im_position - Im_width * Current_scale; 

        float Im_step = Im_scale * Current_scale;

        for ( size_t pixel_y = 0; pixel_y < H; pixel_y++, ImZ += Im_step )
        {
            float ReZ = Re_position - Re_width * Current_scale;

            float Re_step = Re_scale * Current_scale;

            for ( size_t pixel_x = 0; pixel_x < W; pixel_x++, ReZ += Re_step )
            {   

                size_t N = 0;                
                float ReZ_N = ReZ;
                float ImZ_N = ImZ;

                for(; N < N_max; N++)
                    {
                        
                        // Doing formula z^2 = (ReZ^2 - ImZ^2) + 2 ImZ ReZ i

                        float ReZ_N_2     = ReZ_N * ReZ_N;
                        float ImZ_N_2     = ImZ_N * ImZ_N;
                        float ImZ_N_ReZ_N = ImZ_N * ReZ_N;

                        float abs_Z_N_2 = ReZ_N_2 + ImZ_N_2; 

                        if (abs_Z_N_2 >= MAX_ABS)
                            break;
                        
                        ReZ_N = ReZ_N_2 - ImZ_N_2 + ReZ;
                        ImZ_N = ImZ_N_ReZ_N + ImZ_N_ReZ_N  + ImZ;

                        //ImZ_N = 2 * ImZ_N * ReZ_N + ImZ; Works bad (draws dumb mandelbrot-like figure)
                        
                    }

                buffer[pixel_y][pixel_x][0] = N % 64 * 2;
                buffer[pixel_y][pixel_x][1] = N;
                buffer[pixel_y][pixel_x][2] = N % 64 * 2;
            }
        }

        glDrawPixels( W, H, GL_RGB, GL_UNSIGNED_BYTE, buffer );
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}