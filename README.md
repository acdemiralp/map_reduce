#### Getting Started
- Copy map_reduce.hpp to your project (which should have a TBB dependency).

#### Example Usage 
- Hello World:

    ```cpp
    // Based on https://stackoverflow.com/questions/12375761/good-mapreduce-examples
    auto results = map_reduce<std::size_t, std::string, std::size_t>(
      {"what", "savannah", "the", "where", "research", "you", "then", "and", "when", "steve"},
      [ ] (const std::string& datum)
      {
        return datum.size();
      },
      [ ] (const std::size_t& key, const std::vector<const std::string&>& values)
      {
        return values.size();
      });
    ```