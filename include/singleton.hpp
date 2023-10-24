#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <string>

template <typename T> class Singleton {
  public:
    static T &instance();

    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton) = delete;

  protected:
    struct token {};
    Singleton() {}
};

template <typename T> T &Singleton<T>::instance() {
    static T instance{token{}};
    return instance;
}

#endif /* SINGLETON_H */
