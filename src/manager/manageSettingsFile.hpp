#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>

namespace sf {
    class SettingsFile {
        std::string path;
        std::ifstream file;

        public:
            SettingsFile(std::string);
            ~SettingsFile(); // deconstructor
            void set_param();

    };
};