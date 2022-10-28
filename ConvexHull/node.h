#pragma once

#include<memory>

namespace ch_project
{
	template<class T>
	class node
	{
	public:
		T get_data() const { return m_data; };
		void set_data(const T& data) { m_data = data; };

		node<T>* get_next() { return m_next; };
		void set_next(node<T>* next) { m_next = next; };

		//node<T>* get_previous() { return m_prev; };
		//void set_previous(node<T>* prev) { m_prev = prev; };

		//node<T>* get_head() {
		//	node<T>* head = this;

		//	while (head->get_previous() != nullptr)
		//		head = head->get_previous();

		//	return head;
		//}

		node(): m_next(nullptr)/*, m_prev(nullptr)*/ {};
		node(const T& data) : m_data(data), m_next(nullptr)/*, m_prev(nullptr)*/ {};
	private:
		T m_data;
		node<T>* m_next;
		//node<T>* m_prev;
	};
}
