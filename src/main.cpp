#include <launchdarkly/context_builder.hpp>
// #include <launchdarkly/client_side/flag_change_event.hpp>
#include <boost/asio.hpp>

#include <cstring>
#include <iostream>

#include "remote_client.hpp"

// Set MOBILE_KEY to your LaunchDarkly mobile key.
#define MOBILE_KEY "mob-6dd0eb29-3491-4e4c-9fc6-ed13152a28f8"

// Set FEATURE_FLAG_KEY to the feature flag key you want to evaluate.
#define FEATURE_FLAG_KEY "new-stuff"

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

int main() {

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
            std::cout << "The flag was " << event->OldValue() << " now is " << event->NewValue() << "\n";
        }
    });

    io_service.run();
    return 0;
}