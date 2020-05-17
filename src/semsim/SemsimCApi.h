//
// Created by Ciaran on 5/8/2020.
//

#ifndef LIBSEMSIM_SEMSIMCAPI_H
#define LIBSEMSIM_SEMSIMCAPI_H

#include "semsim/RDF.h"
#include "semsim/Editor.h"
#include "semsim/XmlAssistant.h"
#include "semsim/PhysicalEntity.h"
#include "semsim/PhysicalProcess.h"
#include "semsim/PhysicalForce.h"

namespace semsim {

#ifdef __cplusplus
    extern "C" {
#endif

        void free_c_char_star(char* c);

        void free_c_char_star_star(char** c, int size);

        char* _func_that_returns_dynamic_alloc_str();

        char** _func_that_returns_dynamic_alloc_char_star_star();

/***************************************************
 * RDF class methods
 */

    semsim::RDF *RDF_new();

    void RDF_delete(semsim::RDF *rdf_ptr);

    char *RDF_getBaseUri(semsim::RDF *rdf_ptr);

    void RDF_setBaseUri(semsim::RDF *rdf_ptr, const char *uri);

    char * RDF_toString(semsim::RDF *rdf_ptr, const char *format, const char *base_uri);

    void RDF_fromString(semsim::RDF *rdf_ptr, const char *str, const char *format);

    const char *RDF_queryResultsAsStr(semsim::RDF *rdf_ptr, const char *query_str, const char *results_format);

    int RDF_size(semsim::RDF *rdf_ptr);

    Editor *RDF_toEditor(semsim::RDF *rdf_ptr, const char *xml, semsim::XmlAssistantType type);

//RDF_toFile(semsim::RDF* rdf_ptr, );
//RDF_listOptions(semsim::RDF* rdf_ptr, );
//RDF_setNamespaces(semsim::RDF* rdf_ptr, );
//RDF_getNamespaces(semsim::RDF* rdf_ptr, );
//semsim::RDF* RDF_fromUrl(semsim::RDF* rdf_ptr, const char* url, const char* filename, const char* format);
//semsim::RDF* RDF_fromXML(semsim::RDF* rdf_ptr, const char* filename, const char* format);
//semsim::RDF* RDF_fromFile(semsim::RDF* rdf_ptr, const char* filename, const char* format);
//semsim::RDF* RDF_fromOmex(semsim::RDF* rdf_ptr, const char* filename_or_uri, const char* format);

/*********************************************
 *  Editor class methods
 */

    void Editor_addNamespace(Editor *editor_ptr, const char *namespace_, const char *prefix);

    void Editor_addSingleAnnotation(Editor *editor_ptr, SingularAnnotation *singularAnnotation);

    void Editor_addPhysicalEntity(Editor *editor_ptr, PhysicalEntity *physicalEntity);

    void Editor_addPhysicalProcess(Editor *editor_ptr, PhysicalProcess *physicalProcess);

    void Editor_addPhysicalForce(Editor *editor_ptr, PhysicalForce *physicalForce);

    void Editor_checkValidMetaid(Editor *editor_ptr, const char *id);

    void Editor_toRDF(Editor *editor_ptr);

    void Editor_delete(Editor *editor_ptr);


/*********************************************************************
 * SingularAnnotation class methods
 */
    SingularAnnotation *SingularAnnotation_new(Editor *editor_ptr);

    void SingularAnnotation_delete(SingularAnnotation* singularAnnotation);

    SingularAnnotation *SingularAnnotation_setAbout(SingularAnnotation *singular_annotation, const char *about);

    SingularAnnotation *
    SingularAnnotation_setPredicate(SingularAnnotation *singular_annotation, const char *namespace_,
                                    const char *term);

    SingularAnnotation *
    SingularAnnotation_setPredicateNew(SingularAnnotation *singular_annotation, const char *namespace_,
                                       const char *term, const char *prefix);

    SingularAnnotation *
    SingularAnnotation_setResourceLiteral(SingularAnnotation *singular_annotation, const char *literal);

    SingularAnnotation *
    SingularAnnotation_setResourceUri(SingularAnnotation *singular_annotation, const char *identifiers_uri);

    SingularAnnotation *
    SingularAnnotation_setResourceBlank(SingularAnnotation *singular_annotation, const char *blank_id);

    char * SingularAnnotation_getAbout(SingularAnnotation *singular_annotation);

    char *
    SingularAnnotation_str(SingularAnnotation *singular_annotation, const char *format, const char *base_uri);

    char * SingularAnnotation_getPredicate(SingularAnnotation *singular_annotation);

    char *SingularAnnotation_getResource(SingularAnnotation *singular_annotation);

/*********************************************************************
 * PhysicalEntity class methods
 */
    PhysicalEntity *PhysicalEntity_new(Editor *editor_ptr);

    void PhysicalEntity_delete(PhysicalEntity *physical_entity_ptr);

    PhysicalEntity *PhysicalEntity_setAbout(PhysicalEntity *physical_entity_ptr, const char *about);

    PhysicalEntity *PhysicalEntity_setPhysicalProperty(
            PhysicalEntity *physical_entity_ptr, const char *physical_property);

    PhysicalEntity *PhysicalEntity_setIdentity(
            PhysicalEntity *physical_entity_ptr, const char *identity_resource);

    PhysicalEntity *PhysicalEntity_addLocation(
            PhysicalEntity *physical_entity_ptr, const char *location_resource);


    char *PhysicalEntity_getAbout(PhysicalEntity *physical_entity_ptr);

    char *PhysicalEntity_getPhysicalProperty(PhysicalEntity *physical_entity_ptr);

    char *PhysicalEntity_getIdentity(PhysicalEntity *physical_entity_ptr);

    int PhysicalEntity_getNumLocations(PhysicalEntity *physicalEntity);

    char **PhysicalEntity_getLocations(PhysicalEntity *physical_entity_ptr);

    char *PhysicalEntity_str(PhysicalEntity *physical_entity_ptr, const char *format, const char *base_uri);

/*********************************************************************
 * PhysicalProcess class methods
 */
    PhysicalProcess *PhysicalProcess_new(Editor *editor_ptr);

    void PhysicalProcess_delete(PhysicalProcess* physicalProcess);

    PhysicalProcess *PhysicalProcess_setAbout(
            PhysicalProcess *physical_process, const char *about);

    PhysicalProcess *PhysicalProcess_setPhysicalProperty(
            PhysicalProcess *physical_process, const char *physical_property);

    PhysicalProcess *PhysicalProcess_addSource(
            PhysicalProcess *physical_process, const char *source_metai, double multiplier,
            const char *physical_entity_reference);

    PhysicalProcess *PhysicalProcess_addSink(
            PhysicalProcess *physical_process, const char *sink_metaid, double multiplier,
            const char *physical_entity_reference);

    PhysicalProcess *PhysicalProcess_addMediator(
            PhysicalProcess *physical_process, const char *mediator_metaid, double multiplier,
            const char *physical_entity_reference);

    char *PhysicalProcess_str(PhysicalProcess *physical_process_ptr, const char *format, const char *base_uri);

    char *PhysicalProcess_getAbout(PhysicalProcess *physical_process_ptr);

    char *PhysicalProcess_getPhysicalProperty(PhysicalProcess *physical_process_ptr);

/*********************************************************************
 * PhysicalForce class methods
 */
    PhysicalForce *PhysicalForce_new(Editor *editor_ptr);

    void PhysicalForce_delete(PhysicalForce* physicalForce);

    PhysicalForce *PhysicalForce_setAbout(
            PhysicalForce *physical_force_ptr, const char *about);

    PhysicalForce *PhysicalForce_addSource(
            PhysicalForce *physical_force_ptr, const char *source_metaid, double multiplier,
            const char *physical_entity_reference);

    PhysicalForce *PhysicalForce_addSink(
            PhysicalForce *physical_force_ptr, const char *sink_metaid, double multiplier,
            const char *physical_entity_reference);

    PhysicalForce *PhysicalForce_setPhysicalProperty(
            PhysicalForce *physical_force_ptr, const char *physical_property);

    int PhysicalForce_getNumSources(PhysicalForce *physicalForce);

    int PhysicalForce_getNumSinks(PhysicalForce *physicalForce);

    char *PhysicalForce_str(PhysicalForce *physical_force_ptr, const char *format, const char *base_uri);

    char *PhysicalForce_getAbout(PhysicalForce *physical_force_ptr);

    char *PhysicalForce_getPhysicalProperty(PhysicalForce *physical_force_ptr) ;



#ifdef __cplusplus
    }
#endif
}

#endif //LIBSEMSIM_SEMSIMCAPI_H
























