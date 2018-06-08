#ifndef MAP_REDUCE_HPP_
#define MAP_REDUCE_HPP_

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

#include <tbb/concurrent_hash_map.h>
#include <tbb/parallel_for.h>

template<typename key_type, typename value_type, typename reduction_type>
std::vector<std::pair<key_type, reduction_type>> map_reduce(
  const std::vector<value_type>&                                                               data           ,
  const std::function<key_type(const value_type&)>&                                            map_function   , 
  const std::function<reduction_type(const key_type&, const std::vector<const value_type*>&)>& reduce_function)
{
  // Map.
  std::vector<key_type> keys(data.size());
  tbb::parallel_for(std::size_t(0), keys.size(), std::size_t(1), [&] (std::size_t index)
  {
    keys[index] = map_function(data[index]);
  });

  // Shuffle.
  using group_map_type          = tbb::concurrent_hash_map<key_type, std::vector<const value_type*>>;
  using group_map_accessor_type = typename group_map_type::accessor;
  group_map_type group_map;
  tbb::parallel_for(std::size_t(0), keys.size(), std::size_t(1), [&] (std::size_t index)
  {
    group_map_accessor_type accessor;
    group_map.count(keys[index]) ? group_map.find(accessor, keys[index]) : group_map.insert(accessor, keys[index]);
    accessor->second.push_back(&data[index]);
  });
  std::vector<std::pair<key_type, std::vector<const value_type*>>> groups(group_map.begin(), group_map.end());

  // Reduce.
  std::vector<std::pair<key_type, reduction_type>> reductions(groups.size());
  tbb::parallel_for(std::size_t(0), reductions.size(), std::size_t(1), [&] (std::size_t index)
  {
    reductions[index] = std::pair<key_type, reduction_type>{groups[index].first, reduce_function(groups[index].first, groups[index].second)};
  });
  
  return reductions;
}

#endif