#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_library/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_library/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_library/stb_truetype.h"

int read_ttf();
int copy_file(long text_file_size, FILE *file_pointer);
long find_file_size(FILE *file_pointer);

int main()
{
  FILE *file_pointer = fopen("again.txt", "rb");
  long text_file_size = find_file_size(file_pointer);
  copy_file(text_file_size, file_pointer);
  read_ttf();
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
  fseek(file_pointer, 0, SEEK_SET);
  printf("Size of .txt file in bytes is %ld\n", text_file_size);  
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

int read_ttf()
{
  FILE *font_file = fopen("arialfont.ttf", "rb");
  if (!font_file)
  {
    printf("Could not open the font file\n");
    return EXIT_FAILURE;
  } 
  fseek(font_file, 0, SEEK_END);
  long font_file_size = ftell(font_file);
  fseek(font_file, 0, SEEK_SET);
  printf("size of arialfont.ttf in bytes %ld\n", font_file_size);
  unsigned char *font_data = malloc(font_file_size);
  if (font_data == NULL)
  {
    printf("Memory allocation failed for font\n");
    fclose(font_file);
    return EXIT_FAILURE;
  }
  size_t got = fread(font_data, sizeof(char), font_file_size, font_file);
  // fread needs the buffer font_data to already exist in RAM
  fclose(font_file); 

}
