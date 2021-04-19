#ifndef TOPOLOGY_HPP
#define TOPOLOGY_HPP

#include <iostream>
#include <list>
#include <memory>

/*struct control_node
{
    std::shared_ptr<node> _node = std::make_shared<node>();
};*/


template<class L, class N> //   L - long long N - node t  
class node_t{
private:
	using iterator = typename std::list<std::shared_ptr<N>>::iterator
    std::list<std::shared_ptr<N>> children;
    long long id;
public:
    
	explicit node_t() noexcept : children(), id(0) {}
	node_t (L id1){
		id = id1; 
	}

	bool insert(const L & parent, const L & elem) {

		for (iterator it1 = children.begin(); it1 != children.end(); ++it1) {// проходим по списку детей
			if (it1->id == parent) { // если в этом списке найден родитель, вставляем к нему в список ребенка
				node_t new_node(elem);
				std::shared_ptr<node_t> new_node_ptr = std::make_shared(new_node);
				it1->children.push_back(new_node_ptr);
				return true;
			}else if(!it1.children.empty()){// если это не родитель, проверяем есть ли у данного узла дети,
			// если есть проверяем и вставляем в их списки
				if(it1.insert(parent,elem)){
					return true;
				}
			}

		}
		return false;
	}

	void gluing(iterator it1){

		//this->children;
		
	}

	bool erase(const L & elem) {

		for (iterator it1 = children.begin(); it1 != children.end(); ++it1) {// проходим по списку детей
			if (it1->id == elem) { // если в этом списке найден elem, берем скписок этого elem и копируем в children
				//gluing(&it1);
				(*it1).last_element();


				node_t new_node(elem);
				std::shared_ptr<node_t> new_node_ptr = std::make_shared(new_node);
				it1->children.push_back(new_node_ptr);
				return true;
			}else if(!it1.children.empty()){// если это не родитель, проверяем есть ли у данного узла дети,
			// если есть проверяем и вставляем в их списки
				if(it1.insert(parent,elem)){
					return true;
				}
			}
		}



		for (list_iterator it1 = container.begin(); it1 != container.end(); ++it1) {
			for (iterator it2 = it1->begin(); it2 != it1->end(); ++it2) {
				if (*it2 == elem) {
					if (it1->size() > 1) {
						it1->erase(it2);
					} else {
						container.erase(it1);
					}
					--container_size;
					return true;
				}
			}
		}



		return false;
	}
	long long get_id(){
		return this->id;
	}

	long long last_element(){
		iterator it1 = this->children.begin();
		if (!it1.children.empty()){
			return it1->last_element()
		}else{
			return it1->id;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	long long find(const T & elem) {
		long long ind = 0;
		for (list_iterator it1 = container.begin(); it1 != container.end(); ++it1) {
			for (iterator it2 = it1->begin(); it2 != it1->end(); ++it2) {
				if (*it2 == elem) {
					return ind;
			    /* if (!it2.empty){
					find2(it2,elem)
				}
				*/
				}
			}
			++ind; // придумать что-то с индексом
		}
		return -1;
	}
	/*
	find2(const T & par, const T & elem){
		for (list_iterator it1 = par.begin(); it1 != par.end(); ++it1) {
			if (*it2 == elem) {
					return ind;
			}else if (!it2.empty){
				find2(it2,elem)
			}
					
			
			++ind;// придумать что-то с индексом
		}
	}
	*/

	size_t size() {
		return container_size;
	}

	template<class U>
	friend std::ostream & operator << (std::ostream & of, const topology_t<U> & top) {
		for (auto it1 = top.container.begin(); it1 != top.container.end(); ++it1) {
			of << "{";
			for (auto it2 = it1->begin(); it2 != it1->end(); ++it2) {
				of << *it2 << " ";
			}
			of << "}" << std::endl;
		}
		return of;
	}

    ~node_t(){}
};

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


template<class T, class T1>//   T- long long   T1 - node t
class topology_t {
private:
	//using list_type = std::list<T>;
	//using iterator = typename T::iterator; // t is std::list<long long>
	//using list_iterator = typename list_type::iterator;
	
	//list_type container;
	size_t container_size;
	std::shared_ptr<T1> root;
public:

	explicit topology_t() noexcept : root(nullptr), container_size(0) {}
	topology_t(std::shared_ptr<T> new_root){
		this->root(new_root);
		//this->root = new_root;
		++container_size;
	}
	~topology_t() {}

	bool insert(const T & parent, const T & elem) {// ДОСТУП К ЭЛЕМЕНТАМ !!!!!!!!!!!
		if (this->root->id == parent){
			root.children.push_back(elem);
			return true;
			++container_size;
		}
		return root.insert(parent,elem); // ++container_size;
	}
	void insert(const T & elem) {
		T1 new_root(elem);
		this->root = std::make_shared<T1>(new_root) // T1 - node t
		++container_size;
		//container.push_back(new_list);
	}

	//long long last_element(){
	//	this->
	//}

	bool erase(const T & elem){ // ДОСТУП К ЭЛЕМЕНТАМ !!!!!!!!!!!
		if (root->id == elem){


			std::list<std::shared_ptr<T1>>::iterator it1;
			it1 = root.children.begin()
			root->id = it1->id;
			root.children.pop_front();
			return true;
		}
		return root.erase(elem);
	}
///////////////////////////////////////////////////////????????????????????????????????????
	long long find(const T & elem) {
		long long ind = 0;
		for (list_iterator it1 = container.begin(); it1 != container.end(); ++it1) {
			for (iterator it2 = it1->begin(); it2 != it1->end(); ++it2) {
				if (*it2 == elem) {
					return ind;
			    /* if (!it2.empty){
					find2(it2,elem)
				}
				*/
				}
			}
			++ind; // придумать что-то с индексом
		}
		return -1;
	}
	/*
	find2(const T & par, const T & elem){
		for (list_iterator it1 = par.begin(); it1 != par.end(); ++it1) {
			if (*it2 == elem) {
					return ind;
			}else if (!it2.empty){
				find2(it2,elem)
			}
					
			
			++ind;// придумать что-то с индексом
		}
	}
	*/

	

	size_t size() {
		return container_size;
	}

	template<class U>
	friend std::ostream & operator << (std::ostream & of, const topology_t<U> & top) {
		for (auto it1 = top.container.begin(); it1 != top.container.end(); ++it1) {
			of << "{";
			for (auto it2 = it1->begin(); it2 != it1->end(); ++it2) {
				of << *it2 << " ";
			}
			of << "}" << std::endl;
		}
		return of;
	}
};


#endif /* TOPOLOGY_HPP */


