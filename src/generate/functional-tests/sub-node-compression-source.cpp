#include "sub-node-compression-source.h"
namespace {
    node const nABILITIES = {
            from_bits(0x1U),
            nullptr};
    node const nABILITIEe[]{nABILITIES};
    node const nABILITIE = {
            from_bits(0x80000U),
            nABILITIEe};
    node const nABILITIe[]{nABILITIE};
    node const nABILITI = {
            from_bits(0x20U),
            nABILITIe};
    node const nABILITe[]{nABILITI, nABILITIES};
    node const nABILIT = {
            from_bits(0x2000200U),
            nABILITe};
    node const nABILIe[]{nABILIT};
    node const nABILI = {
            from_bits(0x100000U),
            nABILIe};
    node const nABILe[]{nABILI};
    node const nABIL = {
            from_bits(0x200U),
            nABILe};
    node const nABIe[]{nABIL};
    node const nABI = {
            from_bits(0x1000U),
            nABIe};
    node const nABL = {
            from_bits(0x20U),
            nABILITe + 1};
    node const nABe[]{nABI, nABL};
    node const nAB = {
            from_bits(0x1200U),
            nABe};
    node const nAe[]{nAB};
    node const nA = {
            from_bits(0x4U),
            nAe};
    node const root_nodee[]{nA};
    node const root_node = {
            from_bits(0x3U),
            root_nodee};
}  // namespace
node const& lexicon{root_node};
