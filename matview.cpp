#include "matview.h"

MatView::MatView(QQuickItem *parent) :
    QQuickItem(parent) {
	setFlag(QQuickItem::ItemHasContents);

}

void MatView::capture() {
	cv::VideoCapture cam(0);
	cam.set(cv::CAP_PROP_FRAME_WIDTH, 320);
	cam.set(cv::CAP_PROP_FRAME_HEIGHT, 240);
	if (!cam.isOpened()) {
		qDebug() << "Could not open cam";
	}
	cam >> _mat;
	cv::flip(_mat, _mat, 0);
	if (!_mat.data) {
		qDebug() << "Could not capture a frame";
	}
	_img = QImage(_mat.data, _mat.cols, _mat.rows, QImage::Format_BGR888);
	update();
}

QSGNode *MatView::updatePaintNode(QSGNode *oldNode,
                                  QQuickItem::UpdatePaintNodeData *data) {
	Q_UNUSED(data);
	QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>(oldNode);
	if (!node) {
		node = new QSGSimpleTextureNode;
	}
	QSGTexture *texture = window()->createTextureFromImage(_img);
	node->setOwnsTexture(true);
	node->setRect(boundingRect());
	node->setTexture(texture);
	node->markDirty(QSGNode::DirtyForceUpdate);
	return node;
}
