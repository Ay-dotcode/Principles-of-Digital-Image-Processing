#include "image.h"
#include <iostream>

using namespace std;

int main() {
  GrayscaleImage I, O;
  I.Load("lena.png");
  I.Save("Images/grayscale.png");
  int Iw = I.GetWidth(), Ih = I.GetHeight();
  int Ow = O.GetWidth(), Oh = O.GetHeight();

  cout << "Current image size: w " << Iw << ", h " << Ih << endl;
  cout << "What size do you want to resize to? \nw h: ";
  cin >> Ow >> Oh;

  O = GrayscaleImage(Ow, Oh);
  float x_ratio = (float)Iw / Ow, y_ratio = (float)Ih / Oh;
  float new_x, new_y, fx, fy, x1, x2;
  Byte color;

  for (int y = 0; y < Oh; y++) {
    for (int x = 0; x < Ow; x++) {
      new_x = x_ratio * x;
      new_y = y_ratio * y;

      fx = new_x - floor(new_x);
      fy = new_y - floor(new_y);

      new_x = floor(new_x);
      new_y = floor(new_y);

      x1 = I(new_x, new_y) * (1 - fx) + I(car(new_x + 1, Iw), new_y) * fx;

      x2 = I(new_x, car(new_y + 1, Ih)) * (1 - fx) +
           I(new_x + 1, car(new_y + 1, Ih)) * fx;
           
      color = x1 * (1 - fy) + x2 * (fy);

      O(x, y) = color;
    }
  }

  O.Save("Images/bilinear.png");

  return 0;
}