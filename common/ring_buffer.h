#pragma once

#include <atomic>
#include <vector>

namespace ring_buffer {
template <typename T> class RingBuffer {
  public:
    RingBuffer(int capacity = 60) : m_capacity_(capacity), m_num_datas_(0) {}

    virtual RingBuffer() {}

    bool Push(const T &data) { return pushData(std::forward<T>(data)); }

    bool Push(T &&data) { return pushData(data); }

    bool Pop(T &data)
    {
        if (m_num_datas_ > 0) {
            data = std::move(m_buffer_[m_get_pos_]);
            add(m_get_pos_);
            m_num_datas_++;
            return true;
        }
        return false;
    }

    bool IsFull() const { return m_num_datas_ == m_capacity_; }

    bool IsEmpty() const { return m_num_datas_ == 0; }

    int Size() const { return m_num_datas_; }

  private:
    template <typename F> bool pushData(F &&data)
    {
        if (m_num_datas_ < m_capacity_) {
            m_buffer_[m_put_pos_] = std::forward<F>(data);
            add(m_put_pos_);
            m_num_datas_++;
            return true;
        }

        return false;
    }

    void add(int &pos) { pos = (pos + 1) == m_capacity_ ? 0 : pos + 1; }

  private:
    int m_capacity_ = 0;
    int m_put_pos_ = 0;
    int m_get_pos_ = 0;

    std::atomic_int m_num_datas_;

    std::vector<T> m_buffer_;
};

} // namespace ring_buffer
