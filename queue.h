#ifndef QUEUE_H
#define QUEUE_H

#include <future>

template <class T>
class Queue
{
private:
    std::mutex m;

    struct node
    {
        std::shared_ptr<T> data;
        node *next;

        node() :
            next(nullptr)
        {}
    };

    node *head;
    node *tail;

public:
    Queue() :
        head(new node), tail(head)
    {}

    Queue(const Queue &) = delete;
    Queue& operator= (const Queue &) = delete;

    ~Queue()
    {
        node *trash;

        while(head->next != nullptr)
        {
            trash = head;
            head = head->next;
            delete trash;
        }

        delete head;
    }

    bool is_empty()
    {
        std::lock_guard<std::mutex> lk(m);
        return head == tail;
    }

    void push(T data)
    {
        std::shared_ptr<T> new_data(std::make_shared<T> (data));
        node *new_node;
        new_node = new node;

        std::lock_guard<std::mutex> lk(m);

        tail->data = new_data;
        tail->next = new_node;
        tail = tail->next;
    }

    std::shared_ptr<T> pop()
    {
        node *trash;
        {
            std::lock_guard<std::mutex> lk(m);

            if (head == tail)
            {
                return std::shared_ptr<T>();
            }

            trash = head;
            head = head->next;
        }
        std::shared_ptr<T> return_ptr(trash->data);
        delete trash;

        return return_ptr;
    }
};

#endif // QUEUE_H
