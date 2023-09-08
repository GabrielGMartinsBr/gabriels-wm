class FrameWindowBase {
 protected:
  static const int border = 6;
  static const int header = 24;

 protected:
  int x;
  int y;
  int width;
  int height;
  bool maximized;
};