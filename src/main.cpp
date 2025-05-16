/* Shafikov Ruslan
   st112650@student.spbu.ru
   LabWork1
 */
#include "../header/BMP.h"

BMPHeader bmpHeader{};
DIBHeader dibHeader{};

int main() {
    BMPHeader bmpHeader{};
    DIBHeader dibHeader{};
    
    std::string fileInput;
    while (1)
    {
        std::string validFiles[] = { "Airplane.bmp", "sample1.bmp", "sample2.bmp", "sample3.bmp", "sample4.bmp" };

        std::cout << "Please, choose an input file from Airplane.bmp, sample1.bmp, sample2.bmp, sample3.bmp, sample4.bmp" << std::endl;
        std::cout << "Or else the program will stop" << std::endl;
        std::cin >> fileInput;
        bool isValidFile = false;

        for (const auto& file : validFiles) {
            if (fileInput == file) {
                isValidFile = true;
            }
        }
        if (!isValidFile) {
            return -1;
        }
        int input;
        std::cout << "Enter 1 if you want rotate your file by 90 degrees" << std::endl;
        std::cout << "Enter 2 if you want rotate your file by 180 degrees" << std::endl;
        std::cout << "Enter 3 if you want rotate your file by 270 degrees" << std::endl;
        std::cout << "Enter 4 if you want to use Gauss filter" << std::endl;
        std::cout << "Enter 5 if you want to save file in outfile" << std::endl;
        std::cout << "if you enter anything else the program will stop" << std::endl;
        std::cin >> input;

       
            if (input > 5 || input <= 0) {
                std::cout<< "Please start again and enter a command from 1 to 5" << std::endl;
                return -1;
            }
            std::ifstream infile(fileInput, std::ios::binary);
            std::ofstream outfile("output1.bmp", std::ios::binary);

            if (!infile) {
                std::cerr << "Error in opening input file" << std::endl;
                return 1;
            }
            if (!outfile) {
                std::cerr << "Error in opening output file" << std::endl;
                return 1;
            }

            infile.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
            infile.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));
            const int width = *reinterpret_cast<int32_t*>(dibHeader.width);
            const int height = *reinterpret_cast<int32_t*>(dibHeader.height);
            const int offset = *reinterpret_cast<int32_t*>(bmpHeader.offset);
            const int headerSize = *reinterpret_cast<int32_t*>(dibHeader.size);
            const int bytesPerPixel = *reinterpret_cast<int32_t*>(dibHeader.bitsPerPixel) / 8;
            const int padding = (4 - (width * bytesPerPixel) % 4) % 4;
            const int rowSize = width * bytesPerPixel + padding;
            const int additionalDataSize = headerSize - 40;
            const int fileSize = *reinterpret_cast<int32_t*>(bmpHeader.size);
            unsigned char* additionalData = new unsigned char[additionalDataSize];
            if (additionalDataSize > 0) {
                infile.read(reinterpret_cast<char*>(additionalData), additionalDataSize);
            }
            infile.seekg(offset, std::ios::beg);

            unsigned char** rgbArray = new unsigned char* [height];
            for (int i = 0; i < height; ++i) {
                rgbArray[i] = new unsigned char[rowSize];
            }

            unsigned char ch = 0;
            int row = 0;
            while (row < height) {
                for (int i = 0; i < width * bytesPerPixel; ++i) {
                    infile.get(reinterpret_cast<char&>(ch));
                    if (ch == EOF) {
                        break;
                    }
                    rgbArray[row][i] = ch;
                }
                if (ch == EOF) {
                    break;
                }
                ++row;
            }


            const int metaDataSize = fileSize - rowSize * height - 54 - additionalDataSize;
            unsigned char* metaData = new unsigned char[metaDataSize];
            if (metaDataSize > 0) {
                infile.read(reinterpret_cast<char*>(metaData), metaDataSize);
            }
            if (input == 4) {
                double kernel[7][7]{ 0.0 };
                double sigma = 0.0;
                std::cout << "Please, enter sigma" << std::endl;
                std::cin >> sigma;
                generateGaussianKernel(sigma, kernel);
                applyGaussianFilter(rgbArray, width, height, kernel);
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

                for (int i = 0; i < height; ++i) {
                    delete[] rgbArray[i];
                }

                delete[] rgbArray;
                delete[] metaData;
                delete[] additionalData;
            }

            else if (input == 3) {
                Rotate270(rgbArray, bmpHeader, dibHeader, outfile, bytesPerPixel, padding, additionalData, additionalDataSize);
                for (int i = 0; i < height; ++i) {
                    delete[] rgbArray[i];
                }


                outfile.write(reinterpret_cast<char*>(metaData), metaDataSize);
                delete[] rgbArray;
                delete[] additionalData;
                delete[] metaData;
            }

            else if (input == 2) {
                Rotate180(rgbArray, bmpHeader, dibHeader, outfile, bytesPerPixel, padding, additionalData, additionalDataSize);
                for (int i = 0; i < height; ++i) {
                    delete[] rgbArray[i];
                }

                outfile.write(reinterpret_cast<char*>(metaData), metaDataSize);
                delete[] rgbArray;
                delete[] additionalData;
                delete[] metaData;
            }

            else if (input == 1) {
                Rotate90(rgbArray, bmpHeader, dibHeader, outfile, bytesPerPixel, padding, additionalData, additionalDataSize);
                for (int i = 0; i < height; ++i) {
                    delete[] rgbArray[i];
                }
                outfile.write(reinterpret_cast<char*>(metaData), metaDataSize);
                delete[] rgbArray;
                delete[] additionalData;
                delete[] metaData;
            }

            else if (input==5) {
                saveFile(additionalDataSize, additionalData, rgbArray, metaData, metaDataSize, bmpHeader, dibHeader, outfile);

                for (int i = 0; i < height; ++i) {
                    delete[] rgbArray[i];
                }
                delete[] rgbArray;
                delete[] metaData;
                delete[] additionalData;
            }

            else {
                break;
            }
            infile.close();
            outfile.close();
        
       
    }
    return 0;
}
