#include "catch.hpp"

#include <cstddef>
#include <string>
#include <vector>

#include <map_reduce/map_reduce.hpp>

TEST_CASE("Map Reduce Test", "[map_reduce]")
{
  // Taken from https://stackoverflow.com/questions/12375761/good-mapreduce-examples
  auto results = map_reduce<std::size_t, std::string, std::size_t>(
    {"what", "savannah", "the", "where", "research", "you", "then", "and", "when", "steve"},
    [ ] (const std::string& datum)
    {
      return datum.size();
    },
    [ ] (const std::size_t& key, const std::vector<const std::string*>& values)
    {
      return values.size();
    });

  REQUIRE(results[0].first  == 3);
  REQUIRE(results[0].second == 3);
  REQUIRE(results[1].first  == 4);
  REQUIRE(results[1].second == 3);
  REQUIRE(results[2].first  == 5);
  REQUIRE(results[2].second == 2);
  REQUIRE(results[3].first  == 8);
  REQUIRE(results[3].second == 2);
}