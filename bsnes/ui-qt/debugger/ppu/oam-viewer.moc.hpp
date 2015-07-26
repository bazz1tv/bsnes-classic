class OamViewer : public Window {
  Q_OBJECT

public:
  QHBoxLayout *layout;
  QTreeWidget *list;
  QVBoxLayout *controlLayout;
    QHBoxLayout *canvasLayout;
    QVBoxLayout *zoomLayout;
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

  QPushButton *soloButton;
  QCheckBox *toggleBox;

  void autoUpdate();
  void soloSprite(QTreeWidgetItem *item);
  OamViewer();

public slots:
  void show();
  void refresh();
  void refresh(QTreeWidgetItem *item);
  void refresh(QTreeWidgetItem *item, int column);
  void setZoom(int zoomlevel);
  void toggleSprite(QTreeWidgetItem *item);
  void toggleSprite(QTreeWidgetItem *item, bool show);
  void toggleCurrentSprite();
  void toggleCurrentSprite(bool state);
  void soloSprite_slot();
  void showContextMenu(const QPoint &pos);
};

extern OamViewer *oamViewer;
