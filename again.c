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
     stbtt_InitFont(&font, font_data, stbtt_GetFontOffsetForIndex(font_data, 0));
     // stbtt_fontinfo is a custom struct (data type)

     // Generate bitmap of character A
     int width, height, x_offset, y_offset;

     unsigned char *bitmap = stbtt_GetCodepointBitmap(
         &font,
         0,
         stbtt_ScaleForPixelHeight(&font, 200),
         'A',
         &width,
         &height,
         &x_offset,
         &y_offset
     );

     // Save bitmap as PNG
     stbi_write_png("letter_A.png", width, height, 1, bitmap, width);

     // Cleanup
     stbtt_FreeBitmap(bitmap, NULL);
     free(font_data);
     printf("Saved letter_A.png (%d x %d)\n", width, height);
  return 0;
}
