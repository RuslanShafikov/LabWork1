# BMP Image Processing Library

## Overview
C++ library for BMP image manipulation featuring:
- 90°/180°/270° rotations
- Gaussian blur filtering
- Header-preserving transformations

## Key Features
- **Efficient Processing**: Uses OpenMP for parallelization
- **Precise Filters**: 7x7 Gaussian kernel with configurable sigma
- **Format Compliance**: Maintains all BMP header data
- **Custom Math**: Includes optimized exp() approximation

## Technical Details
- **Language**: C++17
- **Dependencies**: None (Standard Library only)
- **Parallelism**: OpenMP directives
- **File Support**: 24-bit uncompressed BMP