//
// Created by 이일재 on 2015. 8. 19..
//

#include "CsvParser.h"
#include <cstdlib>
#include <boost/thread.hpp>

const char * GetNextField(char *& cursor){
  if(*cursor=='\0') return nullptr;

  char *cursor_itr=cursor;
  char *cursor_field_start=nullptr;
  char *cursor_field_end=nullptr;
  int quote_count=0;
  while(true){
    if((*cursor_itr!=' ') &&
       (*cursor_itr!='\t')) {
      if(cursor_field_start==nullptr)
        cursor_field_start=cursor_itr;
      cursor_field_end=(cursor_itr+1);
    }
    if( *cursor_itr=='"' ) quote_count++;
    if( (quote_count%2==0) && (
        (*cursor_itr==',')  ||
        (*cursor_itr=='\n') ||
        (*cursor_itr=='\0') ||
        (*cursor_itr=='\r'))) {
      if(cursor_field_end==(cursor_itr+1)){
        cursor_field_end=cursor_itr;
      }
      break;
    }
    cursor_itr++;
  }
  if(*cursor_itr!='\0') {
    *cursor_field_end='\0';
    cursor_itr++;
  }
  cursor=cursor_itr;
  return cursor_field_start;
}
void PopulateRow(Row &row,char *start){
  row.clear();
  const char *field;
  while((field=GetNextField(start))!= nullptr) {
    row.push_back(field);
  }
}

CsvParser::CsvParser(const std::string &file_path) :
  file_path_(file_path),
  file_(nullptr),
  current_line_(0),
  fields_count_(0) {
}
CsvParser::~CsvParser() {
  FreeLine();
  fclose(static_cast<FILE*>(file_));
}
bool CsvParser::ReadLine() {
  size_t size(0);
  char * line;
  if(getline(&line,&size,static_cast<FILE*>(file_))){
    lines_.push_back(static_cast<void*>(line));
    current_line_++;
    return true;
  }
  return false;
}
bool CsvParser::FreeLine() {
  for(auto line_iter=lines_.begin();
      line_iter!=lines_.end();
      line_iter++) {
    free(*line_iter);
  }
  lines_.clear();

  return true;
}
bool CsvParser::Init() {
  file_=fopen(file_path_.c_str(),"r");
  return (file_!=NULL);
}
bool CsvParser::Read(Row &row) {
  if(ReadLine()){
    char *line=static_cast<char*>(lines_[0]);
    PopulateRow(row,line);
    return true;
  }
  return false;
}
bool CsvParser::Read(Row *rows, unsigned int lines) {
  if(file_==NULL) return false;

  lines_.reserve(lines);
  FreeLine();

  for(int i=0;i<(int)lines;i++){
    if(!ReadLine()) { // EOF
      lines=lines_.size();
      break;
    }
  }

  if(lines==0) return false;

  boost::thread_group field_searches;
  for(int i=0;i<(int)lines;i++){
    field_searches.add_thread(new boost::thread(PopulateRow,rows[i],static_cast<char*>(lines_[i])));
  }
  field_searches.join_all();

  return true;
}
