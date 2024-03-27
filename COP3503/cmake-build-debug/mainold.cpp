#include <iostream>
#include <fstream>
#include <vector>
#include "ImageData.h"
using namespace std;

unsigned char CharOutput(ifstream& readFile){
    char fileItem;
    readFile.read((char*) &fileItem, 1);

    return fileItem;
}

short ShortOutput(ifstream& readFile){
    short fileItem;
    readFile.read((char*) &fileItem, 2);

    return fileItem;
}

//Function reads the data from the file, writes to a new file and
//returns the size of the image for the pixel reading function
int ReadAndWriteHeader(ifstream& readFile, ofstream& writeFile, bool writingData){
    ImageData data;
    data.idLength = CharOutput(readFile);
    data.colorMapType = CharOutput(readFile);
    data.dataTypeCode = CharOutput(readFile);
    data.colorMapOrigin = ShortOutput(readFile);
    data.colorMapLength = ShortOutput(readFile);
    data.colorMapDepth = CharOutput(readFile);
    data.xOrigin = ShortOutput(readFile);
    data.yOrigin = ShortOutput(readFile);
    data.width = ShortOutput(readFile);
    data.height = ShortOutput(readFile);
    data.bitsPerPixel = CharOutput(readFile);
    data.imageDescriptor = CharOutput(readFile);

    if(writingData) {
        writeFile.write(reinterpret_cast<char*>(&data.idLength), sizeof(data.idLength));
        writeFile.write(reinterpret_cast<char*>(&data.colorMapType), sizeof(data.colorMapType));
        writeFile.write(reinterpret_cast<char*>(&data.dataTypeCode), sizeof(data.dataTypeCode));
        writeFile.write(reinterpret_cast<char*>(&data.colorMapOrigin), sizeof(data.colorMapOrigin));
        writeFile.write(reinterpret_cast<char*>(&data.colorMapLength), sizeof(data.colorMapLength));
        writeFile.write(reinterpret_cast<char*>(&data.colorMapDepth), sizeof(data.colorMapDepth));
        writeFile.write(reinterpret_cast<char*>(&data.xOrigin), sizeof(data.xOrigin));
        writeFile.write(reinterpret_cast<char*>(&data.yOrigin), sizeof(data.yOrigin));
        writeFile.write(reinterpret_cast<char*>(&data.width), sizeof(data.width));
        writeFile.write(reinterpret_cast<char*>(&data.height), sizeof(data.height));
        writeFile.write(reinterpret_cast<char*>(&data.bitsPerPixel), sizeof(data.bitsPerPixel));
        writeFile.write(reinterpret_cast<char*>(&data.imageDescriptor), sizeof(data.imageDescriptor));
    }

    cout << "idLength: " << (int)data.idLength << endl;
    cout << "colorMapType: " << (int)data.colorMapType << endl;
    cout << "dataTypeCode: " << (int)data.dataTypeCode << endl;
    cout << "colorMapOrigin: " << data.colorMapOrigin << endl;
    cout << "colorMapLength: " << data.colorMapLength << endl;
    cout << "colorMapDepth: " << (int)data.colorMapDepth << endl;
    cout << "xOrigin: " << data.xOrigin << endl;
    cout << "yOrigin: " << data.yOrigin << endl;
    cout << "height: " << data.height << endl;
    cout << "width: " << data.width << endl;
    cout << "bitsPerPixel: " << (int)data.bitsPerPixel << endl;
    cout << "imageDescriptor: " << (int)data.imageDescriptor << endl;

    int imgSize = data.width * data.height;
    //cout << imgSize << endl;
    return imgSize;
}

vector<unsigned char> PixelReader(ifstream& readFile, int imageSize){
    Pixel pixels;
    vector<unsigned char> pixelArray;

    for(int i = 0; i < imageSize; i++) {
        pixels.BluePixel = CharOutput(readFile);
        pixelArray.push_back(pixels.BluePixel);
        pixels.GreenPixel = CharOutput(readFile);
        pixelArray.push_back(pixels.GreenPixel);
        pixels.RedPixel = CharOutput(readFile);
        pixelArray.push_back(pixels.RedPixel);
    }

    return pixelArray;
}

void ClearStream(ifstream& readFile1, ifstream& readFile2, ofstream& writeFile, ifstream* readFile3 = nullptr){
    readFile1.close();
    readFile1.clear();
    readFile2.close();
    readFile2.clear();
    writeFile.close();
    writeFile.clear();
    if(readFile3){
        readFile3->close();
        readFile3->clear();
    }
}

unsigned char Multiply(unsigned char file1pixels, unsigned char file2pixels){
    float normalizedPixel1 = ((float)file1pixels / 255.0f);
    float normalizedPixel2 = ((float)file2pixels / 255.0f);

    float outputFilePixels = normalizedPixel1 * normalizedPixel2;
    outputFilePixels *= 255.0f;
    outputFilePixels += 0.5f;

    return (unsigned char)outputFilePixels;
}

unsigned char Subtract(int file1pixels, int file2pixels){
    int diff = file1pixels - file2pixels;
    if(diff < 0)
        diff = 0;
    return (unsigned char)diff;
}

unsigned char Screen(unsigned char file1pixels, unsigned char file2pixels){
    float normalizedPixel1 = ((float)file1pixels / 255.0f);
    float normalizedPixel2 = ((float)file2pixels / 255.0f);

    float outputFilePixels = 1-((1-normalizedPixel1)*(1-normalizedPixel2));
    outputFilePixels *= 255.0f;
    outputFilePixels += 0.5f;

    return (unsigned char)outputFilePixels;
}

unsigned char Overlay(unsigned char file1pixels, unsigned char file2pixels){
    float normalizedPixel1 = ((float)file1pixels / 255.0f);
    float normalizedPixel2 = ((float)file2pixels / 255.0f);
    float outputFilePixels;

    if(normalizedPixel2 <= 0.5f){
        outputFilePixels = 2 * normalizedPixel1 * normalizedPixel2;
    }else{
        outputFilePixels = 1 - (2 * (1 - normalizedPixel1) * (1 - normalizedPixel2));
    }
    outputFilePixels *= 255.0f;
    outputFilePixels += 0.5f;

    return (unsigned char)outputFilePixels;
}

unsigned char Part6(int file1pixels){
    file1pixels += 200;
    if(file1pixels > 255)
        file1pixels = 255;

    return (unsigned char)file1pixels;
}

unsigned char Part7Red(int file1pixels){
    file1pixels *= 4;
    if(file1pixels > 255)
        file1pixels = 255;

    return (unsigned char)file1pixels;
}

unsigned char Part7Blue(){
    return (unsigned char) 0;
}

int main(int argc, const char* argv[]) {
    //Part 1
    string fileName1 = R"(input\layer1.tga)";
    string fileName2 = R"(input\pattern1.tga)";
    string taskOutput = R"(output\part1.tga)";

    ifstream readFile1(fileName1, ios_base::binary);
    ifstream readFile2(fileName2, ios_base::binary);
    ofstream writeFile(taskOutput, ios_base::binary);

    if(writeFile.is_open()){
        int imageSize = ReadAndWriteHeader(readFile1, writeFile, true);
        ReadAndWriteHeader(readFile2, writeFile, false);
       vector<unsigned char> file1pixels = PixelReader(readFile1, imageSize);
       vector<unsigned char> file2pixels = PixelReader(readFile2, imageSize);

       for(int i = 0; i < imageSize*3; i++){
           unsigned char pixelProduct = Multiply(file1pixels.at(i),file2pixels.at(i));
           writeFile.write(reinterpret_cast<char*>(&pixelProduct), sizeof(pixelProduct));
       }
        ClearStream(readFile1, readFile2, writeFile);
    }

    //Part 2
    fileName1 = R"(../cmake-build-debug/input/car.tga)";
    fileName2 = R"(../cmake-build-debug/input/layer2.tga)";
    taskOutput = R"(../cmake-build-debug/output/part2.tga)";

    readFile1.open(fileName1, ios_base::binary|ios_base::in);
    readFile2.open(fileName2, ios_base::binary|ios_base::in);
    writeFile.open(taskOutput, ios_base::binary|ios_base::out);

    if(writeFile.is_open()) {
        int imageSize = ReadAndWriteHeader(readFile1, writeFile, true);
        ReadAndWriteHeader(readFile2, writeFile, false);
        vector<unsigned char> file1pixels = PixelReader(readFile1, imageSize);
        vector<unsigned char> file2pixels = PixelReader(readFile2, imageSize);

        for (int i = 0; i < imageSize * 3; i++) {
            unsigned char pixelDifference = Subtract((int)file1pixels.at(i), (int)file2pixels.at(i));
            writeFile.write(reinterpret_cast<char*>(&pixelDifference), sizeof(pixelDifference));
        }
        ClearStream(readFile1, readFile2, writeFile);
    }

    //Part 3
    fileName1 = R"(../cmake-build-debug/input/layer1.tga)";
    fileName2 = R"(../cmake-build-debug/input/pattern2.tga)";
    string fileName3 = R"(../cmake-build-debug/input/text.tga)";
    taskOutput = R"(../cmake-build-debug/output/part3.tga)";

    readFile1.open(fileName1, ios_base::binary|ios_base::in);
    readFile2.open(fileName2, ios_base::binary|ios_base::in);
    ifstream readFile3(fileName3, ios_base::binary|ios_base::in);
    writeFile.open(taskOutput, ios_base::binary|ios_base::out);

    if(writeFile.is_open()) {
        int imageSize = ReadAndWriteHeader(readFile1, writeFile, true);
        ReadAndWriteHeader(readFile2, writeFile, false);
        ReadAndWriteHeader(readFile3, writeFile, false);
        vector<unsigned char> file1pixels = PixelReader(readFile1, imageSize);
        vector<unsigned char> file2pixels = PixelReader(readFile2, imageSize);
        vector<unsigned char> file3pixels = PixelReader(readFile3, imageSize);
        vector<unsigned char> temp;

        for (int i = 0; i < imageSize * 3; i++) {
            unsigned char pixelProduct = Multiply(file1pixels.at(i), file2pixels.at(i));
            temp.push_back(pixelProduct);
        }

        for(int i = 0; i < temp.size(); i++){
            unsigned char pixelScreen = Screen(temp.at(i), file3pixels.at(i));
            writeFile.write(reinterpret_cast<char*>(&pixelScreen), sizeof(pixelScreen));
        }
        ClearStream(readFile1, readFile2, writeFile, &readFile3);
    }

    //Part 4
    fileName1 = R"(../cmake-build-debug/input/layer2.tga)";
    fileName2 = R"(../cmake-build-debug/input/circles.tga)";
    fileName3 = R"(../cmake-build-debug/input/pattern2.tga)";
    taskOutput = R"(../cmake-build-debug/output/part4.tga)";

    readFile1.open(fileName1, ios_base::binary|ios_base::in);
    readFile2.open(fileName2, ios_base::binary|ios_base::in);
    readFile3.open(fileName3, ios_base::binary|ios_base::in);
    writeFile.open(taskOutput, ios_base::binary|ios_base::out);

    if(writeFile.is_open()) {
        int imageSize = ReadAndWriteHeader(readFile1, writeFile, true);
        ReadAndWriteHeader(readFile2, writeFile, false);
        ReadAndWriteHeader(readFile3, writeFile, false);
        vector<unsigned char> file1pixels = PixelReader(readFile1, imageSize);
        vector<unsigned char> file2pixels = PixelReader(readFile2, imageSize);
        vector<unsigned char> file3pixels = PixelReader(readFile3, imageSize);
        vector<unsigned char> temp;

        for (int i = 0; i < imageSize * 3; i++) {
            unsigned char pixelProduct = Multiply(file1pixels.at(i), file2pixels.at(i));
            temp.push_back(pixelProduct);
        }

        for(int i = 0; i < temp.size(); i++){
            unsigned char pixel4 = Subtract(temp.at(i), file3pixels.at(i));
            writeFile.write(reinterpret_cast<char*>(&pixel4), sizeof(pixel4));
        }
        ClearStream(readFile1, readFile2, writeFile, &readFile3);
    }

    //Part 5
    fileName1 = R"(../cmake-build-debug/input/layer1.tga)";
    fileName2 = R"(../cmake-build-debug/input/pattern1.tga)";
    taskOutput = R"(../cmake-build-debug/output/part5.tga)";

    readFile1.open(fileName1, ios_base::binary|ios_base::in);
    readFile2.open(fileName2, ios_base::binary|ios_base::in);
    writeFile.open(taskOutput, ios_base::binary|ios_base::out);

    if(writeFile.is_open()) {
        int imageSize = ReadAndWriteHeader(readFile1, writeFile, true);
        ReadAndWriteHeader(readFile2, writeFile, false);
        vector<unsigned char> file1pixels = PixelReader(readFile1, imageSize);
        vector<unsigned char> file2pixels = PixelReader(readFile2, imageSize);

        for (int i = 0; i < imageSize * 3; i++) {
            unsigned char pixelOverlay = Overlay(file1pixels.at(i), file2pixels.at(i));
            writeFile.write(reinterpret_cast<char*>(&pixelOverlay), sizeof(pixelOverlay));
        }
        ClearStream(readFile1, readFile2, writeFile);
    }

    //Part 6
    fileName1 = R"(../cmake-build-debug/input/car.tga)";
    taskOutput = R"(../cmake-build-debug/output/part6.tga)";

    readFile1.open(fileName1, ios_base::binary|ios_base::in);
    writeFile.open(taskOutput, ios_base::binary|ios_base::out);

    if(writeFile.is_open()) {
        int imageSize = ReadAndWriteHeader(readFile1, writeFile, true);
        vector<unsigned char> file1pixels = PixelReader(readFile1, imageSize);

        for (int i = 0; i < imageSize * 3; i+=3) {
            writeFile.write(reinterpret_cast<char*>(&file1pixels.at(i)), sizeof(file1pixels.at(i)));

            unsigned char pixelGreen = Part6((int)file1pixels.at(i+1));
            writeFile.write(reinterpret_cast<char*>(&pixelGreen), sizeof(pixelGreen));

            writeFile.write(reinterpret_cast<char*>(&file1pixels.at(i+2)), sizeof(file1pixels.at(i+2)));
        }
        ClearStream(readFile1, readFile2, writeFile);
    }

    //Part 7
    fileName1 = R"(../cmake-build-debug/input/car.tga)";
    taskOutput = R"(../cmake-build-debug/output/part7.tga)";

    readFile1.open(fileName1, ios_base::binary|ios_base::in);
    writeFile.open(taskOutput, ios_base::binary|ios_base::out);

    if(writeFile.is_open()) {
        int imageSize = ReadAndWriteHeader(readFile1, writeFile, true);
        vector<unsigned char> file1pixels = PixelReader(readFile1, imageSize);

        for (int i = 0; i < imageSize * 3; i+=3) {
            unsigned char pixelBlue = Part7Blue();
            writeFile.write(reinterpret_cast<char*>(&pixelBlue), sizeof(pixelBlue));

            writeFile.write(reinterpret_cast<char*>(&file1pixels.at(i+1)), sizeof(file1pixels.at(i+1)));

            unsigned char pixelRed = Part7Red((int)file1pixels.at(i+2));
            writeFile.write(reinterpret_cast<char*>(&pixelRed), sizeof(pixelRed));
        }
        ClearStream(readFile1, readFile2, writeFile);
    }

    //Part 8
    fileName1 = R"(../cmake-build-debug/input/car.tga)";
    string taskOutputR = R"(../cmake-build-debug/output/part8_r.tga)";
    string taskOutputG = R"(../cmake-build-debug/output/part8_g.tga)";
    string taskOutputB = R"(../cmake-build-debug/output/part8_b.tga)";

    readFile1.open(fileName1, ios_base::binary|ios_base::in);
    ofstream writeBlue(taskOutputB, ios_base::binary);
    ofstream writeGreen(taskOutputG, ios_base::binary);
    ofstream writeRed(taskOutputR, ios_base::binary);
    //writeFile.open(taskOutput, ios_base::binary|ios_base::out);

    if(writeBlue.is_open() && writeGreen.is_open() && writeRed.is_open()) {
        //MUST CLOSE AND REOPEN FILE SO IT WILL START FROM THE BEGINNING OF THE IMAGE
        int imageSize = ReadAndWriteHeader(readFile1, writeBlue, true);
        readFile1.close();
        readFile1.open(fileName1, ios_base::binary|ios_base::in);
        ReadAndWriteHeader(readFile1, writeGreen, true);
        readFile1.close();
        readFile1.open(fileName1, ios_base::binary|ios_base::in);
        ReadAndWriteHeader(readFile1, writeRed, true);
        vector<unsigned char> file1pixels = PixelReader(readFile1, imageSize);

        for (int i = 0; i < imageSize * 3; i+=3) {
            writeBlue.write(reinterpret_cast<char*>(&file1pixels.at(i)), sizeof(file1pixels.at(i)));
            writeBlue.write(reinterpret_cast<char*>(&file1pixels.at(i)), sizeof(file1pixels.at(i)));
            writeBlue.write(reinterpret_cast<char*>(&file1pixels.at(i)), sizeof(file1pixels.at(i)));
            writeGreen.write(reinterpret_cast<char*>(&file1pixels.at(i+1)), sizeof(file1pixels.at(i+1)));
            writeGreen.write(reinterpret_cast<char*>(&file1pixels.at(i+1)), sizeof(file1pixels.at(i+1)));
            writeGreen.write(reinterpret_cast<char*>(&file1pixels.at(i+1)), sizeof(file1pixels.at(i+1)));
            writeRed.write(reinterpret_cast<char*>(&file1pixels.at(i+2)), sizeof(file1pixels.at(i+2)));
            writeRed.write(reinterpret_cast<char*>(&file1pixels.at(i+2)), sizeof(file1pixels.at(i+2)));
            writeRed.write(reinterpret_cast<char*>(&file1pixels.at(i+2)), sizeof(file1pixels.at(i+2)));
        }
        ClearStream(readFile1, readFile2, writeFile);
    }

    //Part 9
    string fileNameR =  R"(../cmake-build-debug/input/layer_red.tga)";
    string fileNameG =  R"(../cmake-build-debug/input/layer_green.tga)";
    string fileNameB =  R"(../cmake-build-debug/input/layer_blue.tga)";
    taskOutput = R"(../cmake-build-debug/output/part9.tga)";

    readFile3.open(fileNameR, ios_base::binary|ios_base::in);
    readFile2.open(fileNameG, ios_base::binary|ios_base::in);
    readFile1.open(fileNameB, ios_base::binary|ios_base::in);
    writeFile.open(taskOutput, ios_base::binary|ios_base::out);

    if(writeFile.is_open()){
        int imageSize = ReadAndWriteHeader(readFile1, writeFile, true);
        vector<unsigned char> file1pixels = PixelReader(readFile1, imageSize);
        ReadAndWriteHeader(readFile2, writeFile, false);
        vector<unsigned char> file2pixels = PixelReader(readFile2, imageSize);
        ReadAndWriteHeader(readFile3, writeFile, false);
        vector<unsigned char> file3pixels = PixelReader(readFile3, imageSize);

        for (int i = 0; i < imageSize * 3; i+=3) {
            writeFile.write(reinterpret_cast<char*>(&file1pixels.at(i)), sizeof(file1pixels.at(i)));
            writeFile.write(reinterpret_cast<char*>(&file2pixels.at(i)), sizeof(file2pixels.at(i)));
            writeFile.write(reinterpret_cast<char*>(&file3pixels.at(i)), sizeof(file3pixels.at(i)));
        }
        ClearStream(readFile1, readFile2, writeFile, &readFile3);
    }

    //Part 10
    fileName1 = R"(../cmake-build-debug/input/text2.tga)";
    taskOutput = R"(../cmake-build-debug/output/part10.tga)";

    readFile1.open(fileName1, ios_base::binary|ios_base::in);
    writeFile.open(taskOutput, ios_base::binary|ios_base::out);

    if(writeFile.is_open()) {
        int imageSize = ReadAndWriteHeader(readFile1, writeFile, true);
        vector<unsigned char> file1pixels = PixelReader(readFile1, imageSize);

        for (int i = (imageSize*3)-1; i >= 0; i-=3) {
            for(int j = 2; j >= 0; j--) {
                writeFile.write(reinterpret_cast<char*>(&file1pixels.at(i-j)), sizeof(file1pixels.at(i-j)));
            }
        }
        ClearStream(readFile1, readFile2, writeFile);
    }

    return 0;
}
