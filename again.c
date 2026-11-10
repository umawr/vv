#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_library/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_library/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_library/stb_truetype.h"

int copy_file(long file_size, FILE *file_pointer);
long find_file_size(FILE *file_pointer);
int main()
{
  FILE *file_pointer = fopen("again.txt", "rb");
  long file_size = find_file_size(file_pointer);
  copy_file(file_size, file_pointer);
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
  long file_size = ftell(file_pointer);
  fseek(file_pointer, 0, SEEK_SET);
  printf("Size of .txt file in bytes is %ld\n", file_size);  
  return file_size;
}
int copy_file(long file_size, FILE *file_pointer)
{
  char *text_array = malloc(file_size + 1);
  if (text_array == NULL)
  {
    printf("Memory allocation failed for text\n");
    fclose(file_pointer);
    return EXIT_FAILURE;
  }
  size_t text_read = fread(text_array, 1, file_size, file_pointer);
  printf("fine");
  return 0;
}
