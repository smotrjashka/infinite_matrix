#ifndef INFINITE_MATRIX_MATRIX_H
#define INFINITE_MATRIX_MATRIX_H

#include <set>

template<typename T, T def_value>
struct Matrix{

    Matrix() : real_size_(0) {

    };

    //TODO copy constructor


    int size(){
        return real_size_;
    }

    void IncrementSize(){
        real_size_++;
    }

        struct MatrixCell{
            MatrixCell(Matrix<T, def_value>* matrix_ptr, unsigned int row_number, unsigned int index)
                    : row_number_(row_number), index_(index), value_(def_value), matrix_ptr_(matrix_ptr){

            };
            MatrixCell(Matrix<T, def_value>* matrix_ptr, unsigned int row_number, unsigned int index, T value)
                    : row_number_(row_number), index_(index), value_(value), matrix_ptr_(matrix_ptr){

            };

            MatrixCell(MatrixCell& matrixCell){
                row_number_ = matrixCell.RowNumber();
                index_ = matrixCell.Index();
                value_ = matrixCell.Value();
            };

            MatrixCell(const MatrixCell& matrixCell){
                row_number_ = matrixCell.RowNumber();
                index_ = matrixCell.Index();
                value_ = matrixCell.Value();
            };

            bool is_same_index(unsigned int row_number, unsigned int index){
                return (index_ == index)&&(row_number==row_number_);
            };

            T& operator=(T value){
                value_ = value;
                matrix_ptr_->IncrementSize();
            };

            bool operator==(T value){
                return value==value_;
            }

            bool operator< (const MatrixCell cell) const {
                if (cell.row_number_==row_number_) {
                    return index_ < cell.index_;
                } else {
                    return row_number_ < cell.row_number_;
                }
            };

            T Value(){
                return value_;
            };

            unsigned int RowNumber(){
                return row_number_;
            }

            unsigned int Index(){
                return index_;
            };

            bool is_same_row(unsigned int row_number){
                return row_number_==row_number;
            }


        private:
            unsigned int row_number_;
            unsigned int index_;
            T value_;
            Matrix<T, def_value>* matrix_ptr_;
        };


    struct MatrixRow{

        MatrixRow(Matrix<T, def_value>* matrix_ptr, unsigned int row_index) : matrix_ptr_(matrix_ptr),
                                                row_index_(row_index){
            for(MatrixCell* matrix_cell_ptr : matrix_ptr_->rows_) {
                if (matrix_cell_ptr->is_same_row(row_index_)){
                    row_.insert(matrix_cell_ptr);
                }
            }
        }

        MatrixCell& operator[](unsigned int left_index){
            for (MatrixCell* matrix_cell_ptr : row_) {
                if (matrix_cell_ptr->Index()==left_index){
                    return *matrix_cell_ptr;
                }
            }
            auto matrix_cell_new = new MatrixCell(matrix_ptr_, row_index_, left_index);
            matrix_ptr_->rows_.insert(matrix_cell_new);
            return *matrix_cell_new;
        }



    private:
        Matrix* matrix_ptr_;
        std::set<MatrixCell*> row_;
        unsigned int row_index_;
    };

    MatrixRow& operator[](unsigned int right_index ){

        return *(new MatrixRow(this, right_index));

    } ;


    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = MatrixCell;
        using pointer           = MatrixCell*;  // or also value_type*
        using reference         = MatrixCell&;  // or also value_type&

        Iterator(std::set<MatrixCell*> row_ptrs, pointer ptr) : m_ptr_(ptr), row_ptrs_(row_ptrs) {}

        pointer operator->() { return m_ptr_; }

        // Prefix increment
        Iterator& operator++() {
            m_ptr_ = *((row_ptrs_.find(m_ptr_))++);
            return *this;
        }

        /*// Postfix increment
        Iterator operator++(int) {
            Iterator tmp = *this;
            for (auto it = row_ptrs_.begin(); it < row_ptrs_.end(); it++) {
                if ((*it)==m_ptr_){
                    m_ptr_ == *(it++);
                }
            };
            return tmp; }*/

        std::tuple<int, int, T> operator*() const
        {
            return std::make_tuple((*m_ptr_).RowNumber(),
                                   (*m_ptr_).Index(),
                                   (*m_ptr_).Value());
        }


        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr_ == b.m_ptr_; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr_ != b.m_ptr_; };

    private:
        std::set<MatrixCell*> row_ptrs_;
        pointer m_ptr_;
    };



    Iterator begin() { return Iterator(rows_, *(rows_.begin())); }
    Iterator end()   { return Iterator(rows_, *(rows_.end())); }

private:
    std::set<MatrixCell*> rows_;
    unsigned int real_size_;
};

#endif //INFINITE_MATRIX_MATRIX_H
