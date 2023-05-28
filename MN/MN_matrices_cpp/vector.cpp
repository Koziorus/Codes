#include "vector.h"

#include <iostream>

Vector::Vector(int length, double initializing_value)
{
    this->length = length;

    vector = new double[length];

    for(int i = 0; i < length; i++)
    {
        vector[i] = initializing_value;
    }

}

Vector::~Vector()
{
    delete[] vector;
}

double & Vector::operator[] (int column) const
{
    return this->vector[column];
}

Vector::Vector()
{

}

Vector::Vector(int length, double* arr) : Vector(length, 0.0)
{
    for(int i = 0; i < length; i++)
    {
        vector[i] = arr[i];
    }
}
