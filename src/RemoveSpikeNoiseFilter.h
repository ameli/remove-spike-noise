/*
 * ============================================================================
 *
 *       Filename:  RemoveSpikeNoiseFilter.h
 *
 *    Description:  A Filter that removes spike noises
 *
 *        Version:  1.0
 *        Created:  09/14/2014 01:09:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Siavash Ameli
 *   Organization:  University Of California, Berkeley
 *
 * ============================================================================
 */

#ifndef __RemoveSpikeNoiseFilter_h
#define __RemoveSpikeNoiseFilter_h

// ====================
// Forward Declarations
// ====================

#include <vtkDataSetAlgorithm.h>

// =========================
// Remove Spike Noise Filter
// =========================

class RemoveSpikeNoiseFilter : public vtkDataSetAlgorithm
{
    public:
        static RemoveSpikeNoiseFilter * New();
        vtkTypeMacro(RemoveSpikeNoiseFilter,vtkDataSetAlgorithm);
        virtual void PrintSelf(ostream &os,vtkIndent Indent);

        // Accessors/Mutators
        vtkGetMacro(StencilSize,unsigned int);
        vtkSetMacro(StencilSize,unsigned int);



    protected:
        // Constructors
        RemoveSpikeNoiseFilter();
        virtual ~RemoveSpikeNoiseFilter();

        // Pipeline methods
        virtual int FillOutputPortInformation(
                int Port,
                vtkInformation *OutputInformation);

        virtual int RequestDataObject(
                vtkInformation *Request,
                vtkInformationVector **InputInformationVector,
                vtkInformationVector *OutputInformationVector);

        virtual int RequestData(
                vtkInformation *Request,
                vtkInformationVector **InputInformationVector,
                vtkInformationVector *OutputInformationVector);

        // Member Methods
        void RemoveSpikeNoise(
                vtkImageData *InputImageData,
                vtkImageData *OutputImageData);   // Output

        inline void GetPointIndex(
                int *GridResolution,
                int PointId,
                unsigned int *PointIndex);  // Output

        inline int GetPointId(
                int *GridResolution,
                int *PointIndex);

        inline void GetStencilIds(
                int *GridResolution,
                int PointId,
                unsigned int TargetDirection,
                int *StencilIds);  // Output

    private:
        RemoveSpikeNoiseFilter(const RemoveSpikeNoiseFilter &);  // Not implemented
        void operator=(const RemoveSpikeNoiseFilter &);          // Not implemented

        // Member Data
        unsigned int StencilSize;
};


#endif
