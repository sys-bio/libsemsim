# ifndef SEMSIM_RELATION_H_
# define SEMSIM_RELATION_H_


# include "semsim/URI.h"

namespace semsim {

    /**
     * A relation is the predicate part of an RDF triple.
     * SemSim uses the
     * <a href="http://co.mbine.org/standards/qualifiers">BioModels biology qualifiers</a>
     * for describing model elements.
     */
    class  Relation {
    public:
        /**
         * Construct from URI.
         * @param uri The URI of the ontology term
         */
        Relation(const URI &uri)
                : uri_(uri) {}

        Relation(URI &&uri) : uri_(std::move(uri)) {}

        std::string toString() const {
            return uri_.toString();
        }

        /**
         * @return the URI for this relation.
         */
        const URI &getURI() const {
            return uri_;
        }

        /**
         * Return a human--readable representation of the annotation
         * information. Ontology terms will be replaced with human-readable
         * names.
         */
        std::string humanize() const;

        /// Comparison operator
        bool operator==(const Relation &other) const {
            return uri_ == other.uri_;
        }

    protected:
        URI uri_;
    };
}
# endif
