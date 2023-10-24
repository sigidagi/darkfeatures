#include <launchdarkly/client_side/client.hpp>
#include <launchdarkly/context_builder.hpp>
#include <boost/asio.hpp>

#include <iostream>
#include <cstring>

// Set MOBILE_KEY to your LaunchDarkly mobile key.
#define MOBILE_KEY "mob-6dd0eb29-3491-4e4c-9fc6-ed13152a28f8"

// Set FEATURE_FLAG_KEY to the feature flag key you want to evaluate.
#define FEATURE_FLAG_KEY "new-stuff"

// Set INIT_TIMEOUT_MILLISECONDS to the amount of time you will wait for
// the client to become initialized.
#define INIT_TIMEOUT_MILLISECONDS 3000



boost::asio::io_service io_service;
boost::posix_time::seconds interval(5);
boost::asio::deadline_timer timer(io_service, interval);


using namespace launchdarkly;


int main() {
    if (!strlen(MOBILE_KEY)) {
        printf(
            "*** Please edit main.c to set MOBILE_KEY to your LaunchDarkly "
            "mobile key first\n\n");
        return 1;
    }
    
    auto config = client_side::ConfigBuilder(MOBILE_KEY).Build();
    if (!config) {
        std::cout << "error: config is invalid: " << config.error() << '\n';
        return 1;
    }
    


    launchdarkly::Value mm = Value();
    
    if (mm.IsBool()) {
        std::cout << "mm is bool\n";
    }

    auto context = ContextBuilder().Kind("user", "example-user-key").Name("Sandy").Build();
    auto client = client_side::Client(std::move(*config), std::move(context));
    auto start_result = client.StartAsync();

    auto status = start_result.wait_for(std::chrono::milliseconds(INIT_TIMEOUT_MILLISECONDS));
    if (status == std::future_status::ready) {
        if (start_result.get()) {
            std::cout << "*** SDK successfully initialized!\n\n";
        } else {
            std::cout << "*** SDK failed to initialize\n";
            return 1;
        }
    } else {
        std::cout << "*** SDK initialization didn't complete in "
                  << INIT_TIMEOUT_MILLISECONDS << "ms\n";
        return 1;
    }

    bool flag_value = client.BoolVariation(FEATURE_FLAG_KEY, false);

    std::cout << "*** Feature flag '" << FEATURE_FLAG_KEY << "' is "
              << (flag_value ? "true" : "false") << " for this user\n\n";


    return 0;
}
