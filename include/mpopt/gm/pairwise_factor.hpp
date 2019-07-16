#ifndef LIBMPOPT_GM_PAIRWISE_FACTOR_HPP
#define LIBMPOPT_GM_PAIRWISE_FACTOR_HPP

namespace mpopt {
namespace gm {

template<typename ALLOCATOR = std::allocator<cost>>
class pairwise_factor {
public:
  using allocator_type = ALLOCATOR;
  static constexpr cost initial_cost = std::numeric_limits<cost>::signaling_NaN();
  static constexpr index primal_unset = std::numeric_limits<index>::max();

  pairwise_factor(index number_of_labels0, index number_of_labels1, const ALLOCATOR& allocator = ALLOCATOR())
  : costs_(number_of_labels0 * number_of_labels1, initial_cost, allocator)
  , no_labels0_(number_of_labels0)
  , no_labels1_(number_of_labels1)
#ifndef NDEBUG
  , index0_(-1)
  , index1_(-1)
#endif
  {
  }

  pairwise_factor(const pairwise_factor& other) = delete;
  pairwise_factor& operator=(const pairwise_factor& other) = delete;

#ifndef NDEBUG
  void set_debug_info(index idx0, index idx1)
  {
    index0_ = idx0;
    index1_ = idx1;
  }

  std::string dbg_info() const
  {
    std::ostringstream s;
    s << "uv(" << index0_ << "," << index1_ << ")";
    return s.str();
  }
#endif

  auto size() const { return std::tuple(no_labels0_, no_labels1_); }

  bool is_prepared() const
  {
    bool result = true;
    for (const auto& x : costs_)
        result = result && !std::isnan(x);
    return result;
  }

  void set(const index idx0, const index idx1, cost c)
  {
    assert_index(idx0, idx1);
    const index linear_idx = to_linear(idx0, idx1);
    costs_[linear_idx] = c;
  }

  cost get(const index idx0, const index idx1)
  {
    assert_index(idx0, idx1);
    const index linear_idx = to_linear(idx0, idx1);
    return costs_[linear_idx];
  }

  cost lower_bound() const
  {
    return *std::min_element(costs_.begin(), costs_.end());
  }

  void repam0(const index idx0, const cost msg)
  {
    for (index idx1 = 0; idx1 < no_labels1_; ++idx1) {
      assert_index(idx0, idx1);
      auto linear_idx = to_linear(idx0, idx1);
      costs_[linear_idx] += msg;
    }
  }

  void repam1(const index idx1, const cost msg)
  {
    for (index idx0 = 0; idx0 < no_labels0_; ++idx0) {
      assert_index(idx0, idx1);
      auto linear_idx = to_linear(idx0, idx1);
      costs_[linear_idx] += msg;
    }
  }

  template<bool left>
  void repam(const index idx, const cost msg)
  {
    if constexpr (left)
      repam0(idx, msg);
    else
      repam1(idx, msg);
  }

  void reset_primal() { primal_ = primal_unset; }

  cost evaluate_primal() const
  {
    if (primal_ != primal_unset)
      return costs_[primal_];
    else
      return std::numeric_limits<cost>::infinity();
  }

  auto& primal() { return primal_; }
  const auto& primal() const { return primal_; }

  void round_primal()
  {
    auto min = std::min_element(costs_.cbegin(), costs_.cend());
    primal_ = min - costs_.cbegin();
  }

protected:
  void assert_index(const index idx0, const index idx1) const
  {
    assert(idx0 >= 0 && idx0 < no_labels0_);
    assert(idx1 >= 0 && idx1 < no_labels1_);
    assert(idx0 * idx1 < costs_.size());
  }

  size_t to_linear(const index idx0, const index idx1) const
  {
    assert_index(idx0, idx1);
    size_t result = idx0 * no_labels1_ + idx1;
    assert(result >= 0 && result < costs_.size());
    return result;
  }

  fixed_vector_alloc_gen<cost, ALLOCATOR> costs_;
  index primal_;
  index no_labels0_, no_labels1_;

#ifndef NDEBUG
  index index0_, index1_;
#endif
};

}
}

#endif

/* vim: set ts=8 sts=2 sw=2 et ft=cpp: */