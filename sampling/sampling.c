#include<stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"
int new_width;
unsigned char * sampling_img = NULL;
FILE * file_pointer = NULL;
FILE *font_file = fopen("arial.ttf", "rb")
int tone;
int scaled_value;
int pixel_index = 0;
int line_counter = 0;
char ramp[] = {'@', 'W', '#', 'm', '=', '+', ';'};
int ramp_size = (sizeof(ramp)/sizeof(char));


void sampling_printer(float lumin)
{
    file_pointer = fopen("sample_out/sample_apple100x.txt", "a");
    if (line_counter == new_width)
    {
        fprintf(file_pointer, "\n");
        line_counter = 0;
    }
    for (int i = 0; i < ramp_size; i++)
    {
        if (lumin >= (i + 0) * 255/ramp_size && lumin <= (i + 1) * 255/ramp_size)
        {
            fprintf(file_pointer, "%c", ramp[i]);
            // printf("%c", ramp[i]);
        }
    }
    fclose(file_pointer);
}

void lumin_calc(int red_count, int green_count, int blue_count)
{
    float weight_red = 0.299, weight_green = 0.587, weight_blue = 0.114;
    float lumin = (red_count * weight_red + green_count * weight_green + blue_count * weight_blue) / 4;
    sampling_printer(lumin);
}

void color_parser(unsigned char * top_left_pixel, unsigned char * top_rigth_pixel,
                    unsigned char * bottom_left_pixel, unsigned char * bottom_right_pixel)
{
    unsigned char red, green, blue;
    int red_count = 0, green_count = 0, blue_count = 0;
    unsigned char *pixel_group[] = {top_left_pixel, top_rigth_pixel, bottom_left_pixel, bottom_right_pixel};
    for (int i = 0; i <= 3; i++)
    {
        red = pixel_group[i][0];
        red_count += red;
        green = pixel_group[i][1];
        green_count += green;
        blue = pixel_group[i][2];
        blue_count += blue;
    }
    lumin_calc(red_count, green_count, blue_count);
}

int main(void)
{
    int width, height, channels;
    unsigned char *data = stbi_load("sample_in/apple100x.jpg", &width, &height, &channels, 0);
    new_width = width / 2;
    int new_height = height / 2;
    size_t resize_img_size = new_width * new_height * channels;
    sampling_img = malloc(resize_img_size);
    printf("Loaded image of width %d pixels, height %d pixels & %d channels\n", width, height, channels);
    do
    {
        printf("Enter output tone (through 2 - 255)\n");
        scanf("%d", &tone);
        if(tone == 1)
            printf("Enter a valid choice\n");
    } while (tone == 1 || tone <= 0 );
    if (data == NULL)
    {
        printf("Error in loading image\n");
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < height - 1; y = y + 2)
    {
        for (int x = 0; x < width - 1; x = x + 2)
        {
            unsigned char * top_left = &data[(width * y + x) * channels];
            unsigned char * top_right = &data[(width * y + (x + 1)) * channels];
            unsigned char * bottom_left = &data[(width * (y + 1) + x) * channels];
            unsigned char * bottom_right = &data[(width * (y + 1) + (x + 1)) * channels];

            color_parser(top_left, top_right, bottom_left, bottom_right);
            line_counter += 1;
        }
    }
    int channels_new = 1;
    stbi_write_png("sample_out/apple_sampling_out.png", new_width, new_height, channels_new, sampling_img, new_width * channels_new);
    stbi_image_free(data);
    free(sampling_img);
    return 0;
}

// for (int i = 0; i < tone; i++)
// {
//     int upper_limit = (i == tone - 1) ? 255 : (i + 1) * 255 / tone;
//     int lower_limit = i * 255 / tone;
//     int match = (i == tone - 1) ? (lumin >= lower_limit && lumin <= upper_limit) : (lumin >= lower_limit && lumin < upper_limit);
//     if (match)
//     {
//         scaled_value =  i * 255 / (tone - 1);
//         resize_img[pixel_index + 0] = scaled_value;
//         resize_img[pixel_index + 1] = scaled_value;
//         resize_img[pixel_index + 2] = scaled_value;
//         break;
//     }
// }
// pixel_index = pixel_index + 3;

// 1st iteration, fewer details

// if (scaled_value == 0)
// {
//     fprintf(file_pointer, "%c", ramp[0]);
//     printf("%c", ramp[0]);
// }
// else
// {
//     fprintf(file_pointer, "%c", ramp[5]);
//     printf("%c", ramp[5]);
// }

// 2nd iteration, too much data in the code

// if (lumin >= 0 && lumin <= 36.428) // 36.428 is (i + 1) * 255/(sizeof(ramp)/sizeof(char))
// {
//     fprintf(file_pointer, "%c", ramp[0]);
//     printf("%c", ramp[0]);
// } 
// else if (lumin >= 36.428 && lumin <= 72.857)
// {
//     fprintf(file_pointer, "%c", ramp[1]);
//     printf("%c", ramp[1]);
// }
// else if (lumin >= 72.857 && lumin <= 109.28)
// {
//     fprintf(file_pointer, "%c", ramp[2]);
//     printf("%c", ramp[2]);

// }
// else if (lumin >= 109.28 && lumin <= 145.71)
// {
//     fprintf(file_pointer, "%c", ramp[3]);
//     printf("%c", ramp[3]);
// }
// else if (lumin >= 145.71 && lumin <= 182.14)
// {
//     fprintf(file_pointer, "%c", ramp[4]);
//     printf("%c", ramp[4]);
// }
// else if (lumin >= 182.14 && lumin <= 218.57)
// {
//     fprintf(file_pointer, "%c", ramp[5]);
//     printf("%c", ramp[5]);
// }
// else if (lumin >= 218.57 && lumin <= 255)
// {
//     fprintf(file_pointer, "%c", ramp[6]);
//     printf("%c", ramp[6]);
// }
