#include "image.h"

using namespace std;

void britness(const GrayscaleImage &I, double b = 0) {
  GrayscaleImage O(I.GetWidth(), I.GetHeight());

  for (int y = 0; y < I.GetHeight(); y++)
    for (int x = 0; x < I.GetWidth(); x++)
      O(x, y) = I.Get(x, y) + b;

  O.Save("britness.png");
}
void contrast(const GrayscaleImage &I, double c = 1) {
  GrayscaleImage O(I.GetWidth(), I.GetHeight());
  int midpoint = 128;

  for (int y = 0; y < I.GetHeight(); y++)
    for (int x = 0; x < I.GetWidth(); x++)
      O(x, y) = c * (I.Get(x, y) - midpoint) + midpoint;

  O.Save("contrast.png");
}

int main() {
  GrayscaleImage I;
  I.Load("lena.png");
  britness(I, -30.5);
  contrast(I, 1.2);

  return 0;
}