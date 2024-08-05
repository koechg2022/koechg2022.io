#include "prototypes"




unsigned long string_functions::string_length(const char *the_string, const char term) {
    unsigned long the_answer;
    for (the_answer = 0; the_string[the_answer] != term; the_answer++);
    return the_answer;
}




char string_functions::to_caps(const char c) {
    return (string_functions::is_lower(c)) ? (c - ('a' - 'A')) : c;
}

char string_functions::to_lower(const char c) {
    return (string_functions::is_caps(c)) ? (c + ('a' - 'A')) : c;
}

bool string_functions::is_caps(const char c) {
    return (c >= 'A' && c <= 'Z');
}

bool string_functions::is_lower(const char c) {
    return (c >= 'a' && c <= 'z');
}

bool string_functions::is_number(const char c) {
    return (c >= '0' && c <= '9');
}

bool string_functions::same_char(const char a, const char b, bool ignore_case) {
    return (ignore_case) ? string_functions::to_caps(a) == string_functions::to_caps(b) : a == b;
}

bool string_functions::same_string(const char *first, const char *second, bool ignore_case, const char term) {
    unsigned long index;
    for (index = 0;
        !string_functions::same_char(first[index], term, ignore_case) &&
        !string_functions::same_char(second[index], term, ignore_case) &&
        string_functions::same_char(first[index], second[index], ignore_case);
        index = index + 1
    );
    return string_functions::same_char(first[index], term, ignore_case) && string_functions::same_char(second[index], term, ignore_case);
}