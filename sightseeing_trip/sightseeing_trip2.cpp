/*
 * sightseeing_trip.cpp
 *
 *  Created on: Jun 27, 2016
 *      Author: alessandro
 *
Timus 1004 Sightseeing Trip

Time limit: 0.5 second
Memory limit: 64 MB

There is a travel agency in Adelton town on Zanzibar island.
It has decided to offer its clients, besides many other attractions,
sightseeing the town. To earn as much as possible from this attraction,
the agency has accepted a shrewd decision: it is necessary to find the
shortest route which begins and ends at the same place.
Your task is to write a program which finds such a route.
In the town there are N crossing points numbered from 1 to N and M two-way roads
numbered from 1 to M. Two crossing points can be connected by multiple roads,
but no road connects a crossing point with itself.
Each sightseeing route is a sequence of road numbers y1, …, yk, k > 2.
The road yi (1 ≤ i ≤ k − 1) connects crossing points xi and xi+1,
the road yk connects crossing points xk and x1.
All the numbers x1, …, xk should be different. The length of the sightseeing route is
the sum of the lengths of all roads on the sightseeing route,
i.e. L(y1) + L(y2) + … + L(yk) where L(yi) is the length of the road yi (1 ≤ i ≤ k).
Your program has to find such a sightseeing route, the length of which is minimal,
or to specify that it is not possible, because there is no sightseeing route in the town.

Input

Input contains T tests (1 ≤ T ≤ 5). The first line of each test contains two integers:
the number of crossing points N and the number of roads M
(3 ≤ N ≤ 100; 3 ≤ M ≤ N · (N − 1)).
Each of the next M lines describes one road.
It contains 3 integers: the number of its first crossing point a,
the number of the second one b, and the length of the road l
(1 ≤ a, b ≤ N; a ≠ b; 1 ≤ l ≤ 300). Input is ended with a “−1” line.

Output
Each line of output is an answer. It contains either a string “No solution.” in case
there isn't any sightseeing route, or it contains the numbers of all crossing points
on the shortest sightseeing route in the order how to pass them
(i.e. the numbers x1 to xk from our definition of a sightseeing route),
separated by single spaces. If there are multiple sightseeing routes of the minimal length,
you can output any one of them.

Input:

5 7
1 4 1
1 3 300
3 1 10
1 2 16
2 3 100
2 5 15
5 3 20
4 3
1 2 10
1 3 20
1 4 30
-1

Output:

1 3 5 2
No solution.

The proposed solution is based a really simple one based on customized DFS
on undirected graph. It gets TLE at test 3.

Solution in C++11.
 *
 */
#include <list>
#include <utility>
#include <functional>
#include <algorithm>
#include <limits>
#include <memory>
#include <iostream>
#include <tuple>
#include <vector>
#include <memory>

using std::list;
using std::vector;
using std::pair;
using std::pair;
using std::make_pair;
using std::fill;
using std::numeric_limits;
using std::cout;
using std::cin;
using std::endl;
using std::tuple;
using std::get;
using std::shared_ptr;
using std::make_shared;

template<typename Dtype, typename Ctype>
class BagOfVerteces {
 public:
   BagOfVerteces() {
	   connected = make_shared<list<pair<Dtype, Ctype> > >();
   }
   void add_edge(Dtype vertex, Ctype cost) {
	 auto cc = make_pair(vertex, cost);
	 connected.get()->push_back(cc);
   }
   typename list<pair<Dtype, Ctype> >::size_type get_rank() const {
	 return connected.get()->size();
   }
   list<pair<Dtype, Ctype> >* get_outgoing_edges() const {
	 return connected.get();
   }
   void sort_edges() {
	 connected.get()->sort();
   }
 private:
   shared_ptr<list<pair<Dtype, Ctype> > > connected;
};

template<typename Dtype, typename Ctype>
class Graph {
 public:
   Graph(Dtype _num_verteces) {
     num_verteces = _num_verteces;
     verteces = new BagOfVerteces<Dtype,Ctype>[num_verteces]();
   }
   ~Graph() {
	   delete[] verteces;
   }
   void add_edge(Dtype v1, Dtype v2, Ctype c) {
	 verteces[v1].add_edge(v2,c);
     verteces[v2].add_edge(v1,c);
   }
   typename list<pair<Dtype, Ctype> >::size_type get_vertex_rank(Dtype v) const {
     return verteces[v].get_rank();
   }
   list<pair<Dtype, Ctype> >* outgoing_edges(Dtype v) const {
	 return verteces[v].get_outgoing_edges();
   }
   Dtype get_num_verteces() const {
     return num_verteces;
   }
   void sort_edges() {
	 for(int i=0; i < num_verteces; i++ ) {
		 verteces[i].sort_edges();
	 }
   }
 private:
   Dtype num_verteces;
   BagOfVerteces<Dtype,Ctype> *verteces;
};

template<typename Dtype, typename Ctype>
class DFS {
 public:
   DFS(const Graph<Dtype,Ctype>& _g, Dtype& _start, Dtype& _end) {
     g = &_g;
	 costs = new Dtype[_g.get_num_verteces()];
	 fill(costs, costs+_g.get_num_verteces(), numeric_limits<int>::max());
	 previous = new Dtype[_g.get_num_verteces()];
	 fill(previous, previous+_g.get_num_verteces(), -1);
	 start = _start;
	 end = _end;
	 minimum_cost = numeric_limits<int>::max();
   }
   ~DFS() {
	   delete costs;
	   delete previous;
   }
   list<Dtype>* search_func(const Dtype& current_v, unsigned int depth) {
	 list<Dtype>* l = NULL;
	 list<Dtype>* candidate_l;
	 int candidate_cost;
	 typedef list<pair<Dtype, Ctype> > edges_list;
	 const edges_list *edges = g->outgoing_edges(current_v);
	 Ctype current_cost, new_cost;
	 if(edges->begin()==edges->end()) return NULL;
	 for(auto iter = edges->begin(); iter != edges->end(); iter++) {
       auto v = iter->first;
       auto c = iter->second;
       if (depth==0) {
    	   current_cost = 0;
       } else {
    	   current_cost = costs[current_v];
       }
       new_cost = current_cost + c;
       if ( v == end and  new_cost  < costs[v] and depth>=2 ) {
    	   previous[v] = current_v;
    	   costs[v] = new_cost;
    	   if (costs[v] > 0  and costs[v] < minimum_cost) {
    		   l = shortest_path();
               minimum_cost = costs[v];
    	   }
    	   return l; //close path! OK V
       }
       //if this is the cheapest path so far to get to vertex ith,
       //update the cost and its previous vertex
       if ( new_cost < costs[v] and v!=end) {
    	 costs[v] = new_cost;
    	 previous[v] = current_v;
    	 //go recursively until its over
    	 candidate_l = search_func(v, depth + 1);
    	 //new candidate, if not null, is surely better than old one
    	 if (candidate_l!=NULL) l = candidate_l;
       }
	 }
	 return l;
   }
   list<Dtype>* search() {
	   list<Dtype>* l = search_func(start,0);
       return l;
   }
   inline list<Dtype>* shortest_path() {
	   list<Dtype>* l = new list<Dtype>();
       Dtype v = end;
	   do {
		   l->push_back(previous[v]);
		   v = previous[v];
	   } while ( v != start);
	   return l;
   }
   void clear() {
	   fill(costs, costs+g->get_num_verteces(), numeric_limits<int>::max());
	   fill(previous, previous+g->get_num_verteces(), -1);
   }
   int get_minimum_cost() const {
	   return minimum_cost;
   }
 private:
   int *costs;
   int minimum_cost;
   int *previous;
   Dtype start;
   Dtype end;
   const Graph<Dtype, Ctype>* g;
};

int read_input(list<tuple<int,int,int> >* roads) {

	int crossing_points, num_roads;
    int a, b, cost;
	cin >> crossing_points;
	if (crossing_points == -1) return -1;
	cin >> num_roads;
    for(int i=0; i<num_roads; i++) {
    	cin >> a >> b >> cost;
    	roads->push_back(tuple<int,int,int>(a, b, cost));
    }
    return crossing_points;
}

int main() {

	int num_verteces;
	int a, b, cost;
	list<tuple<int, int, int> > * l_ptr = new list<tuple<int, int, int> >();
	shared_ptr<Graph<int, int> > g;
	DFS<int,int> *dfs;
	int *costs;
	vector<list<int>* > paths;
	//read inputs
	while( (num_verteces = read_input( l_ptr )) != -1 ) {
        //Graph initialization
		g = make_shared<Graph<int,int> >(num_verteces);
        for(list<tuple<int, int, int> >::const_iterator i = l_ptr->begin(); i != l_ptr->end(); i++ ) {
        	a = get<0>(*i);
        	b = get<1>(*i);
        	cost = get<2>(*i);
        	g.get()->add_edge(a-1,b-1,cost);
        }
        //search for the shortest path
        paths.reserve(num_verteces);
        costs = new int[num_verteces];
        for(int i = 0; i < num_verteces; i++ ) {
        	costs[i] = numeric_limits<int>::max();
            //instatiate the custom DFS
            dfs = new DFS<int, int>(*(g.get()), i, i);
            paths.push_back(dfs->search());
            costs[i] = dfs->get_minimum_cost();
            delete dfs;
        }
        int minimum_cost = numeric_limits<int>::max();
        int best = -1;
        for(int i = 0; i < num_verteces; i++ ) {
            if( costs[i] < minimum_cost and paths[i] != NULL ) {
                minimum_cost = costs[i];
                best = i;
            }
        }
        if (best>=0) {
        	int counter = 0;
            for(list<int>::reverse_iterator it = paths[best]->rbegin(); it != paths[best]->rend(); it++ ) {
                cout << *it + 1;
                if (counter < (num_verteces-1) ) {
                    cout << " ";
                }
                counter++;
            }
            cout << endl;
        } else {
        	cout << "No solution." << endl;
        }
        for ( int i=0; i < num_verteces; i++ ) {
            if (paths[i]!=NULL) delete paths[i];
        }
        paths.clear();
		delete l_ptr;
		l_ptr = new list<tuple<int, int, int> >();
    }
	delete l_ptr;

}
