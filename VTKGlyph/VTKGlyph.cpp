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
	// 创建法向量的符号：圆锥形
	vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	cone->SetResolution(6);
	// or, read data
	vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();
	reader->SetFileName("fran_cut.vtk");
	// 计算 poly data 中 points 和 cell 的法向量
	vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	// 将多边形数据集作为法向量对象的数据输入
	normals->SetInputConnection(reader->GetOutputPort());
	// 将法线翻转，默认的法线方向指向内侧
	normals->FlipNormalsOn();
	// 由于读入的模型数据比较大，点比较多，因此使用 vtkMaskPoints 类采样部分数据，该类保留输入数据中的点数据及其属性，并支持点数据的采样
	vtkSmartPointer<vtkMaskPoints> maskPt = vtkSmartPointer<vtkMaskPoints>::New();
	maskPt->SetInputConnection(normals->GetOutputPort());
	// 设置采样率为 1/10
	maskPt->SetOnRatio(10);
	// 打开随机采样模式
	maskPt->RandomModeOn();
	// 偏移
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	transform->Translate(0.5, 0, 0);

	// 2. filter
	vtkSmartPointer<vtkTransformPolyDataFilter> filter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	// 将法向量的符号：圆锥形设置偏移
	filter->SetInputConnection(cone->GetOutputPort());
	filter->SetTransform(transform);

	vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
	// 设置被标识的点
	glyph->SetInputConnection(maskPt->GetOutputPort());
	// 设置被偏移的符号
	glyph->SetSourceConnection(filter->GetOutputPort());
	// 设置向量为法线方向
	glyph->SetVectorModeToUseNormal();
	glyph->SetScaleModeToScaleByVector();
	// 设置比例，缩小法向量的大小
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
