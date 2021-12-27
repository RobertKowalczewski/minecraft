#pragma once
#include<mutex>
#include<stack>
/*#include<mutex>
#include<deque>
#include<condition_variable>


template<typename T>
class Queue {
public:
	Queue() = default;

	void push(T message);
	bool pop(T& value);

private:
	std::mutex mu;
	std::condition_variable cond;

	std::deque<T> buffer_;
	const unsigned int size_ = 200;
};*/

template <typename T> class Stack {
public:
	void push(const T& item);
    void pop();
    T top() const;
	bool empty();

	void push_unsafe(const T& item);
	void pop_unsafe();
	T top_unsafe() const;
	bool empty_unsafe();

private:
    mutable std::mutex m_mutex;
    std::stack<T> m_stack;
};
template<typename T>
inline void Stack<T>::push(const T& item)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_stack.push(item);
}

template<typename T>
inline void Stack<T>::pop()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_stack.pop();
}

template<typename T>
inline T Stack<T>::top() const
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_stack.top();
}

template<typename T>
inline bool Stack<T>::empty()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_stack.empty();
}

template<typename T>
inline void Stack<T>::push_unsafe(const T& item)
{
	m_stack.push(item);
}

template<typename T>
inline void Stack<T>::pop_unsafe()
{
	m_stack.pop();
}

template<typename T>
inline T Stack<T>::top_unsafe() const
{
	return m_stack.top();
}

template<typename T>
inline bool Stack<T>::empty_unsafe()
{
	return m_stack.empty();
}
