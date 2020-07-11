//
// Created by Ciaran on 4/22/2020.
//

#include "PhysicalPhenomenon.h"

#include <utility>


namespace omexmeta {

    PhysicalPhenomenon::PhysicalPhenomenon(librdf_model *model, std::string local_uri,
                                           PhysicalProperty propertyResource, AnnotationType type)
            : model_(model), physical_property_(std::move(propertyResource)), type_(type),
            local_uri_(local_uri){}

    PhysicalPhenomenon::~PhysicalPhenomenon() = default;


    PhysicalPhenomenon::PhysicalPhenomenon(librdf_model *model)
            : model_(model) {}

    PhysicalPhenomenon::PhysicalPhenomenon(librdf_model *model, std::string local_uri)
        : model_(model), local_uri_(local_uri) {}

    const std::string & PhysicalPhenomenon::getSubjectStr() const {
        return physical_property_.getSubjectStr();
    }

    AnnotationType PhysicalPhenomenon::getType() const {
        return type_;
    }

    std::string PhysicalPhenomenon::generateMetaId(const std::string& id_base) const {
        return OmexMetaUtils::generateUniqueMetaid(model_, id_base);
    }

    PhysicalProperty PhysicalPhenomenon::getPhysicalProperty() const {
        return physical_property_;
    }

    Triples PhysicalPhenomenon::toTriples() {
        throw NotImplementedException("Base class PhysicalPhenomenon "
                                      "does cannot produce triples.");
    }


    const std::string & PhysicalPhenomenon::getAbout() const {
        return physical_property_.getSubjectStr();
    }

    PhysicalPhenomenon& PhysicalPhenomenon::setAbout(std::string about) {
        physical_property_.setSubject(about);
        return *this;
    }


    PhysicalPhenomenon::PhysicalPhenomenon(PhysicalPhenomenon &&phenomenon) noexcept {
        model_ = phenomenon.model_;
        phenomenon.model_ = nullptr; // not sure if this is right.
        physical_property_ = std::move(phenomenon.physical_property_);
        type_ = phenomenon.type_;
        local_uri_ = phenomenon.local_uri_;
    }

    PhysicalPhenomenon &PhysicalPhenomenon::operator=(PhysicalPhenomenon &&phenomenon) noexcept {
        if (this != &phenomenon) {
            model_ = phenomenon.model_;
            phenomenon.model_ = nullptr; // not sure if this is right.
            physical_property_ = std::move(phenomenon.physical_property_);
            type_ = phenomenon.type_;
        local_uri_ = phenomenon.local_uri_;
        }
        return *this;
    }

    bool PhysicalPhenomenon::operator==(const PhysicalPhenomenon &rhs) const {
        return physical_property_ == rhs.physical_property_;
    }

    bool PhysicalPhenomenon::operator!=(const PhysicalPhenomenon &rhs) const {
        return !(rhs == *this);
    }

    void PhysicalPhenomenon::setPhysicalProperty(const PhysicalProperty &physicalProperty) {
        physical_property_ = physicalProperty;
    }

    void PhysicalPhenomenon::setType(AnnotationType type) {
        type_ = type;
    }

    const std::string &PhysicalPhenomenon::getPhysicalPropertyId() const {
        return physical_property_id_;
    }

    void PhysicalPhenomenon::setPhysicalPropertyId(const std::string &physicalPropertyId) {
        physical_property_id_ = physicalPropertyId;
    }

    const std::string &PhysicalPhenomenon::getLocalUri() const {
        if (local_uri_.empty()){
            throw std::invalid_argument("std::invalid_argument: local_uri_ is empty. "
                                        "Please use setLocalUri or pass to the constructor a "
                                        "local uri. ");
        }
        return local_uri_;
    }

    void PhysicalPhenomenon::setLocalUri(const std::string &localUri) {
        local_uri_ = localUri;
    }


}




/*
 * todo
 *  How would we parse rdf back into a composite annotation
 *  structure? It might be possible if composite annotations
 *  themselves had an flag attribute which tells software which type of
 *  annotation were dealing with. This would simplify the ascertaining of all
 *  (say) physical process triples and enable us think about
 *  reassembling them again
 */






