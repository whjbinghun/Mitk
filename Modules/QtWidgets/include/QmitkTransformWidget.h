

#ifndef QmitkTransformWidget_h
#define QmitkTransformWidget_h


#include <MitkQtWidgetsExports.h>
#include "ui_transformationWidget.h"
#include "mitkBaseRenderer.h"

#include <QWidget>
#include <QEvent>
#include <QPushButton>
#include <QToolButton>
#include <QMenuBar>
#include <QAction>
#include <QLabel>
#include <QTimer>

class QmitkStdMultiWidget;


class MITKQTWIDGETS_EXPORT QmitkTransformWidget : public QWidget, public Ui::Form
{
	Q_OBJECT

public:
	QmitkTransformWidget(QWidget* parent = 0, Qt::WindowFlags f = 0, mitk::BaseRenderer * b = 0, QmitkStdMultiWidget* mw = 0);
	virtual ~QmitkTransformWidget();

	/*! Set layout index. Defines layout direction (axial, coronal, sagital or threeD) of the parent. */
	void SetLayoutIndex(unsigned int layoutIndex);
protected:


signals:


public slots:


protected slots:


public:


protected:
	/*! Index of layout direction. 0: axial; 1: saggital; 2: coronal; 3: threeD */
	unsigned int        m_Layout;

private:

	mitk::BaseRenderer::Pointer m_Renderer;

	QmitkStdMultiWidget* m_MultiWidget;

	QWidget* m_Parent;

	Ui::Form* ui;
};

#endif // QmitkTransformWidget_H
