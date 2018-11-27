/**
 Program:	queue.h
 Description:	Declarations for the Queue class.
 Author:	Ricardo Salazar
 Date:		November, 2015
 */

#ifndef HOMEMADE_QUEUE_H
#define HOMEMADE_QUEUE_H

#include <queue>
/**
 We are going to write a 'wrapper' for our queue class.
 To distinguish it from std::queue let us enclose it into
 our very own namespace.
 */
namespace Pic10B {
    
    template <typename T>
    class queue {
    public:
        // Only one of 'the big 4' is needed here.
        inline queue() : theQueue() {}
        
        inline bool is_empty() const { return theQueue.empty(); }
        inline void enter( T value ) { theQueue.push(value); return; }
        inline T leave() { T r=theQueue.front(); theQueue.pop(); return r; }
        inline T peek() const { return theQueue.front(); }
        
    private:
        std::queue<T> theQueue;
    };
    // No need to implement extra functions... but if we had to,
    // they would go here.
    
} // <-- Unlike a class, here we DO NOT NEED the semi-colon at the end

#endif