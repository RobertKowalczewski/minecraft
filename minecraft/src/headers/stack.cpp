/*#include<condition_variable>
#include<mutex>
#include<deque>

#include "queue.h"

template<typename T>
void Queue<T>::push(T message)
{
	while (true) {
		std::unique_lock<std::mutex> locker(mu);
		cond.wait(locker, [this]() { return buffer_.size() < size_; });
		buffer_.push_back(message);
		locker.unlock();
		cond.notify_all();
		return;
	}
}

template<typename T>
bool Queue<T>::pop(T& value)
{
	while (true) {
		std::unique_lock<std::mutex> locker(mu);
		if (buffer_.size() == 0) {
			return false;
		}
		value = buffer_.front();
		buffer_.pop_front();
		locker.unlock();
		cond.notify_all();
		return true;
	}
}*/
//#include"stack.h"
//#include<mutex>
//#include<stack>
//
//
//template<typename T>
//void Stack<T>::push(const T& item)
//{
//	std::lock_guard<std::mutex> lock(m_mutex);
//	m_stack.push(item);
//}
//
//template<typename T>
//void Stack<T>::pop()
//{
//	std::lock_guard<std::mutex> lock(m_mutex);
//	m_stack.pop();
//}
//
//template<typename T>
//T Stack<T>::top() const
//{
//	std::lock_guard<std::mutex> lock(m_mutex);
//	return m_stack.top();
//}