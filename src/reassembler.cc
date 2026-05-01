#include "reassembler.hh"

using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring, Writer& output )
{
  // Your code here.
  uint64_t first = first_index;
  uint64_t next_byte_index = output.bytes_pushed();
  if(is_last_substring){ // datagram may come at random order, need to make sure it is the last
    starting_sequence_number.emplace(first_index,data);
    last_byte_index=first_index+data.size();
  }

  

  if (first_index > next_byte_index){
    starting_sequence_number.emplace(first_index,data);
  } else if(first_index == next_byte_index){
    output.push(data);
    while()
  }else{ //? smaller than next_byte_index?
    return;
  }
}

uint64_t Reassembler::bytes_pending() const
{
  // Your code here.
  return {};
}
