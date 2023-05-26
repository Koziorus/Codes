//
// Created by Tomek on 25.05.2023.
//

#include "vector.h"

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
