#pragma once

#include <iostream>
#include <limits.h>
#include <stdexcept>

  //  head_                                                 
 //     |                                                                    
//  [А|next] --> [Б|next] --> [В|next] --> [Г|next] --> nullptr

// int узел 16 байт, 4 данные, 4 в паддинг и 8. последний узел хранит в next_ нулл птр, который тоже 8 занимает.

template <class T>
class MyList{
private:

    size_t size_; // количество узлов 
    
    struct Node {
        T data_;
        Node* next_; 
        Node(T value) : data_(value), next_(nullptr) {}
    };
    Node* head_;

    void move_(MyList<T>& other) {
        size_ = other.size_;
        head_ = other.head_; 
        other.head_ = nullptr; 
        other.size_ = 0; 
    }

    void deleting_() {
        Node* current_;
        Node* ptratnext_;             //нужно удалять все элементы начиная с первого и запоминая указатель на следующий элемент. 
        if (size_ >= 1) {                               // размер 1 и более - тогда действуем
            for (int i = 0; i < size_; i++) {          // проходим все узлы 
                if (i == 0) {                         // Первый узел
                    current_ = head_;                // указывает на первый узел, значит current_ получает адрес первого.
                    ptratnext_ = current_ -> next_; // next_ первого узла указывает на второй.
                    delete head_;                  // Удаляем первый узел 
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
    
    void copyelements_(const MyList<T>& other){ 
        size_ = 0;
        head_ = nullptr;
        Node* coppyer = other.head_;
        
        if (other.size_ >= 1) { //Проходим по оригу и пушбэкаем в паль
            for (int i = 0; i < other.size_; i++) {
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
        Node* iter; 

        T& operator*() {
            return iter -> data_; 
        }

        iterator& operator++() {
            iter = iter -> next_;
            return* this; 
        }

        bool operator!=(const iterator& other) const {
            return iter != other.iter;
        }
    };
    
    iterator begin() {
        iterator iterat;
        iterat.iter = head_;
        return iterat;          
    }

    iterator end() {
        iterator iterat;
        iterat.iter = nullptr; 
        return iterat; 
    }

    T& operator[](size_t j) {
        if (j >= size_) {
            throw std::out_of_range("Error: this element are not in list");
        } else {
            Node* current_;
            for (int i = 0; i != j + 1; i++) {
                if (i == 0) {  
                    current_ = head_;    
                } else {  
                    current_ = current_ -> next_; 
                }
            }  
            return current_ -> data_;
        } 
    }          

    MyList& operator=(const MyList<T>& other) {
        if(this == &other){
            return *this;
        } else {
            deleting_();
            size_ = 0;
            head_ = nullptr; 
            copyelements_(other);
            return *this;
        }
    }

    MyList& operator=(MyList<T>&& other) noexcept {
        if(this == &other){
            return *this;
        } else {
            deleting_();
            size_ = 0;
            head_ = nullptr; 
            move_(other);
            return *this;
        }
    }

    int size() const { 
        return size_; 
    }; 

    void insert(T value, size_t position) {
        if (position == 0) { 
            Node* newNode = new Node(value);  
            newNode -> next_ = head_;    
            head_ = newNode;
        } else { 
            if (size_ >= 1 && position <= size_) {
                Node* current_ = head_; 
                Node* newNode = new Node(value); 
                for (int i = 0; i < position - 1; i++) {
                    current_ = current_ -> next_;   // дошли до Б
                }
                newNode -> next_ = current_ -> next_;       
                current_ -> next_ = newNode;             
            } else {
                throw std::out_of_range("Error: position are not in");
            }
        }
        size_++;
    }

    void erase(size_t position) {
        if (position >= size_) {
            throw std::out_of_range("Error: position are not in this range");
        } else {
        if (position > 0) {
            Node* ptratnext_;
            Node* current_;
            if (size_ >= 1) {
            for (int i = 0; i < position; i++) {
                if (i == 0) {  
                    current_ = head_;  
                } else {
                    current_ = current_ -> next_;
                }
            }
                current_ = current_ -> next_;
                ptratnext_ = current_ -> next_;
                delete current_;
                
            for (int i = 0; i < position; i++) {
                if (i == 0) {  
                    current_ = head_;  
                } else {
                    current_ = current_ -> next_;
                }
            }    
            current_ -> next_ = ptratnext_;   
            } else {
                throw std::out_of_range("Error: there is no - values");
            }
        } else if (position == 0) {
            Node* current_;
            current_ = head_ -> next_;
            delete head_;
            head_ = current_; 
        } else {
            throw std::out_of_range("Error: there is no - values");
        }
        size_--;
        }
    }

    void push_back(T value) { 
        Node* current_; // Kakoe zn-e zakidivat v escho odin uzel
        if (size_ == 0){  
            head_ = new Node(value);
        } else {
            for (int i = 0; i < size_ ; i++) {
                if (i == 0) {
                    current_ = head_;
                } else {
                    current_ = current_ -> next_;
                } 
            } 
            current_ -> next_ = new Node(value);
        }
        size_++;
    }

    void print_list() const { // PROXOD // тот же самый алгоритм с деструктора только без удаления и с парой модификаций 
        if (size_ >= 1) {
            Node* current_;
            for (int i = 0; i < size_; i++) {
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

    MyList(const MyList<T>& other) {
        copyelements_(other);
    }

    MyList() { // default constructor cantain only 1 element who will point at nullptr
        size_ = 0; 
        head_ = nullptr;
    }

    MyList(size_t size, T value) {
        Node* previous_;
        size_ = size; 
        
        if (size_ >= 1) {
            for (int i = 0; i < size_; i++) {
                if (i == 0) { // Первый круг 
                    head_ = new Node(value); // создали новый узел на который указывает head 
                } else if (i == 1) { // Второй круг
                    previous_ = head_; // Закинули в переменную адрес того, на что указывает хед
                    head_ -> next_ = new Node(value); // новый узел теперь связан с еще одним новым узлом. 
                } else { // Последующие круги
                    previous_ -> next_ -> next_ = new Node(value); // новый узел теперь связан с еще одним новым узлом.
                    previous_ = previous_ -> next_; // обновляю значение внутри previous 
                }
            }
        } else {
            head_ = nullptr;
        }
    }

    MyList(MyList<T>&& other) noexcept {
        move_(other); 
    }

    ~MyList() {
        deleting_();
    }
};