/********************************************************
* Name: Andrew Yang                                     *
* Teacher: Mr. Creelman                                 *
* Course: ICS4U                                         *
* Date: March 19 2018                                   *
* File Name: rpnModule.cpp                              *
*                                                       *
* Purpose - to supplant classes for the RPN Calculator. *
*********************************************************/

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/mouse.h>
#include <cstring>
#include <apstring.h>
#include <apstring.cpp>
#include "rpnClasses.h"
#include "rpnModule.h"

#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)

using namespace std;

// Constructor without parameters
Button::Button()
{
    xRef = 0;
    yRef = 0;
    width = 0;
    length = 0;
    displayText = "";
    backColour = WHITE;
    foreColour = BLACK;
    displayFont = nullptr;
    isPressed = false;
}

// Constructor with parameters
Button::Button(int x, int y, int w, int l, char text[], ALLEGRO_COLOR back, ALLEGRO_COLOR fore, ALLEGRO_FONT *font){
    xRef = x;
    yRef = y;
    width = w;
    length = l;
    displayText = text;
    backColour = back;
    foreColour = fore;
    displayFont = font;
    isPressed = false;
}

// Desctructor
Button::~Button()
{

}

int Button::getX()
{
    return xRef;
}

int Button::getY()
{
    return yRef;
}

int Button::getW()
{
    return width;
}

int Button::getL()
{
    return length;
}

apstring Button::getText()
{
    return displayText;
}

ALLEGRO_COLOR Button::getBack()
{
    return backColour;
}

ALLEGRO_COLOR Button::getFore()
{
    return foreColour;
}

ALLEGRO_FONT* Button::getFont()
{
    return displayFont;
}

// Determines if a certain coordinate lies within the screen
bool Button::validCoord(int SCREEN_W, int SCREEN_H){
    if (xRef < 0 || xRef + width > SCREEN_W){
        cerr << "Illegal x-coordinate or width." << endl;
        return false;
    } else if (yRef < 0 || yRef + length > SCREEN_H){
        cerr << "Illegal y-coordinate or length." << endl;
        return false;
    } else {
        return true;
    }
}

bool Button::pressed()
{
    return isPressed;
}

void Button::setX(int x)
{
    xRef = x;
}

void Button::setY(int y)
{
    yRef = y;
}

void Button::setW(int w)
{
    width = w;
}

void Button::setL(int l)
{
    length = l;
}

void Button::setText(char text[])
{
    if (strlen(text) <= limit){
        displayText = text;
    }
}

void Button::setText(char text)
{
    displayText = text;
}

void Button::setText(string text)
{
    if (text.size() <= limit){
        displayText = text.c_str();
    }
}

void Button::addText(char text[])
{
    if (strlen(text) + displayText.length() <= limit){
        displayText+= text;
    }
}

void Button::addText(apstring text)
{
    if (text.length() + displayText.length() <= limit){
        displayText+= text;
    }
}

void Button::addText(char text)
{
    if (1 + displayText.length() <= limit){
        displayText+= text;
    }
}

void Button::setBackColour(ALLEGRO_COLOR colour)
{
    backColour = colour;
}

void Button::setForeColour(ALLEGRO_COLOR colour)
{
    foreColour = colour;
}

void Button::setFont(ALLEGRO_FONT *font)
{
    displayFont = font;
}

void Button::press()
{
    isPressed = true;
}

void Button::unPress()
{
    isPressed = false;
}

// This function draws a button to the screen
int Button::draw(int SCREEN_W, int SCREEN_H, int thickness)
{
    if (validCoord(SCREEN_W, SCREEN_H)){
        al_draw_filled_rectangle(xRef + 1, yRef + 1, xRef + width - 2, yRef + length - 2, backColour);
        al_draw_rectangle(xRef, yRef, xRef + width, yRef + length, foreColour, thickness);
        al_draw_text(displayFont, foreColour, xRef + width / 2, yRef + length / 3, ALLEGRO_ALIGN_CENTRE, displayText.c_str());
        return 0;
    } else {
        return -1;
    }
}

// This function clears a box
int Box::clear(int SCREEN_W, int SCREEN_H, int thickness)
{
    if (validCoord(SCREEN_W, SCREEN_H)){
        al_draw_filled_rectangle(getX() + 1, getY() + 1, getX() + getW() - 1, getY() + getL() - 1, getBack());
        al_draw_rectangle(getX(), getY(), getX() + getW(), getY() + getL(), getFore(), thickness);
        return 0;
    } else {
        return -1;
    }
}

// This function displays the text in the text box
int Box::print(int SCREEN_W, int SCREEN_H, int margin)
{
    if (validCoord(SCREEN_W, SCREEN_H)){
        al_draw_text(getFont(), getFore(), SCREEN_W - margin, getY() + getL() / 3, ALLEGRO_ALIGN_RIGHT, getText().c_str());
        return 0;
    } else {
        return -1;
    }
}

// This function prints a '>' symbol in the selected text box
int Box::showLine(int SCREEN_W, int SCREEN_H, int margin)
{
    if (validCoord(SCREEN_W, SCREEN_H)){
        al_draw_text(getFont(), getFore(), margin, getY() + getL() / 3, ALLEGRO_ALIGN_LEFT, ">");
        return 0;
    } else {
        return -1;
    }
}

Stack::Stack()
{
    head = nullptr;
}

Stack::~Stack()
{
    head = nullptr;
}

bool Stack::isEmpty()
{
    if (!head){
        return true;
    }
    return false;
}

bool Stack::isSingle()
{
    if (!head->next){
        return true;
    }
    return false;
}

// This function pops the top of a stack
float Stack::pop()
{
    if (!isEmpty()){
        float d = head->num;
        head = head->next;
        return d;
    }
    cerr << "Cannot pop an empty stack." << endl;
    return ' ';
}

// This function determines the size of a stack
int Stack::size()
{
    int count = 0;
    for (Node *t = head; t; t = t->next){
        count++;
    }
    return count;
}

// This function prints the entire stack
void Stack::show()
{
    cout << "Stack: ";
    for (Node *t = head; t; t = t->next){
        cout << t->num << " ";
    }
    cout << endl;
}

// This function pushes a float onto the stack
void Stack::push(float d)
{
    Node *t = new Node;
    t->num = d;
    t->next = head;
    head = t;
}

// This function performs a calculation on the top one or two member of the stack
float rpnStack::calculate(char o)
{
    float returnValue = 0.0;
    if (!isEmpty()){
        if (inString("!nL", o)){
            returnValue = func(pop(), o);
        } else if (!isSingle() && inString("+-x/^g", o)){
            returnValue = operate(pop(), pop(), o);
        }
    }

    push(returnValue);
    return returnValue;
}

float rpnStack::calculate(char o, angleType unit)
{
    float returnValue = 0.0;
    if (!isEmpty() && inString("sScCtT", o)){
        returnValue = trigFunc(pop(), o, unit);
    }

    push(returnValue);
    return returnValue;
}

// This function retrieves a float by parsing text
void rpnStack::pushFromText(const apstring &line, int lineNum)
{
    if (isEmpty() || lineNum == size() && validNum(line)){
        if (!isEmpty()){
            pop();
        }
        if (validNum(line)){
            push(stof(line.c_str()));
        }
    }
}
