#include <iostream>
#include <string>
#include <utility>

void print() {
    std::cout << std::endl;
}

template<typename T, typename... Args>
void print(T&& first, Args&&... rest) {
    std::cout << std::forward<T>(first) << " ";
    print(std::forward<Args>(rest)...);
}

int main() {
    int x = 42;
    std::string name = "Tamim";
    double pi = 3.14159;

    print("Hello", name);
    print("Value of x:", x);
    print("Pi is", pi, "roughly");
    print(1, 2.2, 'A', std::string("done"));

    return 0;
}
