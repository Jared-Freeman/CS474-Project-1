# CS474-Project-1

## Description

This repository implements Image Quantization and Histogram Specification image transformation to study their impacts on image quality. Only .PGM format files are supported.

Only instructions for Linux provided.

## Building

Q2 and Q4 may be build using Cmake, or the provided makefile.

### Build Using Cmake:

Create a build directory (recommended) in the project folder.

```bash
mkdir build
cd build/
cmake ..
make all
```

## Running

In the newly-created build directory:

```bash
./CS474 -image image_directory
```

Replace `image_directory` with the path to the desired .PGM image you wish to convert.
