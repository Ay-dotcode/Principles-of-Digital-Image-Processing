#include "./image.h"

using namespace std;

int main() {
  ColorImage img;
  img.Load("lena.png");

  int w = img.GetWidth();
  int h = img.GetHeight();
  int britness = 10;

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      RGBA pixel = img(x, y);
      if (pixel.r == 0 && pixel.g == 0 && pixel.b == 0 || pixel.a == 0)
        continue;
      int r = car(pixel.r + britness, 255);
      int g = car(pixel.g + britness, 255);
      int b = car(pixel.b + britness, 255);
      img(x, y) = RGBA(r, g, b);
    }
  }

  img.Save("output.png");

  return 0;
}