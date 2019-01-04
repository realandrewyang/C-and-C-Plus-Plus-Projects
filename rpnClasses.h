#ifndef RPNCLASSES_H_INCLUDED
#define RPNCLASSES_H_INCLUDED

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/mouse.h>
#include <cmath>
#include <cstring>
#include <apstring.h>
#include <iostream>

#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)

using namespace std;

enum angleType {radians, degrees};
const unsigned int N = 8;
const unsigned int limit = 20;

struct Node
{
    float num;
    Node *next;
};

class Button
{
  public:

    // constructors/destructor
    Button(); // construct button with empty values
    Button(int x, int y, int w, int l, char text[],
           ALLEGRO_COLOR back, ALLEGRO_COLOR fore, ALLEGRO_FONT *font); // construct button with parameters
    ~Button(); // destructor

    // accessors
    int getX();
    int getY();
    int getW();
    int getL();
    apstring getText();
    ALLEGRO_COLOR getBack();
    ALLEGRO_COLOR getFore();
    ALLEGRO_FONT* getFont();
    bool validCoord(int SCREEN_W, int SCREEN_H);
    bool pressed();

    // modifiers
    void setX(int x);
    void setY(int y);
    void setW(int w);
    void setL(int l);
    void setText(char text[]);
    void setText(char text);
    void setText(string text);
    void addText(char text[]);
    void addText(apstring text);
    void addText(char text);
    void setBackColour(ALLEGRO_COLOR colour);
    void setForeColour(ALLEGRO_COLOR colour);
    void setFont(ALLEGRO_FONT *font);
    void press();
    void unPress();
    int draw(int SCREEN_W, int SCREEN_H, int thickness);

  private:

    int xRef;
    int yRef;
    int width;
    int length;
    ALLEGRO_COLOR backColour;
    ALLEGRO_COLOR foreColour;
    ALLEGRO_FONT *displayFont;
    bool isPressed;
    apstring displayText;
};

class Box: public Button
{
  public:

    int clear(int SCREEN_W, int SCREEN_H, int thickness);
    int print(int SCREEN_W, int SCREEN_H, int margin);
    int showLine(int SCREEN_W, int SCREEN_H, int margin);
};

class Stack
{
  public:

    // constructors/destructor
    Stack();                            // construct empty stack
    ~Stack();

    // accessors
    bool isEmpty();
    bool isSingle();
    float pop();
    int size();
    void show();

    // mutators
    void push(float c);

  private:
    Node *head;
};

class rpnStack: public Stack
{
  public:

    float calculate(char o);
    float calculate(char o, angleType unit);
    void pushFromText(const apstring &line, int lineNum);
};

#endif // RPNCLASSES_H_INCLUDED
