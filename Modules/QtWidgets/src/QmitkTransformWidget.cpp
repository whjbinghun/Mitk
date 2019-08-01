#include "QmitkTransformWidget.h"

QmitkTransformWidget::QmitkTransformWidget(QWidget *parent, Qt::WindowFlags f, mitk::BaseRenderer *b, QmitkStdMultiWidget* mw)
	:QWidget(NULL, Qt::Tool | Qt::FramelessWindowHint),
	m_Renderer(b),
	m_MultiWidget(mw),
	m_Parent(parent),
	m_Layout(0),
	ui(new Ui::Form)
{
	ui->setupUi(this);
	this->setVisible(true);
}

QmitkTransformWidget::~QmitkTransformWidget()
{
}

void QmitkTransformWidget::SetLayoutIndex(unsigned int layoutIndex)
{
	m_Layout = layoutIndex;
}