#include "./image.h"
#include <cmath>
#include <iostream>

using namespace std;

int main() {
  ColorImage input, output;
  input.Load("lena.png");

  int input_w = input.GetWidth();
  int input_h = input.GetHeight();

  cout << "Current image size: w " << input_w << ", h " << input_h << endl;
  int output_w, output_h;
  cout << "What size do you want to resize to? \nw: ";
  cin >> output_w;
  cout << "h: ";
  cin >> output_h;

  output = ColorImage(output_w, output_h);
  double x_ratio = static_cast<double>(input_w) / output_w;
  double y_ratio = static_cast<double>(input_h) / output_h;

  for (int y = 0; y < output_h; y++) {
    for (int x = 0; x < output_w; x++) {
      output(x, y) = input.Get(round(x * x_ratio), round(y * y_ratio));
    }
  }

  output.Save("nearest_neigbor.png");

  return 0;
}