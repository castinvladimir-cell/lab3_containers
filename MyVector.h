#pragma once

#include <iostream>
#include <limits>                 
#include <stdexcept>
#include <utility>
#include <iterator>
#include <compare>
#include <memory>

template <class T>
class MyVector{ 
private:
    size_t capacity_ = 0;       // Максимально доступный размер памяти
    size_t size_ = 0;          // Текущий размер 
    std::unique_ptr<T[]> data_ = nullptr; // Место (обновление)

    void copyelements(const MyVector<T>& other) {
        data_ = std::make_unique<T[]>(other.capacity_);
        capacity_ = other.capacity_;
        size_ = other.size_;

        for(size_t i = 0; i < size_; i++) {
            data_[i] = other.data_[i]; 
        }
    } // element equals another elements (copying elemenets) 

    void memexp() { 
        if(size_ + 1 > capacity_) { 
            if (capacity_ == 0) {                                            //Пустой capacity
                data_ = std::make_unique<T[]>(2); 
                capacity_ = 2; 
            } else if (capacity_ < std::numeric_limits<size_t>::max() / 2) { // capacity меньше половины макс. зн - я 
                auto tmp = std::make_unique<T[]>(capacity_ * 2);                                
                for(size_t i = 0; i < size_; i++){ 
                    tmp[i] = std::move(data_[i]); 
                } 
                data_ = std::move(tmp); 
                capacity_ = capacity_ * 2; 
            } else { 
                throw std::length_error("Error: Object exceeds maximum implementation length."); 
            }                                                                // Просто пушбэк если для сайза есть место                 
        } 
    }; 

    void move_(MyVector<T>& other) {
        size_ = other.size_; 
        capacity_ = other.capacity_; 
        data_ = std::move(other.data_); 
        other.size_ = 0; 
        other.capacity_ = 0; 
    }

public:

    struct iterator {
    private:
        T* iter = nullptr; 
    public:
        using iterator_category = std::contiguous_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;

        T&        operator*() const { return *iter; }
        iterator& operator++() { iter += 1; return* this; }
        iterator  operator++(int) { auto tmp = *this; ++iter; return tmp; }
        iterator& operator--() { --iter; return *this; }
        iterator  operator--(int) { auto tmp = *this; --iter; return tmp; }
        bool      operator!=(const iterator& other) const { return iter != other.iter; }

        iterator&       operator+=(difference_type n) { iter += n; return *this; }
        iterator&       operator-=(difference_type n) { iter -= n; return *this; }
        friend iterator operator+(iterator it, difference_type n) { it += n; return it; }
        friend iterator operator+(difference_type n, iterator it) { it += n; return it; }
        friend iterator operator-(iterator it, difference_type n) { it -= n; return it; }

        friend difference_type operator-(const iterator& lhs, const iterator& rhs) { return lhs.iter- rhs.iter; }
        reference              operator[](difference_type n) const { return *(iter + n); }
        pointer                operator->() const { return iter; }
        auto                   operator<=>(const iterator& other) const = default;

        explicit iterator(T* ptr = nullptr) : iter(ptr) {}
    };

    struct const_iterator {
    private:
        const T* iter = nullptr;
    public:
        using iterator_category = std::contiguous_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const T*;
        using reference         = const T&;

        const T&        operator*() const { return *iter; }
        const_iterator& operator++() { iter += 1; return* this; }
        const_iterator  operator++(int) { auto tmp = *this; ++iter; return tmp; }
        const_iterator& operator--() { --iter; return *this; }
        const_iterator  operator--(int) { auto tmp = *this; --iter; return tmp; }
        bool            operator!=(const const_iterator& other) const { return iter != other.iter; }

        const_iterator&       operator+=(difference_type n) { iter += n; return *this; }
        const_iterator&       operator-=(difference_type n) { iter -= n; return *this; }
        friend const_iterator operator+(const_iterator it, difference_type n) { it += n; return it; }
        friend const_iterator operator+(difference_type n, const_iterator it) { it += n; return it; }
        friend const_iterator operator-(const_iterator it, difference_type n) { it -= n; return it; }

        friend difference_type operator-(const const_iterator& lhs, const const_iterator& rhs) { return lhs.iter - rhs.iter; }
        reference              operator[](difference_type n) const { return *(iter + n); }
        pointer                operator->() const { return iter; }
        auto                   operator<=>(const const_iterator& other) const = default;

        explicit const_iterator(const T* ptr = nullptr) : iter(ptr) {}
    };
    
    iterator begin() {
        return iterator(data_.get());        
    }

    iterator end() {
        return iterator(data_.get() + size_);
    }
    
    const_iterator begin() const {
        return const_iterator(data_.get());        
    }

    const_iterator end() const {
        return const_iterator(data_.get() + size_);
    }
    
    MyVector& operator=(const MyVector<T>& other) {  // Метод который копирует в какой- то уже созданный массив. 
        if(this == &other){
            return *this;
        } else {
            copyelements(other);
            return *this;            
        }
    }

    MyVector& operator=(MyVector<T>&& other) noexcept {  
        if(this == &other){
            return *this;
        } else {
            move_(other);
            return *this;            
        }
    }

    T& operator[](size_t i) { 
        return data_[i];  // Возврат элемента по сссылке 
    } 

    const T& operator[](size_t i) const{ 
        return data_[i];  // Возврат элемента по сссылке без доступа к изменению
    } 

    MyVector(const MyVector<T>& other) {//Если нет массива куда копировать 
        copyelements(other);
    }

    MyVector() = default; 

    MyVector(const size_t size)
        : capacity_(size)
        , size_(size)
        , data_(std::make_unique<T[]>(size)) {} 

    void insert(const T& ElVal, size_t ElNum) { 
        if(ElNum <= size_) {
            memexp(); 
            for(size_t i = size_; i != ElNum ; i--){ 
                data_[i] = std::move(data_[i - 1]); // Первый цикл для сдвига всего вправо
            } 
            data_[ElNum] = ElVal;
            size_++;
        } else {
            throw std::out_of_range("Error: vrong index.");
        }
    }

    void erase(size_t j) { 
        if(j >= size_){ 
            throw std::out_of_range("Error: Object are not in data_."); 
        } else {  
            for(size_t i = j; i < size_ - 1; i++){ 
                data_[i] = std::move(data_[i + 1]); 
            } 
            size_ = size_ - 1; 
        } 
    } 

    void push_back(const T& value) { 
        memexp();                    // константая ссылка 
        data_[size_] = value; 
        size_++; 
    }; 

    size_t size() const { 
        return size_; 
    }; 

    size_t capacity() const { 
        return capacity_; 
    } 

    void print_elements() const { 
        if(size_ != 0){ 
            std::cout << "Элементы: "; 
            for(size_t i = 0; i < size_; i++){ 
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

    MyVector(MyVector<T>&& other) noexcept
        : capacity_(other.capacity_)
        , size_(other.size_)
        , data_(std::move(other.data_)) {
        other.size_ = 0;
        other.capacity_ = 0;
    }
};