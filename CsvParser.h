//
// Created by 이일재 on 2015. 8. 19..
//

#ifndef FAST_CSV_PARSER_CSVPARSER_H
#define FAST_CSV_PARSER_CSVPARSER_H

#include <vector>
#include <string>

class Row : public std::vector<const char*> {
public:
  Row(unsigned int fields) : std::vector<const char*>(fields) {}
  Row() : std::vector<const char*>() {};
};

class Rows : public std::vector<Row> {
  friend class CsvParser;
public:
  Rows(unsigned int lines) :
  std::vector<Row>(lines),
  valid_rows_(0) {}
  int valid_rows() {return valid_rows_;}

private:
  int valid_rows_;
};

class CsvParser {
  public:
    CsvParser(const std::string& File_path);
    virtual ~CsvParser();

    bool Init();
    bool Read(Row &row);
    bool Read(Rows &rows);

  private:
    bool ReadLine(unsigned int lines=1);
    bool FreeLine();

    std::string file_path_;

    // File information count
    void *file_;
    int current_line_;

    // Current line information
    std::vector<std::pair<void*,size_t>> lines_;
    int valid_lines_;
};


#endif //FAST_CSV_PARSER_CSVPARSER_H
