#include "../graph/api.h"
#include "../graph/tree_plus/immutable_graph_tree_plus.h"
#include "../lib_extensions/sparse_table_hash.h"
#include "../pbbslib/random_shuffle.h"
#include "../trees/utils.h"

#include <cstring>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <thread>
#include <vector>

#include <fstream>
#include <iostream>

#include "rmat_util.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " <num_vertices>" << std::endl;
    return 0;
  }

  size_t num_kv_pairs = std::stol(argv[1]);


  // Inserting key/value pairs one at a time
  std::vector<std::tuple<uintV, uintV>> stream_of_kv;
  auto r = pbbs::random(num_kv_pairs * 2);
  for (int i = 0; i < num_kv_pairs; i++) {
    stream_of_kv.push_back(
        std::make_tuple(r.ith_rand(2 * i), r.ith_rand((2 * i) + 1)));
  }

  auto tree_plus_graph = empty_treeplus_graph();
  timer st;
  st.start();
  for (auto kv : stream_of_kv) {
    tree_plus_graph.insert_edges_batch(1, &kv);
  }
  double runtime = st.stop();
  std::cout << "runtime (insert one-at-a-time) = " << runtime << std::endl;

  st.start();
  for (auto kv : stream_of_kv) {
    tree_plus_graph.delete_edges_batch(1, &kv);
  }
  runtime = st.stop();
  std::cout << "runtime (delete one-at-a-time) = " << runtime << std::endl;

  // Inserting key/value pairs in batches
  tree_plus_graph = empty_treeplus_graph();
  st.start();
  tree_plus_graph.insert_edges_batch(stream_of_kv.size(), stream_of_kv.data());
  runtime = st.stop();
  std::cout << "runtime (insert batches) = " << runtime << std::endl;

  // Deleting key/value pairs in batches
  st.start();
  tree_plus_graph.delete_edges_batch(stream_of_kv.size(), stream_of_kv.data());
  runtime = st.stop();
  std::cout << "runtime (delete batches) = " << runtime << std::endl;

  // // using just the tree plus
  // sym_immutable_graph_tree_plus tree_plus;
  // st.start();
  // tree_plus.insert_edges_batch(stream_of_kv.size(), stream_of_kv.data());
  // runtime = st.stop();
  // std::cout << "runtime (tree plus) = " << runtime << std::endl;
}
