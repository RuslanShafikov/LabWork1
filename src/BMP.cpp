/* Shafikov Ruslan
   st112650@student.spbu.ru
   LabWork1
 */
#include "../header/BMP.h"
#include <chrono>
#include <iostream>
void printBMPHeader(BMPHeader& bmp) {
	std::cout << "BMP Header:\n";
	std::cout << "Signature: " << bmp.header[0] << bmp.header[1] << "\n";
	std::cout << "File Size: " << *reinterpret_cast<const uint32_t*>(bmp.size) << "\n";
	std::cout << "Reserved1: " << *reinterpret_cast<const uint16_t*>(bmp.reserved1) << "\n";
	std::cout << "Reserved2: " << *reinterpret_cast<const uint16_t*>(bmp.reserved2) << "\n";
	std::cout << "Data Offset: " << *reinterpret_cast<const uint32_t*>(bmp.offset) << "\n";
}

void printDIBHeader(DIBHeader& dib) {
	std::cout << "DIB Header:\n";
	std::cout << "Header Size: " << *reinterpret_cast<const uint32_t*>(dib.size) << "\n";
	std::cout << "Width: " << *reinterpret_cast<const int32_t*>(dib.width) << "\n";
	std::cout << "Height: " << *reinterpret_cast<const int32_t*>(dib.height) << "\n";
	std::cout << "Planes: " << *reinterpret_cast<const uint16_t*>(dib.planes) << "\n";
	std::cout << "Bits per Pixel: " << *reinterpret_cast<const uint16_t*>(dib.bitsPerPixel) << "\n";
	std::cout << "Compression: " << *reinterpret_cast<const uint32_t*>(dib.compression) << "\n";
	std::cout << "Image Size: " << *reinterpret_cast<const uint32_t*>(dib.sizeImage) << "\n";
	std::cout << "Horizontal Resolution: " << *reinterpret_cast<const int32_t*>(dib.horizontalResolution) << "\n";
	std::cout << "Vertical Resolution: " << *reinterpret_cast<const int32_t*>(dib.verticalResolution) << "\n";
	std::cout << "Colors Used: " << *reinterpret_cast<const uint32_t*>(dib.amountOfColors) << "\n";
	std::cout << "Important Colors: " << *reinterpret_cast<const uint32_t*>(dib.amountOfImportantColors) << "\n";
}

void Rotate270(unsigned char** array, BMPHeader& bmpHeader, DIBHeader& dibHeader, std::ofstream& outfile, const int& bytesPerPixel, const int& padding, unsigned char* additionalData, const int& additionalDataSize) {
	auto start = std::chrono::high_resolution_clock::now();
  	const int height = *reinterpret_cast<int*>(dibHeader.height);
	const int width = *reinterpret_cast<int*>(dibHeader.width);
	unsigned char** arrayCopy = new unsigned char* [width];
	for (int i = 0; i < width; ++i) {
		arrayCopy[i] = new unsigned char[height * bytesPerPixel + padding];
	}

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			arrayCopy[j][(height - 1 - i) * bytesPerPixel] = array[i][j * bytesPerPixel];
			arrayCopy[j][(height - 1 - i) * bytesPerPixel + 1] = array[i][j * bytesPerPixel + 1];
			arrayCopy[j][(height - 1 - i) * bytesPerPixel + 2] = array[i][j * bytesPerPixel + 2];
		}
	}

	int m = *reinterpret_cast<int*>(dibHeader.width);
	*reinterpret_cast<int*>(dibHeader.width) = height;
	*reinterpret_cast<int*>(dibHeader.height) = m;

	outfile.write(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
	outfile.write(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));
	outfile.write(reinterpret_cast<char*>(additionalData), additionalDataSize);
	for (int i = 0; i < width; ++i) {
		outfile.write(reinterpret_cast<char*>(arrayCopy[i]), height * bytesPerPixel + padding);
	}

	for (int j = 0; j < width; ++j) {
		delete[] arrayCopy[j];
	}
	delete[] arrayCopy;
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Rotate180 computation time: " << elapsed.count() << " seconds\n";
}



void Rotate90(unsigned char** array, BMPHeader& bmpHeader, DIBHeader& dibHeader, std::ofstream& outfile, const int& bytesPerPixel, const int& padding, unsigned char* additionalData, const int& additionalDataSize) {
	auto start = std::chrono::high_resolution_clock::now();
  	const int height = *reinterpret_cast<int*>(dibHeader.height);
	const int width = *reinterpret_cast<int*>(dibHeader.width);
	unsigned char** arrayCopy = new unsigned char* [width];
	for (int i = 0; i < width; ++i) {
		arrayCopy[i] = new unsigned char[height * bytesPerPixel + padding];
	}

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			arrayCopy[width - 1 - j][(i)*bytesPerPixel] = array[i][j * bytesPerPixel];
			arrayCopy[width - 1 - j][(i)*bytesPerPixel + 1] = array[i][j * bytesPerPixel + 1];
			arrayCopy[width - 1 - j][(i)*bytesPerPixel + 2] = array[i][j * bytesPerPixel + 2];
		}
	}

	int m = *reinterpret_cast<int*>(dibHeader.width);
	*reinterpret_cast<int*>(dibHeader.width) = height;
	*reinterpret_cast<int*>(dibHeader.height) = m;

	outfile.write(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
	outfile.write(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));
	outfile.write(reinterpret_cast<char*>(additionalData), additionalDataSize);
	for (int i = 0; i < width; ++i) {
		outfile.write(reinterpret_cast<char*>(arrayCopy[i]), height * bytesPerPixel + padding);
	}

	for (int j = 0; j < width; ++j) {
		delete[] arrayCopy[j];
	}
	delete[] arrayCopy;
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Rotate90 computation time: " << elapsed.count() << " seconds\n";
}


void Rotate180(unsigned char** array, BMPHeader& bmpHeader, DIBHeader& dibHeader, std::ofstream& outfile, const int& bytesPerPixel, const int& padding, unsigned char* additionalData, const int& additionalDataSize) {
	auto start = std::chrono::high_resolution_clock::now();
  	const int height = *reinterpret_cast<int*>(dibHeader.height);
	const int width = *reinterpret_cast<int*>(dibHeader.width);
	unsigned char** arrayCopy = new unsigned char* [height];
	for (int i = 0; i < height; ++i) {
		arrayCopy[i] = new unsigned char[width * bytesPerPixel + padding];
	}

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			arrayCopy[height - 1 - i][(width - j - 1) * bytesPerPixel] = array[i][j * bytesPerPixel];
			arrayCopy[height - 1 - i][(width - j - 1) * bytesPerPixel + 1] = array[i][j * bytesPerPixel + 1];
			arrayCopy[height - 1 - i][(width - j - 1) * bytesPerPixel + 2] = array[i][j * bytesPerPixel + 2];
		}
	}

	outfile.write(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
	outfile.write(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));
	outfile.write(reinterpret_cast<char*>(additionalData), additionalDataSize);
	for (int i = 0; i < height; ++i) {
		outfile.write(reinterpret_cast<char*>(arrayCopy[i]), width * bytesPerPixel + padding);
	}

	for (int j = 0; j < height; ++j) {
		delete[] arrayCopy[j];
	}
	delete[] arrayCopy;
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Rotate180 computation time: " << elapsed.count() << " seconds\n";
}

const double PI = 3.14159265358979323846;

void generateGaussianKernel(double& sigma, double kernel[7][7]) {

	double sum = 0.0;
	for (int i = -3; i <= 3; ++i) {
		for (int j = -3; j <= 3; ++j) {
			kernel[i + 3][j + 3] = (1.0 / (2 * PI * sigma * sigma)) * exp_approx(-(i * i + j * j) / (2 * sigma * sigma));
			sum += kernel[i + 3][j + 3];
		}
	}

	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 7; ++j) {
			kernel[i][j] /= sum;
		}
	}
}
void applyGaussianFilter(unsigned char**& image, const int width, const int height, double kernel[7][7]) {
	auto start = std::chrono::high_resolution_clock::now();
 	int kSize = 5;
	int half = kSize / 2;

	unsigned char** newImage = new unsigned char* [height];
	for (int i = 0; i < height; ++i) {
		newImage[i] = new unsigned char[width * 3];
	}

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			for (int c = 0; c < 3; ++c) {
				double sum = 0.0;

				for (int iBorder = -half; iBorder <= half; ++iBorder) {
					for (int jBorder = -half; jBorder <= half; ++jBorder) {
						int X = j + iBorder;
						int Y = i + jBorder;

						if (X >= 0 && X < width && Y >= 0 && Y < height) {
							sum += (double)(image[Y][X * 3 + c]) * kernel[iBorder + half][jBorder + half];
						}
					}
				}
				sum = std::max(0.0, std::min(255.0, sum));
				newImage[i][j * 3 + c] = static_cast<unsigned char>(sum);
			}
		}
	}
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width * 3; ++j) {
			image[i][j] = newImage[i][j];
		}
	}

	for (int i = 0; i < height; ++i) {
		delete[] newImage[i];
	}
	delete[] newImage;
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Gauss computation time: " << elapsed.count() << " seconds\n";
}

double exp_approx(double x) {
	double result = 1.0;
	double term = 1.0;
	int n = 1;
	const double epsilon = 1e-10;


	while (true) {
		term *= x / n;
		result += term;


		if (std::abs(term) < epsilon) {
			break;
		}

		++n;
	}

	return result;
}

void saveFile(const int additionalDataSize, unsigned char* additionalData, unsigned char** rgbArray, unsigned char* metaData, const int metaDataSize, BMPHeader& bmpHeader, DIBHeader& dibHeader, std::ofstream& outfile) {
	const int width = *reinterpret_cast<int32_t*>(dibHeader.width);
	const int height = *reinterpret_cast<int32_t*>(dibHeader.height);
	const int bytesPerPixel = *reinterpret_cast<int32_t*>(dibHeader.bitsPerPixel) / 8;
	const int padding = (4 - (width * bytesPerPixel) % 4) % 4;
	const int rowSize = width * bytesPerPixel + padding;
	outfile.write(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
	outfile.write(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));
	outfile.write(reinterpret_cast<char*>(additionalData), additionalDataSize);
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < rowSize; ++j) {
			outfile.put(rgbArray[i][j]);
		}

		outfile.write("\0\0\0\0", padding);
	}

	outfile.write(reinterpret_cast<char*>(metaData), metaDataSize);
}