#define BOOST_TEST_NO_LIB 1
#include <boost/test/unit_test.hpp>

#include <boost/filesystem.hpp>   // includes all needed Boost.Filesystem declarations
#include <iostream>               // for std::cout
using namespace boost::filesystem;          // for ease of tutorial presentation;
                                  //  a namespace alias is preferred practice in real code
#include <assert.h>
#include <stdio.h>
#include <iostream>

#include <Inventor/nodes/SoNode.h>

#include <TestSuiteUtils.h>
#include <TestSuiteMisc.h>


using namespace SIM::Coin3D::Coin;


static bool
testCorrectFile(SoNode * root, std::string & filename) {
  BOOST_CHECK_MESSAGE(root != NULL, (std::string("failed to read file ") + filename).c_str() );
  return root!=NULL;
}

static bool
testInCorrectFile(SoNode * root, std::string & filename) {
  BOOST_CHECK_MESSAGE(root == NULL, (std::string("Managed to read an incorrect file ") + filename).c_str() );
  return root!=NULL;
}

static bool
testOutOfSpecFile(SoNode * root, std::string & filename) {
  BOOST_CHECK_MESSAGE(root != NULL, (std::string("This out of spec file could be read in an earlier version ") + filename).c_str() );
  return root!=NULL;
}


namespace {
  const char * standardSuffixes_initializer [] = { ".wrl", "wrml", "wrl.gz", "wrml.gz", ".iv" };
  static std::vector<std::string> standardSuffixes(standardSuffixes_initializer,&standardSuffixes_initializer[sizeof(standardSuffixes_initializer)/sizeof(standardSuffixes_initializer[0])]);
}


BOOST_AUTO_TEST_SUITE(StandardTests);

BOOST_AUTO_TEST_CASE(loadCorrectfiles)
{
  TestSuite::test_all_files("models",standardSuffixes,&testCorrectFile);
}

BOOST_AUTO_TEST_CASE(loadIncorrectfiles)
{
  TestSuite::test_all_files("killers",standardSuffixes,&testInCorrectFile);
}

BOOST_AUTO_TEST_CASE(loadOutOfSpecFilesWhichWeAccept)
{
  TestSuite::test_all_files("slackers",standardSuffixes,&testOutOfSpecFile);
}

BOOST_AUTO_TEST_SUITE_END();
