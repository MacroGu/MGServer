/*
	author: Macro Gu
	email: macrogu@qq.com
	QQ: 877188891
*/

#pragma once

#include <memory>
#include <thread>
#include <mutex>



template <typename T>
class ISingleton
{
	public:
		static T& GetInstance()
		{
			static std::mutex s_mutex;
			if (s_instance.get() == nullptr)
			{
				s_mutex.lock();
				if (s_instance.get() == nullptr)
				{
					s_instance.reset(new T());
				}
				s_mutex.unlock();
			}
			
			return *s_instance;
		}

	protected:
		ISingleton() {}
		~ISingleton() {}

		// Use auto_ptr to make sure that the allocated memory for instance
		// will be released when program exits (after main() ends)
		static std::auto_ptr<T> s_instance;
	
	private:
		ISingleton(const ISingleton&);
		ISingleton& operator = (const ISingleton&);
};

template <typename T>
std::auto_ptr<T> ISingleton<T>::s_instance;