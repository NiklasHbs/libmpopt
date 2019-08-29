#ifndef LIBMPOPT_QAP_H
#define LIBMPOPT_QAP_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mpopt_qap_solver_t mpopt_qap_solver;
typedef struct mpopt_qap_graph_t mpopt_qap_graph;
typedef struct mpopt_qap_unary_node_t mpopt_qap_unary_node;
typedef struct mpopt_qap_uniqueness_node_t mpopt_qap_uniqueness_node;
typedef struct mpopt_qap_pairwise_node_t mpopt_qap_pairwise_node;

mpopt_qap_solver* mpopt_qap_solver_create();
void mpopt_qap_solver_destroy(mpopt_qap_solver* s);
void mpopt_qap_solver_finalize(mpopt_qap_solver* s);
mpopt_qap_graph* mpopt_qap_solver_get_graph(mpopt_qap_solver* s);
mpopt_qap_unary_node* mpopt_qap_graph_add_unary(mpopt_qap_graph* graph, int idx, int number_of_labels, int number_of_forward, int number_of_backward);
mpopt_qap_uniqueness_node* mpopt_qap_graph_add_uniqueness(mpopt_qap_graph* graph, int idx, int number_of_unaries);
mpopt_qap_pairwise_node* mpopt_qap_graph_add_pairwise(mpopt_qap_graph* graph, int idx, int number_of_labels0, int number_of_labels1);
void mpopt_qap_graph_add_pairwise_link(mpopt_qap_graph* graph, int idx_unary0, int idx_unary1, int idx_pairwise);
void mpopt_qap_graph_add_uniqueness_link(mpopt_qap_graph* graph, int idx_unary, int label, int idx_uniqueness, int slot);
void mpopt_qap_solver_run(mpopt_qap_solver* s, int max_iterations);
void mpopt_qap_solver_solve_ilp(mpopt_qap_solver* s);
void mpopt_qap_solver_execute_combilp(mpopt_qap_solver* s);
double mpopt_qap_solver_lower_bound(mpopt_qap_solver* s);
double mpopt_qap_solver_evaluate_primal(mpopt_qap_solver* s);

void mpopt_qap_unary_set_cost(mpopt_qap_unary_node* n, int label, double cost);
void mpopt_qap_uniqueness_set_cost(mpopt_qap_uniqueness_node* n, int unary, double cost);
void mpopt_qap_pairwise_set_cost(mpopt_qap_pairwise_node* n, int l0, int l1, double cost);

#ifdef __cplusplus
}
#endif

#endif

/* vim: set ts=8 sts=2 sw=2 et ft=c: */
