//  Author: Rasmus Stougaard

#ifndef _HORN_H
#define _HORN_H 1

class Horn {
public:

  Horn(int outputPin);
  ~Horn(){};

  void update()
  {
    signal(HORN_UPDATE);
  };

  void red()
  {
    signal(HORN_RED);
  };
  void green()
  {
    signal(HORN_GREEN);
  };
  void yellow()
  {
    signal(HORN_YELLOW);
  };

private:
  void signal(int type);
  static int output;

  static const int HORN_UPDATE = 0;
  static const int HORN_RED = 1;
  static const int HORN_GREEN = 2;
  static const int HORN_YELLOW = 3;
  static const int HORN_TICK_DURATION_MS = 300;
};

#endif 
// _HORN_H
