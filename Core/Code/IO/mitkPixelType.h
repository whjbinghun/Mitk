/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef PIXELTYPE_H_HEADER_INCLUDED_C1EBF565
#define PIXELTYPE_H_HEADER_INCLUDED_C1EBF565

#include <MitkExports.h>
#include "mitkCommon.h"
#include "mitkPixelTypeTraits.h"

#include <typeinfo>
#include <string>

#include <itkImageIOBase.h>
#include <itkImage.h>

namespace mitk {

template< typename ComponentT>
const char* ComponentTypeToString()
{
  return typeid(ComponentT).name();
}

template<typename PixelT>
const char* PixelTypeToString()
{
  return typeid(PixelT).name();
}


//##Documentation
//## @brief Class for defining the data type of pixels 
//##
//## To obtain additional type information not provided by this class
//## itk::ImageIOBase can be used by passing the return value of 
//## PixelType::GetItkTypeId() to itk::ImageIOBase::SetPixelTypeInfo
//## and using the itk::ImageIOBase methods GetComponentType,
//## GetComponentTypeAsString, GetPixelType, GetPixelTypeAsString.
//## @ingroup Data
class MITK_CORE_EXPORT PixelType
{
public:
  itkTypeMacro(PixelType, None);

  typedef itk::ImageIOBase::IOPixelType ItkIOPixelType;

  PixelType(void);

  PixelType(const mitk::PixelType & aPixelType);

  /** \brief Get the \a type_info of the scalar (!) type. Each element
  * may contain m_NumberOfComponents (more than one) of these scalars.
  *
  * \sa GetItkTypeId
  */
  inline const std::type_info& GetTypeId() const
  {
    return m_ComponentType;
  }

  inline const std::type_info& GetPixelTypeId() const
  {
    return m_PixelType;
  }

  std::string GetItkTypeAsString() const
  {
    return m_PixelTypeName;
  }

  std::string GetComponentTypeAsString() const
  {
      return m_ComponentTypeName;
  }

  /** \brief Get the number of bits per element (of an
  * element)
  *
  * A vector of double with three components will return
  * 8*sizeof(double)*3.
  * \sa GetBitsPerComponent
  * \sa GetItkTypeId
  * \sa GetTypeId
  */
  size_t GetSize() const
  {
    return (m_NumberOfComponents * m_BytesPerComponent);
  }

  size_t GetBpe() const
  {
    return this->GetSize() * 8;
  }

  /** \brief Get the number of components of which each element consists
  *
  * Each pixel can consist of multiple components, e.g. RGB.
  */
  inline int GetNumberOfComponents() const
  {
    return m_NumberOfComponents;    
  }
  
  /** \brief Get the number of bits per components
  * \sa GetBitsPerComponent
  */
  inline int GetBitsPerComponent() const
  {
    return m_BytesPerComponent * 8;
  }
  
  PixelType& operator=(const PixelType& other);

  bool operator==(const PixelType& rhs) const;
  bool operator!=(const PixelType& rhs) const;

  bool operator==(const std::type_info& typeId) const;
  bool operator!=(const std::type_info& typeId) const;

  void Initialize(const std::type_info& aTypeId, int numberOfCompontents = 1, ItkIOPixelType anItkIoPixelType = itk::ImageIOBase::UNKNOWNPIXELTYPE);

  virtual ~PixelType() {}

private:

  friend class ItkImageFileReader;

  template <typename TPixelType>
  friend class NrrdTbssImageReader;

  template< typename ComponentT, typename PixelT, std::size_t numberOfComponents >
    friend PixelType MakePixelType();

  template< typename ItkImageType >
    friend PixelType MakePixelType();

  PixelType( const std::type_info& componentType,
             const std::type_info& pixelType,
             std::size_t bytesPerComponent,
             std::size_t numberOfComponents,
             const char* componentTypeName = 0,
             const char* pixelTypeName = 0 )
    : m_ComponentType( componentType ),
      m_PixelType( pixelType ),
      m_NumberOfComponents( numberOfComponents ),
      m_BytesPerComponent( bytesPerComponent )
  {
    if(componentTypeName) m_ComponentTypeName = componentTypeName;
    else m_ComponentTypeName = componentType.name();

    if(pixelTypeName) m_PixelTypeName = pixelTypeName;
    else m_PixelTypeName = pixelType.name();
  }

  /** \brief the \a type_info of the scalar (!) component type. Each element
    may contain m_NumberOfComponents (more than one) of these scalars.
  */
  const std::type_info& m_ComponentType;

  const std::type_info& m_PixelType;

  std::string m_ComponentTypeName;

  std::string m_PixelTypeName;
  
  std::size_t m_NumberOfComponents;

  std::size_t m_BytesPerComponent;

};

template< typename ComponentT, typename PixelT, std::size_t numOfComponents >
PixelType MakePixelType()
{
  typedef itk::Image< PixelT, numOfComponents> ItkImageType;

  return PixelType( typeid(ComponentT), typeid(ItkImageType),
                    sizeof(ComponentT), numOfComponents,
                    ComponentTypeToString<ComponentT>(),
                    PixelTypeToString<ItkImageType>()
                   );
}


template< typename ItkImageType >
PixelType MakePixelType()
{
  // define new type, since the ::PixelType is used to distinguish between simple and compound types
  typedef typename ItkImageType::PixelType ImportPixelType;

  // get the component type ( is either directly ImportPixelType or ImportPixelType::ValueType for compound types )
  typedef typename GetComponentType<ImportPixelType>::ComponentType ComponentT;

  // FIXME The PixelType is the same as the ComponentT for simple types
  typedef typename ItkImageType::PixelType PixelT;

  // Get the length of compound type ( initialized to 1 for simple types and variable-length vector images)
  size_t numComp = ComponentsTrait<
    (isPrimitiveType<PixelT>::value || isVectorImage<PixelT, ComponentT>::value), ItkImageType >::Size;

  // call the constructor
  return PixelType(
            typeid(ComponentT), typeid(ItkImageType),
            sizeof(ComponentT), numComp,
            ComponentTypeToString<ComponentT>(),
            PixelTypeToString<PixelT >()
         );
}


template< typename T>
PixelType MakeSimpleType()
{
    return MakePixelType<T,T,1>();
}

} // namespace mitk

#endif /* PIXELTYPE_H_HEADER_INCLUDED_C1EBF565 */
