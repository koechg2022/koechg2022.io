#include "misc_functions.h++"



const std::string misc_functions::get_current_time() {
    std::time_t now = std::time(nullptr);
    std::string the_answer = (const std::string) (std::ctime(&now));
    return the_answer.substr(0, the_answer.length() - 1);
}


int misc_functions::get_terminal_width() {
    int the_answer = 0;
    #if defined(unix_os)
        struct winsize the_window;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &the_window);
        the_answer = the_window.ws_col;
    #else
        CONSOLE_SCREEN_BUFFER_INFO cbsi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbsi);
        the_answer = cbsi.srWindow.Right - cbsi.srWindow.Left + 1;
    #endif
    return the_answer;
}


int misc_functions::get_terminal_height() {
    int the_answer = 0;
    #if defined(unix_os)
        struct winsize the_window;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &the_window);
        the_answer = the_window.ws_row;
    #else
        CONSOLE_SCREEN_BUFFER_INFO cbsi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbsi);
        the_answer = cbsi.srWindow.Bottom - cbsi.srWindow.Top + 1;
    #endif
    return the_answer;
}