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

// Qmitk related includes
#include "QmitkPointSetInteractionView.h"
#include "ui_QmitkPointSetInteractionViewControls.h"

#include <berryIWorkbenchWindow.h>
#include <berryISelectionService.h>
#include <QInputDialog>
#include <QLineEdit>
#include <mitkProperties.h>
#include <QmitkPointListWidget.h>
#include <mitkDataNodeObject.h>
#include <mitkDataNodeSelection.h>
#include <QMessageBox>
#include <QDateTime>

QmitkPointSetInteractionView::QmitkPointSetInteractionView( QObject* /*parent*/ )
: m_Controls(0)
{
	pelvisMarkers.resize(10);
	femurMarkers.resize(12);
	for (size_t i = 0; i < pelvisMarkers.size(); ++i)
	{
		Marker tmp;
		tmp.x = 0.f;
		tmp.y = 0.f;
		tmp.z = 0.f;
		tmp.flag = false;
		pelvisMarkers[i] = tmp;
	}
	for (size_t i = 0; i < femurMarkers.size(); ++i)
	{
		Marker tmp;
		tmp.x = 0.f;
		tmp.y = 0.f;
		tmp.z = 0.f;
		tmp.flag = false;
		femurMarkers[i] = tmp;
	}
}

QmitkPointSetInteractionView::~QmitkPointSetInteractionView()
{
}

void QmitkPointSetInteractionView::CreateQtPartControl( QWidget *parent )
{
  m_Controls = new Ui::QmitkPointSetInteractionControls;
  m_Controls->setupUi(parent);
  m_Controls->m_PbAddPointSet->connect( m_Controls->m_PbAddPointSet, SIGNAL( clicked() )
    , this, SLOT( OnAddPointSetClicked() ) );
  
  QObject::connect(m_Controls->pelvisMarkersList, SIGNAL(itemClicked(QListWidgetItem*)),
	  this, SLOT(clickedPelvis(QListWidgetItem*)));
  QObject::connect(m_Controls->femurMarkersList, SIGNAL(itemClicked(QListWidgetItem*)),
	  this, SLOT(clickedFemur(QListWidgetItem*)));

  QObject::connect( m_Controls->pelvisMarkersList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), 
	  this, SLOT(doubleClickedPelvis(QListWidgetItem*)));
  QObject::connect(m_Controls->femurMarkersList, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
	  this, SLOT(doubleClickedFemur(QListWidgetItem*)));
  m_Controls->m_PbAddPointSet->setVisible(false);

  QObject::connect( m_Controls->saveBtn, SIGNAL(clicked()), 
	  this, SLOT(SavePoints()) );
}
void QmitkPointSetInteractionView::clickedPelvis(QListWidgetItem* item)
{
	MITK_INFO("PointSetPelvis") << m_Controls->pelvisMarkersList->row(item) << item->text();
	//ÏÈ»ñµÃÁÐ±íÊý¾Ý
	mitk::DataNode::Pointer pelvis = this->GetDefaultDataStorage()->GetNamedNode("Pelvis");
	if (pelvis)
	{
		// make new selection and emulate selection for this
		std::vector<mitk::DataNode*> selection;
		selection.push_back(pelvis);
		this->FireNodesSelected(selection);
		this->OnSelectionChanged(selection);

		//»ñµÃ¶ÔÓ¦µÄmarkerµã
		int curIndex = m_Controls->pelvisMarkersList->row(item);
		if (pelvisMarkers[curIndex].flag == true)
		{
			int index = pelvisMarkers[curIndex].index;
		}
	}
}
void QmitkPointSetInteractionView::clickedFemur(QListWidgetItem* item)
{
	MITK_INFO("PointSetFemur") << m_Controls->femurMarkersList->row(item) << item->text();
	//ÏÈ»ñµÃÁÐ±íÊý¾Ý
	mitk::DataNode::Pointer femur = this->GetDefaultDataStorage()->GetNamedNode("Femur");
	//ÒÑ¾­´æÔÚ
	if (femur)
	{
		// make new selection and emulate selection for this
		std::vector<mitk::DataNode*> selection;
		selection.push_back(femur);
		this->FireNodesSelected(selection);
		this->OnSelectionChanged(selection);
	}
}

void QmitkPointSetInteractionView::doubleClickedPelvis(QListWidgetItem* item)
{
	MITK_INFO("PointSetPelvis") << m_Controls->pelvisMarkersList->row(item) << item->text();
	//ÏÈ»ñµÃÁÐ±íÊý¾Ý
	mitk::DataNode::Pointer pelvis = this->GetDefaultDataStorage()->GetNamedNode("Pelvis");
	//ÒÑ¾­´æÔÚ
	if (pelvis)
	{
		// make new selection and emulate selection for this
		std::vector<mitk::DataNode*> selection;
		selection.push_back(pelvis);
		this->FireNodesSelected(selection);
		this->OnSelectionChanged(selection);
		//»ñµÃ×î½üÌí¼ÓµÄµã
		float x, y, z = 0.f;
		int index = -1;
		bool valid = m_Controls->m_PointListWidget->GetLastCoord(x, y, z, index);

		if (valid)
		{
			int curIndex = m_Controls->pelvisMarkersList->row(item);
// 			curIndex -= 1;
			if (pelvisMarkers[curIndex].flag == false)
			{
				QString coordStr = item->text() + "=" + QString::number(x) + "," + QString::number(y) + "," + QString::number(z);
				m_Controls->pelvisMarkersList->item(curIndex)->setText(coordStr);
				Marker curMarker;
				curMarker.x = x;
				curMarker.y = y;
				curMarker.z = z;
				curMarker.flag = true;
				curMarker.index = index;
				pelvisMarkers[curIndex] = curMarker;
			}
			else
			{
				QString coordStr = item->text();
				coordStr = coordStr.mid(0, coordStr.indexOf('=') + 1) + QString::number(x) + "," + QString::number(y) + "," + QString::number(z);
				m_Controls->pelvisMarkersList->item(curIndex)->setText(coordStr);
				Marker curMarker;
				curMarker.x = x;
				curMarker.y = y;
				curMarker.z = z;
				curMarker.flag = true;
				curMarker.index = index;
				pelvisMarkers[curIndex] = curMarker;
			}
		}
	}
	//´´½¨
	else
	{
		//Create a new empty pointset
		mitk::PointSet::Pointer pointSet = mitk::PointSet::New();
		// Create a new data tree node
		mitk::DataNode::Pointer pointSetNode = mitk::DataNode::New();
		// fill the data tree node with the appropriate information
		pointSetNode->SetData(pointSet);
		pointSetNode->SetProperty("pointsize", mitk::FloatProperty::New(10));
		pointSetNode->SetProperty("name", mitk::StringProperty::New("Pelvis"));
		pointSetNode->SetProperty("opacity", mitk::FloatProperty::New(1));
		pointSetNode->SetColor(1.0, 1.0, 0.0);

		// add the node to the ds
		this->GetDefaultDataStorage()->Add(pointSetNode);
		// make new selection and emulate selection for this
		std::vector<mitk::DataNode*> selection;
		selection.push_back(pointSetNode);
		this->FireNodesSelected(selection);
		this->OnSelectionChanged(selection);

		//´¥·¢¼Óµã
		m_Controls->m_PointListWidget->OnBtnAddPoint(true);
	}
}
void QmitkPointSetInteractionView::doubleClickedFemur(QListWidgetItem* item)
{
	MITK_INFO("PointSetFemur") << m_Controls->femurMarkersList->row(item) << item->text();

	//ÏÈ»ñµÃÁÐ±íÊý¾Ý
	mitk::DataNode::Pointer femur = this->GetDefaultDataStorage()->GetNamedNode("Femur");
	//ÒÑ¾­´æÔÚ
	if (femur)
	{
		// make new selection and emulate selection for this
		std::vector<mitk::DataNode*> selection;
		selection.push_back(femur);
		this->FireNodesSelected(selection);
		this->OnSelectionChanged(selection);
		//»ñµÃ×î½üÌí¼ÓµÄµã
		float x, y, z = 0.f;
		int index = -1;
		bool valid = m_Controls->m_PointListWidget->GetLastCoord(x, y, z, index);

		if (valid)
		{
			int curIndex = m_Controls->femurMarkersList->row(item);
// 			curIndex -= 1;
			if (femurMarkers[curIndex].flag == false)
			{
				QString coordStr = item->text() + "=" + QString::number(x) + "," + QString::number(y) + "," + QString::number(z);
				m_Controls->femurMarkersList->item(curIndex)->setText(coordStr);
				Marker curMarker;
				curMarker.x = x;
				curMarker.y = y;
				curMarker.z = z;
				curMarker.flag = true;
				curMarker.index = index;
				femurMarkers[curIndex] = curMarker;
			}
			else
			{
				QString coordStr = item->text();
				coordStr = coordStr.mid(0, coordStr.indexOf('=') + 1) + QString::number(x) + "," + QString::number(y) + "," + QString::number(z);
				m_Controls->femurMarkersList->item(curIndex)->setText(coordStr);
				Marker curMarker;
				curMarker.x = x;
				curMarker.y = y;
				curMarker.z = z;
				curMarker.flag = true;
				curMarker.index = index;
				femurMarkers[curIndex] = curMarker;
			}
		}
	}
	//´´½¨
	else
	{
		//Create a new empty pointset
		mitk::PointSet::Pointer pointSet = mitk::PointSet::New();
		// Create a new data tree node
		mitk::DataNode::Pointer pointSetNode = mitk::DataNode::New();
		// fill the data tree node with the appropriate information
		pointSetNode->SetData(pointSet);
		pointSetNode->SetProperty("pointsize", mitk::FloatProperty::New(10));
		pointSetNode->SetProperty("name", mitk::StringProperty::New("Femur"));
		pointSetNode->SetProperty("opacity", mitk::FloatProperty::New(1));
		pointSetNode->SetColor(1.0, 1.0, 0.0);

		// add the node to the ds
		this->GetDefaultDataStorage()->Add(pointSetNode);
		// make new selection and emulate selection for this
		std::vector<mitk::DataNode*> selection;
		selection.push_back(pointSetNode);
		this->FireNodesSelected(selection);
		this->OnSelectionChanged(selection);

		//´¥·¢¼Óµã
		m_Controls->m_PointListWidget->OnBtnAddPoint(true);
	}
}

void QmitkPointSetInteractionView::Activated()
{
  // emulate datamanager selection
  std::vector<mitk::DataNode*> selection = this->GetDataManagerSelection();
  this->OnSelectionChanged( selection );
}

void QmitkPointSetInteractionView::Deactivated()
{
  // emulate empty selection
  std::vector<mitk::DataNode*> selection;
  this->OnSelectionChanged( selection );
  m_Controls->m_PointListWidget->DeactivateInteractor(true);
}

void QmitkPointSetInteractionView::OnAddPointSetClicked()
{
  //Ask for the name of the point set
  bool ok = false;
  QString name = QInputDialog::getText( QApplication::activeWindow()
    , "Add point set...", "Enter name for the new point set", QLineEdit::Normal, "PointSet", &ok );
  if ( ! ok || name.isEmpty() )
    return;

  //
  //Create a new empty pointset
  //
  mitk::PointSet::Pointer pointSet = mitk::PointSet::New();
  //
  // Create a new data tree node
  //
  mitk::DataNode::Pointer pointSetNode = mitk::DataNode::New();
  //
  // fill the data tree node with the appropriate information
  //
  pointSetNode->SetData( pointSet );
  pointSetNode->SetProperty( "name", mitk::StringProperty::New( name.toStdString() ) );
  pointSetNode->SetProperty( "opacity", mitk::FloatProperty::New( 1 ) );
  pointSetNode->SetColor( 1.0, 1.0, 0.0 );
  //
  // add the node to the ds
  //
  this->GetDefaultDataStorage()->Add(pointSetNode);

  // make new selection and emulate selection for this
  std::vector<mitk::DataNode*> selection;
  selection.push_back( pointSetNode );
  this->FireNodesSelected( selection );
  this->OnSelectionChanged( selection );
}

void QmitkPointSetInteractionView::OnSelectionChanged(std::vector<mitk::DataNode*> nodes)
{
  mitk::DataNode* selectedNode = 0;
  if(nodes.size() > 0)
    selectedNode = nodes.front();

  mitk::PointSet* pointSet = 0;
  if(selectedNode)
    pointSet = dynamic_cast<mitk::PointSet*> ( selectedNode->GetData() );

  if (pointSet)
  {
    m_SelectedPointSetNode = selectedNode;
    m_Controls->m_CurrentPointSetLabel->setText(QString::fromStdString(selectedNode->GetName()));
    m_Controls->m_PointListWidget->SetPointSetNode(selectedNode);
  }
  else
  {
    m_Controls->m_CurrentPointSetLabel->setText("None");
    m_Controls->m_PointListWidget->SetPointSetNode(nullptr);
    std::vector<mitk::DataNode*> emptyList;
    emptyList.push_back(nullptr);
    this->FireNodesSelected( emptyList );
  }

}

bool QmitkPointSetInteractionView::IsExclusiveFunctionality() const
{
  return true;
}

void QmitkPointSetInteractionView::NodeChanged( const mitk::DataNode* node )
{
  if(node == m_SelectedPointSetNode && m_Controls->m_CurrentPointSetLabel->text().toStdString() != node->GetName())
  {
    m_Controls->m_CurrentPointSetLabel->setText(QString::fromStdString(node->GetName()));
	//¿ªÆôÊ°È¡Ä£Ê½
	m_Controls->m_PointListWidget->OnBtnAddPoint(true);
  }
}

void QmitkPointSetInteractionView::StdMultiWidgetAvailable( QmitkStdMultiWidget& stdMultiWidget )
{
    if(m_Controls)
      m_Controls->m_PointListWidget->SetMultiWidget( &stdMultiWidget );
}

void QmitkPointSetInteractionView::StdMultiWidgetClosed( QmitkStdMultiWidget& /*stdMultiWidget*/ )
{
   if(m_Controls)
      m_Controls->m_PointListWidget->SetMultiWidget( 0 );
}

void QmitkPointSetInteractionView::SavePoints()
{
	//×ó²àASISµã
	/*hip.asis.left
	//ÓÒ²àASISµã
	hip.asis.right
	//×ó²à÷Å¾ÊÎÑÉÏÔµ
	hip.left.acetabulum.superior
	hip.left.acetabulum.anterior
	hip.left.acetabulum.posterior
	hip.left.acetabulum.center
	hip.right.acetabulum.superior
	hip.right.acetabulum.anterior
	hip.right.acetabulum.posterior
	hip.right.acetabulum.center*/
	MITK_INFO("PointSet") << "SavePoints";
	QString fileName = "e:/data/config.ini";
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
		return;
	QTextStream in(&file);
	//¼ÓÈëÊ±¼ä´Á
	//get current date and time
	QDateTime dateTime = QDateTime::currentDateTime();
	QString dateTimeString = dateTime.toString();
	in << dateTimeString << "\n";
	in << "[POINTS]" << "\n";
	for (size_t i = 0; i < pelvisMarkers.size(); ++i)
	{
// 		Marker curItem = pelvisMarkers[i];
		in << m_Controls->pelvisMarkersList->item(i)->text() << "\n";
	}
	in << "\n";
	for (size_t i = 0; i < femurMarkers.size(); ++i)
	{
		in << m_Controls->femurMarkersList->item(i)->text() << "\n";
	}
	file.flush();
	file.close();
}
