#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "helpers.h"
//#include "bmp.h"
// Convert image to grayscale

int calculate_sepia_red(int red, int green, int blue);
int calculate_sepia_green(int red, int green, int blue);
int calculate_sepia_blue(int red, int green, int blue);
int calculate_color_blur(int i, int j, int height, int width, RGBTRIPLE picture[height][width], char color);

int calculate_edges_pixel_value(float gx, float gy);
float calculate_g(int i, int j, int height, int width, RGBTRIPLE picture[height][width], char color, char x_y);

void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double average = ((float)image[i][j].rgbtBlue + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtRed) / 3;

            image[i][j].rgbtBlue = (int) round(average);
            image[i][j].rgbtGreen = (int) round(average);
            image[i][j].rgbtRed = (int) round(average);
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE aux[height][width];   //auxiliar array
    
    for (int i = 0; i < height; i++)
    {   
        for (int j = 0; j < width; j++)
        {
            aux[i][j] = image[i][j];
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = calculate_sepia_blue(aux[i][j].rgbtRed, aux[i][j].rgbtGreen, aux[i][j].rgbtBlue);
            image[i][j].rgbtGreen = calculate_sepia_green(aux[i][j].rgbtRed, aux[i][j].rgbtGreen, aux[i][j].rgbtBlue);
            image[i][j].rgbtRed = calculate_sepia_red(aux[i][j].rgbtRed, aux[i][j].rgbtGreen, aux[i][j].rgbtBlue);
        }
    }
            
}

int calculate_sepia_red(int red, int green, int blue) // calculates red sepia value
{
    float sepiaRed = (0.393 * red) + (0.769 * green) + (0.189 * blue);
    
    if (sepiaRed > 255)
    {
        sepiaRed = 255;
    }
 
    return (int) round(sepiaRed);
    
}
int calculate_sepia_green(int red, int green, int blue) // calculates green sepia value
{
    float sepiaGreen = (0.349 * red) + (0.686 * green) + (0.168 * blue);
    
    if (sepiaGreen > 255)
    {
        sepiaGreen = 255;
    }
 
    return (int) round(sepiaGreen);
  
}
int calculate_sepia_blue(int red, int green, int blue) // calculates blue sepia value
{
    float sepiaBlue = (0.272 * red) + (0.534 * green) + (0.131 * blue);
    
    if (sepiaBlue > 255)
    {
        sepiaBlue = 255;
    }
 
    return (int) round(sepiaBlue);
    
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE aux[height][width]; //auxiliar array
    
    for (int i = 0; i < height; i++)
    {   
        for (int j = 0; j < width; j++)
        {
            aux[i][j] = image[i][j];
        }
    }
    
    for (int i = 0; i < height; i++)
    {   
        int k = (width - 1);
        
        for (int j = 0; j < width; j++)
        {
            image[i][j] = aux[i][k];
            k--;
        }
    }
        
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE aux[height][width]; //auxiliar array
    
    for (int i = 0; i < height; i++)
    {   
        for (int j = 0; j < width; j++)
        {
            aux[i][j] = image[i][j];
        }
    }
    
   
    for (int i = 0; i < height; i++)
    {   
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = calculate_color_blur(i, j, height, width, aux, 'b');
            image[i][j].rgbtGreen = calculate_color_blur(i, j, height, width, aux, 'g');
            image[i][j].rgbtRed = calculate_color_blur(i, j, height, width, aux, 'r');
        }
    }
}

int calculate_color_blur(int i, int j, int height, int width, RGBTRIPLE picture[height][width],
                         char color) //calculates color average for blur
{
    float average = 0;
    float sum = 0;
    int pixel_counter = 0;
    int i_max = i;
    int j_max = j;
    
    if (i > 0) // define minimum range value for i
    {
        i--;
    }
    
    if (j > 0) // define minimum range value for j
    {
        j--;
    }
    
    if (i_max < (height - 1)) //define maximum range value for i
    {
        i_max++;
    }
    
    if (j_max < (width - 1)) //define maximum range value for j
    {
        j_max++;
    }
    
    for (int k = i; k <= i_max; k++)
    {
        for (int m = j; m <= j_max; m++)
        {
            if (color == 'b')
            {
                sum += picture[k][m].rgbtBlue;  //blue sum
                pixel_counter ++;
            }
            if (color == 'g')
            {
                sum += picture[k][m].rgbtGreen;  //green sum
                pixel_counter ++;
            }
            if (color == 'r')
            {
                sum += picture[k][m].rgbtRed;  //red sum
                pixel_counter ++;
            }
        }
    }
    
    average = sum / pixel_counter;
    
    return (int) round(average);
    
}


void edges(int height, int width, RGBTRIPLE image[height][width])
{
    float gx_red, gx_green, gx_blue = 0;
    float gy_red, gy_green, gy_blue = 0;
    
    RGBTRIPLE aux[height][width]; //auxiliar array
    
    for (int i = 0; i < height; i++)
    {   
        for (int j = 0; j < width; j++)
        {
            aux[i][j] = image[i][j];
        }
    }
    
    
    for (int i = 0; i < height; i++)
    {   
        for (int j = 0; j < width; j++)
        {
            gx_red = calculate_g(i, j, height, width, aux, 'r', 'x');
            gx_green = calculate_g(i, j, height, width, aux, 'g', 'x');
            gx_blue = calculate_g(i, j, height, width, aux, 'b', 'x');
            
            gy_red = calculate_g(i, j, height, width, aux, 'r', 'y');
            gy_green = calculate_g(i, j, height, width, aux, 'g', 'y');
            gy_blue = calculate_g(i, j, height, width, aux, 'b', 'y');
            
            image[i][j].rgbtRed = calculate_edges_pixel_value(gx_red, gy_red);
            image[i][j].rgbtGreen = calculate_edges_pixel_value(gx_green, gy_green);
            image[i][j].rgbtBlue = calculate_edges_pixel_value(gx_blue, gy_blue);
        }
    }
}

float calculate_g(int i, int j, int height, int width, RGBTRIPLE picture[height][width], char color, char x_y)
{
    
    float gx_times_block[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    float gy_times_block[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    float (*g_times_block)[3];
    
    float sum = 0;
    
    int k_block = 0;
    int m_block = 0;
    int m_block_start = 0;
    int i_max = i;
    int j_max = j;
    
    if (i == 0)                                                                             // define minimum for i if its in i edge and k start
    {
        k_block++;
    }
    else
    {
        i--;
    }

    if (j == 0)                                                                             // define minimum for j if its in j edge and m start
    {
        m_block_start++;
    }
    else
    {
        j--;
    }
    
    if (i_max < (height - 1))                                                               // define maximum for i if its in i edge
    {
        i_max++;
    }
    
    if (j_max < (width - 1))                                                                // define maximum for j if its in j edge
    {
        j_max++;
    }
    
    if (x_y == 'x')                                                  // identify if its gy or gx and points to correct block array
    {
        g_times_block = gx_times_block;
    }
    else
    {
        g_times_block = gy_times_block;
    }
    
    
    for (int x = i; x <= i_max; x++)
    {
        m_block = m_block_start;
        
        for (int y = j; y <= j_max; y++)                                                    // select color and calculates its pixels series
        {
            if (color == 'r')
            {
                sum += (float)picture[x][y].rgbtRed * g_times_block[k_block][m_block];
            }
            if (color == 'g')
            {
                sum += (float)picture[x][y].rgbtGreen * g_times_block[k_block][m_block];
            }
            if (color == 'b')
            {
                sum += (float)picture[x][y].rgbtBlue * g_times_block[k_block][m_block];
            }
   
            m_block++;
        }

        k_block++;
                    
    }
                
    
    return sum;
}

int calculate_edges_pixel_value(float gx, float gy)                         // calculates formula for edges filter pixel value
{
    float result = sqrt((float)pow(gx, 2) + (float)pow(gy, 2));
    
    if (result > 255)
    {
        result = 255;
    }
    
    return (int) round(result);
}