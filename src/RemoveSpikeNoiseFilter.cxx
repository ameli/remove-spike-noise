/*
 * ============================================================================
 *
 *       Filename:  RemoveSpikeNoiseFilter.cxx
 *
 *    Description:  A filter that removes spike noises
 *
 *        Version:  1.0
 *        Created:  09/14/2014 01:09:28 PM
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

#include "RemoveSpikeNoiseFilter.h"

// Pipeline
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkDataObject.h>
#include <vtkObjectFactory.h>
#include <vtkDemandDrivenPipeline.h>
#include <vtkImageData.h>

// STL
#include <cstdlib>


// ======
// Macros
// ======

vtkStandardNewMacro(RemoveSpikeNoiseFilter);

#define DIMENSION 2


// ===========
// Constructor
// ===========

RemoveSpikeNoiseFilter::RemoveSpikeNoiseFilter()
{
}


// ==========
// Destructor
// ==========

RemoveSpikeNoiseFilter::~RemoveSpikeNoiseFilter()
{
}


// ==========
// Print Self
// ==========

void RemoveSpikeNoiseFilter::PrintSelf(ostream &os,vtkIndent Indent)
{
    this->Superclass::PrintSelf(os,Indent);
}

// ============================
// Fill Output Port Information
// ============================

int RemoveSpikeNoiseFilter::FillOutputPortInformation(
        int Port,
        vtkInformation *OutputInformation)
{
    if(Port == 0)
    {
        OutputInformation->Set(vtkDataObject::DATA_TYPE_NAME(),"vtkImageData");
    }

    return 1;
}


// ===================
// Request Data Object
// ===================

int RemoveSpikeNoiseFilter::RequestDataObject(
        vtkInformation *Request,
        vtkInformationVector **vtkNotUsed(InputInformationVector),
        vtkInformationVector *OutputInformationVector)
{
    vtkInformation *OutputInformation = \
        OutputInformationVector->GetInformationObject(0);
    vtkImageData *OutputImageData = vtkImageData::SafeDownCast(
        OutputInformation->Get(vtkDataObject::DATA_OBJECT()));

    // Create new instance
    if(OutputImageData == NULL)
    {
        OutputImageData = vtkImageData::SafeDownCast(vtkImageData::New());
        OutputInformation->Set(vtkDataObject::DATA_OBJECT(),OutputImageData);
        OutputImageData->FastDelete();
        this->GetOutputInformation(0)->Set(
                vtkDataObject::DATA_EXTENT_TYPE(),
                OutputImageData->GetExtentType());
    }

    return 1;
}


// ============
// Request Data
// ============

int RemoveSpikeNoiseFilter::RequestData(
        vtkInformation *Request,
        vtkInformationVector **InputInformationVector,
        vtkInformationVector *OutputInformationVector)
{
    // Input
    vtkInformation *InputInformation = \
        InputInformationVector[0]->GetInformationObject(0);
    vtkImageData *InputImageData = vtkImageData::SafeDownCast(
            InputInformation->Get(vtkDataObject::DATA_OBJECT()));

    // Output
    vtkInformation *OutputInformation = \
        OutputInformationVector->GetInformationObject(0);
    vtkImageData *OutputImageData = vtkImageData::SafeDownCast(
        OutputInformation->Get(vtkDataObject::DATA_OBJECT()));

    // Remove Spike Noise
    this->RemoveSpikeNoise(InputImageData,OutputImageData);

    // Copy input to output
    OutputImageData->CopyStructure(InputImageData);

    return 1;
}


// ==================
// Remove Spike Noise
// ==================

void RemoveSpikeNoiseFilter::RemoveSpikeNoise(
        vtkImageData *InputImageData,
        vtkImageData *OutputImageData)   // Output
{
    // Number of Points
    unsigned int NumberOfPoints = InputImageData->GetNumberOfPoints();

    if(NumberOfPoints < 1)
    {
        vtkErrorMacro("Input data does not have points.");
        exit(0);
    }

    #ifdef _OPENMP
    #pragma omp parallel for default(none) \
    shared(InputImageData,NumberOfPoints,std::cout)
    #endif
    for(unsigned int PointId = 0; PointId < NumberOfPoints; PointId++)
    {
        // Get Point
        double PointCoordinate[3];
        InputImageData->GetPoint(PointId,PointCoordinate);

        if(PointId == 0)
        {
            #pragma omp critical
            {
                std::cout << PointCoordinate[0] << ", ";
                std::cout << PointCoordinate[1] << ", ";
                std::cout << PointCoordinate[2] << std::endl;
            }
        }
    }
}


// ===============
// Get Point Index
// ===============

// Description:
// Converts the list style ID if a point into coordinate-wise indices if the
// point in a structured grid. Grid Resolution is number of points in each
// direction. Note: PointId is 0-offset. So the first point is 0. Therefore,
// the PointIndex are also offset from 0. So the first point is (0,0,0) in 3D.

void RemoveSpikeNoiseFilter::GetPointIndex(
        int *GridResolution,
        int PointId,
        unsigned int *PointIndex)  // Output
{
    for(unsigned int DimensionIterator = 0;
        DimensionIterator < DIMENSION;
        DimensionIterator++)
    {
        div_t DivisionResult;
        DivisionResult = std::div(
            static_cast<int>(PointId),GridResolution[DimensionIterator]);
        PointIndex[DimensionIterator] = DivisionResult.rem;
        PointId = DivisionResult.quot;
    }
}


// ============
// Get Point Id
// ============

// Description:
// Given coordinate index for a point, it returns the point Id in structured
// grid. All components of PointIndex are offset form 0. So the output PointId
// will be offset from 0. GridResolution is number of points at each direction.

inline int RemoveSpikeNoiseFilter::GetPointId(
        int *GridResolution,
        int *PointIndex)
{
    int PointId = 0;
    unsigned int DimensionProduct = 1;

    for(unsigned int DimensionIterator = 0;
        DimensionIterator < DIMENSION;
        DimensionIterator++)
    {
        PointId += DimensionProduct * PointIndex[DimensionIterator];
        DimensionProduct *= GridResolution[DimensionIterator];
    }

    return PointId;
}

// ===============
// Get Stencil Ids
// ===============

// Description:
// Finds the point Ids of stencil neighbors  of a point on a structured grid.
// Note: If point is on the boundary, it considers the point itself instead of
// a stencil outdside the domain. Thus the central point duplicates.
// Note: The stencil is only two points in a specific direction. Direction = 0
// returns the two stencil points in x-axis, Direction = 1 returns the two
// stencils in y-axis, etc. Output is an array with two elements as following
// StencilIds[0] is the back point Id.
// StencilIds[1] is the front point Id.

void RemoveSpikeNoiseFilter::GetStencilIds(
        int *GridResolution,
        int PointId,
        unsigned int TargetDirection,
        int *StencilIds)   // Output
{
    // Get point index
    unsigned int PointIndex[DIMENSION];
    this->GetPointIndex(GridResolution,PointId,PointIndex);

    // Stencils Index
    int BackStencilIndex[DIMENSION];
    int FrontStencilIndex[DIMENSION];

    for(unsigned int DimensionIterator = 0;
        DimensionIterator < DIMENSION;
        DimensionIterator++)
    {
        // Initialize Stencil indices the same as point index
        BackStencilIndex[DimensionIterator] = PointIndex[DimensionIterator];
        FrontStencilIndex[DimensionIterator] = PointIndex[DimensionIterator];

        // Set stencil indices in the requested direction
        if(DimensionIterator == TargetDirection)
        {
            // Check right boundaries for front stencil
            if(PointIndex[TargetDirection] < static_cast<unsigned int>(
                GridResolution[TargetDirection]-1))
            {
                FrontStencilIndex[TargetDirection]++;
            }

            // Check left boundaries for back stencil
            if(PointIndex[TargetDirection] > 0)
            {
                BackStencilIndex[TargetDirection]--;
            }
        }
    }

    // Convert Stencil index to list Id
    StencilIds[0] = vtkStructuredData::ComputePointId(
            GridResolution,BackStencilIndex);  // Back stencil
    StencilIds[1] = vtkStructuredData::ComputePointId(
            GridResolution,FrontStencilIndex); // Front stencil
}
