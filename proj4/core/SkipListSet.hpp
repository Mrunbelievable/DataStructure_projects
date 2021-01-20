// SkipListSet.hpp
//
// ICS 46 Spring 2020
// Project #4: Set the Controls for the Heart of the Sun
//
// A SkipListSet is an implementation of a Set that is a skip list, implemented
// as we discussed in lecture.  A skip list is a sequence of levels
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the keys and their
// values.  Instead, you'll need to implement your own dynamically-allocated
// nodes, with pointers connecting them.  You can, however, use other parts of
// the C++ Standard Library -- including <random>, notably.
//
// Each node should contain only two pointers: one to the node that follows it
// on the same level and another to the equivalent node on the level below it.
// Additional pointers use more memory but don't enable any techniques not
// enabled by the other two.
//
// A couple of utilities are included here: SkipListKind and SkipListKey.
// You can feel free to use these as-is and probably will not need to
// modify them, though you can make changes to them, if you'd like.

#ifndef SKIPLISTSET_HPP
#define SKIPLISTSET_HPP

#include <memory>
#include <random>
#include "Set.hpp"


#include <iostream>

// SkipListKind indicates a kind of key: a normal one, the special key
// -INF, or the special key +INF.  It's necessary for us to implement
// the notion of -INF and +INF separately, since we're building a class
// template and not all types of keys would have a reasonable notion of
// -INF and +INF.

enum class SkipListKind
{
    Normal,
    NegInf,
    PosInf
};




// A SkipListKey represents a single key in a skip list.  It is possible
// to compare these keys using < or == operators (which are overloaded here)
// and those comparisons respect the notion of whether each key is normal,
// -INF, or +INF.

template <typename ElementType>
class SkipListKey
{
public:
    SkipListKey(SkipListKind kind, const ElementType& element);
    SkipListKey(SkipListKind kind) : kind{kind} {
    }

    ElementType getElement() { return element; }

    bool operator==(const SkipListKey& other) const;
    bool operator<(const SkipListKey& other) const;

private:
    SkipListKind kind;
    ElementType element;
};


template <typename ElementType>
SkipListKey<ElementType>::SkipListKey(SkipListKind kind, const ElementType& element)
    : kind{kind}, element{element}
{
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator==(const SkipListKey& other) const
{
    return kind == other.kind
        && (kind != SkipListKind::Normal || element == other.element);
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator<(const SkipListKey& other) const
{
    switch (kind)
    {
    case SkipListKind::NegInf:
        return other.kind != SkipListKind::NegInf;

    case SkipListKind::PosInf:
        return false;

    default: // SkipListKind::Normal
        return other.kind == SkipListKind::PosInf
            || (other.kind == SkipListKind::Normal && element < other.element);
    }
}



// The SkipListLevelTester class represents the ability to decide whether
// a key placed on one level of the skip list should also occupy the next
// level.  This is the "coin flip," so to speak.  Note that this is an
// abstract base class with one implementation, RandomSkipListLevelTester,
// just below it.  RandomSkipListLevelTester is what it sounds like: It
// makes the decision at random (with a 50/50 chance of deciding whether
// a key should occupy the next level).  However, by setting things up
// this way, we have a way to control things more carefully in our
// testing (as you can, as well).
//
// DO NOT MAKE CHANGES TO THE SIGNATURES OF THE MEMBER FUNCTIONS OF
// THE "level tester" CLASSES.  You can add new member functions or even
// whole new level tester classes, but the ones declared below are part
// of how we test your implementation, so they need to maintain the
// parts of the interface that are declared already.

template <typename ElementType>
class SkipListLevelTester
{
public:
    virtual ~SkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) = 0;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() = 0;
};


template <typename ElementType>
class RandomSkipListLevelTester : public SkipListLevelTester<ElementType>
{
public:
    RandomSkipListLevelTester();

    bool shouldOccupyNextLevel(const ElementType& element) override;
    std::unique_ptr<SkipListLevelTester<ElementType>> clone() override;

private:
    std::default_random_engine engine;
    std::bernoulli_distribution distribution;
};


template <typename ElementType>
RandomSkipListLevelTester<ElementType>::RandomSkipListLevelTester()
    : engine{std::random_device{}()}, distribution{0.5}
{
}


template <typename ElementType>
bool RandomSkipListLevelTester<ElementType>::shouldOccupyNextLevel(const ElementType& element)
{
    return distribution(engine);
}


template <typename ElementType>
std::unique_ptr<SkipListLevelTester<ElementType>> RandomSkipListLevelTester<ElementType>::clone()
{
    return std::unique_ptr<SkipListLevelTester<ElementType>>{
        new RandomSkipListLevelTester<ElementType>};
}




template <typename ElementType>
class SkipListSet : public Set<ElementType>
{
public:
    // Initializes an SkipListSet to be empty, with or without a
    // "level tester" object that will decide, whenever a "coin flip"
    // is needed, whether a key should occupy the next level above.
    SkipListSet();
    explicit SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester);

    // Cleans up the SkipListSet so that it leaks no memory.
    ~SkipListSet() noexcept override;

    // Initializes a new SkipListSet to be a copy of an existing one.
    SkipListSet(const SkipListSet& s);

    // Initializes a new SkipListSet whose contents are moved from an
    // expiring one.
    SkipListSet(SkipListSet&& s) noexcept;

    // Assigns an existing SkipListSet into another.
    SkipListSet& operator=(const SkipListSet& s);

    // Assigns an expiring SkipListSet into another.
    SkipListSet& operator=(SkipListSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a SkipListSet, false otherwise.
    bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function runs in an expected time
    // of O(log n) (i.e., over the long run, we expect the average to be
    // O(log n)) with very high probability.
    void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in an expected time of O(log n)
    // (i.e., over the long run, we expect the average to be O(log n))
    // with very high probability.
    bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    unsigned int size() const noexcept override;


    // levelCount() returns the number of levels in the skip list.
    unsigned int levelCount() const noexcept;


    // elementsOnLevel() returns the number of elements that are stored
    // on the given level of the skip list.  Level 0 is the bottom level;
    // level 1 is the one above level 0; and so on.  If the given level
    // doesn't exist, this function returns 0.  (Note that the -INF
    // and +INF shouldn't be counted.)
    unsigned int elementsOnLevel(unsigned int level) const noexcept;


    // isElementOnLevel() returns true if the given element is on the
    // given level, false otherwise.  Level 0 is the bottom level; level 1
    // is the one above level 0; and so on.  If the given level doesn't
    // exist, this function returns false.
    bool isElementOnLevel(const ElementType& element, unsigned int level) const;

    void printSkipList() {
        SkipListNode *h = head, *t = tail;
        while (h != nullptr) {
            printLevelList(h, t);
            h = h->down;
            t = t->down;
        }
    }

private:
    std::unique_ptr<SkipListLevelTester<ElementType>> levelTester;
    struct SkipListNode {
        SkipListKey<ElementType> keyValue;
        SkipListNode *prev, *next, *up, *down;

        SkipListNode(const SkipListKey<ElementType> &kv) 
            : keyValue(kv) {
            prev = next = nullptr;
            up = down = nullptr;
        }
    };
    int totalNodes;
    int totalLevel;

    SkipListNode *head, *tail;

    void clone(const SkipListSet &s);
    void empty();
    void insertNode(SkipListNode *p, SkipListNode *q);
    void addEmptyLevel();

    SkipListNode* findNode(const SkipListKey<ElementType> &kv) {
        SkipListNode *p = head;

        while (true) {
            while (p->next != nullptr && p->next->keyValue < kv) {
                p = p->next;
            }

            if (p->down != nullptr) p = p->down;
            else break;
        }

        return p;
    }

    void initHeadTail() {
        SkipListKey<ElementType> hk(SkipListKind::NegInf);
        SkipListKey<ElementType> tk(SkipListKind::PosInf);

        head = new SkipListNode(hk);
        tail = new SkipListNode(tk);
    }

    void printLevelList(SkipListNode *h, SkipListNode *t) {
        SkipListNode *p = h->next;
        while (p != t) {
            std::cout << p->keyValue.getElement() << std::endl;
            p = p->next;
        }
        std::cout << "---\n";
    }


};


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet()
    : SkipListSet{std::make_unique<RandomSkipListLevelTester<ElementType>>()} // levelTester
{
    totalNodes = 0;
    totalLevel = 0;

    initHeadTail();

    head->next = tail;
    tail->prev = head;
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester)
    : levelTester{std::move(levelTester)}
{
    totalNodes = 0;
    totalLevel = 0;

    initHeadTail();

    head->next = tail;
    tail->prev = head;
}

template <typename ElementType>
void SkipListSet<ElementType>::empty()
{
    SkipListNode *tmp;
    SkipListNode *p = head, *q, *t;

    if (head == nullptr) return;
    
    p = head;
    t = tail;

    while (p != nullptr) { // free data nodes
        q = p->next;
        while (q != t) {
            tmp = q->next;
            delete q;
            q = tmp;
        }
        p = p->down; // next level
        t = t->down;
    }

    p = head;
    t = tail;

    while (p != nullptr) { // free head and tail nodes
        tmp = p->down;
        delete p;
        p = tmp;

        tmp = t->down;
        delete t;
        t = tmp;
    }
}

template <typename ElementType>
SkipListSet<ElementType>::~SkipListSet() noexcept
{
    empty();
}


template <typename ElementType>
void SkipListSet<ElementType>::clone(const SkipListSet& s)
{
    totalNodes = s.totalNodes;
    totalLevel = s.totalLevel;

    //levelTester = std::move(s.levelTester);

    head = tail = nullptr;

    // copy head and tail
    SkipListNode *p, *q, *t, *hq = nullptr, *tq = nullptr, 
                 *ph, *pt, *bp, *bt;
    p = s.head;

    if (p == nullptr) return;

    initHeadTail();

    head->next = tail;
    tail->prev = head;

    ph = head;
    pt = tail;

    p = p->down;
    t = tail->down;
    bp = p;
    bt = t;

    SkipListKey<ElementType> hk(SkipListKind::NegInf);
    SkipListKey<ElementType> tk(SkipListKind::PosInf);

    while (p != nullptr) { // copy head and tail list
        hq = new SkipListNode(hk);
        tq = new SkipListNode(tk);

        hq->next = tq;
        tq->prev = hq;

        ph->down = hq;
        hq->up = ph;
        ph = hq;

        pt->down = tq;
        tq->up = pt;
        pt = tq;

        bp = p;
        bt = t;

        p = p->down;
        t = t->down;
    }

    ph = hq;
    pt = tq;

    if (hq == nullptr) ph = head;
    if (tq == nullptr) pt = tail;

    SkipListNode *tmp;
    // copy nodes from bottom to up
    p = bp;
    t = bt;
    if (p != nullptr) {
        q = p->next;
        hq = ph;

        while (q != t) {
            tmp = new SkipListNode(q->keyValue);

            hq->next = tmp; // add one node in line
            tmp->prev = hq;
            hq = tmp;
            tmp->next = tail;
            tail->prev = tmp;

            // add column list
            SkipListNode *sln = q->up;
            // newhead
            SkipListNode *newtail = tail, *newhead;

            while (sln != nullptr) {
                newhead = newtail->prev;

                SkipListNode *t1 = new SkipListNode(sln->keyValue);

                // add level nodes
                newhead->next = t1;
                t1->prev = newhead;
                t1->next = newtail;
                newtail->prev = t1;

                tmp->up = t1;
                t1->down = tmp;
                tmp = t1;

                newtail = newtail->up;
                sln = sln->up;
            }

            q = q->next;
        }
    }
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(const SkipListSet& s)
{
    clone(s);
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(SkipListSet&& s) noexcept
{
    totalNodes = s.totalNodes;
    totalLevel = s.totalLevel;

    levelTester = std::move(s.levelTester);

    head = s.head;
    tail = s.tail;
    s.head = nullptr;
    s.tail = nullptr;
}

template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(const SkipListSet& s)
{
    if (this != &s) {
        if (head != nullptr) empty();
        clone(s);
    }

    return *this;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(SkipListSet&& s) noexcept
{
    if (this != &s) {
        totalNodes = s.totalNodes;
        totalLevel = s.totalLevel;

        //levelTester = std::move(s.levelTester);

        head = s.head;
        tail = s.tail;
        s.head = nullptr;
        s.tail = nullptr;
    }

    return *this;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

template <typename ElementType>
void SkipListSet<ElementType>::insertNode(SkipListNode *p, SkipListNode *q) 
{
    q->next = p->next;
    q->prev = p;
    p->next->prev = q;
    p->next = q;
}

template <typename ElementType>
void SkipListSet<ElementType>::addEmptyLevel()
{
    // add one level
    SkipListKey<ElementType> hk(SkipListKind::NegInf);
    SkipListKey<ElementType> tk(SkipListKind::PosInf);

    SkipListNode *p1 = new SkipListNode(hk);
    SkipListNode *p2 = new SkipListNode(tk);

    p1->next = p2;
    p1->down = head;
    p2->prev = p1;
    p2->down = tail;

    head->up = p1;
    tail->up = p2;

    head = p1;
    tail = p2;

    totalLevel += 1;
}

template <typename ElementType>
void SkipListSet<ElementType>::add(const ElementType& element)
{
    if (contains(element)) return;

    SkipListKey<ElementType> *kv = 
        new SkipListKey<ElementType>(SkipListKind::Normal, element);

    SkipListNode* p = findNode(*kv);
    SkipListNode *q = new SkipListNode(*kv);
    insertNode(p, q); // add new node

    int currentLevel = 0;
    while (levelTester->shouldOccupyNextLevel(element)) {
        if (currentLevel >= totalLevel) { // add new level
            addEmptyLevel();
        }
        
        while (p->up == nullptr) { // find prev node
            p = p->prev;
        }

        p = p->up;

        // add level node
        SkipListNode *z = new SkipListNode(*kv);
        insertNode(p, z);

        // build relationship
        z->down = q;
        q->up = z;

        // move up
        q = z;
        currentLevel += 1;
    }

    totalNodes += 1;

    delete kv;
}


template <typename ElementType>
bool SkipListSet<ElementType>::contains(const ElementType& element) const
{
    if (head == nullptr) return false;

    if (isElementOnLevel(element, 0)) return true;

    return false;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::size() const noexcept
{
    return totalNodes;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::levelCount() const noexcept
{
    if (head == nullptr) return 0;

    return totalLevel+1;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::elementsOnLevel(unsigned int level) const noexcept
{
    SkipListNode *p = head, *q = tail;
    if (p == nullptr) return 0;
    if (level > totalLevel) return 0;

    while (p->down != nullptr && totalLevel-level > 0) {
        p = p->down;
        q = q->down;
        level += 1;
    }

    int count = 0;
    while (p->next != q) {
        count += 1;
        p = p->next;
    }

    return count;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isElementOnLevel(const ElementType& element, unsigned int level) const
{
    SkipListNode *p = head, *q = tail;
    if (p == nullptr) return false;
    if (level > totalLevel) return false;

    while (totalLevel-level > 0 && p->down != nullptr) {
        p = p->down;
        q = q->down;
        level += 1;
    }

    SkipListKey<ElementType> sk{SkipListKind::Normal, element};
    p = p->next;
    while (p != q) {
        if (p->keyValue == sk) return true;
        p = p->next;
    }

    return false;
}



#endif // SKIPLISTSET_HPP


