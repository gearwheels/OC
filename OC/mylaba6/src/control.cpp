#include <unistd.h>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <time.h>



#include "topology.hpp"
#include "zmq_std.hpp"

using node_id_type = long long;

struct thr_data_t{
	std::shared_ptr<std::vector< std::pair<void*, void*> >> childs;
	node_id_type iter;
	int ind;
};

struct thr_data_t thr_data;

int main() {
	int rc;
	topology_t<node_id_type, node_t> control_node;
	std::vector< std::pair<void*, void*> > childs;
	std::vector<node_id_type> ids;

	

	std::string s;
	node_id_type id;
	while (std::cin >> s >> id) {
		if (s == "create") {
			node_id_type parent_id;
			std::cin >> parent_id;
			if (parent_id == -1) {
				void* new_context = NULL;
				void* new_socket = NULL;
				zmq_std::init_pair_socket(new_context, new_socket);
				rc = zmq_bind(new_socket, ("tcp://*:" + std::to_string(PORT_BASE + id)).c_str());
				assert(rc == 0);

				int fork_id = fork();
				if (fork_id == 0) {
					rc = execl(NODE_EXECUTABLE_NAME, NODE_EXECUTABLE_NAME, std::to_string(id).c_str(), NULL);
					assert(rc != -1);
					return 0;
				} else {
					bool ok = true;
					node_token_t reply_info({fail, id, id});
					ok = zmq_std::recieve_msg_wait(reply_info, new_socket);

					node_token_t* token = new node_token_t({ping, id, id});
					node_token_t reply({fail, id, id});
					ok = zmq_std::send_recieve_wait(token, reply, new_socket);
					if (ok and reply.action == success) {
						childs.push_back(std::make_pair(new_context, new_socket));
						control_node.insert(id);
						std::cout << "OK: " << reply_info.id << std::endl;
						ids.push_back(id);
					} else {
						rc = zmq_close(new_socket);
						assert(rc == 0);
						rc = zmq_ctx_term(new_context);
						assert(rc == 0);
					}
				}
			} else if (control_node.find(parent_id) == -1) {
				std::cout << "Error: Not found" << std::endl;
			} else {
				if (control_node.find(id) != -1) {
					std::cout << "Error: Already exists" << std::endl;
				} else {
					int ind = control_node.find(parent_id);
					node_token_t* token = new node_token_t({create, parent_id, id});
					node_token_t reply({fail, id, id});
					if (zmq_std::send_recieve_wait(token, reply, childs[ind].second) and reply.action == success) {
						std::cout << "OK: " << reply.id << std::endl;
						ids.push_back(id);
						control_node.insert(parent_id, id);
					} else {
						std::cout << "Error: Parent is unavailable" << std::endl;
					}
				}
			}
		} else if (s == "remove") {
			int ind = control_node.find(id);
			//std::cout << control_node << std::endl;
			if (ind != -1) {
				node_token_t* token = new node_token_t({destroy, id, id});
				node_token_t reply({fail, id, id});
				bool ok = zmq_std::send_recieve_wait(token, reply, childs[ind].second);
				if (reply.action == destroy and reply.parent_id == id) {
					rc = zmq_close(childs[ind].second);
					assert(rc == 0);
					rc = zmq_ctx_term(childs[ind].first);
					assert(rc == 0);
					std::vector< std::pair<void*, void*> >::iterator it = childs.begin();
					while (ind--) {
						++it;
					}
					childs.erase(it);
				} else if (reply.action == bind and reply.parent_id == id) {
					rc = zmq_close(childs[ind].second);
					assert(rc == 0);
					rc = zmq_ctx_term(childs[ind].first);
					assert(rc == 0);
					zmq_std::init_pair_socket(childs[ind].first, childs[ind].second);
					rc = zmq_bind(childs[ind].second, ("tcp://*:" + std::to_string(PORT_BASE + reply.id)).c_str());
					assert(rc == 0);
				}
				if (ok) {
					control_node.erase(id);
					for (auto iter = ids.begin(); iter != ids.end(); ++iter){
						if (*iter == id){
							ids.erase(iter);
							break;
						}
					}
					std::cout << "OK" << std::endl;
				} else {
					std::cout << "Error: Node is unavailable" << std::endl;
				}
			} else {
				std::cout << "Error: Not found" << std::endl;
			}
		} else if (s == "ping") {
			int ind = 0;
			int tmp = 0;
			double start = 0, end = 0;
			std::cout << "start = " << start <<std::endl;
    		start = clock();
			std::cout << "start = " << start <<std::endl;
			for(;;){
				std::cout << "start = " << start <<std::endl;
				for (auto iter = ids.begin(); iter != ids.end(); ++iter){
					ind = control_node.find(*iter);
					if (ind == -1){
						std::cout << "Error: Not found" << *iter << std::endl;
					}else{
						node_token_t* token = new node_token_t({ping, *iter, *iter});
						node_token_t reply({fail, *iter, *iter});
						if (zmq_std::send_recieve_wait(token, reply, childs[ind].second) and reply.action == success) {
							std::cout << "OK:" << *iter << std::endl;
						} else {
							std::cout << "Heartbit: node" << *iter << "is unavailable now" << std::endl;
						}
					}
					++tmp;
				}
				std::cout << "end = " << end <<std::endl;
				end = clock();
				std::cout << "end = " << end <<std::endl;
				if(start > 4*id or end > 4*id){
					break;
				}
			}		
		} else if (s == "back") {
			int ind = control_node.find(id);
			if (ind != -1) {
				node_token_t* token = new node_token_t({back, id, id});
				node_token_t reply({fail, id, id});
				if (zmq_std::send_recieve_wait(token, reply, childs[ind].second)) {
					if (reply.action == success) {
						node_token_t* token_back = new node_token_t({back, id, id});
						node_token_t reply_back({fail, id, id});
						std::vector<unsigned int> calculated;
						while (zmq_std::send_recieve_wait(token_back, reply_back, childs[ind].second) and reply_back.action == success) {
							calculated.push_back(reply_back.id);
						}
						if (calculated.empty()) {
							std::cout << "OK: " << reply.id << " : -1" << std::endl;
						} else {
							std::cout << "OK: " << reply.id << " : ";
							for (size_t i = 0; i < calculated.size() - 1; ++i) {
								std::cout << calculated[i] << ", ";
							}
							std::cout << calculated.back() << std::endl;
						}
					} else {
						std::cout << "Error: No calculations to back" << std::endl;
					}
				} else {
					std::cout << "Error: Node is unavailable" << std::endl;
				}
			} else {
				std::cout << "Error: Not found" << std::endl;
			}
		} else if (s == "exec") {
			std::string pattern, text;
			std::cin >> pattern >> text;
			int ind = control_node.find(id);
			if (ind != -1) {
				bool ok = true;
				std::string text_pattern = pattern + SENTINEL + text + SENTINEL;
				for (size_t i = 0; i < text_pattern.size(); ++i) {
					node_token_t* token = new node_token_t({exec, text_pattern[i], id});
					node_token_t reply({fail, id, id});
					if (!zmq_std::send_recieve_wait(token, reply, childs[ind].second) or reply.action != success) {
						ok = false;
						break;
					}
				}
				if (ok) {
					std::cout << "OK" << std::endl;
				} else {
					std::cout << "Error: Node is unavailable" << std::endl;
				}
			} else {
				std::cout << "Error: Not found" << std::endl;
			}
		}
	}

	for (size_t i = 0; i < childs.size(); ++i) {
		rc = zmq_close(childs[i].second);
		assert(rc == 0);
		rc = zmq_ctx_term(childs[i].first);
		assert(rc == 0);
	}
}