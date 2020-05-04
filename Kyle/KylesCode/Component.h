# ifndef SEMSIM_COMPONENT_H_
# define SEMSIM_COMPONENT_H_


# include "semsim/AnnotationBase.h"
# include "semsim/SingularAnnotation.h"
# include "semsim/CompositeAnnotation.h"
#include "url.h"

# include <string>

namespace semsim {

    /**
     * A @ref Component is any element of an SBML or CellML model.
     * It can represent an SBML species, reaction, or compartment.
     */
    class Component {
    public:
        /// Empty constructor
        Component() = default;

        /// Construct from a singular annotation
        explicit Component(SingularAnnotation &annotation)
                : annotation_(new SingularAnnotation(annotation)) {}

        /// Move-construct from a singular annotation
        explicit Component(SingularAnnotation &&annotation)
                : annotation_(new SingularAnnotation(std::move(annotation))) {}

        /// Construct from a composite annotation
        explicit Component(CompositeAnnotation &annotation)
                : annotation_(new CompositeAnnotation(annotation)) {}

        /// Move-construct from a composite annotation
        explicit Component(CompositeAnnotation &&annotation)
                : annotation_(new CompositeAnnotation(std::move(annotation))) {}

        Component(Component &other)
                : annotation_(other.hasAnnotation() ? other.getAnnotation().clone() : nullptr) {}

        /// Move-construct from a component
        Component(Component &&other) noexcept: annotation_(std::move(other.annotation_)) {}

        /// Virtual destructor
        ~Component() = default;

        /// Create a copy of this component
        /// todo should this just be the copy constructor?
        virtual Component *clone() {
            return new Component(*this);
        }

        /// @return @c true if the component has an annotation (singular or composite).
        bool hasAnnotation() {
            return !!annotation_;
        }

        /// @return @c true if the component has a composite annotation
        bool hasCompositeAnnotation() {
            return annotation_ && annotation_->isComposite();
        }

        /// @return @c true if the component has a singular annotation
        bool hasSingularAnnotation() {
            return annotation_ && !annotation_->isComposite();
        }

        /**
         * Get this component's annotation.
         * The component may have a singular or composite annotation,
         * but it will be returned as a pointer to the base
         * class for annotations.
         */
        AnnotationBase &getAnnotation() {
            if (!annotation_)
                throw std::runtime_error("No annotation set");
            return *annotation_;
        }

        /**
         * Get this component's annotation.
         * The component may have a singular or composite annotation,
         * but it will be returned as a pointer to the base
         * class for annotations.
         */
        AnnotationBase &getAnnotation() {
            if (!annotation_)
                throw std::runtime_error("No annotation set");
            return *annotation_;
        }

        /**
         * Get this component's annotation and cast it
         * to a composite annotation.
         * It is an error to call this method unless
         * the component's annotation is a composite annotation.
         */
        CompositeAnnotation &getCompositeAnnotation() {
            if (!annotation_)
                throw std::runtime_error("No annotation set");
            if (!annotation_->isComposite())
                throw std::runtime_error("Annotation is not composite");
            return dynamic_cast< CompositeAnnotation &>(*annotation_);
        }

        /**
         * Get this component's annotation and cast it
         * to a composite annotation.
         * It is an error to call this method unless
         * the component's annotation is a composite annotation.
         */
        CompositeAnnotation &getCompositeAnnotation() {
            if (!annotation_)
                throw std::runtime_error("No annotation set");
            if (!annotation_->isComposite())
                throw std::runtime_error("Annotation is not composite");
            return dynamic_cast<CompositeAnnotation &>(*annotation_);
        }

        /**
         * Manually set the annotation (from a raw pointer).
         * This @ref Component will own the passed raw pointer.
         * If the @ref Component currently has an annotation set,
         * it will be freed.
         * @param annotation The annotation for this @ref Component to own.
         */
        void setAnnotation(AnnotationBase *annotation) {
            annotation_.reset(annotation);
        }

        /**
         * Manually set the annotation (copy the passed annotation).
         * If the @ref Component currently has an annotation set,
         * it will be freed.
         * @param annotation The annotation for this @ref Component to own.
         */
        void setAnnotation(AnnotationBase &annotation) {
            annotation_.reset(annotation.clone());
        }

        /**
         * Manually set the annotation.
         * If the @ref Object currently has an annotation set,
         * it will be freed.
         * @param annotation The annotation for this @ref Object to own.
         */
        void setAnnotation(AnnotationPtr &&annotation) {
            annotation_ = std::move(annotation);
        }

        /**
         * Return a human--readable representation of the annotation
         * information. Ontology terms will be replaced with human-readable
         * names.
         */
        std::string humanize() {
            if (annotation_)
                return annotation_->humanize();
            else
                return "";
        }

        /// Return @c true if this component has a meta id (required for serialization).
        bool hasMetaId() {
            if (annotation_)
                return true;
            else return !metaid_.empty();
        }

        /// Get the meta id of this component.
        std::string &getMetaId() {
            if (!metaid_.empty())
                return metaid_;
            else if (annotation_)
                return annotation_->getMetaId();
            else
                throw std::runtime_error("Meta id of component is not set.");
        }

        /// Set the meta id of this component.
        void setMetaId(std::string &metaid) {
            metaid_ = metaid;
        }

        /// Get the local Url of this component
        virtual Url getURI(Url &base) {
            std::string metaid = getMetaId();
            return base.fragment(metaid);
        }

        /**
         * Serialize this annotation to RDF using the Raptor library.
         * @param sbml_base_uri   The base Url of the SBML document relative to this (e.g. a relative path in a COMBINE archive).
         * @param world      Raptor world object. Must be initialized prior to calling this function.
         * @param serializer Raptor serializer object. Must be initialized prior to calling this function.
         * @return the Url for this entity.
         */
        virtual void serializeToRDF(Url &sbml_base_uri, raptor_world *world, raptor_serializer *serializer) {
            if (annotation_)
                getAnnotation().serializeToRDF(sbml_base_uri, world, serializer);
        }

        virtual std::string getRDF(Url &sbml_base_uri, std::string &format = "rdfxml") {
            if (hasAnnotation())
                return annotation_->getRDF(sbml_base_uri, format);
            else
                throw std::runtime_error("No annotation");
        }

        virtual bool isProcess() {
            return false;
        }

        virtual bool containsMetaId(std::string &metaid) {
            return metaid_ == metaid;
        }

    protected:
        AnnotationPtr annotation_;
        std::string metaid_;
    };

    /**
     * An owning pointer for a @ref Component.
     * Automatically uses std::unique_ptr or std::tr1::shared_ptr depending on C++ standard.
     * @see UniquePtr.
     */
    typedef std::unique_ptr<Component> ComponentPtr;

}

# endif