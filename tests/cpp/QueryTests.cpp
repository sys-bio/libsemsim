//
// Created by Ciaran on 4/28/2020.
//


#include <librdf.h>
#include <gtest/gtest.h>
#include <semsim/RDF.h>
#include <semsim/Triples.h>
#include "SBMLFactory.h"
#include "AnnotationSamples.h"
#include "semsim/Query.h"

class QueryTests : public ::testing::Test {

public:

    AnnotationSamples samples;
    semsim::RDF rdf;

    std::string q;

    QueryTests() {
        rdf = semsim::RDF::fromString(
                samples.singular_annotation2
        );

        q = "SELECT ?x ?y ?z \n"
            "WHERE {\n"
            "  ?x ?y ?z \n"
            "}\n";
    };

    ~QueryTests(){
//        model.freeModel();

    }
};


TEST_F(QueryTests, TestStr) {
    semsim::Query query(rdf.getModel(), q);
    std::string actual = query.resultsAsStr("csv");
    std::string expected = "x,y,z\r\n"
                           "file://./MyModel.xml#modelmeta1,http://biomodels.net/model-qualifiers/isDescribedBy,https://identifiers.org/pubmed/12991237\r\n";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    query.freeQuery();
}

TEST_F(QueryTests, TestRunQueryTwice) {
    semsim::Query query(rdf.getModel(), q);
    query.runQuery();
    std::string actual = query.resultsAsStr("csv");
    std::string expected = "x,y,z\r\n"
                           "file://./MyModel.xml#modelmeta1,http://biomodels.net/model-qualifiers/isDescribedBy,https://identifiers.org/pubmed/12991237\r\n";
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    query.freeQuery();
}

TEST_F(QueryTests, TestgetResultsAsMap) {
    semsim::Query query(rdf.getModel(), q);
    semsim::ResultsMap resultsMap = query.resultsAsMap();
    std::string expected = "http://biomodels.net/model-qualifiers/isDescribedBy";
    std::string actual = resultsMap["y"][0];
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    query.freeQuery();

}

TEST_F(QueryTests, TestgetResultsAsMapTwice) {
    semsim::Query query(rdf.getModel(), q);
    semsim::ResultsMap resultsMap = query.resultsAsMap();
    semsim::ResultsMap resultsMap2 = query.resultsAsMap();
    std::string expected = "http://biomodels.net/model-qualifiers/isDescribedBy";
    std::string actual = resultsMap["y"][0];
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    query.freeQuery();
}

TEST_F(QueryTests, TestResultsAsStream) {
    semsim::Query query(rdf.getModel(), q);
    librdf_stream *stream = query.resultsAsLibRdfStream();
    ASSERT_TRUE(stream); // aka not null
    librdf_free_stream(stream);
    query.freeQuery();
}












