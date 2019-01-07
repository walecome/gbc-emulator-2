#include "Processor.hh"

// 8bit
void Processor::setValueA(register8_t value) { A->setValue(value); }
void Processor::setValueB(register8_t value) { B->setValue(value); }
void Processor::setValueC(register8_t value) { C->setValue(value); }
void Processor::setValueD(register8_t value) { D->setValue(value); }
void Processor::setValueE(register8_t value) { E->setValue(value); }
void Processor::setValueF(register8_t value) { F->setValue(value); }
void Processor::setValueH(register8_t value) { H->setValue(value); }
void Processor::setValueL(register8_t value) { L->setValue(value); }

// 16bit
void Processor::setValueAF(register16_t value) { AF->setValue(value); }
void Processor::setValueBC(register16_t value) { BC->setValue(value); }
void Processor::setValueDE(register16_t value) { DE->setValue(value); }
void Processor::setValueHL(register16_t value) { HL->setValue(value); }