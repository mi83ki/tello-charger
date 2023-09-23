/**
 * @file CurrentReader.h
 * @brief 電流取得クラス
 * @author Tatsuya Miyazaki
 * @date 2023-09-23
 *
 */

#pragma once

#include <Timer.h>
#include <Filter.h>

class CurrentReader {
 public:
  CurrentReader(uint32_t);
  ~CurrentReader();
  bool isConnect(void);
  float getCurrent(void);
  void readIna219(void);
  void loop(void);

 private:
  float _getCurrent(void);
  bool _isConnect;
  Timer _timer;
  float _current;
  MovAveFilter _movAveFilter;
};
