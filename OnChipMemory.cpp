#include <systemc.h>

SC_MODULE(OnChipMemory) {
    sc_in<bool> clk;
    sc_in<bool> reset;

    sc_out<int> address;   // Adres
    sc_in<int> data_in;   // Dane wejœciowe
    sc_out<int> data_out; // Dane wyjœciowe

    static const int MEM_SIZE = 256; // Define memory size
    int memory[MEM_SIZE];            // Internal memory storage

    SC_CTOR(OnChipMemory) {
        // Inicjalizacja pamiêci na 0
        for (int i = 0; i < MEM_SIZE; ++i) {
            memory[i] = 0;  // Inicjalizacja pamiêci przy tworzeniu
        }

        SC_METHOD(process);
        sensitive << clk.pos();  // Reagowanie na dodatni brzeg zegara
    }

    void process() {
        if (reset.read()) {
            data_out.write(0);  // Resetowanie wyjœcia
            for (int i = 0; i < MEM_SIZE; ++i) {
                memory[i] = 0;  // Resetowanie zawartoœci pamiêci
            }
        }
        else {
            int addr = address.read();  // Odczyt adresu
            if (addr >= 0 && addr < MEM_SIZE) {  // Sprawdzanie poprawnoœci adresu
                // Operacja odczytu i zapisu
                data_out.write(memory[addr]);  // Odczyt danych z pamiêci
                memory[addr] = data_in.read(); // Zapis danych do pamiêci
            }
            else {
                // Zg³aszanie b³êdu, gdy adres jest poza zakresem
                SC_REPORT_ERROR("OnChipMemory", "Address out of bounds!");
            }
        }
    }
};
