# PPM Image Processing Toolkit

A high-performance C++ command-line tool for processing PPM (P3) format images with various image enhancement operations including sharpening, blurring, and edge detection.

## 🖼️ Project Overview

This project implements a robust image processing toolkit specifically designed for PPM (Portable Pixmap) P3 format images. The tool provides various image manipulation operations with performance optimization and batch processing capabilities.

## ✨ Features

- **Image Sharpening**: Enhanced sharpening algorithms for image clarity
- **Blur Operations**: Multiple blur filters (Gaussian, Box blur)
- **Edge Detection**: Sobel and Prewitt edge detection algorithms
- **Batch Processing**: Process multiple images with automated scripts
- **Performance Analysis**: Timing logs and performance benchmarking
- **Format Support**: Native PPM (P3) format reading/writing
- **Memory Optimization**: Efficient memory management for large images

## 🛠️ Technologies Used

- **Programming Language**: C++ 17
- **Build System**: Makefile
- **Scripting**: Bash for batch processing
- **Libraries**: Standard Template Library (STL)
- **Tools**: GCC/G++ Compiler, Git
- 
# Sharpening an image
./image_tool --input input_image.ppm --output output_image.ppm --sharpen

# Applying Gaussian blur
./image_tool --input input_image.ppm --output output_image.ppm --blur gaussian

# Edge detection
./image_tool --input input_image.ppm --output output_image.ppm --edge sobel
# Process all images in a directory
./scripts/batch_process.sh --input-dir ./images/input --output-dir ./images/output --operation sharpen

# Performance benchmarking
./scripts/benchmark.sh --images ./test_images --iterations 10
