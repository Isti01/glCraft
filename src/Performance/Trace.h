#pragma once

#include <utility>

#include "../glCraft.h"


class Trace {
private:
  static Trace* instancePtr;

  std::ofstream traceFile;
  uint64_t eventCount = 0;

  Trace(const std::string& traceFile);
  ~Trace();

public:
  static void start(const std::string& traceFile);
  static void end();
  void writeEvent(std::string eventName, uint64_t start, uint64_t end);

  class TraceTimer {
    using TimeUnit = std::chrono::microseconds;
    using Clock = std::chrono::high_resolution_clock;

    bool startNewTrace;
    uint64_t startTime;
    const std::string name;

    [[nodiscard]] uint64_t getTimestamp() const;

  public:
    explicit TraceTimer(const std::string& name);
    ~TraceTimer();
  };
};

#ifdef ENABLE_TRACING
#define CONCAT_(prefix, suffix) prefix##suffix
#define CONCAT(prefix, suffix) CONCAT_(prefix, suffix)
#define START_TRACE(traceFile) Trace::start(traceFile)
#define END_TRACE() Trace::end()
#define TRACE_SCOPE(name) Trace::TraceTimer CONCAT(traceTimer, __COUNTER__)(name)
#define TRACE_FUNCTION() TRACE_SCOPE(__FUNCSIG__)
#else
#define START_TRACE(name)
#define END_TRACE()
#define TRACE_SCOPE(name)
#define TRACE_FUNCTION()
#endif