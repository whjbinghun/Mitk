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
#include "mitkToFConfig.h"
#include <pmdsdk2.h>

//Plugin defines for CamCube
#define SOURCE_PARAM ""
#define PROC_PARAM ""

extern PMDHandle m_PMDHandle; //TODO

extern PMDDataDescription m_DataDescription; //TODO


namespace mitk
{
  ToFCameraPMDCamCubeController::ToFCameraPMDCamCubeController()
  {
    m_SourcePlugin = MITK_TOF_PMDCAMCUBE_SOURCE_PLUGIN;
    m_SourceParam = SOURCE_PARAM;
    m_ProcPlugin = MITK_TOF_PMDCAMCUBE_PROCESSING_PLUGIN;
    m_ProcParam = PROC_PARAM;

    m_CaptureWidth  = 0;
    m_CaptureHeight = 0;
    m_PixelNumber = m_CaptureWidth * m_CaptureHeight;
    m_PMDRes = 0;
    m_ConnectionCheck = false;

    this->m_SourceDataSize = 0;
  }

  ToFCameraPMDCamCubeController::~ToFCameraPMDCamCubeController()
  {
  }

  bool ToFCameraPMDCamCubeController::OpenCameraConnection()
  {
    if(!m_ConnectionCheck)
    {
      m_PMDRes = pmdOpen (&m_PMDHandle , m_SourcePlugin , m_SourceParam , m_ProcPlugin , m_ProcParam );
      m_ConnectionCheck = ErrorText(m_PMDRes);
      if (!m_ConnectionCheck)
      {
        return m_ConnectionCheck;
      }

      // get image properties from camera
      this->UpdateCamera();
/*
      PMDDataDescription dd;
      m_PMDRes = pmdGetSourceDataDescription(m_PMDHandle, &dd);
*/
      m_PMDRes = pmdGetSourceDataDescription(m_PMDHandle, &m_DataDescription);
      ErrorText(m_PMDRes);
      m_CaptureWidth = m_DataDescription.img.numColumns;
      m_CaptureHeight = m_DataDescription.img.numRows;
      m_PixelNumber = m_CaptureWidth*m_CaptureHeight;
      m_NumberOfBytes = m_PixelNumber * sizeof(float);
      m_SourceDataSize = m_DataDescription.size;
      m_SourceDataStructSize = m_DataDescription.size + sizeof(PMDDataDescription);

      char serial[16];
      m_PMDRes = pmdSourceCommand (m_PMDHandle, serial, 16, "GetSerialNumber");
      ErrorText(m_PMDRes);

      MITK_INFO << "Serial-No: " << serial <<std::endl;
      MITK_INFO << "Datasource size: " << this->m_SourceDataSize <<std::endl;
      MITK_INFO << "Integration Time: " << this->GetIntegrationTime();
      MITK_INFO << "Modulation Frequence: " << this->GetModulationFrequency();

      return m_ConnectionCheck;
    }
    else return m_ConnectionCheck;
  }

  void mitk::ToFCameraPMDCamCubeController::SetDistanceOffset( float offset )
  {
    std::stringstream command;
    command<<"SetSoftOffset "<<offset;
    this->m_PMDRes = pmdSourceCommand(m_PMDHandle,0,0,command.str().c_str());
    ErrorText(this->m_PMDRes);
  }

  float mitk::ToFCameraPMDCamCubeController::GetDistanceOffset()
  {
    char offset[16];
    this->m_PMDRes = pmdSourceCommand(m_PMDHandle, offset, 16, "GetSoftOffset");
    ErrorText(this->m_PMDRes);
    return atof(offset);
  }

  void mitk::ToFCameraPMDCamCubeController::SetRegionOfInterest( unsigned int leftUpperCornerX, unsigned int leftUpperCornerY, unsigned int width, unsigned int height )
  {
    // check if leftUpperCornerX and width are divisible by 3 otherwise round to the next value divisible by 3
    unsigned int factor = leftUpperCornerX/3;
    leftUpperCornerX = 3*factor;
    factor = width/3;
    width = 3*factor;
    std::stringstream command;
    command<<"SetROI "<<leftUpperCornerX<<" "<<leftUpperCornerY<<" "<<width<<" "<<height;
    this->m_PMDRes = pmdSourceCommand(m_PMDHandle,0,0,command.str().c_str());
    ErrorText(this->m_PMDRes);
  }

  void mitk::ToFCameraPMDCamCubeController::SetRegionOfInterest( unsigned int roi[4] )
  {
    this->SetRegionOfInterest(roi[0],roi[1],roi[2],roi[3]);
  }

  unsigned int* mitk::ToFCameraPMDCamCubeController::GetRegionOfInterest()
  {
    /*
    char result[64];
    this->m_PMDRes = pmdSourceCommand(m_PMDHandle, result, 64, "GetROI");
    ErrorText(this->m_PMDRes);
    // convert char array to uint array
    unsigned int roi[4];
    std::stringstream currentValue;
    int counter = 0;
    std::string resultString = result;
    char blank = ' ';
    for (int i=0; i<64; i++)
    {
      if (result[i]!=blank)
      {
        currentValue<<result[i];
      }
      else
      {
        if (counter<4)
        {
          roi[counter] = atoi(currentValue.str().c_str());
          counter++;
          // empty the stream
          currentValue.str("");
          currentValue.clear();
        }
      }
    }
    return roi;
    */
    return NULL;
  }

  void mitk::ToFCameraPMDCamCubeController::SetExposureMode( int mode )
  {
    if (mode==0) // normal mode
    {
      this->m_PMDRes = pmdSourceCommand(m_PMDHandle, 0, 0, "SetExposureMode Normal");
      ErrorText(this->m_PMDRes);
    }
    else if (mode==1) // SMB mode
    {
      this->m_PMDRes = pmdSourceCommand(m_PMDHandle, 0, 0, "SetExposureMode SMB");
      ErrorText(this->m_PMDRes);
    }
  }

  void mitk::ToFCameraPMDCamCubeController::SetFieldOfView( float fov )
  {
    std::stringstream commandStream;
    commandStream<<"SetFOV "<<fov;
    this->m_PMDRes = pmdProcessingCommand(m_PMDHandle, 0, 0, commandStream.str().c_str());
    ErrorText(this->m_PMDRes);
  }

  void mitk::ToFCameraPMDCamCubeController::SetFPNCalibration( bool on )
  {
    if(on)
    {
      this->m_PMDRes=pmdSourceCommand(m_PMDHandle,0,0,"SetFPNCalibration On");
      this->ErrorText(this->m_PMDRes);
    }
    else
    {
      this->m_PMDRes=pmdSourceCommand(m_PMDHandle,0,0,"SetFPNCalibration Off");
      this->ErrorText(this->m_PMDRes);
    }
  }

  void mitk::ToFCameraPMDCamCubeController::SetFPPNCalibration( bool on )
  {
    if(on)
    {
      this->m_PMDRes=pmdProcessingCommand(m_PMDHandle,0,0,"SetFPPNCalibration On");
      this->ErrorText(this->m_PMDRes);
    }
    else
    {
      this->m_PMDRes=pmdProcessingCommand(m_PMDHandle,0,0,"SetFPPNCalibration Off");
      this->ErrorText(this->m_PMDRes);
    }
  }

  void mitk::ToFCameraPMDCamCubeController::SetLinearityCalibration( bool on )
  {
    if(on)
    {
      this->m_PMDRes=pmdProcessingCommand(m_PMDHandle,0,0,"SetLinearityCalibration On");
      this->ErrorText(this->m_PMDRes);
    }
    else
    {
      this->m_PMDRes=pmdProcessingCommand(m_PMDHandle,0,0,"SetLinearityCalibration Off");
      this->ErrorText(this->m_PMDRes);
    }
  }

  void mitk::ToFCameraPMDCamCubeController::SetLensCalibration( bool on )
  {
    if (on)
    {
      this->m_PMDRes = pmdProcessingCommand(m_PMDHandle, 0, 0, "SetLensCalibration On");
      ErrorText(this->m_PMDRes);
    }
    else
    {
      this->m_PMDRes = pmdProcessingCommand(m_PMDHandle, 0, 0, "SetLensCalibration Off");
      ErrorText(this->m_PMDRes);
    }
  }

}
