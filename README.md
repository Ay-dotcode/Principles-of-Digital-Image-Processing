# Principles of Digital Image Processing

A collection of C++ programs covering foundational digital image processing techniques, written as coursework exercises. All programs use a custom, header-only image library (`image.h`) built on top of **libpng**.

---

## Repository Structure

| File | Description |
|------|-------------|
| `image.h` | Custom header-only library providing `ColorImage`, `GrayscaleImage`, RGBA pixel types, histogram utilities, and helper functions. |
| `1.cpp` | Brightness adjustment on a colour image (pixel-level addition with clamping). |
| `week3.cpp` | Point operations on a grayscale image: brightness, contrast, gamma correction, inverse, and contrast stretching. |
| `nearest_neighbor_interpolation.cpp` | Image resizing using **nearest-neighbour** interpolation (colour image). |
| `bilinear_interpolation.cpp` | Image resizing using **bilinear** interpolation (grayscale image). |
| `lena.png` | Standard test image used as input for all programs. |
| `Makefile` | Build + run rules; compiles any `.cpp` file with a single `make` target. |

---

## Prerequisites

| Dependency | Purpose |
|------------|---------|
| `g++` (C++20) | Compiler |
| `libpng` | PNG read/write |

**Install libpng on Debian/Ubuntu:**
```bash
sudo apt install libpng-dev
```

**Install libpng on macOS (Homebrew):**
```bash
brew install libpng
```

---

## Building & Running

The `Makefile` provides a pattern rule that compiles **and** immediately runs any `.cpp` file:

```bash
# General form
make <target>       # target = cpp filename without extension

# Examples
make 1                                 # brightness adjustment
make week3                             # point operations
make nearest_neighbor_interpolation    # nearest-neighbour resize
make bilinear_interpolation            # bilinear resize

# Remove object/output files
make clean
```

> **Note:** The resize programs (`nearest_neighbor_interpolation`, `bilinear_interpolation`) prompt for a target width and height at runtime.

---

## Topics Covered

### Week 1 — Brightness Adjustment (`1.cpp`)
Loads `lena.png` as a colour image, adds a fixed brightness offset to every non-transparent pixel with clamping (0–255), and saves the result to `output.png`.

### Week 3 — Point Operations (`week3.cpp`)
Applies five classic point transformations to a grayscale version of `lena.png` and saves each result to `Images/`:

| Function | Formula | Output file |
|----------|---------|------------|
| Brightness | `O = I + b` | `Images/brightness.png` |
| Contrast | `O = c × (I − 128) + 128` | `Images/contrast.png` |
| Gamma correction | `O = 255 × (I/255)^γ` | `Images/gamma.png` |
| Inverse | `O = 255 − I` | `Images/inverse.png` |
| Contrast stretching | `O = (I − min) × 255 / (max − min)` | `Images/contrastStreching.png` |

### Spatial Interpolation
Both interpolation programs read `lena.png`, ask for a desired output resolution, resample the image, and save the result to `Images/`:

- **Nearest-neighbour** (`nearest_neighbor_interpolation.cpp`) — maps each output pixel to the closest input pixel.
- **Bilinear** (`bilinear_interpolation.cpp`) — maps each output pixel using a weighted average of the four surrounding input pixels.

---

## Custom Image Library (`image.h`)

The header provides two main classes:

```cpp
ColorImage     // Stores RGBA pixels (4 × Byte per pixel)
GrayscaleImage // Stores single-channel Byte pixels
```

Both expose a common interface:

```cpp
void Load(std::string filename);    // read a PNG
void Save(std::string filename);    // write a PNG
int  GetWidth()  const;
int  GetHeight() const;
T&   operator()(int x, int y);      // read/write pixel
T    Get(int x, int y) const;       // bounds-checked read (returns 0/transparent on OOB)
```

Additional utilities:

```cpp
SaveHist(image, filename, scale);   // save a pixel-intensity histogram as PNG
int car(float val, int limit);      // clamp-and-round helper (0 … limit)
```
