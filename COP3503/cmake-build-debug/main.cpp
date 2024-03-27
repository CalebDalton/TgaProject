#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <map>
#include "ImageData.h"
using namespace std;

//dalton.caleb.project2

bool TgaChecker(string& fileLink){
    if(fileLink.substr(fileLink.find_last_of('.')) == ".tga")
        return true;
    else
        return false;
}

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

int ReadAndWriteHeader(ifstream& readFile, ofstream& writeFile, bool writingData){
    ImageData data;
    vector<unsigned char> pixelArray;
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

    int imgSize = data.width * data.height;

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

    return imgSize;
}

void ClearStream(ifstream& readFile1, ifstream& readFile2, ifstream* readFile3 = nullptr){
    readFile1.close();
    readFile1.clear();
    readFile2.close();
    readFile2.clear();
    if(readFile3){
        readFile3->close();
        readFile3->clear();
    }
}

unsigned char multiply(unsigned char file1pixels, unsigned char file2pixels){
    float normalizedPixel1 = ((float)file1pixels / 255.0f);
    float normalizedPixel2 = ((float)file2pixels / 255.0f);

    float outputFilePixels = normalizedPixel1 * normalizedPixel2;
    outputFilePixels *= 255.0f;
    outputFilePixels += 0.5f;

    return (unsigned char)outputFilePixels;
}

unsigned char subtract(unsigned char file1pixels, unsigned char file2pixels){
    int diff = (int)file1pixels - (int)file2pixels;
    if(diff < 0)
        diff = 0;
    return (unsigned char)diff;
}

unsigned char overlay(unsigned char file1pixels, unsigned char file2pixels){
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

unsigned char screen(unsigned char file2pixels, unsigned char file1pixels){
    float normalizedPixel1 = ((float)file1pixels / 255.0f);
    float normalizedPixel2 = ((float)file2pixels / 255.0f);

    float outputFilePixels = 1-((1-normalizedPixel1)*(1-normalizedPixel2));
    outputFilePixels *= 255.0f;
    outputFilePixels += 0.5f;

    return (unsigned char)outputFilePixels;
}

unsigned char addred(unsigned char file1pixels, int color){
    color += (int)file1pixels;
    if(color > 255)
        color = 255;

    return (unsigned char)color;
}

unsigned char addgreen(unsigned char file1pixels, int color){
    color += (int)file1pixels;
    if(color > 255)
        color = 255;

    return (unsigned char)color;
}

unsigned char addblue(unsigned char file1pixels, int color){
    color += (int)file1pixels;
    if(color > 255)
        color = 255;

    return (unsigned char)color;
}

unsigned char scalered(unsigned char file1pixels, int color){
    color *= (int)file1pixels;
    if(color > 255)
        color = 255;

    return (unsigned char)color;
}

unsigned char scalegreen(unsigned char file1pixels, int color){
    color *= (int)file1pixels;
    if(color > 255)
        color = 255;

    return (unsigned char)color;
}

unsigned char scaleblue(unsigned char file1pixels, int color){
    color *= (int)file1pixels;
    if(color > 255)
        color = 255;

    return (unsigned char)color;
}

vector<unsigned char> onlyred(vector<unsigned char> trackingPixels, int imageSize){
    vector<unsigned char> pixels;
    for(int j = 0; j < imageSize*3; j+=3){
        unsigned char red = trackingPixels[j+2];
        pixels.push_back(red);
        pixels.push_back(red);
        pixels.push_back(red);
    }
    return pixels;
}

vector<unsigned char> onlygreen(vector<unsigned char> trackingPixels, int imageSize){
    vector<unsigned char> pixels;
    for(int j = 0; j < imageSize*3; j+=3){
        unsigned char green = trackingPixels[j+1];
        pixels.push_back(green);
        pixels.push_back(green);
        pixels.push_back(green);
    }
    return pixels;
}

vector<unsigned char> onlyblue(vector<unsigned char> trackingPixels, int imageSize){
    vector<unsigned char> pixels;
    for(int j = 0; j < imageSize*3; j+=3){
        unsigned char blue = trackingPixels[j];
        pixels.push_back(blue);
        pixels.push_back(blue);
        pixels.push_back(blue);
    }
    return pixels;
}







int main(int argc, char* argv[]) {
    if (argc == 1 || strcmp(argv[1], "--help") == 0) {
        cout << "Project 2: Image Processing, Spring 2024" << endl << endl
             << "Usage:" << endl << '\t' << argv[0] << " [output] [firstImage] [method] [...]" << endl;
        return 0;
    }


    //string trackingFile;
    map<string, int> funcArgs = {{"multiply", 2},{"subtract", 2},{"overlay", 2},{"screen", 2},
                                 {"combine", 3},{"flip", 0},{"onlyred", 0},{"onlygreen", 0},{"onlyblue", 0},
                                 {"addred", 1},{"addgreen", 1},{"addblue", 1},
                                 {"scalered", 1},{"scalegreen", 1},{"scaleblue", 1}};
    string fileName1;
    string fileName2;
    string outputFile;
    string trackingFile;

    ifstream readTrackingFile;
    ifstream readFile1;
    ifstream readFile2;
    ofstream writeFile;

    unsigned char (*oneFuncPtr)(unsigned char, unsigned char);
    vector<unsigned char> (*zeroFuncPtr)(vector<unsigned char>, int);
    unsigned char (*intFuncPtr)(unsigned char, int);

    outputFile = argv[1];
    writeFile.open(outputFile, ios_base::binary|ios_base::out);
    trackingFile = argv[2];
    readTrackingFile.open(trackingFile, ios_base::binary|ios_base::out);

    int imageSize;

    //Start looping thru argv
    if(argc >= 3 && writeFile.is_open()){


        imageSize = ReadAndWriteHeader(readTrackingFile, writeFile, true);
        for(int i = 3; i < argc; i++) {

            if (TgaChecker(outputFile) && TgaChecker(trackingFile)) {

                if(funcArgs.find((string)argv[i]) != funcArgs.end()){
                    int count = funcArgs.find(argv[i])->second;
                    cout << "count: " << count << endl;

                    if (count == 3) {
                        fileName1 = (string)argv[i+1];
                        fileName2 = (string)argv[i+2];
                        readFile1.open(fileName1, ios_base::binary);
                        readFile2.open(fileName2, ios_base::binary);
                        ReadAndWriteHeader(readFile1, writeFile, false);
                        ReadAndWriteHeader(readFile2, writeFile, false);
                        vector<unsigned char> trackingPixels = PixelReader(readTrackingFile, imageSize);
                        vector<unsigned char> file1pixels = PixelReader(readFile1, imageSize);
                        vector<unsigned char> file2pixels = PixelReader(readFile2, imageSize);

                        for (int j = 0; j < imageSize * 3; j+=3) {
                            writeFile.write(reinterpret_cast<char*>(&trackingPixels.at(j)), sizeof(trackingPixels.at(j)));
                            writeFile.write(reinterpret_cast<char*>(&file1pixels.at(j)), sizeof(file1pixels.at(j)));
                            writeFile.write(reinterpret_cast<char*>(&file2pixels.at(j)), sizeof(file2pixels.at(j)));
                        }
                        i+=2;
                        cout << "Combine ran." << endl;
                        ClearStream(readFile1, readFile2);
                    }

                    else if (count == 2) {
                        if(strcmp(argv[i], "multiply") == 0)
                            oneFuncPtr = &multiply;
                        else if(strcmp(argv[i], "subtract") == 0)
                            oneFuncPtr = &subtract;
                        else if(strcmp(argv[i], "overlay") == 0)
                            oneFuncPtr = &overlay;
                        else if(strcmp(argv[i], "screen") == 0)
                            oneFuncPtr = &screen;

                        fileName1 = (string)argv[i+1];
                        readFile1.open(fileName1, ios_base::binary);
                        ReadAndWriteHeader(readFile1, writeFile, false);
                        vector<unsigned char> trackingPixels = PixelReader(readTrackingFile, imageSize);
                        vector<unsigned char> file1Pixels = PixelReader(readFile1, imageSize);

                        for(int j = 0; j < imageSize*3; j++){
                            unsigned char pixelFunc = oneFuncPtr(trackingPixels.at(j), file1Pixels.at(j));
                            writeFile.write(reinterpret_cast<char*>(&pixelFunc), sizeof(pixelFunc));
                        }
                        ClearStream(readFile1, readFile2);
                        cout << "oneFuncPtr: " << i << endl;
                        i++;
                    }

                    else if (count == 1) {
                        intFuncPtr = reinterpret_cast<unsigned char (*)(unsigned char, int)>(argv[i + 1]);
                        vector<unsigned char> trackingPixels = PixelReader(readTrackingFile, imageSize);
                        if(strcmp(argv[i], "addred") == 0)
                            intFuncPtr = &addred;
                        else if(strcmp(argv[i], "addgreen") == 0)
                            intFuncPtr = &addgreen;
                        else if(strcmp(argv[i], "addblue") == 0)
                            intFuncPtr = &addblue;
                        else if(strcmp(argv[i], "scalered") == 0)
                            intFuncPtr = &scalered;
                        else if(strcmp(argv[i], "scalegreen") == 0)
                            intFuncPtr = &scalegreen;
                        else if(strcmp(argv[i], "scaleblue") == 0)
                            intFuncPtr = &scaleblue;

                        for(int j = 0; j < imageSize*3; j++){
                            unsigned char pixelFunc = intFuncPtr(trackingPixels.at(j), atoi(argv[i+1]));
                            writeFile.write(reinterpret_cast<char*>(&pixelFunc), sizeof(pixelFunc));
                        }
                        cout << "intFuncPtr: " << i << endl;
                        i++;
                    }

                    else {
                        zeroFuncPtr = reinterpret_cast<vector<unsigned char> (*)(vector<unsigned char>, int)>(argv[i+1]);
                        vector<unsigned char> trackingPixels = PixelReader(readTrackingFile, imageSize);

                        if(strcmp(argv[i], "onlyred") == 0)
                            zeroFuncPtr = &onlyred;
                        else if(strcmp(argv[i], "onlygreen") == 0)
                            zeroFuncPtr = &onlygreen;
                        else if(strcmp(argv[i], "onlyblue") == 0)
                            zeroFuncPtr = &onlyblue;
//                        else if(strcmp(argv[i], "flip") == 0)
//                            zeroFuncPtr = &flip;
                        cout << "zeroFuncPtr: " << i << endl;
                        vector<unsigned char> temp = zeroFuncPtr(trackingPixels, imageSize);
                        writeFile.write(reinterpret_cast<char*>(&temp[0]), temp.size());
                    }


                }else{
                    cout << "Invalid method name. " << endl;
                }
                //funcPtr = reinterpret_cast<unsigned char (*)(unsigned char, unsigned char)>(argv[i]);



            } else {
                cout << "Invalid File Name." << endl;
            }
        }
    }
    else if(!writeFile.is_open()){
        cout << "Invalid File Name." << endl;
    }
    return 0;
}