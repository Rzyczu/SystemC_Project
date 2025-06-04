#include <systemc.h>

SC_MODULE(NiosIIProcessor) {
    sc_in<bool> clk;          // Sygna³ zegara
    sc_in<bool> reset;        // Sygna³ resetu

    sc_out<int> data_out;     // Dane wyjœciowe
    sc_in<int> data_in;       // Dane wejœciowe
    sc_out<int> address_out;  // Adres wyjœciowy (dodany port)

    int internal_register;    // Rejestr wewnêtrzny procesora (np. akumulator)

    sc_event interrupt_event; // Zdarzenie przerwania

    SC_CTOR(NiosIIProcessor) : internal_register(0) {
        SC_METHOD(process);
        sensitive << clk.pos(); // Wra¿liwoœæ na dodatni brzeg zegara
    }

    void process() {
        if (reset.read()) {
            // Procesor w stanie resetu
            data_out.write(0);
            address_out.write(0);  // Ustawienie adresu na 0 w przypadku resetu
            internal_register = 0; // Resetowanie rejestru
        }
        else {
            // Jeœli wyst¹pi³o przerwanie, obs³ugujemy je
            if (interrupt_event.triggered()) {
                // Obs³uguje przerwanie
                internal_register = data_in.read();
                data_out.write(internal_register);
                address_out.write(0x1); // Przyk³adowy adres
                interrupt_event.cancel(); // Resetowanie przerwania po obs³udze
            }
            else {
                // Normalna logika procesora
                internal_register = data_in.read();
                data_out.write(internal_register);  // Wyjœcie z danych
                address_out.write(0x2); // Inny adres
            }
        }
    }

    void trigger_interrupt() {
        interrupt_event.notify(SC_ZERO_TIME);  // Generowanie przerwania
    }
};
