#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_library/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_library/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_library/stb_truetype.h"

int print_img(unsigned char *font_data);
unsigned char * read_ttf(FILE *font_file);
int copy_file(long text_file_size, FILE *file_pointer);
long find_file_size(FILE *file_pointer);

int main()
{
  FILE *file_pointer = fopen("again.txt", "rb");
  long text_file_size = find_file_size(file_pointer);
  copy_file(text_file_size, file_pointer);
  FILE *font_file = fopen("arialfont.ttf", "rb");
  unsigned char * font_data = read_ttf(font_file);
  print_img(font_data);
  return 0;
}

long find_file_size(FILE *file_pointer)
{
  if (file_pointer == NULL)
  {
    printf("Error in reading .txt file\n");
    return EXIT_FAILURE;
  }
  fseek(file_pointer, 0, SEEK_END);
  long text_file_size = ftell(file_pointer);
  rewind(file_pointer);
  return text_file_size;
}

int copy_file(long text_file_size, FILE *file_pointer)
{
  char *text_array = malloc(text_file_size + 1);
  if (text_array == NULL)
  {
    printf("Memory allocation failed for text\n");
    fclose(file_pointer);
    return EXIT_FAILURE;
  }
  size_t text_read = fread(text_array, sizeof(char), text_file_size, file_pointer);
  fclose(file_pointer);
  return 0;
}

unsigned char * read_ttf(FILE *font_file)
{
  if (!font_file)
  {
    printf("Could not open the font file\n");
    return NULL;
  }
  fseek(font_file, 0, SEEK_END);
  long font_file_size = ftell(font_file);
  rewind(font_file);
  printf("size of arialfont.ttf in bytes %ld\n", font_file_size);
  unsigned char *font_data = malloc(font_file_size);
  if (font_data == NULL)
  {
    printf("Memory allocation failed for font\n");
    fclose(font_file);
    return NULL;
  }
  size_t got = fread(font_data, sizeof(char), font_file_size, font_file);
  // fread needs the buffer font_data to already exist in RAM when it rads the .ttf file into  memory
  fclose(font_file);
  return font_data;
}

int print_img(unsigned char *font_data)
{
    stbtt_fontinfo font;
    // overhere it defines font as a struct (data type), "stbtt_fontinfo" is a custom struct data type
    stbtt_InitFont(&font, font_data, stbtt_GetFontOffsetForIndex(font_data, 0));
    //(address of the empty font info struct, pointer to the font file, finds the offset of the font (font file pointer, 0th (1st) font of that font file))

    char rack[] = "ozymandias";
    int n = sizeof(rack) / sizeof(char);
    printf("%d\n", n);
    
    int canvas_width  = 900;                 // change this later to something dynamic
    int canvas_height = 300;
    int cell_w = canvas_width / n;
    int cell_h = canvas_height;
    
    unsigned char *canvas = malloc(canvas_width * canvas_height);
    for (int i = 0; i < canvas_width * canvas_height; i++)
        canvas[i] = 255;
    
    // find scale 
    float scale = 1e9f;
    int min_y0 = 1e9, max_y1 = -1e9;
    printf("writing %d x %d\n", canvas_width, canvas_height);
    for (int g = 0; g < n; g++)
    {
        int x0, y0, x1, y1;
        stbtt_GetCodepointBox(&font, rack[g], &x0, &y0, &x1, &y1);
    
        float sx = (cell_w * 0.9f) / (x1 - x0);  // scale at which this letter's width fills the cell (x1 - x0) is glyph width
        float sy = (cell_h * 0.9f) / (y1 - y0);  // scale at which this letter's height fills the cell (x1 - x0) is glyph height
        float s  = (sx < sy ? sx : sy);
        if (s < scale) scale = s;
    
        if (y0 < min_y0) min_y0 = y0;
        if (y1 > max_y1) max_y1 = y1;
    }
    
    // 2. Put the shared baseline so the tallest/lowest letters are centered vertically
    float total_h   = (max_y1 - min_y0) * scale;
    int   baseline  = (int)((canvas_height - total_h) / 2 + max_y1 * scale);
    
    // 3. Render each letter and copy it into its own cell, centered horizontally
    for (int g = 0; g < n; g++)
    {
        int glyph_w, glyph_h, x_offset, y_offset;
        unsigned char *bitmap = stbtt_GetCodepointBitmap(
            &font, 0, scale, rack[g], &glyph_w, &glyph_h, &x_offset, &y_offset);
    
        int start_x = g * cell_w + (cell_w - glyph_w) / 2;
        int start_y = baseline + y_offset;
    
        for (int y = 0; y < glyph_h; y++)
            for (int x = 0; x < glyph_w; x++)
            {
                int cx = start_x + x;
                int cy = start_y + y;
                if (cx < 0 || cx >= canvas_width || cy < 0 || cy >= canvas_height)
                    continue;
                canvas[cy * canvas_width + cx] = 255 - bitmap[y * glyph_w + x];
            }
    
        stbtt_FreeBitmap(bitmap, NULL);
    }
    
    if (!stbi_write_png("output.png", canvas_width, canvas_height, 1, canvas, canvas_width))
        printf("PNG write FAILED\n");
    else
        printf("PNG write was successful\n");
    
    stbi_write_png("outp1ut.png", canvas_width, canvas_height, 1, canvas, canvas_width);
    // free(canvas);    printf("%d\n", *(bitmap + 10000));
     
    // stbtt_FreeBitmap(bitmap, NULL);
    free(font_data);
    printf("Saved letter_A.png (%d x %d)\n", canvas_width, canvas_height);
    return 0;
}
