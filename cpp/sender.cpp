#include "mqtt/client.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>

const std::string ADDRESS("tcp://localhost:1883");
const std::string CLIENT_ID("CPP_Publisher");

const std::string TOPIC("hello");
const std::string MESSAGE("this was sent from cpp land");

class in_mem_persistence : virtual public mqtt::iclient_persistence {
  bool openStatus;

  std::map<std::string, std::string> storage;

public:
  in_mem_persistence() : openStatus(false) {}

  void open(const std::string &clientID, const std::string &server) override {
    std::cout << "Opening persistent store for " << clientID << " on " << server
              << std::endl;
  }

  void close() override {
    std::cout << "Closed the persistent store" << std::endl;
  }

  void clear() override {
    std::cout << "Clearing storage..." << std::endl;
    storage.clear();
    std::cout << "Cleared storage." << std::endl;
  }

  bool contains_key(const std::string &key) override {
    return storage.find(key) != storage.end();
  }

  const mqtt::string_collection &keys() const override {
    static mqtt::string_collection ks;
    ks.clear();
    for (const auto &k : storage) {
      ks.push_back(k.first);
    }
    return ks;
  }

  void put(const std::string &key,
           const std::vector<mqtt::string_view> &bufs) override {
    std::string holder;
    for (const auto &b : bufs) {
      holder += b.str();
    }
    std::cout << "Adding data '" << key << " : " << holder << "' to key store"
              << std::endl;
    storage[key] = std::move(holder);
  }

  mqtt::string_view get(const std::string &key) const override {
    std::cout << "searching for '" << key << "' in persistent storage"
              << std::endl;
    auto item = storage.find(key);
    if (item == storage.end()) {
      throw mqtt::persistence_exception();
    }
    return mqtt::string_view(item->second);
  }

  void remove(const std::string &key) override {
    auto item = storage.find(key);
    if (item == storage.end()) {
      throw mqtt::persistence_exception();
    }
    storage.erase(item);
    std::cout << "Erased item with key '" << key << "'" << std::endl;
  }
};

class user_callback : public virtual mqtt::callback {
  void connection_lost(const std::string &cause) override {
    std::string error;
    if (cause.empty()) {
      error = "idk";
    } else {
      error = cause;
    }
    std::cout << "Lost connection b/c of: " << error << std::endl;
  }

  void delivery_complete(mqtt::delivery_token_ptr token) override {
    std::cout << "Delivered: " << (token ? token->get_message_id() : -1)
              << std::endl;
  }

public:
};

int main(int argc, char *argv[]) {
  std::cout << "Starting up" << std::endl;

  in_mem_persistence persist;
  mqtt::client client(ADDRESS, CLIENT_ID, &persist);
  user_callback cb;
  client.set_callback(cb);

  mqtt::connect_options options;
  options.set_keep_alive_interval(20);
  options.set_clean_session(true);

  std::cout << "Set up options" << std::endl;

  try {
    std::cout << std::endl << "Connecting" << std::endl;
    client.connect(options);
    std::cout << std::endl << "Connected successfully" << std::endl;

    std::cout << "Sending messsage: " << MESSAGE << std::endl;
    client.publish(mqtt::message(TOPIC, MESSAGE, 0, false));
    std::cout << "Sent message." << std::endl;
  } catch (const mqtt::persistence_exception &ex) {
    std::cout << "Persistence error: " << ex.what() << ": "
              << ex.get_reason_code() << std::endl;
    return 1;
  } catch (const mqtt::exception &ex) {
    std::cout << "Other error: " << ex.what() << std::endl;
    return 1;
  }
  std::cout << "Done" << std::endl;
  return 0;
}
