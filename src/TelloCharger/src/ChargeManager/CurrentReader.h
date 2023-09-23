/**
 * @file CurrentReader.h
 * @brief 電流取得クラス
 * @author Tatsuya Miyazaki
 * @date 2023-09-23
 *
 */

#pragma once

class CurrentReader {
 public:
  CurrentReader();
  ~CurrentReader();
  bool isConnect(void);
  float getCurrent(void);
  void readIna219(void);

 private:
  bool _isConnect;
};
