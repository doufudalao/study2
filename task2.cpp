#include <iostream>
#include <string>

int main() {
    const std::string banner = R"(
  +--------------------------------+
  |                                |
  |   INFINITE PROGRESS            |
  |                                |
  |              /\                |
  |             /  \               |
  |            / /\ \              |
  |           /_/  \_\             |
  |              8                 |
  |                                |
  |   LEARN  |  BUILD  |  GROW      |
  +--------------------------------+
)";

    std::cout << banner;
    std::cout << "Welcome to my Ubuntu terminal!\n";
    std::cout << "Every day, one step forward.\n";

    return 0;
}