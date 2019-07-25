#pragma once

#include <climits> // For INT_MAX, INT_MIN
#include <list>
#include <vector>
#include <iostream>
#include <queue>

class graph {
  public:
    /* graph(n)
       Construct a graph with n nodes and no edges, initially.
    */
    graph(int n); 

    ~graph();

    /* add_edge(a,b)
       Add an edge from node a to node b. Note that self edges are not allowed,
       so attempting add_edge(a,a) should be ignored. Similarly, this is not
       a multigraph, so if an edge a -> b already exists, a second one should
       be ignored.
       Should run in O(E) time in the worst case. 
    */
    void add_edge(int a, int b);

    /* has_edge(a,b)
       Returns true if there is an edge from a to b. Should return false if
       either a or b is out-of-range (< 0 or >= count_nodes()).
       Should run in O(E) time.
    */
    bool has_edge(int a, int b);

    /* count_nodes()
       Returns the number of nodes in this graph.
       Should run in O(1) time
    */
    int count_nodes();

    /* count_edges()
       Returns the total number of edges in this graph.
       Should run in O(E) time.
    */
    int count_edges();

    /* count_edges(n)
       Returns the number of outbound edges from node n.
       Should run in O(E) time
    */
    int count_edges(int a);

    /* bfs(n)
       Perform a breadth-first search, starting at node n, and returning a
       vector that gives the distance to every other node. (If a node is 
       unreachable from n, then set its distance to INT_MAX.)
       Should run in O(E + N) time.
    */
    std::vector<int> bfs(int n);

    /* is_connected(a,b)
       Returns true if a path exists from node a to b. 
       Should run in O(E + N) time.
    */
    bool is_connected(int a, int b);

  private:
    std::vector<std::list<int>> *adj_list;
    int numNodes = 0;
    int numEdges = 0;

};

graph::graph(int n) {
  numNodes = n;
  adj_list = new std::vector<std::list<int>>(n);
}

graph::~graph() {
  free(adj_list);
}

/* is_connected(a,b)
    Returns true if a path exists from node a to b. 
    Should run in O(E + N) time.
*/
bool graph::is_connected(int a, int b) {
  return (bfs(a)[b] ==  INT_MAX) ? false : true;
}

void graph::add_edge(int a, int b) {
  // do not add edge (a, b) if it already exists
  if(b == a || has_edge(a, b)) {
    return;
  }

  // get the current node
  std::list<int> *node = &adj_list->at(a);

  node->push_back(b);

  numEdges++;
};

bool graph::has_edge(int a, int b) {
  std::list<int> *nodeA;

  // get node 'a' in adj_list, return false if out of range.
  try {
    nodeA = &adj_list->at(a);
  } catch (const std::out_of_range& oor) {
    return false;
  }

  if(nodeA->empty()) {
    return false;
  }
  
  // iterate through items checking for node 'b'
  for(int node : *nodeA) {
    if(node == b) {
      return true;
    }
  }

  // return false if 'b' is not find in list
  return false;
};

int graph::count_nodes() {
  return adj_list->size();
};

int graph::count_edges() {
  int count = 0;
  // for each node in the vector
  for( std::list<int> node : *adj_list) {
    // get iterator for current node
    for( int _ : node) {
      count++;
    }
  }

  return count;
}

int graph::count_edges(int a) {
  int count = 0;

  for( int _ : adj_list->at(a)) {
    count++;
  }

  return count;
}

/* bfs(n)
    Perform a breadth-first search, starting at node n, and returning a
    vector that gives the distance to every other node. (If a node is 
    unreachable from n, then set its distance to INT_MAX.)
    Should run in O(E + N) time.
*/
std::vector<int> graph::bfs(int n) {
  int currentNode;
  std::list<int> nodesPointedToByCurrNode;
  std::queue<int> nodesInQueue;
  std::vector<int> visitedNodes(numNodes, INT_MAX);
  
  // add first node
  nodesInQueue.push(n);
  visitedNodes[n] = 0;

  while(!nodesInQueue.empty()) {
    // Get the node at the front of the queue
    currentNode = nodesInQueue.front();
    // Get the node (list of edges out) at the front of the queue.
    nodesPointedToByCurrNode = adj_list->at(nodesInQueue.front());

    // For each node pointed to by the current node...
    for(int node : nodesPointedToByCurrNode) {
      // If the node hasn't been visited yet, add it to the queue and set its
      // distance
      if(visitedNodes[node] == INT_MAX) {
        nodesInQueue.push(node);
        visitedNodes[node] = visitedNodes[currentNode] + 1;
      } //end if
    } // end for
    nodesInQueue.pop();
  } // end while

  return visitedNodes;
};