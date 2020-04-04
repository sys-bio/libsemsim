# ifndef SEMSIM_QUERY_CONDITION_H_
# define SEMSIM_QUERY_CONDITION_H_

#include "semsim/SemSim.h"

namespace semsim {

    /**
     * The base class for all conditions in queries.
     */
    class  Condition {
    public:
        /// Virtual dtor
        virtual ~Condition() = default;

        /// @return @c true if the condition matches the given element.
        virtual bool matches( Component &component) = 0;
    };

    /**
     * An owning pointer for a @ref Condition.
     * Automatically uses std::unique_ptr or std::tr1::shared_ptr depending on C++ standard.
     * @see UniquePtr.
     */
    typedef UniquePtr<Condition>::type ConditionPtr;

}

# endif