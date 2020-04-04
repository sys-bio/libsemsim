# ifndef SEMSIM_SBML_MODEL_H_
# define SEMSIM_SBML_MODEL_H_

# include "sbml/SBMLTypes.h"
# include <raptor2.h>

# include "SemsimModel.h"
# include "Process.h"
# include "BiomodelsQualifiers.h"
# include "SemSimQualifiers.h"
# include "MetaID.h"
# include "Annotation.h"
# include <unordered_map>


namespace semsim {

    /**
     * This class represents an SBML model imported into SemSim.
     */
    class  SemSimSBMLModel : public SemsimModel {
    public:
        /**
         * Construct from an SBML model.
         * Creates libSemSim @ref Component "Components"
         * for all species and compartments in the model
         * and a corresponding mapping table.
         * @param m The SBML model to use for initialization.
         */
        explicit SemSimSBMLModel(libsbml::SBMLDocument *d)
                : SemsimModel(), d_(d) {
            libsbml::Model *m = d->getModel();
            // all elements must have meta ids
            MetaID metaId(*m);
            metaId.assignMetaIds();
            for (unsigned int k = 0; k < m->getNumCompartments(); ++k) {
                libsbml::Compartment *c = m->getCompartment(k);
                if (c->isSetMetaId()) {
                    Component *o = addComponent(Component());
                    element_map_.insert(std::make_pair(c, o));
                    o->setMetaId(c->getMetaId());
                    if (c->isSetIdAttribute())
                        element_id_map_.insert(std::make_pair(c->getId(), o));
                }
            }
            for (unsigned int k = 0; k < m->getNumSpecies(); ++k) {
                libsbml::Species *s = m->getSpecies(k);
                if (s->isSetMetaId()) {
                    Component *o = addComponent(Component());
                    element_map_.insert(std::make_pair(s, o));
                    o->setMetaId(s->getMetaId());
                    if (s->isSetIdAttribute())
                        element_id_map_.insert(std::make_pair(s->getId(), o));
                }
            }
            for (unsigned int k = 0; k < m->getNumReactions(); ++k) {
                libsbml::Reaction *r = m->getReaction(k);
                if (r->isSetMetaId()) {
                    Component *o = addComponent(Process());
                    element_map_.insert(std::make_pair(r, o));
                    o->setMetaId(r->getMetaId());
                    if (r->isSetIdAttribute())
                        element_id_map_.insert(std::make_pair(r->getId(), o));
                }
            }
            for (unsigned int k = 0; k < m->getNumParameters(); ++k) {
                libsbml::Parameter *p = m->getParameter(k);
                if (p->isSetMetaId()) {
                    Component *o = addComponent(Component());
                    element_map_.insert(std::make_pair(p, o));
                    o->setMetaId(p->getMetaId());
                    if (p->isSetIdAttribute())
                        element_id_map_.insert(std::make_pair(p->getId(), o));
                }
            }
        }

        /// Move constructor
        SemSimSBMLModel(SemSimSBMLModel &&other) noexcept :
                SemsimModel(std::move(other)),
                element_map_(std::move(other.element_map_)),
                element_id_map_(std::move(other.element_id_map_)),
                d_(other.d_) {
        }

        /// copy assignment operator
        SemSimSBMLModel &operator=( SemSimSBMLModel &other) {
            if (this != &other) {
                this->d_ = other.d_;
                this->element_id_map_ = other.element_id_map_;
                this->element_map_ = other.element_map_;
            }
            return *this;
        }

        /**
         * Set the annotation of a @ref Component based on SBML id.
         * Do nothing if the id is not mapped.
         * @param id         The SBML element.
         * @param annotation The annotation for the component.
         */
        void setComponentAnnotation(libsbml::SBase *s,  AnnotationBase &annotation) {
            if (hasComponent(s))
                element_map_.find(s)->second->setAnnotation(annotation);
        }

        /**
         * Set the annotation of a @ref Component based on SBML id.
         * Do nothing if the id is not mapped.
         * @param s         The SBML element.
         * @param annotation The annotation for the component.
         */

        void setComponentAnnotation(libsbml::SBase *s,  AnnotationPtr &annotation) {
            if (hasComponent(s))
                element_map_.find(s)->second->setAnnotation(*annotation);
        }

# if __cplusplus >= 201103L

        /**
         * Set the annotation of a @ref Component based on SBML id.
         * Do nothing if the id is not mapped.
         * @param id         The SBML element.
         * @param annotation The annotation for the component.
         */

        void setComponentAnnotation(libsbml::SBase *s, AnnotationPtr &&annotation) {
            if (hasComponent(s))
                element_map_.find(s)->second->setAnnotation(std::move(annotation));
        }

# endif

        /**
         * Check whether a @ref Component exists for the given SBML id.
         * (species and compartments should return true).
         * @param  id The id of an SBML element.
         * @return    Whether the SBML element has been mapped to a libSemSim @ref Component or not.
         */
        bool hasComponent( std::string id)  {
            return !(element_id_map_.find(id) == element_id_map_.end());
        }

        /**
         * Check whether a @ref Component exists for the given SBML element.
         * (species and compartments should return true).
         * @param  s The SBML element.
         * @return    Whether the SBML element has been mapped to a libSemSim @ref Component or not.
         */
        bool hasComponent(libsbml::SBase *s)  {
            return !(element_map_.find(s) == element_map_.end());
        }

        /**
         * Check whether a @ref Process exists for the given SBML element.
         * (species and compartments should return true).
         * @param  s The SBML element.
         * @return    Whether the SBML element has been mapped to a libSemSim @ref Process or not.
         */

        bool hasProcess(libsbml::SBase *s)  {
            return !(element_map_.find(s) == element_map_.end()) && element_map_.find(s)->second->isProcess();
        }

        /**
         * Return the component corresponding to the given SBML element id (if it exists in the mapping table).
         * Compartments and species should exist in the mapping table.
         * @param  s The SBML element.
         * @return    The component for the given SBML id (if it exists).
         */

         Component *getComponent(libsbml::SBase *s)  {
            if (!hasComponent(s))
                throw std::out_of_range("Component does not exist in mapping table");
            return element_map_.find(s)->second;
        }

        /**
         * Return the component corresponding to the given SBML element (if it exists in the mapping table).
         * Compartments and species should exist in the mapping table.
         * @param  s The SBML element.
         * @return    The component for the given SBML id (if it exists).
         */

        Component *getComponent(libsbml::SBase *s) {
            if (!hasComponent(s))
                throw std::out_of_range("Component does not exist in mapping table");
            return element_map_.find(s)->second;
        }

        /**
         * Return the physical process corresponding to the given SBML element (if it exists in the mapping table).
         * Compartments and species should exist in the mapping table.
         * @param  s The SBML element.
         * @return    The physical process for the given SBML id (if it exists).
         */

        Process *getProcess(libsbml::SBase *s) {
            if (!hasProcess(s))
                throw std::out_of_range("Component does not exist in mapping table");
            return dynamic_cast<Process *>(element_map_.find(s)->second);
        }

        /**
         * Return the component corresponding to the given SBML element id (if it exists in the mapping table).
         * Compartments and species should exist in the mapping table.
         * @param  id The SBML id.
         * @return    The component for the given SBML id (if it exists).
         */
         Component *getComponentForId( std::string &id)  {
            if (!hasComponent(id))
                throw std::out_of_range("Component does not exist in mapping table");
            return element_id_map_.find(id)->second;
        }

        /**
         * Return the component corresponding to the given SBML element id (if it exists in the mapping table).
         * Compartments and species should exist in the mapping table.
         * @param  id The SBML id.
         * @return    The component for the given SBML id (if it exists).
         */
        Component *getComponentForId( std::string &id) {
            if (!hasComponent(id))
                throw std::out_of_range("Component does not exist in mapping table");
            return element_id_map_.find(id)->second;
        }

        /**
         * Get the RDF serialization of this model.
         * The serialized RDF will always need to refer back
         * to the original SBML or CellML model using a Url.
         * Usually, the RDF will be written to a COMBINE archive
         * along with the model, in which case the @p sbml_base_uri
         * argument should be the relative path to the model file
         * in the COMBINE archive.
         * @param  format        The RDF serialization format. Choices include "rdfxml", "ntriples", "turtle", "trig", "rss-tag-soup", "grddl", "rdfa", "json", and "nquads".
         * @param  sbml_base_uri A Url that points to the original model file. Usually a relative path in a COMBINE archive.
         * @return               A string representation of the RDF for model using the desired RDF serialization format.
         */
        std::string getRDF(Url &sbml_base_uri,  std::string &format = "rdfxml")  {
            raptor_world *world = raptor_new_world();
            raptor_serializer *serializer = raptor_new_serializer(world, format.c_str());
            if (!serializer)
                throw std::runtime_error("Could not create Raptor serializer for format " + format);

            raptor_uri *base_uri = raptor_new_uri(world, ( unsigned char *) "");

            raptor_serializer_set_namespace(serializer,
                                            raptor_new_uri(world, ( unsigned char *) bqb::root.c_str()),
                                            ( unsigned char *) "bqb");
            raptor_serializer_set_namespace(serializer,
                                            raptor_new_uri(world, ( unsigned char *) semsim::root.c_str()),
                                            ( unsigned char *) "semsim");

            void *output;
            size_t length;
            raptor_serializer_start_to_string(serializer, base_uri, &output, &length);

            for (Components::const_iterator i = components_.begin(); i != components_.end(); ++i)
                (*i)->serializeToRDF(sbml_base_uri, world, serializer);

            raptor_serializer_serialize_end(serializer);

            raptor_free_serializer(serializer);
            raptor_free_world(world);

            std::string result((char *) output);
            free(output);
            return result;
        }

# ifdef SWIG
        /// For swig
        std::string getRDF( std::string& sbml_base_uri,  std::string& format="rdfxml")  {
          return getRDF(sbml_base_uri, format);
        }
# endif

        /**
         * Return the XML encoding of the attached SBML or CellML model.
         * @return The XML content.
         */
        std::string encodeXML()  {

            libsbml::SBMLWriter w;
            return w.writeSBMLToString(d_);
        }

        /**
         * @return "sbml" if an SBML model, "cellml" if a cellml model.
         */
        virtual std::string getFormat()  {
            return "sbml";
        }

    protected:
        /// Maps SBML model elements to corresponding libSemSim @ref Component.

        std::unordered_map<libsbml::SBase *, Component *> element_map_;
        /// Maps SBML model elements to corresponding libSemSim @ref Component.
        std:: unordered_map<std::string, Component *> element_id_map_;
        /// Stores the SBML model

        libsbml::SBMLDocument *d_;
    };

}

# endif