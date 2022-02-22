#include "expected_source.h"
namespace {
node const nLESS = {
  from_bits(0x1U),
  nullptr
};
node const nLESe[] {nLESS};
node const nLES = {
  from_bits(0x80000U),
  nLESe
};
node const nLEe[] {nLES};
node const nLE = {
  from_bits(0x80000U),
  nLEe
};
node const nLe[] {nLE};
node const nL = {
  from_bits(0x20U),
  nLe
};
node const nSPA = {
  from_bits(0x100000U),
  nLESe
};
node const nSPOTLESSNESS = {
  from_bits(0x20U),
  nLEe
};
node const nSPOTLESSNESe[] {nSPOTLESSNESS};
node const nSPOTLESSNES = {
  from_bits(0x80000U),
  nSPOTLESSNESe
};
node const nSPOTLESSNEe[] {nSPOTLESSNES};
node const nSPOTLESSNE = {
  from_bits(0x80000U),
  nSPOTLESSNEe
};
node const nSPOTLESSNe[] {nSPOTLESSNE};
node const nSPOTLESSN = {
  from_bits(0x20U),
  nSPOTLESSNe
};
node const nSPOTLESSe[] {nSPOTLESSN};
node const nSPOTLESS = {
  from_bits(0x4000U),
  nSPOTLESSe
};
node const nSPOTLESe[] {nSPOTLESS};
node const nSPOTLES = {
  from_bits(0x80000U),
  nSPOTLESe
};
node const nSPOTLEe[] {nSPOTLES};
node const nSPOTLE = {
  from_bits(0x80000U),
  nSPOTLEe
};
node const nSPOTLe[] {nSPOTLE};
node const nSPOTL = {
  from_bits(0x20U),
  nSPOTLe
};
node const nSPOTe[] {nSPOTL};
node const nSPOT = {
  from_bits(0x1000U),
  nSPOTe
};
node const nSPOe[] {nSPOT};
node const nSPO = {
  from_bits(0x100000U),
  nSPOe
};
node const nSPe[] {nSPA,nSPO};
node const nSP = {
  from_bits(0x8002U),
  nSPe
};
node const nSe[] {nSP};
node const nS = {
  from_bits(0x10000U),
  nSe
};
node const root_nodee[] {nL,nS};
node const root_node = {
  from_bits(0x81001U),
  root_nodee
};
} //namespace
node const& lexicon{root_node};
