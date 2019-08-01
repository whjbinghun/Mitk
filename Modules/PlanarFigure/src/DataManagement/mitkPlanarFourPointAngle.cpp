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


#include "mitkPlanarFourPointAngle.h"
#include "mitkPlaneGeometry.h"


mitk::PlanarFourPointAngle::PlanarFourPointAngle()
: FEATURE_ID_ANGLE( this->AddFeature( "Angle", "deg" ) )
{
  // Four point angle has two control points
  this->ResetNumberOfControlPoints( 2 );
  this->SetNumberOfPolyLines( 2 );
}

void mitk::PlanarFourPointAngle::confirmMidpoint()
{
	if (this->GetNumberOfControlPoints() < 4)
	{
		// Angle not yet complete.
		return;
	}
	//»ñµÃÁ½ÌõÏß
	const Point2D &p0 = this->GetControlPoint(0);
	const Point2D &p1 = this->GetControlPoint(1);
	const Point2D &p2 = this->GetControlPoint(2);
	const Point2D &p3 = this->GetControlPoint(3);

	//Á½¸öÖÐµã
	Point2D mid1, mid2;
	mid1[0] = (p0[0] + p1[0])*0.5f;
	mid1[1] = (p0[1] + p1[1])*0.5f;

	mid2[0] = (p2[0] + p3[0])*0.5f;
	mid2[1] = (p2[1] + p3[1])*0.5f;

	this->ClearPolyLines();
	AppendPointToPolyLine(0, mid1);
	AppendPointToPolyLine(0, mid2);
}

void mitk::PlanarFourPointAngle::GeneratePolyLine()
{
  this->ClearPolyLines();

  for (unsigned int i = 0; i < this->GetNumberOfControlPoints(); ++i)
    this->AppendPointToPolyLine(i / 2, this->GetControlPoint(i));
  if (this->GetNumberOfControlPoints() == 4)
	  confirmMidpoint();
}

void mitk::PlanarFourPointAngle::GenerateHelperPolyLine(double /*mmPerDisplayUnit*/, unsigned int /*displayHeight*/)
{
  // Generate helper-poly-line for an four point angle
  // Need to discuss a sensible implementation
}

void mitk::PlanarFourPointAngle::EvaluateFeaturesInternal()
{
  if ( this->GetNumberOfControlPoints() < 4 )
  {
    // Angle not yet complete.
    return;
  }

  // Calculate angle between lines
  const Point2D &p0 = this->GetControlPoint( 0 );
  const Point2D &p1 = this->GetControlPoint( 1 );
  const Point2D &p2 = this->GetControlPoint( 2 );
  const Point2D &p3 = this->GetControlPoint( 3 );

  Vector2D v0 = p1 - p0;
  Vector2D v1 = p3 - p2;

  v0.Normalize();
  v1.Normalize();
  double angle = acos( v0 * v1 );

  this->SetQuantity( FEATURE_ID_ANGLE, angle );
}


void mitk::PlanarFourPointAngle::PrintSelf( std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf( os, indent );

}

 bool mitk::PlanarFourPointAngle::Equals(const mitk::PlanarFigure& other) const
 {
   const mitk::PlanarFourPointAngle* otherFourPtAngle = dynamic_cast<const mitk::PlanarFourPointAngle*>(&other);
   if ( otherFourPtAngle )
   {
     return Superclass::Equals(other);
   }
   else
   {
     return false;
   }
 }
