#include "omexmeta/OmexMetaCApi.h"

using namespace omexmeta;

int main(){
    const char* sbml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                       "<!-- Created by libAntimony version v2.12.0 with libSBML version 5.18.1. -->\n"
                       "<sbml xmlns=\"http://www.sbml.org/sbml/level3/version1/core\" level=\"3\" version=\"1\">\n"
                       "  <model metaid=\"SimpleRegulation\" id=\"SimpleRegulation\">\n"
                       "    <listOfCompartments>\n"
                       "      <compartment id=\"cell\" spatialDimensions=\"3\" size=\"1\" constant=\"true\" metaid=\"#OmexMetaId0000\"/>\n"
                       "    </listOfCompartments>\n"
                       "    <listOfSpecies>\n"
                       "      <species id=\"A\" compartment=\"cell\" initialConcentration=\"10\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\" metaid=\"#OmexMetaId0001\"/>\n"
                       "      <species id=\"B\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\" metaid=\"#OmexMetaId0002\"/>\n"
                       "      <species id=\"C\" compartment=\"cell\" initialConcentration=\"10\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\" metaid=\"#OmexMetaId0003\"/>\n"
                       "      <species id=\"D\" compartment=\"cell\" initialConcentration=\"0\" hasOnlySubstanceUnits=\"false\" boundaryCondition=\"false\" constant=\"false\" metaid=\"#OmexMetaId0004\"/>\n"
                       "    </listOfSpecies>\n"
                       "    <listOfParameters>\n"
                       "      <parameter id=\"S\" value=\"0\" constant=\"true\"/>\n"
                       "      <parameter id=\"k1\" value=\"0.1\" constant=\"true\"/>\n"
                       "      <parameter id=\"k2\" value=\"0.1\" constant=\"true\"/>\n"
                       "      <parameter id=\"k3\" value=\"0.1\" constant=\"true\"/>\n"
                       "      <parameter id=\"k4\" value=\"0.1\" constant=\"true\"/>\n"
                       "    </listOfParameters>\n"
                       "    <listOfReactions>\n"
                       "      <reaction id=\"R1\" reversible=\"false\" fast=\"false\" metaid=\"#OmexMetaId0005\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"A\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"B\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw metaid=\"#OmexMetaId0006\">\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k1 </ci>\n"
                       "              <ci> A </ci>\n"
                       "              <ci> S </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"R2\" reversible=\"false\" fast=\"false\" metaid=\"#OmexMetaId0007\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"B\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"A\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw metaid=\"#OmexMetaId0008\">\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k2 </ci>\n"
                       "              <ci> B </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"R3\" reversible=\"false\" fast=\"false\" metaid=\"#OmexMetaId0009\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"C\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"D\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <listOfModifiers>\n"
                       "          <modifierSpeciesReference species=\"B\"/>\n"
                       "        </listOfModifiers>\n"
                       "        <kineticLaw metaid=\"#OmexMetaId0010\">\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k3 </ci>\n"
                       "              <ci> C </ci>\n"
                       "              <ci> B </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "      <reaction id=\"R4\" reversible=\"false\" fast=\"false\" metaid=\"#OmexMetaId0011\">\n"
                       "        <listOfReactants>\n"
                       "          <speciesReference species=\"D\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfReactants>\n"
                       "        <listOfProducts>\n"
                       "          <speciesReference species=\"C\" stoichiometry=\"1\" constant=\"true\"/>\n"
                       "        </listOfProducts>\n"
                       "        <kineticLaw metaid=\"#OmexMetaId0012\">\n"
                       "          <math xmlns=\"http://www.w3.org/1998/Math/MathML\">\n"
                       "            <apply>\n"
                       "              <times/>\n"
                       "              <ci> k4 </ci>\n"
                       "              <ci> D </ci>\n"
                       "            </apply>\n"
                       "          </math>\n"
                       "        </kineticLaw>\n"
                       "      </reaction>\n"
                       "    </listOfReactions>\n"
                       "  </model>\n"
                       "</sbml>";

    RDF* rdf = RDF_new();

    Editor* editor = RDF_toEditor(rdf, sbml, true, false);

    PhysicalEntity* physicalEntity = PhysicalEntity_new(editor);
    PhysicalEntity_setPhysicalProperty(physicalEntity,"#OmexMetaId0001", "OPB:OPB_00340");
    PhysicalEntity_setIdentity(physicalEntity, "uniprot/P84022");
    PhysicalEntity_addLocation(physicalEntity, "fma/FMA:63877");
    PhysicalEntity_addLocation(physicalEntity, "fma:FMA:70737");
    PhysicalEntity_addLocation(physicalEntity, "fma/FMA:24178");
    PhysicalEntity_addLocation(physicalEntity, "fma/FMA:7163");

    Editor_addPhysicalEntity(editor, physicalEntity);
    char* rdf_string = RDF_toString(rdf, "turtle");
    printf("%s\n", rdf_string);

    free(rdf_string);
    PhysicalEntity_delete(physicalEntity);
    Editor_delete(editor);
    RDF_delete(rdf);
    return 0;
}