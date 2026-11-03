#pragma once

#include <iostream>
#include <limits.h>
#include <stdexcept>

template <class T>
class MyVector{ 
private: 
    size_t capacity_; // фактический размер блока данных. Razmer videlennoy pamati / максимально доступный 
    size_t size_;    // Polzovatelyu < capacity / текущий размер 
    T* data_;       // будет хранить место (обновление). Выделение динамической памяти (здесь запись). //тут необязателен int 

    void copyelements(const MyVector<T>& other) {
        data_ = new T[other.capacity_];
        capacity_ = other.capacity_;
        size_ = other.size_;

        for(int i = 0; i < size_; i++) {
            data_[i] = other.data_[i]; 
        }
    } // element equals another elements (copying elemenets) 

    void memexp() { 
        if(size_ + 1 > capacity_) { 
            if (capacity_ == 0) {   //Пустой капасити 
                T *tmp = new T[2]; //тут необязателен int 
                delete[] data_; 
                data_ = tmp; 
                capacity_ = 2; 
            } else if (capacity_ < INT_MAX / 2) {    // 1. capacity меньше половины макс. зн - я 
                T* tmp = new T[capacity_ * 2];      //тут необязателен int 
                for(int i = 0; i < size_; i++){ 
                    tmp[i] = data_[i]; 
                } 
                delete[] data_; 
                data_ = tmp; 
                capacity_ = capacity_ * 2; 
            } else if (capacity_ != INT_MAX) { // 2. больше половины 
                T* tmp = new T[INT_MAX];      // тут необязателен int 
                for(int i = 0; i < size_; i++){ 
                    tmp[i] = data_[i]; 
                } 
                delete[] data_; 
                data_ = tmp; 
                capacity_ = INT_MAX; 
            } else { 
                throw std::length_error("Error: Object exceeds maximum implementation length."); 
            } // Просто пушбэк если для сайза есть место 
        } 
    }; 

    void move_(MyVector<T>& other) {
        size_ = other.size_; 
        capacity_ = other.capacity_; 
        data_ = other.data_; 
        other.data_ = nullptr; //копирование 
        other.size_ = 0; 
        other.capacity_ = 0; 
    }

public:

    struct iterator {
        T* iter; 

        T& operator*() {
            return *iter; 
        }

        iterator& operator++() {
            iter += 1;
            return* this; 
        }

        bool operator!=(const iterator& other) const {
            return iter != other.iter;
        }

    };
    
    iterator begin() {
        iterator iterat;
        iterat.iter = data_;
        return iterat;          
    }

    iterator end() {
        iterator iterat;
        iterat.iter = data_ + size_; 
        return iterat; 
    }
    
    MyVector& operator=(const MyVector<T>& other) {  // Метод который копирует в какой- то уже созданный массив. 
        if(this == &other){
            return *this;
        } else {
            delete[] data_; 
            copyelements(other);
            return *this;            
        }
    }

    MyVector& operator=(MyVector<T>&& other) noexcept {  
        if(this == &other){
            return *this;
        } else {
            delete[] data_; 
            move_(other);
            return *this;            
        }
    }

    T& operator[](int i) { 
        return data_[i];  // Возврат элемента по сссылке 
    } 

    MyVector(const MyVector<T>& other) {  //Если нет массива куда копировать
        copyelements(other);
    }

    MyVector() { 
        size_ = 0; 
        capacity_ = 0; 
        data_ = nullptr; 
    } 

    MyVector(const int size) { 
        data_ = new T[size]; //тут необязателен int 
        capacity_ = size; 
        size_ = size; 
    } 

    void insert(T ElVal, int ElNum) { 
        if(ElNum >= 0 && ElNum <= size_){
            memexp(); 
            for(int i = size_; i != ElNum ; i--){ 
                data_[i] = data_[i - 1]; // Первый цикл для сдвига всего вправо
            } 

            data_[ElNum] = ElVal;
            size_++;

        } else {
            throw std::out_of_range("Error: there is no elements with index less than 0.");
        }
    }

    void erase(int j) { 
        if(j >= size_){ 
            throw std::out_of_range("Error: Object are not in data_."); 
        } else { 
            if(j < 0){ 
                throw std::out_of_range("Error: the data_ num starts at 0."); 
            } else { 
                for(int i = j; i < size_ - 1; i++){ 
                    data_[i] = data_[i + 1]; 
                } 
                size_ = size_ - 1; 
            } 
        } 
    } 

    void push_back(T value) { //тут необязателен int 
        memexp();
        data_[size_] = value; 
        size_++; 
    }; 

    int size() const { 
        return size_; 
    }; 

    void print_elements() const { 
        if(size_ != 0){ 
            std::cout << "Элементы: "; 
            for(int i = 0; i < size_; i++){ 
                std::cout << data_[i]; 
                if(i < size_ - 1){ 
                    std::cout << ", "; 
                } 
            } 
            std::cout << std::endl; 
        } else { 
            std::cout << "Массив пуст" << std::endl; 
        } 
    }; 

    MyVector(MyVector<T>&& other) noexcept {
        move_(other); 
    }

    ~MyVector() { 
        delete[] data_; 
    } 
};