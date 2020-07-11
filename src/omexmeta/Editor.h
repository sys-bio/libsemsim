//
// Created by Ciaran on 4/15/2020.
//

#ifndef LIBOMEXMETA_EDITOR_H
#define LIBOMEXMETA_EDITOR_H


#include "omexmeta/SemsimXmlAssistant.h"
#include "omexmeta/Predicate.h"
#include "omexmeta/Resource.h"
#include "omexmeta/Triple.h"
#include "omexmeta/Participant.h"
#include "omexmeta/PhysicalPhenomenon.h"
#include "omexmeta/PhysicalEntity.h"
#include "omexmeta/PhysicalProcess.h"
#include "omexmeta/PhysicalForce.h"
#include "RedlandAPIWrapper.h"
#include "omexmeta/Error.h"
#include "omexmeta/OmexMetaUtils.h"
#include "omexmeta/PersonalInformation.h"

#include "librdf.h"

#include <utility>


using namespace redland;

/*
 * Here's the basic logic that we use in SemGen to determine which OPB physical property is represented by a <species> in an SBML model:
 *
 * Using libSBML, we first look up the substance base units for the model and also whether the species's "hasOnlySubstanceUnits" attribute is true...
 *
 * if the substance base unit is mole
 *      if hasOnlySubstanceUnits for species is TRUE, then use OPB:Chemical_molar_amount (OPB_00425)
 *      else use OPB:Chemical_concentration (OPB_00340)
 *
 * else if the substance base unit is item
 *      if hasOnlySubstanceUnits for species is TRUE, then use OPB:Particle_count (OPB_01001)
 *      else use OPB:Particle_concentration (OPB_01000)
 *
 * else if the base unit is kilogram or gram
 *      if hasOnlySubstanceUnits is TRUE, then use OPB:Mass_of_solid_entity (OPB_01226)
 *      else
 *           if the spatial dimensions of the compartment containing the species = 1, use OPB:Mass_lineal_density (OPB_00190)
 *           else, if the spatial dimensions of the compartment containing the species = 2, use OPB:Mass_areal_density (OPB_00258)
 *           else, if the spatial dimensions of the compartment containing the species = 3, use OPB:Mass_volumetric_density (OPB_00101)
 *
 * Hope that's helpful for determining which OPB property to use in composite annotations for SBML <species>.
 *
 * Any questions, just lemme know.
 */

namespace omexmeta {

    typedef std::unordered_map<std::string, std::string> NamespaceMap;

    /*
     * @brief Add or change annotations in xml.
     *
     */
    class Editor {
    private:
        std::string xml_;
        std::vector<std::string> metaids_;
        const LibrdfModel &model_;
        bool create_ids_ = false;
        std::unordered_map<std::string, std::string> &namespaces_;
        std::string repository_name_ = "http://myOmexLibrary.org/";
        std::string archive_name_ = "http://myOmexLibrary.org/MyOmex.omex";
        std::string model_name_ = "http://myOmexLibrary.org/MyOmex.omex/mymodel.xml";
        std::string local_name_ = "http://myOmexLibrary.org/MyOmex.omex/mymodel.rdf#";

    private:
        void extractNamespacesFromTriplesVector(PhysicalPhenomenon *pp);

    public:

        /*
         * @brief constructor for Editor.
         * @param xml The valid xml content for annotation
         * @param create_ids. Autogenerate metaids for xml element that do not already have them
         * @param SemsimXmlType indicated which type of xml is being annotated. SEMSIM_TYPE_SBML, SEMSIM_TYPE_CELLML or SEMSIM_TYPE_UNKNOWN.
         * @param model a reference to the current model (owned by RDF).
         * @param nm_map a set of namespaces for current xml
         *
         * The Editor is usually instantiated from the RDF::to_editor class which
         * automatically takes care of the @param model and @param ns_map arguments. When
         * instantiated, the editor class automatically reads the XML and adds metaids to
         * some or all of the elements, depending on the XML type parameter. If the user
         * specifies that they are annotating an SBML model, the elements returned by
         * SBMLAssistant::getValidElements() are given metaids (if not exist), while if
         * the user chooses cellml the elements returned by CellMLAssistant::getValidElements()
         * are used. If the type is unknown, then all elements are given metaids.
         */
        explicit Editor(const std::string &xml, SemsimXmlType type, bool create_ids,
                        const LibrdfModel &model, NamespaceMap &ns_map);

        /*
         * We no longer required to free the
         * triples_ used by Editor since they are
         * created and freed inplace - i.e.
         * locally, not at the class scope.
         */
        ~Editor() = default;

        [[nodiscard]] int size() const;


        /*
         * @brief returns a hashmap of namespaces to prefixes.
         * @return Map where keys are namespaces and values are prefixes
         */
        [[nodiscard]] const NamespaceMap &getNamespaces() const;

        /*
         * @brief return the underlying librdf_model* pointer
         * @return the librdf_model pointer
         */
        [[nodiscard]] librdf_model *getModel() const;

        /*
         * @brief set the namespace map.
         */
        void setNamespaces(const NamespaceMap &namespaces);

        /*
         * @brief return the xml
         * @return the xml string after metaids are added
         */
        [[nodiscard]] const std::string &getXml() const;

        /*
         * @brief returns a list of metaids both that existed previously and that was added
         * during instantiation.
         * @return vector of metaids used in the xml
         */
        [[nodiscard]] const std::vector<std::string> &getMetaids() const;

        /*
         * @brief add a namespace
         * @param ns the namespace
         * @param prefix the prefix used in serialized annotations to refer to the namespace
         */
        void addNamespace(const std::string &ns, std::string prefix);

        /*
         * @brief Add a SingleAnnotation (aka a Triple) to the model
         * @param subject the subject portion of the triple
         * @param pointer to the predicate the predicate portion of the triple. Ths is a pointer to support polymorphic calls.
         * @param resource the resource portion of the triple
         */
        void addSingleAnnotation(Subject subject, const PredicatePtr &predicate_ptr, const Resource &resource);

        /*
         * @brief Add a SingleAnnotation (aka Triple) to the rdf graph.
         * @param singularAnnotation An instance of SingularAnnotation to add to the model
         */
        void addSingleAnnotation(SingularAnnotation &singularAnnotation);

        /*
         * @brief remove a singular annotation (aka Triple) from the rdf graph
         * @param singularAnnotation An instance of SingularAnnotation to remove from the model
         */
        void removeSingleAnnotation(const SingularAnnotation &singularAnnotation) const;

        /*
         * @brief add a composite annotation to the rdf graph.
         * @param phenomenonPtr A pointer to an object of type PhysicalPhenomenon, the superclass of
         * the composite annotations.
         *
         * Composite annotations currently supported are PhysicalEntity,
         * PhysicalProcess and PhysicalForce. The PhysicalDependency
         * type will be supported in future releases.
         *
         * For developers. Consider removing this function in favour of using the
         * add* functions.
         */
        void addCompositeAnnotation(PhysicalPhenomenon *phenomenonPtr);

        /*
         * @brief add a composite annotation of type PhysicalEntity to the rdf graph
         * @param physicalEntity An instance of a PhysicalEntity object to add to the rdf graph.
         */
        void addPhysicalEntity(PhysicalEntity &physicalEntity);

        /*
         * @brief remove triples associated with a a PhysicalEntity object from the rdf graph
         * @param physicalEntity the PhysicalEntityto remove
         */
        void removePhysicalEntity(PhysicalEntity &physicalEntity) const;

        /*
         * @brief add a composite annotation of type PhysicalProcess to the rdf graph
         * @param physicalProcess An instance of a PhysicalProcess object to add to the rdf graph.
         */
        void addPhysicalProcess(PhysicalProcess &physicalProcess);

        /*
         * @brief remove triples associated with a PhysicalProcess object from the rdf graph
         * @param physicalProcess the PhysicalProcessto remove
         */
        void removePhysicalProcess(PhysicalProcess &physicalProcess) const;

        /*
         * @brief add a composite annotation of type PhysicalForce to the rdf graph
         * @param physicalForce An instance of a PhysicalForce objec to add to the rdf graph.
         */
        void addPhysicalForce(PhysicalForce &physicalForce);

/*
         * @brief remove triples associated with a a PhysicalForce object from the rdf graph
         * @param physicalForce the PhysicalForce to remove
         */
        void removePhysicalForce(PhysicalForce &physicalForce) const;


        /*
         * @brief check that a metaid is valid by comparing
         * with the output from Editor::getMetaIds()
         */
        void checkValidMetaid(const std::string &metaid);

        /*
         * @brief extract namespace part of uri from @parameter predicate_string
         * and add it to namespace_ if we know it.
         */
        void addNamespaceFromAnnotation(const std::string &predicate_string);

        PhysicalEntity createPhysicalEntity();

        PhysicalForce createPhysicalForce();

        PhysicalProcess createPhysicalProcess();

        void addSingleAnnotationNoValidation(SingularAnnotation &singularAnnotation);

        void addCompositeAnnotation2(PhysicalPhenomenon *phenomenonPtr);

        void removePhysicalEntity2(PhysicalEntity &physicalEntity) const;

        void addTriples(Triples &triples);

        void removePhysicalPhenomenon(PhysicalPhenomenon *physicalPhenomenon) const;

        void addAuthor(const std::string &orcid_id);

        void addCurator(const std::string &orcid_id);

        /*
         * @brief Set the url for repository (myOmexlib).
         * @param repository_name the name. Default is "https://myOmexLibrary.org/".
         *
         * This is the namespace attached to the `myOMEXlib`
         * prefix.
         */
        void setOmexRepository(std::string repository_name);

        /*
         * @brief setter for the archive_name_ attribute.
         *
         * If @param archive_name already begins with "http",
         * "https://: or "file://", the archive_name is assumed
         * properly formatted and used without modification. If
         * @param archive_name does not begin with one of these
         * three prefixes, the current repository_name_ (which
         * defaults to `https://myOmexLibrary.org/`) is used as the namespace. For example
         * setting archive_name_ to "myOmex" becomes "https://myOmexLibrary.org/myOmex.omex".
         */
        void setArchiveName(std::string archive_name);

        /*
         * @brief get the current value of archive_name_
         */
        [[nodiscard]] std::string getArchiveName() const;

        /*
         * @brief Set the models name (myOMEX)
         * @param The value of model_name. This is
         * by definition relative to the repository_name attribute.
         */
        void setModelName(std::string model_name);

        /*
         * @brief get the current value of local_name_
         */
        std::string getLocalName() const;

        /*
         * @brief get the current value of model_name_
         */
        [[nodiscard]] std::string getModelName() const;


        /*
         * @brief get the current value of archive_name_
         */
        [[nodiscard]] std::string getOmexRepository() const;

        /*
         * @brief set the name local to the current rdf document.
         * @param local_name the string to use for local
         *
         * The local_name string should contain the omex
         * repository and the model name. For example:
         * "http://myOmexLibrary.org/myOmex.omex/myModel.rdf".
         * Note that the corresponsing model should be:
         * "http://myOmexLibrary.org/myOmex.omex/myModel.sbml" or
         * "http://myOmexLibrary.org/myOmex.omex/myModel.xml" or
         * "http://myOmexLibrary.org/myOmex.omex/myModel.cellml".
         */
        void setLocalName(std::string local_name);

        LibrdfNode createNodeWithLocalUri(const std::string& string) const;

        void addModelLevelAnnotation(std::string, const LibrdfNode *node);

        void taxon(const std::string &taxon_id);

        void pubmed(const std::string &pubmedid);

        void addDescription(const std::string &date);

        void addDateCreated(const std::string &date);

        void addPersonalInformation(const PersonalInformation &personalInformation) const;

        SingularAnnotation newSingularAnnotation(std::string metaid);
    };

}

#endif //LIBOMEXMETA_EDITOR_H
