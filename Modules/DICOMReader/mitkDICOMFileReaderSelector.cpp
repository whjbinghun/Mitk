/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "mitkDICOMFileReaderSelector.h"
#include "mitkDICOMReaderConfigurator.h"

#include "mitkModuleContext.h"

#include <usGetModuleContext.h>
#include <usModuleResource.h>
#include <usModuleResourceStream.h>
#include <usModule.h>

mitk::DICOMFileReaderSelector
::DICOMFileReaderSelector()
{
}

mitk::DICOMFileReaderSelector
::~DICOMFileReaderSelector()
{
}

std::list<mitk::DICOMFileReader::Pointer>
mitk::DICOMFileReaderSelector
::GetAllConfiguredReaders() const
{
  return m_Readers;
}

void
mitk::DICOMFileReaderSelector
::AddConfigsFromResources(const std::string& path)
{
  std::vector<ModuleResource> configs = GetModuleContext()->GetModule()->FindResources(path, "*.xml", false);

  for (std::vector<ModuleResource>::iterator iter = configs.begin();
       iter != configs.end();
       ++iter)
  {
    ModuleResource& resource = *iter;
    if (resource.IsValid())
    {
      ModuleResourceStream stream(resource);

      // read all into string s
      std::string s;

      stream.seekg(0, std::ios::end);
      s.reserve(stream.tellg());
      stream.seekg(0, std::ios::beg);

      s.assign((std::istreambuf_iterator<char>(stream)),
                std::istreambuf_iterator<char>());

      this->AddConfig(s);
    }
  }
}

void
mitk::DICOMFileReaderSelector
::AddConfigFromResource(ModuleResource& resource)
{
  if (resource.IsValid())
  {
    ModuleResourceStream stream(resource);

    // read all into string s
    std::string s;

    stream.seekg(0, std::ios::end);
    s.reserve(stream.tellg());
    stream.seekg(0, std::ios::beg);

    s.assign((std::istreambuf_iterator<char>(stream)),
              std::istreambuf_iterator<char>());

    this->AddConfig(s);
  }
}

void
mitk::DICOMFileReaderSelector
::AddConfigFromResource(const std::string& resourcename)
{
  ModuleResource r = GetModuleContext()->GetModule()->GetResource(resourcename);
  this->AddConfigFromResource(r);
}

void
mitk::DICOMFileReaderSelector
::AddFileReaderCanditate(DICOMFileReader::Pointer reader)
{
  if (reader.IsNotNull())
  {
    m_Readers.push_back( reader );
  }
}

void
mitk::DICOMFileReaderSelector
::LoadBuiltIn3DConfigs()
{
  //this->AddConfigsFromResources("configurations/3D");
  // in this order of preference...
  this->AddConfigFromResource("configurations/3D/instancenumber.xml");
  this->AddConfigFromResource("configurations/3D/slicelocation.xml");
  this->AddConfigFromResource("configurations/3D/imageposition.xml");
  this->AddConfigFromResource("configurations/3D/imageposition_byacquisition.xml");
  //this->AddConfigFromResource("configurations/3D/imagetime.xml"); // no sense in this one? want to see a real-world example first
  this->AddConfigFromResource("configurations/3D/classicreader.xml"); // currently is 3D+t actually
}

void
mitk::DICOMFileReaderSelector
::LoadBuiltIn3DnTConfigs()
{
  this->AddConfigsFromResources("configurations/3DnT");
}

void
mitk::DICOMFileReaderSelector
::AddConfig(const std::string& xmlDescription)
{
  DICOMReaderConfigurator::Pointer configurator = DICOMReaderConfigurator::New();
  DICOMFileReader::Pointer reader = configurator->CreateFromUTF8ConfigString(xmlDescription);

  if (reader.IsNotNull())
  {
    m_Readers.push_back( reader );
    m_PossibleConfigurations.push_back(xmlDescription);
  }
  else
  {
    std::stringstream ss;
    ss << "Could not parse reader configuration. Ignoring it.";
    throw std::invalid_argument( ss.str() );
  }
}

void
mitk::DICOMFileReaderSelector
::AddConfigFile(const std::string& filename)
{
  std::ifstream file(filename.c_str());
  std::string s;

  file.seekg(0, std::ios::end);
  s.reserve(file.tellg());
  file.seekg(0, std::ios::beg);

  s.assign((std::istreambuf_iterator<char>(file)),
              std::istreambuf_iterator<char>());

  this->AddConfig(s);
}

void
mitk::DICOMFileReaderSelector
::SetInputFiles(StringList filenames)
{
  m_InputFilenames = filenames;
}

const mitk::StringList&
mitk::DICOMFileReaderSelector
::GetInputFiles() const
{
  return m_InputFilenames;
}

mitk::DICOMFileReader::Pointer
mitk::DICOMFileReaderSelector
::GetFirstReaderWithMinimumNumberOfOutputImages()
{
  ReaderList workingCandidates;

  // let all readers analyze the file set
  unsigned int readerIndex(0);
  for (ReaderList::iterator rIter = m_Readers.begin();
       rIter != m_Readers.end();
       ++readerIndex, ++rIter)
  {
    (*rIter)->SetInputFiles( m_InputFilenames );
    try
    {
      (*rIter)->AnalyzeInputFiles();
      workingCandidates.push_back( *rIter );
      MITK_INFO << "Reader " << readerIndex << " (" << (*rIter)->GetConfigurationLabel() << ") suggests " << (*rIter)->GetNumberOfOutputs() << " 3D blocks";
      if ((*rIter)->GetNumberOfOutputs() == 1)
      {
        MITK_DEBUG << "Early out with reader #" << readerIndex << " (" << (*rIter)->GetConfigurationLabel() << "), less than 1 block is not possible";
        return *rIter;
      }
    }
    catch (std::exception& e)
    {
      MITK_ERROR << "Reader " << readerIndex << " (" << (*rIter)->GetConfigurationLabel() << ") threw exception during file analysis, ignoring this reader. Exception: " << e.what();
    }
    catch (...)
    {
      MITK_ERROR << "Reader " << readerIndex << " (" << (*rIter)->GetConfigurationLabel() << ") threw unknown exception during file analysis, ignoring this reader.";
    }
  }

  DICOMFileReader::Pointer bestReader;

  unsigned int minimumNumberOfOutputs = std::numeric_limits<unsigned int>::max();
  readerIndex = 0;
  unsigned int bestReaderIndex(0);
  // select the reader with the minimum number of mitk::Images as output
  for (ReaderList::iterator rIter = workingCandidates.begin();
       rIter != workingCandidates.end();
       ++readerIndex, ++rIter)
  {
    unsigned int thisReadersNumberOfOutputs = (*rIter)->GetNumberOfOutputs();
    if (   thisReadersNumberOfOutputs > 0  // we don't count readers that don't actually produce output
        && thisReadersNumberOfOutputs < minimumNumberOfOutputs )
    {
      minimumNumberOfOutputs = (*rIter)->GetNumberOfOutputs();
      bestReader = *rIter;
      bestReaderIndex = readerIndex;
    }
  }

  MITK_DEBUG << "Decided for reader #" << bestReaderIndex << " (" << bestReader->GetConfigurationLabel() << ")";
  MITK_DEBUG << m_PossibleConfigurations[bestReaderIndex];

  return bestReader;
}
