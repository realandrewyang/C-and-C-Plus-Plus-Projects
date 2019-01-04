#ifndef RPNMODULE_H_INCLUDED
#define RPNMODULE_H_INCLUDED

#include "rpnClasses.h"
#include <apvector.h>

const char operatorList[] = "+-*/^LldSsCcTt";
const int J = 8;
const float e = 2.71828182845904523536, pi = 3.14159265358979323846, phi = 1.61803398874989484820;

// Prototyping functions
bool isOperator(char a);
double operate(double a, double b, char operation);
double func(double a, char operation);
double trigFunc(double a, char operation, angleType unit);
double constant(apstring name);
int factorial(int n);
int checkScreen(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *event_queue);
int loadTextBox(Box textBox[], int x, int y, int size, int SCREEN_W, int SCREEN_H, ALLEGRO_FONT *font);
int loadButtons(Button key[][J], int stdSize, int largeSize, int SCREEN_W, int SCREEN_H,
                ALLEGRO_FONT *font, char keyFileName[]);
int count(apstring object, char ch);
bool onButton(int x, int y, Button someButton);
bool inString(char str[], char check);
bool validNum(const apstring &text);
int process(Button key[][8], Box text[], Button *&currentButton, Box *&currentLine, rpnStack &myStack, ALLEGRO_DISPLAY *display,
            ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_MOUSE_STATE &mouse_position, int stdSize, bool &newLine,
            angleType angle, int SCREEN_W, int SCREEN_H, int margin);
int sum(const apvector<int> &v);

#endif // RPNMODULE_H_INCLUDED
