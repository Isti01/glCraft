#pragma once

#include "../glCraft.h"

class Persistence {
  std::string path;

  void deserialize();
  void serialize();

public:
  explicit Persistence(std::string path);


  ~Persistence();
};