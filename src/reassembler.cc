#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  // Your code here.
  // 1 基本限制检查
  uint64_t first_unread = output.reader().bytes_popped();
  uint64_t capacity = output.available_capacity() + output.writer().bytes_pushed() - first_unread;
  uint64_t first_unacceptable = first_unread + capacity;

  // 记录终点
  if ( is_last_substring ) {
    _is_last_received = true;
    _last_index = first_index + data.size();
  }

  // 2 裁剪数据：去掉已读的和超出容量的部分
  uint64_t upper_bound = min( first_index + data.size(), first_unacceptable );
  uint64_t lower_bound = max( first_index, output.writer().bytes_pushed() );

  if ( lower_bound >= upper_bound ) {
    // 处理空包的情况； 如果是最后一个包且为空，虽然没数据也要尝试关闭
    if ( _is_last_received && output.writer().bytes_pushed() >= _last_index ) {
      output.close();
    }
    return;
  }

  string new_data = data.substr( lower_bound - first_index, upper_bound - lower_bound );
  uint64_t new_index = lower_bound;

  // 3. 核心：在Map中合并重叠片段... not clear
  auto it = _buffer.lower_bound( new_index );
  if ( it != _buffer.begin() ) {
    auto prev_it = prev( it );
    if ( prev_it->first + prev_it->second.size() >= new_index ) {
      // 与前一个片段重合，合并到前一个
      if ( prev_it->first + prev_it->second.size() >= new_index + new_data.size() ) {
        // new data完全被包含在prev_it 中
        new_data = prev_it->second;
      } else {
        new_data = prev_it->second + new_data.substr( prev_it->first + prev_it->second.size() - new_index );
      }
      new_index = prev_it->first;
      _bytes_pending -= prev_it->second.size();
      _buffer.erase( prev_it );
    }
  }
  // 查找并合并右边所有重叠的片段
  it = _buffer.lower_bound( new_index ); //? red black tree?
  while ( it != _buffer.end() && new_index + new_data.size() >= it->first ) {
    if ( new_index + new_data.size() < it->first + it->second.size() ) {
      // 最后一个受影响的片段只是部分重叠
      new_data += it->second.size() > ( new_index + new_data.size() - it->first )
                    ? it->second.substr( new_index + new_data.size() - it->first )
                    : "";
    }
    _bytes_pending -= it->second.size();
    it = _buffer.erase( it );
  }
  // 4. 插入新合并的片段
  _buffer[new_index] = move( new_data );
  _bytes_pending += _buffer[new_index].size();

  // 5. 尝试推送到 ByteStream
  while ( !_buffer.empty() && _buffer.begin()->first == output.writer().bytes_pushed() ) {
    auto it_first = _buffer.begin();
    _bytes_pending -= it_first->second.size();
    output.push( move( it_first->second ) );
    _buffer.erase( it_first );
  }

  if ( _is_last_received && output.writer().bytes_pushed() >= _last_index ) {
    output.close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return _bytes_pending;
}
