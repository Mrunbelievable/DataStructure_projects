// HashSet.hpp
//
// ICS 46 Spring 2020
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"



template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    ~HashSet() noexcept override;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8, in which case
    // the new capacity should be determined by this formula:
    //
    //     capacity * 2 + 1
    //
    // In the case where the array is resized, this function runs in linear
    // time (with respect to the number of elements, assuming a good hash
    // function); otherwise, it runs in constant time (again, assuming a good
    // hash function).  The amortized running time is also constant.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    HashFunction hashFunction;

    struct HashNode{
        ElementType key;
        HashNode* next = NULL;
    };
    void deleteNode(HashNode* temp){
        if(temp==NULL)
            return;
        deleteNode(temp->next);
        delete temp;
    }
    HashNode** hashtable;
    unsigned int hash_capacity;
    unsigned int hash_size;
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
    hash_capacity = DEFAULT_CAPACITY;
    hash_size = 0;

    hashtable = new HashNode*[hash_capacity];
    for (int i = 0; i < hash_capacity; i++) hashtable[i] = NULL;
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    if (hashtable != NULL) {
        for (unsigned int i = 0; i < hash_capacity; i++) {
            if (hashtable[i] != NULL) deleteNode(hashtable[i]);
        }
        delete hashtable;
    }

    hash_capacity = DEFAULT_CAPACITY;
    hash_size = 0;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    // copy constructor
    hashFunction = s.hashFunction;
    hash_capacity = s.hash_capacity;
    hash_size = s.hash_size;

    hashtable = new HashNode*[hash_capacity]; // deep copy
    for (int i = 0; i < hash_capacity; i++)
        hashtable[i] = NULL;

    for (int i = 0; i < hash_capacity; i++) {
        if (s.hashtable[i] == NULL) continue;

        hashtable[i] = new HashNode;
        hashtable[i]->key = s.hashtable[i]->key;

        HashNode *ptr = hashtable[i];
        HashNode *nxt = s.hashtable[i]->next;

        while (nxt != NULL) {
            ptr->next = new HashNode;
            ptr->next->key = nxt->key;

            nxt = nxt->next;
            ptr = ptr->next;
        }

        ptr->next = nxt;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    hashFunction = s.hashFunction;
    hash_capacity = s.hash_capacity;
    hashtable = s.hashtable; // non-deep copy
    hash_size = s.hash_size;
    s.hashtable = NULL;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    if (this != &s) { // copy

        if (hashtable != NULL) {
            // delete first
            for (unsigned int i = 0; i < hash_capacity; i++) {
                deleteNode(hashtable[i]);
            }
            delete []hashtable;
        }

        // do copy
        hashFunction = s.hashFunction;
        hash_capacity = s.hash_capacity;
        hash_size = s.hash_size;

        hashtable = new HashNode*[hash_capacity]; // deep copy
        for (int i = 0; i < hash_capacity; i++)
            hashtable[i] = NULL;

        for (int i = 0; i < hash_capacity; i++) {
            if (s.hashtable[i] == NULL) continue;

            hashtable[i] = new HashNode;
            hashtable[i]->key = s.hashtable[i]->key;

            HashNode *ptr = hashtable[i];
            HashNode *nxt = s.hashtable[i]->next;

            while (nxt != NULL) {
                ptr->next = new HashNode;
                ptr->next->key = nxt->key;

                nxt = nxt->next;
                ptr = ptr->next;
            }

            ptr->next = nxt;
        }
    }

    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    if (this != &s) {
        hashFunction = s.hashFunction;
        hash_capacity = s.hash_capacity;
        hashtable = s.hashtable; // non-deep copy
        hash_size = s.hash_size;
        s.hashtable = NULL;
    }

    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    unsigned int idx = hashFunction(element)%hash_capacity;

    HashNode *ptr = hashtable[idx];
    while (ptr != NULL) { // traverse all elements
        if (ptr->key == element) return true;
        ptr = ptr->next;
    }

    return false;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return hash_size;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    int i = 0;

    HashNode *ptr = hashtable[index];
    while (ptr != NULL) { // traverse all elements
        i += 1;
        ptr = ptr->next;
    }

    return i;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    HashNode *ptr = hashtable[index];

    while (ptr != NULL) { // traverse all elements
        if (element == ptr->key) return true;
        ptr = ptr->next;
    }

    return false;
}



#endif // HASHSET_HPP

