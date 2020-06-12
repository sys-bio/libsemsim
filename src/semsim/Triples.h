//
// Created by Ciaran on 4/29/2020.
//

#ifndef LIBSEMSIM_TRIPLES_H
#define LIBSEMSIM_TRIPLES_H

#include "semsim/Triple.h"
#include "semsim/Predicate.h"
#include "semsim/Subject.h"
#include "semsim/Resource.h"
#include "RedlandAPIWrapper.h"
#include "semsim/Error.h"
//#include "semsim/TripleWriter.h"

#include <vector>

using namespace redland;

namespace semsim {

//    class Triple;

    typedef std::vector<std::shared_ptr<Triple>> SharedTripleVector;
    typedef std::vector<Triple> TripleVector;

    class Triples {
    private:

        TripleVector triples_;

    public:
        Triples();

        /*
         * @brief construct a Triples object from a single triple
         * @param triple The triple to put into triples
         * @details The triple is moved into element 0 of a new
         * Triples object. The Triple must be passed by reference
         * so that the triple is moved directly, instead of copied
         * and then moved into the Triples object (which is a memory leak).
         */
        explicit Triples(Triple &triple);

        explicit Triples(std::vector<Triple> triples);

        const Triple &operator[](int index) const;

        void move_back(Triple &triple);

        void emplace_back(Subject subject, const PredicatePtr &predicatePtr, const Resource &resource);

        void emplace_back(Subject subject, const Predicate &predicate, const Resource &resource);

        void emplace_back(Subject subject, BiomodelsBiologyQualifier predicate, const Resource &resource);

        void emplace_back(Subject subject, BiomodelsModelQualifier predicate, const Resource &resource);

        void emplace_back(Subject subject, DCTerm predicate, const Resource &resource);

        void emplace_back(Subject subject, SemSim predicate, const Resource &resource);

        void emplace_back(librdf_node *subject, librdf_node *predicate, librdf_node *resource);

        std::vector<std::string> getSubjectsStr();

        std::vector<std::string> getPredicates();

        std::vector<std::string> getResources();

        int size() const;

        TripleVector::iterator begin();

        TripleVector::iterator end();

        std::string str(const std::string &format = "rdfxml-abbrev", std::string base = "file://./annotations.rdf");

        void freeTriples();

    };

    typedef std::vector<Triples> NestedTriples;

    //todo implement equality operators
}

#endif //LIBSEMSIM_TRIPLES_H
