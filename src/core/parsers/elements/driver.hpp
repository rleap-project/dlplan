#ifndef SRC_CORE_PARSERS_ELEMENTS_DRIVER_HPP_
#define SRC_CORE_PARSERS_ELEMENTS_DRIVER_HPP_

#include <memory>

#include "include/dlplan/core.h"
#include "src/core/parsers/common/config.hpp"


namespace dlplan::core::parsers::elements {
class Driver {
private:
     dlplan::core::SyntacticElementFactory& element_factory;

public:
     Driver(
        dlplan::core::SyntacticElementFactory& element_factory);

     /// @brief Root call.
     std::shared_ptr<const Boolean> parse_boolean(
          const std::string& source,
          const std::string& filename);

     /// @brief Nested call.
     std::shared_ptr<const Boolean> parse_boolean(
          iterator_type& iter,
          iterator_type end,
          const std::string& filename);

     /// @brief Root call.
     std::shared_ptr<const Numerical> parse_numerical(
          const std::string& source,
          const std::string& filename);

     /// @brief Nested call.
     std::shared_ptr<const Numerical> parse_numerical(
          iterator_type& iter,
          iterator_type end,
          const std::string& filename);

     /// @brief Root call.
     std::shared_ptr<const Concept> parse_concept(
          const std::string& source,
          const std::string& filename);

     /// @brief Nested call.
     std::shared_ptr<const Concept> parse_concept(
          iterator_type& iter,
          iterator_type end,
          const std::string& filename);

     /// @brief Root call.
     std::shared_ptr<const Role> parse_role(
          const std::string& source,
          const std::string& filename);

     /// @brief Nested call.
     std::shared_ptr<const Role> parse_role(
          iterator_type& iter,
          iterator_type end,
          const std::string& filename);
};

}


#endif
