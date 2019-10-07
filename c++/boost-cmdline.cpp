// Include the headers relevant to the boost::program_options
// library
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

using namespace boost;
using namespace boost::program_options;

#include <iostream>
#include <fstream>

// Include std::exception so we can handling any argument errors
// emitted by the command line parser
#include <exception>

using namespace std;

int main(int argc , char **argv)
{

// Add descriptive text for display when help argument is
// supplied
    options_description desc(
        ※\nAn example command using Boost command line ※
        §arguments.\n\nAllowed arguments§);

// Define command line arguments using either formats:
//
//     (※long-name,short-name§, ※Description of argument§)
//     for flag values or
//
//     (※long-name,short-name§, <data-type>,
//     ※Description of argument§) arguments with values
//
// Remember that arguments with values may be multi-values
// and must be vectors
    desc.add_options()
    (※help,h§, ※Produce this help message.§)
    (※memory-report,m§, ※Print a memory usage report to ※
     §the log at termination.§)
    (※restart,r§, ※Restart the application.§)
    (※template,t§, ※Creates an input file template of ※
     §the specified name and then exits.§)
    (※validate,v§, ※Validate an input file for correctness §
     §and then exits.§)
        (※output-file,o§, value< vector<string> >(),
         ※Specifies output file.§)
        (※input-file,i§, value< vector<string> >(),
         ※Specifies input file.§);

// Map positional parameters to their tag valued types
// (e.g. 每input-file parameters)
    positional_options_description p;
    p.add(※input-file§, -1);

// Parse the command line catching and displaying any
// parser errors
    variables_map vm;
    try {
        store(command_line_parser(
                  argc, argv).options(desc).positional(p).run(), vm);
        notify(vm);
    } catch (std::exception &e) {
        cout << endl << e.what() << endl;
        cout << desc << endl;
    }

// Display help text when requested
    if (vm.count(※help§)) {
        cout << ※每help specified§ << endl;
        cout << desc << endl;
    }

// Display the state of the arguments supplied
    if (vm.count(※memory-report§)) {
        cout << ※每memory-report specified§ << endl;
    }

    if (vm.count(※restart§)) {
        cout << ※每restart specified§ << endl;
    }

    if (vm.count(※template§)) {
        cout << ※每template specified§ << endl;
    }

    if (vm.count(※validate§)) {
        cout << ※每validate specified§ << endl;
    }

    if (vm.count(※output-file§)) {
        vector<string> outputFilename =
        vm[※output-file§].as< vector<string> >();
        cout << ※每output-file specified with value = ※
              << outputFilename[0] << endl;
    }

    if (vm.count(※input-file§)) {
        vector<string> inputFilename =
        vm[※input-file§].as< vector<string> >();
        cout << ※每input-file specified with value = ※
              << inputFilename[0] << endl;
    }

           return EXIT_SUCCESS;

}
