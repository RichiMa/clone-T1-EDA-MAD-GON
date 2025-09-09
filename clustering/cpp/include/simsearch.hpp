#ifndef _SIM_SEARCH_HPP
#define _SIM_SEARCH_HPP

#include "matrix.hpp"
#include "cluster.hpp"
#include <vector>

// Estructura de resultado con métricas
struct SearchResult {
    std::vector<size_t> top;   // índices de los vecinos
    std::vector<float> dists;  // distancias correspondientes
    long long comparisons;     // número de comparaciones
    double time_no_sort;       // ms antes de ordenar
    double time_total;         // ms total
};

class SimSearch {
private:
    const Matrix &mat_data;      // dataset
    const Cluster &mat_clusters; // clusters/centroides

public:
    SimSearch(const Matrix &mat, const Cluster &clust);

    SearchResult search_without(const float* query, size_t top_k);
    SearchResult search_with_clusters(const float* query, size_t top_k);
};

#endif
