#include "Trace.h"

#include <utility>

Trace* Trace::instancePtr = nullptr;

Trace::Trace(const std::string& traceFile) : traceFile(traceFile) {
  this->traceFile << R"({"otherData": {},)" << '\n' << R"("traceEvents":[)" << std::endl;
}

void Trace::start(const std::string& traceFile) {
  assert(instancePtr == nullptr);
  instancePtr = new Trace(traceFile);
}

void Trace::end() {
  assert(instancePtr != nullptr);
  delete instancePtr;
  instancePtr = nullptr;
}

void Trace::writeEvent(std::string eventName, uint64_t start, uint64_t end) {
  assert(instancePtr != nullptr);

  if (eventCount > 0) {
    traceFile << ",\n";
  }
  eventCount++;
  std::replace(eventName.begin(), eventName.end(), '"', '\'');

  traceFile << "{"
            << R"("cat":"function",)"
            << R"("dur":)" << (end - start) << ',' << R"("name":")" << eventName << R"(",)"
            << R"("ph":"X",)"
            << R"("pid":0,)"
            << R"("tid":0,)"
            << R"("ts":)" << start << "}";
  traceFile.flush();
}
Trace::~Trace() {
  this->traceFile << "]\n}" << std::endl;
}

Trace::TraceTimer::TraceTimer(const std::string& name)
    : startNewTrace(instancePtr == nullptr),
      startTime(getTimestamp()),
      name(name) {
  if (startNewTrace) {
    std::cerr << "Started a new trace session with name " << name << std::endl;
    Trace::start(name + ".json");
  }
}

uint64_t Trace::TraceTimer::getTimestamp() const {
  return std::chrono::time_point_cast<TimeUnit>(Clock::now()).time_since_epoch().count();
}

Trace::TraceTimer::~TraceTimer() {
  const bool isTracerActive = instancePtr != nullptr;
  assert(isTracerActive);
  if (!isTracerActive)
    return;

  instancePtr->writeEvent(name, startTime, getTimestamp());
}
