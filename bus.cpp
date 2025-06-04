#include <systemc.h>

SC_MODULE(Bus) {
    sc_in<bool> clk;           // Sygna� zegara
    sc_in<bool> reset;         // Sygna� resetu

    sc_out<int> data;          // Magistrala danych (jako wyj�cie)
    sc_out<int> address;       // Magistrala adresowa
    sc_out<bool> read_write;   // Operacja odczytu/zapisu
    sc_out<bool> chip_select;  // Selekcja uk�adu

    SC_CTOR(Bus) {
        SC_METHOD(process);
        sensitive << clk.pos(); // Zegar dodatni brzeg
    }

    void process() {
        if (reset.read()) {
            // Resetowanie magistrali
            address.write(0);
            data.write(0);           // Zak�adaj�c, �e int jest odpowiedni
            read_write.write(false);
            chip_select.write(false);
        }
        else {
            // Przyk�adowa logika magistrali
            address.write(0x1);      // Przyk�adowy adres
            data.write(0xFF);        // Przyk�adowe dane
            read_write.write(true);  // Operacja zapisu
            chip_select.write(true); // Aktywacja selekcji uk�adu
        }
    }
};
