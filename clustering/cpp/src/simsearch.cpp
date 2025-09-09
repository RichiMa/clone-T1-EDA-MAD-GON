#include "simsearch.hpp"
#include "utils.hpp"
#include <numeric>
#include <chrono>

SimSearch::SimSearch(const Matrix &mat, const Cluster &clust)
    : mat_data(mat), mat_clusters(clust) {}

SearchResult SimSearch::search_without(const float* query, size_t top_k) {
    SearchResult res;
    res.comparisons = 0;

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<float> dist(mat_data.getN());
    for (size_t i = 0; i < mat_data.getN(); i++) {
        dist[i] = vec_compute_distance(mat_data.getRow(i), query, mat_data.getDim());
        res.comparisons++;
    }

    auto mid = std::chrono::high_resolution_clock::now();

    std::vector<size_t> idxs(mat_data.getN());
    std::iota(idxs.begin(), idxs.end(), 0);
    argsort(dist.data(), 0, mat_data.getN() - 1, idxs);

    res.top.assign(idxs.begin(), idxs.begin() + top_k);
    res.dists.assign(dist.begin(), dist.begin() + top_k);

    auto end = std::chrono::high_resolution_clock::now();
    res.time_no_sort = std::chrono::duration<double, std::milli>(mid - start).count();
    res.time_total   = std::chrono::duration<double, std::milli>(end - start).count();

    return res;
}

SearchResult SimSearch::search_with_clusters(const float* query, size_t top_k) {
    SearchResult res;
    res.comparisons = 0;

    auto start = std::chrono::high_resolution_clock::now();

    // distancias a centroides
    std::vector<float> dist_to_centroid(mat_clusters.get_k());
    for (size_t i = 0; i < mat_clusters.get_k(); i++) {
        dist_to_centroid[i] = vec_compute_distance(query, mat_clusters.getCentroid(i), mat_data.getDim());
        res.comparisons++;
    }

    std::vector<size_t> sorted_centroids(mat_clusters.get_k());
    std::iota(sorted_centroids.begin(), sorted_centroids.end(), 0);
    argsort(dist_to_centroid.data(), 0, mat_clusters.get_k() - 1, sorted_centroids);

    // seleccionar clusters suficientes
    size_t num_clusters = 1;
    size_t elements = mat_clusters.getInds(sorted_centroids[0]).size();
    while (elements < top_k && num_clusters < mat_clusters.get_k()) {
        elements += mat_clusters.getInds(sorted_centroids[num_clusters]).size();
        num_clusters++;
    }

    // recolectar índices candidatos
    std::vector<size_t> idxs;
    for (size_t c = 0; c < num_clusters; c++) {
        auto inds = mat_clusters.getInds(sorted_centroids[c]);
        idxs.insert(idxs.end(), inds.begin(), inds.end());
    }

    // distancias a candidatos , ahora si que si
    std::vector<float> dist(idxs.size());
    for (size_t i = 0; i < idxs.size(); i++) {
        dist[i] = vec_compute_distance(mat_data.getRow(idxs[i]), query, mat_data.getDim());
        res.comparisons++;
    }

    auto mid = std::chrono::high_resolution_clock::now();

    argsort(dist.data(), 0, idxs.size() - 1, idxs);

    res.top.assign(idxs.begin(), idxs.begin() + top_k);
    res.dists.assign(dist.begin(), dist.begin() + top_k);

    auto end = std::chrono::high_resolution_clock::now();
    res.time_no_sort = std::chrono::duration<double, std::milli>(mid - start).count();
    res.time_total   = std::chrono::duration<double, std::milli>(end - start).count();

    return res;
}
