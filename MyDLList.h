#pragma once

#include <iostream>
#include <stdexcept>
#include <iterator>

  //              head_                                                     tail_
 //                 |                                                        |               
//nullptr <-- [prev|А|next] <--> [prev|Б|next] <--> [prev|В|next] <--> [prev|Г|next] --> nullptr

template <class T>
class MyDLList {
private:
    size_t size_ = 0; 
    
    struct Node 
    {
        T data_ = 0;
        Node* next_ = nullptr;
        Node* prev_ = nullptr; 
        Node(const T& value) : data_(value), next_(nullptr), prev_(nullptr) {}
    };
    Node* head_ = nullptr; 
    Node* tail_ = nullptr; 

    void move_(MyDLList<T>& other) {
        size_ = other.size_; 
        head_ = other.head_; 
        tail_ = other.tail_; 
        other.size_ = 0;
        other.head_ = nullptr;
        other.tail_ = nullptr;
    }

    void deleting_() {
        Node* current_ = nullptr;
        Node* ptratnext_ = nullptr;                      // нужно удалять все элементы начиная с первого и запоминая указатель на следующий элемент. 
        if (size_ >= 1) {                               // размер 1 и более - тогда действуем
            for (size_t i = 0; i < size_; i++) {       // проходим все узлы
                if (i == 0) {                         // Первый узел
                    current_ = head_;                // указывает на первый узел, значит current_ получает адрес первого.
                    ptratnext_ = current_ -> next_; // next_ первого узла указывает на второй.
                    delete head_;                  // удаляем первый узел 
                    current_ = ptratnext_;
                } else {                         // со второго узела нужно удалить все
                    ptratnext_ = current_ -> next_;
                    delete current_; 
                    current_ = ptratnext_; 
                }
            }
        } else {
            delete head_;
        }
    }

    void copyelements_(const MyDLList<T>& other) { 
        size_ = 0;
        head_ = nullptr;
        tail_ = nullptr; 
        Node* coppyer = other.head_;
        
        if (other.size_ >= 1) { //Проходим по оригу и пушбэкаем в паль
            for (size_t i = 0; i < other.size_; i++) {
                if (i == 0) {  
                    push_back(coppyer -> data_);    
                } else {  
                    coppyer = coppyer -> next_;
                    push_back(coppyer -> data_);
                }
            }
        } 
    }
    
public: 
    struct iterator {
    private: 
        Node* iter_ = nullptr;
        const MyDLList* ptratlist_ = nullptr;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        T&        operator*() const { return iter_ -> data_; }
        iterator& operator++() { iter_ = iter_ -> next_; return* this; }
        iterator  operator++(int) { auto tmp = *this; iter_ = iter_ -> next_ ; return tmp; }
        iterator& operator--() { iter_ = (iter_ == nullptr) ? ptratlist_ -> tail_ : iter_ -> prev_; return* this;  }
        iterator  operator--(int) { auto tmp = *this; iter_ = (iter_ == nullptr) ? ptratlist_ -> tail_: iter_ -> prev_ ; return tmp; }
        bool      operator!=(const iterator& other) const { return iter_ != other.iter_; }
        bool      operator==(const iterator& other) const = default; 
        T*        operator->() const { return &iter_ -> data_; }

        explicit iterator(Node* ptr = nullptr, const MyDLList* patrik = nullptr) : iter_(ptr), ptratlist_(patrik) {}
    };
    
    iterator begin() { 
        return iterator(head_, this);          
    }

    iterator end() {
        return iterator(nullptr, this); 
    }

    iterator last() {
        return iterator(tail_, this);      
    }

    iterator begin() const { 
        return iterator(head_, this);          
    }

    iterator end() const {
        return iterator(nullptr, this); 
    }

    iterator last() const {
        return iterator(tail_, this);      
    }


    T& operator[](size_t j) {
        if (j >= size_) {
            throw std::out_of_range("Error: this element are not in list");
        } else {
            Node* current_ = nullptr;
            for (size_t i = 0; i != j + 1; i++) {
                if (i == 0) {  
                    current_ = head_;    
                } else {  
                    current_ = current_ -> next_; 
                }
            }  
            return current_ -> data_;
        } 
    }

    const T& operator[](size_t j) const {
        if (j >= size_) {
            throw std::out_of_range("Error: this element are not in list");
        } else {
            Node* current_ = nullptr;
            for (size_t i = 0; i != j + 1; i++) {
                if (i == 0) {  
                    current_ = head_;       
                } else {  
                    current_ = current_ -> next_; 
                }
            }  
            return current_ -> data_;
        } 
    }
    
    MyDLList& operator=(const MyDLList<T>& other) {
        if(this == &other){
            return *this;
        } else {
            deleting_();
            size_ = 0;
            head_ = nullptr; 
            tail_ = nullptr; 
            copyelements_(other);
            return *this;
        }
    } 

    MyDLList& operator=(MyDLList<T>&& other) noexcept {
        if(this == &other){
            return *this;
        } else {
            deleting_();
            size_ = 0;
            head_ = nullptr; 
            tail_ = nullptr; 
            move_(other);
            return *this;
        }
    } 

    void push_back(const T& value) {
        if (size_ >= 1) {
            Node* current = nullptr;
            current = tail_;
            current -> next_ = new Node(value);
            tail_ = current -> next_;
            tail_ -> prev_ = current; 
            size_ ++; 

        } else {
            head_ = new Node(value);
            tail_ = head_;  
            tail_ -> prev_ = nullptr;
            size_ ++; 
        }
    }

    size_t size() const {
        return size_;
    }

    void erase(size_t position) {
        if (position >= size_) { 
            throw std::out_of_range("Error: position are not in this range");
        } else {
            Node* current_ = nullptr; // Успешная проверка на порядковый номер удаляемого елемента 
            if (position == 0) {
                if (size_ >= 2) {
                    current_ = head_; 
                    head_ = current_ -> next_; 
                    delete current_;
                    head_ -> prev_ = nullptr; 
                } else {
                    current_ = head_; 
                    delete current_;
                    head_ = nullptr;
                    tail_ = nullptr;
                }
            } else if (position == size_ - 1) {
                current_ = tail_; 
                tail_ = tail_ -> prev_;
                delete current_; 
                tail_ -> next_ = nullptr;
            } else {
                if (position > size_ / 2) {
                    current_ = tail_; 
                    for (size_t i = 0; i < (size_ - 1) - position; i++) {
                        current_ = current_ -> prev_;
                    }
                    current_ -> prev_ -> next_ = current_ -> next_;
                    current_ -> next_ -> prev_ = current_ -> prev_;  
                        
                    delete current_; 
                     
                } else {     
                    current_ = head_; 
                    for (size_t i = 0; i < position; i++) {
                        current_ = current_ -> next_;
                    }
                    current_ -> prev_ -> next_ = current_ -> next_;
                    current_ -> next_ -> prev_ = current_ -> prev_;  
                    delete current_; 
                }
            }
            size_--;
        }
    }

    void insert(const T& value, size_t position) {
        if (position == 0) {
            Node* newNode = new Node(value);
            newNode -> next_ = head_;
            if (size_ != 0) {
                head_ -> prev_ = newNode;   
            } else {
                tail_ = newNode;            
            }
            head_ = newNode;
        } else if (position == size_) {
            Node* newNode = new Node(value);
            newNode -> prev_ = tail_;       
            tail_ -> next_ = newNode;       
            tail_ = newNode;               
        } else if (position < size_) {
            Node* newNode = new Node(value);
            Node* current_ = head_;
            for (size_t i = 0; i < position - 1; i++) {
                current_ = current_ -> next_;   // дошли до левого 
            }
            newNode -> next_ = current_ -> next_;  
            newNode -> prev_ = current_;           
            current_ -> next_ -> prev_ = newNode;   
            current_ -> next_ = newNode;            
        } else {
            throw std::out_of_range("Error: position are not in");
        }
        size_++;
    }

    void print_list() {
        if (size_ >= 1) {
            Node* current_ = nullptr;
            for (size_t i = 0; i < size_; i++) {
                if (i == 0) {  
                    current_ = head_;
                    std::cout << current_ -> data_ << std::endl;    
                } else {  
                    current_ = current_ -> next_;
                    std::cout << current_ -> data_ << std::endl; 
                    
                }
            }
        } else {
            std::cout << "нет элементов :("<< std::endl;  
        }
    }

    MyDLList(const MyDLList<T>& other) {
        copyelements_(other);
    }

    MyDLList() = default;

    MyDLList(size_t size,const T& value) { // Нужно сделать так, чтобы создалось опр. кол-во узлов с опр. типом и значением этого типа
        for (size_t i = 0; i < size; i++) {
            push_back(value); 
        }
    }

    MyDLList(MyDLList<T>&& other) noexcept
        : size_(other.size_)
        , head_(other.head_)
        , tail_(other.tail_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
    }

    ~MyDLList() {
        deleting_();
    }
};