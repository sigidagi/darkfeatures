#include "singleton.h"

#include <iostream>
class Test final : public Singleton<Test> {
  public:
    Test(token) { std::cout << "constructed" << std::endl; }
    ~Test() { std::cout << "destructed" << std::endl; }

    void use() const { std::cout << "in use" << std::endl; };
};

int main() {
    std::cout << "Entering main()" << std::endl;
    {
        auto const &t = Test::instance();
        t.use();
    }
    {
        auto const &t = Test::instance();
        t.use();
    }
    {
        auto t1 = Test({});
        t1.use();
    }

    std::cout << "Leaving main()" << std::endl;
}
