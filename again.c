#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_library/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_library/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_library/stb_truetype.h"

int main()
{
  FILE *file_pointer = fopen("again.txt", "rb");
  if (file_pointer == NULL) // fallback statement
  {
    printf("Error in reading .txt file\n");
    return EXIT_FAILURE;
  }
  fseek(file_pointer, 0, SEEK_END);
  long file_size = ftell(file_pointer);
  fseek(file_pointer, 0, SEEK_SET);
  printf("%ld", file_size);
  return 0;
}