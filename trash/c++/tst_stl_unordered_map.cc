#include <iostream>
#include <list>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Node {
  int key;
  int value;
  int frequency;

  Node(int k, int v, int f) : key(k), value(v), frequency(f) {}
};

using CacheType = std::unordered_map<int, std::list<Node>>;

void dump(CacheType& map_freq_data, bool reset = false) {
  static int counter = 0;
  if (reset) {
    counter = 0;
  }
  printf("%2dst@ \n", counter);
  std::map<int, std::list<Node>> map_freq_data_sorted_;
  for (auto iter = map_freq_data.begin(); iter != map_freq_data.end();
       iter++) {
    map_freq_data_sorted_.emplace(iter->first, iter->second);
  }

  for (auto iter = map_freq_data_sorted_.begin();
       iter != map_freq_data_sorted_.end(); iter++) {
    std::cout << "  freq " << iter->first << " -> ";
    auto data = iter->second;
    for (auto iter2 = data.begin(); iter2 != data.end(); iter2++) {
      std::cout << iter2->key << ":" << iter2->value << ":" << iter2->frequency << " ";
    }
    std::cout << std::endl;
  }

  counter++;
}

int main() {
  CacheType map_freq_data;
  Node node(1, 1, 1);
  map_freq_data[node.frequency].emplace_front(node);
  Node node1(2, 2, 2);
  map_freq_data[node1.frequency].emplace_front(node1);
  Node node2(2, 3, 2);
  map_freq_data[node2.frequency].emplace_front(node2);
  Node node3(3, 3, 3);
  map_freq_data[node3.frequency].emplace_front(node3);

  dump(map_freq_data);

  return 0;
}
