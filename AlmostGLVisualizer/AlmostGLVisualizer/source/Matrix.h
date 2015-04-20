#ifndef Matrix_h__
#define Matrix_h__

template<typename T, unsigned int ROWS, unsigned int COLS>
class Matrix
{
public:
private:
    T data_[ROWS][COLS];
};

#endif // Matrix_h__
