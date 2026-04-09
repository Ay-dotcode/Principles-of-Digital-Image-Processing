#include "./image.h"
#include <algorithm>
#include <cmath>
#include <queue>
#include <vector>

using namespace std;

// ── Point
// ─────────────────────────────────────────────────────────────────────

struct Point {
  int x, y;
  Point(int x = 0, int y = 0) : x(x), y(y) {}
};

// ── AA Line
// ───────────────────────────────────────────────────────────────────

static inline double fracPart(double v) { return v - floor(v); }
static inline double invFracPart(double v) { return 1.0 - fracPart(v); }

static inline void plotAA(GrayscaleImage &img, int x, int y, Byte color,
                          double coverage) {
  if (x < 0 || y < 0 || x >= img.GetWidth() || y >= img.GetHeight())
    return;
  double t = max(0.0, min(1.0, coverage));
  img(x, y) = static_cast<Byte>(round(img(x, y) * (1.0 - t) + color * t));
}

static void drawLine(GrayscaleImage &img, int x1, int y1, int x2, int y2,
                     Byte color = 255) {
  double x0 = x1, y0 = y1, xe = x2, ye = y2;
  bool steep = abs(ye - y0) > abs(xe - x0);
  if (steep) {
    swap(x0, y0);
    swap(xe, ye);
  }
  if (x0 > xe) {
    swap(x0, xe);
    swap(y0, ye);
  }
  double dx = xe - x0, dy = ye - y0;
  if (dx == 0.0) {
    int xi = (int)round(x0);
    for (int yi = (int)floor(min(y0, ye)); yi <= (int)ceil(max(y0, ye)); yi++)
      steep ? plotAA(img, yi, xi, color, 1.0) : plotAA(img, xi, yi, color, 1.0);
    return;
  }
  double grad = dy / dx;
  double xe1 = round(x0), ye1 = y0 + grad * (xe1 - x0),
         xg1 = invFracPart(x0 + 0.5);
  int xp1 = (int)xe1, yp1 = (int)floor(ye1);
  if (steep) {
    plotAA(img, yp1, xp1, color, invFracPart(ye1) * xg1);
    plotAA(img, yp1 + 1, xp1, color, fracPart(ye1) * xg1);
  } else {
    plotAA(img, xp1, yp1, color, invFracPart(ye1) * xg1);
    plotAA(img, xp1, yp1 + 1, color, fracPart(ye1) * xg1);
  }
  double intery = ye1 + grad;
  double xe2 = round(xe), ye2 = ye + grad * (xe2 - xe),
         xg2 = fracPart(xe + 0.5);
  int xp2 = (int)xe2, yp2 = (int)floor(ye2);
  if (steep) {
    plotAA(img, yp2, xp2, color, invFracPart(ye2) * xg2);
    plotAA(img, yp2 + 1, xp2, color, fracPart(ye2) * xg2);
  } else {
    plotAA(img, xp2, yp2, color, invFracPart(ye2) * xg2);
    plotAA(img, xp2, yp2 + 1, color, fracPart(ye2) * xg2);
  }
  for (int x = xp1 + 1; x < xp2; x++) {
    int y = (int)floor(intery);
    if (steep) {
      plotAA(img, y, x, color, invFracPart(intery));
      plotAA(img, y + 1, x, color, fracPart(intery));
    } else {
      plotAA(img, x, y, color, invFracPart(intery));
      plotAA(img, x, y + 1, color, fracPart(intery));
    }
    intery += grad;
  }
}

// ── Edge detection → binary map
// ───────────────────────────────────────────────

static vector<vector<bool>> sobelMask(GrayscaleImage &I, int threshold = 128) {
  int w = I.GetWidth(), h = I.GetHeight();
  vector<vector<bool>> mask(h, vector<bool>(w, false));
  for (int y = 1; y < h - 1; y++) {
    for (int x = 1; x < w - 1; x++) {
      int gx = -I(x - 1, y - 1) + I(x + 1, y - 1) - 2 * I(x - 1, y) +
               2 * I(x + 1, y) - I(x - 1, y + 1) + I(x + 1, y + 1);
      int gy = -I(x - 1, y - 1) - 2 * I(x, y - 1) - I(x + 1, y - 1) +
               I(x - 1, y + 1) + 2 * I(x, y + 1) + I(x + 1, y + 1);
      int mag = min(255, (int)sqrt((double)(gx * gx + gy * gy)));
      mask[y][x] = (mag >= threshold);
    }
  }
  return mask;
}

// ── Connected components via BFS (8-connectivity)
// ───────────────────────────── Returns one vector<Point> per contour — no
// cross-image jumps possible.

vector<vector<Point>> findContours(GrayscaleImage &I, int threshold = 128,
                                   int minContourSize = 10) {
  int w = I.GetWidth(), h = I.GetHeight();
  auto mask = sobelMask(I, threshold);

  vector<vector<bool>> visited(h, vector<bool>(w, false));
  vector<vector<Point>> contours;

  const int dx8[] = {-1, 0, 1, -1, 1, -1, 0, 1};
  const int dy8[] = {-1, -1, -1, 0, 0, 1, 1, 1};

  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      if (!mask[y][x] || visited[y][x])
        continue;

      // BFS flood-fill this connected edge region
      vector<Point> contour;
      queue<Point> q;
      q.push({x, y});
      visited[y][x] = true;

      while (!q.empty()) {
        Point p = q.front();
        q.pop();
        contour.push_back(p);

        for (int d = 0; d < 8; d++) {
          int nx = p.x + dx8[d];
          int ny = p.y + dy8[d];
          if (nx < 0 || ny < 0 || nx >= w || ny >= h)
            continue;
          if (!mask[ny][nx] || visited[ny][nx])
            continue;
          visited[ny][nx] = true;
          q.push({nx, ny});
        }
      }

      if ((int)contour.size() >= minContourSize)
        contours.push_back(std::move(contour));
    }
  }
  return contours;
}

// ── Render
// ────────────────────────────────────────────────────────────────────

void drawContours(GrayscaleImage &img, const vector<vector<Point>> &contours,
                  Byte color = 255) {
  for (const auto &contour : contours)
    for (const auto &p : contour)
      if (p.x >= 0 && p.x < img.GetWidth() && p.y >= 0 && p.y < img.GetHeight())
        img(p.x, p.y) = color;
}
// ── Main
// ──────────────────────────────────────────────────────────────────────

int main() {
  GrayscaleImage I;
  I.Load("lena.png");
  I.Save("Images/edges.png");


  // Each element is one connected edge chain — do whatever you want with them
  vector<vector<Point>> contours = findContours(I, 128, 10);

  GrayscaleImage out(I.GetWidth(), I.GetHeight());
  drawContours(out, contours);
  out.Save("Images/edges_poly.png");

  return 0;
}