/* Shafikov Ruslan
   st112650@student.spbu.ru
   LabWork1
 */

#ifndef structH
#define structH
#include <fstream>
#include <iostream>
#include <string>
#include <cstdint>
struct BMPHeader {
	char header[2];
	char size[4];
	char reserved1[2];
	char reserved2[2];
	char offset[4];

	~BMPHeader() {

	}
};

struct DIBHeader {
	char size[4];
	char width[4];
	char height[4];
	char planes[2];
	char bitsPerPixel[2];
	char compression[4];
	char sizeImage[4];
	char horizontalResolution[4];
	char verticalResolution[4];
	char amountOfColors[4];
	char amountOfImportantColors[4];

	~DIBHeader() {

	}
};

void printBMPHeader(BMPHeader& bmp);

void printDIBHeader(DIBHeader& dib);

void Rotate270(unsigned char** array, BMPHeader& bmpHeader, DIBHeader& dibHeader, std::ofstream& outfile, const int& bytesPerPixel, const int& padding, unsigned char* additionalData, const int& additionalDataSize);



void Rotate90(unsigned char** array, BMPHeader& bmpHeader, DIBHeader& dibHeader, std::ofstream& outfile, const int& bytesPerPixel, const int& padding, unsigned char* additionalData, const int& additionalDataSize);


void Rotate180(unsigned char** array, BMPHeader& bmpHeader, DIBHeader& dibHeader, std::ofstream& outfile, const int& bytesPerPixel, const int& padding, unsigned char* additionalData, const int& additionalDataSize);
void generateGaussianKernel(double& sigma, double kernel[7][7]);
void applyGaussianFilter(unsigned char**& image, const int width, const int height, double kernel[7][7]);
double exp_approx(double x);
void saveFile(const int additionalDataSize, unsigned char* additionalData, unsigned char** rgbArray, unsigned char* metaData, const int metaDataSize, BMPHeader& bmpHeader, DIBHeader& dibHeader, std::ofstream& outfile);

#endif
