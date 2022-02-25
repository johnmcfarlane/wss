#include "expected_source.h"
namespace {
node const nCARS = {
  from_bits(0x1U),
  nullptr
};
node const nCARe[] {nCARS};
node const nCAR = {
  from_bits(0x80001U),
  nCARe
};
node const nCAe[] {nCAR,nCAR};
node const nCA = {
  from_bits(0x140000U),
  nCAe
};
node const nCe[] {nCA};
node const nC = {
  from_bits(0x2U),
  nCe
};
node const nDON = {
  from_bits(0x20U),
  nCARe
};
node const nDOe[] {nCAR,nDON};
node const nDO = {
  from_bits(0x4081U),
  nDOe
};
node const nDe[] {nDO};
node const nD = {
  from_bits(0x8000U),
  nDe
};
node const root_nodee[] {nC,nD,nC};
node const root_node = {
  from_bits(0x39U),
  root_nodee
};
} //namespace
node const& lexicon{root_node};
