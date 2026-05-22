#include "image.h"

using namespace std;

void segment(const ColorImage &img) {
  double s[img.GetWidth()][img.GetHeight()];
  double segment = 0;
  GrayscaleImage out(img.GetWidth(), img.GetHeight());

  for (int y = 0; y < img.GetHeight(); y++) {
    for (int x = 0; x < img.GetWidth(); x++) {
      RGBA pixel = img(x, y);
      double avg = double(pixel.r + pixel.g + pixel.b) / 3;

      int max = pixel.r;
      int min = pixel.r;
      max = max > pixel.g ? max : pixel.g;
      max = max > pixel.b ? max : pixel.b;
      min = min < pixel.g ? min : pixel.g;
      min = min < pixel.b ? min : pixel.b;

      if (avg == 0)
        continue;
      s[x][y] = (max - min) / avg;
      segment += s[x][y];
    }
  }

  segment = segment / (img.GetHeight() * img.GetWidth());

  out = img;
  for (int y = 0; y < img.GetHeight(); y++)
    for (int x = 0; x < img.GetWidth(); x++)
      out(x, y) = segment > s[x][y] ? 255 : 0;
  out.Save("Images/final.png");
}

int main() {
  ColorImage img;
  img.Load("lena.png");
  segment(img);
  return 0;
}