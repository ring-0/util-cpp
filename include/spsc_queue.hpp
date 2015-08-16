#pragma once

#include <unistd.h>

namespace util {
namespace container {

// This SingleProducerSingleConsumer queue implementation was *inspired* by:
// http://software.intel.com/en-us/articles/single-producer-single-consumer-queue/

template<typename T>
T load_acquire(T const* addr)
{
    T v = *const_cast<T const volatile*>(addr);
    __asm__ __volatile__ ("": : :"memory");
    return(v);
}


template<typename T>
void store_release(T* addr, T v)
{
    __asm__ __volatile__ ("": : :"memory");
    *const_cast<T volatile*>(addr) = v;
}


template<typename T>
class spsc_queue
{
    spsc_queue(const spsc_queue<T>&) = delete;
    void operator=(const spsc_queue<T>&) = delete;

private:
    struct node
    {
        node*	next;
        T		value;
    };

    // consumer
    node* tail;
    char cachepadd[64];
    // producer
    node* head;
    node* first;
    node* tailCopy;
    bool  bound;

    node* __allocateNode()
    {
        node* n;
        if(first != tailCopy) {
            n		= first;
            first	= first->next;
            return(n);
        }

        tailCopy = load_acquire(&tail);
        if(first != tailCopy) {
            n		= first;
            first	= first->next;
            return(n);
        }

        if(bound)
            return(NULL);
        else
            n = new node;

        return(n);
    }

public:
    spsc_queue() :
        tail(NULL), head(NULL), first(NULL), tailCopy(NULL), bound(false)
    {
        bound	= false;
        node* n	= new node;
        n->next	= NULL;
        tail	= head = first = tailCopy = n;
    }

    bool setBound(size_t size)
    {
        // check if already set
        if(bound)
            return false;

        bound = true;

        for(size_t i=0; i<size; i++) {
            node* n = new node;
            n->next = NULL;
            store_release(&head->next, n);
            head = n;
            store_release(&tail, tail->next);
        }

        return true;
    }

    ~spsc_queue()
    {
        node* n = first;
        node* next;
        do {
            next = n->next;
            delete(n);
            n = next;
        } while(n);
    }

    bool push(T v)
    {
        node* n		= __allocateNode();
        if(!n && bound)
            return(false);

        n->next		= NULL;
        n->value	= v;
        store_release(&head->next, n);
        head = n;
        return(true);
    }

    bool pop(T& v)
    {
        if(load_acquire(&tail->next)) {
            v = tail->next->value;
            store_release(&tail, tail->next);
            return(true);
        } else {
            return(false);
        }
    }

    bool peek(T& v)
    {
        if(load_acquire(&tail->next)) {
            v = tail->next->value;
            return(true);
        } else {
            return(false);
        }
    }

    bool	empty() const		{ return(load_acquire(&tail->next) == NULL);					}
    bool	full()	const		{ return(bound && load_acquire(&first) == load_acquire(&tail));	}

}; // end class spsc_queue


} // end namespace container
} // end namespace util
