/************************************************************************
* Name: Andrew Yang                                                     *
* Teacher: Mr. Creelman                                                 *
* Course: ICS4U                                                         *
* Date: March 19 2018                                                   *
* File Name: rpnModule.cpp                                              *
*                                                                       *
* Purpose - to supplant the necessary functions for the RPN Calculator. *
*************************************************************************/

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/mouse.h>
#include <cstring>
#include <fstream>
#include "rpnModule.h"
#include <apvector.h>

#define _USE_MATH_DEFINES
#include <cmath>
#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define SELECTED al_map_rgb(128, 128, 255)

bool isOperator(char a){
    for (int i = 0; i < strlen(operatorList); i++){
        if (a == operatorList[i]){
            return true;
        }
    }
    return false;
}

// This function performs an operation that requires two operands
double operate(double a, double b, char operation){
    switch (operation){
        case '+':
            return a + b;
        case '-':
            return a - b;
        case 'x':
            return a * b;
        case '/':
            return a / b;
        case '^':
            return pow(a, b);
        case 'g':
            return log(b) / log(a);
        default:
            return 0.0;
    }
}

// This function performs an operation that requires one operand
double func(double a, char operation){
    switch (operation){
        case 'n':
            return log(a);
        case 'L':
            return log10(a);
        case '!':
            return factorial(int(a));
        default:
            return 0.0;
    }
}

// This function performs trigonometric functions
double trigFunc(double a, char operation, angleType unit){

    double returnValue = 0.0;

    switch (operation){
        case 's':
            if (unit == degrees){
                a = a * pi / 180.0;
            }
            return sin(a);
        case 'c':
            if (unit == degrees){
                a = a * pi / 180.0;
            }
            return cos(a);
        case 't':
            if (unit == degrees){
                a = a * pi / 180.0;
            }
            return tan(a);
        case 'S':
            returnValue = asin(a);
            if (unit == degrees){
                returnValue = returnValue * 180.0 / pi;
            }
            return returnValue;
        case 'C':
            returnValue = acos(a);
            if (unit == degrees){
                returnValue = returnValue * 180.0 / pi;
            }
            return returnValue;
        case 'T':
            returnValue = atan(a);
            if (unit == degrees){
                returnValue = returnValue * 180.0 / pi;
            }
            return returnValue;
        default:
            return 0.0;
    }
}

// This function converts the name of a mathematical constant into the value it refers to
double constant(apstring name){
    if (name == "e"){
        return e;
    } else if (name == "pi"){
        return pi;
    } else if (name == "phi"){
        return phi;
    } else {
        return 0.0;
    }
}

// This function calculates a factorial
int factorial(int n){
    if (n < 0){
        return -1;
    } else if (n == 0 || n == 1){
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

// This function checks if the GUI has been loaded properly
int checkScreen(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue){

    if (!display){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    if (!al_init_primitives_addon()){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize primitives addon!",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    if (!al_init_image_addon()){
        al_show_native_message_box(display, "Error", "Error", "Failed to initialize image addon!",
                                   nullptr, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    if(!event_queue) {
        al_show_native_message_box(display, "Error", "Error", "Failed to create event queue!",
                                nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    	return -1;
    }

    al_flip_display();
    return 0;
}

// This function loads the text boxes
int loadTextBox(Box textBox[], int x, int y, int size, int SCREEN_W, int SCREEN_H, ALLEGRO_FONT *font){

    int errorCheck = 0;
    textBox[0].setY(y);

    for (int i = 0; i < 4; i++){

        // Initialize text box
        textBox[i].setX(1);
        textBox[i].setW(SCREEN_W - 1);
        textBox[i].setY(i * size + textBox[0].getY());
        textBox[i].setL(size);
        textBox[i].setText("");
        textBox[i].setBackColour(WHITE);
        textBox[i].setForeColour(BLACK);
        textBox[i].setFont(font);

        // Display text box
        errorCheck = textBox[i].draw(SCREEN_W, SCREEN_H, 1);
        if (errorCheck){
            return -1;
        }
    }

    // Initialize the bottom-most text box as the selected text box
    errorCheck = textBox[3].showLine(SCREEN_W, SCREEN_H, 20);
    if (errorCheck){
        return -1;
    }
    return 0;
}

// This function loads all the buttons
int loadButtons(Button key[][J], int stdSize, int largeSize, int SCREEN_W, int SCREEN_H,
                ALLEGRO_FONT *font, char keyFileName[]){

    // Declare and initialize variables
    char temp[7] = "";
    int errorCheck = 0;

    // Open file
    ifstream keyFile (keyFileName);
    if (!keyFile){
        cout << keyFileName << " not found." << endl;
        return -1;
    }

    // Initialize top right corner of the button board
    key[0][0].setX(1);
    key[0][0].setY(SCREEN_H - 200);

    // Load Buttons
    for (int i = 0; i < 4; i++){

        key[i][0].setX(1);
        key[i][0].setW(stdSize);

        for (int j = 0; j < 8; j++){

            keyFile >> temp;
            key[i][j].setY(i * stdSize + key[0][0].getY());
            key[i][j].setL(stdSize);
            key[i][j].setText(temp);
            key[i][j].setBackColour(WHITE);
            key[i][j].setForeColour(BLACK);
            key[i][j].setFont(font);
            key[i][j].unPress();

            // Create wider buttons for trigonometric functions and the radian/degree panel
            if (j > 0){
                key[i][j].setX(key[i][j - 1].getX() + key[i][j - 1].getW());

                if (j == 1 || j == 2){
                    key[i][j].setW(largeSize);
                } else if (j != 0){
                    key[i][j].setW(stdSize);
                }
            }

            // Draw button
            errorCheck = key[i][j].draw(SCREEN_W, SCREEN_H, 1);
            if (errorCheck){
                return -1;
            }
        }
    }

    // Initialize radians
    key[0][1].setBackColour(SELECTED);
    errorCheck = key[0][1].draw(SCREEN_W, SCREEN_H, 1);
    if (errorCheck){
        return -1;
    }

    keyFile.close();

    al_flip_display();

    return 0;
}

// This function counts the number of occurences of 'ch' in object
int count(apstring object, char ch){
    int count = 0;
    for (int i = 0; i < object.length(); i++){
        if (object[i] == ch){
            count++;
        }
    }
    return count;
}

// This function determines if a mouse's coordinates are within a button
bool onButton(int x, int y, Button someButton){
    return (x > someButton.getX() && x < someButton.getX() + someButton.getW() &&
            y > someButton.getY() && y < someButton.getY() + someButton.getL() && !someButton.pressed());
}

// This function determines if 'check' is in 'str'
bool inString(char str[], char check){
    for (int i = 0; i < strlen(str); i++){
        if (str[i] == check){
            return true;
        }
    }
    return false;
}

// This function determines if 'text' can be converted to a valid float
bool validNum(const apstring &text){

    if (text == "" || count(text, '.') > 1 || text == "."){
        return false;
    }

    return true;
}

/* This function acts as the 'kernel' of the program. It checks each button to see if it has
   been pressed, changes the selected text box, and performs the calculations. */
int process(Button key[][8], Box text[], Button *&currentButton, Box *&currentLine, rpnStack &myStack, ALLEGRO_DISPLAY *display,
             ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_MOUSE_STATE &mouse_position, int stdSize, bool &newLine,
             angleType angle, int SCREEN_W, int SCREEN_H, int margin){

    bool exit = false;

    while (!exit){

        // Create event queue and initialize variables
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        char temp[7] = "";
        int i = 0, j = 0, errorCheck = 0;

        // User presses mouse
        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
            al_get_mouse_state(&mouse_position);

            // Check that mouse is in the lower half of the screen and the text box is valid
            if (mouse_position.y > stdSize * 4 + 1 &&
                currentLine >= (&text[0] - 1) && currentLine <= &text[3]){

                i = 0; j = 0;

                // Check the number pad
                for (i = 0; i < 4; i++){
                    for (j = 5; j < 8; j++){
                        if (onButton(mouse_position.x, mouse_position.y, key[i][j]) &&
                            (i < 3 || j < 7) && currentLine >= &text[0] && currentLine <= &text[3]){

                            // Add characters to the selected line of text
                            currentLine->addText(key[i][j].getText());

                            break;
                        }
                    }
                }

                // Check the four basic operators
                for (i = 0; i < 4; i++){
                    if (onButton(mouse_position.x, mouse_position.y, key[i][4]) && !myStack.isEmpty()){

                        // Perform the operation if the stack has at least two valid operands
                        if (currentLine >= &text[0] && myStack.size() >= 2 && validNum(currentLine->getText())){

                            // Retrieve the operator character and perform the operation
                            strcpy(temp, key[i][4].getText().c_str());
                            string ree = to_string(myStack.calculate(temp[0]));
                            currentLine->setText("");
                            (currentLine + 1)->setText(ree);
                            newLine = true;
                        } else if (myStack.isSingle()){
                            break;

                        // Display error message if there are too many decimal points
                        } else if (count(currentLine->getText(), '.') > 1){
                            al_show_native_message_box(display, "Error", "Error", "Too many decimal points!",
                                                       nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                            break;
                        }

                        j = 4;

                        break;
                    }
                }

                // Check the power and log operators
                for (i = 0; i <= 1; i++){
                    if (onButton(mouse_position.x, mouse_position.y, key[i][3])){
                        if (currentLine >= &text[0] && validNum(currentLine->getText()) && myStack.size() >= 2){

                            // Retrieve the operator character and perform the operation
                            strcpy(temp, key[i][3].getText().c_str());
                            string ree = to_string(myStack.calculate(temp[key[i][3].getText().length() - 1]));
                            currentLine->setText("");
                            (currentLine + 1)->setText(ree);
                            newLine = true;
                        } else if (!myStack.isSingle()){
                            break;

                        } else if (count(currentLine->getText(), '.') > 1){
                            al_show_native_message_box(display, "Error", "Error", "Too many decimal points!",
                                                        nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                            break;
                        }

                        j = 3;
                        break;
                    }
                }

                // Check the natural log and factorial operators
                for (i = 2; i <= 3; i++){
                    if (onButton(mouse_position.x, mouse_position.y, key[i][3])){
                        if (currentLine >= &text[0] && validNum(currentLine->getText()) && !myStack.isEmpty()){

                            // Retrieve the operator character and perform the operation
                            strcpy(temp, key[i][3].getText().c_str());
                            string ree = to_string(myStack.calculate(temp[key[i][3].getText().length() - 1]));
                            currentLine->setText(ree);
                            if (currentLine - 1 >= &text[0]){
                                currentLine--;
                                newLine = true;
                            }
                        } else if (myStack.isEmpty() || currentLine->getText() == ""){
                            break;
                        } else if (count(currentLine->getText(), '.') > 1){
                            al_show_native_message_box(display, "Error", "Error", "Too many decimal points!",
                                                        nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                            break;
                        }

                        j = 3;
                        break;
                    }
                }

                // Check the radian/degrees panel
                for (j = 1; j <= 2; j++){
                    if (onButton(mouse_position.x, mouse_position.y, key[0][j])){
                        if (angle == radians){
                            angle = degrees;
                        } else if (angle == degrees){
                            angle = radians;
                        }

                        i = 0;
                        break;
                    }
                }

                // Check the trig functions
                for (i = 1; i < 4; i++){
                    for (j = 1; j <= 2; j++){
                        if (onButton(mouse_position.x, mouse_position.y, key[i][j])){
                            if (currentLine >= &text[0] && validNum(currentLine->getText()) && !myStack.isEmpty()){

                                // Retrieve the operator character and perform the operation
                                strcpy(temp, key[i][j].getText().c_str());

                                // Adjust operator character for inverse trig functions
                                if (key[i][j].getText().length() > 3){
                                    temp[0] = toupper(temp[3]);
                                }
                                string ree = to_string(myStack.calculate(temp[0], angle));
                                currentLine->setText(ree);
                                if (currentLine - 1 >= &text[0]){
                                    currentLine--;
                                    newLine = true;
                                }
                            } else if (myStack.isEmpty() || currentLine->getText() == ""){
                                break;
                            } else if (count(currentLine->getText(), '.') > 1){
                                al_show_native_message_box(display, "Error", "Error", "Too many decimal points!",
                                                            nullptr, ALLEGRO_MESSAGEBOX_ERROR);
                                break;
                            }
                            break;
                        }
                    }
                }

                // Check constants
                for (i = 0; i < 3; i++){
                    if (onButton(mouse_position.x, mouse_position.y, key[i][0])){
                        currentLine->setText(to_string(constant(key[i][0].getText())));

                        j = 0;
                        break;
                    }
                }

                // Check enter button
                if (onButton(mouse_position.x, mouse_position.y, key[3][7])){
                    if (currentLine > &text[0] && validNum(currentLine->getText())){
                        currentLine--;
                        newLine = true;
                    }

                    i = 3; j = 7;
                }

                // Check all clear button
                if (onButton(mouse_position.x, mouse_position.y, key[3][0])){
                    if (currentLine == &text[0] - 1){
                        currentLine++;
                    }

                    currentLine->setText("");

                    if (currentLine < &text[3]){
                        currentLine++;
                    }

                    if (!myStack.isEmpty()){
                        myStack.pop();
                    }
                    i = 3; j = 0;
                }

                // Press the selected button
                currentButton = &key[i][j];
                currentButton->press();

                // Re-draw all text boxes
                for (int k = 0; k < 4; k++){
                    text[k].clear(SCREEN_W, SCREEN_H, 1);
                    text[k].print(SCREEN_W, SCREEN_H, margin);
                    if (currentLine == &text[k]){
                        currentLine->showLine(SCREEN_W, SCREEN_H, margin);
                    }
                }

                // Reassign the colours to the radian-degree panel
                if (angle == radians){
                    key[0][1].setBackColour(SELECTED);
                    key[0][2].setBackColour(WHITE);
                } else if (angle == degrees){
                    key[0][2].setBackColour(SELECTED);
                    key[0][1].setBackColour(WHITE);
                }

                // Re-draw the radian-degree panel
                for (j = 1; j <= 2; j++){
                    key[0][j].draw(SCREEN_W, SCREEN_H, 1);
                }

                // Push without popping if there is a new line
                if (newLine){
                    if (validNum(currentLine->getText())){
                        myStack.push(stof(currentLine->getText().c_str()));
                        newLine = false;
                    }
                }

                // Dynamically check the text after every button click
                myStack.pushFromText(currentLine->getText(), &text[3] - currentLine + 1);
                // myStack.show();
                al_flip_display();
            }

        // User releases mouse
        } else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
            currentButton->unPress();

        // User closes screen
        } else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            al_destroy_display(display);
            exit = true;
        }
    }

    return 0;
}
int sum(const apvector<int> &v){
    int count = 0;
    for (int i = 0; i < v.length(); i++){
        count+= v[i];
    }
    return count;
}
