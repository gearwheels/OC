#ifndef TOPOLOGY_HPP
#define TOPOLOGY_HPP

#include <iostream>
#include <list>
#include <memory>
#include <utility>

class node_t{
private:
    std::pair<long long,std::list<node_t>> id_node;
	using iterator = typename std::list<node_t>::iterator;
public:
    
	explicit node_t() noexcept : id_node() {}

    node_t(const long long & elem){
        id_node.first = elem;
    }

	bool insert(const long long & parent, const long long & elem) {

		for (iterator it1 = id_node.second.begin(); it1 != id_node.second.end(); ++it1) {// проходим по списку детей
			if ((*it1).get_id() == parent) { // если в этом списке найден родитель, вставляем к нему в список ребенка
				node_t new_node(elem);
                (*it1).id_node.second.push_back(new_node);
				return true;
			}else if(!(*it1).node_empty()){// если это не родитель, проверяем есть ли у данного узла дети,
			// если есть проверяем и вставляем в их списки
				if((*it1).insert(parent,elem)){
					return true;
				}
			}

		}
		return false;
	}

	bool insert(const long long & elem) {
		this->id_node.second.push_back(elem);
		return true;
	}

    bool erase(const long long & elem) {
        for (iterator it1 = this->id_node.second.begin(); it1 != this->id_node.second.end(); ++it1) {// проходим по списку детей
			if ((*it1).get_id() == elem) {
                if (!((*it1).node_empty())){// если список детей узла на который указывает итератор не пуст то передаем этот узел в last_elem_del 
                    long long id1 = (*it1).last_elem_del();
					(*it1).set_id(id1);
					return true;
                }else{
					this->id_node.second.erase(it1);
					return true;
				}
            }else if(not((*it1).node_empty())){ 
				if ((*it1).erase(elem)){
					return true;
				}
			}

        }
		return false;
    }

    long long get_id(){
		return this->id_node.first;
	}

	 void set_id(const long long & elem){
		this->id_node.first = elem;
	}

	long long last_elem_del(){
		if (this->id_node.second.front().node_empty()){// если полученый узел имеет детей а его первый ребенок не имеет детей
		// то запоминаем id первого ребенка и удаляем его
			long long id1 = this->id_node.second.front().get_id();
			this->id_node.second.pop_front();
			return id1;
		}else{
			return this->id_node.second.front().last_elem_del();
		}
		
	}
	long long find(const long long & elem) {
		long long ind = 0;
		for (iterator it1 = this->id_node.second.begin(); it1 != this->id_node.second.end(); ++it1) {
			if ((*it1).get_id() == elem){
				return ind;
			}else if(!(*it1).node_empty()){
				if ((*it1).find(elem) == 0){
					return ind;
				}
				
			}
		}
		return -1;
	}
	bool node_empty(){
		return this->id_node.second.empty();
	}

    ~node_t(){}
};

template<class L, class N> //   L - long long N - node t
class topology_t {
private:
	std::shared_ptr<N> root;
	size_t container_size;
public:
	explicit topology_t() noexcept : root(nullptr), container_size(0) {}
	~topology_t() {}

	bool insert(const L & parent, const L & elem) {
		if (this->root != nullptr){
			if((*root).get_id() == parent){
				return (*root).insert(elem);
			}else{
				return (*root).insert(parent, elem);
			}
		}
		return false;
	}
	void insert(const L & elem) {
		if (root == nullptr){
			N new_node(elem);
			root = std::make_shared<N>(new_node);
		}else{
			std::cout << "ERROR: Root exists" << std::endl;
		}
	}
	long long find(const L & elem) {
		long long ind = 0;
		if (this->root != nullptr){
			if((*root).get_id() == elem){
				return ind;
			}else{
				return (*root).find(elem);
			}
		}
		return -1; 
	}
		
	bool erase(const L & elem) {
		if (this->root != nullptr){
			if(((*root).get_id() == elem) and (not((*root).node_empty()))){// если удаляем корень
				long long id1 = (*root).last_elem_del(); // находим последний эл-т и удаляем, его id копируем в корень
				(*root).set_id(id1);
				return true;
			}else if(((*root).get_id() == elem) and ((*root).node_empty())){
				this->root = nullptr;
				return true;
			}else if(((*root).get_id() != elem) and (not((*root).node_empty()))){
				return (*root).erase(elem);
			}
		}
		return false;
	}

};

#endif