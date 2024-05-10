/**
 * @author Geoffrey Koech
 * 
 * @brief My implementation of some basic data structures.
 * @version 1.0
 * 
 * @package data_structures
 * @namespace data_structures
 * 
*/



#ifndef EOF
#include <iostream>
#endif
#include <stdexcept>
#include "useful_functions.h"





namespace data_structures {

	namespace {


		template <typename data_> class node_root {

			protected:
				data_ this_data;
			
			public:

				node_root(){}

				node_root(data_ new_data) : this_data(new_data){
				}

				void set_data(data_ d) {
					this->this_data = d;
				}

				data_ get_data() const {
					return this->this_data;
				}

				data_ &get_data_ref() {
					return this->this_data;
				}

		};


		template <typename data_, typename indx_ = signed long> class numbered_node : public node_root<data_> {


			private:
				indx_ index;



			protected:

				void set_index(indx_ index) {
					this->index = index;
				}


				indx_ get_index() const {
					return this->index;
				}

			public:

				numbered_node() {
				}

				numbered_node(data_ new_data, indx_ index = -1) : node_root<data_>(new_data) {
					this->index = index;
				}

		};


		template <typename data_> class linear_node : public node_root<data_> {
			
			protected:
				linear_node<data_>* next_node;
				linear_node<data_>* previous_node;

			public:

				linear_node() {
					this->next_node = nullptr;
				}

				linear_node(data_ new_data, linear_node<data_>* next = nullptr, linear_node<data_>* previous = nullptr) {
					this->this_data = new_data;
					this->next_node = next;
					this->previous_node = previous;
				}

				void set_next(linear_node<data_>* next) {
					this->next_node = next;
				}

				linear_node<data_>* get_next() const {
					return this->next_node;
				}

				void set_previous(linear_node<data_>* previous) {
					this->previous_node = previous;
				}

				linear_node<data_>* get_previous() const {
					return this->previous_node;
				}

		};


		template <typename data_> class bst_node : public node_root<data_> {

			private:
				bst_node<data_> *left_child, *right_child;

			public:

				bst_node() {
					this->left_child = this->right_child = nullptr;
				}

				bst_node(data_ new_data) {
					this->this_data = new_data;
					this->left_child = this->right_child = nullptr;
				}

				void set_left_child(bst_node<data_>* left) {
					this->left_child = left;
				}

				void set_right_child(bst_node<data_>* right) {
					this->right_child = right;
				}

				bst_node<data_>* get_left_child() {
					return this->left_child;
				}

				bst_node<data_>* get_right_child() {
					return this->right_child;
				}

		};

	}


	template <typename data_> class linear_linked_list {


		private:
			linear_node<data_> *front, *rear, *frame;
			unsigned long size, frame_index;

			void node_shifter(linear_node<data_>* this_node, unsigned long node_index, signed long by) {

				signed long start_index = (signed long) node_index;
				while ((by != 0) && (this_node != nullptr) && (node_index != by)) {
					if (by > 0) {
						this_node = this_node->get_next();
						node_index = node_index + 1;
					}
					else {
						this_node = this_node->get_previous();
						node_index = node_index - 1;
					}
				}
			}

			void frame_shifter(signed long by) {
				while ((by != 0) && (this->frame != nullptr)) {
					this->frame = (by > 0) ? this->frame->get_next() : this->frame->get_previous();
					this->frame_index = (by > 0) ? this->frame_index + 1 : this->frame_index - 1;
					by = (by > 0) ? by - 1 : by + 1;
				}
			}


		public:

			linear_linked_list() {
				this->front = this->rear = this->frame = nullptr;
				this->size = this->frame_index = 0;
			}


			linear_linked_list(data_ new_data) {
				this->front = this->rear = this->frame = new linear_node<data_>(new_data);
				this->size = 1;
				this->frame_index = 0;
			}

			// Copy Constructor

			linear_linked_list(linear_linked_list<data_>& other_list) {
				if (this == &other_list) {
					return;
				}
				this->front = this->rear = this->frame = nullptr;
				this->size = 0;
				unsigned long index;
				for (index = 0; index < other_list.length(); index = index + 1) {
					this->push(other_list.peek(index));
				}
			}


			~linear_linked_list() {
				this->reset();
			}

			// Operator Overloading.

			// Comparison operators
			bool operator==(linear_linked_list<data_>& other) {
				// fprintf(stderr, "== operator not yet implemented\n");
				// exit(EXIT_FAILURE);
				if (this == &other) {
					return true;
				}

				if (this->size == other.size) {
					signed long index;
					for (index = 0; index < this->size; index = index + 1) {
						if (this->peek(index) != other.peek(index)) {
							return false;
						}
					}
					return true;
				}
				return false;
			}

			bool operator!=(linear_linked_list<data_>& other) {
				// return (*this == other) ? false : true;
				if (this != *other) {
					return true;
				}
				unsigned long other_index = 0;

				for (this->frame = this->front, this->frame_index = 0; this->frame != nullptr; this->frame = this->frame->get_next(), this->frame_index = this->frame_index + 1) {
					if (this->frame->get_data() == other[this->frame_index]) {
						return false;
					}
				}
				return true;
			}

			bool operator<=(linear_linked_list<data_>& other) {
				// fprintf(stderr, "<= operator not yet implemented\n");
				// exit(EXIT_FAILURE);
				if (this->size <= other.size) {
					unsigned long index;
					for (index = 0; index < this->size; index = index + 1) {
						if (other.contains(this->peek(index)) == -1) {
							return false;
						}
					}
					return true;
				}
				return false;
			}

			bool operator>=(linear_linked_list<data_>& other) {
				// fprintf(stderr, "=> operator not yet implemented\n");
				// exit(EXIT_FAILURE);
				if (this->size >= other.size) {
					unsigned long index;
					for (index = 0; index < other.size; index = index + 1) {
						if (this->contains(other[index]) == -1) {
							return false;
						}
					}
					return true;
				}
				return false;
			}

			data_& operator [](signed long index) {
				try {
					this->peek(index);
					return this->frame->get_data_ref();
				}
				catch (std::range_error except) {
					throw except;
				}
			}

			// assignment operator
			linear_linked_list<data_> operator=(linear_linked_list<data_> other) {
				// this->reset();
				signed long index;
				for (index = 0; index < other.length(); index = index + 1) {
					this->push(other[index]);
				}
				return *this;
			}

			// arithmetic operators
			linear_linked_list<data_> operator+(linear_linked_list<data_> other) {
				linear_linked_list<data_> the_answer(*this);
				signed long index;
				for (index = 0; index < other.length(); index = index + 1) {
					the_answer.push(other[index]);
				}
				return the_answer;
			}

			

			/**
			 * @brief Check if the current linked list is empty or not.
			 * 
			 * @returns `(bool)` : true if the linked list is empty, false otherwise.
			*/
			bool empty() const {
				return (this->size == 0);
			}


			/**
			 * @brief Get the size of the linked list.
			 * 
			 * @returns `(unsigned long)` : The length of the linked list.
			*/
			unsigned long length() const {
				return this->size;
			}


			/**
			 * @brief Empties out the linked list and frees up memory.
			 * 
			 * @returns void.
			*/
			void reset() {
				// fprintf(stdout, "Inside reset()\n");
				this->frame = this->front;
				while (this->frame != nullptr) {
					this->front = this->frame->get_next();
					delete this->frame;
					this->frame = this->front;
					this->size = this->size - 1;
				}
				this->front = this->rear = this->frame = nullptr;
				this->frame_index = 0;
				// fprintf(stdout, "Done resetting.\n");
			}


			/**
			 * @brief Push new data onto the linked list.
			 * 
			 * @note Can push to negative indexes. Negative indexes will push from the back of the list.
			 * 
			 * @param new_data `(Generic)` : The new data to be pushed onto the linked list.
			 * 
			 * @param index `(signed long)` : The index where the data should be pushed to. Defaults to -1.
			 * 
			 * @returns void.
			 * 
			 * @throws Throws a `std::range_error` if the index passed in references an index greater than the current size.
			 * 
			*/
			void push(data_ new_data, signed long index = -1) {
				unsigned long add_index = (index < 0) ? (this->size - (unsigned long) useful_functions::absolute<signed long>(index)) + 1 : (unsigned long) useful_functions::absolute<signed long>(index);
				if (add_index > this->size) {
					throw std::range_error("Cannot push data to negative index whose absolute value converts to a larger value than the size of the list");
				}
				
				linear_node<data_>* new_node = new linear_node<data_>(new_data);
				
				if (this->size == 0) {
					this->front = this->rear = this->frame = new_node;
				}

				else if (this->size == 1) {

					if (add_index == 0) {
						this->front->set_previous(new_node);
						this->front->get_previous()->set_next(this->front);
						this->front = this->front->get_previous();
						this->frame = this->front;
					}

					else {
						// index is 1
						this->rear->set_next(new_node);
						this->rear->get_next()->set_previous(this->rear);
						this->rear = this->rear->get_next();
						this->frame = this->rear;
					}
				}

				else {
					
					if (add_index == 0) {
						this->front->set_previous(new_node);
						this->front->get_previous()->set_next(this->front);
						this->front = this->front->get_previous();
						this->frame = this->front;
						this->frame_index = 0;
					}
					else if (add_index == this->size) {
						this->rear->set_next(new_node);
						this->rear->get_next()->set_previous(this->rear);
						this->rear = this->rear->get_next();
						this->frame = this->rear;
						this->frame_index = this->size;
					}
					else {
						// this is where some extra work comes into play.
						signed long from_first = (signed long) add_index, from_rear = (signed long) (this->size - add_index), from_frame = (signed long) add_index - (signed long) add_index;
						unsigned long frame_abs = (from_frame < 0) ? ((unsigned long) (-1 * from_frame)) : (unsigned long) from_frame;
						signed long* signed_lists[] = {&from_first, &from_rear, &from_frame};
						useful_functions::selection_sort<signed long>(signed_lists, 3, true);
						
						if (signed_lists[0] == &from_first) {
							// shortest distance is from the front
							this->frame = this->front;
							this->frame_index = 0;
							this->frame_shifter(from_first);
						}
						else if (signed_lists[0] == &from_rear) {
							// shoftest distance is from the rear
							this->frame = this->rear;
							this->frame_index = this->size - 1;
							this->frame_shifter((signed long) ( -1 * from_rear));
						}
						else {
							// shortest distance is from the frame
							// fprintf(stdout, "Inside else branch of the push method. Pushing data towards the center of the list\n");
							this->frame_shifter(from_frame);
						}

						// Now frame should be at the proper index.
						new_node->set_previous(this->frame->get_previous());
						this->frame->get_previous()->set_next(new_node);
						this->frame->set_previous(new_node);
						new_node->set_next(this->frame);
						this->frame = this->frame->get_previous();

					}
				}
				this->size = this->size + 1;
			}


			/**
			 * @brief Peek at the data inside the linked list at a specific index.
			 * 
			 * @param index `(Generic)` : Defaults to -1. The index whose data is to be looked up.
			 * 
			 * @note This method can accept negative indexes. If the index references data 
			 * outside the range of the linked list, a length error is thrown.
			 * 
			 * @returns `(Generic)` : The data at the index specified.
			*/
			data_ peek(signed long index = -1) {
				unsigned long peek_index = (index < 0) ? (this->size - ((unsigned long) useful_functions::absolute<signed long>(index))) : (unsigned long) index;
				// fprintf(stdout, "peek_index : %lu\n", peek_index);
				if (this->size == 0) {
					throw std::length_error("linear linked list is empty.");
				}
				// fprintf(stdout, "peek_index is %lu\n", peek_index);
				if (peek_index == 0) {
					this->frame = this->front;
					this->frame_index = 0;
				}
				else if (peek_index == this->size - 1) {
					// fprintf(stdout, "peeking rear... peek_index is %lu\n", peek_index);
					this->frame = this->rear;
					this->frame_index = this->size - 1;
				}
				else {
					// This is where some extra work comes into play.
					// this is constly on a smaller linked list,
					// but the payoff is big for a larger linked list.
					unsigned long from_first = peek_index, from_rear = this->size - 1 - peek_index;
					unsigned long from_frame = useful_functions::max_data<unsigned long>(peek_index, frame_index) - useful_functions::min_data<unsigned long>(peek_index, frame_index);
					unsigned long* distances[] = {&from_first, &from_rear, &from_frame};
					useful_functions::insertion_sort<unsigned long>(distances, 3, true);
					if (distances[0] == &from_first || (distances[0] == &from_frame && from_first == from_frame)) {
						this->frame = this->front;
						this->frame_index = 0;
						this->frame_shifter((signed long) from_first);
					}
					else if (distances[0] == &from_rear || (distances[0] == &from_frame && from_rear == from_frame)) {
						this->frame = this->rear;
						this->frame_index = this->size - 1;
						this->frame_shifter(((signed long) from_rear) * -1);
					}
					else {
						this->frame_shifter((peek_index >= this->frame_index) ? (signed long) from_frame : ((signed long) from_frame) * -1);
					}
				}
				return this->frame->get_data();
			}
			

			/**
			 * @brief This method pops data off the linked list.
			 * 
			 * @param index `(signed long)` : The index of the data to be 
			 * removed. This value defaults to -1.
			 * 
			 * @returns The data at the index passed in.
			 * 
			 * @throws length_error exception if the index passed in is not 
			 * within the appropriate range for the `linear_linked_list`.
			*/
			data_ pop(signed long index = -1) {
				this->peek(index);
				// this->frame should now have all the necessary data.
				data_ the_answer = this->frame->get_data();
				if (this->size == 1) {
					this->reset();
					this->size = 1;
				}

				else if (this->frame_index == 0) {
					this->front = this->front->get_next();
					delete this->frame;
					this->frame = this->front;
				}

				else if (this->frame_index == this->size - 1) {
					this->rear = this->rear->get_previous();
					this->rear->set_next(nullptr);
					delete this->frame;
					this->frame = this->rear;
					this->frame_index = this->frame_index - 1;
				}
				
				else {
					linear_node<data_>* temp = this->frame;
					this->frame->get_previous()->set_next(this->frame->get_next());
					this->frame->get_previous()->get_next()->set_previous(this->frame->get_previous());
					this->frame = this->frame->get_next();
					temp->set_previous(nullptr);
					temp->set_next(nullptr);
					delete temp;
				}
				this->size = this->size - 1;
				return the_answer;
			}


			/**
			 * @brief Check if a piece of data is contained within the 
			 * `linear_linked_list`.
			 * 
			 * @param to_find `(Generic)` : The data to be found within the 
			 * linear_linked_list.
			 * 
			 * @returns -1 if the `to_find` piece of data is not within the linked_list. 
			 * Otherwise, the index of the data `to_find` is returned.
			*/
			signed long contains(data_ to_find) {
				signed long index;
				this->frame = this->front;
				this->frame_index = 0;
				while (this->frame != nullptr) {
					if (this->frame->get_data() == to_find) {
						return (signed long) this->frame_index;
					}
					this->frame = this->frame->get_next();
					this->frame_index = this->frame_index + 1;
				}
				this->frame = this->rear;
				this->frame_index = (this->frame_index == this->size) ? this->size - 1 : this->frame_index;
				return -1;
			}

	};


	template <typename data_> class binary_search_tree {

		private:
			unsigned long size;
			signed long height;
			bst_node<data_>* root;
		
			void push_new_data(bst_node<data_>* current, data_ new_data, signed long current_height) {
				// fprintf(stdout, "Inside the push_new_data\n");
				if (current == nullptr) {
					return;
				}
				else if (new_data >= current->get_data()) {
					// go to the right of the current node.
					if (current->get_right_child() == nullptr) {
						// set the child here.
						current->set_right_child(new bst_node<data_>(new_data));
						// current->get_right_child()->set_parent(current);
						this->size = this->size + 1;
						if (current_height + 1 > this->height) {
							this->height = this->height + 1;
						}
						return;
					}
					this->push_new_data(current->get_right_child(), new_data, current_height + 1);
				}
				else {
					// go to the left of the current node.
					if (current->get_left_child() == nullptr) {
						// set the child here
						current->set_left_child(new bst_node<data_>(new_data));
						// current->get_left_child()->set_parent(current);
						this->size = this->size + 1;
						if (current_height + 1 > this->height) {
							this->height = this->height + 1;
						}
						return;
					}
					this->push_new_data(current->get_left_child(), new_data, current_height + 1);
				}
			}

			void update_heights(bst_node<data_>* current, signed long this_height) {
				if (current == nullptr) {
					if (current == this->root) {
						this->height = -1;
					}
					return;
				}
				// current->set_height(this_height);
				if (this_height > this->height) {
					this->height = this_height;
				}
				update_heights(current->get_left_child(), this_height + 1);
				update_heights(current->get_right_child(), this_height + 1);
				return;
			}

			signed long height_of(bst_node<data_>* current, data_ data, signed long current_height = -1) {
				if (current == nullptr) {
					return -1;
				}
				if (current->get_data() == data) {
					return current_height;
				}
				return this->height_of((data > current->get_data()) ? current->get_right_child() : current->get_left_child(), data, current_height + 1);
			}

			void free_tree(bst_node<data_>* current) {
				if (current == nullptr) {
					return;
				}

				free_tree(current->get_left_child());
				free_tree(current->get_right_child());
				delete current;
			}

			void order_iterator(bst_node<data_>* current, linear_linked_list<data_>* to_fill, char* type = (char*) "pre-order") {
				if (current == nullptr) {
					return;
				}
				if (useful_functions::same_string(type, (char *) "pre-order")) {
					to_fill->push(current->get_data());
					this->order_iterator(current->get_left_child(), to_fill, type);
					this->order_iterator(current->get_right_child(), to_fill, type);
				}
				else if (useful_functions::same_string(type, (char *) "in-order")) {
					this->order_iterator(current->get_left_child(), to_fill, type);
					to_fill->push(current->get_data());
					this->order_iterator(current->get_right_child(), to_fill, type);
				}

				else if (useful_functions::same_string(type, (char *) "post-order")) {
					this->order_iterator(current->get_left_child(), to_fill, type);
					this->order_iterator(current->get_right_child(), to_fill, type);
					to_fill->push(current->get_data());
				}
				else {
					fprintf(stderr, "Not a recognized iterator \"%s\"\n", type);
					return;
				}
			}

			bst_node<data_>* remove_from_subtree(bst_node<data_>* current, data_ find_and_remove, signed long current_height = 0) {

				// base case
				if (!current) {
					return current;
				}

				// recursive calls for ancestors of
				// node to be deleted
				if (find_and_remove > current->get_data()) {
					current->set_right_child(this->remove_from_subtree(current->get_right_child(), find_and_remove, current_height + 1));
					return current;
				}

				else if (find_and_remove < current->get_data()) {
					current->set_left_child(this->remove_from_subtree(current->get_left_child(), find_and_remove, current_height + 1));
					return current;
				}

				// We reach here when root is the node
				// to be deleted.

				// there are no children
				if (!current->get_left_child() && !current->get_right_child()) {
					delete current;
					this->size = this->size - 1;
					return nullptr;
				}

				// there is a right child only.
				else if (!current->get_left_child() && current->get_right_child()) {
					bst_node<data_>* temp = current->get_right_child();
					delete current;
					this->size = this->size - 1;
					return temp;
				}

				// there is a left child only.
				else if (current->get_left_child() && !current->get_right_child()) {
					bst_node<data_>* temp = current->get_left_child();
					delete current;
					this->size = this->size - 1;
					return temp;
				}

				// there are children on both sides.
				else {
					bst_node<data_>* child_parent = current, *child = current->get_right_child();

					while (child->get_left_child()) {
						child_parent = child;
						child = child->get_left_child();
						current_height = current_height + 1;
					}

					if (child_parent != current) {
						child_parent->set_left_child(child->get_right_child());
					}
					else {
						child_parent->set_right_child(child->get_right_child());
					}

					current->set_data(child->get_data());

					delete child;
					this->size = this->size - 1;
					return current;
				}
			}

		public:

			binary_search_tree() {
				this->size = 0;
				this->height = -1;
				this->root = nullptr;
			}

			binary_search_tree(data_ new_data) {
				this->root = new bst_node<data_>(new_data);
				this->root->set_height(0);
				this->size = 1;
				this->height = 0;
			}

			binary_search_tree(binary_search_tree<data_>& other_tree) {
				if (this == &other_tree) {
					return;
				}
				this->root = nullptr;
				this->size = 0;
				this->height = -1;
				signed long index;
				linear_linked_list<data_> tree_list = other_tree.pre_order_iterator();
				for (index = 0; index < tree_list.length(); index = index + 1) {
					this->add(tree_list[index]);
				}
			}

			~binary_search_tree() {
				this->free_tree(this->root);
			}

			//  Operator Overloading

			// Comparison Operators
			bool operator==(binary_search_tree<data_>& other_tree) {
				if (this == &other_tree) {
					return true;
				}
				if (this->size != other_tree.get_size()) {
					return false;
				}
				if (this->get_height() != other_tree.get_height()) {
					return false;
				}
				linear_linked_list<data_> this_list = this->in_order_itereator();
				linear_linked_list<data_> other_list = this->in_order_itereator();
				signed long index;
				for (index = 0; index < this_list.length(); index = index + 1) {
					if (this_list[index] != other_list[index]) {
						return false;
					}
				}
				return true;
			}

			bool operator!=(binary_search_tree<data_>& other_tree) {
				if (this == &other_tree) {
					return false;
				}
				if (this->size != other_tree.size) {
					return true;
				}
				if (this->height != other_tree.height) {
					return true;
				}
				linear_linked_list<data_> this_list = this->in_order_itereator();
				linear_linked_list<data_> other_list = this->in_order_itereator();
				signed long index;
				for (index = 0; index < this_list.length(); index = index + 1) {
					if (this_list[index] != other_list[index]) {
						return true;
					}
				}
				return false;
			}

			bool operator>=(binary_search_tree<data_>& other_tree) {
				if (this == &other_tree) {
					return true;
				}
				linear_linked_list<data_> this_list = this->in_order_itereator();
				linear_linked_list<data_> other_list = other_tree.in_order_itereator();
				return this_list >= other_list;
			}

			bool operator<=(binary_search_tree<data_>& other_tree) {
				if (this == &other_tree) {
					return true;
				}
				linear_linked_list<data_> this_list = this->in_order_itereator();
				linear_linked_list<data_> other_list = other_tree.in_order_itereator();
				return this_list <= other_list;
			}


			// Assignment operator
			binary_search_tree<data_> operator=(binary_search_tree<data_> other_tree) {
				if (this == &other_tree) {
					return *this;
				}
				this->reset();
				linear_linked_list<data_> in_order = other_tree.pre_order_itereator();
				signed long index;
				for (index = 0; index < in_order.length(); index = index + 1) {
					this->add(in_order[index]);
				}
				return *this;
			}


			// Arithmetic operator
			binary_search_tree<data_> operator+(binary_search_tree<data_> other_tree) {
				binary_search_tree<data_> the_answer(*this);
				linear_linked_list<data_> pre_order = this->pre_order_iterator();
				signed long index;
				for (index = 0; index < pre_order.length(); index = index + 1) {
					the_answer.add(pre_order[index]);
				}
				return the_answer;
			}

			/**
			 * @brief Get the height of the binary search tree. Does not mutate the binary search tree.
			 * 
			 * @returns `(signed long)` : The height of the tree, -1 for an empty tree.
			*/
			unsigned long get_height() const {
				return this->height;
			}


			/**
			 * @brief Get the size of the binary search tree. Does not mutate the binary search tree.
			 * 
			 * @returns `(unsigned long)` : The size of the binary search tree. 0 is returned for an empty tree.
			*/
			signed long get_size() const {
				return this->size;
			}


			/**
			 * @brief Check if a binary search tree is empty or not. Returns a bool. 
			 * Does not modify the binary search tree.
			 * 
			 * @returns `(bool)` : `true` if the binary search tree is empty, `false` otherwise.
			*/
			bool empty() const {
				return this->size == 0;
			}


			/**
			 * @brief Add new data to the binary search tree. Modifies the binary search tree.
			 * 
			 * @param new_data `(Generic)` : The data to be added to the binary search tree.
			 * 
			 * @returns `(void)`.
			*/
			void add(data_ new_data) {
				if (this->size == 0) {
					this->root = new bst_node<data_>(new_data);
					this->size = 1;
					this->height = 0;
				}
				else {
					this->push_new_data(this->root, new_data, 0);
				}
			}


			/**
			 * @brief Check if the `binary_search_tree` contains the `to_find` data passed in.
			 * 
			 * @param `(Generic)` : The data to be found in the binary search tree.
			 * 
			 * @returns `(bool)` : `true` if `to_find` is found within the `binary_search_tree`, 
			 * `false` otherwise.
			*/
			bool contains(data_ to_find) {
				if (this->size == 0) {
					return false;
				}
				return this->height_of(this->root, to_find, 0) != -1;
			}


			/**
			 * @brief Get the height of `to_find` within the `binary_search_tree`. -1 is 
			 * returned if the `binary_search_tree` doesn't contain the data passed in, 
			 * otherwise the height of the data within the tree is returned.
			 * 
			 * @param to_find `(Generic)` : The data to find in the `binary_search_tree`.
			 * 
			 * @returns `(signed long)` : The height of the data in the `binary_search_tree`, 
			 * provided it exists, -1 otherwise.
			*/
			signed long data_height(data_ to_find) {
				if (this->size == 0) {
					return -1;
				}
				return this->height_of(this->root, to_find, 0);
			}


			/**
			 * @brief Reset the `binary_search_tree` and delete all it's data.
			 * 
			 * @returns `(void)`.
			*/
			void reset() {
				this->free_tree(this->root);
			}


			/**
			 * @brief Get a `linear_linked_list` containing all the data in `binary_search_tree` 
			 * organized into a pre-ordered manner.
			 * 
			 * @returns `(linear_linked_list<Generic>)`.
			*/
			linear_linked_list<data_> pre_order_iterator() {
				linear_linked_list<data_> the_answer;
				this->order_iterator(this->root, &the_answer);
				return the_answer;
			}


			/**
			 * @brief Get a `linear_linked_list` containing all the data in `binary_search_tree` 
			 * organized into a in-ordered manner.
			 * 
			 * @returns `(linear_linked_list<Generic>)`.
			*/
			linear_linked_list<data_> in_order_itereator() {
				linear_linked_list<data_> the_answer;
				this->order_iterator(this->root, &the_answer, (char *) "in-order");
				return the_answer;
			}


			/**
			 * @brief Get a `linear_linked_list` containing all the data in `binary_search_tree` 
			 * organized into a post-ordered manner.
			 * 
			 * @returns `(linear_linked_list<Generic>)`.
			*/
			linear_linked_list<data_> post_order_iterator() {
				linear_linked_list<data_> the_answer;
				this->order_iterator(this->root, &the_answer, (char *) "post-order");
				return the_answer;
			}


			/**
			 * @brief Removes `to_remove` from the `binary_search_tree`, provided it exists in the 
			 * `binary_search_tree`.
			 * 
			 * @note The data passed in is removed from the binary search tree, provided it exists, 
			 * and the internal height and size of the `binary_search_tree` is updated accordingly.
			 * 
			 * @param `(Generic)` : The data to be removed.
			 * 
			 * @returns `(void)`.
			*/
			void remove(data_ to_remove) {
				this->root = this->remove_from_subtree(this->root, to_remove);
				this->height = -1;
				this->update_heights(this->root, 0);
			}


	};


}