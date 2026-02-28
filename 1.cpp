#include "./image.h"

using namespace std;

int main() {
  ColorImage img;
  img.Load("input.png");

  int w = img.GetWidth();
  int h = img.GetHeight();

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      RGBA pixel = img(x, y);
      img(x, y) = RGBA(255 - pixel.r, 255 - pixel.g, 255 - pixel.b, pixel.a);
    }
  }

  img.Save("output.png");

  return 0;
}