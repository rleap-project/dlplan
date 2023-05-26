

namespace dlplan::core {


template<typename T>
const T* DenotationsCaches::insert_denotation(T&&) {
    static_assert(std::is_same<T, ConceptDenotation>::value ||
                  std::is_same<T, RoleDenotation>::value ||
                  std::is_same<T, bool>::value ||
                  std::is_same<T, int>::value ||
                  std::is_same<T, ConceptDenotations>::value ||
                  std::is_same<T, RoleDenotations>::value ||
                  std::is_same<T, BooleanDenotations>::value  ||
                  std::is_same<T, NumericalDenotations>::value ,
                  "Unsupported type for insert_denotation method.");
    return nullptr;
}


template<typename T>
void DenotationsCaches::insert_denotations(int, const T*) {
    static_assert(std::is_same<T, ConceptDenotations>::value  ||
                    std::is_same<T, RoleDenotations>::value  ||
                    std::is_same<T, BooleanDenotations>::value  ||
                    std::is_same<T, NumericalDenotations>::value ,
                    "Unsupported type for insert_denotations method.");
}

template<typename T>
const T* DenotationsCaches::get_denotations(int) const {
    static_assert(std::is_same<T, ConceptDenotations>::value  ||
                    std::is_same<T, RoleDenotations>::value  ||
                    std::is_same<T, BooleanDenotations>::value  ||
                    std::is_same<T, NumericalDenotations>::value ,
                    "Unsupported type for get_denotations method.");
    return nullptr;
}


template<typename T>
void DenotationsCaches::insert_denotation(int, int, int, const T*) {
    static_assert(std::is_same<T, ConceptDenotation>::value  ||
                    std::is_same<T, RoleDenotation>::value  ||
                    std::is_same<T, bool>::value  ||
                    std::is_same<T, int>::value ,
                    "Unsupported type for insert_denotations method.");
}
template<typename T>
const T* DenotationsCaches::get_denotation(int, int, int) const {
    static_assert(std::is_same<T, ConceptDenotation>::value  ||
                    std::is_same<T, RoleDenotation>::value  ||
                    std::is_same<T, bool>::value  ||
                    std::is_same<T, int>::value ,
                    "Unsupported type for get_denotation method.");
    return nullptr;
}

}
