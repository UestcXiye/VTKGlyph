#include "VTKGlyph.h"

#include <vtkConeSource.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataNormals.h>
#include <vtkMaskPoints.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkGlyph3D.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

VTKGlyph::VTKGlyph(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	_pVTKWidget = new QVTKOpenGLNativeWidget();
	this->setCentralWidget(_pVTKWidget);
	// this->showMaximized();

	// 1. generate data
	// �����������ķ��ţ�Բ׶��
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	cone->SetResolution(6);
	// or, read data
	vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
	reader->SetFileName("fran_cut.vtk");
	// ���� poly data �� points �� cell �ķ�����
	vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	// ����������ݼ���Ϊ�������������������
	normals->SetInputConnection(reader->GetOutputPort());
	// �����߷�ת��Ĭ�ϵķ��߷���ָ���ڲ�
	normals->FlipNormalsOn();
	// ���ڶ����ģ�����ݱȽϴ󣬵�Ƚ϶࣬���ʹ�� vtkMaskPoints ������������ݣ����ౣ�����������еĵ����ݼ������ԣ���֧�ֵ����ݵĲ���
	vtkSmartPointer<vtkMaskPoints> maskPt = vtkSmartPointer<vtkMaskPoints>::New();
	maskPt->SetInputConnection(normals->GetOutputPort());
	// ���ò�����Ϊ 1/10
	maskPt->SetOnRatio(10);
	// ���������ģʽ
	maskPt->RandomModeOn();
	// ƫ��
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	transform->Translate(0.5, 0, 0);

	// 2. filter
	vtkSmartPointer<vtkTransformPolyDataFilter> filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	// ���������ķ��ţ�Բ׶������ƫ��
	filter->SetInputConnection(cone->GetOutputPort());
	filter->SetTransform(transform);

	vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
	// ���ñ���ʶ�ĵ�
	glyph->SetInputConnection(maskPt->GetOutputPort());
	// ���ñ�ƫ�Ƶķ���
	glyph->SetSourceConnection(filter->GetOutputPort());
	// ��������Ϊ���߷���
	glyph->SetVectorModeToUseNormal();
	glyph->SetScaleModeToScaleByVector();
	// ���ñ�������С�������Ĵ�С
	glyph->SetScaleFactor(0.004);

	// 3. mapper
	vtkSmartPointer<vtkPolyDataMapper> franMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> spikeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	// 4. actor
	vtkSmartPointer<vtkActor> franActor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> spikeActor = vtkSmartPointer<vtkActor>::New();

	// 5. renderer
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0.3, 0.6, 0.3);

	// 6. connect
	franMapper->SetInputConnection(normals->GetOutputPort());
	spikeMapper->SetInputConnection(glyph->GetOutputPort());
	franActor->SetMapper(franMapper);
	spikeActor->SetMapper(spikeMapper);
	renderer->AddActor(franActor);
	renderer->AddActor(spikeActor);

	this->_pVTKWidget->renderWindow()->AddRenderer(renderer);
	this->_pVTKWidget->renderWindow()->Render();
}

VTKGlyph::~VTKGlyph()
{}
