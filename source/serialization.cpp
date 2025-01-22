#include "serialization.h"

#include <fstream>
#include <sstream>

std::vector<int> load_map(const std::string& filename) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    return {};
  }

  std::string line;

  std::getline(file, line);

  std::vector<int> map;

  while (std::getline(file, line)) {
    std::stringstream stream(line);

    std::string buffer;

    for (auto index = 0; index < 8; ++index) {
      std::getline(stream, buffer, ' ');

      map.push_back(atoi(buffer.c_str()));
    }
  }

  return map;
}
