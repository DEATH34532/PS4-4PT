//
// Created by Víctor Jiménez Rugama on 12/26/21.
//

#ifndef GRAPHICS_USES_FONT
#define GRAPHICS_USES_FONT 1
#endif

#include <string>
#include "../../include/utils/AppGraphics.h"
#include "../../include/view/keyboardInput.h"
#include "../../include/base.h"
#include "../../include/utils/LANG.h"


keyboardInput::keyboardInput(Scene2D * mainScene, FT_Face font, int x, int y, int width, int height, const char * enterText, const char * placeHolderText, bool hasEnter) : upperDict(), lowerDict(), dictPosition(), enter(enterText), specDictPosition(){
    this->mainScene = mainScene;
    text = placeHolderText;
    this->currChar = 0;
    this->hasEnter = hasEnter;
    if(hasEnter)
        specialKey = enter;
    else
        specialKey = LANG::mainLang->SPACE.c_str();
    this->font = font;

    this->borderColor = {180,180,180};
    this->bgColor = {255,255,255};
    this->textColor = {0,0,0};
    this->selectedTextColor = {255,0,0};//{120,120,120};


    this->containerX = x;
    this->containerY=y-height/2;
    this->containerWidth = width;
    this->containerHeight = height;

    this->boxWidth = width * (1 - KEYBOARD_HORIZONTAL_BORDER_SIZE);
    this->boxHeight = (height / 2) * (1 - 2*KEYBOARD_VERTICAL_BORDER_SIZE);

    int borderSize=(width-boxWidth)/2;
    this->boxX = x+borderSize;

    this->xText = boxX+borderSize+boxWidth*KEYBOARD_X_PAD;
    this->yInputBox = y - height / 2 + height*KEYBOARD_VERTICAL_BORDER_SIZE;
    this->yInputText = y - height/6;
    this->yKeyboardBox = y;
    this->yKeyboardTextUpper = y + borderSize + height / 8;
    this->yKeyboardTextLower = y + borderSize + 3* height / 8;

    this->keyBoardIsSelected = false;
    this->isOnUpperRow = true;
    this->entered = false;
    this->changed = false;
    std::string character;
    for(int i = 0; i < DICT_SIZE; i++) {
        character = DICT[i];
        upperDict[i] = new char[1];
        strcpy(upperDict[i], character.c_str());
        dictPosition[i] = xText+ i * boxWidth / (DICT_SIZE + 3);
    }
    delPos = xText + DICT_SIZE * boxWidth/(DICT_SIZE+3);


    for(int i = 0; i < SPEC_DICT_SIZE; i++) {
        character = SPEC_DICT[i];
        lowerDict[i] = new char[1];
        strcpy(lowerDict[i], character.c_str());
        specDictPosition[i] = xText+ i * boxWidth / (SPEC_DICT_SIZE + 5);
    }
    specialPos = xText + SPEC_DICT_SIZE * boxWidth / (SPEC_DICT_SIZE + 5);
}

void keyboardInput::updateView() {
    int currCharTEMP;
    bool isOnUpperRowTEMP;
    mainScene->DrawRectangle(containerX, containerY, containerWidth, containerHeight, borderColor);
    mainScene->DrawRectangle(boxX, yInputBox, boxWidth, boxHeight, bgColor);
    mainScene->DrawText((char*)text.c_str(),font,xText,yInputText,textColor,textColor);
    currCharTEMP = currChar;
    isOnUpperRowTEMP = isOnUpperRow;
    if(keyBoardIsSelected){
        mainScene->DrawRectangle(boxX, yKeyboardBox, boxWidth, boxHeight, bgColor);
        for(int i = 0; i < DICT_SIZE; i++) {
            if (currCharTEMP != i || !isOnUpperRowTEMP) {
                mainScene->DrawText((char *) upperDict[i], font, dictPosition[i], yKeyboardTextUpper, textColor, textColor);
            }
        }
        for(int i = 0; i < SPEC_DICT_SIZE; i++) {
            if (currCharTEMP != i || isOnUpperRowTEMP) {
                mainScene->DrawText((char *) lowerDict[i], font, specDictPosition[i], yKeyboardTextLower, textColor, textColor);
            }
        }
        if(isOnUpperRowTEMP) {
            if(currCharTEMP < DICT_SIZE) {
                mainScene->DrawText((char *) upperDict[currCharTEMP], font, dictPosition[currCharTEMP], yKeyboardTextUpper, selectedTextColor, selectedTextColor);
                mainScene->DrawText((char*) del, font, delPos, yKeyboardTextUpper, textColor, textColor);
            }else
                mainScene->DrawText((char*) del, font, delPos, yKeyboardTextUpper, selectedTextColor, selectedTextColor);
            mainScene->DrawText((char*) specialKey, font, specialPos, yKeyboardTextLower, textColor, textColor);
        } else {
            if(currCharTEMP < SPEC_DICT_SIZE) {
                mainScene->DrawText((char *) lowerDict[currCharTEMP], font, specDictPosition[currCharTEMP], yKeyboardTextLower, selectedTextColor, selectedTextColor);
                mainScene->DrawText((char*) specialKey, font, specialPos, yKeyboardTextLower, textColor, textColor);
            }else
                mainScene->DrawText((char*) specialKey, font, specialPos, yKeyboardTextLower, selectedTextColor, selectedTextColor);
            mainScene->DrawText((char*) del, font, delPos, yKeyboardTextUpper, textColor, textColor);
        }
    }
}

void keyboardInput::nextKey() {
    int tempCurrChar = currChar+1;
    if(isOnUpperRow)
        currChar = tempCurrChar%(DICT_SIZE+1);
    else
        currChar = tempCurrChar%(SPEC_DICT_SIZE+1);
}

void keyboardInput::previousKey() {
    int tempCurrChar = currChar-1;
    if(tempCurrChar < 0) {
        if (isOnUpperRow)
            currChar = DICT_SIZE;
        else
            currChar = SPEC_DICT_SIZE;
    } else {
        currChar = tempCurrChar;
    }
}

void keyboardInput::deleteChar() {
    text.pop_back();
    changed = true;
}
void keyboardInput::pressX() {
    if(currChar == DICT_SIZE && isOnUpperRow)
        deleteChar();
    else if(currChar == SPEC_DICT_SIZE && !isOnUpperRow) {
        if(hasEnter)
            entered = true;
        else{
            text += " ";
            changed = true;
        }
    }else {
        text += getCurrentKey();
        changed = true;
    }
}
char keyboardInput::getCurrentKey() {
    if((currChar == DICT_SIZE && isOnUpperRow) || (currChar == SPEC_DICT_SIZE && !isOnUpperRow))
        return NULL;
    else if(isOnUpperRow)
        return *upperDict[currChar];
    else
        return *lowerDict[currChar];
}

void keyboardInput::setUpperRow() {
    currChar %= DICT_SIZE+2;
    isOnUpperRow = true;
}
void keyboardInput::setLowerRow() {
    currChar %= SPEC_DICT_SIZE+2;
    isOnUpperRow = false;
}
void keyboardInput::selectKeyboard() {
    keyBoardIsSelected = true;
}
void keyboardInput::unSelectKeyboard() {
    keyBoardIsSelected = false;
}
bool keyboardInput::hasEntered() const {
    return entered;
}
std::string keyboardInput::readText() {
    entered = false;
    return text;
}

keyboardInput::~keyboardInput() {
    for(auto & i : upperDict) {
        delete i;
    }
    for(auto & i : lowerDict) {
        delete i;
    }
}

bool keyboardInput::hasChanged() {
    bool hasChanged = changed;
    changed = false;
    return hasChanged;
}

bool keyboardInput::active() {
    return keyBoardIsSelected;
}



