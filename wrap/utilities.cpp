/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file utilities.ccp
 * @author Frank Dellaert
 **/

#include <iostream>

#include <boost/foreach.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "utilities.h"

namespace wrap {

using namespace std;
using namespace boost::gregorian;

/* ************************************************************************* */
string file_contents(const string& filename, bool skipheader) {
  ifstream ifs(filename.c_str());
  if(!ifs) throw CantOpenFile(filename);

  // read file into stringstream
  stringstream ss;
  if (skipheader) ifs.ignore(256,'\n');
  ss << ifs.rdbuf();
  ifs.close();

  // return string
  return ss.str();
}

/* ************************************************************************* */
bool assert_equal(const string& expected, const string& actual) {
	if (expected == actual)
		return true;
	printf("Not equal:\n");
	cout << "expected: [" << expected << "]\n";
	cout << "actual: [" << actual << "]" << endl;
	return false;
}

/* ************************************************************************* */
bool assert_equal(const vector<string>& expected, const vector<string>& actual) {
  bool match = true;
  if (expected.size() != actual.size())
    match = false;
  vector<string>::const_iterator
  	itExp = expected.begin(),
  	itAct = actual.begin();
  if(match) {
  	for (; itExp!=expected.end() && itAct!=actual.end(); ++itExp, ++itAct) {
  		if (*itExp != *itAct) {
  			match = false;
  			break;
  		}
  	}
  }
	if(!match) {
	  cout << "expected: " << endl;
	  BOOST_FOREACH(const vector<string>::value_type& a, expected) { cout << "["  << a << "] "; }
	  cout << "\nactual: " << endl;
	  BOOST_FOREACH(const vector<string>::value_type& a, actual) { cout << "["  << a << "] "; }
	  cout << endl;
	  return false;
	}
	return true;

}

/* ************************************************************************* */
bool files_equal(const string& expected, const string& actual, bool skipheader) {
  try {
    string expected_contents = file_contents(expected, skipheader);
    string actual_contents   = file_contents(actual, skipheader);
    bool equal = actual_contents == expected_contents;
    if (!equal) {
      stringstream command;
      command << "diff " << actual << " " << expected << endl;
      system(command.str().c_str());
    }
    return equal;
  }
  catch (const string& reason) {
    cerr << "expection: " << reason << endl;
    return false;
  }
  catch (CantOpenFile& e) {
  	cerr << "file opening error: " << e.what() << endl;
  	return false;
  }
  return true;
}

/* ************************************************************************* */
void generateHeaderComment(ofstream& ofs, const string& delimiter) {
  date today = day_clock::local_day();
  ofs << delimiter << " automatically generated by wrap on " << today << endl;
}

/* ************************************************************************* */
string maybe_shared_ptr(bool add, const string& type) {
  string str = add? "shared_ptr<" : "";
  str += type;
  if (add) str += ">";
  return str;
}

/* ************************************************************************* */
void generateUsingNamespace(ofstream& ofs, const vector<string>& using_namespaces) {
	if (using_namespaces.empty()) return;
	BOOST_FOREACH(const string& s, using_namespaces)
		ofs << "using namespace " << s << ";" << endl;
}

/* ************************************************************************* */
void generateIncludes(ofstream& ofs, const string& class_name,
		const vector<string>& includes) {
	ofs << "#include <wrap/matlab.h>" << endl;
	if (includes.empty()) // add a default include
		ofs << "#include <" << class_name << ".h>" << endl;
	else {
		BOOST_FOREACH(const string& s, includes)
	  		ofs << "#include <" << s << ">" << endl;
	}
}

/* ************************************************************************* */

} // \namespace wrap
