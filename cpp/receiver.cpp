#include "mqtt/client.h"
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

const std::string SERVER("tcp://127.0.0.1:1883");
const std::string CLIENT_ID("cppTestReceiver");
const std::string TOPIC("$SYS/#");
const std::string TOPIC1("hello");
const std::string TOPIC2("world");

class callback : public virtual mqtt::callback {
  mqtt::client &base_client;

  void connected(const std::string &message) override {
    std::cout << std::endl << "Connected: " << message << std::endl;
  }

  void connection_lost(const std::string &error) override {
    if (!error.empty()) {
      std::cout << std::endl << "Error: " << error << std::endl;
    }
  }

  void message_arrived(mqtt::const_message_ptr msg) override {
    std::cout << msg->get_topic() << ": " << msg->get_payload_str()
              << std::endl;
  }

  void delivery_complete(mqtt::delivery_token_ptr token) override {}

public:
  callback(mqtt::client &cli) : base_client(cli) {}
};

int main(int argc, char *argv[]) {
  mqtt::connect_options options;

  options.set_keep_alive_interval(10);
  options.set_clean_session(false);
  options.set_automatic_reconnect(true);

  mqtt::client cli(SERVER, CLIENT_ID);

  callback cb(cli);
  cli.set_callback(cb);

  try {
    std::cout << "Connecting to server " << SERVER << std::flush;
    cli.connect(options);
    cli.subscribe(TOPIC, 0);
    cli.subscribe(TOPIC1, 0);
    cli.subscribe(TOPIC2, 0);
    std::cout << "all OK" << std::endl;
    std::cout << "connected" << std::endl;
  } catch (const mqtt::exception &exc) {
    std::cout << "we errored: " << exc.what() << std::endl;
    return 1;
  }

  while (true)
    ;
}
