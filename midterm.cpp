#include "./image.h"

using namespace std;

void Halve(GrayscaleImage &I) {
  int w = I.GetWidth();
  int h = I.GetHeight();
  GrayscaleImage O(w / 2, h / 2);

  for (int y = 0; y < h; y += 2) {
    for (int x = 0; x < w; x += 2) {
      int sum = 0;
      sum += I(x, y) + I(x + 1, y) + I(x, y + 1) + I(x + 1, y + 1);
      O(x / 2, y / 2) = sum / 4;
    }
  }
  O.Save("Images/midterm.png");
}
void Halve2(GrayscaleImage &I) {
  int w = I.GetWidth() / 2;
  int h = I.GetHeight() / 2;
  GrayscaleImage O(w, h);

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int sum = 0;
      sum += I((2 * x), (2 * y)) + I((2 * x) + 1, (2 * y));
      sum += I((2 * x), (2 * y) + 1) + I((2 * x) + 1, (2 * y) + 1);
      O(x / 2, y / 2) = sum / 4;
    }
  }
  O.Save("Images/midterm.png");
}

int main() {
  GrayscaleImage I;
  I.Load("lena.png");
  Halve2(I);
  return 0;
}