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
  int minVal, maxVal;
  minVal = maxVal = I.Get(0, 0);
  for (int y = 0; y < O.GetHeight(); y++)
    for (int x = 0; x < O.GetWidth(); x++) {
      maxVal = I.Get(x, y) > maxVal ? I.Get(x, y) : maxVal;
      minVal = I.Get(x, y) < minVal ? I.Get(x, y) : minVal;
    }

  for (int y = 0; y < O.GetHeight(); y++)
    for (int x = 0; x < O.GetWidth(); x++)
      O(x, y) = (I.Get(x, y) - minVal) * pixels / (maxVal - minVal);

  O.Save("Images/contrastStreching.png");
}

void histeq(const GrayscaleImage &I) {
  GrayscaleImage O(I.GetWidth(), I.GetHeight());
  int highest = 255;
  int pixels = I.GetWidth() * I.GetHeight();
  int count[256] = {};
  double pdf[256], cdf[256];
  int rep[256];

  for (int y = 0; y < O.GetHeight(); y++)
    for (int x = 0; x < O.GetWidth(); x++)
      count[I.Get(x, y)]++;

  pdf[0] = (double)count[0] / (pixels);
  cdf[0] = pdf[0];
  rep[0] = round(cdf[0] * highest);
  for (int i = 1; i <= highest; i++) {
    pdf[i] = (double)count[i] / (pixels);
    cdf[i] = cdf[i - 1] + pdf[i];
    rep[i] = round(cdf[i] * highest);
  }

  for (int y = 0; y < O.GetHeight(); y++)
    for (int x = 0; x < O.GetWidth(); x++)
      O(x, y) = rep[I.Get(x, y)];

  O.Save("Images/histeq.png");
}

int main() {
  GrayscaleImage I;
  I.Load("lena.png");
  
  brightness(I, -30.5);
  contrast(I, 1.2);
  gamma(I, 0.5);
  inverse(I);
  contrastStreching(I);
  histeq(I);

  return 0;
}