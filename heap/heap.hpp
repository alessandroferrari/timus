#ifndef HEAP_HPP_GUARD
#define HEAP_HPP_GUARD

#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>
#include <stdexcept>

template<typename T>
class Heap
{
public:
	Heap(bool (*_cmp)(const T&, const T&)) {
		cmp = _cmp;
		pq.push_back(static_cast<T>(NULL ));
	}
	bool isEmpty()
	{
		return (n == 0);
	};
	int size()
	{
		return n;
	};
	void insert(const T& key)
	{
		pq.push_back(key);
		n++;
		swim(n);
	};
    T pop()
    {
    	T maxKey = pq[1];
    	exch(1,n--);
    	sink(1);
    	return maxKey;
    };
    void printOut()
    {
    	for(int i = 1; i < pq.size(); i++) {
    		std::cout << i << ":" << pq[i] << std::endl;
    	}
    }
private:
    bool (*cmp)(const T&, const T&);
	std::vector<T> pq;
	int n = 0;
    void exch(int i, int j)
    {
    	T tmp = pq[i];
    	pq[i] = pq[j];
    	pq[j] = tmp;
    };
    void swim(int k)
    {
    	while ( k>1 and cmp(pq[k/2],pq[k]) )
    	{
    		exch(k/2, k);
    		k = k/2;
    	}
    };
    void sink(int k)
    {
    	while ( 2*k <= n )
    	{
    		int j = 2*k;
    		if (j < n and cmp(pq[j], pq[j+1])) j++;
    		if (!cmp(pq[k],pq[j])) break;
    		exch(k, j);
    		k = j;
    	}
    }
};

template<typename T>
class IndexedHeap
{
public:
	IndexedHeap(int _max_n, bool (*_cmp)(const T&, const T&)) :
	                                                           sp_pq(new int[_max_n], std::default_delete<int[]>()),
															   sp_qp(new int[_max_n], std::default_delete<int[]>()),
															   sp_keys(new T[_max_n], std::default_delete<T[]>())
															   {
		max_n = _max_n;
		cmp = _cmp;
        sp_pq.get()[0] = -1;
		std::fill(sp_qp.get(), sp_qp.get() + max_n, -1);
		std::fill(sp_keys.get(), sp_keys.get() + max_n, static_cast<T>(NULL));
	}
	bool isEmpty() {
		return (n==0);
	}
	bool contains(int i) {
		return (qp(i) != -1);
	}
	int size()
	{
		return n;
	};
	void insert(int i, T k) {
		n++;
		qp(i) = n;
		pq(n) = i;
		keys(i) = k;
		swim(n);

	}
	T minKey() {
		if (isEmpty()) {
			throw std::out_of_range("There are not items pushed in this heap.");
		}
		return keys2pq(1);
	}
	std::pair<int,T> pop() {
		if (isEmpty()) {
			throw std::out_of_range("There are not items pushed in this heap.");
		}
		int indexOfMin = pq(1);
		T k = keys(indexOfMin);
		exch(1,n);
		n--;
		sink(1);
		keys2pq(n+1) = static_cast<T>(NULL);
		qp(pq(n+1)) = -1;
		return std::make_pair(indexOfMin,k);
	}
	void printOut()
	{
		for(int i = 1; i < n+1; i++) {
			std::cout << i << ": (" << pq(i) << "," << keys2pq(i) << ")" << std::endl;
		}
	}
private:
	bool (*cmp)(const T&, const T&);
	int max_n, n = 0;
	std::shared_ptr<int> sp_pq, sp_qp;
	std::shared_ptr<T> sp_keys;
	inline int& pq(const int& i) { return sp_pq.get()[i]; }
	inline int& qp(const int& i) { return sp_qp.get()[i]; }
	inline T& keys(const int& i) { return sp_keys.get()[i]; }
	inline T& keys2pq(const int& i) { return sp_keys.get()[sp_pq.get()[i]]; }
	void exch(int i, int j)
	{
		int tmp = pq(i);
		pq(i) = pq(j);
		pq(j) = tmp;
		tmp = qp(i);
		qp(i) = qp(j);
		qp(j) = tmp;

	};
	bool wrapped_cmp(int i, int j) {
		return cmp(keys(i), keys(j));
	}
	void swim(int k)
	{
		while ( k>1 and wrapped_cmp(pq(k/2),pq(k)) )
		{
			exch(k/2, k);
			k = k/2;
		}
	};
	void sink(int k)
	{
		while ( 2*k <= n )
		{
			int j = 2*k;
			if (j < n and wrapped_cmp(pq(j), pq(j+1))) j++;
			if (!wrapped_cmp(pq(k), pq(j))) break;
			exch(k, j);
			k = j;
		}
	}
};

#endif

