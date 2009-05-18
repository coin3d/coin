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

bool compare_suffix(const std::string & input ,const std::string & suffix) {
  int suffixLength = suffix.size();

  int n = input.size();
  if (n<suffixLength)
    return false;

  std::string toCompare = input.substr(n-suffixLength,suffixLength);
  return toCompare == suffix;
}

bool find_file( const path & dir_path,         // in this directory,
                const std::string & suffix, // search for this name,
                std::vector<path> & paths )            // placing path here if found
{
  bool file_found = false;
  if ( !exists( dir_path ) ) return false;
  directory_iterator end_itr; // default construction yields past-the-end
  for ( directory_iterator itr( dir_path );
        itr != end_itr;
        ++itr )
    {
      if ( is_directory(itr->status()) )
        {
          if ( find_file( itr->path(), suffix, paths ) )
            file_found = true;
        }
      else if ( compare_suffix(itr->leaf(),suffix) ) // see below
        {
          paths.push_back(itr->path());
          file_found = true;
        }
    }
  return file_found;
}

using namespace SIM::Coin3D::Coin;

BOOST_AUTO_TEST_SUITE(StandardTests);

BOOST_AUTO_TEST_CASE(loadfiles)
{
  path basepath;
  {
    char buf[1024];
    getcwd(buf,sizeof(buf));
    basepath=buf;
  }
  std::vector<path> paths;
  find_file("models", ".wrl", paths);
  find_file("models", ".wrml", paths);
  find_file("models", ".wrl.gz", paths);
  find_file("models", ".wrml.gz", paths);
  find_file("models", ".iv", paths);
  for (
       std::vector<path>::const_iterator it = paths.begin();
       it != paths.end();
       ++it)
    {
      std::string tmp = it->string();
      int n = tmp.find_last_of('/');
      path dir = tmp.substr(0,n);
      path file = tmp.substr(n+1,tmp.size()-n-1);
      chdir(dir.string().c_str());
      SoNode * fileroot = TestSuite::ReadInventorFile(file.string().c_str());
      BOOST_CHECK_MESSAGE(fileroot != NULL, (std::string("failed to read file ") + it->string()).c_str() );
      /*
      if (fileroot == NULL)
        abort();
      */
      if (fileroot != NULL) {
        fileroot->ref();
        fileroot->unref();
      }
      chdir(basepath.string().c_str());
    }
}

BOOST_AUTO_TEST_SUITE_END();
