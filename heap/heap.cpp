#include "heap.hpp"
#include <iostream>

using std::cout;
using std::endl;

bool less(const int& p, const int& q) {
	return p<q;
}

int main() {

	cout << "Binary Heap" << endl;
	Heap<int> maxPQ = Heap<int>(less);
	cout << "Insert key 1" << endl;
	maxPQ.insert(1);
	maxPQ.printOut();
	cout << "Insert key 2" << endl;
	maxPQ.insert(2);
	maxPQ.printOut();
	cout << "Insert key 3" << endl;
	maxPQ.insert(3);
	maxPQ.printOut();
	cout << "Insert key 4" << endl;
	maxPQ.insert(4);
	maxPQ.printOut();

	auto n = maxPQ.size();
	cout << "Sorted output:" << endl;
	for(int i = 0; i < n; i++)
	{
		cout << "Out:" << maxPQ.pop() << endl;
	}

	cout << endl << "Indexed Binary Heap" << endl;

	IndexedHeap<int> idxmaxPQ = IndexedHeap<int>(10,less);
	cout << "Insert (index,key) : (4,1)" << endl;
	idxmaxPQ.insert(4,1);
	idxmaxPQ.printOut();
	cout << "Insert (index,key) : (3,2)" << endl;
	idxmaxPQ.insert(3,2);
	idxmaxPQ.printOut();
	cout << "Insert (index,key) : (2,3)" << endl;
	idxmaxPQ.insert(2,3);
	idxmaxPQ.printOut();
	cout << "Insert (index,key) : (1,4)" << endl;
	idxmaxPQ.insert(1,4);
	idxmaxPQ.printOut();
	n = idxmaxPQ.size();
	cout << "Sorted output:" << endl;
	for(int i = 0; i < n; i++)
	{
		auto m = idxmaxPQ.pop();
		cout << "Out: (" << m.first << "," << m.second << ")" << endl;
	}
}


