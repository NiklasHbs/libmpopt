#ifndef LIBMPOPT_COMMON_SOLVER_HPP
#define LIBMPOPT_COMMON_SOLVER_HPP

namespace mpopt {

template<typename DERIVED_TYPE>
class solver {
public:
  using clock_type = std::chrono::steady_clock;

  solver()
  : constant_(0)
  , iterations_(0)
  , duration_(0)
  { }

  cost lower_bound() const
  {
    static_cast<const DERIVED_TYPE*>(this)->graph_.check_structure();
    cost result = constant_;

    static_cast<const DERIVED_TYPE*>(this)->for_each_node([&result](const auto* node) {
      result += node->factor.lower_bound();
    });

    return result;
  }

  bool check_primal_consistency() const
  {
    bool result = true;
    const auto* derived = static_cast<const DERIVED_TYPE*>(this);
    derived->for_each_node([&](const auto* node) {
      result = result && derived->check_primal_consistency(node);
    });
    return result;
  }

  cost evaluate_primal() const
  {
    const auto* derived = static_cast<const DERIVED_TYPE*>(this);

    derived->graph_.check_structure();
    cost result = constant_;

    derived->for_each_node(
      [&](const auto* node) {
        if (!derived->check_primal_consistency(node))
          result += infinity;
        result += node->factor.evaluate_primal();
      });

    return result;
  }

  cost upper_bound() const { return evaluate_primal(); }

  void reset_primal()
  {
    static_cast<DERIVED_TYPE*>(this)->for_each_node([](const auto* node) {
      node->factor.reset_primal();
    });
  }

  void run(const int max_iterations)
  {
    assert(false && "Not implemented!");
  }

  double runtime() const
  {
    using seconds = std::chrono::duration<double>;
    return std::chrono::duration_cast<seconds>(duration_).count();
  }

  void solve_ilp()
  {
#ifdef ENABLE_GUROBI
    // We do not reset the primal as they will be used as a MIP start.
    typename DERIVED_TYPE::gurobi_model_builder_type builder(gurobi_env());
    builder.set_constant(constant_);

    static_cast<DERIVED_TYPE*>(this)->for_each_node([&builder](const auto* node) {
      builder.add_factor(node);
    });

    builder.finalize();
    builder.optimize();
    builder.update_primals();
#else
    std::cerr << "Abort: ENABLE_GUROBI was unset during configuration of libmpopt.";
    std::abort();
#endif
  }

protected:
  cost constant_;
  int iterations_;
  clock_type::duration duration_;

#ifdef ENABLE_GUROBI
  std::optional<GRBEnv> gurobi_env_;

  GRBEnv& gurobi_env()
  {
    if (gurobi_env_)
      return *gurobi_env_;
    else
      return gurobi_env_.emplace();
  }
#endif
};

}

#endif

/* vim: set ts=8 sts=2 sw=2 et ft=cpp: */
