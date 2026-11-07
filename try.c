#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_library/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_library/stb_image_write.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_library/stb_truetype.h"

int main(void)
{
  FILE *read_char = fopen("sample_out/out.txt", "rb"); //open txt file
  if (read_char == NULL) // fallback statement
  {
    printf("Error in reading .txt file\n");
    return EXIT_FAILURE;
  }

  fseek(read_char, 0, SEEK_END); // point read_char to the end the .txt file
  long number_of_char = ftell(read_char); // store the last value of the file in number_of_char
  rewind(read_char); // point read_char to the begining of the file
  printf("Number of characters: %ld\n", number_of_char); // print number_of_char in .txt file

  char *text_array = malloc(number_of_char + 1); // allocate memory to store the txt file in heap that + 1 is for the \0 char
  if (text_array == NULL) // fallback statement
  {
    printf("Memory allocation failed for text\n");
    fclose(read_char);
    return EXIT_FAILURE;
  }

  size_t text_read = fread(text_array, sizeof(char), number_of_char, read_char); //text_read tells the no. of bytes read from the file
  text_array[text_read] = '\0'; // last element of text_array is \0
  fclose(read_char); // close the txt file

  // printf("%s", text_array);// print the text in the .txt file

  FILE *font_file = fopen("arialfont.ttf", "rb"); // open the .ttf file
  if (!font_file) // fallback statement
  {
    printf("Could not open the font file\n");
    free(text_array);
    return EXIT_FAILURE;
  }

  fseek(font_file, 0, SEEK_END); // go to end of font_file
  long font_size = ftell(font_file); // saves the current (which is the last) position of font_file to font_size
  fseek(font_file, 0, SEEK_SET); // resets the position of font_file to the start
  printf("\nsize of arialfont.ttf in bytes %ld\n", font_size); // print size of .ttf file

  unsigned char *font_data = malloc(font_size); // allocate memory (size of font_file / value of font_size) for the .ttf file in heap
  if (font_data == NULL) // fallback statement
  {
    printf("Memory allocation failed for font\n");
    fclose(font_file);
    free(text_array);
    return EXIT_FAILURE;
  }

  size_t got = fread(font_data, sizeof(char), font_size, font_file); // got tells the size of font_file read
  fclose(font_file); // close the font_file
  printf("bytes read: %zu\n", got); // bytes of the font_file

  if (got != (size_t)font_size) // fallback statement
  {
    printf("Short read on font file\n");
    free(font_data);
    free(text_array);
    return EXIT_FAILURE;
  }

  stbtt_fontinfo font; // font is a struct that stores info about the font idk
  int offset = stbtt_GetFontOffsetForIndex(font_data, 0); // searches the font data for a font bcz some .ttf files have multiple variations of the same font
  int ok = stbtt_InitFont(&font, font_data, offset); // initializes font structure
  printf("InitFont: %d\n", ok); 

  if (!ok) // fallback statement
  {
    free(font_data);
    free(text_array);
    return EXIT_FAILURE;
  }

  int ascent, descent, lineGap;
  stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap); // store the variables
  printf("%d %d %d\n", ascent, descent, lineGap);

  float scale = stbtt_ScaleForPixelHeight(&font, 32.0f); // scale to 32px for better readability
  int line_height = (int)((ascent - descent + lineGap) * scale); // some formula for line height
  printf("line_height at 32px: %d\n", line_height);
  
  int width = 1000, height = 1000; // dimensions of the canvas
  unsigned char *canvas = malloc(width * height); // allocate memory for the canvas
  if (canvas == NULL)
  {
    printf("Memory allocation failed for canvas\n");
    free(font_data);
    free(text_array);
    return EXIT_FAILURE;
  }
  
  memset(canvas, 255, width * height); // set all the bytes of the canvas array to 255 (white background)

  int from_left = 20;
  int from_top = 40;
  
  int x = from_left; // current horizontal position like (20, 0)
  int baseline = from_top + (int)(ascent * scale); // baseline calculation idk ???
  
  for (int i = 0; i < number_of_char; i++) // processes every char in the txt
  {
    int ch = (unsigned char)text_array[i]; // gets the first element from the text_array
  
    // if (ch == '\r') continue;

    if (ch == '\n')
    {
      x = from_left;// at the left position (20, 0)
      baseline += line_height;// switch to a newline
      continue;
    }

    // glyph is the "design" of the character, like the letter "u" might means the same but has a different "design" in various font styles
    // bitmap is the array of bits that represent that particular character's glyph 
    // offset is a particular variant of font ...ig
  
    int x_topleft, y_topleft, x_bottomright, y_bottomright;
    stbtt_GetCodepointBitmapBox(&font, ch, scale, scale, &x_topleft, &y_topleft, &x_bottomright, &y_bottomright);// calculates the rectangular boundary of a particular character, only tells the dimensions and offsets of the bitmap
    // &font initialized font | ch character to render | scale horizontal scaling | scale vertical scaling | &x_topleft left boundary output | &y_topleft top boundary output | &x_bottomright right boundary output | &y_bottomright bottom boundary output |
    int glyph_width = x_bottomright - x_topleft;// width of the glyph
    int glyph_height = y_bottomright - y_topleft;// height of the glyph
    
      if (glyph_width > 0 && glyph_height > 0)
    {
      unsigned char *glyph = malloc(glyph_width * glyph_height);// area of the glyph
      if (glyph)
      {
        stbtt_MakeCodepointBitmap(&font, glyph, glyph_width, glyph_height, glyph_width, scale, scale, ch);
        // font info | memory where bitmap pixels will be stored | bitmap width | bitmap height | row stride (no. of bytes to move)?? | horizontal scale | vertical scale | character to render 
        for (int gy = 0; gy < glyph_height; gy++) // processes every row of a character's bitmap
        {
          for (int gx = 0; gx < glyph_width; gx++) // processes every pixel in that row
          {
            int cx = x + x_topleft + gx;
            int cy = baseline + y_topleft + gy;
            if (cx < 0 || cx >= width || cy < 0 || cy >= height) continue;
            
            // Alpha blending for black text on a white canvas
            unsigned char g = glyph[gy * glyph_width + gx];
            unsigned char existing = canvas[cy * width + cx];
            canvas[cy * width + cx] = (255 - g) * g / 255 + existing * (255 - g) / 255;
          }
        }
        free(glyph);
      }
    }
  
    int advance, lsb;
    stbtt_GetCodepointHMetrics(&font, ch, &advance, &lsb);
    x += (int)(advance * scale);
  }
  
  // after the loop (done once)
  stbi_write_png("output3.png", width, height, 1, canvas, width);
  printf("Successfully generated output3.png!\n");

  // free heap memory
  free(canvas);
  free(font_data);
  free(text_array);
  return 0;
}
