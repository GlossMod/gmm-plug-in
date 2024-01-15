#include "Config.h"

std::vector<std::string> readLinesFromFile(char *filename)
{
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open())
    {
        PrintBottomCentre("�޷����ļ�");
    }

    std::string line;
    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    file.close();
    return lines;
}