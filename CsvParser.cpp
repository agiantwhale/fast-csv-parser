//
// Created by 이일재 on 2015. 8. 19..
//

#include "CsvParser.h"
#include <cstdlib>
#include <boost/thread.hpp>

const char * GetNextField(char *& cursor){
  if(*cursor==0) return nullptr;

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
        (*cursor_itr==0) )) {
      if(cursor_field_end==(cursor_itr+1)){
        cursor_field_end=cursor_itr;
      }
      break;
    }
    cursor_itr++;
  }
  if( (*cursor_itr!=0) ||
      (*cursor_itr!='\n') ) {
    cursor_itr++;
  }
  *cursor_field_end=0;
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
  lines_(),
  valid_lines_(0) {
}
CsvParser::~CsvParser() {
  FreeLine();
  if(file_!=nullptr)
    fclose(static_cast<FILE*>(file_));
}
bool CsvParser::ReadLine(unsigned int lines) {
  // FreeLine();
  // char * line=(char *)malloc(last_line_size_);
  // lines_.reserve(lines);
  int needed_alloc=lines-lines_.size();
  for(int i=0;i<needed_alloc;i++) {
    size_t size(0);
    if(lines_.size()>0) size = lines_.back().second;
    void * mem = malloc(size);
    auto line=std::make_pair(mem,size);
    lines_.push_back(line);
  }

  valid_lines_=0;

  for(auto line_iter=lines_.begin();
      line_iter!=lines_.end();
      line_iter++){
    auto &line=*line_iter;
    if(getline((char**)&line.first,&line.second,static_cast<FILE*>(file_))!=-1){
      valid_lines_++;
      current_line_++;
    } else return false;
  }
  return true;
}
bool CsvParser::FreeLine() {
  for(auto line_iter=lines_.begin();
      line_iter!=lines_.end();
      line_iter++) {
    auto &line=*line_iter;
    free(line.first);
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
    char *line=static_cast<char*>(lines_[0].first);
    PopulateRow(row,line);
    return true;
  }
  return false;
}
bool CsvParser::Read(Rows &rows) {
  if(file_==NULL) return false;

  int lines = rows.capacity();
  bool read_result=ReadLine(lines);
  rows.valid_rows_=valid_lines_;
  if(!read_result &&
      valid_lines_==0) return false;

  boost::thread_group field_searches;
  for(int i=0;i<valid_lines_;i++){
    field_searches.add_thread(
        new boost::thread(
          PopulateRow,rows[i],
          static_cast<char*>(lines_[i].first)
          )
        );
  }
  field_searches.join_all();

  return true;
}
