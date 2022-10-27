/*
 * ============================================================================
 *
 *       Filename:  main.h
 *
 *    Description:  Remove Spike Noise
 *
 *        Version:  1.0
 *        Created:  09/14/2014 01:44:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Siavash Ameli 
 *   Organization:  University Of California, Berkeley
 *
 * ============================================================================
 */

#ifndef __RemoveSpikeNoise_h
#define __RemoveSpikeNoise_h


// =======
// Headers
// =======

// ==========
// Prototypes
// ==========

void ParseArguments(
        int &argc,
        char *argv[],
        char *InputFilename);   // Output

void PrintUsage(const char *InputFilename);

void GenerateOutputFilename(
        const char *InputFilename,
        char *OutputFilename);   // Output

#endif
