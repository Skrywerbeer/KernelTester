#ifndef MATVIEW_H
#define MATVIEW_H

#include <QQuickItem>
#include <QQmlEngine>
#include <QImage>
#include <QSGNode>
#include <QSGSimpleTextureNode>

#include <opencv2/opencv.hpp>

class MatView : public QQuickItem {
		Q_OBJECT
		QML_ELEMENT
	public:
		MatView(QQuickItem *parent = nullptr);

		Q_INVOKABLE void capture();

		QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data) override;

	signals:

	private:
		QImage _img;
		cv::Mat _mat;
};

#endif // MATVIEW_H
