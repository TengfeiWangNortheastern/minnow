#include "byte_stream.hh"

#include <algorithm>
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void Writer::push( const string& data )
{
  // Your code here.
  if(is_closed() || data.empty()){
    return;
  }
  const uint64_t can_push = available_capacity();
  const uint64_t actual_push = min(can_push, static_cast<uint64_t>(data.size()));
  if(actual_push > 0){
    string content = data.substr(0, actual_push);
    buffer_.emplace_back(move(content));
    bytes_pushed_ += actual_push;
    bytes_buffered_ += actual_push;
  }
}

void Writer::close()
{
  // Your code here.
  closed_ = true;
}

void Writer::set_error()
{
  // Your code here.
  error_ = true;
}

bool Writer::is_closed() const
{
  // Your code here.
  return closed_;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - bytes_buffered_;
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return bytes_pushed_;
}

string_view Reader::peek() const
{
  // Your code here.
  if(buffer_.empty()){
    return {};
  }
  return buffer_.front();
}

bool Reader::is_finished() const
{
  // Your code here.
  return closed_ && (bytes_buffered_ == 0);
}

bool Reader::has_error() const
{
  // Your code here.
  return error_;
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  const uint64_t bytes_to_pop = min(len, bytes_buffered_);
  uint64_t remaining = bytes_to_pop;
  while(remaining>0 && !buffer_.empty()){
    string& front_str = buffer_.front();
    if(front_str.size()<=remaining){
      remaining -= front_str.size();
      buffer_.pop_front();
    }else{
      front_str.erase(0, remaining);
      remaining = 0;
    }
  }
  bytes_popped_ +=bytes_to_pop;
  bytes_buffered_ -= bytes_to_pop;
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return bytes_buffered_;
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return bytes_popped_;
}