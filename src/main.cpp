
#include <cstddef>
#include <cstdio>
#include <curses.h>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <new>
#include <ostream>
#include <stdexcept>
#include <sys/types.h>
#include <vector>
#include <ncurses.h>
#include <glm/ext.hpp>

#define dispw(window, format, ...) \
    if(window){\
    wclear(window); \
    mvwprintw(window, 0, 0, format, __VA_ARGS__); \
    wrefresh(window);}

#define dispe(element, format, ...) \
    if(element){\
    wclear(element->window); \
    mvwprintw(element->window, 0, 0, format, __VA_ARGS__); \
    wrefresh(element->window);}


#include "player.hpp"

namespace wm
{
    class Window
    {
    private:
    public:
        WINDOW *window = nullptr;
        Window(const Window &) = delete;
        Window() : window(nullptr) {}
        Window(WINDOW *win) : window(win) {}
        Window(uint_fast16_t width, uint_fast16_t height, uint_fast16_t x_offset, uint_fast16_t y_offset) : window(nullptr)
        {
            window = subwin(stdscr, width, height, x_offset, y_offset);
        }

        glm::ivec2 size()
        {
            int x, y;
            getmaxyx(window, y, x);
            return {x, y};
        }

        operator glm::ivec2()
        {
            int x, y;
            getmaxyx(window, y, x);
            return {x, y};
        }

        operator WINDOW *()
        {
            return window;
        }

        Window &operator=(const Window &) = delete;

        Window &operator=(WINDOW *wind)
        {
            if (exists())
            {
                delwin(window);
            }
            window = wind;
            return *this;
        }

        bool exists()
        {
            return (window != nullptr && window != stdscr);
        }
        ~Window()
        {
            delwin(window);
        }
    };

    struct Element
    {
        Window window;
        size_t size = 0;
    };


    class Row
    {
    private:
        std::vector<Element *> vec;

    public:
        Row(){};
        Row(std::initializer_list<Element *> el) : vec(el){};
        ~Row() = default;

        Element *append(Element *el)
        {
            vec.push_back(el);
            return el;
        }
        Element *append(size_t relative_size)
        {
            auto el = new Element{Window(nullptr), relative_size};
            vec.push_back(el);
            return el;
        }
        void insert(Element *el, size_t position)
        {
            auto it = vec.begin();
            std::advance(it, position);
            if (it >= vec.end())
                throw std::out_of_range("Insert was out of range");
            vec.insert(it, el);
        }
        void insert(Element **el, size_t n, size_t position)
        {
            auto it = vec.begin();
            std::advance(it, position);
            if (it >= vec.end())
                throw std::out_of_range("Insert was out of range");
            vec.insert(it, el, el + n);
        }

        void resize(size_t idx, long pm)
        {
            vec[idx]->size += pm;
        }

        Element *operator[](size_t idx)
        {
            return vec[idx];
        }

        int refresh(long height, size_t height_offset)
        {
            uint x, y;
            getmaxyx(stdscr, y, x);
            if (height < 0)
            {
                height = y + height;
            }

            if (vec.size() == 0)
                return -1;
            size_t
                percent = 0,
                pos = 0;
            for (auto &&i : vec)
            {
                percent += i->size;
            }

            if (!percent)
                return -1;

            for (auto &&i : vec)
            {
                size_t width = round((double)(x * (i->size)) / percent);
                if (width + pos > x)
                {
                    width = x - pos;
                }

                i->window = subwin(stdscr, height, width, height_offset, pos);
                if (height > 2)
                    box(i->window, 0, 0);
                wrefresh(i->window);
                pos += width;
            }
            return OK;
        }
    };

    enum RowType : u_char
    {
        UNDEFINED,
        RELATIVE,
        PERCENT,
        ABSOLUTE,
    };

    struct RowElement
    {
        Row row;
        uint size = 1;
        RowType type = RELATIVE;
        uint calculate_height(size_t abs_area, uint relative_percent = 1)
        {
            uint x, y;
            getmaxyx(stdscr, y, x);
            double idk;
            switch (type)
            {
            case RowType::UNDEFINED:
            case RowType::RELATIVE:
                idk = round((double)((y - abs_area) * size) / relative_percent);
                // logvar(idk)
                // logvar(y-abs_area)
                // logvar((double) size/relative_percent)
                return idk;
            case PERCENT:
                return round((double)size / 100);
            case ABSOLUTE:
                return size;
            default:
                return size;
            }
        }
    };
    // aka just a bunch of rows
    class WindowManager
    {
    private:
        std::vector<RowElement *> rows;

    public:
        void append(RowElement *re)
        {
            rows.push_back(re);
        }

        void refresh()
        {
            uint x, y;
            getmaxyx(stdscr, y, x);
            size_t pos = 0, percent = 0, absolute_space = 0;
            for (auto &&i : rows)
            {
                if (i->type == RowType::RELATIVE || i->type == RowType::UNDEFINED)
                {
                    percent += i->size;
                }
                if (i->type == RowType::ABSOLUTE)
                {
                    absolute_space += i->size;
                }
            }
            for (auto &&i : rows)
            {
                size_t height = i->calculate_height(absolute_space, percent);
                if (height + pos > y)
                {
                    height = y - pos;
                }
                i->row.refresh(height, pos);
                pos += height;
            }
        }

        void resize(size_t idx, long pm)
        {
            rows[idx]->size += pm;
        }

        RowElement *operator[](size_t idx)
        {
            return rows[idx];
        }

        size_t size()
        {
            return rows.size();
        }

        WindowManager() {}
        WindowManager(std::initializer_list<RowElement *> ls) : rows(ls) {}
        ~WindowManager() {}
    };
} // namespace wm

int startup()
{
    initscr();
    cbreak();             // Disable line buffering
    noecho();             // Don't echo user input
    keypad(stdscr, TRUE); // Enable function keys
    refresh();
    return 0;
}

int stop()
{
    endwin();
    return 0;
}

enum InputMode : u_char
{
    COMMAND,
    TEXT,
};
InputMode mode = InputMode::COMMAND;

Player p;


int main(int argc, char const *argv[])
{
    logvar("Starting program");

    logvar("test/")
    try{
        p.add("test/");
    }
    catch(std::runtime_error er){
        err(er.what());
    }


    startup();
    wm::WindowManager w;

    w.append(new wm::RowElement{wm::Row(), 1, wm::RowType::ABSOLUTE});
    w.append(new wm::RowElement{wm::Row(), 2, wm::RowType::RELATIVE});
    w.append(new wm::RowElement{wm::Row(), 1, wm::RowType::ABSOLUTE});

    auto log_element = w[0]->row.append(2);

    // wm[0]->row.append(7);

    auto playlist_element = w[1]->row.append(6);
    w[1]->row.append(3);
    w[1]->row.append(2);

    wm::Element *text_element = w[2]->row.append(1);
    

    std::string input;

    while (true)
    {
        w.refresh();
        refresh();

        //changes every refresh
        {
            auto prefix = "\n  ";
            std::string files =  prefix + p.to_string(prefix); 
            wclear(playlist_element->window);
            mvwprintw(playlist_element->window, 0, 0, "%s", files.c_str());
            box(playlist_element->window, 0, 0);
            wrefresh(playlist_element->window);
        }

        auto c = wgetch(stdscr);
        
        switch (mode)
        {
        case InputMode::COMMAND:
        {
            if (c == 'q')
            { // resize event
                goto exit;
            }
            else if (c == 'i')
            {
                mode = InputMode::TEXT;
                input = "";
                dispe(text_element, ":%s", input.c_str())
                break;
            }
            else
            {
                dispe(log_element, "(%i): %c", c, c)
            }
        }
        break;
        case InputMode::TEXT:
        {
            switch (c)
            {
            case 10:
                // enter
                dispe(log_element, "input: %s", input.c_str())

                // do something with the input

                input = "";
                break;

            case 263:
                if (input.length() > 0)
                    input.pop_back();
            case 410:
                dispe(text_element, ":%s", input.c_str());
                break;

            case 27:
                mode = InputMode::COMMAND;
                break;

            default:
                input += c; // 263 //27???
                dispe(text_element, ":%s", input.c_str());
            }
        }
        }
    }

#undef disp_input

exit:
    stop();
    return 0;
}
