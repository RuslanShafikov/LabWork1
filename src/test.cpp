#include <gtest/gtest.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include <sstream>
#include "../header/BMP.h"

std::string captureStdout(void (*func)(BMPHeader&), BMPHeader& header) {
    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    func(header);
    std::cout.rdbuf(oldCout);
    return oss.str();
}

std::string captureStdout(void (*func)(DIBHeader&), DIBHeader& header) {
    std::ostringstream oss;
    std::streambuf* oldCout = std::cout.rdbuf(oss.rdbuf());
    func(header);
    std::cout.rdbuf(oldCout);
    return oss.str();
}

TEST(ExpApproximationTest, HandlesZero) {
    EXPECT_NEAR(exp_approx(0.0), 1.0, 1e-5);
}

TEST(ExpApproximationTest, HandlesPositive) {
    EXPECT_NEAR(exp_approx(1.0), std::exp(1.0), 1e-5);
}

TEST(ExpApproximationTest, HandlesNegative) {
    EXPECT_NEAR(exp_approx(-1.0), std::exp(-1.0), 1e-5);
}

TEST(GaussianKernelTest, NormalizesCorrectly) {
    double sigma = 1.0;
    double kernel[7][7];
    generateGaussianKernel(sigma, kernel);

    double sum = 0.0;
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            sum += kernel[i][j];
        }
    }
    EXPECT_NEAR(sum, 1.0, 1e-6);
}

TEST(RotationTest, Rotate180MaintainsDimensions) {
    BMPHeader bmp;
    DIBHeader dib;
    int width = 2, height = 1;
    int32_t widthVal = width, heightVal = height;
    std::memcpy(dib.width, &widthVal, 4);
    std::memcpy(dib.height, &heightVal, 4);

    unsigned char** image = new unsigned char*[height];
    image[0] = new unsigned char[6]{10,20,30,40,50,60};

    std::ofstream outfile("test_rotate180.bmp", std::ios::binary);
    unsigned char additionalData[1] = {0};
    Rotate180(image, bmp, dib, outfile, 3, 0, additionalData, 0);
    outfile.close();

    int32_t newWidth, newHeight;
    std::memcpy(&newWidth, dib.width, 4);
    std::memcpy(&newHeight, dib.height, 4);

    EXPECT_EQ(newWidth, 2);
    EXPECT_EQ(newHeight, 1);

    delete[] image[0];
    delete[] image;
}

TEST(HeaderPrintTest, BMPHeaderOutput) {
    BMPHeader header;
    std::memcpy(header.header, "BM", 2);
    uint32_t fileSize = 100;
    std::memcpy(header.size, &fileSize, 4);

    std::string output = captureStdout(printBMPHeader, header);
    EXPECT_TRUE(output.find("File Size: 100") != std::string::npos);
}

TEST(HeaderPrintTest, DIBHeaderOutput) {
    DIBHeader header;
    int32_t width = 800;
    std::memcpy(header.width, &width, 4);

    std::string output = captureStdout(printDIBHeader, header);
    EXPECT_TRUE(output.find("Width: 800") != std::string::npos);
}

TEST(GaussianFilterTest, BlursSinglePixel) {
    const int width = 3, height = 3;
    unsigned char** image = new unsigned char*[height];
    for (int i = 0; i < height; ++i) {
        image[i] = new unsigned char[width * 3];
        memset(image[i], 0, width * 3);
    }

    image[1][3] = 255;
    image[1][4] = 255;
    image[1][5] = 255;

    double kernel[7][7];
    double sigma = 1.0;
    generateGaussianKernel(sigma, kernel);
    applyGaussianFilter(image, width, height, kernel);

    EXPECT_LT(image[1][3], 255);
    EXPECT_LT(image[1][4], 255);
    EXPECT_LT(image[1][5], 255);

    EXPECT_GT(image[0][3], 0);
    EXPECT_GT(image[1][0], 0);
    EXPECT_GT(image[2][5], 0);

    for (int i = 0; i < height; ++i) delete[] image[i];
    delete[] image;
}

TEST(GaussianFilterTest, HandlesImageBoundaries) {
    const int width = 2, height = 2;
    unsigned char** image = new unsigned char*[height];
    for (int i = 0; i < height; ++i) {
        image[i] = new unsigned char[width * 3];
        for (int j = 0; j < width * 3; j += 3) {
            image[i][j] = 100;
            image[i][j+1] = 150;
            image[i][j+2] = 200;
        }
    }

    double kernel[7][7];
    double sigma = 1.5;
    generateGaussianKernel(sigma, kernel);
    applyGaussianFilter(image, width, height, kernel);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width * 3; ++j) {
            EXPECT_GE(image[i][j], 0);
            EXPECT_LE(image[i][j], 255);
        }
    }

    for (int i = 0; i < height; ++i) delete[] image[i];
    delete[] image;
}

TEST(RotationTest, Rotate90Content) {
    BMPHeader bmp;
    DIBHeader dib;
    int width = 2, height = 1;
    int32_t widthVal = width, heightVal = height;
    std::memcpy(dib.width, &widthVal, 4);
    std::memcpy(dib.height, &heightVal, 4);

    unsigned char** image = new unsigned char*[height];
    image[0] = new unsigned char[6]{10,20,30,40,50,60}; // Pixel1: (10,20,30), Pixel2: (40,50,60)

    std::string filename = "test_rotate90.bmp";
    std::ofstream outfile(filename, std::ios::binary);
    unsigned char additionalData[1] = {0};
    Rotate90(image, bmp, dib, outfile, 3, 0, additionalData, 0);
    outfile.close();

    // Read back the rotated image
    std::ifstream infile(filename, std::ios::binary);
    infile.seekg(sizeof(BMPHeader) + sizeof(DIBHeader));

    unsigned char pixel1[3], pixel2[3];
    infile.read(reinterpret_cast<char*>(pixel1), 3);
    infile.read(reinterpret_cast<char*>(pixel2), 3);
    infile.close();

    // Verify rotation
    EXPECT_EQ(pixel1[0], 40); // Original pixel2 becomes first
    EXPECT_EQ(pixel1[1], 50);
    EXPECT_EQ(pixel1[2], 60);

    EXPECT_EQ(pixel2[0], 10); // Original pixel1 becomes second
    EXPECT_EQ(pixel2[1], 20);
    EXPECT_EQ(pixel2[2], 30);

    delete[] image[0];
    delete[] image;
}

TEST(RotationTest, Rotate270Content) {
    BMPHeader bmp;
    DIBHeader dib;
    int width = 2, height = 1;
    int32_t widthVal = width, heightVal = height;
    std::memcpy(dib.width, &widthVal, 4);
    std::memcpy(dib.height, &heightVal, 4);

    unsigned char** image = new unsigned char*[height];
    image[0] = new unsigned char[6]{10,20,30,40,50,60}; // Pixel1: (10,20,30), Pixel2: (40,50,60)

    std::string filename = "test_rotate270.bmp";
    std::ofstream outfile(filename, std::ios::binary);
    unsigned char additionalData[1] = {0};
    Rotate270(image, bmp, dib, outfile, 3, 0, additionalData, 0);
    outfile.close();

    std::ifstream infile(filename, std::ios::binary);
    infile.seekg(sizeof(BMPHeader) + sizeof(DIBHeader));

    unsigned char pixel1[3], pixel2[3];
    infile.read(reinterpret_cast<char*>(pixel1), 3);
    infile.read(reinterpret_cast<char*>(pixel2), 3);
    infile.close();

    EXPECT_EQ(pixel1[0], 10);
    EXPECT_EQ(pixel1[1], 20);
    EXPECT_EQ(pixel1[2], 30);

    EXPECT_EQ(pixel2[0], 40);
    EXPECT_EQ(pixel2[1], 50);
    EXPECT_EQ(pixel2[2], 60);

    delete[] image[0];
    delete[] image;
}

TEST(RotationTest, Rotate180Content) {
    BMPHeader bmp;
    DIBHeader dib;
    int width = 2, height = 1;
    int32_t widthVal = width, heightVal = height;
    std::memcpy(dib.width, &widthVal, 4);
    std::memcpy(dib.height, &heightVal, 4);

    unsigned char** image = new unsigned char*[height];
    image[0] = new unsigned char[6]{10,20,30,40,50,60};

    std::string filename = "test_rotate180.bmp";
    std::ofstream outfile(filename, std::ios::binary);
    unsigned char additionalData[1] = {0};
    Rotate180(image, bmp, dib, outfile, 3, 0, additionalData, 0);
    outfile.close();

    std::ifstream infile(filename, std::ios::binary);
    infile.seekg(sizeof(BMPHeader) + sizeof(DIBHeader));

    unsigned char pixel1[3], pixel2[3];
    infile.read(reinterpret_cast<char*>(pixel1), 3);
    infile.read(reinterpret_cast<char*>(pixel2), 3);
    infile.close();

    EXPECT_EQ(pixel1[0], 40);
    EXPECT_EQ(pixel1[1], 50);
    EXPECT_EQ(pixel1[2], 60);

    EXPECT_EQ(pixel2[0], 10);
    EXPECT_EQ(pixel2[1], 20);
    EXPECT_EQ(pixel2[2], 30);

    delete[] image[0];
    delete[] image;
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}