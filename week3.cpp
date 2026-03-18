#include "image.h"
#include <cmath>

using namespace std;

void brightness(const GrayscaleImage &I, double b = 0) {
  GrayscaleImage O(I.GetWidth(), I.GetHeight());

  for (int y = 0; y < I.GetHeight(); y++)
    for (int x = 0; x < I.GetWidth(); x++)
      O(x, y) = I.Get(x, y) + b;

  O.Save("Images/brightness.png");
}

void contrast(const GrayscaleImage &I, double c = 1) {
  GrayscaleImage O(I.GetWidth(), I.GetHeight());
  int midpoint = 128;

  for (int y = 0; y < I.GetHeight(); y++)
    for (int x = 0; x < I.GetWidth(); x++)
      O(x, y) = c * (I.Get(x, y) - midpoint) + midpoint;

  O.Save("Images/contrast.png");
}

void gamma(const GrayscaleImage &I, double g = 1) {
  GrayscaleImage O(I.GetWidth(), I.GetHeight());
  int pixels = pow(2, 8) - 1;

  for (int y = 0; y < I.GetHeight(); y++)
    for (int x = 0; x < I.GetWidth(); x++)
      O(x, y) = pixels * pow((double)I.Get(x, y) / pixels, g);

  O.Save("Images/gamma.png");
}

void inverse(const GrayscaleImage &I) {
  GrayscaleImage O(I.GetWidth(), I.GetHeight());
  int pixels = pow(2, 8) - 1;

  for (int y = 0; y < I.GetHeight(); y++)
    for (int x = 0; x < I.GetWidth(); x++)
      O(x, y) = pixels - I.Get(x, y);

  O.Save("Images/inverse.png");
}

void contrastStreching(const GrayscaleImage &I) {
  GrayscaleImage O(I.GetWidth(), I.GetHeight());
  int pixels = pow(2, 8) - 1;

  for (int y = 0; y < O.GetHeight(); y++)
    for (int x = 0; x < O.GetWidth(); x++)
      O(x, y) = (I.Get(x, y) - 0) * pixels / 255 - 0;

  O.Save("Images/contrastStreching.png");
}

int main() {
  GrayscaleImage I;
  I.Load("lena.png");
  brightness(I, -30.5);
  contrast(I, 1.2);
  gamma(I, 0.5);
  inverse(I);
  contrastStreching(I);

  return 0;
}