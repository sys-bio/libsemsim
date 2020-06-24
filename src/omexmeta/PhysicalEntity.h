//
// Created by Ciaran on 4/22/2020.
//

#ifndef LIBOMEXMETA_PHYSICALENTITY_H
#define LIBOMEXMETA_PHYSICALENTITY_H

#include "omexmeta/PhysicalProperty.h"
#include "omexmeta/Resource.h"
#include "omexmeta/Subject.h"
#include "omexmeta/PhysicalPhenomenon.h"
#include "omexmeta/MetaID.h"
#include "RedlandAPIWrapper.h"
#include <utility>
#include "omexmeta/Triple.h"
#include "omexmeta/Query.h"
#include "omexmeta/SemsimUtils.h"
#include "memory"

using namespace redland;

namespace semsim {
    /*
     * The PhysicalEntity type of PhysicalPhenomenon
     */
    class PhysicalEntity : public PhysicalPhenomenon {
    private:
        Resource identity_resource_;
        Resources location_resources_;

    public:

        /*
         * @breif default constructor for PhysicalEntity
         * @details deliberately deleted. If you try using the
         * builder interface (chaining setter methods) from a default
         * instantiated PhysicalEntity you will get an error, because there
         * will be no model assicated with PhysicalEntity. Instead, always
         * instantiate a PhysicalEntity from the Editor::addPhysicalEntity() method.
         */
        PhysicalEntity() = delete;

        /*
         * @brief constructor for instantiating a PhysicalEntity object
         * @param model the model being used by the current rdf graph.
         * @param about The subject portion of the PhysicalEntity representing the metaid for the PhysicalEntity
         * @param physicalProperty A term from the ontology of physical for biology of type PhysicalProperty.
         * @param is The "what" portion of a PhysicalEntity object.
         * @param is_part_of The "where" portion of the PhysicalEntity object.
         *
         *
         * Users should not need to use this constructor directly as it is embedded in the
         * builder interface. The @param is_part_of parameter is actually a std::vector of
         * Resource objects. It can be as long as needed.
         */
        PhysicalEntity(librdf_model *model,
                       PhysicalProperty physicalProperty, Resource is,
                       Resources is_part_of);

        ~PhysicalEntity() = default;

                /*
         * @brief free resources uses by PhysicalEntity
         *
         * PhysicalEntity objects are owned either by the caller or by a Triples
         * object, depending on whether you have "given" the PhysicalEntity to
         * a Triples object by calling "str" or "toTriples".
         */
        void free();

        /*
         * @brief constructor for PhysicalEntity object.
         * @param model the librdf_model object in use. Passed down from Editor class during
         * instantiation.
         *
         * This alternative constructor is used when users use the builder interface
         * (which should actually be most of the time). The PhysicalEntity is instantiated
         * with only the model to allow for a better way of collecting necessary information
         * from the user.
         */
        explicit PhysicalEntity(librdf_model *model);

        /*
         * @brief convert PhysicalEntity to a Triples object, which can then be
         * passed to a model via the Editor::addPhysicalEntity method.
         * @return a Triples objects containing the Triple objects associated with this PhysicalEntity
         *
         * When using "toTriples" you are giving ownership of the nodes used by
         * PhysicalEntity to the returned Triples object, which automatically cleans up
         * after itself in its destructor. If you instantiate a PhysicalEntity
         * and do not call toTriples (which will not be often), then the caller
         * is responsible for calling PhysicalEntity::free when finished.
         */
        [[nodiscard]] Triples toTriples() override;

        /*
         * @brief return the IdentityResource in use. I.e. the "what"
         * portion of the PhysicalEntity
         * @return the identity Resource
         */
        [[nodiscard]] const Resource &getIdentityResource() const;

        /*
         * @brief return a vector of resources representing the
         * "where" part of the PhysicalEntity
         * @return vector of Resource objects representing anatomical location of physical entity
         */
        [[nodiscard]] const Resources &getLocationResources() const;

        /*
         * @brief sets the about portion of the PhysicalEntity.
         * @param metaid the metaid for the PhysicalEntity.
         * @return a reference to this Physical entity. Allows chaining together builder commands.
         *
         * A Subject with a URI node is automatically created with the metaid,
         * which is the id of the model element representing the PhysicalEntity. The
         * metaid is relative to the base_uri of the rdf document.
         *
         * This method is a part of the PhysicalEntity builder interface
         *
         */
        PhysicalEntity &setAbout(const std::string& metaid);

        /*
         * @brief sets the physical property for a PhysicalEntity instance.
         * @param physicalProperty a string representing the OPB term used by the PhysicalEntity
         * @return a reference to this Physical entity. Allows chaining together builder commands.
         *
         * The OBP argument requires a string of the form "obp:opbxxx" where "xxx" is the id for the OPB term.
         * An instance of PhysicalProperty is instantiated with @param physicalProperty as its value.
         */
        PhysicalEntity &setPhysicalProperty(const std::string &subject_metaid, const std::string &physicalProperty);

        /*
         * @brief sets the physical property for a PhysicalEntity instance
         * @return a reference to this Physical entity. Allows chaining together builder commands.
         *
         * Prefer the alternative setPhysicalProperty instance, since you do not need to instantiate
         * the PhysicalProperty yourself.
         *
         * For developers. Consider removing.
         */
        [[maybe_unused]] PhysicalEntity &setPhysicalProperty(PhysicalProperty physicalProperty);

        /*
         * @brief sets the identity portion of the PhysicalEntity (the "What").
         * @param resource The resource to be used for the identity.
         * @return a reference to this Physical entity. Allows chaining together builder commands.
         *
         * The input string gets converted to a Resource automatically.
         *
         * See Resource documentation for more details of valid input strings.
         */
        PhysicalEntity &setIdentity(const std::string& resource);

        /*
         * @brief add a location to the PhysicalEntity.
         * @param where The resource representing a location.
         * @return a reference to this Physical entity. Allows chaining together builder commands.
         *
         * The input string gets converted to a Resource automatically. An arbitrary
         * number of locations are allowed. The location is added to the back of a vector
         * containing the Resources. Left most elements of this vector represent larger
         * physiological locations which get smaller as the index of this vector increases
         * via use of the "isPartOf" predicate. For example, a cytosolic TGFb molecule
         * in a dermal fibroblast could have a locations vector first containing
         * a reference to skin, then dermis, then fibroblast, then cytosol. This ends up
         * being cytosol<isPartOf>fibroblast<isPartOf>dermis<isPartOf>skin.
         *
         * See Resource documentation for more details of valid input strings.
         */
        PhysicalEntity &addLocation(const std::string& where);

        /*
         * @brief returns the number of locations used by PhysicalEntity
         * @return the number of locations in a PhysicalEntity
         */
        [[nodiscard]] int getNumLocations() const;

        bool operator==(const PhysicalEntity &rhs) const;

        bool operator!=(const PhysicalEntity &rhs) const;


    };
}

#endif //LIBOMEXMETA_PHYSICALENTITY_H