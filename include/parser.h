#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "geometry.h"

class Parser {
public:
  bool Parse(const std::string &, std::vector<triangle> &);
  virtual bool Parse(std::istream &, std::vector<triangle> &) = 0;
};

class STLParser : public Parser {
public:
  bool Parse (std::istream &, std::vector<triangle> &) override;
private:
  bool ParseAscii(std::istream &, std::vector<triangle> &);
  bool ParseBinary(std::istream &, std::vector<triangle> &);
};

#endif // PARSER_H
