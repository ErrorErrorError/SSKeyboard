//
//  main.cpp
//  preset_parse
//
//  Created by Erik Bautista on 11/7/19.
//  Copyright © 2019 ErrorErrorError. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void parse_e0_preset(string parse, ofstream* filename, bool isGS65);
void parse_0b_preset(string effectsArray, ofstream* filename);
string getFileName(const string& s);
string convertToNewFile(string s, string model);
uint8_t undoSplittingRGB(uint8_t first, uint8_t second);
uint8_t undoColorDelta(uint8_t startColor, uint8_t delta ,uint16_t duration);
int main(int argc, const char * argv[]) {
    
    fstream file;
    string line;
    file.open(argv[1]);
    string model = argv[2];
    string newFileName = convertToNewFile(getFileName(argv[1]), model);
    
    if (!file) {
        cerr << "Unable to open file" << endl;
        exit(1);
    }
    
    if (file.is_open())
    {
        ofstream newfile(newFileName, ios::out | ios::binary);
        vector<string> effectArray;

        while (getline(file,line))
        {
            uint8_t first;
            string t = line.substr(0,2);
            first = stoi(t, 0, 16);
            if(first == 0x0e) {
                parse_e0_preset(line, &newfile, model == "gs65");
            } else if (first == 0x0b) {
                effectArray.push_back(line);
            }
        }
        
        
        for (int i = 0; i < effectArray.size(); i++) {
            parse_0b_preset(effectArray[i], &newfile);
        }
        
        file.close();
    }
    
    return 0;
}

void parse_e0_preset(string parse, ofstream* filename, bool isGS65) {
    uint8_t newFormat[524]{0};
    uint8_t region = 0;
    const uint8_t incrementBy2 = 2;
    int size = 0;
    long sizeStr = (parse.size()/2)/12;
    for (int i = 0; i < sizeStr; i++) {
        uint32_t indexArray = i * 12;
        uint32_t index = (2 + (i * 12)) * 2;
        
        if (index == 4) {
            string re = parse.substr(index, 2);
            region = stoi(re, 0, 16);
            if (region == 0x18) {
                size = 0x19;
            } else if (region == 0x24 && isGS65) {
                size = 0x14;
            } else if (region == 0x24 && !isGS65) {
                size = 0x25;
            } else if (region == 0x0b) {
                size = 0xc;
            } else if (region == 0x13) {
                region = 0x24;
                size = 0x14;
            } else {
                size = 0x2b;
            }

            newFormat[indexArray] = region;
            newFormat[indexArray + 1] = region;
            index += incrementBy2;
        } else {
            index += incrementBy2;
            string re = parse.substr(index, 2);
            uint8_t key = stoi(re, 0, 16);
            if (key == 0) {
                break;
            }
            if (key == 0x31) {
                // printf("");
            }
            newFormat[indexArray] = region;
            newFormat[indexArray + 1] = key;
        }
        
        index += incrementBy2;
        /// RGB Section Main Color
        string rstring = parse.substr(index, 2);
        uint8_t rColor = stoi(rstring, 0, 16);
        newFormat[indexArray + 2] = rColor;
        index += incrementBy2;

        string gstring = parse.substr(index, 2);
        uint8_t gColor = stoi(gstring, 0, 16);
        newFormat[indexArray + 3] = gColor;
        index += incrementBy2;

        string bstring = parse.substr(index, 2);
        uint8_t bColor = stoi(bstring, 0, 16);
        newFormat[indexArray + 4] = bColor;
        index += incrementBy2;
        
        /// RGB Section active Color
        string rastring = parse.substr(index, 2);
        uint8_t raColor = stoi(rastring, 0, 16);
        newFormat[indexArray + 5] = raColor;
        index += incrementBy2;

        string gastring = parse.substr(index, 2);
        uint8_t gaColor = stoi(gastring, 0, 16);
        newFormat[indexArray + 6] = gaColor;
        index += incrementBy2;

        string bastring = parse.substr(index, 2);
        uint8_t baColor = stoi(bastring, 0, 16);
        newFormat[indexArray + 7] = baColor;
        index += incrementBy2;
        
        /// duration
        string durationOneStr = parse.substr(index, 2);
        uint8_t durationOne = stoi(durationOneStr, 0, 16);
        newFormat[indexArray + 8] = durationOne;
        index += incrementBy2;
        
        string durationTwoStr = parse.substr(index, 2);
        uint8_t durationTwo = stoi(durationTwoStr, 0, 16);
        newFormat[indexArray + 9] = durationTwo;
        index += incrementBy2;

        /// effectId
        string effectIdStr = parse.substr(index, 2);
        uint8_t effectId = stoi(effectIdStr, 0, 16);
        newFormat[indexArray + 10] = effectId;
        index += incrementBy2;
        
        /// Mode
        string modeStr = parse.substr(index, 2);
        uint8_t mode = stoi(modeStr, 0, 16);
        
        if (mode != 0) {
            if (mode == 1) {
                mode = 0;
            } else {
                mode = 3;
            }
        } else {
            mode = 1;
        }
        
        newFormat[indexArray + 11] = mode;
    }
    /*
    for (int i = 0; i < (size * 12); i++) {
        if (i == 240) {
            printf("");
        }
        printf("%02x", newFormat[i]);
    }
    cout << endl;
     */
    
    if (filename->is_open()) {
        filename->write(reinterpret_cast<char *>(&newFormat), size * 12);
    }
}

void parse_0b_preset(string effectsArray, ofstream* filename) {
    uint8_t newFormat[524]{0};
    const uint8_t incrementBy2 = 2;
    int index = incrementBy2 * 2;
    int indexArray = 0;
    
    // EffectID
    string effectIdStr = effectsArray.substr(index, 2);
    uint8_t effectId = stoi(effectIdStr, 0, 16);
    newFormat[indexArray + 0] = effectId;
    index += incrementBy2;
    index += incrementBy2;
    
    // Number of Transitions
    string numOfTransitionStr = effectsArray.substr(0x96 * 2, 2);
    uint8_t numTransitions = stoi(numOfTransitionStr, 0, 16);
    newFormat[indexArray + 1] = numTransitions;
    
    // Start Color
    int startColorIndex = (0x84 * 2);
    string startRFstr = effectsArray.substr(startColorIndex, 2);
    uint8_t startRF = stoi(startRFstr, 0, 16);
    startColorIndex += incrementBy2;
    string startRSstr = effectsArray.substr(startColorIndex, 2);
    uint8_t startRS = stoi(startRSstr, 0, 16);
    uint8_t startRColor = undoSplittingRGB(startRF, startRS);
    startColorIndex += incrementBy2;
    
    string startGFstr = effectsArray.substr(startColorIndex, 2);
    uint8_t startGF = stoi(startGFstr, 0, 16);
    startColorIndex += incrementBy2;
    string startGSstr = effectsArray.substr(startColorIndex, 2);
    uint8_t startGS = stoi(startGSstr, 0, 16);
    uint8_t startGColor = undoSplittingRGB(startGF, startGS);
    startColorIndex += incrementBy2;

    string startBFstr = effectsArray.substr(startColorIndex, 2);
    uint8_t startBF = stoi(startBFstr, 0, 16);
    startColorIndex += incrementBy2;
    string startBSstr = effectsArray.substr(startColorIndex, 2);
    uint8_t startBS = stoi(startBSstr, 0, 16);
    uint8_t startBColor = undoSplittingRGB(startBF, startBS);

    // printf("%02x, %02x, %02x", startRColor, startGColor, startBColor);
    // printf("\n");
    for (int i = 0; i < numTransitions; i++) {
        indexArray = 2 + (i * 5);
        
        string rcolorstr = effectsArray.substr(index, 2);
        uint8_t rcolor = stoi(rcolorstr, 0, 16);
        // newFormat[indexArray] = rcolor;
        index += incrementBy2;

        string gcolorstr = effectsArray.substr(index, 2);
        uint8_t gcolor = stoi(gcolorstr, 0, 16);
        // newFormat[indexArray + 1] = gcolor;
        index += incrementBy2;

        string bcolorstr = effectsArray.substr(index, 2);
        uint8_t bcolor = stoi(bcolorstr, 0, 16);
        // newFormat[indexArray + 2] = bcolor;
        index += incrementBy2;
        index += incrementBy2;
        
        string firstDurStr = effectsArray.substr(index, 2);
        uint8_t firstDur = stoi(firstDurStr, 0, 16);
        newFormat[indexArray + 3] = firstDur;
        index += incrementBy2;

        string secondDurStr = effectsArray.substr(index, 2);
        uint8_t secondDur = stoi(secondDurStr, 0, 16);
        newFormat[indexArray + 4] = secondDur;
        index += incrementBy2;
        index += incrementBy2;
        index += incrementBy2;
        
        uint8_t rDelta = rcolor;
        uint8_t gDelta = gcolor;
        uint8_t bDelta = bcolor;

        rcolor = startRColor;
        gcolor = startGColor;
        bcolor = startBColor;
        uint16_t dur = (secondDur << 8) | firstDur;
        startRColor = undoColorDelta(startRColor, rDelta, dur);
        startGColor = undoColorDelta(startGColor, gDelta, dur);
        startBColor = undoColorDelta(startBColor, bDelta, dur);

        newFormat[indexArray] = rcolor;
        newFormat[indexArray + 1] = gcolor;
        newFormat[indexArray + 2] = bcolor;

    }
    indexArray += 5;
    
    index = 0x9a * 2;
    string waveDirStr = effectsArray.substr(index, 2);
    uint8_t waveDir = stoi(waveDirStr, 0, 16);
    newFormat[indexArray + 0] = waveDir;

    // wavelength
    index = 0x94 * 2;
    string firstDurStr = effectsArray.substr(index, 2);
    uint8_t firstDur = stoi(firstDurStr, 0, 16);
    newFormat[indexArray + 1] = firstDur;
    index += incrementBy2;

    string secondDurStr = effectsArray.substr(index, 2);
    uint8_t secondDur = stoi(secondDurStr, 0, 16);
    newFormat[indexArray + 2] = secondDur;

    /// wave origin
    index = 0x8c * 2;
    string orgXOneStr = effectsArray.substr(index, 2);
    uint8_t orgXOne = stoi(orgXOneStr,0, 16);
    newFormat[indexArray + 3] = orgXOne;
    index += incrementBy2;
    string orgXTwoStr = effectsArray.substr(index, 2);
    uint8_t orgXTwo = stoi(orgXTwoStr,0, 16);
    newFormat[indexArray + 4] = orgXTwo;
    index += incrementBy2;
    
    string orgYOneStr = effectsArray.substr(index, 2);
    uint8_t orgYOne = stoi(orgYOneStr,0, 16);
    newFormat[indexArray + 5] = orgYOne;
    index += incrementBy2;
    string orgYTwoStr = effectsArray.substr(index, 2);
    uint8_t orgYtwo = stoi(orgYTwoStr,0, 16);
    newFormat[indexArray + 6] = orgYtwo;
    index += incrementBy2;
    
    /// Wave rad control
    string xWaveStr = effectsArray.substr(index, 2);
    uint8_t xWave = stoi(xWaveStr, 0, 16);
    index += incrementBy2;
    index += incrementBy2;
    
    string yWaveStr = effectsArray.substr(index, 2);
    uint8_t yWave = stoi(yWaveStr, 0, 16);
    index += incrementBy2;
    index += incrementBy2;
    
    uint8_t waveRadControl = 0;
    if (yWave == 1 && xWave == 1) {
        waveRadControl = 0;
    } else if (yWave == 0 && xWave == 1) {
        waveRadControl = 1;
    } else if (yWave == 1 && xWave == 0){
        waveRadControl = 2;
    } else {
        waveRadControl = 0;
    }
    
    newFormat[indexArray + 7] = waveRadControl;
    newFormat[indexArray + 8] = (yWave == 0 && xWave == 0) ? 0 : 1;
    int size = 11 + (numTransitions * 5);
    /*
    for (int i = 0; i < size; i++) {
        printf("%02x", newFormat[i]);
    }
     */

    cout << endl;
    
    if (filename->is_open()) {
        filename->write(reinterpret_cast<const char *>(&newFormat), size);
    }

}

string getFileName(const string& s) {

   char sep = '/';

   size_t i = s.rfind(sep, s.length());
   if (i != string::npos) {
      return(s.substr(i+1, s.length() - i));
   }

   return("");
}

uint8_t undoColorDelta(uint8_t startColor, uint8_t delta ,uint16_t duration) {
    uint8_t divider = duration / 16;
    uint8_t calc = startColor + (divider * delta);
    return calc;
}

uint8_t undoSplittingRGB(uint8_t first, uint8_t second) {
    uint8_t firstShift = first >> 4;
    uint8_t secondShift = second << 4;
    uint8_t addingBoth = secondShift | firstShift;
    return addingBoth;
}

string convertToNewFile(string s, string model) {
    char sep = '.';
    size_t i = s.rfind(sep, s.length());
    std::string test;
    if (i != string::npos) {
        string newStr = s.insert(i,  "-" + model);
        string bin = "bin";
       return(newStr.replace(newStr.length() - 3, bin.length(), bin));
    }

    return("");
}
