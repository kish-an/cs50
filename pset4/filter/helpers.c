#include <math.h>
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Set pointer to individual pixel RGB struct (helps shorten code below)
            RGBTRIPLE *pixel = &image[i][j];
            // Cant use *pixel.rgbtBlue... need to use (*pixel).rgbtBlue
            int average = round(((*pixel).rgbtBlue + (*pixel).rgbtGreen + (*pixel).rgbtRed) / 3.0);
            (*pixel).rgbtBlue = (int) average;
            (*pixel).rgbtGreen = (int) average;
            (*pixel).rgbtRed = (int) average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE *pixel = &image[i][j];
            int sepiaRed = round(0.189 * (*pixel).rgbtBlue + 0.769 * (*pixel).rgbtGreen + 0.393 * (*pixel).rgbtRed);
            int sepiaGreen = round(0.168 * (*pixel).rgbtBlue + 0.686 * (*pixel).rgbtGreen + 0.349 * (*pixel).rgbtRed);
            int sepiaBlue = round(0.131 * (*pixel).rgbtBlue + 0.534 * (*pixel).rgbtGreen + 0.272 * (*pixel).rgbtRed);

            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            (*pixel).rgbtBlue = sepiaBlue;
            (*pixel).rgbtGreen = sepiaGreen;
            (*pixel).rgbtRed = sepiaRed;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        // Reverse width array
        int n = width;
        int end = width - 1;

        for (int j = 0; j < n / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][end];
            image[i][end] = tmp;
            end--;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // // Make a copy of the image
    RGBTRIPLE pixel[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixel[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        // For every pixel...
        for (int j = 0; j < width; j++)
        {
            int counter = 0, red = 0, green = 0, blue = 0;
            // Current pixel
            red += pixel[i][j].rgbtRed;
            green += pixel[i][j].rgbtGreen;
            blue += pixel[i][j].rgbtBlue;
            counter++;

            // Check if pixel above and to the left exists
            if (i - 1 >= 0 && j - 1 >= 0)
            {
                red += pixel[i - 1][j - 1].rgbtRed;
                green += pixel[i - 1][j - 1].rgbtGreen;
                blue += pixel[i - 1][j - 1].rgbtBlue;
                counter++;
            }
            // Pixel directly above
            if (i - 1 >= 0 && j >= 0)
            {
                red += pixel[i - 1][j].rgbtRed;
                green += pixel[i - 1][j].rgbtGreen;
                blue += pixel[i - 1][j].rgbtBlue;
                counter++;
            }
            // Pixel above and to the right
            if (i - 1 >= 0 && j + 1 < width)
            {
                red += pixel[i - 1][j + 1].rgbtRed;
                green += pixel[i - 1][j + 1].rgbtGreen;
                blue += pixel[i - 1][j + 1].rgbtBlue;
                counter++;
            }
            // Pixel to the left
            if (j - 1 >= 0)
            {
                red += pixel[i][j - 1].rgbtRed;
                green += pixel[i][j - 1].rgbtGreen;
                blue += pixel[i][j - 1].rgbtBlue;
                counter++;
            }
            //Pixel to the right
            if (j + 1 < width)
            {
                red += pixel[i][j + 1].rgbtRed;
                green += pixel[i][j + 1].rgbtGreen;
                blue += pixel[i][j + 1].rgbtBlue;
                counter++;
            }
            // Pixel below and to the left
            if (i + 1 < height && j - 1 >= 0)
            {
                red += pixel[i + 1][j - 1].rgbtRed;
                green += pixel[i + 1][j - 1].rgbtGreen;
                blue += pixel[i + 1][j - 1].rgbtBlue;
                counter++;
            }
            // Pixel directly below
            if (i + 1 < height && j >= 0)
            {
                red += pixel[i + 1][j].rgbtRed;
                green += pixel[i + 1][j].rgbtGreen;
                blue += pixel[i + 1][j].rgbtBlue;
                counter++;
            }
            // Pixel below and to the right
            if (i + 1 < height && j + 1 < width)
            {
                red += pixel[i + 1][j + 1].rgbtRed;
                green += pixel[i + 1][j + 1].rgbtGreen;
                blue += pixel[i + 1][j + 1].rgbtBlue;
                counter++;
            }

            image[i][j].rgbtRed = round(red / (counter * 1.0));
            image[i][j].rgbtGreen = round(green / (counter * 1.0));
            image[i][j].rgbtBlue = round(blue / (counter * 1.0));
        }
    }
    return;
}
