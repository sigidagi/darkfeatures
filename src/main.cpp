#include <launchdarkly/context_builder.hpp>
// #include <launchdarkly/client_side/flag_change_event.hpp>
#include <boost/asio.hpp>

#include <cstring>
#include <iostream>

#include "enum.h"
#include "remote_client.hpp"
#include "spdlog/fmt/fmt.h"

// Set MOBILE_KEY to your LaunchDarkly mobile key.
#define MOBILE_KEY "mob-6dd0eb29-3491-4e4c-9fc6-ed13152a28f8"
// #define MOBILE_KEY "mob-d7ea5f46-4b28-4fdf-a457-d50a3c98bf04"

// Set FEATURE_FLAG_KEY to the feature flag key you want to evaluate.
#define FEATURE_FLAG_KEY "new-stuff"
#define NEWMESSAGE_FLAG_KEY "New-message"

// just
boost::asio::io_service io_service;
boost::posix_time::seconds interval(5);
boost::asio::deadline_timer timer(io_service, interval);

void tick(const boost::system::error_code & /*e*/) {

    // std::cout << "tick" << std::endl;
    //  Reschedule the timer in the future:
    timer.expires_at(timer.expires_at() + interval);
    // Posts the timer event
    timer.async_wait(tick);
}

struct Foo {
    int one;
    std::string msg;
};

BETTER_ENUM(Error_e, int, UNDEFINED = 1, INPUT_ERROR, UNKNOWN)

tl::expected<Foo, Error_e> getConfiguration(std::string_view name) {

    if (name.empty()) {
        return tl::make_unexpected(Error_e::UNDEFINED);
    }

    std::cout << (+Error_e::INPUT_ERROR)._to_string() << "\n";

    return Foo{.one = 5, .msg = std::string{name}};
}

int main() {

    tl::expected<Foo, Error_e> e1 = Foo{.one = 3, .msg = "this is good"};
    if (e1.has_value()) {
        fmt::print("Expected value {}\n", e1.value().msg);
    }

    auto ret = getConfiguration("ok");
    if (ret.has_value()) {
        std::cout << "expected value " << ret.value().msg << "\n";
    } else {
        std::cout << "expected error " << ret.error() << "\n";
    }

    bool success = dark::RemoteClient::instance().init(MOBILE_KEY);
    if (!success) {
        return 1;
    }

    timer.async_wait(&tick);

    auto client = dark::RemoteClient::instance().client();

    client->FlagNotifier().OnFlagChange(FEATURE_FLAG_KEY, [](auto event) {
        if (event->Deleted()) {
            std::cout << "The flag was deleted\n";
        } else {
            std::cout << "The flag was: '" << event->OldValue() << "' now is: ''" << event->NewValue() << "'\n";
            fmt::print("{}", event->OldValue().AsBool());
        }
    });

    client->FlagNotifier().OnFlagChange(NEWMESSAGE_FLAG_KEY, [](auto event) {
        fmt::print("Oh new message recieve: {}\n", event->NewValue().AsString());
    });

    std::string msg = client->StringVariation(NEWMESSAGE_FLAG_KEY, "I do not know");
    fmt::print("At startup initial message: {}\n\n", msg);

    io_service.run();
    return 0;
}
