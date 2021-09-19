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

## Running Q2

In the newly-created build directory:

```bash
./CS474 -in image_directory -out output_directory
```

- Replace `image_directory` with the path to the desired .PGM image you wish to convert. Multiple input images may be specified.

- Replace `output_directory` with the desired output directory. New files will be automatically named, and placed there. Only the first output directory will be considered as of now. Specifying no output will place the files in the directory with the program.

