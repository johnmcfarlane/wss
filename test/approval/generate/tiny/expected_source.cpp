#include "expected_source.h"
namespace {
node const nAT = {
  from_bits(0x1U),
  nullptr
};
node const nAe[] {nAT};
node const nA = {
  from_bits(0x100000U),
  nAe
};
node const nOT = {
  from_bits(0x1000U),
  nAe
};
node const nOe[] {nOT};
node const nO = {
  from_bits(0x100000U),
  nOe
};
node const root_nodee[] {nA,nO};
node const root_node = {
  from_bits(0x8003U),
  root_nodee
};
} //namespace
node const& lexicon{root_node};
