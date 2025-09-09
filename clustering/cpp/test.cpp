#include "utils.hpp"
#include "matrix.hpp"
#include "cluster.hpp"
#include "simsearch.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

int main() {
    // cargar dataset y queries
    Matrix mat("/home/eda/eda_cpp/clustering/data_eda.npy");
    Matrix queries("/home/eda/eda_cpp/clustering/queries_eda.npy");
    size_t Q = queries.getN();

    // valores de m y k a testear
    std::vector<size_t> m_values = {16, 32, 64, 128};
    std::vector<size_t> k_values = {8, 16, 32, 64, 128};

    // encabezado bonito
    std::cout << std::left
              << std::setw(6)  << "m"
              << std::setw(6)  << "k"
              << std::setw(14) << "Comp_exh"
              << std::setw(14) << "NoSort_exh(ms)"
              << std::setw(14) << "Total_exh(ms)"
              << std::setw(14) << "Comp_clu"
              << std::setw(16) << "NoSort_clu(ms)"
              << std::setw(16) << "Total_clu(ms)"
              << std::setw(14) << "Error_prom"
              << std::endl;

    std::cout << std::string(108, '-') << std::endl;

    for (size_t m : m_values) {
        for (size_t k : k_values) {
            // acumuladores
            double total_time_exh = 0, total_no_sort_exh = 0;
            long long total_comp_exh = 0;

            double total_time_clu = 0, total_no_sort_clu = 0;
            long long total_comp_clu = 0;
            double total_error = 0.0;

            // preparar cluster y buscador
            Cluster cluster(mat, k);
            cluster.applyClustering();
            SimSearch simsearch(mat, cluster);

            for (size_t q = 0; q < Q; q++) {
                const float* query = queries.getRow(q);

                // exhaustivo
                auto res_exh = simsearch.search_without(query, m);
                total_time_exh += res_exh.time_total;
                total_no_sort_exh += res_exh.time_no_sort;
                total_comp_exh += res_exh.comparisons;

                // clustering
                auto res_clu = simsearch.search_with_clusters(query, m);
                total_time_clu += res_clu.time_total;
                total_no_sort_clu += res_clu.time_no_sort;
                total_comp_clu += res_clu.comparisons;

                // error entre distancias
                double err_q = 0.0;
                for (size_t i = 0; i < m; i++) {
                    err_q += std::abs(res_clu.dists[i] - res_exh.dists[i]);
                }
                err_q /= m;
                total_error += err_q;
            }

            // promedios
            double avg_time_exh = total_time_exh / Q;
            double avg_no_sort_exh = total_no_sort_exh / Q;
            double avg_comp_exh = (double)total_comp_exh / Q;

            double avg_time_clu = total_time_clu / Q;
            double avg_no_sort_clu = total_no_sort_clu / Q;
            double avg_comp_clu = (double)total_comp_clu / Q;

            double avg_error = total_error / Q;

            // imprimir fila alineada
            std::cout << std::left
                      << std::setw(6)  << m
                      << std::setw(6)  << k
                      << std::setw(14) << avg_comp_exh
                      << std::setw(14) << avg_no_sort_exh
                      << std::setw(14) << avg_time_exh
                      << std::setw(14) << avg_comp_clu
                      << std::setw(16) << avg_no_sort_clu
                      << std::setw(16) << avg_time_clu
                      << std::setw(14) << avg_error
                      << std::endl;
        }
    }

    return 0;
}
