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

class CsvParser {
  public:
    CsvParser(const std::string& File_path);
    virtual ~CsvParser();

    bool Init();
    bool Read(Row &row);
    bool Read(Row *rows, unsigned int lines=1);

  private:
    bool ReadLine();
    bool FreeLine();

    std::string file_path_;

    // File information count
    void *file_;
    int current_line_;
    int fields_count_;

    // Current line information
    std::vector<void*> lines_;
    char *line_current_;
};


#endif //FAST_CSV_PARSER_CSVPARSER_H
