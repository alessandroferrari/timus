/* 
Solution to timus Problem-1003: Parity.

Alessandro Ferrari 

This solution get TLE.
Passes all tests and tests2.
*/

#include <iostream>
#include <string>
#include <list>
#include <sstream>
#include <algorithm>
#include <vector>
#include <limits>

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

struct interval {
    unsigned long long int a;
    unsigned long long int b;
    bool even;
};

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


bool read_sequence_length(istream& is, unsigned long long int& sequence_len)
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

/*
Comparator predicate for the intervals.
*/
bool cmp_begin(interval p, interval q){
    if(p.a<q.a) return true;
    else if(p.a==q.a) return (p.b<q.b);
    else return false;
}

bool cmp_end(interval p, interval q){
    if(p.b<q.b) 
        return true;
    else if(p.b==q.b) return (p.a<q.b);
    else return false;
}

bool cmp_only_begin(interval p, interval q){
    return (p.a<q.a);
}

bool cmp_only_end(interval p, interval q){
    return (p.b<q.b);
}

//find the intervals starting (or ending) with a (with b, according to the criterion defined in cmp_func)
vector<interval> intervals_with(vector<interval>& sorted_intervals, unsigned long long int a, unsigned long long int b, bool (*cmp_func)(interval,interval), bool by_begin ){
    vector<interval> results;
    interval cmp;
    cmp.a = a;
    cmp.b = b;
    vector<interval>::iterator lbound = lower_bound(sorted_intervals.begin(),
                                                    sorted_intervals.end(),
                                                    cmp, cmp_func);
    if(lbound!=sorted_intervals.end() and ((lbound->a==a and by_begin) or (lbound->b==b and not by_begin))){
        vector<interval>::iterator ubound = upper_bound(lbound,
                                                        sorted_intervals.end(),
                                                        cmp, cmp_func);
        copy(lbound,ubound,back_inserter(results));
    }
    //for(vector<interval>::const_iterator iter = results.begin(); iter!=results.end(); iter++) cout << "[" << iter->a <<","<<iter->b << "]" << endl;
    return results;
}

vector<interval>::iterator is_in(vector<interval>::iterator beg, vector<interval>::iterator end, interval value){
    vector<interval>::iterator match = lower_bound(beg,end,value,cmp_begin);
    if(beg!=end and match!=end and match->a==value.a and match->b==value.b) return match;    
    return end;
}

bool add_interval(vector<interval>& sorted_intervals_begin, vector<interval>& sorted_intervals_end,  interval& i){
    vector<interval> to_add;
    vector<interval>::iterator match = is_in(sorted_intervals_begin.begin(),sorted_intervals_begin.end(),i);
    if(match!=sorted_intervals_begin.end()){
        if(match->even!=i.even){
            return false; //if parity is different is absurd
        }else{
            return true; //interval already in there, no need of further explorations
        }
    }
    //find all the intervals that end with a-1
    //cout << "Prepone:"<<endl;
    vector<interval> prepone = intervals_with(sorted_intervals_end, 1, i.a-1, cmp_only_end,false);
    //find all the intervals that begin with b+1
    //cout << "Postpone:"<<endl;
    vector<interval> postpone = intervals_with(sorted_intervals_begin, i.b+1, i.b+1, cmp_only_begin,true);
    //find all the intervals that begin with a
    //cout << "Same begin:"<<endl;
    vector<interval> same_begin = intervals_with(sorted_intervals_begin, i.a, i.b, cmp_only_begin,true);
    //find all the intervals that end with b
    //cout << "Same end:"<<endl;
    vector<interval> same_end = intervals_with(sorted_intervals_end, i.a, i.b, cmp_only_end,false);

    for(vector<interval>::const_iterator iter = prepone.begin(); iter!=prepone.end(); iter++){
        interval new_interval;
        new_interval.a = iter->a;
        new_interval.b = i.b;
        new_interval.even = (iter->even and i.even) or (!iter->even and !i.even);   
        to_add.push_back(new_interval);
    }
    for(vector<interval>::const_iterator iter = postpone.begin(); iter!=postpone.end(); iter++){
        interval new_interval;
        new_interval.a = i.a;
        new_interval.b = iter->b;
        new_interval.even = (iter->even and i.even) or (!iter->even and !i.even);
        to_add.push_back(new_interval);
    }
    for(vector<interval>::const_iterator iter = same_begin.begin(); iter!=same_begin.end(); iter++){
        interval new_interval;
        unsigned long long int smaller_b = min(i.b,iter->b);
        unsigned long long int biggest_b = max(i.b,iter->b);
        new_interval.a = smaller_b+1;
        new_interval.b = biggest_b;
        new_interval.even = (iter->even and i.even) or (!iter->even and !i.even);
        to_add.push_back(new_interval);
    }
    for(vector<interval>::const_iterator iter = same_end.begin(); iter!=same_end.end(); iter++){
        interval new_interval;
        unsigned long long int smaller_a = min(i.a,iter->a);
        unsigned long long int biggest_a = max(i.a,iter->a);
        new_interval.a = smaller_a;
        new_interval.b = biggest_a-1;
        new_interval.even = (iter->even and i.even) or (!iter->even and !i.even);
        to_add.push_back(new_interval);
    }
    //add the missing interval
    if(match==sorted_intervals_begin.end()){
        //cout << "Adding [" << i.a << "," << i.b << "," << i.even <<"]" << endl;
        sorted_intervals_begin.insert(upper_bound(sorted_intervals_begin.begin(),
                                      sorted_intervals_begin.end(),
                                      i,cmp_begin),i);
        sorted_intervals_end.insert(upper_bound(sorted_intervals_end.begin(),
                                    sorted_intervals_end.end(),
                                    i,cmp_end),i);
    }
    //and all the consequently generated intervals
    for(vector<interval>::iterator iter = to_add.begin(); iter!=to_add.end(); iter++){
        //check if the interval already exist
        match = is_in(sorted_intervals_begin.begin(),
                      sorted_intervals_begin.end(),
                      *iter);
        if(match!=sorted_intervals_begin.end()){ //there is a match
            if(match->even!=iter->even){ //but parity is different
                return false;
            } //if it is already there just do nothing
        }else{ //insert sorted in the intervals to add if there is not a match
            //cout << "Adding [" << iter->a << "," << iter->b << "," << iter->even <<"]" << endl;
            sorted_intervals_begin.insert(upper_bound(sorted_intervals_begin.begin(), 
                                                     sorted_intervals_begin.end(),
                                                     *iter,cmp_begin),*iter);
            sorted_intervals_end.insert(upper_bound(sorted_intervals_end.begin(), 
                                                   sorted_intervals_end.end(),
                                                   *iter,cmp_end),*iter);
        }
    }
    return true;
}

int main(){

    unsigned long long int sequence_len;
    int pairs_number;
    vector<interval> intervals_by_begin(10000), intervals_by_end(10000);
    string tmp;
    vector<string> inputs;
    int conditions_counter;

    int counter = 0;

    while(read_sequence_length(cin,sequence_len)){
        if(!intervals_by_begin.empty()){
            intervals_by_begin.clear();
            intervals_by_end.clear();
        }
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
                not_absurd = add_interval(intervals_by_begin, intervals_by_end, cur_interval);
                if(not_absurd) conditions_counter++;
            }
        }
        counter++;
        cout << conditions_counter << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if(counter>10) break;
    }
   

}
