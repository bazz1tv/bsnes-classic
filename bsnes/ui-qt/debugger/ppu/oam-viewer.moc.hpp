class OamViewer : public Window {
  Q_OBJECT

public:
  QHBoxLayout *layout;
  QTreeWidget *list;
  QVBoxLayout *controlLayout;
    QHBoxLayout *zoomLayout;
  struct Canvas : public QWidget {
    QImage *image;
    void paintEvent(QPaintEvent*);
    Canvas();
  } *canvas;
  QCheckBox *autoUpdateBox;
  QPushButton *refreshButton;

  QLabel *zoomLabel;
  QSpinBox *zoom;
  int zoomLevel;

  void autoUpdate();
  OamViewer();

public slots:
  void show();
  void refresh();
  void refresh(QTreeWidgetItem *item);
  void setZoom(int zoomlevel);
};

extern OamViewer *oamViewer;
