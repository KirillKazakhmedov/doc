#include "ThreadPool.hpp"
#include "Task.hpp"

#include <stdlib.h>
#include <chrono>
#include <algorithm>

using namespace std::chrono_literals;

// Transform string to upper case
std::string worker(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
  return str;
}

class Test {
public:
  int Sum(int a, int b) { return a + b; }
};

int main(int argc, char** argv)
{
  core::ThreadPool pool(6, 1000);
  {
    // Execute worker function
    std::vector<std::future<std::string>> results;
    for (int j = 0; j < 100; ++j) {
      core::Task task(core::TaskPriority::Medium);
      results.emplace_back(task.assign(worker, "From thread pool " + std::to_string(j)));
      pool.push_task(task);
    }
    for (auto& result : results) {
      std::cout << result.get() << std::endl;
    }
    // Execute worker function
  }
  std::this_thread::sleep_for(2s);

  {
    // Execute class method
    std::vector<std::future<int>> results;
    Test test;
    for (int j = 0; j < 100; ++j) {
      core::Task task(core::TaskPriority::Medium);
      results.emplace_back(task.assign(&test, &Test::Sum, j, j + 1));
      pool.push_task(task);
    }
    for (auto& result : results) {
      std::cout << result.get() << std::endl;
    }
    // Execute class method
  }

  pool.join_all();
  return 0;
}
