#include <curses.h>
#include <ncurses.h>

int main() {
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Get screen dimensions
    int height, width;
    getmaxyx(stdscr, height, width);

    // Define the coordinates for the box
    int starty = height / 2 - 5; // Top-left corner y-coordinate
    int startx = width / 2 - 20; // Top-left corner x-coordinate
    int endy = height / 2 + 5;   // Bottom-right corner y-coordinate
    int endx = width / 2 + 20;   // Bottom-right corner x-coordinate

    auto win = subwin(stdscr, 6, 6, 6, 6);

    // Draw the box
    box(win, 0, 0); // Draw a box around the screen
    wrefresh(win);
    // Add a title to the box
    mvprintw(starty, (width - 10) / 2, "My Box");

    // Refresh the screen to display the box
    refresh();

    // Wait for user input
    getch();

    // Terminate ncurses
    endwin();

    return 0;
}
