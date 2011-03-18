/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date: 2010-05-27 16:06:53 +0200 (Do, 27 Mai 2010) $
Version:   $Revision: $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "mitkToFCameraPMDCamCubeController.h"

namespace mitk
{
  ToFCameraPMDCamCubeController::ToFCameraPMDCamCubeController()
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
  }

  ToFCameraPMDCamCubeController::~ToFCameraPMDCamCubeController()
  {
  }

  bool ToFCameraPMDCamCubeController::OpenCameraConnection()
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
    return false;
  }

  void mitk::ToFCameraPMDCamCubeController::SetDistanceOffset( float offset )
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
  }

  float mitk::ToFCameraPMDCamCubeController::GetDistanceOffset()
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
    return 0.0f;
  }

  void mitk::ToFCameraPMDCamCubeController::SetRegionOfInterest( unsigned int leftUpperCornerX, unsigned int leftUpperCornerY, unsigned int width, unsigned int height )
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
  }

  void mitk::ToFCameraPMDCamCubeController::SetRegionOfInterest( unsigned int roi[4] )
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
  }

  unsigned int* mitk::ToFCameraPMDCamCubeController::GetRegionOfInterest()
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
    return NULL;
  }

  void mitk::ToFCameraPMDCamCubeController::SetExposureMode( int mode )
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
  }

  void mitk::ToFCameraPMDCamCubeController::SetFieldOfView( float fov )
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
  }

  void mitk::ToFCameraPMDCamCubeController::SetFPNCalibration( bool on )
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
  }

  void mitk::ToFCameraPMDCamCubeController::SetFPPNCalibration( bool on )
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
  }

  void mitk::ToFCameraPMDCamCubeController::SetLinearityCalibration( bool on )
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
  }

  void mitk::ToFCameraPMDCamCubeController::SetLensCalibration( bool on )
  {
    MITK_WARN("ToF") << "Error: PMD Camcube currently not available";
  }
}
