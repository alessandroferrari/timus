/* 
Solution to timus Problem-1003: Parity.

Alessandro Ferrari 

This solution gets ME.
*/

#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <algorithm>
#include <vector>
#include <limits>
#include <map>

using std::string;
using std::istringstream;
using std::istream;
using std::cin;
using std::cout;
using std::endl;
using std::list;
using std::getline;
using std::vector;
using std::upper_bound;
using std::lower_bound;
using std::back_inserter;
using std::min;
using std::max;
using std::equal;
using std::map;
using std::pair;

typedef int interval_int;

struct interval {
    interval_int a;
    interval_int b;
    bool even;
};

typedef int interval_int;
typedef map<interval_int, interval> interval_map;
typedef map<interval_int, map<interval_int, interval> > interval_map_map;

int escape_input = -1;

/*bool read_sequence_length(istream& is, int& sequence_len)
{
    is >> sequence_len;
    if (sequence_len==escape_input){
        return false;
    }
    return true;
}*/

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

bool is_in(interval_map_map& edges_map, interval_int a, interval_int b){
    interval_map_map::const_iterator it = edges_map.find(a);
    interval_map::const_iterator it2;
    if(it!=edges_map.end()){
        it2 = it->second.find(b);
        if(it2!=it->second.end()){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

bool is_in(interval_map& edge_map, interval_int b){
    interval_map::const_iterator it = edge_map.find(b);
    if(it!=edge_map.end()){
        return true;
    }else{
        return false;
    }
}

bool add_interval(interval_map_map& intervals_by_begin, interval_map_map& intervals_by_end, interval& i)
{
    //find all the intervals that begin with a
    interval_map_map::iterator it = intervals_by_begin.find(i.a);
    interval *new_interval;
    list<interval*> new_intervals;
    interval_int min_a, min_b, max_a, max_b;
    if(it!=intervals_by_begin.end()){
        //does this interval already exist?
        if(is_in(it->second,i.b)){
            if((it->second)[i.b].even==i.even){
                return true; //yes and parity is coherent
            }else{
                return false; //no and parity is not coherent
            }
        }
        //search for newly generated intervals
        for(interval_map::const_iterator iter = it->second.begin(); iter != it->second.end(); iter++){
            new_interval = new interval;
            min_b = min(iter->second.b,i.b);
            max_b = max(iter->second.b,i.b);
            new_interval->a = min_b+1;
            new_interval->b = max_b;
            new_interval->even = (i.even and iter->second.even) or (!i.even and !iter->second.even);
            new_intervals.push_back(new_interval);
        }
    }
    //add the current interval
    new_interval = new interval;
    new_interval->a = i.a;
    new_interval->b = i.b;
    new_interval->even = i.even;
    new_intervals.push_back(new_interval);
    //find all the intervals that end with b
    it = intervals_by_end.find(i.b);
    if(it!=intervals_by_end.end()){
        for(interval_map::const_iterator iter = it->second.begin(); iter != it->second.end(); iter++){
            new_interval = new interval;
            min_a = min(iter->second.a,i.a);
            max_a = max(iter->second.b,i.b)-1;
            new_interval->a = min_a;
            new_interval->b = max_a;
            new_interval->even = (i.even and iter->second.even) or (!i.even and !iter->second.even);
            new_intervals.push_back(new_interval);
        }
    }
    //find all the intervals that end with a-1
    if(i.a>1){
        it = intervals_by_end.find(i.a-1);
        if(it!=intervals_by_end.end()){
            for(interval_map::const_iterator iter = it->second.begin(); iter != it->second.end(); iter++){
                new_interval = new interval;
                new_interval->a = iter->second.a;
                new_interval->b = i.b;
                new_interval->even = (i.even and iter->second.even) or (!i.even and !iter->second.even);
                new_intervals.push_back(new_interval);
            }
        }
    }
    //find all the intervals that start with b+1
    it = intervals_by_begin.find(i.b+1);
    if(it!=intervals_by_begin.end()){
        for(interval_map::const_iterator iter = it->second.begin(); iter != it->second.end(); iter++){
            new_interval = new interval;
            new_interval->a = i.a;
            new_interval->b = iter->second.b;
            new_interval->even = (i.even and iter->second.even) or (!i.even and !iter->second.even);
            new_intervals.push_back(new_interval);
        }
    }
    interval_int a_tmp, b_tmp;
    bool even_tmp;
    //check coherence and eventually add the candidates
    for(list<interval*>::const_iterator iter = new_intervals.begin(); iter != new_intervals.end(); iter++ ){
        a_tmp = (*iter)->a;
        b_tmp = (*iter)->b;
        even_tmp = (*iter)->even;
        //cout << "Adding interval a:" << a_tmp << " b:" << b_tmp << " even:" << even_tmp << endl; 
        if(is_in(intervals_by_begin,a_tmp,b_tmp)){
            delete *iter;
            if(intervals_by_begin[a_tmp][b_tmp].even!=even_tmp)
                return false;  //pre-existing not coherent interval
        }else{
            intervals_by_begin[a_tmp][b_tmp] = **iter; //iterator to pointer, very evil
            intervals_by_end[b_tmp][a_tmp] = **iter;
            delete *iter;
        }
    }
    return true; //everything looked coherent to me
}

int main(){

    interval_int sequence_len;
    int pairs_number;
    interval_map_map intervals_by_begin, intervals_by_end;
    string tmp;
    vector<string> inputs;
    int conditions_counter;

    int counter = 0;

    while(read_sequence_length(cin,sequence_len)){
        bool not_absurd = true;
        cin >> pairs_number;
        //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        /*if(sequence_len==10 and pairs_number==3){
            //cout << "do it" << endl;
            pairs_number = 4;
        }*/
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
                not_absurd = add_interval(intervals_by_begin, intervals_by_end, cur_interval);
                if(not_absurd) conditions_counter++;
            }
        }
        counter++;
        cout << conditions_counter << endl;
        for(interval_map_map::iterator it1 = intervals_by_begin.begin(); it1 != intervals_by_begin.end(); it1++){
            it1->second.clear();
        }
        intervals_by_begin.clear();
        for(interval_map_map::iterator it1 = intervals_by_end.begin(); it1 != intervals_by_end.end(); it1++){
            it1->second.clear();
        }
        intervals_by_end.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
   

}
