/*
 * ============================================================================
 *
 *       Filename:  main.cxx
 *
 *    Description:  Remove Spike Noise
 *
 *        Version:  1.0
 *        Created:  09/14/2014 01:08:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Siavash Ameli
 *   Organization:  University Of California, Berkeley
 *
 * ============================================================================
 */


// =======
// Headers
// =======

#include "main.h"
#include "RemoveSpikeNoiseFilter.h"

// VTK
#include <vtkSmartPointer.h>
#include <vtkStructuredPointsReader.h>
#include <vtkStructuredPointsWriter.h>

// STL
#include <iostream>
#include <sstream>


// ====
// Main
// ====

int main(int argc, char *argv[])
{
    // Parse Arguments
    char InputFilename[256];
    ParseArguments(argc,argv,InputFilename);

    // Read Data
    vtkSmartPointer<vtkStructuredPointsReader> Reader = \
        vtkSmartPointer<vtkStructuredPointsReader>::New();
    Reader->SetFileName(InputFilename);
    Reader->Update();

    // Remove Spike Noise Filter
    vtkSmartPointer<RemoveSpikeNoiseFilter> RemoveSpikeNoise = \
        vtkSmartPointer<RemoveSpikeNoiseFilter>::New();
    RemoveSpikeNoise->SetInputConnection(Reader->GetOutputPort());
    RemoveSpikeNoise->Update();

    // Generate Output filename
    char OutputFilename[256];
    GenerateOutputFilename(InputFilename,OutputFilename);
    std::cout << OutputFilename << std::endl;

    // Write
    vtkSmartPointer<vtkStructuredPointsWriter> Writer = \
        vtkSmartPointer<vtkStructuredPointsWriter>::New();
    Writer->SetInputConnection(RemoveSpikeNoise->GetOutputPort());
    Writer->SetFileName(OutputFilename);
    Writer->Update();

    return EXIT_SUCCESS;
}


// ===============
// Parse Arguments
// ===============

void ParseArguments(
        int &argc,
        char *argv[],
        char *InputFilename)   // Output
{
    if(argc < 2)
    {
        PrintUsage(argv[0]);
        exit(0);
    }
    else
    {
        strcpy(InputFilename,argv[1]);
    }

    // Check Inputs
    if(strlen(InputFilename) < 1)
    {
        std::cerr << "InputFilename is empty. Program terminated.";
        std::cerr << std::endl;
        exit(0);
    }
}


// ===========
// Print Usage
// ===========

void PrintUsage(const char *ExecutableName)
{
    std::cerr << "Usage:  " << ExecutableName << "  InputFilename";
    std::cerr << std::endl;
}


// ========================
// Generate Output Filename
// ========================

void GenerateOutputFilename(
        const char *InputFilename,
        char *OutputFilename)
{
    std::stringstream OutputFilenameStream;
    OutputFilenameStream << "Filtered-" << InputFilename;
    strcpy(OutputFilename,OutputFilenameStream.str().c_str());
}
