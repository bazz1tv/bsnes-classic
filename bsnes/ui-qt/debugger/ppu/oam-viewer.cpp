#include "oam-viewer.moc"
OamViewer *oamViewer;

void OamViewer::show() {
  Window::show();
  refresh();
}

void OamViewer::refresh()
{
  QList<QTreeWidgetItem*> items = list->findItems("", Qt::MatchContains);
  for(unsigned v = 0; v < items.count(); v++) {
    refresh(items[v]);
  }

  for(unsigned i = 0; i <= 7; i++) list->resizeColumnToContents(i);
}

void OamViewer::refresh(QTreeWidgetItem *item)
{
  unsigned i = item->data(0, Qt::UserRole).toUInt();

  uint8_t d0 = SNES::memory::oam[(i << 2) + 0];
  uint8_t d1 = SNES::memory::oam[(i << 2) + 1];
  uint8_t d2 = SNES::memory::oam[(i << 2) + 2];
  uint8_t d3 = SNES::memory::oam[(i << 2) + 3];
  uint8_t d4 = SNES::memory::oam[512 + (i >> 2)];
  bool x    = d4 & (1 << ((i & 3) << 1));
  bool size = d4 & (2 << ((i & 3) << 1));

  //TODO: create method to expose ChipDebugger::property values by name
  unsigned width, height;
  switch(SNES::ppu.regs.oam_basesize)
  {
    default:
    case 0: width = !size ?  8 : 16; height = !size ?  8 : 16; break;
    case 1: width = !size ?  8 : 32; height = !size ?  8 : 32; break;
    case 2: width = !size ?  8 : 64; height = !size ?  8 : 64; break;
    case 3: width = !size ? 16 : 32; height = !size ? 16 : 32; break;
    case 4: width = !size ? 16 : 64; height = !size ? 16 : 64; break;
    case 5: width = !size ? 32 : 64; height = !size ? 32 : 64; break;
    case 6: width = !size ? 16 : 32; height = !size ? 32 : 64; break;
    case 7: width = !size ? 16 : 32; height = !size ? 32 : 32; break;
  }

  //fprintf(stderr, "%d: w = %d, h = %d\n", v, width, height);

  signed xpos = (x << 8) + d0;
  if(xpos > 256) xpos = sclip<9>(xpos);
  unsigned ypos = d1;
  unsigned character = d2;
  unsigned priority = (d3 >> 4) & 3;
  unsigned palette = (d3 >> 1) & 7;
  string flags;
  bool use_name=false, hflip=false, vflip=false;
  if(d3 & 0x80)
  {
    flags << "V";
    vflip = true;
  }
  if(d3 & 0x40)
  {
    flags << "H";
    hflip = true;
  }
  if(d3 & 0x01)
  {
    flags << "N";
    use_name = true;
  }

  item->setText(1, string() << width << "x" << height);
  item->setText(2, string() << xpos);
  item->setText(3, string() << ypos);
  item->setText(4, string() << character);
  item->setText(5, string() << priority);
  item->setText(6, string() << palette);
  item->setText(7, flags);

  // Use the character (Tile#) based from OBJ VRAM location
  // $2101 and PPU Appendix 1,2 are very useful to understandin OAM
  //fprintf(stderr, "cur_item = %d\n", atoi(list->currentItem()->text(0).toUtf8().data()));
  //fprintf(stderr, "OAM VRAM addr = $%X, name sel = $%X\n", SNES::ppu.regs.oam_tdaddr, SNES::ppu.regs.oam_nameselect);
  if (list->currentItem() == item)
  {
    canvas->image->fill(0x000000);
    //fprintf(stderr, "item %d matches!\n", v);
    // Update the image
    unsigned tile_width = width / 8;
    unsigned tile_height = height / 8;

    // zoom-level bounds check
    while(width*zoomLevel > 128 || height*zoomLevel > 128)
    {
      zoom->setValue(zoomLevel-1);
    }
    const uint8_t *source = SNES::memory::vram.data();
    source += SNES::ppu.regs.oam_tdaddr;
    source += use_name ? (SNES::ppu.regs.oam_nameselect+1) * (0x2000) : 0;
    source += character * 0x20; // 4bpp tile size
    uint32_t *dest = (uint32_t*)canvas->image->bits();
    unsigned palette_index = 128 + (palette * 16);
    unsigned zy,zx; // direct replacement of px/py for zoom level drawing
    // This code was borrowed modified from VRAM viewer
    for(unsigned ty = 0; ty < tile_height; ty++) 
    {
      for(unsigned tx = 0; tx < tile_width; tx++) 
      {
        for(unsigned py = zy = 0; py < 8; py++,zy+=zoomLevel)
        {
          for(unsigned px = zx = 0; px < 8; px++,zx+=zoomLevel)
          {
            uint8_t pixel = 0;
            d0 = (source[ 0]>>px) & 1;
            d1 = (source[ 1]>>px) & 1;
            d2 = (source[16]>>px) & 1;
            d3 = (source[17]>>px) & 1;

            pixel = (d3<<3) | (d2<<2) | (d1<<1) | d0;

            uint16_t color;
            uint32_t output;
            // This code was taken from CgramViewer::refresh() and should be refactored into a common utility function
            if (pixel)
            {
              color = SNES::memory::cgram[(palette_index+pixel) * 2];
              color |= SNES::memory::cgram[(palette_index+pixel) * 2 + 1] << 8;

              uint8_t r = (color >>  0) & 31;
              uint8_t g = (color >>  5) & 31;
              uint8_t b = (color >> 10) & 31;

              r = (r << 3) | (r >> 2);
              g = (g << 3) | (g >> 2);
              b = (b << 3) | (b >> 2);

              output = (r << 16) | (g << 8) | (b << 0);
            }
            else output = 0;
            
            for (unsigned y=0; y < zoomLevel; y++)
              for (unsigned x=0; x < zoomLevel; x++)
              {
                unsigned hf_val=0,vf_val=0;

                vf_val = vflip ? ((8*zoomLevel)-zy) : zy;
                hf_val = hflip ? zx : ((8*zoomLevel)-zx);

                dest[(ty * 8 + vf_val + y) * 128 + (tx * 8 + hf_val + x)] = output;
              }
          }
          source += 2;
        }
        source += 16;
      }
    }
    canvas->update();
  }
}

void OamViewer::autoUpdate() {
  if(autoUpdateBox->isChecked()) refresh();
}

OamViewer::OamViewer() :
zoomLevel(2)
{
  setObjectName("oam-viewer");
  setWindowTitle("Sprite Viewer");
  setGeometryString(&config().geometry.oamViewer);
  application.windowList.append(this);

  layout = new QHBoxLayout;
  layout->setAlignment(Qt::AlignLeft);
  layout->setMargin(Style::WindowMargin);
  layout->setSpacing(Style::WidgetSpacing);
  setLayout(layout);

  list = new QTreeWidget;
  list->setColumnCount(8);
  list->setHeaderLabels(QStringList() << "#" << "Size" << "X" << "Y" << "Char" << "Pri" << "Pal" << "Flags");
  list->setAllColumnsShowFocus(true);
  list->setAlternatingRowColors(true);
  list->setRootIsDecorated(false);
  list->setSortingEnabled(false);
  layout->addWidget(list);

  for(unsigned i = 0; i < 128; i++) {
    QTreeWidgetItem *item = new QTreeWidgetItem(list);
    item->setData(0, Qt::UserRole, QVariant(i));
    item->setTextAlignment(0, Qt::AlignHCenter);
    item->setTextAlignment(1, Qt::AlignHCenter);
    item->setTextAlignment(2, Qt::AlignRight);
    item->setTextAlignment(3, Qt::AlignRight);
    item->setTextAlignment(4, Qt::AlignRight);
    item->setTextAlignment(5, Qt::AlignRight);
    item->setTextAlignment(6, Qt::AlignRight);
    item->setTextAlignment(7, Qt::AlignLeft);
    item->setText(0, string() << i);
  }

  controlLayout = new QVBoxLayout;
  controlLayout->setAlignment(Qt::AlignTop);
  controlLayout->setSpacing(0);
  layout->addLayout(controlLayout);

  canvas = new Canvas;
  canvas->setFixedSize(128, 128);
  controlLayout->addWidget(canvas);

  autoUpdateBox = new QCheckBox("Auto update");
  controlLayout->addWidget(autoUpdateBox);

  refreshButton = new QPushButton("Refresh");
  controlLayout->addWidget(refreshButton);

  zoomLayout = new QHBoxLayout;
  zoomLayout->setAlignment(Qt::AlignLeft);
  zoomLayout->setMargin(Style::WindowMargin);
  zoomLayout->setSpacing(Style::WidgetSpacing);
  controlLayout->addLayout(zoomLayout);

  // Zoom widgets
  zoomLabel = new QLabel("Zoom", this);
  zoom = new QSpinBox(this);
  zoom->setValue(2);
  zoomLayout->addWidget(zoomLabel);
  zoomLayout->addWidget(zoom);

  //Signals
  connect(zoom, SIGNAL(valueChanged(int)), this, SLOT(setZoom(int)));
  connect(refreshButton, SIGNAL(released()), this, SLOT(refresh()));
  connect(list, SIGNAL(itemClicked(QTreeWidgetItem *,int)), this, SLOT(refresh(QTreeWidgetItem *)));
}

void OamViewer::setZoom(int zoomLevel)
{
  this->zoomLevel = zoomLevel;
}

void OamViewer::Canvas::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.drawImage(0, 0, *image);
}

OamViewer::Canvas::Canvas() {
  image = new QImage(128, 128, QImage::Format_RGB32);
  image->fill(0x000000);
}
