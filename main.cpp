/****************************************************
* Name: Andrew Yang                                 *
* Teacher: Mr. Creelman                             *
* Course: ICS4U                                     *
* Date: March 19 2018                               *
* Program Name: RPN Calculator                      *
*                                                   *
* Purpose: to display an RPN calculator with        *
*          a GUI and the following operations:      *
*          - addition                               *
*          - subtraction                            *
*          - multiplication                         *
*          - divison                                *
*          - basic trigonometic functions           *
*          - exponents, logarithms, and factorials  *
*****************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/mouse.h>
#include "rpnModule.h"
#include "rpnClasses.h"

#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define SELECTED al_map_rgb(128, 128, 255)

const int SCREEN_H = 400 + 1;
const int SCREEN_W = 460 + 1;

int main(int argc, char *argv[]){

    // Declare and initialize pointers and variables
    ALLEGRO_DISPLAY *display = nullptr;
    ALLEGRO_EVENT_QUEUE *event_queue = nullptr;
    ALLEGRO_MOUSE_STATE mouse_position;

    int errorCheck = 0;
    bool newLine = false;
    const int stdSize = 50, largeSize = 80, margin = 20;
    char input[30] = "", keyFileName[8] = "key.txt";
    float num1 = 0, num2 = 0;
    angleType angle = radians;
    Button key[4][8];
    Button *currentButton = &key[0][0];
    Box text[4];
    Box *currentLine = &text[3];
    rpnStack myStack;

    // Initialize Allegro
    al_init();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon();
    al_install_mouse(); // install mouse driver

    // Load fonts
    ALLEGRO_FONT *font = al_load_ttf_font("Ikaros-Regular.otf", 20, 0);
    ALLEGRO_FONT *boxFont = al_load_ttf_font("COURIER.TTF", 20, 0);

    // Load GUI
    display = al_create_display(SCREEN_W, SCREEN_H);
    al_set_window_title(display, "RPN Calculator");
    al_clear_to_color(WHITE);
    event_queue = al_create_event_queue();

    // Check if GUI has been loaded properly
    errorCheck = checkScreen(display, event_queue);
    if (errorCheck){
        cerr << "Failed to load screen." << endl;
        return -1;
    }

    // Register event queue
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));

    // Initialize and display text boxes
    errorCheck = loadTextBox(text, 1, 1, stdSize, SCREEN_W, SCREEN_H, boxFont);
    if (errorCheck){
        return -1;
    }

    // Initialize and display buttons
    errorCheck = loadButtons(key, stdSize, largeSize, SCREEN_W, SCREEN_H, font, keyFileName);
    if (errorCheck){
        return -1;
    }

    // Check for mouse actions
    errorCheck = process(key, text, currentButton, currentLine, myStack, display,event_queue,
                         mouse_position, stdSize, newLine, angle, SCREEN_W, SCREEN_H, margin);
    if (errorCheck){
        return -1;
    }

    return 0;
}
