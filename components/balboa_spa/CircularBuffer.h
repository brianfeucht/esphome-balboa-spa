#pragma once

#include "stdint.h"
#include <cstddef>
#include <deque>
#include <algorithm>
#include "esphome/core/log.h"

namespace esphome
{
    namespace balboa_spa
    {

        static const char *CIRCULAR_BUFFER_TAG = "BalboaSpa.CircularBuffer";

        template <typename T, size_t s>
        class CircularBuffer
        {
        private:
            std::deque<T> backingQue;
            const size_t max_size = s;
            /** @private */
            CircularBuffer(CircularBuffer &&) = delete;

            // disable the assignment operator
            /** @private */
            CircularBuffer &operator=(const CircularBuffer &) = delete;
            /** @private */
            CircularBuffer &operator=(CircularBuffer &&) = delete;

        public:
            CircularBuffer(const CircularBuffer &cb)
            {
                this->backingQue = cb.backingQue;
            };
            CircularBuffer(/* args */) {};
            ~CircularBuffer()
            {
                backingQue.clear();
            };

            size_t size()
            {
                return this->backingQue.size();
            }

            size_t copyToArray(T *arr)
            {
                for (size_t offset = 0; offset < this->backingQue.size(); offset++)
                {
                    arr[offset] = this->backingQue[offset];
                }
                return this->backingQue.size();
            }

            void sort()
            {
                std::sort(backingQue.begin(), backingQue.end());
            }

            bool push(T val)
            {
                if (this->max_size <= this->backingQue.size())
                {
                    this->backingQue.pop_front();
                    this->backingQue.push_back(val);
                    return false;
                }
                this->backingQue.push_back(val);
                return true;
            };

            const T &last()
            {
                if (this->backingQue.empty())
                {
                    ESP_LOGV(CIRCULAR_BUFFER_TAG, "last() called on empty buffer");
                    static const T empty_value{};
                    return empty_value;
                }
                return backingQue.back();
            };

            const T &first()
            {
                if (this->backingQue.empty())
                {
                    ESP_LOGV(CIRCULAR_BUFFER_TAG, "first() called on empty buffer");
                    static const T empty_value{};
                    return empty_value;
                }
                return backingQue.front();
            };

            T at(size_t index)
            {
                return backingQue.at(index);
            };

            T operator[](size_t index)
            {
                if (index >= this->size())
                {
                    ESP_LOGE(CIRCULAR_BUFFER_TAG, "INDEX %u out of bounds. size=%u", (unsigned)index, (unsigned)this->size());
                    return T();
                }
                return backingQue.at(index);
            };

            void clear()
            {
                backingQue.clear();
            };

            T pop()
            {
                if (this->backingQue.empty())
                {
                    ESP_LOGV(CIRCULAR_BUFFER_TAG, "pop() called on empty buffer");
                    return T();
                }
                T val = backingQue.back();
                backingQue.pop_back();
                return val;
            };

            bool unshift(T val)
            {
                if (this->max_size > this->backingQue.size())
                {
                    this->backingQue.push_front(val);
                    return true;
                }
                return false;
            }
        };

    } // ns balboa_spa
} // ns esphome