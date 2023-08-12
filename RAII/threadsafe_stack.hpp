#include <stack>
#include <mutex>
#include <exception>

struct empty_stack:std::exception
{
    const char* what() const throw();
};

template<class T>
class threadsafe_stack
{
private:

    std::stack<T> m_stack_;
    std::mutex m_mutex_

public:
    threadsafe_stack() {};
    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.m_mutex_);
        m_stack_ = other.m_stack_;
    }

    ~threadsafe_stack() {
        std::lock_guard<std::mutex> lock(m_mutex_);
        std::stack<T> s;
        m_stack_.swap(s);
    };

    threadsafe_stack& operator=(const threadsafe_stack&) = delete;

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lock(m_mutex_);
        if(m_stack_.empty())
            throw empty_stack();

        std::shared_ptr<T> const res(std::make_shared<T>(m_stack_.top()));
        m_stack_.top();
        return res;
    }

    void pop(T &value)
    {
        std::lock_guard<std::mutex> lock(m_mutex_);
        if (m_stack_.empty())
            throw empty_stack();

        value = m_stack_.top();
        m_stack_.pop();
    }

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lock(m_mutex_);
        m_stack_.push(new_value);
    }

    void empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex_);
        return m_stack_.empty();
    }

};

