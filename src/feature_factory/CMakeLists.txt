add_library(feature_factory
    feature_factory.cpp
    feature.cpp
    numerical_features/numerical_feature.cpp
    numerical_features/count_feature.cpp
)
target_link_libraries(feature_factory element_factory)
