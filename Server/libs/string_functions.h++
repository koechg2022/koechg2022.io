

#include "../headers/included.h++"




#if not defined(_STRING_FUNC_)
    #define _STRING_FUNC_
    

    namespace string_functions {

        namespace {
            const struct timeval default_timeout = (const struct timeval) {0, 100000};
        }

        const std::string DIRECTORY = "Directory", FILE = "File";

        /**
            @brief Checks if the character passed in is uppercase.
            @param c (const char) : the character to check it's case.
            @returns `true` if `c` is capital case, `false` otherwise.
        */
        bool is_caps(const char c);
        
        
        /**
            @brief Check if the character passed in is lowercase.
            @param c (const char) : the character to check it's case.
            @returns `true` if `c` is lowercase, `false` otherwise.
        */
        bool is_lower(const char c);
        

        /**
            @brief Check if the char passed in is a letter (lowercase or capital case)
            @param c (const char) : the character to check if it's a letter or not.
            @returns `true` if the char passed in is lowercase or uppercase, false otherwise.
        */
        bool is_letter(const char c);
        

        /**
            @brief Check if the char passed in is a number or not.
            @param c (const char) : the character to check if it's a number or not.
            @returns `true` if the character passed in is a number, false otherwise.
        */
        bool is_number(const char c);


        /**
            @brief Convert a char to uppercase, if it is a letter that is lowercase.
            @param c (const char) the character to be converted into a capital case letter.
            @returns the capital version of `c`
        */
        char to_caps(const char c);


        /**
            @brief Convert a char to lowercase, if it is a letter that was uppercase.
            @param c (const char) : the character to be converted to a lower case letter.
            @returns the lower case version of `c`
        */
        char to_lower(const char c);


        /**
            @brief Check if the two chars passed in, `a` and `b` are the same character or not.
            @param a (const char) : one of the two characters to be compared.
            @param b (const char) : the other of the two characters to be compared.
            @param ignore_case (bool) : (Defaults to true) A flag that specifies that if the characters are both 
                letters, case should be ignored.
            @returns `true` if `a` and `b` are the same character 
                (as depending on how `ignore_case` is defined)
                and `false` otherwise.
        */
        bool same_char(const char a, const char b, bool ignore_case = true);


        /**
            @brief Check if the two strings (const char*) passed 
                in are the same string or not and return `true` if they are,
                `false` if they aren't.
            @param first (const char*) : one of the strings to be compared.
            @param second (const char*) : the other of the two strings to be compared.
            @param ignore_case (bool) : (Defaults to true) A flag to specify whether 
                letter cases should be ignored or not.
            @param term (const char) : (Defaults to NULL (\0)) 
                The terminating character to be looked for in each string being compared.
            @returns `true` if `first` and `second` are the same string, `false` if they are not.
        */
        bool same_string(const char* first, const char* second, bool ignore_case = true, const char term = '\0');
        
        
        /**
            @brief Check if the two strings (const char*) passed 
                in are the same string or not and return `true` if they are,
                `false` if they aren't.
            @param first (const char*) : one of the strings to be compared.
            @param second (const char*) : the other of the two strings to be compared.
            @param ignore_case (bool) : (Defaults to true) A flag to specify whether 
                letter cases should be ignored or not.
            @returns `true` if `first` and `second` are the same string, `false` if they are not.
        */
        bool same_string(const std::string first, const std::string second, bool ignore_case = true);


        /**
            @brief Check if all the characters in the `char*` are numbers or not.
            @param nums `const char*` The string of characters to check.
            @param term (Defaults to `'\0'`). This is the last character to search to
            in `nums`.
        */
        bool all_numbers(const char* nums, const char term = '\0');


        /**
            @brief Check if all the characters in the `char*` are letters or not.
            @param the_string `const char*` The string of characters to check.
            @param term (Defaults to `'\0'`). This is the last character to search to
            in `the_string`.
        */
        bool all_letters(const char* the_string, const char term = '\0');


        /**
            @brief Get the length of the string passed in.
            @param the_string (const char*) : The string whose length is to be determined
            @param term (const char) : The terminating character to look for at the end of the_string. Defaults to NULL (`\0`)
            @returns the number of characters in `the_string` not including the terminating character.
        */
        unsigned long string_length(const char* the_string, const char term = '\0');
        

        /**
            * @brief get keyboard input.
            * @param prompt (const std::string) Read keyboard input.
            * @returns the keyboard input.
        */
        std::string get_input(const std::string prompt = "");

        
        /**
        *   @brief Get the uppercase of the `std::string` passed in.
        *   @param the_string `std::string`. The string to get it's uppercase of.
        *
        *   @note this does not modify the `the_string` that gets passed in.
        *
        *   @returns the uppercase version of `the_string`
        */
        std::string get_uppercase(const std::string the_string);

        
        /**
        *   @brief Get the uppercase of the `std::string` passed in.
        *   @param the_string `std::string`. The string to get it's uppercase of.
        *
        *   @note this does modify the `the_string` that gets passed in.
        */
        void get_uppercase(std::string& the_string);

        
        /**
        *   @brief Get the lowercase of the `std::string` passed in.
        *   @param the_string `std::string`. The string to get it's lowercase of.
        *
        *   @note this does not modify the `the_string` that gets passed in.
        *
        *   @returns the lowercase version of `the_string`
        */
        std::string get_lowercase(const std::string the_string);

        
        /**
        *   @brief Get the lowercase of the `std::string` passed in.
        *   @param the_string `std::string`. The string to get it's lowercase of.
        *
        *   @note this does modify the `the_string` that gets passed in.
        */
        void get_lowercase(std::string& the_string);

        
        /**
        *
        *   @brief Check if the keyboard has input available or not.
        *
        *
        *   @returns `true` if there is keyboard input, `false` otherwise.
        *
        */
        bool has_keyboard_input();


        /**
            @brief Remove excess strings at the beginning and end of
            `the_string`.

            @param the_string (std::string&) : The string to be stripped.
            @param to_remove (const std::string) : The string to be stripped.
         */
        void strip(std::string& the_string, const std::string to_remove);


        /**
            @brief Replaces all instances of `to_replace` with `the_string`
            @param the_string (`std::string&) : The string to have parts of itself replaced.
            @param to_replace (`const std::string`) : The string to replace within `the_string`.
            @param replace_with (`const std::string`) : The string to replace `to_replace` with.
         */
        void replace_all(std::string& the_string, const std::string to_replace, const std::string replace_with);


        std::map<std::string, std::string> get_file_data(const std::string& file_name);

        
        std::map<std::string, std::vector<std::string> > get_directory_content(const std::string& file_name);


        template <typename data_> bool contains(std::map<std::string, data_>& to_search, const std::string& to_find_key, bool ignore_case = true) {
        for (auto pair = to_search.begin(); pair != to_search.end(); pair++) {
            if (same_string(pair->first, to_find_key, ignore_case)) {
                return true;
            }
        }
        return false;
    }

    }
    

#endif