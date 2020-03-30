# ifndef SEMSIM_OMEX_OUTPUT_H_
# define SEMSIM_OMEX_OUTPUT_H_


# include "semsim/SemsimModel.h"


namespace semsim {
    /**
     * Write a @ref SemsimModel and associated annotation information to a
     * Combine archive.
     * @param model            The libSemSim @ref SemsimModel to write out.
     * @param archive_path     The path for the Combine archive.
     * @param model_entry_path The path to the model file (SBML or CellML) within the archive.
     * @param rdf_entry_path   The path to the annotation RDF metadata within the archive.
     */
    void exportToOmex(const SemsimModel &model,
                      const std::string &archive_path,
                      const std::string &model_entry_path = "./model.xml",
                      const std::string &rdf_entry_path = "./annotations.xml");
}
# endif
