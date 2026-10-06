#include "parser.h"

// parser

bool Parser::Parse(const std::string &filename, std::vector<triangle> &tris)
{
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "cannot open file '" << filename << "'" << std::endl;
    return false;
  }

  return Parse(file, tris);
}

// stl parser
bool STLParser::Parse(std::istream &stream, std::vector<triangle> &tris)
{
  tris.clear();

  std::string solid;
  stream >> solid;
  if (solid == "solid")
    return ParseAscii(stream, tris);
  else
    return ParseBinary(stream, tris);

  return true;
}

bool STLParser::ParseBinary(std::istream &stream, std::vector<triangle> &tris)
{
  stream.seekg(0, std::ios::end);
  std::streamsize size = stream.tellg();

  if (size < (80 + 4 + 50)) {
    std::cerr << "failed to read file: not STL format" << std::endl;
    return false;
  }

  // ignore first 80 bytes
  stream.seekg(80);

  int n;
  stream.read(reinterpret_cast<char*>(&n), sizeof(n));

  size_t expected_size = 80 + 4 + 50*n;
  if (size < expected_size) {
    std::cerr << "failed to read file: either not STL format or corrupted." << std::endl
              << "expected size: " << expected_size << " bytes" << std::endl;
    return false;
  }

  // skip first normal
  stream.seekg(sizeof(vec3), std::ios::cur);

  for (int i = 0; i < n; ++i) {

    triangle tri;
    stream.read(reinterpret_cast<char*>(&tri), sizeof(tri));
    tris.push_back(tri);

    // ignore attrib + normal
    stream.seekg(sizeof(short) + sizeof(vec3), std::ios::cur);
  }

  return true;
}

bool STLParser::ParseAscii(std::istream &stream, std::vector<triangle> &tris)
{
  std::streampos pos, prevpos;
  std::string str, line;

  // move pos to beginning
  stream.seekg(0);
  // read first line
  stream >> str;
  std::getline(stream, line);

  stream >> str;
  while (str == "facet") {
    // ignore normal
    std::getline(stream, line);
    // outer loop
    std::getline(stream, line);

    triangle t;
    float x,y,z;

    for (int i = 0; i < 3; i++) {
      // vertex
      stream >> str;
      if (str != "vertex") {
        std::cerr << "fails to read file: expects vertex but got '" << str << "''" << std::endl;
        return false;
      }
      stream >> t[i].x >> t[i].y >> t[i].z;
    }
    tris.push_back(t);

    // goto next line
    std::getline(stream, line);
    // endloop
    std::getline(stream, line);
    // endfacet
    std::getline(stream, line);

    // try get next facet
    stream >> str;
  }

  return true;
}
