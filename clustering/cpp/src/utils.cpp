#include "utils.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <cstdlib>

float vec_compute_distance(const float* u, const float* v, size_t dim){
    float d = 0;
    float s = 0;
    float diff = 0;
    for (size_t i = 0;  i < dim; i++){
        diff = (u[i] - v[i]);
        s = s + diff * diff;
    }
    d = std::sqrt(s);
    return d;
}

void vec_add(float* s, const float* u,  size_t dim){
    for (size_t i = 0;  i < dim; i++){
        s[i] = u[i] + s[i];
    }
}

void vec_divide(float* u, float scalar, size_t dim){
    for (size_t i = 0;  i < dim; i++){
        u[i] = u[i] / scalar;
    }
}

void set_array(float *array, size_t dim, float val){
    for (size_t i = 0;  i < dim; i++){
        array[i] = val;
    }
}

float vec_compute_avg_dif(const float *u, const float* v,  size_t dim){

    float dif = 0;
    for (size_t i = 0;  i < dim; i++){        
        dif = dif + std::abs(u[i] - v[i]);
        //std::cout << u[i] << " - " << v[i] << "  " << std::abs(u[i] - v[i]) << std::endl;
    }   
    //std::cout << dif <<std::endl;
    return dif / dim;
}

void print_array(const float *array, size_t d){
    for (size_t i = 0; i< d; i ++){
        std::cout << array[i] << " ";
    }
    std::cout<< std::endl;
}

// QuickSort modificado para que devuelva los índices en orden.
void swap1(std::vector<size_t> &idxs, int i, int j){
    size_t aux = idxs[i];
    idxs[i] = idxs[j];
    idxs[j] = aux;
}

void swap2(float* A, int i, int j){
    float aux = A[i];
    A[i] = A[j];
    A[j] = aux;
}

int split_qs(float* A, int i, int j, std::vector<size_t> &idxs){
    /***
     * split for quicksort
     * i,j are the endpoints
     */
    int p = i + rand()%(j - i + 1);
    // if (!p%10) std::cout << "Sorting... " << std::endl;

    while (i  < j) {

        while ( i < p && A[i] <= A[p]){
            i = i + 1;
        }

        while ( j > p && A[j] >= A[p]){
            j = j - 1;
        }

        swap1(idxs, i, j);
        swap2(A, i, j);
    
        if (i == p){
            p = j;
        }
        else if (j == p){
            p = i;
        }
    }
    return p;
}

void argsort(float* A, int i, int j, std::vector<size_t> &idxs){
    if (i < j){
        int k = split_qs(A, i, j, idxs);
        argsort(A, i, k-1, idxs);
        argsort(A, k + 1, j, idxs);
    }
}

// (Creo que se puede implementar una versión modificada de QuickSort para argsort
// en lugar de usar la librería <algorithm>) ESTA ES UNA ALTERNATIVA
// std::vector<size_t> argsort(const float* dist, size_t N)
// {
//     // Now, I want an array with the indexes, which will be the one to be sort
//     std::vector<size_t> idxs(N);
//     // The std::iota() function can fill the array with values from 0 to 999
//     std::iota(idxs.begin(), idxs.end(), 0);
//     // Now, idxs = [0,1, ..., 999]
//     // The next step is more complex:
//     // I want to have the indexes of the dist array sorted as if dist was sorted descendingly, this is
//     // to "argsort" dist and save the output to idxs.
//     // Right now dist[idxs[i]] = dist[i]. I will sort idxs, based on the values of dist, in such a way that
//     // dist[idxs[0]] = the closest point to the query
//     // dist[idxs[1]] = the second closest point to the query
//     // and so on.
//     sort(idxs.begin(), idxs.end(),
//         [&dist](size_t i1, size_t i2) {return dist[i1] > dist[i2];});

//     return idxs;
// }
