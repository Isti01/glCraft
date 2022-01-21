#include "Persistence.h"

#include "../Util/Util.h"

Persistence::Persistence(std::string path) : path(std::move(path)) {
  serialize();
}

void Persistence::serialize() {
  Ref<std::string> data = Util::readBinaryFile(path);
  if (data == nullptr) {
    // todo failed
  }
  // todo
}

void Persistence::deserialize() {
  // todo
}

Persistence::~Persistence() {
  deserialize();
}
