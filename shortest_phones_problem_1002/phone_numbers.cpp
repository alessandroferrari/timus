/*
Solution to timus Problem-1002: Phone numbers.

Alessandro Ferrari
*/
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <limits>

using std::cin;
using std::cout;
using std::sort;
using std::endl;
using std::vector;
using std::string;
using std::istream;
using std::equal;
using std::map;
using std::transform;
using std::search;
using std::back_inserter;
using std::stringstream;
using std::numeric_limits;
using std::fill_n;

string escape_number = "-1";

bool read_phone_number(istream& is, string& phone_number)
{
    is >> phone_number;
    if (phone_number.size()==escape_number.size()){
        if (equal(phone_number.begin(),phone_number.end(),escape_number.begin())){
            return false;
        }
    }
    return true;
}

typedef map<char,char> letter2num_map;

letter2num_map create_mapping(){
    
    letter2num_map mapping;

    mapping['i'] = '1';
    mapping['j'] = '1';
    
    mapping['a'] = '2';
    mapping['b'] = '2';
    mapping['c'] = '2';

    mapping['d'] = '3';
    mapping['e'] = '3';
    mapping['f'] = '3';

    mapping['g'] = '4';
    mapping['h'] = '4';

    mapping['k'] = '5';
    mapping['l'] = '5';
    
    mapping['m'] = '6';
    mapping['n'] = '6';

    mapping['p'] = '7';
    mapping['r'] = '7';
    mapping['s'] = '7';

    mapping['t'] = '8';
    mapping['u'] = '8';
    mapping['v'] = '8';

    mapping['w'] = '9';
    mapping['x'] = '9';
    mapping['y'] = '9';

    mapping['o'] = '0';
    mapping['q'] = '0';
    mapping['z'] = '0';

    return mapping;
}

letter2num_map mapping = create_mapping();

char letter2num_func(const char& c)
{
    return mapping[c];
}

string map_word(const string& word)
{
    string number;
    transform(word.begin(), word.end(), back_inserter(number), letter2num_func);   
    return number;
}

vector<vector<string> > find_matches(const string& phone_number,vector<string>& numbers, int paths_costs[]){

    string number;
    vector<vector<string> > matches;
    int new_cost, node_offset;
    for(vector<string>::iterator i = numbers.begin(); i!=numbers.end(); i++){
        number = *i;   
        vector<string> sequence;
        //check if it is a good last word for the sequence
        if (phone_number.size()==number.size() and equal(number.begin(),number.end(),phone_number.begin())){
            new_cost = paths_costs[0] + 1;
            node_offset = number.size();
            //close the sequence only if there are not better solution at that point (dynamic programming - prune sub-optimal branches)
            if(new_cost<paths_costs[node_offset]){
                //update cost and finalize current path
                paths_costs[node_offset] = new_cost;
                sequence.push_back(*i);
                matches.push_back(sequence);
            }
        //check if it is not last good middle sub-sequence
        }else if(phone_number.size()>number.size()){
            if(equal(number.begin(),number.end(),phone_number.begin())){
                new_cost = paths_costs[0] + 1;
                node_offset = number.size();
                //extend sequence only if there are not better solution at that point
                if(new_cost<paths_costs[node_offset]){
                    paths_costs[node_offset] = new_cost;
                    //recursion
                    vector<vector<string> > sub_matches = find_matches(string(phone_number.begin()+number.size(),phone_number.end()),numbers,paths_costs+node_offset);
                    if (sub_matches.size()>0){
                        //add valid paths
                        for(vector<vector<string> >::iterator sub_iter = sub_matches.begin(); sub_iter != sub_matches.end(); sub_iter++){
                            sub_iter->push_back(number);
                            matches.push_back(*sub_iter);
                        }
                    }
                }
            }
        }  
    }

    return matches;
}

bool len_cmp(const string& i, const string& j) { return i.size()<j.size(); }

string find_words(string& phone_number, vector<string>& words)
{

    //map the words to numbers, but keep it reversible
    string number;
    vector<string> numbers;
    map<string,string> numbers2words;
    for(vector<string>::iterator i = words.begin(); i != words.end(); i++){
        number = map_word(*i);
        numbers.push_back(number);
        numbers2words[number]=*i;
    }
     
    //from no path to path to the end (n+1 states)
    int paths_costs[phone_number.size()+1];
    fill_n(paths_costs,phone_number.size()+1,numeric_limits<int>::max());
    paths_costs[0] = 0; //current cost is null, because it is the empty path

    //start with the recursive solution
    vector<vector<string> > matched_sequences = find_matches(phone_number, numbers, paths_costs);
    
    //reverse un-roll of the found solutions
    vector<string> matched_str;
    for(vector<vector<string> >::iterator i = matched_sequences.begin(); i!=matched_sequences.end(); i++){
        stringstream ss;
        vector<string>::size_type sz = i->size();
        if(sz>0){
            ss << numbers2words[(*i)[sz-1]];
            for(vector<string>::reverse_iterator j = i->rbegin() + 1; j != i->rend(); j++){
                ss << " " <<  numbers2words[*j];
            }
            matched_str.push_back(ss.str());
        }
    }
    //if there are not good candidates
    if(matched_str.size()==0){
       return "No solution.";
    }else{ //or find the shortest sequence
       string str_min = *std::min_element(matched_str.begin(), 
                                          matched_str.end(), 
                                          len_cmp);
       return str_min;
    }
}

bool len_cmp_reverse(const string& i, const string& j) { return i.size()>j.size(); }

int main()
{
    letter2num_map mapping = create_mapping();
    string phone_number;
    int num_words;
    vector<string> words;

    string word;

    while (read_phone_number(cin,phone_number)){
        cin >> num_words;
        if (words.size()>0){
            words.clear();
        }
        for(int i=0;i<num_words;i++){
           cin >> word;
           words.push_back(word);
        }
        sort(words.begin(),words.end(),len_cmp_reverse);
        cout << find_words(phone_number,words) << endl;
    }
}
