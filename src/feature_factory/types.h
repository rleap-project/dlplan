#include <memory>
#include <vector>

namespace dlp {
class Element;
class ConceptElement;
class RoleElement;
class Feature;
class NumericalFeature;
class BooleanFeature;

using Element_Ptr = std::shared_ptr<Element>;
using ConceptElement_Ptr = std::shared_ptr<ConceptElement>;
using RoleElement_Ptr = std::shared_ptr<RoleElement>;

using Feature_Ptr = std::shared_ptr<Feature>;
using NumericalFeature_Ptr = std::shared_ptr<NumericalFeature>;
using BooleanFeature_Ptr = std::shared_ptr<BooleanFeature>;

using Name_Vec = const std::vector<const std::string>;
using Index_Vec = const std::vector<unsigned>;

}
