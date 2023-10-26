#include "spdlog/fmt/fmt.h"
#include "spdlog/spdlog.h"
#include <iostream>

#include "remote_client.hpp"
#include "toml.hpp"
#include <launchdarkly/context_builder.hpp>

// Set INIT_TIMEOUT_MILLISECONDS to the amount of time you will wait for
// the client to become initialized.
#define INIT_TIMEOUT_MILLISECONDS 3000

namespace ld = launchdarkly;

namespace dark {

RemoteClient::RemoteClient() {}

bool RemoteClient::init(const std::string &key) {

    if (key.empty()) {
        printf("*** Please edit main.c to set MOBILE_KEY to your LaunchDarkly "
               "mobile key first\n\n");
        return 1;
    }

    auto config = ld::client_side::ConfigBuilder(key).Build();
    if (!config) {
        spdlog::error("error: config is invalid: ");
        return false;
    }

    std::string filename = "configuration.toml";
    User user;
    toml::table tbl;
    try {
        tbl = toml::parse_file(filename);
        user.name = tbl["Name"].value_or("");
        user.born = tbl["Born"].value_or(0);
        user.location = tbl["Location"].value_or("");
        user.key = tbl["Key"].value_or("");
        user.work = tbl["Work"].value_or("");
        user.male = tbl["Male"].value_or(false);
        // user.city = tbl["City"].value_or("");

        std::cout << "---- TBL -----------\n";
        std::cout << tbl << "\n";

    } catch (const toml::parse_error &e) {
        spdlog::error("pasing configuration file '{}' failed", filename);
        spdlog::error("error {}", e.description());
        return false;
    }

    auto context = ld::ContextBuilder()
                       .Kind("User", user.key)
                       .Name(user.name)
                       .Set("Location", user.location)
                       .Set("Work", user.work)
                       .Set("Born", user.born)
                       //.Set("City", user.city)
                       .Set("Male", user.male)
                       .Build();

    // auto context = ld::ContextBuilder().Kind("user", "example-user-key").Name("Sandy").Build();
    client_ = std::make_shared<launchdarkly::client_side::Client>(std::move(*config), std::move(context));

    auto start_result = client_->StartAsync();

    auto status = start_result.wait_for(std::chrono::milliseconds(INIT_TIMEOUT_MILLISECONDS));
    if (status == std::future_status::ready) {
        if (start_result.get()) {
            spdlog::info("*** SDK successfully initialized!");
            spdlog::info("All flags size: {}", client_->AllFlags().size());

            /*
             *            for (auto [flag_key, flag_value] : client_->AllFlags()) {
             *
             *                if (flag_value.IsBool()) {
             *                    fmt::print("key: {}, value: {}\n", flag_key, flag_value.AsBool());
             *                } else if (flag_value.IsString()) {
             *                    fmt::print("key: {}, value: {}\n", flag_key, flag_value.AsString());
             *                } else if (flag_value.IsObject()) {
             *                    auto object = flag_value.AsObject();
             *                    fmt::print("--- Object ---\n");
             *                    for (auto it = object.begin(); it != object.end(); ++it) {
             *                        std::cout << it->first << ": " << it->second << "\n";
             *                    }
             *                    fmt::print("------\n");
             *                }
             *            }
             */

            return true;
        } else {
            spdlog::error("*** SDK failed to initialize");
            return false;
        }
    } else {
        spdlog::warn("*** SDK initialization didn't complete in {} ms", INIT_TIMEOUT_MILLISECONDS);
        return false;
    }
}

std::shared_ptr<ld::client_side::Client> RemoteClient::client() const { return client_; }

} // namespace dark
