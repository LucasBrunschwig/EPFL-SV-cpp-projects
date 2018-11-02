#include "network.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include "random.h"
#include <iostream>

/*Modification test we want everything*/


typedef std::multimap<size_t, size_t>::iterator MMAPIterator;
typedef std::multimap<size_t, size_t>::const_iterator cMMAPIterator;

void Network::resize(const size_t& _n) {
	if( _n >= 0) {
		if(values.size() == 0) {  
		values = std::vector<double>(_n,0); 
		RNG.normal(values);
		}
		else if(values.size() < _n) {
		for(size_t i(values.size()) ; i < _n ; i++) values.push_back(RNG.normal());
		}
		else if(values.size() > _n) {
			for(size_t i(values.size()) ; i > _n ; i--) values.pop_back();
		}
	}
	
}

bool Network::add_link(const size_t& _a, const size_t& _b) {
	
	if(_a!=_b  and _a>=0 and _b>=0 and _a < values.size() and _b < values.size()) {
		
	std::pair<MMAPIterator, MMAPIterator> result = links.equal_range(_a);
	
		for (MMAPIterator it = result.first; it != result.second; it++) {
			if(it->second == _b) return false; //la pair existe déja!
		}
		
		links.insert(std::make_pair(_a,_b));
		links.insert(std::make_pair(_b,_a));
		return true;
	}
	else return false;
}

size_t Network::random_connect(const double& mean) {
	size_t nLinks(0);
	links.clear();
	std::vector<int> nodes(values.size()), degrees(values.size());
	RNG.poisson(degrees, mean);
	
	for(size_t i(0) ; i < values.size() ; i++) {
	size_t degree(degrees[i]);
	if(degree > nodes.size()) degree=nodes.size();
			for(size_t j(0) ; j < degree ; j++)  {
				RNG.uniform_int(nodes, 0, values.size());
				if(add_link(i,nodes[j]))  { nLinks++;}
				else if(degree < values.size()) degree++;
			}
	}
	return nLinks;
}

size_t Network::set_values(const std::vector<double>& next_values) {  
	
	for(auto& I : values) I=0;
	
			if(next_values.size() == values.size()) {
				values = next_values;
				return values.size();
			} else if (next_values.size() < values.size()) {
				for(size_t i(0) ; i < next_values.size() ; i++) {values[i]=next_values[i];}
				return next_values.size();
			} else  {
				for(size_t i(0) ; i < values.size() ; i++) {values[i]=next_values[i];}
				return values.size();
			}
}
 
size_t Network::size() const { return values.size(); }

size_t Network::degree(const size_t &_n) const {
	 if(_n < values.size() and _n >= 0) return links.count(_n);
	 else return 0;
}

double Network::value(const size_t &_n) const { 
	if(_n < values.size() and _n >= 0 ) return values[_n]; 
	else return 0;
}

std::vector<double> Network::sorted_values() const {
	std::vector<double> sortValues(values);
	std::sort(sortValues.begin(), sortValues.end(), std::greater<double>());
	return sortValues;}

std::vector<size_t> Network::neighbors(const size_t& _a) const { 
	std::vector<size_t> Neighbors;
	
	if(_a < values.size() and _a >= 0) {
	
	std::pair<cMMAPIterator, cMMAPIterator> result = links.equal_range(_a);
	
		for (cMMAPIterator it = result.first; it != result.second; it++) {
			Neighbors.push_back(it->second);
		}
	}
	
	return Neighbors;
	
	
	}
