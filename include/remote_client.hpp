#ifndef REMOTE_CLIENT_HPP
#define REMOTE_CLIENT_HPP

#include <launchdarkly/client_side/client.hpp>
#include <memory>
#include <string>

namespace ld = launchdarkly;

namespace dark {

struct User {
    std::string key;
    std::string name;
    std::string location;
    std::string work;
    std::string city;
    int born;
    bool male;
};

class RemoteClient {
  private:
    RemoteClient(); // Disallow instantiation outside of the class.
    std::shared_ptr<ld::client_side::Client> client_ = nullptr;

  public:
    RemoteClient(const RemoteClient &) = delete;
    RemoteClient &operator=(const RemoteClient &) = delete;
    RemoteClient(RemoteClient &&) = delete;
    RemoteClient &operator=(RemoteClient &&) = delete;

    bool init(const std::string &key);
    std::shared_ptr<ld::client_side::Client> client() const;

    static auto &instance() {
        static RemoteClient remote;
        return remote;
    }
};

} // namespace dark

#endif /* REMOTE_CLIENT_HPP */
