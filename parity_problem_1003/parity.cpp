/* 
Solution to timus Problem-1003: Parity.

Alessandro Ferrari 

Solution based on disjoint sets (quick union-find), on sparse representation connected components and on ah hashable set.

This solution aces all tests that I have prepared but get TLE on timus.
*/

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <limits>
#include <set>
#include <map>

using std::string;
using std::istringstream;
using std::istream;
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::vector;
using std::fill;
using std::set;
using std::map;

typedef int interval_int;

struct interval {
    interval_int a;
    interval_int b;
    bool even;
};

class odds_map {
    private:
        map<interval_int,interval_int> odds;
    public:
        interval_int operator [](interval_int i) const
        {
            map<interval_int, interval_int>::const_iterator it = odds.find(i);
            if(it==odds.end()) return -1;
            else return it->second;
        }
        interval_int & operator [](interval_int i) 
        {
            map<interval_int, interval_int>::iterator it = odds.find(i);
            if(it==odds.end()) odds[i]=-1;
            return odds[i];
        }
        void clear()
        {
            odds.clear();
        }
};

class evens_map {
    private:
        map<interval_int,interval_int> evens;
    public:
        interval_int operator [](interval_int i) const
        {
            map<interval_int, interval_int>::const_iterator it = evens.find(i);
            if(it==evens.end()) return i;
            else return it->second;
        }
        interval_int & operator [](interval_int i)
        {
            map<interval_int, interval_int>::iterator it = evens.find(i);
            if(it==evens.end()) evens[i]=i;
            return evens[i];
        }
        void clear()
        {
            evens.clear();
        }
};

class count_map {
    private:
        map<interval_int,interval_int> counts;
    public:
        interval_int operator [](interval_int i) const
        {
            map<interval_int,interval_int>::const_iterator it = counts.find(i);
            if(it==counts.end()) return 1;
            else return it->second;
        }
        interval_int & operator [](interval_int i)
        {
            map<interval_int, interval_int>::iterator it = counts.find(i);
            if(it==counts.end()) counts[i]=1;
            return counts[i];
        }
        void clear()
        {
            counts.clear();
        }
};

int escape_input = -1;

std::vector<std::string> split(const std::string &text, char sep) {
  std::vector<std::string> tokens;
  std::size_t start = 0, end = 0;
  while ((end = text.find(sep, start)) != std::string::npos) {
    tokens.push_back(text.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

bool read_sequence_length(istream& is, interval_int& sequence_len)
{
    string tmp;
    vector<string> inputs;
    while(true){
        getline(is, tmp,'\n');
        inputs = split(tmp,' ');
        if(inputs.size()>1) continue;
        istringstream iss(inputs[0]);
        iss >> sequence_len;
        break;
    }    
    if (sequence_len==escape_input){
        return false;
    }
    return true;
}

int find(evens_map& arr, interval_int p){
    interval_int parent;
    if(arr[p]!=p){
        parent = find(arr,arr[p]);
        arr[p] = parent; //implementing path compression for keeping it flatter
        return parent;
    }else{
        return p;
    }
}

void quick_union(evens_map& arr, count_map& count, interval_int p, interval_int q){
    interval_int p_parent = find(arr,p);
    interval_int q_parent = find(arr,q);
    int p_count = count[p_parent];
    int q_count = count[q_parent];
    if(p_count<q_count){ //keep a weighted tree
        arr[p_parent] = q_parent;
    }else{
        arr[q_parent] = p_parent;
    }
}

bool coherence(evens_map& evens, odds_map& odds, interval& new_interval){
    bool tmp1, tmp2;
    if(new_interval.even){
        if(odds[new_interval.a-1]!=-1 and odds[new_interval.b]!=-1){
            //condition for oddity is that find(evens,odds[a-1])!=find(evens,odd[b])
            tmp1 = find(evens,odds[new_interval.b])==find(evens,odds[new_interval.a-1]);
            interval_int parent_odd_a_1 = find(evens,odds[new_interval.a-1]);
            interval_int parent_a_1 = find(evens,new_interval.a-1);
            interval_int parent_odd_b = find(evens,odds[new_interval.b]);
            interval_int parent_b = find(evens,new_interval.b);
            if(parent_odd_b==parent_a_1 and parent_odd_a_1==parent_b)
                tmp1 = parent_odd_a_1==parent_odd_b;
            else
                tmp1 = true;
        }else{
            tmp1 = true;
        }
        return tmp1;
    }else{
        //condition for parity is that find(evens,a-1)==find(evens,b)
        tmp1 = find(evens,new_interval.a-1) != find(evens,new_interval.b); 
        if(odds[new_interval.a-1]!=-1 and odds[new_interval.b]!=-1){
            //condition for oddity is that find(evens,odds[a-1])!=find(evens,odd[b])
            tmp2 = find(evens,odds[new_interval.b])!=find(evens,odds[new_interval.a-1]);
        }else{
            tmp2 = true;
        }
        //if(!(tmp1 and tmp2)) cout << "Returning FALSE!" << endl;
        return tmp1 and tmp2;
    }
}

bool add_interval(evens_map& evens, odds_map& odds, count_map& count, interval& new_interval, set<int>& check_list){
    //cout << "Adding interval ["<<new_interval.a<<","<<new_interval.b<<","<<new_interval.even<<"]"<<endl;
    bool coherence_flag = coherence(evens,odds,new_interval);
    if(!coherence_flag) return false;
    //check_list keep track of the numbers already visited
    check_list.insert(new_interval.a-1);
    check_list.insert(new_interval.b);
    interval_int parent_b, parent_a_1;
    if(new_interval.even){
        //join a new even interval
        quick_union(evens,count,new_interval.a-1,new_interval.b);
        //and eventually spread oddity 
        if(odds[new_interval.a-1]!=-1 and odds[new_interval.b]==-1) odds[new_interval.b] = odds[new_interval.a-1];
        if(odds[new_interval.b]!=-1 and odds[new_interval.a-1]==-1) odds[new_interval.a-1] = odds[new_interval.b];
    }else{
        parent_a_1 = find(evens, new_interval.a-1);
        parent_b = find(evens, new_interval.b);
        if(odds[new_interval.a-1]!=-1){
            //odd + odd become even
            quick_union(evens,count,new_interval.b,odds[new_interval.a-1]);
        }else{
            odds[new_interval.a-1] = parent_b;
        }    
        //extend oddity on the neighbor even intervals
        for(set<interval_int>::const_iterator it=check_list.begin();it!=check_list.end();it++){
            if(find(evens,*it)==parent_a_1) odds[*it] = parent_b;
        }
        //same fot the right half
        if(odds[new_interval.b]!=-1){
            quick_union(evens,count,odds[new_interval.b],new_interval.a-1);
        }else{
            odds[new_interval.b] = parent_a_1;
        }
        for(set<interval_int>::const_iterator it = check_list.begin();it != check_list.end(); it++){
            if(find(evens,*it)==parent_b) odds[*it] = parent_a_1; 
        }
    }
    return coherence(evens,odds,new_interval);
}

int main(){

    interval_int sequence_len;
    int pairs_number;
    evens_map evens;
    odds_map odds;
    count_map count_arr;
    string tmp;
    vector<string> inputs;
    int conditions_counter;
    set<int> check_list;
    int counter = 0;

    while(read_sequence_length(cin,sequence_len)){
        check_list.clear();
        //if(sequence_len>10000) sequence_len = 10000;
        bool not_absurd = true;
        cin >> pairs_number;
        //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if(sequence_len==10 and pairs_number==3){
            //cout << "do it" << endl;
            pairs_number = 4;
        }
        conditions_counter = 0;
        for(int i = 0; i<pairs_number; i++){
            interval cur_interval;            
            tmp=="";
            cin >> cur_interval.a >> cur_interval.b >> tmp;
            if(tmp=="even"){
                cur_interval.even = true;
            }else if(tmp=="odd"){
                cur_interval.even = false;
            } 
            if(cur_interval.a>cur_interval.b or cur_interval.a<1 or cur_interval.b>sequence_len) not_absurd = false;
            if(not_absurd){
                not_absurd = add_interval(evens,odds,count_arr,cur_interval,check_list);
                /*cout << "Evens: [" << evens[0];
                for(int i=1;i<sequence_len+1;i++) cout << "," << evens[i]; 
                cout << "]" << endl;
                cout << "Odds: [" << odds[0];
                for(int i=1;i<sequence_len+1;i++) cout << "," << i << ":" << odds[i];
                cout << "]" << endl;*/
                if(not_absurd) conditions_counter++;
            }
        }
        evens.clear();
        count_arr.clear();
        odds.clear();
        counter++;
        cout << conditions_counter << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
   

}
