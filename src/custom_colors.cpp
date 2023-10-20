#include <ncurses.h>
#include <iostream>

int main() {
    initscr(); // Initialize ncurses
    start_color(); // Enable color support

    // Define custom colors
    init_color(100, 500, 0, 0); // Define a custom color with RGB values (100, 0, 0)
    init_color(101, 0, 0, 1000); // Define a custom color with RGB values (100, 0, 0)

    init_pair(1, COLOR_WHITE, COLOR_BLUE); // Initialize a color pair with default colors (white text on blue background)
    init_pair(2, 100, 101); // Initialize a color pair using the custom color (color 100) on a black background

    attron(COLOR_PAIR(1));
    mvprintw(10, 10, "Default Color Pair"); // Use the default color pair
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(2));
    mvprintw(12, 10, "Custom Color Pair"); // Use the custom color pair
    attroff(COLOR_PAIR(2));

    refresh();
    getch(); // Wait for user input
    endwin(); // Clean up and exit ncurses

    return 0;
}