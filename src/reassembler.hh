#pragma once
#include <map>
#include <string>
#include <cstdint> // 用于 uint64_t
#include "byte_stream.hh"

class Reassembler
{
public:
  /*
   * Insert a new substring to be reassembled into a ByteStream.
   *   `first_index`: the index of the first byte of the substring
   *   `data`: the substring itself
   *   `is_last_substring`: this substring represents the end of the stream
   *   `output`: a mutable reference to the Writer
   *
   * The Reassembler's job is to reassemble the indexed substrings (possibly out-of-order
   * and possibly overlapping) back into the original ByteStream. As soon as the Reassembler
   * learns the next byte in the stream, it should write it to the output.
   *
   * If the Reassembler learns about bytes that fit within the stream's available capacity
   * but can't yet be written (because earlier bytes remain unknown), it should store them
   * internally until the gaps are filled in.
   *
   * The Reassembler should discard any bytes that lie beyond the stream's available capacity
   * (i.e., bytes that couldn't be written even if earlier gaps get filled in).
   *
   * The Reassembler should close the stream after writing the last byte.
   */
  void insert( uint64_t first_index, std::string data, bool is_last_substring, Writer& output );
  std::map<uint64_t, std::string> _buffer {};
  uint64_t _bytes_pending = 0;           // cached bytes
  // How many bytes are stored in the Reassembler itself?
  bool _is_last_received = false; // if received the end sign
  uint64_t _last_index = 0;       // 结束字节的绝对位置
  uint64_t bytes_pending() const;
};
