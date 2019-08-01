﻿/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "QmitkImageNavigatorView.h"

#include <QmitkStepperAdapter.h>
#include <QmitkRenderWindow.h>

#include <mitkTimeGeometry.h>

#include <berryConstants.h>
#include <mitkPlaneGeometry.h>

#include "QmitkStdMultiWidget.h"
#include <mitkILinkedRenderWindowPart.h>
#include <berryIWorkbenchPage.h>
const std::string QmitkImageNavigatorView::VIEW_ID = "org.mitk.views.imagenavigator";


QmitkImageNavigatorView::QmitkImageNavigatorView()
  : m_AxialStepper(0)
  , m_SagittalStepper(0)
  , m_FrontalStepper(0)
  , m_TimeStepper(0)
  , m_Parent(0)
  , m_IRenderWindowPart(0)
{
	bInit_ = false;
}

QmitkImageNavigatorView::~QmitkImageNavigatorView()
{
}
void QmitkImageNavigatorView::OnPreprocessing()
{
	MITK_INFO("org.mitk.views.imagenavigator") << "Preprocessing";
// 	m_Controls.leftBtn->setChecked(true);
// 	m_Controls.leftBtn->setEnabled(true);
// 	m_Controls.rightBtn->setEnabled(true);
// 	m_Controls.refLineBtn->setEnabled(true);
// 	m_Controls.shaftNeckBtn->setEnabled(true);
// 	m_Controls.stemAxisBtn->setEnabled(true);
// 	m_Controls.diffBtn->setEnabled(true);

	//¼¤»îgraphcut view
	QString view_id = "org.mitk.views.imagegraphcut3dsegmentation";
	this->GetSite()->GetPage()->ShowView(view_id);

	//ÔÝÍ£ÏìÓ¦crosshair²Ù×÷
// 	mitk::ILinkedRenderWindowPart* linkedRenderWindowPart = dynamic_cast<mitk::ILinkedRenderWindowPart*>(this->GetRenderWindowPart());
// 	if (linkedRenderWindowPart != NULL)
// 		linkedRenderWindowPart->EnableSlicingPlanes(false);
	if (QmitkStdMultiWidget::app())
	{
		if (!bInit_)
		{
			QmitkStdMultiWidget::app()->navigator = this;

			connect(m_Controls.preprocessingBtn, SIGNAL(clicked()),
				QmitkStdMultiWidget::app(), SLOT(turnOnDDR()));

			/*connect(m_Controls.rightBtn, SIGNAL(clicked()),
				QmitkStdMultiWidget::app(), SLOT(rightPart()));
			connect(m_Controls.leftBtn, SIGNAL(clicked()),
				QmitkStdMultiWidget::app(), SLOT(leftPart()));

			connect(m_Controls.refLineBtn, SIGNAL(clicked()),
				QmitkStdMultiWidget::app(), SLOT(refLine()));*/

			connect(m_Controls.planBtn, SIGNAL(clicked()),
				QmitkStdMultiWidget::app(), SLOT(planning()));
		}
	}
}
void QmitkImageNavigatorView::OnPlan()
{
	MITK_INFO("org.mitk.views.imagenavigator") << "Plan";
	mitk::DataStorage::Pointer ds = this->GetDataStorage();
	ds->bEmulationMode = false;
	//¼¤»îGeometryTools view
	QString view_id = "org.mitk.views.geometrytools";
	this->GetSite()->GetPage()->ShowView(view_id);

}
void QmitkImageNavigatorView::OnEmulation()
{
	MITK_INFO("org.mitk.views.imagenavigator") << "Emulation";

// 	mitk::RenderingManager::GetInstance()->bEmulationMode = true;
	mitk::DataStorage::Pointer ds = this->GetDataStorage();
	ds->bEmulationMode = true;
	//¼¤»îGeometryTools view
	QString view_id = "org.mitk.views.geometrytools";
	this->GetSite()->GetPage()->ShowView(view_id);
}

void QmitkImageNavigatorView::CreateQtPartControl(QWidget *parent)
{
  // create GUI widgets
  m_Parent = parent;
  m_Controls.setupUi(parent);
  m_Controls.m_SliceNavigatorAxial->SetInverseDirection(true);

  connect(m_Controls.m_XWorldCoordinateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnMillimetreCoordinateValueChanged()));
  connect(m_Controls.m_YWorldCoordinateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnMillimetreCoordinateValueChanged()));
  connect(m_Controls.m_ZWorldCoordinateSpinBox, SIGNAL(valueChanged(double)), this, SLOT(OnMillimetreCoordinateValueChanged()));

  m_Parent->setEnabled(false);

  mitk::IRenderWindowPart* renderPart = this->GetRenderWindowPart();
  this->RenderWindowPartActivated(renderPart);

  m_Controls.m_TimeLabel->setVisible(false);
  m_Controls.m_SliceNavigatorTime->setVisible(false);

  connect(m_Controls.preprocessingBtn, SIGNAL(clicked()), this, SLOT(OnPreprocessing()));
  connect(m_Controls.planBtn, SIGNAL(clicked()), this, SLOT(OnPlan()));
  connect(m_Controls.emulationBtn, SIGNAL(clicked()), this, SLOT(OnEmulation()));

  if (QmitkStdMultiWidget::app())
  {
	  QmitkStdMultiWidget::app()->navigator = this;

	  connect(m_Controls.preprocessingBtn, SIGNAL(clicked()),
		  QmitkStdMultiWidget::app(), SLOT(turnOnDDR()));

	  /*connect(m_Controls.rightBtn, SIGNAL(clicked()),
		  QmitkStdMultiWidget::app(), SLOT(rightPart()));
	  connect(m_Controls.leftBtn, SIGNAL(clicked()),
		  QmitkStdMultiWidget::app(), SLOT(leftPart()));

	  connect(m_Controls.refLineBtn, SIGNAL(clicked()),
		  QmitkStdMultiWidget::app(), SLOT(refLine()));*/

	  connect(m_Controls.planBtn, SIGNAL(clicked()),
		  QmitkStdMultiWidget::app(), SLOT(planning()));

	  bInit_ = true;
  }
}

void QmitkImageNavigatorView::SetFocus ()
{
  m_Controls.m_XWorldCoordinateSpinBox->setFocus();
}

void QmitkImageNavigatorView::RenderWindowPartActivated(mitk::IRenderWindowPart* renderWindowPart)
{
  if (this->m_IRenderWindowPart != renderWindowPart)
  {
    this->m_IRenderWindowPart = renderWindowPart;
    this->m_Parent->setEnabled(true);

    QmitkRenderWindow* renderWindow = renderWindowPart->GetQmitkRenderWindow("axial");
    if (renderWindow)
    {
      if (m_AxialStepper) m_AxialStepper->deleteLater();
      m_AxialStepper = new QmitkStepperAdapter(m_Controls.m_SliceNavigatorAxial,
                                                     renderWindow->GetSliceNavigationController()->GetSlice(),
                                                     "sliceNavigatorAxialFromSimpleExample");
      m_Controls.m_SliceNavigatorAxial->setEnabled(true);
      m_Controls.m_AxialLabel->setEnabled(true);
      m_Controls.m_ZWorldCoordinateSpinBox->setEnabled(true);
      connect(m_AxialStepper, SIGNAL(Refetch()), this, SLOT(OnRefetch()));
    }
    else
    {
      m_Controls.m_SliceNavigatorAxial->setEnabled(false);
      m_Controls.m_AxialLabel->setEnabled(false);
      m_Controls.m_ZWorldCoordinateSpinBox->setEnabled(false);
    }

    renderWindow = renderWindowPart->GetQmitkRenderWindow("sagittal");
    if (renderWindow)
    {
      if (m_SagittalStepper) m_SagittalStepper->deleteLater();
      m_SagittalStepper = new QmitkStepperAdapter(m_Controls.m_SliceNavigatorSagittal,
                                                  renderWindow->GetSliceNavigationController()->GetSlice(),
                                                  "sliceNavigatorSagittalFromSimpleExample");
      m_Controls.m_SliceNavigatorSagittal->setEnabled(true);
      m_Controls.m_SagittalLabel->setEnabled(true);
      m_Controls.m_YWorldCoordinateSpinBox->setEnabled(true);
      connect(m_SagittalStepper, SIGNAL(Refetch()), this, SLOT(OnRefetch()));
    }
    else
    {
      m_Controls.m_SliceNavigatorSagittal->setEnabled(false);
      m_Controls.m_SagittalLabel->setEnabled(false);
      m_Controls.m_YWorldCoordinateSpinBox->setEnabled(false);
    }

    renderWindow = renderWindowPart->GetQmitkRenderWindow("coronal");
    if (renderWindow)
    {
      if (m_FrontalStepper) m_FrontalStepper->deleteLater();
      m_FrontalStepper = new QmitkStepperAdapter(m_Controls.m_SliceNavigatorFrontal,
                                                 renderWindow->GetSliceNavigationController()->GetSlice(),
                                                 "sliceNavigatorFrontalFromSimpleExample");
      m_Controls.m_SliceNavigatorFrontal->setEnabled(true);
      m_Controls.m_CoronalLabel->setEnabled(true);
      m_Controls.m_XWorldCoordinateSpinBox->setEnabled(true);
      connect(m_FrontalStepper, SIGNAL(Refetch()), this, SLOT(OnRefetch()));
    }
    else
    {
      m_Controls.m_SliceNavigatorFrontal->setEnabled(false);
      m_Controls.m_CoronalLabel->setEnabled(false);
      m_Controls.m_XWorldCoordinateSpinBox->setEnabled(false);
    }

    mitk::SliceNavigationController* timeController = renderWindowPart->GetTimeNavigationController();
    if (timeController)
    {
      if (m_TimeStepper) m_TimeStepper->deleteLater();
      m_TimeStepper = new QmitkStepperAdapter(m_Controls.m_SliceNavigatorTime,
                                              timeController->GetTime(),
                                              "sliceNavigatorTimeFromSimpleExample");
      m_Controls.m_SliceNavigatorTime->setEnabled(true);
      m_Controls.m_TimeLabel->setEnabled(true);
    }
    else
    {
      m_Controls.m_SliceNavigatorTime->setEnabled(false);
      m_Controls.m_TimeLabel->setEnabled(false);
    }
  }
}

void QmitkImageNavigatorView::RenderWindowPartDeactivated(mitk::IRenderWindowPart* /*renderWindowPart*/)
{
  m_IRenderWindowPart = 0;
  m_Parent->setEnabled(false);
}

int QmitkImageNavigatorView::GetSizeFlags(bool width)
{
  if(!width)
  {
    return berry::Constants::MIN | berry::Constants::MAX | berry::Constants::FILL;
  }
  else
  {
    return 0;
  }
}

int QmitkImageNavigatorView::ComputePreferredSize(bool width, int /*availableParallel*/, int /*availablePerpendicular*/, int preferredResult)
{
  if(width==false)
  {
    return 200;
  }
  else
  {
    return preferredResult;
  }
}

int QmitkImageNavigatorView::GetClosestAxisIndex(mitk::Vector3D normal)
{
  // cos(theta) = normal . axis
  // cos(theta) = (a, b, c) . (d, e, f)
  // cos(theta) = (a, b, c) . (1, 0, 0) = a
  // cos(theta) = (a, b, c) . (0, 1, 0) = b
  // cos(theta) = (a, b, c) . (0, 0, 1) = c
  double absCosThetaWithAxis[3];

  for (int i = 0; i < 3; i++)
  {
    absCosThetaWithAxis[i] = fabs(normal[i]);
  }
  int largestIndex = 0;
  double largestValue = absCosThetaWithAxis[0];
  for (int i = 1; i < 3; i++)
  {
    if (absCosThetaWithAxis[i] > largestValue)
    {
      largestValue = absCosThetaWithAxis[i];
      largestIndex = i;
    }
  }
  return largestIndex;
}

void QmitkImageNavigatorView::SetBorderColors()
{
  if (m_IRenderWindowPart)
  {
    QString decoColor;

    QmitkRenderWindow* renderWindow = m_IRenderWindowPart->GetQmitkRenderWindow("axial");
    if (renderWindow)
    {
      decoColor = GetDecorationColorOfGeometry(renderWindow);
      mitk::PlaneGeometry::ConstPointer geometry = renderWindow->GetSliceNavigationController()->GetCurrentPlaneGeometry();
      if (geometry.IsNotNull())
      {
        mitk::Vector3D normal = geometry->GetNormal();
        int axis = this->GetClosestAxisIndex(normal);
        this->SetBorderColor(axis, decoColor);
      }
    }

    renderWindow = m_IRenderWindowPart->GetQmitkRenderWindow("sagittal");
    if (renderWindow)
    {
      decoColor = GetDecorationColorOfGeometry(renderWindow);
      mitk::PlaneGeometry::ConstPointer geometry = renderWindow->GetSliceNavigationController()->GetCurrentPlaneGeometry();
      if (geometry.IsNotNull())
      {
        mitk::Vector3D normal = geometry->GetNormal();
        int axis = this->GetClosestAxisIndex(normal);
        this->SetBorderColor(axis, decoColor);
      }
    }

    renderWindow = m_IRenderWindowPart->GetQmitkRenderWindow("coronal");
    if (renderWindow)
    {
      decoColor = GetDecorationColorOfGeometry(renderWindow);
      mitk::PlaneGeometry::ConstPointer geometry = renderWindow->GetSliceNavigationController()->GetCurrentPlaneGeometry();
      if (geometry.IsNotNull())
      {
        mitk::Vector3D normal = geometry->GetNormal();
        int axis = this->GetClosestAxisIndex(normal);
        this->SetBorderColor(axis, decoColor);
      }
    }
  }
}

QString QmitkImageNavigatorView::GetDecorationColorOfGeometry(QmitkRenderWindow* renderWindow)
{
  QColor color;
  float rgb[3] = {1.0f, 1.0f, 1.0f};
  float rgbMax = 255.0f;
  mitk::BaseRenderer::GetInstance(renderWindow->GetVtkRenderWindow())->GetCurrentWorldPlaneGeometryNode()->GetColor(rgb);
  color.setRed(static_cast<int>(rgb[0]*rgbMax + 0.5));
  color.setGreen(static_cast<int>(rgb[1]*rgbMax + 0.5));
  color.setBlue(static_cast<int>(rgb[2]*rgbMax + 0.5));
  QString colorAsString = QString(color.name());
  return colorAsString;
}

void QmitkImageNavigatorView::SetBorderColor(int axis, QString colorAsStyleSheetString)
{
  if (axis == 0)
  {
    this->SetBorderColor(m_Controls.m_XWorldCoordinateSpinBox, colorAsStyleSheetString);
  }
  else if (axis == 1)
  {
    this->SetBorderColor(m_Controls.m_YWorldCoordinateSpinBox, colorAsStyleSheetString);
  }
  else if (axis == 2)
  {
    this->SetBorderColor(m_Controls.m_ZWorldCoordinateSpinBox, colorAsStyleSheetString);
  }
}

void QmitkImageNavigatorView::SetBorderColor(QDoubleSpinBox *spinBox, QString colorAsStyleSheetString)
{
  assert(spinBox);
  spinBox->setStyleSheet(QString("border: 2px solid ") + colorAsStyleSheetString + ";");
}

void QmitkImageNavigatorView::SetStepSizes()
{
  this->SetStepSize(0);
  this->SetStepSize(1);
  this->SetStepSize(2);
}

void QmitkImageNavigatorView::SetStepSize(int axis)
{
  if (m_IRenderWindowPart)
  {
    mitk::BaseGeometry::ConstPointer geometry = m_IRenderWindowPart->GetActiveQmitkRenderWindow()->GetSliceNavigationController()->GetInputWorldGeometry3D();

    if (geometry.IsNotNull())
    {
      mitk::Point3D crossPositionInIndexCoordinates;
      mitk::Point3D crossPositionInIndexCoordinatesPlus1;
      mitk::Point3D crossPositionInMillimetresPlus1;
      mitk::Vector3D transformedAxisDirection;

      mitk::Point3D crossPositionInMillimetres = m_IRenderWindowPart->GetSelectedPosition();
      geometry->WorldToIndex(crossPositionInMillimetres, crossPositionInIndexCoordinates);

      crossPositionInIndexCoordinatesPlus1 = crossPositionInIndexCoordinates;
      crossPositionInIndexCoordinatesPlus1[axis] += 1;

      geometry->IndexToWorld(crossPositionInIndexCoordinatesPlus1, crossPositionInMillimetresPlus1);

      transformedAxisDirection = crossPositionInMillimetresPlus1 - crossPositionInMillimetres;

      int closestAxisInMillimetreSpace = this->GetClosestAxisIndex(transformedAxisDirection);
      double stepSize = transformedAxisDirection.GetNorm();
      this->SetStepSize(closestAxisInMillimetreSpace, stepSize);
    }
  }
}

void QmitkImageNavigatorView::SetStepSize(int axis, double stepSize)
{
  if (axis == 0)
  {
    m_Controls.m_XWorldCoordinateSpinBox->setSingleStep(stepSize);
  }
  else if (axis == 1)
  {
    m_Controls.m_YWorldCoordinateSpinBox->setSingleStep(stepSize);
  }
  else if (axis == 2)
  {
    m_Controls.m_ZWorldCoordinateSpinBox->setSingleStep(stepSize);
  }
}

void QmitkImageNavigatorView::OnMillimetreCoordinateValueChanged()
{
  if (m_IRenderWindowPart)
  {
    mitk::TimeGeometry::ConstPointer geometry = m_IRenderWindowPart->GetActiveQmitkRenderWindow()->GetSliceNavigationController()->GetInputWorldTimeGeometry();

    if (geometry.IsNotNull())
    {
      mitk::Point3D positionInWorldCoordinates;
      positionInWorldCoordinates[0] = m_Controls.m_XWorldCoordinateSpinBox->value();
      positionInWorldCoordinates[1] = m_Controls.m_YWorldCoordinateSpinBox->value();
      positionInWorldCoordinates[2] = m_Controls.m_ZWorldCoordinateSpinBox->value();

      m_IRenderWindowPart->SetSelectedPosition(positionInWorldCoordinates);
    }
  }
}

void QmitkImageNavigatorView::OnRefetch()
{
  if (m_IRenderWindowPart)
  {
    mitk::BaseGeometry::ConstPointer geometry = m_IRenderWindowPart->GetActiveQmitkRenderWindow()->GetSliceNavigationController()->GetInputWorldGeometry3D();
    mitk::TimeGeometry::ConstPointer timeGeometry = m_IRenderWindowPart->GetActiveQmitkRenderWindow()->GetSliceNavigationController()->GetInputWorldTimeGeometry();

    if (geometry.IsNull() && timeGeometry.IsNotNull())
    {
      mitk::TimeStepType timeStep = m_IRenderWindowPart->GetActiveQmitkRenderWindow()->GetSliceNavigationController()->GetTime()->GetPos();
      geometry = timeGeometry->GetGeometryForTimeStep(timeStep);
    }

    if (geometry.IsNotNull())
    {
      mitk::BoundingBox::BoundsArrayType bounds = geometry->GetBounds();

      mitk::Point3D cornerPoint1InIndexCoordinates;
      cornerPoint1InIndexCoordinates[0] = bounds[0];
      cornerPoint1InIndexCoordinates[1] = bounds[2];
      cornerPoint1InIndexCoordinates[2] = bounds[4];

      mitk::Point3D cornerPoint2InIndexCoordinates;
      cornerPoint2InIndexCoordinates[0] = bounds[1];
      cornerPoint2InIndexCoordinates[1] = bounds[3];
      cornerPoint2InIndexCoordinates[2] = bounds[5];

      if (!geometry->GetImageGeometry())
      {
        cornerPoint1InIndexCoordinates[0] += 0.5;
        cornerPoint1InIndexCoordinates[1] += 0.5;
        cornerPoint1InIndexCoordinates[2] += 0.5;
        cornerPoint2InIndexCoordinates[0] -= 0.5;
        cornerPoint2InIndexCoordinates[1] -= 0.5;
        cornerPoint2InIndexCoordinates[2] -= 0.5;
      }

      mitk::Point3D crossPositionInWorldCoordinates = m_IRenderWindowPart->GetSelectedPosition();

      mitk::Point3D cornerPoint1InWorldCoordinates;
      mitk::Point3D cornerPoint2InWorldCoordinates;

      geometry->IndexToWorld(cornerPoint1InIndexCoordinates, cornerPoint1InWorldCoordinates);
      geometry->IndexToWorld(cornerPoint2InIndexCoordinates, cornerPoint2InWorldCoordinates);

      m_Controls.m_XWorldCoordinateSpinBox->blockSignals(true);
      m_Controls.m_YWorldCoordinateSpinBox->blockSignals(true);
      m_Controls.m_ZWorldCoordinateSpinBox->blockSignals(true);

      m_Controls.m_XWorldCoordinateSpinBox->setMinimum(std::min(cornerPoint1InWorldCoordinates[0], cornerPoint2InWorldCoordinates[0]));
      m_Controls.m_YWorldCoordinateSpinBox->setMinimum(std::min(cornerPoint1InWorldCoordinates[1], cornerPoint2InWorldCoordinates[1]));
      m_Controls.m_ZWorldCoordinateSpinBox->setMinimum(std::min(cornerPoint1InWorldCoordinates[2], cornerPoint2InWorldCoordinates[2]));
      m_Controls.m_XWorldCoordinateSpinBox->setMaximum(std::max(cornerPoint1InWorldCoordinates[0], cornerPoint2InWorldCoordinates[0]));
      m_Controls.m_YWorldCoordinateSpinBox->setMaximum(std::max(cornerPoint1InWorldCoordinates[1], cornerPoint2InWorldCoordinates[1]));
      m_Controls.m_ZWorldCoordinateSpinBox->setMaximum(std::max(cornerPoint1InWorldCoordinates[2], cornerPoint2InWorldCoordinates[2]));

      m_Controls.m_XWorldCoordinateSpinBox->setValue(crossPositionInWorldCoordinates[0]);
      m_Controls.m_YWorldCoordinateSpinBox->setValue(crossPositionInWorldCoordinates[1]);
      m_Controls.m_ZWorldCoordinateSpinBox->setValue(crossPositionInWorldCoordinates[2]);

      m_Controls.m_XWorldCoordinateSpinBox->blockSignals(false);
      m_Controls.m_YWorldCoordinateSpinBox->blockSignals(false);
      m_Controls.m_ZWorldCoordinateSpinBox->blockSignals(false);
    }

    this->SetBorderColors();

  }
}
