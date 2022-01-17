#include "writer.h"

#include <ostream>

#include "../include/dlplan/policy.h"


namespace dlplan::policy {

/*
(define
(sketch miconic)
)
(:rule 3 :conditions ((n_unk 38) (n_unk 39)) :effects ((n_unk 38) (n_inc 39)))
(:rule 6 :conditions ((n_unk 38) (n_unk 39)) :effects ((n_unk 39) (n_inc 38)))
)
*/

std::string PolicyWriterImpl::write(const Policy& policy) const {
    return policy.str();
}

}
