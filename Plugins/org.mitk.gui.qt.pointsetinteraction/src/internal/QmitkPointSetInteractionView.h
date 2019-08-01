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

#if !defined(QmitkPointSetInteraction_H__INCLUDED)
#define QmitkPointSetInteraction_H__INCLUDED

#include <berryISelectionListener.h>
#include <QmitkFunctionality.h>
#include <mitkWeakPointer.h>
#include <mitkDataNode.h>
#include <QListWidgetItem>

namespace Ui
{
class QmitkPointSetInteractionControls;
};

/*!
\brief QmitkPointSetInteractionView
*/
class QmitkPointSetInteractionView : public QmitkFunctionality
{
  Q_OBJECT

public:
	typedef struct 
	{
		float x;
		float y;
		float z;
		bool flag;
		//¶ÔÓ¦ÁÐ±íÖÐµÄË÷Òý
		int index;
	}Marker;
  QmitkPointSetInteractionView(QObject *parent=0);
  virtual ~QmitkPointSetInteractionView();


  virtual void CreateQtPartControl(QWidget *parent) override;
  void OnSelectionChanged(std::vector<mitk::DataNode*> nodes) override;
  void Activated() override;
  void Deactivated() override;
  void NodeChanged(const mitk::DataNode* node) override;
  bool IsExclusiveFunctionality() const override;
  virtual void StdMultiWidgetAvailable(QmitkStdMultiWidget& stdMultiWidget) override;
  virtual void StdMultiWidgetClosed(QmitkStdMultiWidget& stdMultiWidget) override;
protected slots:
  void OnAddPointSetClicked();
  void doubleClickedPelvis(QListWidgetItem* item);
  void doubleClickedFemur(QListWidgetItem* item);
  void clickedPelvis(QListWidgetItem* item);
  void clickedFemur(QListWidgetItem* item);
  void SavePoints();
protected:
  Ui::QmitkPointSetInteractionControls * m_Controls;
  mitk::WeakPointer<mitk::DataNode> m_SelectedPointSetNode;
  std::vector<Marker> pelvisMarkers;
  std::vector<Marker> femurMarkers;
};
#endif // !defined(QmitkPointSetInteraction_H__INCLUDED)
