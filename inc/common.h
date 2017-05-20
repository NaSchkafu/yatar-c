//
// Created by Fluxy on 21.08.2016.
//

#ifndef YATAR_C_COMMON_H
#define YATAR_C_COMMON_H

#include <functional>

template<typename T>
struct handle_deleter
{
    handle_deleter() {}
    explicit handle_deleter(std::function<void (T)> deleter)
    :m_deleter(deleter)
    {}

    void operator()(T obj)
    {
        m_deleter(obj);
    }

    using pointer = T;

    std::function<void (T)> m_deleter;
};

#endif //YATAR_C_COMMON_H
