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


//Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>

//Qmitk
#include "QmitkGeometryToolsView.h"

//mitk
#include <mitkImage.h>
#include <mitkAffineBaseDataInteractor3D.h>

//micro services
#include <usModuleRegistry.h>
#include <usGetModuleContext.h>

#include "QmitkStdMultiWidget.h"
#include <mitkWorkbenchUtil.h>
#include <mitkPlanarLine.h>
#include <QDateTime>
#include <QFile>

#include <mitkGizmo.h>

const std::string QmitkGeometryToolsView::VIEW_ID = "org.mitk.views.geometrytools";

void QmitkGeometryToolsView::SetFocus()
{
  m_Controls.m_AddInteractor->setFocus();
  mitk::DataStorage::Pointer ds = this->GetDataStorage();
  if (ds)
  {
	  if (ds->bEmulationMode == true)
		  //   if (mitk::RenderingManager::GetInstance()->bEmulationMode == true)
	  {
		  m_Controls.cancelBtn->setVisible(false);
		  m_Controls.applyBtn->setVisible(false);
		  m_Controls.clipPlaneBtn->setVisible(false);
		  m_Controls.saveBtn->setVisible(false);

		  m_Controls.m_SettingCenterPointBtn->setVisible(true);
// 		  ds->bEmulationMode = false;
	  }
	  else
	  {
		  m_Controls.cancelBtn->setVisible(true);
		  m_Controls.applyBtn->setVisible(true);
		  m_Controls.clipPlaneBtn->setVisible(true);
		  m_Controls.saveBtn->setVisible(true);

		  m_Controls.m_SettingCenterPointBtn->setVisible(false);
	  }
  }

}

void QmitkGeometryToolsView::CreateQtPartControl( QWidget *parent )
{
  m_Controls.setupUi( parent );
  connect( m_Controls.m_AddInteractor, SIGNAL(clicked()), this, SLOT(AddInteractor()) );
  connect( m_Controls.m_RemoveInteractor, SIGNAL(clicked()), this, SLOT(RemoveInteractor()) );
  connect( m_Controls.m_TranslationStep, SIGNAL(valueChanged(double)), this, SLOT(OnTranslationSpinBoxChanged(double)) );
  connect( m_Controls.m_RotationStep, SIGNAL(valueChanged(double)), this, SLOT(OnRotationSpinBoxChanged(double)) );
  connect( m_Controls.m_ScaleFactor, SIGNAL(valueChanged(double)), this, SLOT(OnScaleSpinBoxChanged(double)) );
  connect( m_Controls.m_AnchorPointX, SIGNAL(valueChanged(double)), this, SLOT(OnAnchorPointChanged(double)) );
  connect( m_Controls.m_AnchorPointY, SIGNAL(valueChanged(double)), this, SLOT(OnAnchorPointChanged(double)) );
  connect( m_Controls.m_AnchorPointZ, SIGNAL(valueChanged(double)), this, SLOT(OnAnchorPointChanged(double)) );
  //connect( m_Controls.m_UsageInfoCheckBox, SIGNAL(clicked(bool)), this, SLOT(OnUsageInfoBoxChanged(bool)) );

  connect( m_Controls.m_CustomAnchorPointRadioButton, SIGNAL(toggled(bool)), this, SLOT(OnCustomPointRadioButtonToggled(bool)) );

  connect( m_Controls.m_OriginPointRadioButton, SIGNAL(clicked(bool)), this, SLOT(OnOriginPointRadioButton(bool)) );
  connect( m_Controls.m_CenterPointRadioButton, SIGNAL(clicked(bool)), this, SLOT(OnCenterPointRadioButton(bool)) );

  connect(m_Controls.applyBtn, SIGNAL(clicked()), this, SLOT(OnApply()));
  connect(m_Controls.saveBtn, SIGNAL(clicked()), this, SLOT(OnSave()));
  connect(m_Controls.clipPlaneBtn, SIGNAL(clicked()), this, SLOT(OnClipPlane()));
  connect(m_Controls.m_SettingCenterPointBtn, SIGNAL(clicked()), this, SLOT(OnSettingCenterPoint()));

  m_Controls.m_KeyboardMode->setVisible(false);
  m_Controls.m_MouseMode->setVisible(false);
  //m_Controls.m_UsageInfoCheckBox->setVisible(false);
  m_Controls.label_3->setVisible(false);
  m_Controls.m_ScaleFactor->setVisible(false);
  m_Controls.m_CenterPointRadioButton->setChecked(true);
  m_Controls.m_SettingCenterPointBtn->setVisible(false);
  
  //m_Controls.m_UsageInfo->hide();
  m_Controls.m_CustomAnchorPoint->hide();

}

void QmitkGeometryToolsView::OnClipPlane()
{
// 	berry::WorkbenchWindow::Pointer window = berry::PlatformUI::GetWorkbench()->GetActiveWorkbenchWindow().Cast<berry::WorkbenchWindow>();
// 	if (window)
// 	{
// 		MITK_INFO("org.mitk.views.geometrytools") << "berry::WorkbenchWindow::Pointer is valid";
// 	}
	//Ôö¼ÓplaneÊý¾Ý½áµã
// 	mitk::DataStorage::Pointer ds = this->GetDataStorage();
// 	mitk::DataNode::Pointer plane = ds->GetNamedNode("Plane");
// 	if (plane)
// 	{
// 
// 	}
// 	else
// 	{
// 		mitk::PlanarFigure* figure = mitk::PlanarLine::New();
// 		// Create a new data tree node
// 		auto pointSetNode = mitk::DataNode::New();
// 		pointSetNode->SetName("Plane");
// 		// fill the data tree node with the appropriate information
// 		pointSetNode->SetData(figure);
// 		pointSetNode->SetSelected(true);
// 		// add the node to the ds
// 		ds->Add(pointSetNode);
// 	}
	//¿ªÆôÆ½ÃæÉèÖÃÄ£Ê½
	mitk::RenderingManager::GetInstance()->bClipPlaneMode = true;

	QString view_id = "org.mitk.views.measurement";
	IViewPart::Pointer measurement = this->GetSite()->GetPage()->ShowView(view_id);
}
void QmitkGeometryToolsView::OnSettingCenterPoint()
{
	mitk::RenderingManager::GetInstance()->bSettingCenterPoint = true;

	QString view_id = "org.mitk.views.measurement";
	IViewPart::Pointer measurement = this->GetSite()->GetPage()->ShowView(view_id);
}
void QmitkGeometryToolsView::OnSave()
{
	MITK_INFO("org.mitk.views.geometrytools") << "Save";
	QString fileName = "e:/data/config.ini";
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
		return;
	QTextStream in(&file);
	in << "[MODEL]" << "\n";

	//»ñµÃÊý¾Ý½Úµã
	mitk::DataStorage::Pointer ds = this->GetDataStorage();
	mitk::DataNode::Pointer cup = ds->GetNamedNode( cupFileName.toStdString() );
	mitk::DataNode::Pointer stem = ds->GetNamedNode(tailFileName.toStdString());
	mitk::BaseGeometry* cupGeo = NULL;
	if (cup)
		cupGeo = cup->GetData()->GetGeometry();
	in << "cup.size = " << "\n";
	in << "cup.type = " << "\n";
	in << "cup.Matrix = ";
	if (cup)
	{
		if (cupGeo)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					in << cupGeo->GetIndexToWorldTransform()->GetMatrix()[i][j] << ",";
				}
			}
		}
	}
	in << "\n";
	
	std::ostringstream os;
	itk::Indent indent;
	in << "cup.Offset = ";
	if (cupGeo)
	{
		os << indent << cupGeo->GetIndexToWorldTransform()->GetOffset();
		in << os.str().c_str();
	}
	in << "\n";

	//Çå¿ÕÊý¾Ý
	os.str("");
	in << "cup.Center = ";
	if (cupGeo)
	{
		os << indent << cupGeo->GetIndexToWorldTransform()->GetCenter();
		in << os.str().c_str();
	}
	in << "\n";

	os.str("");
	in << "cup.Translation = ";
	if (cupGeo)
	{
		os << indent << cupGeo->GetIndexToWorldTransform()->GetTranslation();
		in << os.str().c_str();
	}
	in << "\n";

	mitk::BaseGeometry* stemGeo = NULL;
	if (stem)
		stemGeo = stem->GetData()->GetGeometry();
	in << "stem.size = " << "\n";
	in << "stem.type = " << "\n";
	in << "stem.Matrix = ";
	if (stem)
	{
		if (stemGeo)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					in << stemGeo->GetIndexToWorldTransform()->GetMatrix()[i][j] << ",";
				}
			}
		}
	}
	in << "\n";

	//Çå¿ÕÊý¾Ý
	os.str("");
	in << "stem.Offset = ";
	if (stemGeo)
	{
		os << indent << stemGeo->GetIndexToWorldTransform()->GetOffset();
		in << os.str().c_str();
	}
	in << "\n";

	os.str("");
	in << "stem.Center = ";
	if (stemGeo)
	{
		os << indent << stemGeo->GetIndexToWorldTransform()->GetCenter();
		in << os.str().c_str();
	}
	in << "\n";

	os.str("");
	in << "stem.Translation = ";
	if (stemGeo)
	{
		os << indent << stemGeo->GetIndexToWorldTransform()->GetTranslation();
		in << os.str().c_str();
	}
	in << "\n";

	in << "femur.slice.plane = ";
	mitk::DataNode::Pointer plane = ds->GetNamedNode("Plane");
	if (plane)
	{
		mitk::PlanarLine::Pointer lineFigure = dynamic_cast<mitk::PlanarLine*>(plane->GetData());
		if (lineFigure)
		{
			const mitk::Point2D &p0 = lineFigure->GetControlPoint(0);
			const mitk::Point2D &p1 = lineFigure->GetControlPoint(1);
			in << p0[0] << "," << p0[1] <<","<< p1[0] << "," << p1[1];
		}
	}
	in << "\n";

	{
		in << "[OTHERS]" << "\n";
		in << QStringLiteral("¾±¸É½Ç") << "\n";
		in << QStringLiteral("Æ«ÐÄ¾à") << "\n";
		in << QStringLiteral("ÌåÖØÁ¦Á¦±Û") << "\n";
		in << QStringLiteral("ÍâÕ¹¼¡Á¦±Û") << "\n";
		in << QStringLiteral("Ö«Ìå³¤¶È±ê¼Ç") << "\n";
	}

	file.flush();
	file.close();
}
void QmitkGeometryToolsView::OnApply()
{
	cupFilePath = "model/prosthesis hip - acetabular cup-2.STL";
	tailFilePath = "model/prosthesis hip - prosthesis hip tail-2.STL";
	cupFileName = "prosthesis hip - acetabular cup-2";
	tailFileName = "prosthesis hip - prosthesis hip tail-2";
	QStringList fileNames;
	fileNames.append(cupFilePath);
	fileNames.append(tailFilePath);
	if (fileNames.empty())
		return;
	berry::IWorkbenchWindow::Pointer workbenchWindow = this->GetSite()->GetWorkbenchWindow();
	mitk::WorkbenchUtil::LoadFiles(fileNames, workbenchWindow, false);
}
void QmitkGeometryToolsView::OnUsageInfoBoxChanged(bool flag)
{
  //m_Controls.m_UsageInfo->setVisible(flag);
}

void QmitkGeometryToolsView::OnSelectionChanged( berry::IWorkbenchPart::Pointer /*source*/,
                                             const QList<mitk::DataNode::Pointer>& nodes )
{
  foreach( mitk::DataNode::Pointer node, nodes )
  {
    if( node.IsNotNull() )
    {
      m_Controls.m_AddInteractor->setEnabled( true );
      return;
    }
  }
  m_Controls.m_AddInteractor->setEnabled( false );
}

void QmitkGeometryToolsView::OnCustomPointRadioButtonToggled(bool status)
{
  m_Controls.m_CustomAnchorPoint->setVisible(status);
  //change the anchor point to be the custom point
  OnAnchorPointChanged(0.0);
}

void QmitkGeometryToolsView::OnCenterPointRadioButton(bool)
{
  QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  foreach( mitk::DataNode::Pointer node, nodes )
  {
    if( node.IsNotNull() && (node->GetDataInteractor().IsNotNull()) )
    {
      node->SetFloatProperty("AffineBaseDataInteractor3D.Anchor Point X", node->GetData()->GetGeometry()->GetCenter()[0]);
      node->SetFloatProperty("AffineBaseDataInteractor3D.Anchor Point Y", node->GetData()->GetGeometry()->GetCenter()[1]);
      node->SetFloatProperty("AffineBaseDataInteractor3D.Anchor Point Z", node->GetData()->GetGeometry()->GetCenter()[2]);
    }
  }
}

void QmitkGeometryToolsView::OnOriginPointRadioButton(bool)
{
  QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  foreach( mitk::DataNode::Pointer node, nodes )
  {
    if( node.IsNotNull() && (node->GetDataInteractor().IsNotNull()) )
    {
      node->SetFloatProperty("AffineBaseDataInteractor3D.Anchor Point X", node->GetData()->GetGeometry()->GetOrigin()[0]);
      node->SetFloatProperty("AffineBaseDataInteractor3D.Anchor Point Y", node->GetData()->GetGeometry()->GetOrigin()[1]);
      node->SetFloatProperty("AffineBaseDataInteractor3D.Anchor Point Z", node->GetData()->GetGeometry()->GetOrigin()[2]);
    }
  }
}
void QmitkGeometryToolsView::AddInteractorGizmo()
{
	QList<mitk::DataNode::Pointer> allNodes = this->GetDataManagerSelection();
	//ÓÐÐ§Ñ¡Ôñ
	if (allNodes.size() > 0)
	{
		mitk::DataNode::Pointer node = allNodes[0];
		gizmoNodeName = node->GetName();
		mitk::DataStorage::Pointer ds = this->GetDataStorage();
		mitk::Gizmo::AddGizmoToNode(allNodes[0], ds);
	}

}
void QmitkGeometryToolsView::RemoveInteractorGizmo()
{
	QList<mitk::DataNode::Pointer> allNodes = this->GetDataManagerSelection();
	//ÓÐÐ§Ñ¡Ôñ
	if (allNodes.size() > 0)
	{
		mitk::DataNode::Pointer node = allNodes[0];
		std::string curName = node->GetName();
		if (curName == gizmoNodeName)
		{
			mitk::DataStorage::Pointer ds = this->GetDataStorage();
			mitk::Gizmo::RemoveGizmoFromNode(allNodes[0], ds);
		}
	}
}


void QmitkGeometryToolsView::AddInteractor()
{
	mitk::DataStorage::Pointer ds = this->GetDataStorage();
	if (ds->bEmulationMode)
	{
		AddInteractorGizmo();
	}
	else
	{
		QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
		foreach(mitk::DataNode::Pointer node, nodes)
		{
			if (node.IsNotNull())
			{

				mitk::AffineBaseDataInteractor3D::Pointer affineDataInteractor = mitk::AffineBaseDataInteractor3D::New();
				if (m_Controls.m_KeyboardMode->isChecked())
				{
					affineDataInteractor->LoadStateMachine("AffineInteraction3D.xml", us::ModuleRegistry::GetModule("MitkDataTypesExt"));
					affineDataInteractor->SetEventConfig("AffineKeyConfig.xml", us::ModuleRegistry::GetModule("MitkDataTypesExt"));
				}
				else if (m_Controls.m_MouseMode->isChecked())
				{
					affineDataInteractor->LoadStateMachine("AffineInteraction3D.xml", us::ModuleRegistry::GetModule("MitkDataTypesExt"));
					affineDataInteractor->SetEventConfig("AffineMouseConfig.xml", us::ModuleRegistry::GetModule("MitkDataTypesExt"));
				}

				affineDataInteractor->SetDataNode(node);

				node->SetBoolProperty("pickable", true);
				node->SetFloatProperty("AffineBaseDataInteractor3D.Translation Step Size", m_Controls.m_TranslationStep->value());
				node->SetFloatProperty("AffineBaseDataInteractor3D.Rotation Step Size", m_Controls.m_RotationStep->value());
				node->SetFloatProperty("AffineBaseDataInteractor3D.Scale Step Size", m_Controls.m_ScaleFactor->value());
				node->SetFloatProperty("AffineBaseDataInteractor3D.Anchor Point X", m_Controls.m_AnchorPointX->value());
				node->SetFloatProperty("AffineBaseDataInteractor3D.Anchor Point Y", m_Controls.m_AnchorPointY->value());
				node->SetFloatProperty("AffineBaseDataInteractor3D.Anchor Point Z", m_Controls.m_AnchorPointZ->value());

				//ÒÑcrosshairsÎªÖÐÐÄÐý×ª
				if (m_Controls.m_CenterPointRadioButton->isChecked())
				{
					OnCenterPointRadioButton(true);
				}
				//´«ÈëÖ÷´°Ìå
				QmitkStdMultiWidget::app()->affineDataInteractor = affineDataInteractor;
			}
		}
	}
}

void QmitkGeometryToolsView::RemoveInteractor()
{
	mitk::DataStorage::Pointer ds = this->GetDataStorage();
	if (ds->bEmulationMode)
	{
		RemoveInteractorGizmo();
	}
	else
	{
		QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
		foreach(mitk::DataNode::Pointer node, nodes)
		{
			if ((node.IsNotNull()) && (node->GetDataInteractor().IsNotNull()))
			{
				node->SetDataInteractor(nullptr);
				QmitkStdMultiWidget::app()->affineDataInteractor = NULL;
			}
		}
	}

}

void QmitkGeometryToolsView::OnTranslationSpinBoxChanged(double step)
{
  QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  foreach( mitk::DataNode::Pointer node, nodes )
  {
    if( node.IsNotNull() && (node->GetDataInteractor().IsNotNull()) )
    {
      node->SetFloatProperty("AffineBaseDataInteractor3D.Translation Step Size", step);
    }
  }
}

void QmitkGeometryToolsView::OnRotationSpinBoxChanged(double step)
{
  QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  foreach( mitk::DataNode::Pointer node, nodes )
  {
    if( node.IsNotNull() && (node->GetDataInteractor().IsNotNull()) )
    {
      node->SetFloatProperty("AffineBaseDataInteractor3D.Rotation Step Size", step);
    }
  }
}

void QmitkGeometryToolsView::OnScaleSpinBoxChanged(double factor)
{
  QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  foreach( mitk::DataNode::Pointer node, nodes )
  {
    if( node.IsNotNull() && (node->GetDataInteractor().IsNotNull()) )
    {
      node->SetFloatProperty("AffineBaseDataInteractor3D.Scale Step Size", factor);
    }
  }
}

void QmitkGeometryToolsView::OnAnchorPointChanged(double /*value*/)
{
  QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  foreach( mitk::DataNode::Pointer node, nodes )
  {
    if( node.IsNotNull() && (node->GetDataInteractor().IsNotNull()) )
    {
      node->SetFloatProperty("AffineBaseDataInteractor3D.Anchor Point X", m_Controls.m_AnchorPointX->value());
      node->SetFloatProperty("AffineBaseDataInteractor3D.Anchor Point Y", m_Controls.m_AnchorPointY->value());
      node->SetFloatProperty("AffineBaseDataInteractor3D.Anchor Point Z", m_Controls.m_AnchorPointZ->value());
    }
  }
}

