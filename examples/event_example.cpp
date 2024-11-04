#include "Event.hpp"
#include "EventHandler.hpp"

#include <chrono>

using namespace std::chrono_literals;

void OnEvent(const void* psender, double val) { std::cout << val << std::endl; }

class A {
public:
  void OnMessage(const void* psender, std::string str) { std::cout << str << std::endl; }
};

class B {
public:
  B(A& a) { message_event += core::EventHandler::bind(&a, &A::OnMessage); }

  void notify_subscribers() { message_event.notify(this, "Hello from sync B!"); }

  void async_notify_subscribers()
  {
    auto results = message_event.notify_async(this, "Hello from async B!");
    for (auto& result : results) {
      result.wait();
    }
  }

private:
  core::Event<std::string> message_event;
};

int main(int argc, char** argv)
{
  {
    // Function event handler
    core::Event<double> ev;
    ev += core::EventHandler::bind(&OnEvent);
    double val = 5.6;
    ev.notify(nullptr, val);
    ev.notify(nullptr, 10.5);
    auto results = ev.notify_async(nullptr, 4.6);
    for (auto& result : results) {
      result.wait();
    }
    // Function event handler
  }
  std::this_thread::sleep_for(2s);

  A a;
  {
    // Class method event handler
    core::Event<std::string> ev;
    ev += core::EventHandler::bind(&a, &A::OnMessage);
    ev.notify(nullptr, "My message");
    // Class method event handler
  }
  std::this_thread::sleep_for(2s);

  {
    // Interclass notification
    B b(a);
    b.notify_subscribers();
    b.async_notify_subscribers();
    // Interclass notification
  }

  return 0;
}
