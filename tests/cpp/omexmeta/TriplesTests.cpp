//
// Created by Ciaran on 4/29/2020.
//
#include "redland/RedlandAPI.h"
#include "AnnotationSamples.h"
#include "omexmeta/Triples.h"

#include <librdf.h>
#include <gtest/gtest.h>

using namespace omexmeta;

class TriplesTests : public ::testing::Test {

public:

    AnnotationSamples samples;
    LibrdfStorage storage;
    LibrdfModel model;

    LibrdfNode subject;
    LibrdfNode resource;
    BiomodelsBiologyQualifier predicate;
    
    UriHandler uriHandler;


    //todo subject could pass the world to the node
    TriplesTests() {
        model = LibrdfModel(storage.get());
        subject =  LibrdfNode::fromUriString("subby");
        resource = LibrdfNode::fromUriString(("space/id"));
        predicate = BiomodelsBiologyQualifier("is");
    }

    ~TriplesTests() override {
        storage.freeStorage();
        model.freeModel();
    }
};

/*
 * We can create an empty vector and *move* a triple into it.
 */
TEST_F(TriplesTests, TestCreateMoveAfterInstantiation) {
    Triple triple1 = Triple(uriHandler, subject.get(), predicate.get(), resource.get());
    Triples triples;
    triples.moveBack(triple1);
    ASSERT_EQ(1, triples.size());
}

/*
 * We can also move the triple into it on instantiation.
 */
TEST_F(TriplesTests, TestCreateMoveOnInstantiation) {
    Triple triple1 = Triple(uriHandler, subject.get(), predicate.get(), resource.get());
    Triples triples(triple1);
    ASSERT_EQ(1, triples.size());
}

TEST_F(TriplesTests, TestEmplaceBack) {
    Triples triples;
    triples.emplace_back(uriHandler,
                         std::move(subject),
                         std::make_unique<Predicate>(std::move(predicate)),
                         std::move(resource));
    ASSERT_EQ(1, triples.size());
}

/*
 * Note: we no longer need to explicitely call the move method
 * on the subject/predicate/resource arguments since this is
 * done automatically behind the scenes anyway - it is equivalent to
 * TestEmplaceBack
 */
TEST_F(TriplesTests, TestEmplaceBack2) {
    Triples triples;
    triples.emplace_back(uriHandler, std::move(subject),
                         std::make_unique<Predicate>(std::move(predicate)),
                         std::move(resource));
    ASSERT_EQ(1, triples.size());
}

TEST_F(TriplesTests, TestEmplaceBack3) {
    Triples triples;
    triples.emplace_back(uriHandler, std::move(subject),
                         BiomodelsModelQualifier("isDerivedFrom"),
                         std::move(resource));
    ASSERT_EQ(1, triples.size());
    // When you give a subject/predicate/resource to Triple/Triples they
    // are given ownership of the node pointer inside the subject/predicate/resource/
    // In this test we do not use predicate (from setting up test fixture) so we must free it.
}

TEST_F(TriplesTests, TestEmplaceBack4) {
    Triples triples;
    triples.emplace_back(uriHandler, std::move(subject),
                         BiomodelsBiologyQualifier("is"),
                         std::move(resource));
    ASSERT_EQ(1, triples.size());
}

TEST_F(TriplesTests, TestEmplaceBack5) {
    Triples triples;
    triples.emplace_back(uriHandler, std::move(subject),
                         DCTerm("description"),
                         std::move(resource));
    ASSERT_EQ(1, triples.size());
}

TEST_F(TriplesTests, TestEmplaceBack6) {
    Triples triples;
    triples.emplace_back(uriHandler, std::move(subject),
                         SemSim("hasSourceParticipant"),
                         std::move(resource));
    ASSERT_EQ(1, triples.size());
}


TEST(TriplesTestsNoFixture, TestPop) {
    UriHandler uriHandler;
    Triples triples;
    triples.emplace_back(uriHandler,
                         LibrdfNode::fromUriString("subject1").get(),
                         SemSim("hasSinkParticipant").get(),
                         LibrdfNode::fromLiteral("literal node1").get());
    triples.emplace_back(uriHandler,
                         LibrdfNode::fromUriString("subject2").get(),
                         SemSim("hasSourceParticipant").get(),
                         LibrdfNode::fromLiteral("literal node2").get());
    // make sure we have 2 triples
    ASSERT_EQ(2, triples.size());

    // get by move
    Triple triple = triples.pop();

    // make sure we still have 1 Triple objects in Triples
    ASSERT_EQ(1, triples.size());

    // do checks for raptors internal reference counter
    ASSERT_EQ(1, triples[0].getStatement()->usage);

    // make sure we have the right triple left
    ASSERT_STREQ("subject1", triples[0].getSubjectNode().str().c_str());
    ASSERT_STREQ("subject2", triple.getSubjectNode().str().c_str());

    // free the triple and reduce count to 1

    // free the triples. All is accounted for.

}

TEST(TriplesTestsNoFixture, TestPreallocate) {
    UriHandler uriHandler;
    Triples triples(2);
    ASSERT_EQ(2, triples.capacity());

    triples.emplace_back(uriHandler,LibrdfNode::fromUriString("subject1").get(),
                         SemSim("hasSinkParticipant").get(),
                         LibrdfNode::fromLiteral("literal node1").get());
    triples.emplace_back(uriHandler,LibrdfNode::fromUriString("subject2").get(),
                         SemSim("hasSourceParticipant").get(),
                         LibrdfNode::fromLiteral("literal node2").get());
    // make sure we have 2 triples
    ASSERT_EQ(2, triples.size());

}

TEST(TriplesTestsNoFixture, TestStr) {
    UriHandler uriHandler;
    Triples triples;
    triples.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject1.com/subject1").get(),
            LibrdfNode::fromUriString("http://predicate1.com/predicate1").get(),
            LibrdfNode::fromUriString("http://resource1.com/resource1").get()
    );
    triples.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject2.com/subject2").get(),
            LibrdfNode::fromUriString("http://predicate2.com/predicate2").get(),
            LibrdfNode::fromUriString("http://resource2.com/resource2").get()
    );
    std::string expected = "@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n"
                           "@prefix OMEXlib: <http://omex-library.org/> .\n"
                           "@prefix myOMEX: <http://omex-library.org/NewOmex.omex/> .\n"
                           "@prefix local: <http://omex-library.org/NewOmex.omex/NewModel.rdf#> .\n"
                           "\n"
                           "<http://subject1.com/subject1>\n"
                           "    <http://predicate1.com/predicate1> <http://resource1.com/resource1> .\n"
                           "\n"
                           "<http://subject2.com/subject2>\n"
                           "    <http://predicate2.com/predicate2> <http://resource2.com/resource2> .\n"
                           "\n"
                           "";
    std::string actual = triples.str();
    std::cout << actual << std::endl;
    ASSERT_STREQ(expected.c_str(), actual.c_str());
}

TEST(TriplesTestsNoFixture, TestTwoIdenticalTripesObjectsCanBeFreed) {
    UriHandler uriHandler;
    Triples triples1;
    triples1.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject1.com/subject1").get(),
            LibrdfNode::fromUriString("http://predicate1.com/predicate1").get(),
            LibrdfNode::fromUriString("http://resource1.com/resource1").get()
    );
    Triples triples2;
    triples2.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject1.com/subject1").get(),
            LibrdfNode::fromUriString("http://predicate1.com/predicate1").get(),
            LibrdfNode::fromUriString("http://resource1.com/resource1").get()
    );
    ASSERT_EQ(triples1, triples2);



}


TEST(TriplesTestsNoFixture, TestIteration) {
    UriHandler uriHandler;
    Triples triples;
    triples.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject1.com/subject1").get(),
            LibrdfNode::fromUriString("http://predicate1.com/predicate1").get(),
            LibrdfNode::fromUriString("http://resource1.com/resource1").get()
    );
    triples.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject2.com/subject2").get(),
            LibrdfNode::fromUriString("http://predicate2.com/predicate2").get(),
            LibrdfNode::fromUriString("http://resource2.com/resource2").get()
    );
    std::ostringstream os;
    for (int i = 0; i < triples.size(); i++) {
        os << triples[i].getSubjectNode().str() << std::endl;
    }
    std::string expected = "http://subject1.com/subject1\n"
                           "http://subject2.com/subject2\n";
    ASSERT_STREQ(expected.c_str(), os.str().c_str());
}


TEST(TriplesTestsNoFixture, TestEquality) {
    UriHandler uriHandler;
    Triples triples1;
    triples1.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject1.com/subject1").get(),
            LibrdfNode::fromUriString("http://predicate1.com/predicate1").get(),
            LibrdfNode::fromUriString("http://resource1.com/resource1").get()
    );
    triples1.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject2.com/subject2").get(),
            LibrdfNode::fromUriString("http://predicate2.com/predicate2").get(),
            LibrdfNode::fromUriString("http://resource2.com/resource2").get()
    );
    Triples triples2;
    triples2.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject1.com/subject1").get(),
            LibrdfNode::fromUriString("http://predicate1.com/predicate1").get(),
            LibrdfNode::fromUriString("http://resource1.com/resource1").get()
    );
    triples2.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject2.com/subject2").get(),
            LibrdfNode::fromUriString("http://predicate2.com/predicate2").get(),
            LibrdfNode::fromUriString("http://resource2.com/resource2").get()
    );
    ASSERT_EQ(triples1, triples2);


}

//TEST(TriplesTestsNoFixture, TestUsagesEnergyDiff) {
//    UriHandler uriHandler;
//    // replicate energy differential without actually using energy differential
//    Triples triples1(6);
//    triples1.emplace_back(uriHandler,
//            LibrdfNode::fromUriString("./NewModel.sbml#parameter_metaid_0").get(),
//            LibrdfNode::fromUriString("isPropertyOf").get(),
//            LibrdfNode::fromUriString("./NewModel.sbml#force_0").get()
//    );
//
//    triples1.emplace_back(uriHandler,
//            LibrdfNode::fromUriString("./NewModel.sbml#parameter_metaid_0").get(),
//            LibrdfNode::fromUriString("isVersionOf").get(),
//            LibrdfNode::fromUriString("https://identifiers.org/opb/OPB_01058").get()
//    );
//
//    triples1.emplace_back(uriHandler,
//            LibrdfNode::fromUriString("./NewModel.sbml#force_0").get(),
//            LibrdfNode::fromUriString("hasSourceParticipant").get(),
//            LibrdfNode::fromUriString("./NewModel.sbml#source_0").get()
//    );
//
//
//    triples1.emplace_back(uriHandler,
//            LibrdfNode::fromUriString("./NewModel.sbml#force_0").get(),
//            LibrdfNode::fromUriString("hasSinkParticipant").get(),
//            LibrdfNode::fromUriString("./NewModel.sbml#sink_0").get()
//    );
//
//    triples1.emplace_back(uriHandler,
//            LibrdfNode::fromUriString("./NewModel.sbml#source_0").get(),
//            LibrdfNode::fromUriString("hasPhysicalEntityReference").get(),
//            LibrdfNode::fromUriString("./NewModel.sbml#species_metaid_0").get()
//    );
//
//    triples1.emplace_back(uriHandler,
//            LibrdfNode::fromUriString("./NewModel.sbml#sink_0").get(),
//            LibrdfNode::fromUriString("hasPhysicalEntityReference").get(),
//            LibrdfNode::fromUriString("./NewModel.sbml#species_metaid_1").get()
//    );
//
//    Triple& triple1 = triples1[0];
//    auto m1 = triple1.getUsages();
//    ASSERT_EQ(2, m1["subject_uri"]);
//    ASSERT_EQ(1, m1["predicate_uri"]);
//    ASSERT_EQ(3, m1["resource_uri"]);
//
//    Triple& triple2 = triples1[1];
//    auto m2 = triple2.getUsages();
//    ASSERT_EQ(2, m2["subject_uri"]);
//    ASSERT_EQ(1, m2["predicate_uri"]);
//    ASSERT_EQ(1, m2["resource_uri"]);
//
//    Triple& triple3 = triples1[2];
//    auto m3 = triple3.getUsages();
//    ASSERT_EQ(3, m3["subject_uri"]);
//    ASSERT_EQ(1, m3["predicate_uri"]);
//    ASSERT_EQ(2, m3["resource_uri"]);
//
//    Triple& triple4 = triples1[3];
//    auto m4 = triple4.getUsages();
//    ASSERT_EQ(3, m4["subject_uri"]);
//    ASSERT_EQ(1, m4["predicate_uri"]);
//    ASSERT_EQ(2, m4["resource_uri"]);
//
//    Triple& triple5 = triples1[4];
//    auto m5 = triple5.getUsages();
//    ASSERT_EQ(2, m5["subject_uri"]);
//    ASSERT_EQ(2, m5["predicate_uri"]);
//    ASSERT_EQ(1, m5["resource_uri"]);
//
//    Triple& triple6 = triples1[5];
//    auto m6 = triple6.getUsages();
//    ASSERT_EQ(2, m6["subject_uri"]);
//    ASSERT_EQ(2, m6["predicate_uri"]);
//    ASSERT_EQ(1, m6["resource_uri"]);
//
//    triple1.freeTriple();
//    triple2.freeTriple();
//    triple3.freeTriple();
//    triple4.freeTriple();
//    triple5.freeTriple();
//    triple6.freeTriple();
//}


/*
 * Collection of tests to try and debug the
 * reason why I can't free two triples objects
 * that are the same and generated from
 * a PhysicalEntity.
 */
class TestTriplesTwice : public :: testing::Test {
    TestTriplesTwice()= default;;
};

TEST(TestTriplesTwice, TestMakeSameTriplesTwice) {
    UriHandler uriHandler;
    Triples triples1(3);
    triples1.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject1.com/subject1").get(),
            LibrdfNode::fromUriString("http://predicate1.com/predicate1").get(),
            LibrdfNode::fromUriString("http://resource1.com/resource1").get()
    );
    triples1.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject2.com/subject2").get(),
            LibrdfNode::fromUriString("http://predicate2.com/predicate2").get(),
            LibrdfNode::fromUriString("http://resource2.com/resource2").get()
    );
    triples1.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject2.com/subject2").get(),
            LibrdfNode::fromUriString("http://predicate2.com/predicate2").get(),
            LibrdfNode::fromUriString("http://resource2.com/resource2").get()
    );
    std::cout << "creating triples 2 " << std::endl;
    Triples triples2(3);
    triples2.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject1.com/subject1").get(),
            LibrdfNode::fromUriString("http://predicate1.com/predicate1").get(),
            LibrdfNode::fromUriString("http://resource1.com/resource1").get()
    );
    triples2.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject2.com/subject2").get(),
            LibrdfNode::fromUriString("http://predicate2.com/predicate2").get(),
            LibrdfNode::fromUriString("http://resource2.com/resource2").get()
    );
    triples2.emplace_back(uriHandler,
            LibrdfNode::fromUriString("http://subject2.com/subject2").get(),
            LibrdfNode::fromUriString("http://predicate2.com/predicate2").get(),
            LibrdfNode::fromUriString("http://resource2.com/resource2").get()
    );




}






