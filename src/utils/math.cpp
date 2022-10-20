#include "math.h"


namespace dlplan::utils {

int binomial_coefficient(int n, int k) {
    int result = n - k + 1;
    for (int i = 1; i < k; ++i) {
        result = result * (n - k + 1 + i) / (i + 1);
    }
    return result;
}

}

